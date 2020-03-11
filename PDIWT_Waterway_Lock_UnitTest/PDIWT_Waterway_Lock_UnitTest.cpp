#define PURE_MDL //define this macro to disable clr related head files and namespace

#include "stdafx.h"
#include "PDIWT_Waterway_Lock_UnitTestids.h"
#include "PDIWT_Waterway_Lock_UnitTestcmd.h"
#include "RailTool.h"
#include "Common.h"

void Test(WCharCP unparsed)
{
	bvector<DPoint3d> _pts = {
		DPoint3d::FromZero(),
		Dpoint3d::From(1000,1000),
		Dpoint3d::From(2000,1000),
		Dpoint3d::From(2000,5000),
		Dpoint3d::From(4000,6000),
		DPoint3d::FromZero()
	};
	bvector<size_t> _indexs = { 1,2,3,4};
	CurveVectorPtr _curve = PDIWT::CuvreVectorExtension::CreateFilletArcCurve(_pts, _indexs, 100);
	if (_curve != nullptr)
	{
		EditElementHandle _eeh;
		DraftingElementSchema::ToElement(_eeh, *_curve, NULL, true, *ACTIVEMODEL);
		_eeh.AddToModel();
	}

}

void TestRailTool(WCharCP unparsed)
{
	PDIWT::Waterway::Lock::RailTool::InstallNewInstance();
}

/*---------------------------------------------------------------------------------**//**
* @description  Map key-in to function
* Commands
+---------------+---------------+---------------+---------------+---------------+------*/
static MdlCommandNumber s_commandNumbers[] =
{
{ Test, CMD_UNITTEST_TEST },
{ TestRailTool, CMD_UNITTEST_TESTRAILTOOL },
// end of list
0
};

/*---------------------------------------------------------------------------------**//**
* @description  MdlMain
* @param        argc      The number of command line parameters sent to the application.
* @param        argv[]    The array of strings sent to the application on the command line.
* @bsimethod                                                              Bentley Systems
+---------------+---------------+---------------+---------------+---------------+------*/
extern "C" void MdlMain(int argc, WCharCP argv[])
{
	RscFileHandle rfHandle;

	mdlResource_openFile(&rfHandle, NULL, RSC_READONLY);
	mdlState_registerStringIds(STRINGLISTID_Commands, STRINGLISTID_Prompts);
	mdlSystem_registerCommandNumbers(s_commandNumbers);
	mdlParse_loadCommandTable(NULL);
}
