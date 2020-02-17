
#include "MountPierP1PlacementTool.h"

bool PDIWT::Waterway::Lock::MountPierP1PlacementTool::_OnInstall()
{
	return __super::_OnInstall();
}

void PDIWT::Waterway::Lock::MountPierP1PlacementTool::_OnPostInstall()
{
	//Attach new window to tool
	//m_form = gcnew UI::GuidewallP2WinForm();
	//m_form->AttachToToolSettings(Program::Addin);
	m_wpfform = gcnew BMWPF::ToolSettingsHost();
	//m_wpfform->Width = 300;
	//m_wpfform->Height = 480;
	m_wpfform->Title = L"Dolphin P11 Create Tool";
	//The following doesn't work, it can't let the tool window show what is set here.
	//m_wpfform->Icon = gcnew BitmapImage(gcnew Uri(L"pack://application:,,,/PDIWT_Waterway_Lock.UI;component/Images/GuidewallP2.ico", UriKind::RelativeOrAbsolute));
	m_wpfform->Content = gcnew UI::MountPierP1View(Program::Addin);
	m_wpfform->Attach(Program::Addin);

	AccuSnap::GetInstance().EnableSnap(true);
	AccuSnap::GetInstance().EnableLocate(true);
	AccuDraw::GetInstance().SetContext(ACCUDRAW_SetModePolar);

	DVec3d uz = DVec3d::UnitZ();
	AccuDraw::GetInstance().SetContext(ACCUDRAW_SetNormal, NULL, &uz, NULL, NULL, NULL, NULL);
	SetupForNextAction();
	__super::_OnPostInstall();
}

void PDIWT::Waterway::Lock::MountPierP1PlacementTool::_OnRestartTool()
{
	InstallNewInstance();
}

bool PDIWT::Waterway::Lock::MountPierP1PlacementTool::_OnResetButton(DgnButtonEventCR ev)
{
	CallOnRestartTool();
	return false;
}

bool PDIWT::Waterway::Lock::MountPierP1PlacementTool::_OnDataButton(DgnButtonEventCR ev)
{
	SetupForNextAction();
	if (m_pts.empty())
	{
		// Set the data for the first click
		SetData();

		DPoint3d pt = *ev.GetPoint();
		pt.z = m_data.DolphinColumnBottomElevation * ACTIVEMODEL->GetModelInfoCP()->GetUorPerMeter();
		m_pts.push_back(pt);

		_BeginDynamics();
		return false;
	}

	m_pts.push_back(*ev.GetPoint());

	MountPierP1 dolphinp11(m_data);
	EditElementHandle dolphinp11Eeh;
	if (SUCCESS != dolphinp11.CreateMountPier(dolphinp11Eeh, *ACTIVEMODEL))
	{
		mdlOutput_messageCenter(OutputMessagePriority::Error, L"Can't create dolphinP11", L"", OutputMessageAlert::Balloon);
		return false;
	}
	DVec3d rotVec = m_pts.at(1) - m_pts.at(0);
	rotVec.z = 0;
	if (rotVec.IsZero())
		rotVec = DVec3d::UnitX();
	RotMatrix rotM = RotMatrix::From1Vector(rotVec, 0, true);

	Transform trans = Transform::From(rotM, m_pts.at(0));
	if (SUCCESS != dolphinp11Eeh.GetHandler().ApplyTransform(dolphinp11Eeh, TransformInfo(trans)))
	{
		mdlOutput_messageCenter(OutputMessagePriority::Error, L"Can't move dolphinP11 to click point", L"", OutputMessageAlert::Balloon);
		return false;
	}
	dolphinp11Eeh.AddToModel();
	CallOnRestartTool();
	return false;
}

void PDIWT::Waterway::Lock::MountPierP1PlacementTool::_OnDynamicFrame(DgnButtonEventCR ev)
{
	if (m_pts.empty())
		return;
	SetupForNextAction();
	MountPierP1 dolphinp11(m_data);
	EditElementHandle dolphinp11Eeh;
	if (SUCCESS != dolphinp11.CreateMountPier(dolphinp11Eeh, *ACTIVEMODEL))
		return;
	DVec3d rotVec = *ev.GetPoint() - m_pts.at(0);
	rotVec.z = 0;

	if (rotVec.IsZero())
		rotVec = DVec3d::UnitX();
	RotMatrix rotM = RotMatrix::From1Vector(rotVec, 0, true);
	Transform trans = Transform::From(rotM, m_pts.at(0));
	if (SUCCESS != dolphinp11Eeh.GetHandler().ApplyTransform(dolphinp11Eeh, TransformInfo(trans)))
		return;

	RedrawElems redrawElems;
	redrawElems.SetDynamicsViews(IViewManager::GetActiveViewSet(), ev.GetViewport());
	redrawElems.SetDrawMode(DRAW_MODE_TempDraw);
	redrawElems.SetDrawPurpose(DrawPurpose::Dynamics);
	redrawElems.DoRedraw(dolphinp11Eeh);
}

void PDIWT::Waterway::Lock::MountPierP1PlacementTool::_OnCleanup()
{
	////m_form->DetachFromMicroStation();
	////m_form->Close();
	////delete m_form;
	m_wpfform->Detach();
	delete m_wpfform;
	m_pts.clear();
	__super::_OnCleanup();
}

void PDIWT::Waterway::Lock::MountPierP1PlacementTool::SetupForNextAction()
{
	mdlOutput_command(L"MountPierP1");
	if (m_pts.empty())
	{
		mdlOutput_prompt(L"Pick insert point");
	}
	else
	{
		mdlOutput_prompt(L"Rotate");
	}
}


void PDIWT::Waterway::Lock::MountPierP1PlacementTool::InstallNewInstance()
{
	MountPierP1PlacementTool* tool = new MountPierP1PlacementTool();
	tool->InstallTool();
}

void PDIWT::Waterway::Lock::MountPierP1PlacementTool::SetData()
{
	UI::MountPierP1View^ view = dynamic_cast<UI::MountPierP1View^>(m_wpfform->Content);

	UI::ViewModel::MountPierP1Data^ data = view->GetData();
	m_data.DolphinColumnTopElevation=data->DolphinColumnTopElevation;
	m_data.DolphinColumnBottomElevation=data->DolphinColumnBottomElevation;
	m_data.DolphinColumnTopLength=data->DolphinColumnTopLength;
	m_data.DolphinColumnTopWidth=data->DolphinColumnTopWidth;
	m_data.DolphinColumnTopEdgeRadius=data->DolphinColumnTopEdgeRadius;
	m_data.DolphinColumnSideEdgeRadius=data->DolphinColumnSideEdgeRadius;
	m_data.DolphinColumnBottomLength=data->DolphinColumnBottomLength;
	m_data.DolphinColumnBottomWidth=data->DolphinColumnBottomWidth;
	m_data.DolphinColumnHaunchHeight=data->DolphinColumnHaunchHeight;
	m_data.DolphinColumnHaunchLength=data->DolphinColumnHaunchLength;
	m_data.AngleOfFirstPolylineWall=data->AngleOfFirstPolylineWall;
	m_data.HeightOfFirstPolylineWall=data->HeightOfFirstPolylineWall;

	m_data.CushionCapBottomElevation=data->CushionCapBottomElevation;
	m_data.FrontToeLength=data->FrontToeLength;
	m_data.BackToeLength=data->BackToeLength;
	m_data.CushionCapExtraWideLength=data->CushionCapExtraWideLength;

	m_data.CapThickness=data->CapThickness / 1000; //UI Unit: mm

	m_data.PileDiameterOut = data->PileDiameterOut;
	m_data.PileBottomElevation = data->PileBottomElevation;
	m_data.PileTopElevation = data->PileTopElevation;
	m_data.PileSideDistH = data->PileSideDistH;
	m_data.PileSideDistV = data->PileSideDistV;
	m_data.PileWallThick = data->PileWallThick/1000;
}