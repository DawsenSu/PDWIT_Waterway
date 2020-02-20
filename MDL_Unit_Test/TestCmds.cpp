#include "TestCmds.h"
#include "MDL_Unit_TestCmd.h"
#include "Examples.h"


MdlCommandNumber cmdNums[] =
{
	{ (CmdHandler)TestCmds::CmdAll,    CMD_PDITEST_CMDALL},
	{ (CmdHandler)TestCmds::Cmd0,    CMD_PDITEST_CMD0},
	{ (CmdHandler)TestCmds::Cmd1,    CMD_PDITEST_CMD1},
	{ (CmdHandler)TestCmds::Cmd2,    CMD_PDITEST_CMD2},
	{ (CmdHandler)TestCmds::Cmd3,    CMD_PDITEST_CMD3},
	{ (CmdHandler)TestCmds::Cmd4,    CMD_PDITEST_CMD4},
	{ (CmdHandler)TestCmds::Cmd5,    CMD_PDITEST_CMD5},
	{ (CmdHandler)TestCmds::Cmd6,    CMD_PDITEST_CMD6},
	{ (CmdHandler)TestCmds::Cmd7,    CMD_PDITEST_CMD7},
	{ (CmdHandler)TestCmds::Cmd8,    CMD_PDITEST_CMD8},
	{ (CmdHandler)TestCmds::Cmd9,    CMD_PDITEST_CMD9},
	0
};

void TestCmds::IniCmds()
{
	RscFileHandle rscFileH;
	mdlResource_openFile(&rscFileH, NULL, RSC_READONLY);
	mdlParse_loadCommandTable(NULL);

	mdlSystem_registerCommandNumbers(cmdNums);
}

void TestCmds::CmdAll(WCharCP unparsed)
{

}

void TestCmds::Cmd0(WCharCP unparsed)
{
	Examples::SetPara2CeateEle(unparsed);
}

void TestCmds::Cmd1(WCharCP unparsed)
{
	Examples::AddEC2Ele(unparsed);
}

void TestCmds::Cmd2(WCharCP unparsed)
{
	Examples::ReplaceEle(unparsed);
}

void TestCmds::Cmd3(WCharCP unparsed)
{
	Examples::DeleteECSchema5DgnFile();
}

void TestCmds::Cmd4(WCharCP unparsed)
{
}

void TestCmds::Cmd5(WCharCP unparsed)
{
}

void TestCmds::Cmd6(WCharCP unparsed)
{
}

void TestCmds::Cmd7(WCharCP unparsed)
{
}

void TestCmds::Cmd8(WCharCP unparsed)
{
}

void TestCmds::Cmd9(WCharCP unparsed)
{
}
