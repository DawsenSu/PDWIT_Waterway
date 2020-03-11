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
}

void PDIWT::Waterway::Lock::KeyinCommands::CreateGuidewallP3(System::String^ unparsed)
{
	mdlOutput_message(L"CreateGuidewallP3");
}

void PDIWT::Waterway::Lock::KeyinCommands::ArrangeDolphinGuidewall(System::String^ unparsed)
{
	GuidewallPlacementTool::InstallNewInstance();
}

void PDIWT::Waterway::Lock::KeyinCommands::CreateElementByProxyObjs(System::String^ unparsed)
{
	ElementByProxyObjsBuilder^ _interface = gcnew ElementByProxyObjsBuilder();
	UI::CreateElementByProxyObjsWindow::ShowWindow(Program::Addin, _interface);
}

void PDIWT::Waterway::Lock::KeyinCommands::CreateGravitationalGuidewall(System::String^ unparsed)
{
	GuidewallP3Tool::InstallNewInstance();

}
void PDIWT::Waterway::Lock::KeyinCommands::Test(System::String^ unparsed)
{
	//CreateElementByProxyObjs::Create();
#pragma region DisplayHandler->GetOrientation
	//DgnModelP _dgnModel = ISessionMgr::GetActiveDgnModelP();
	//PersistentElementRefP _elem = _dgnModel->FindElementByID((ElementId)1403);
	//EditElementHandle _eleH(_elem);
	//RotMatrix _rot;
	//_eleH.GetDisplayHandler()->GetOrientation(_eleH, _rot);
	//double _r00, _r01, _r02, _r10, _r11, _r12, _r20, _r21, _r22;
	//_rot.GetRowValues(_r00, _r01, _r11, _r10, _r11, _r12, _r20, _r21, _r22);
	//mdlDialog_dmsgsPrint(WPrintfString(L"rot matrix is : (%.2f, %.2f, %.2f,),(%.2f, %.2f, %.2f,),(%.2f, %.2f, %.2f,)", _r00, _r01, _r02, _r10, _r11, _r12, _r20, _r21, _r22));
	//WString _xmlFragment(L"<GuidewallP2Data Name=\"TestTemplate\"><DolphinTopElevation>10</DolphinTopElevation><DolphinBottomElevation>0</DolphinBottomElevation><DolphinTopLength>1.5</DolphinTopLength></GuidewallP2Data>");
#pragma endregion

#pragma region rapidxml
	//try 
	//{
	//	WChar _xml[4096] = L"<GuidewallP2Data Name=\"TestTemplate\"><DolphinTopElevation>10</DolphinTopElevation><DolphinBottomElevation>0</DolphinBottomElevation><DolphinTopLength>1.5</DolphinTopLength></GuidewallP2Data>";
	//	xml_document<WChar> _doc;
	//	_doc.parse<0>(_xml);
	//	xml_node<WChar>* _rootnode = _doc.first_node();
	//	mdlDialog_dmsgsPrint(_rootnode->name());
	//	for (xml_node<WChar>* _dataNode = _rootnode->first_node(); _dataNode; _dataNode = _dataNode->next_sibling())
	//	{
	//		mdlDialog_dmsgsPrint(WPrintfString(L"Property Name:%s, Value:%s",_dataNode->name(), _dataNode->value()));
	//	}
	//}
	//catch (...)
	//{
	//	mdlDialog_dmsgsPrint(L"Something went wrong");
	//}
#pragma endregion

#pragma region ECClass
//	DgnECManagerR _ecMgr = DgnECManager::GetManager();
//	SchemaInfo _ecSchemaInfo(SchemaKey(L"PDWT_Waterway", 1, 0), *ISessionMgr::GetActiveDgnFile());
//	ECSchemaPtr _ecSchemaPtr = _ecMgr.LocateSchemaInDgnFile(_ecSchemaInfo, SchemaMatchType::SCHEMAMATCHTYPE_Latest);
//	if (_ecSchemaPtr.IsNull()) return;
//	ECClassP _ecClassP = _ecSchemaPtr->GetClassP(L"GuidewallP2InputParameters");
//	if (_ecClassP == nullptr) return;
//	
//	DgnECInstanceEnablerP _instanceEnabler = _ecMgr.ObtainInstanceEnabler(*_ecClassP, *ISessionMgr::GetActiveDgnFile());
//	if (_instanceEnabler == nullptr) return;
//	StandaloneECInstanceR _wipInstance = _instanceEnabler->GetSharedWipInstance();
//	_wipInstance.SetValue(L"DolphinTopElevation", ECValue(10.0));
//	WString _instanceXML;
//	if (InstanceWriteStatus::INSTANCE_WRITE_STATUS_Success != _wipInstance.WriteToXmlString(_instanceXML, true, false))
//		return;
//#ifdef LOG
//	mdlDialog_dmsgsPrint(_instanceXML.GetWCharCP());
//#endif
//
//	IECInstancePtr _readInstancePtr;
//	ECInstanceReadContextPtr _instanceReadContextPtr = ECInstanceReadContext::CreateContext(*_ecSchemaPtr);
//	if (_instanceReadContextPtr.IsNull())
//	{
//#ifdef LOG
//		mdlDialog_dmsgsPrint(L"ReadContextPtr is nullptr");
//		return;
//#endif // LOG
//
//	}
//	if (InstanceWriteStatus::INSTANCE_WRITE_STATUS_Success != IECInstance::ReadFromXmlString(_readInstancePtr, _instanceXML.GetWCharCP(), *_instanceReadContextPtr))
//		return;
//	if (_readInstancePtr == nullptr)
//		return;
//	ECValue _dolphinTopElevationValue;
//	if (ECOBJECTS_STATUS_Success != _readInstancePtr->GetValue(_dolphinTopElevationValue, L"DolphinTopElevation"))
//		return;
//#ifdef LOG
//	mdlDialog_dmsgsPrint(WPrintfString(L" DolphinTopElevation -> %f", _dolphinTopElevationValue.GetDouble()));
//#endif // LOG
//
#pragma endregion

#pragma region Create_GuidewallP2
	//WString _parameters =	 L"<GuidewallP2InputParameters xmlns = \"PDWT_Waterway.01.00\"><DolphinTopElevation>10</DolphinTopElevation><DolphinBottomElevation>0</DolphinBottomElevation><DolphinTopLength>1.5</DolphinTopLength><DolphinTopWidth>1.8</DolphinTopWidth><DolphinBottomLength>1.5</DolphinBottomLength><DolphinBottomWidth>3.6</DolphinBottomWidth><CushioncapBottomElevation>-2.5</CushioncapBottomElevation><CushioncapFrontToeLength>1</CushioncapFrontToeLength><CushioncapRearToeLength>1</CushioncapRearToeLength><CushioncapExtraSideLength>0.25</CushioncapExtraSideLength><CushioncapChamferLength>0.3</CushioncapChamferLength><PileTopElevation>-2.4</PileTopElevation><PileBottomElevation>-10</PileBottomElevation><PileDiameter>1</PileDiameter><PileWallThickness>200</PileWallThickness><PileOffsetPerpendicularRiver>1.5</PileOffsetPerpendicularRiver><CushionThickness>100</CushionThickness><WallBottomElevation>3</WallBottomElevation><WallThickness>0.5</WallThickness><WallLength>10</WallLength><HasLeftWall>true</HasLeftWall><HasRightWall>true</HasRightWall><BridgeLength>10</BridgeLength><BridgeHeight>500</BridgeHeight><BridgeWidth>580</BridgeWidth><BridgeTopBoardThickness>120</BridgeTopBoardThickness><BridgeSupportWidth>120</BridgeSupportWidth><BridgeChamferWidth>50</BridgeChamferWidth></GuidewallP2InputParameters>";
	//NativeProxyObjInfo _data{ NativeProxyObjectType::Isolated, NativeProxySolidType::Isolated_P2, _parameters , 10};
	//NativeElementByProxyObjsBuilder::CreateSolidEntityByProxyInfo(bvector<NativeProxyObjInfo>{_data});
#pragma endregion

#pragma region Create Shape By CurveVector
	//PDIWT::Waterway::Lock::GuidwallP3Data m_data{0.6,0.3,10.0,2.0,0,3,PI/4};
	//bvector<DVec3d> _segmentsDVec2d =
	//{
	//	DVec3d::From(m_data.frontToeLength,m_data.frontToeHeight),
	//	DVec3d::From(0,m_data.dolphinHeight - m_data.frontToeHeight),
	//	DVec3d::From(m_data.dolphinTopWidth,0),
	//	DVec3d::From(m_data.firstBackwallSegHeight*sin(m_data.firstBackwallSegAngle),-m_data.firstBackwallSegHeight),
	//	DVec3d::From((m_data.dolphinHeight - m_data.firstBackwallSegHeight) * sin(m_data.secondBackwallSegAngle),-(m_data.dolphinHeight - m_data.firstBackwallSegHeight)),
	//	//DVec2d::From(m_data.frontToeLength,m_data.frontToeHeight),

	//};
	//bvector<DPoint3d> _pts = { DPoint3d::From(0,0) };
	//for (size_t i = 0; i < _segmentsDVec2d.size(); i++)
	//{
	//	_segmentsDVec2d.at(i).Scale(10000);
	//	_pts.push_back(_pts.at(i) + _segmentsDVec2d.at(i));
	//}
	//CurveVectorPtr curveVec = CurveVector::CreateLinear(_pts, CurveVector::BOUNDARY_TYPE_Outer);
	//EditElementHandle _eeh;
	//DraftingElementSchema::ToElement(_eeh, *curveVec, nullptr, true, *ACTIVEMODEL);
	//_eeh.AddToModel();
#pragma endregion

RailTool::InstallNewInstance();

}


