#include "GuidewallP2Tool.h"

bool PDIWT::Waterway::Lock::GuidewallP2Tool::_OnInstall()
{
	return __super::_OnInstall();
}

void PDIWT::Waterway::Lock::GuidewallP2Tool::_OnPostInstall()
{
	//Attach new window to tool
	m_form = gcnew UI::GuidewallP2WinForm();
	m_form->AttachToToolSettings(Program::Addin);
	//m_wpfform = gcnew BMWPF::ToolSettingsHost();
	//m_wpfform->Width = 200;
	//m_wpfform->Height = 200;
	//m_wpfform->Title = L"Guide wall P2 Create Tool";
	//m_wpfform->Content = gcnew UI::GuidewallP2View();
	//m_wpfform->Attach(Program::Addin);
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
	//EditElementHandle dolphinP2Eeh;
	//DolphinColumnP2 dolphinP2 = DolphinColumnP2(0.8, 0.8, 1, 1, 1, 0.2, 0.2);
	//dolphinP2.CrerateDolphin(dolphinP2Eeh, *ACTIVEMODEL);
	//dolphinP2Eeh.AddToModel();

	//EditElementHandle cushioncapEeh;
	//Cushioncap cushioncap = Cushioncap(1,1.2,1,1);
	//cushioncap.CreateCushioncap(cushioncapEeh, *ACTIVEMODEL);
	//cushioncapEeh.AddToModel();

	//EditElementHandle pileEeh;
	//Pile pile = Pile(1,0.5,0.1);
	//pile.CreatePile(pileEeh, *ACTIVEMODEL);
	//pileEeh.AddToModel();

	//EditElementHandle cushionEeh;
	//Cushion cushion = Cushion(1, 1, 0.2);
	//cushion.CreateCushion(cushionEeh, *ACTIVEMODEL);
	//cushionEeh.AddToModel();

	//EditElementHandle wallEeh;
	//Wall wall = Wall(1, 1, 0.2);
	//wall.CreateWall(wallEeh, *ACTIVEMODEL);
	//wallEeh.AddToModel();

	GuideWallP2Data data;
	data.SetdolphinTopElevation(10);
	data.SetdolphinBottomElevation(0);
	data.SetdolphinTopLength(1.5);
	data.SetdolphinTopWidth(1.8);
	data.SetdolphinBottomLength(1.5);
	data.SetdolphinBottomWidth(3.6);

	data.SetcushioncapBottomElevation(-2.5);
	data.SetcushioncapFrontToeLength(1);
	data.SetcushioncapRearToeLength(1);
	data.SetcushioncapExtraSideLength(0.25);
	data.SetcushioncapChamferLength(0.3);

	
	data.SetcushionThickness(0.1);

	data.SetpileTopElevation(0.1);
	data.SetpileBottomElevation(-10);
	data.SetpileDiameter(1);
	data.SetpilewallThickness(0.2);
	data.SetpileOffsetPerpendicularRiver(1.5);

	data.SetwallTopElevation(10.1);
	data.SetwallBottomElevation(3);
	data.SetwallThickness(0.5);
	data.SetwallLength(10);
	data.SethasLeftWall(true);
	data.SethasRightWall(true);


	GuidewallP2 guidewallp2(data);
	EditElementHandle guidewallEeh;
	if (SUCCESS != guidewallp2.CreateGuidewallP2(guidewallEeh, *ACTIVEMODEL))
	{		
		mdlOutput_messageCenter(OutputMessagePriority::Error, L"Can't create dolphin", L"", OutputMessageAlert::Balloon);
		return false;
	}
	guidewallEeh.AddToModel();
	return false;
}

void PDIWT::Waterway::Lock::GuidewallP2Tool::_OnDynamicFrame(DgnButtonEventCR ev)
{

}

void PDIWT::Waterway::Lock::GuidewallP2Tool::_OnCleanup()
{
	m_form->DetachFromMicroStation();
	m_form->Close();
	delete m_form;
	//m_wpfform->Detach();
	//delete m_wpfform;
	__super::_OnCleanup();
}

void PDIWT::Waterway::Lock::GuidewallP2Tool::InstallNewInstance()
{
	GuidewallP2Tool* tool = new GuidewallP2Tool();
	tool->InstallTool();
}
