#pragma once
#ifndef stdafx_h__
#define stdafx_h__
#define  PURE_MDL

#pragma managed(push, off)
#define winNT	1
#include <Mstn/MdlApi/MdlApi.h>
#include <DgnView/AccuDraw.h>
#include <DgnView/DgnElementSetTool.h>
#include <DgnPlatform/ITextEdit.h>
#include <DgnPlatform/TextHandlers.h>
#include <Mstn/isessionmgr.h>
#include <Mstn/MdlApi/msmdlmesh.fdf>
#include <DgnPlatform\DgnPlatformAPI.h>
#include <DgnPlatform/DimensionHandler.h>
#include <DgnPlatform/ISettings.h>
#include <PSolid\PSolidCoreAPI.h>
#include <Mstn\Smartfeature\SmartfeatureAPI.h>
#include <Mstn\Smartfeature\SmartFeature.r.h>

#include "BeDefiner.h"
#include "PDIWTDefiner.h"
#include <iostream>
#include <algorithm>
#include <tuple>
#include "rapidxml.hpp"


#pragma managed(pop)
#ifndef PURE_MDL

#include <vcclr.h>
#include <msclr/marshal.h>
#include <msclr/marshal_cppstd.h>
#include <vcruntime_string.h>
#include <gcroot.h>

#using <ustation.dll>
#using <Bentley.DgnPlatformNET.dll>
#using <Bentley.DgnDisplayNet.dll>
#using <Bentley.GeometryNET.dll>
#using <Bentley.GeometryNET.Structs.dll>
#using <Bentley.GeometryNET.Common.dll>
#using <Bentley.General.1.0.dll>
#using <Bentley.MicroStation.Interfaces.1.0.dll>
#using <Bentley.MicroStation.dll>
#using <Bentley.MicroStation.WPF.dll>
#using <Bentley.UI.dll>
#using <Bentley.Windowing.dll>

#using "mscorlib.dll"
#using <GalaSoft.MvvmLight.dll>
#using <GalaSoft.MvvmLight.Extras.dll>
#using <GalaSoft.MvvmLight.Platform.dll>
#using <PDIWT_Waterway_Lock.UI.dll>

#endif // !PURE_MDL


USING_NAMESPACE_BENTLEY
USING_NAMESPACE_BENTLEY_DGNPLATFORM
USING_NAMESPACE_BENTLEY_MSTNPLATFORM
USING_NAMESPACE_BENTLEY_MSTNPLATFORM_ELEMENT
USING_NAMESPACE_BENTLEY_ECOBJECT
USING_NAMESPACE_SMARTFEATURE
using namespace rapidxml;  //!  for rapid xml toolkit

#ifndef PURE_MDL
//! Managed Class namespace section
using namespace System;
using namespace System::Reflection;
using namespace System::ComponentModel;
using namespace System::Collections;
//using namespace System::Windows::Forms;
using namespace System::Windows::Controls;
using namespace System::Data;
using namespace System::Drawing;
using namespace System::Runtime::InteropServices;
using namespace System::Windows::Media::Imaging;

namespace BD = Bentley::DgnPlatformNET;
namespace BDE = Bentley::DgnPlatformNET::Elements;
namespace BM = Bentley::MstnPlatformNET;
namespace BMW = Bentley::MstnPlatformNET::WinForms;
namespace BMWPF = Bentley::MstnPlatformNET::WPF;
#endif // !PURE_MDL


// Switch for program control
#define LOG //to output dmsgsPrint to debug
#define DEBUG

#endif // stdafx_h__
