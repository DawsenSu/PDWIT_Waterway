#include <Mstn\MdlApi\rscdefs.r.h>
#include <Mstn\MdlApi\cmdclass.r.h>

/*----------------------------------------------------------------------+
|   Local Defines												
+----------------------------------------------------------------------*/
enum CmdTableIds
{
   CT_NONE = 0,
   CT_MAIN,
   CT_TestCmds
};

CommandTable   CT_MAIN =
{ 
	{1, CT_TestCmds, INHERIT, REQ, "PDITest"}
};

CommandTable CT_TestCmds =
{
	{1, CT_NONE, INHERIT, DEF, "CmdAll"},
	{2, CT_NONE, INHERIT, NONE, "Cmd0"},
	{3, CT_NONE, INHERIT, NONE, "Cmd1"},
	{4, CT_NONE, INHERIT, NONE, "Cmd2"},
	{5, CT_NONE, INHERIT, NONE, "Cmd3"},
	{6, CT_NONE, INHERIT, NONE, "Cmd4"},
	{7, CT_NONE, INHERIT, NONE, "Cmd5"},
	{8, CT_NONE, INHERIT, NONE, "Cmd6"},
	{9, CT_NONE, INHERIT, NONE, "Cmd7"},
	{10, CT_NONE, INHERIT, NONE, "Cmd8"},
	{11, CT_NONE, INHERIT, NONE, "Cmd9"},
}