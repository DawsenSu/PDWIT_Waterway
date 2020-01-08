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
