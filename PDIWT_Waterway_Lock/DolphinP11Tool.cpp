#include "DolphinP11Tool.h"

bool PDIWT::Waterway::Lock::DolphinP11Tool::_OnInstall()
{
	return __super::_OnInstall();
}

void PDIWT::Waterway::Lock::DolphinP11Tool::_OnPostInstall()
{
	//Attach new window to tool
	//m_form = gcnew UI::GuidewallP2WinForm();
	//m_form->AttachToToolSettings(Program::Addin);
	m_wpfform = gcnew BMWPF::ToolSettingsHost();
	m_wpfform->Width = 300;
	m_wpfform->Height = 480;
	m_wpfform->Title = L"Dolphin P11 Create Tool";
	//The following doesn't work, it can't let the tool window show what is set here.
	//m_wpfform->Icon = gcnew BitmapImage(gcnew Uri(L"pack://application:,,,/PDIWT_Waterway_Lock.UI;component/Images/GuidewallP2.ico", UriKind::RelativeOrAbsolute));
	m_wpfform->Content = gcnew UI::DolphinP11View(Program::Addin);
	m_wpfform->Attach(Program::Addin);

	AccuSnap::GetInstance().EnableSnap(true);
	AccuSnap::GetInstance().EnableLocate(true);
	AccuDraw::GetInstance().SetContext(ACCUDRAW_SetModePolar);

	DVec3d uz = DVec3d::UnitZ();
	AccuDraw::GetInstance().SetContext(ACCUDRAW_SetNormal, NULL, &uz, NULL, NULL, NULL, NULL);
	SetupForNextAction();
	__super::_OnPostInstall();
}

void PDIWT::Waterway::Lock::DolphinP11Tool::_OnRestartTool()
{
	InstallNewInstance();
}

bool PDIWT::Waterway::Lock::DolphinP11Tool::_OnResetButton(DgnButtonEventCR ev)
{
	CallOnRestartTool();
	return false;
}

bool PDIWT::Waterway::Lock::DolphinP11Tool::_OnDataButton(DgnButtonEventCR ev)
{
	SetupForNextAction();
	if (m_pts.empty())
	{
		// Set the data for the first click
		SetData();

		DPoint3d pt = *ev.GetPoint();
		pt.z = m_data.GetDolphinColumnBottomElevation() * ACTIVEMODEL->GetModelInfoCP()->GetUorPerMeter();
		m_pts.push_back(pt);

		_BeginDynamics();
		return false;
	}

	m_pts.push_back(*ev.GetPoint());

	DolphinP11 dolphinp11(m_data);
	EditElementHandle dolphinp11Eeh;
	if (SUCCESS != dolphinp11.CreateDolphinP11(dolphinp11Eeh, *ACTIVEMODEL))
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

void PDIWT::Waterway::Lock::DolphinP11Tool::_OnDynamicFrame(DgnButtonEventCR ev)
{
	if (m_pts.empty())
		return;
	SetupForNextAction();
	DolphinP11 dolphinp11(m_data);
	EditElementHandle dolphinp11Eeh;
	if (SUCCESS != dolphinp11.CreateDolphinP11(dolphinp11Eeh, *ACTIVEMODEL))
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

void PDIWT::Waterway::Lock::DolphinP11Tool::_OnCleanup()
{
	////m_form->DetachFromMicroStation();
	////m_form->Close();
	////delete m_form;
	m_wpfform->Detach();
	delete m_wpfform;
	m_pts.clear();
	__super::_OnCleanup();
}

void PDIWT::Waterway::Lock::DolphinP11Tool::SetupForNextAction()
{
	mdlOutput_command(L"DolphinP11");
	if (m_pts.empty())
	{
		mdlOutput_prompt(L"Pick insert point");
	}
	else
	{
		mdlOutput_prompt(L"Rotate");
	}
}


void PDIWT::Waterway::Lock::DolphinP11Tool::InstallNewInstance()
{
	DolphinP11Tool* tool = new DolphinP11Tool();
	tool->InstallTool();
}

void PDIWT::Waterway::Lock::DolphinP11Tool::SetData()
{
	UI::DolphinP11View^ view = dynamic_cast<UI::DolphinP11View^>(m_wpfform->Content);

	UI::ViewModel::DolphinP11Data^ data = view->GetData();
	m_data.SetDolphinColumnTopElevation(data->DolphinColumnTopElevation);
	m_data.SetDolphinColumnBottomElevation(data->DolphinColumnBottomElevation);
	m_data.SetDolphinColumnTopLength(data->DolphinColumnTopLength);
	m_data.SetDolphinColumnTopWidth(data->DolphinColumnTopWidth);
	m_data.SetDolphinColumnTopEdgeRadius(data->DolphinColumnTopEdgeRadius);
	m_data.SetDolphinColumnSideEdgeRadius(data->DolphinColumnSideEdgeRadius);
	m_data.SetDolphinColumnBottomLength(data->DolphinColumnBottomLength);
	m_data.SetDolphinColumnBottomWidth(data->DolphinColumnBottomWidth);
	m_data.SetDolphinColumnHaunchHeight(data->DolphinColumnHaunchHeight);
	m_data.SetDolphinColumnHaunchLength(data->DolphinColumnHaunchLength);
	m_data.SetAngleOfFirstPolylineWall(data->AngleOfFirstPolylineWall);
	m_data.SetHeightOfFirstPolylineWall(data->HeightOfFirstPolylineWall);

	m_data.SetCushionCapBottomElevation(data->CushionCapBottomElevation);
	m_data.SetFrontToeLength(data->FrontToeLength);
	m_data.SetBackToeLength(data->BackToeLength);
	m_data.SetCushionCapExtraWideLength(data->CushionCapExtraWideLength);
	
	m_data.SetCapThickness(data->CapThickness / 1000); //UI Unit: mm
}