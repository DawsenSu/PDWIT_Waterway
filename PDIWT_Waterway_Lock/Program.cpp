#include "Program.h"

void PDIWT::Waterway::Lock::KeyinCommands::CreateDolphinP1(System::String^ unparsed)
{
	mdlOutput_message(L"CreateDolphinP1");
}

void PDIWT::Waterway::Lock::KeyinCommands::CreateDolphinP11(System::String^ unparsed)
{
	mdlOutput_message(L"CreateDolphinP11");
}

void PDIWT::Waterway::Lock::KeyinCommands::CreateGuidewallP2(System::String^ unparsed)
{
	GuidewallP2Tool::InstallNewInstance();
	//GuidewallPlacementTool::InstallNewInstance();
}

void PDIWT::Waterway::Lock::KeyinCommands::CreateGuidewallP3(System::String^ unparsed)
{
	mdlOutput_message(L"CreateGuidewallP3");
}