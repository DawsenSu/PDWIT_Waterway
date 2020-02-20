// MDL_Unit_Test.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "TestCmds.h"

extern "C" DLLEXPORT void MdlMain(int argc, WCharCP argv[])
{
	mdlDialog_dmsgsPrint(L"MDL_Unit_Test Startup");
	
	TestCmds::IniCmds();

	mdlDialog_dmsgsPrint(L"MDL_Unit_Test End");
}