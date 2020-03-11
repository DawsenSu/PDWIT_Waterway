/*--------------------------------------------------------------------------------------+
|
|     $Source: MstnExamples/Elements/CurveExample/CurveExampleCmd.r $
|
|  $Copyright: (c) 2015 Bentley Systems, Incorporated. All rights reserved. $
|
+--------------------------------------------------------------------------------------*/
#include <Mstn\MdlApi\rscdefs.r.h>
#include <Mstn\MdlApi\cmdclass.r.h>

#include "PDIWT_Waterway_Lock_UnitTestids.h"

#define CT_NONE         0
#define CT_MAIN         1
#define CT_TEST         2 

#pragma suppressREQCmds

/*----------------------------------------------------------------------+
 Main command table
+----------------------------------------------------------------------*/
CommandTable CT_MAIN =
    { 
    { 1, CT_TEST,     MANIPULATION,   REQ,            "UNITTEST" },
    };

CommandTable CT_TEST =
    {
    { 1, CT_NONE,       INHERIT,        DEF,            "TEST",					CMDNAME_Test },
	{ 2, CT_NONE,       INHERIT,        DEF,            "TESTRAILTOOL",         CMDNAME_TestRailTool },
    };