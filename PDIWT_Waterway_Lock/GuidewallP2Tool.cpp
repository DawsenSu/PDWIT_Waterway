#include "GuidewallP2Tool.h"

bool PDIWT::Waterway::Lock::GuidewallP2Tool::_OnInstall()
{
	return __super::_OnInstall();
}

void PDIWT::Waterway::Lock::GuidewallP2Tool::_OnPostInstall()
{
	//int horizontal = GetSystemMetrics(SM_CXSCREEN);
	//int vertical = GetSystemMetrics(SM_CYSCREEN);
	//mdlDialog_dmsgsPrint(WPrintfString(L"Screen horizontal pixel: %d, Vertical pixel: %d", horizontal, vertical).GetWCharCP());
	//Attach new window to tool
	//m_form = gcnew UI::GuidewallP2WinForm();
	//m_form->AttachToToolSettings(Program::Addin);
	m_wpfform = gcnew BMWPF::ToolSettingsHost();
	m_wpfform->Width = 250 ;
	m_wpfform->Height = 400 ;
	m_wpfform->Title = L"Guide wall P2 Create Tool";
	//The following doesn't work, it can't let the tool window show what is set here.
	//System::Windows::WindowCollection^ window = m_wpfform->OwnedWindows;
	//m_wpfform->Owner->Icon = gcnew BitmapImage(gcnew Uri(L"pack://application:,,,/PDIWT_Waterway_Lock.UI;component/Images/GuidewallP2.ico", UriKind::RelativeOrAbsolute));
	m_wpfform->Content = gcnew UI::GuidewallP2View(Program::Addin);
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
	if (m_pts.empty())
	{
		// Set the data for the first click
		SetData();

		DPoint3d pt = *ev.GetPoint();
		pt.z = m_data->DolphinBottomElevation * ACTIVEMODEL->GetModelInfoCP()->GetUorPerMeter();
		m_pts.push_back(pt);

		SetupForNextAction();
		_BeginDynamics();
		return false;
	}

	m_pts.push_back(*ev.GetPoint());

	GuidewallP2 guidewallp2(m_data);
	EditElementHandle guidewallEeh;
	if (SUCCESS != guidewallp2.CreateGuidewallP2(guidewallEeh, *ACTIVEMODEL))
	{
		WString errorMsg;
		double cushioncapHalfBottomWidth = (m_data->CushioncapRearToeLength + m_data->DolphinBottomWidth + m_data->CushioncapFrontToeLength) / 2;
		if (m_data->DolphinBottomElevation >= m_data->DolphinTopElevation)
			errorMsg += WPrintfString(L"Error -> Dolphin bottom elevation:[%0.2f] >= Dolphin top elevation:[%0.2f]\n", m_data->DolphinBottomElevation, m_data->DolphinTopElevation);
		if (m_data->CushioncapBottomElevation >= m_data->DolphinBottomElevation)
			errorMsg += WPrintfString(L"Error -> Cushioncap bottom elevation:[%0.2f] >=  Dolphin bottom elevation:[%0.2f]\n", m_data->CushioncapBottomElevation, m_data->DolphinBottomElevation);
		if (m_data->WallBottomElevation - GuidewallP2::gapWidth <= m_data->DolphinBottomElevation)
			errorMsg += WPrintfString(L"Error -> Wall bottom elevation:[%0.2f] >=  Dolphin bottom elevation:[%0.2f]\n", m_data->WallBottomElevation - GuidewallP2::gapWidth, m_data->DolphinBottomElevation);
		if (m_data->PileTopElevation >= m_data->DolphinBottomElevation)
			errorMsg += WPrintfString(L"Error -> Pile top elevation:[%0.2f] >=  Pile bottom elevation:[%0.2f]\n", m_data->PileTopElevation, m_data->DolphinBottomElevation);
		if (m_data->PileTopElevation <= m_data->CushioncapBottomElevation)
			errorMsg += WPrintfString(L"Error -> Pile top elevation:[%0.2f] <= Cushioncap bottom elevation:[%0.2f]\n", m_data->PileTopElevation, m_data->CushioncapBottomElevation);
		if (m_data->PileTopElevation <= m_data->PileBottomElevation)
			errorMsg += WPrintfString(L"Error -> Pile top elevation:[%0.2f] <= pile bottom elevation:[%0.2f]\n", m_data->PileTopElevation, m_data->PileBottomElevation);
		if (m_data->PileOffsetPerpendicularRiver <= m_data->PileDiameter / 2)
			errorMsg += WPrintfString(L"Error -> Pile offset perpendicular river:[%0.2f] <= Pile radius:[%0.2f]\n", m_data->PileOffsetPerpendicularRiver, m_data->PileDiameter / 2);
		if (m_data->PileOffsetPerpendicularRiver >= (cushioncapHalfBottomWidth - m_data->PileDiameter / 2))
			errorMsg += WPrintfString(L"Error -> Pile offset perpendicular river:[%0.2f] >= Cushioncap Half bottom width + pile radius:[%0.2f]", m_data->PileOffsetPerpendicularRiver, cushioncapHalfBottomWidth - m_data->PileDiameter / 2);

		mdlOutput_messageCenter(OutputMessagePriority::Error, L"Can't create guidewall, Please check input Parameters", errorMsg.GetWCharCP(), OutputMessageAlert::Balloon);
		m_pts.clear();
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
	m_pts.clear();
	SetupForNextAction();
	return false;
}

void PDIWT::Waterway::Lock::GuidewallP2Tool::_OnDynamicFrame(DgnButtonEventCR ev)
{
	if (m_pts.empty())
		return;

	GuidewallP2 guidewallp2(m_data);
	EditElementHandle guidewallEeh;
	if (SUCCESS != guidewallp2.CreateGuidewallP2(guidewallEeh, *ACTIVEMODEL,true))
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

	//LocateCallback::SetElementDoubleClickFunction([](DisplayPathCP path)->bool
	//{
	//	ElementRefP eP = path->GetHeadElem();
	//	mdlDialog_dmsgsPrint(WPrintfString(L"CallBack in plugin[1], ElementID: %d", eP->GetElementId()));
	//	return true;
	//});
}

void PDIWT::Waterway::Lock::GuidewallP2Tool::SetData()
{
	UI::GuidewallP2View^ view = dynamic_cast<UI::GuidewallP2View^>(m_wpfform->Content);
	UI::Model::GuidewallP2Data^ data = view->GetData();
	m_data = data->CloneGuidewallDataP2InMeters();
	
}