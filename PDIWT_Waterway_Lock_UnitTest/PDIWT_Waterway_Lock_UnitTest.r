/*--------------------------------------------------------------------------------------+
|
|     $Source: MstnExamples/Elements/CurveExample/CurveExampleCmd.r $
|
|  $Copyright: (c) 2015 Bentley Systems, Incorporated. All rights reserved. $
|
+--------------------------------------------------------------------------------------*/
#include <Mstn\MdlApi\rscdefs.r.h>
#include <Mstn\MdlApi\cmdclass.r.h>

/*-----------------------------------------------------------------------
 Setup for native code only MDL app
-----------------------------------------------------------------------*/
#define DLLAPP_PRIMARY 1

DllMdlApp DLLAPP_PRIMARY =
    {
    L"PDIWT_Waterway_Lock_UnitTest", L"PDIWT_Waterway_Lock_UnitTest" // taskid, dllName
    }