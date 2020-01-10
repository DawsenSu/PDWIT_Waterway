#include "GuidewallP2Tool.h"

bool PDIWT::Waterway::Lock::GuidewallP2Tool::_OnInstall()
{
	return __super::_OnInstall();
}

void PDIWT::Waterway::Lock::GuidewallP2Tool::_OnPostInstall()
{
	//Attach new window to tool
	//m_form = gcnew UI::GuidewallP2WinForm();
	//m_form->AttachToToolSettings(Program::Addin);
	m_wpfform = gcnew BMWPF::ToolSettingsHost();
	m_wpfform->Width = 400;
	m_wpfform->Height = 250;
	m_wpfform->Title = L"Guide wall P2 Create Tool";
	//The following doesn't work, it can't let the tool window show what is set here.
	//m_wpfform->Icon = gcnew BitmapImage(gcnew Uri(L"pack://application:,,,/PDIWT_Waterway_Lock.UI;component/Images/GuidewallP2.ico", UriKind::RelativeOrAbsolute));
	m_wpfform->Content = gcnew UI::GuidewallP2View();
	m_wpfform->Attach(Program::Addin);

	AccuSnap::GetInstance().EnableSnap(true);
	AccuSnap::GetInstance().EnableLocate(true);
	AccuDraw::GetInstance().SetContext(ACCUDRAW_SetModePolar);
	
	//DVec3d ux = DVec3d::UnitX();
	//AccuDraw::GetInstance().SetContext(ACCUDRAW_SetXAxis, NULL, &ux, NULL, NULL, NULL, NULL);
	DVec3d uz = DVec3d::UnitZ();
	AccuDraw::GetInstance().SetContext(ACCUDRAW_SetNormal, NULL, &uz, NULL, NULL, NULL, NULL);
	SetupForNextAction();
	__super::_OnPostInstall();
}

void PDIWT::Waterway::Lock::GuidewallP2Tool::_OnRestartTool()
{
	InstallNewInstance();
}

bool PDIWT::Waterway::Lock::GuidewallP2Tool::_OnResetButton(DgnButtonEventCR ev)
{
	CallOnRestartTool();
	return false;
}

bool PDIWT::Waterway::Lock::GuidewallP2Tool::_OnDataButton(DgnButtonEventCR ev)
{
	SetupForNextAction();
	if (m_pts.empty())
	{
		m_pts.push_back(*ev.GetPoint());
		_BeginDynamics();
		return false;
	}

	m_pts.push_back(*ev.GetPoint());

	GuidewallP2 guidewallp2(m_data);
	EditElementHandle guidewallEeh;
	if (SUCCESS != guidewallp2.CreateGuidewallP2(guidewallEeh, *ACTIVEMODEL))
	{		
		mdlOutput_messageCenter(OutputMessagePriority::Error, L"Can't create guidewall", L"", OutputMessageAlert::Balloon);
		return false;
	}
	DVec3d rotVec = m_pts.at(1) - m_pts.at(0);
	rotVec.z = 0;
	if (rotVec.IsZero())
		rotVec = DVec3d::UnitX();
	RotMatrix rotM = RotMatrix::From1Vector(rotVec, 0, true);

	Transform trans = Transform::From(rotM, m_pts.at(0));
	if (SUCCESS != guidewallEeh.GetHandler().ApplyTransform(guidewallEeh, TransformInfo(trans)))
	{
		mdlOutput_messageCenter(OutputMessagePriority::Error, L"Can't move guidewall to click point", L"", OutputMessageAlert::Balloon);
		return false;
	}
	guidewallEeh.AddToModel();
	CallOnRestartTool();
	return false;
}

void PDIWT::Waterway::Lock::GuidewallP2Tool::_OnDynamicFrame(DgnButtonEventCR ev)
{
	if (m_pts.empty())
		return;

	GuidewallP2 guidewallp2(m_data);
	EditElementHandle guidewallEeh;
	if (SUCCESS != guidewallp2.CreateGuidewallP2(guidewallEeh, *ACTIVEMODEL))
		return;
	DVec3d rotVec = *ev.GetPoint() - m_pts.at(0);
	rotVec.z = 0;

	if (rotVec.IsZero())
		rotVec = DVec3d::UnitX();
	RotMatrix rotM = RotMatrix::From1Vector(rotVec, 0, true);
	Transform trans = Transform::From(rotM, m_pts.at(0));
	if (SUCCESS != guidewallEeh.GetHandler().ApplyTransform(guidewallEeh, TransformInfo(trans)))
		return;

	RedrawElems redrawElems;
	redrawElems.SetDynamicsViews(IViewManager::GetActiveViewSet(), ev.GetViewport());
	redrawElems.SetDrawMode(DRAW_MODE_TempDraw);
	redrawElems.SetDrawPurpose(DrawPurpose::Dynamics);
	redrawElems.DoRedraw(guidewallEeh);
}

void PDIWT::Waterway::Lock::GuidewallP2Tool::_OnCleanup()
{
	//m_form->DetachFromMicroStation();
	//m_form->Close();
	//delete m_form;
	m_wpfform->Detach();
	delete m_wpfform;
	m_pts.clear();
	__super::_OnCleanup();
}

void PDIWT::Waterway::Lock::GuidewallP2Tool::SetupForNextAction()
{
	mdlOutput_command(L"GuideWallP2");
	if (m_pts.empty())
	{
		mdlOutput_prompt(L"Pick insert point");
	}
	else
	{
		mdlOutput_prompt(L"Rotate");
	}
}


void PDIWT::Waterway::Lock::GuidewallP2Tool::InstallNewInstance()
{
	GuidewallP2Tool* tool = new GuidewallP2Tool();
	tool->InstallTool();
}
