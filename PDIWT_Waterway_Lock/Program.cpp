#include "Program.h"

void PDIWT::Waterway::Lock::KeyinCommands::CreateDolphinP1(System::String^ unparsed)
{
	mdlOutput_message(L"CreateDolphinP1");
	MountPierP1PlacementTool::InstallNewInstance();
}

void PDIWT::Waterway::Lock::KeyinCommands::CreateDolphinP11(System::String^ unparsed)
{
	DolphinP11Tool::InstallNewInstance();
}

void PDIWT::Waterway::Lock::KeyinCommands::CreateGuidewallP2(System::String^ unparsed)
{
	GuidewallP2Tool::InstallNewInstance();
}

void PDIWT::Waterway::Lock::KeyinCommands::CreateGuidewallP3(System::String^ unparsed)
{
	mdlOutput_message(L"CreateGuidewallP3");
}
