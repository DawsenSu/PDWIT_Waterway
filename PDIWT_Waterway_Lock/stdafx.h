#pragma once

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
#include "BeDefiner.h"

#pragma managed(pop)
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

USING_NAMESPACE_BENTLEY
USING_NAMESPACE_BENTLEY_DGNPLATFORM
USING_NAMESPACE_BENTLEY_MSTNPLATFORM
USING_NAMESPACE_BENTLEY_MSTNPLATFORM_ELEMENT
USING_NAMESPACE_BENTLEY_ECOBJECT

#define PDIWT_WATERWAY_LOCK_NAMESPACE_BEIGN	namespace PDIWT { namespace Waterway { namespace Lock {
#define PDIWT_WATERWAY_LOCK_NAMESPACE_END }}}
#define	USING_NAMESPACE_PDIWT_WATERWAY_LOCK using namespace PDIWT::Waterway::Lock;

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

