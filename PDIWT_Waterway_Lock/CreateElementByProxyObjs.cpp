#include "CreateElementByProxyObjs.h"
PDIWT_NAMESPACE_BEGIN

BentleyStatus ScanCriteriaEx::GetElementBasedOnECClass(ElementAgendaR agd, WStringCR schemaName, WStringCR className)
{
	ElementCallBackData _data{ agd,schemaName,className };

	ScanCriteriaP _sc = ScanCriteria::Create();
	_sc->SetDrawnElements();
	_sc->SetModelRef(ACTIVEMODEL);
	_sc->SetElemRefCallback([](ElementRefP ele, CallbackArgP callback, ScanCriteriaP sc)->int {

		ElementCallBackData* _datap = reinterpret_cast<ElementCallBackData*>(callback);

		DgnECManagerR _ecMgr = DgnECManager::GetManager();
		EditElementHandle _eeh(ele, sc->GetModelRef());
		SchemaInfo _schemaInfo(SchemaKey(_datap->m_schemanName.GetWCharCP(), 1, 0), *sc->GetModelRef()->GetDgnFileP());
		ECSchemaPtr _ecSchema = _ecMgr.LocateSchemaInDgnFile(_schemaInfo, SCHEMAMATCHTYPE_Identical);
		if (!_ecSchema.IsValid()) return 0;
		ECClassP _ecClass = _ecSchema->GetClassP(_datap->m_calssName.GetWCharCP());
		if (nullptr == _ecClass) return 0;
		DgnElementECInstancePtr _eleECInstancePtr = _ecMgr.FindInstanceOnElement(_eeh, *_ecClass);
		if (!_eleECInstancePtr.IsValid()) return 0;
		_datap->m_agd.Insert(_eeh);
		return SUCCESS;

	}, &_data);
	_sc->Scan();
	ScanCriteria::Delete(_sc);

	return SUCCESS;
}
PDIWT_NAMESPACE_END


PDIWT_WATERWAY_LOCK_NAMESPACE_BEGIN
BentleyStatus PDIWT::Waterway::Lock::NativeElementByProxyObjsBuilder::PopulateProxyObjsListFromActiveModel(bvector<NativeProxyObjInfo>& infos)
{
	ElementAgenda _proxyElems;

	if (SUCCESS != ScanCriteriaEx::GetElementBasedOnECClass(_proxyElems, L"PDWT_Waterway", L"ProxyObject")) return ERROR;

	if (_proxyElems.size() == 0)
	{
		mdlOutput_messageCenter(OutputMessagePriority::Warning, L"Active model does not contain any proxy object", L"", OutputMessageAlert::None);
	}

	DgnECManagerR _ecMgr = DgnECManager::GetManager();
	SchemaInfo _schemaInfo(SchemaKey(L"PDWT_Waterway", 1, 0), *ACTIVEMODEL->GetDgnFileP());
	ECSchemaPtr _ecSchema = _ecMgr.LocateSchemaInDgnFile(_schemaInfo, SCHEMAMATCHTYPE_Identical);
	if (!_ecSchema.IsValid()) return ERROR;
	ECClassP _ecClass = _ecSchema->GetClassP(L"ProxyObject");
	if (nullptr == _ecClass) return ERROR;

	for each (auto _ele in _proxyElems)
	{

		long _eleId = _ele.GetElementId();

		//! obtain the element information
		DgnElementECInstancePtr _instancePtr = _ecMgr.FindInstanceOnElement(_ele, *_ecClass);
		if (_instancePtr.IsNull()) continue;
		ECValue _typeECValue;
		if (ECOBJECTS_STATUS_Success != _instancePtr->GetValue(_typeECValue, L"Type")) continue;
		NativeProxyObjectType _proxyObjectType = static_cast<NativeProxyObjectType>(_typeECValue.GetInteger());
		NativeProxySolidType _proxySolidType;
		switch (_proxyObjectType)
		{
		case PDIWT::Waterway::Lock::NativeProxyObjectType::Isolated:
			_proxySolidType = NativeProxySolidType::Isolated_P1;
			break;
		case PDIWT::Waterway::Lock::NativeProxyObjectType::Continuous:
			_proxySolidType = NativeProxySolidType::Continous_P1;
			break;
		default:
			break;
		}


		WString _parameters = L"<GuidewallP2InputParameters xmlns = \"PDWT_Waterway.01.00\"><DolphinTopElevation>10</DolphinTopElevation><DolphinBottomElevation>0</DolphinBottomElevation><DolphinTopLength>1.5</DolphinTopLength><DolphinTopWidth>1.8</DolphinTopWidth><DolphinBottomLength>1.5</DolphinBottomLength><DolphinBottomWidth>3.6</DolphinBottomWidth><CushioncapBottomElevation>-2.5</CushioncapBottomElevation><CushioncapFrontToeLength>1</CushioncapFrontToeLength><CushioncapRearToeLength>1</CushioncapRearToeLength><CushioncapExtraSideLength>0.25</CushioncapExtraSideLength><CushioncapChamferLength>0.3</CushioncapChamferLength><PileTopElevation>-2.4</PileTopElevation><PileBottomElevation>-10</PileBottomElevation><PileDiameter>1</PileDiameter><PileWallThickness>200</PileWallThickness><PileOffsetPerpendicularRiver>1.5</PileOffsetPerpendicularRiver><CushionThickness>100</CushionThickness><WallBottomElevation>3</WallBottomElevation><WallThickness>0.5</WallThickness><WallLength>10</WallLength><HasLeftWall>true</HasLeftWall><HasRightWall>true</HasRightWall><BridgeLength>10</BridgeLength><BridgeHeight>500</BridgeHeight><BridgeWidth>580</BridgeWidth><BridgeTopBoardThickness>120</BridgeTopBoardThickness><BridgeSupportWidth>120</BridgeSupportWidth><BridgeChamferWidth>50</BridgeChamferWidth></GuidewallP2InputParameters>";

		infos.push_back(NativeProxyObjInfo{ _proxyObjectType, _proxySolidType,  _parameters,_eleId });
	}
	return SUCCESS;
}

BentleyStatus Waterway::Lock::NativeElementByProxyObjsBuilder::CreateSolidEntityByProxyInfo(const bvector<NativeProxyObjInfo>& proxyObjInfos)
{
	for (auto _info : proxyObjInfos)
	{
		switch (_info.solidType)
		{
		case NativeProxySolidType::Continous_P1:
			break;
		case NativeProxySolidType::Isolated_P1:
			break;
		case NativeProxySolidType::Isolated_P11:
			break;
		case NativeProxySolidType::Isolated_P2:
			if (SUCCESS != Create_Isolated_P2(_info))
			{
				mdlOutput_messageCenter(OutputMessagePriority::Info, L"Can't create solid", L"", OutputMessageAlert::None);
			}
			break;
		case NativeProxySolidType::Isolated_P3:
			break;
		default:
			break;
		}
	}
	return SUCCESS;
}

BentleyStatus Waterway::Lock::NativeElementByProxyObjsBuilder::Create_Isolated_P1(const NativeProxyObjInfo& info)
{

	return SUCCESS;
}


BentleyStatus Waterway::Lock::NativeElementByProxyObjsBuilder::Create_Isolated_P11(const NativeProxyObjInfo& info)
{
	return SUCCESS;

}

BentleyStatus Waterway::Lock::NativeElementByProxyObjsBuilder::Create_Isolated_P2(const NativeProxyObjInfo& info)
{
	//	WString _dataNodeName(L"GuidewallP2Data");
	//	std::map<WString, WString> _paramterNameList
	//	{
	//		{L"DolphinTopElevation", L"double"},
	//		{L"DolphinBottomElevation",L"double"},
	//		{L"DolphinTopLength",L"double"},
	//		{L"DolphinTopWidth",L"double"},
	//		{L"DolphinBottomLength",L"double"},
	//		{L"DolphinBottomWidth",L"double"},
	//		{L"CushioncapBottomElevation",L"double"},
	//		{L"CushioncapFrontToeLength",L"double"},
	//		{L"CushioncapRearToeLength",L"double"},
	//		{L"CushioncapExtraSideLength",L"double"},
	//		{L"CushioncapChamferLength",L"double"},
	//		{L"PileTopElevation",L"double"},
	//		{L"PileBottomElevation",L"double"},
	//		{L"PileDiameter",L"double"},
	//		{L"PileWallThickness",L"double"},
	//		{L"PileOffsetPerpendicularRiver",L"double"},
	//		{L"CushionThickness",L"double"},
	//		{L"WallBottomElevation",L"double"},
	//		{L"WallThickness",L"double"},
	//		{L"WallLength",L"double"},
	//		{L"HasLeftWall",L"boolean"},
	//		{L"HasRightWall",L"boolean"},
	//		{L"BridgeLength",L"double"},
	//		{L"BridgeHeight",L"double"},
	//		{L"BridgeWidth",L"double"},
	//		{L"BridgeTopBoardThickness",L"double"},
	//		{L"BridgeSupportWidth",L"double"},
	//		{L"BridgeChamferWidth",L"double"}
	//	};
	//	try
	//	{
	//		UI::Model::GuidewallP2Data^ _guideWallData = gcnew UI::Model::GuidewallP2Data();
	//		xml_document<WChar> _xmldoc;
	//		//WChar _xmlData[6049];
	//		//BeStringUtilities::Memcpy(_xmlData, sizeof(_xmlData), info.parameters.GetWCharCP(), info.parameters.size());
	//		WCharP _xmlData = BeStringUtilities::Wcsdup(info.parameters.GetWCharCP());
	//		_xmldoc.parse<rapidxml::parse_default>(_xmlData);
	//		xml_node<WChar>* _xmlrootNode = _xmldoc.first_node(_dataNodeName.GetWCharCP());
	//		if (_xmlrootNode == nullptr) return ERROR;
	//
	//		for (auto _parameterName : _paramterNameList)
	//		{
	//			xml_node<WChar>* _xmlpara = _xmlrootNode->first_node(_parameterName.first.GetWCharCP());
	//			WCharP _valueStr = _xmlpara->value();
	//			if (_parameterName.second == L"double")
	//			{
	//				double _data = BeStringUtilities::Wcstod(_valueStr, nullptr);
	//#ifdef LOG
	//				mdlDialog_dmsgsPrint(WPrintfString(L"%s -> %d", _valueStr, _data));
	//#endif // LOG
	//			}
	//			else if (_parameterName.second.Equals(L"boolean"))
	//			{
	//				bool _data = WString(BeStringUtilities::Wcsupr(_valueStr)).Equals(L"TRUE") ? true : false;
	//#ifdef LOG
	//				mdlDialog_dmsgsPrint(WPrintfString(L"%s -> %d", _valueStr, _data));
	//#endif // LOG
	//				_guideWallData->HasLeftWall = _data;
	//			}
	//		}
	//
	//
	//		delete _xmlData;
	//		return SUCCESS;
	//	}
	//	catch (rapidxml::parse_error e)
	//	{
	//#ifdef LOG
	//		mdlDialog_dmsgsPrint(WPrintfString(L"%s\n%s", e.what(), e.where<const WChar>()));
	//#endif // LOG
	//		return ERROR;
	//	}

	//! parse the XML by instance
	SchemaInfo _ecSchemaInfo(SchemaKey(L"PDWT_Waterway", 1, 0), *ISessionMgr::GetActiveDgnFile());
	WString _ecClassName(L"GuidewallP2InputParameters");
	DgnECManagerR _ecMgr = DgnECManager::GetManager();

	ECSchemaPtr _ecSchemaPtr = _ecMgr.LocateSchemaInDgnFile(_ecSchemaInfo, SCHEMAMATCHTYPE_Latest);
	if (_ecSchemaPtr.IsNull()) return ERROR;

	ECInstanceReadContextPtr _readContextPtr = ECInstanceReadContext::CreateContext(*_ecSchemaPtr);
	if (_readContextPtr == nullptr) return ERROR;
	IECInstancePtr _readInstancePtr;
	if (InstanceWriteStatus::INSTANCE_WRITE_STATUS_Success != IECInstance::ReadFromXmlString(_readInstancePtr, info.parameters.GetWCharCP(), *_readContextPtr))
		return ERROR;
	if (_readInstancePtr == nullptr) return ERROR;

	gcroot<GuidewallP2Data^> _p2Data = gcnew GuidewallP2Data();
	ECValue _propValue;
	ECClassCR _ecClass = _readInstancePtr->GetClass();
	ECPropertyIterable _classIterable = _ecClass.GetProperties(true);
	for (auto _itr : _classIterable)
	{

		WString _propName = _itr->GetName();
		WString _propType = _itr->GetTypeName();
		_propType.ToLower();
		gcroot<PropertyInfo^> _dataPropInfo = _p2Data->GetType()->GetProperty(gcnew String(_propName.GetWCharCP()));
		if (ECOBJECTS_STATUS_Success != _readInstancePtr->GetValue(_propValue, _propName.GetWCharCP()))
		{
#ifdef LOG
			mdlDialog_dmsgsPrint(WPrintfString(L"%s Can't get Value", _propName));
#endif //
			return ERROR;
		}
		if (_propType.Equals(L"double"))
		{
			_dataPropInfo->SetValue(_p2Data, _propValue.GetDouble());
		}
		else if (_propType.Equals(L"boolean"))
		{
			_dataPropInfo->SetValue(_p2Data, _propValue.GetBoolean());
		}
		else if (_propType.Equals(L"string"))
		{
			_dataPropInfo->SetValue(_p2Data, gcnew String(_propValue.GetString()));
		}
		else
		{
#ifdef LOG
			
			mdlDialog_dmsgsPrint(WPrintfString(L"%s: %s is not supported yet", _propName, _propType));
#endif //
			return ERROR;
		}
	}
	gcroot<GuidewallP2Data^> _p2DataInMeter = _p2Data->CloneGuidewallDataP2InMeters();
	GuidewallP2 guidewallp2(_p2DataInMeter);
	EditElementHandle guidewallEeh;
	if (SUCCESS != guidewallp2.CreateGuidewallP2(guidewallEeh, *ACTIVEMODEL))
	{
#ifdef LOG
		mdlDialog_dmsgsPrint(L"Cannot Create guidewallp2");
#endif //
		return ERROR;
	}

	//! get the Proxy Objects transform
	PersistentElementRefP _ele = ISessionMgr::GetActiveDgnModelP()->FindElementByID(info.proxyElementId);
	EditElementHandle _eeh(_ele);
	CurveVectorPtr _cv = ICurvePathQuery::ElementToCurveVector(_eeh);
	if (!_cv.IsValid()) return ERROR;
	ICurvePrimitivePtr _icp = _cv->front();
	const bvector<Dpoint3d>* _pointsList = _icp->GetLineStringCP();

	DPoint3d _insertPoint = DPoint3d::FromInterpolate(_pointsList->at(0), 0.5, _pointsList->at(1));
	DVec3d _xdirection = _pointsList->at(1) - _pointsList->at(0);
	DVec3d _ydirection = _pointsList->at(2) - _pointsList->at(1);
	_xdirection.Normalize();
	_ydirection.Normalize();
	RotMatrix _rotMatrix = RotMatrix::From2Vectors(_xdirection, _ydirection);
	TransformInfo _trans = TransformInfo(Transform::From(_rotMatrix, _insertPoint));
	if (SUCCESS != guidewallEeh.GetHandler().ApplyTransform(guidewallEeh, _trans))
	{
#ifdef LOG
		mdlDialog_dmsgsPrint(L"Cannot apply transform");
#endif // LOG
		return ERROR;
	}

	guidewallEeh.AddToModel();

#ifdef LOG
	mdlOutput_messageCenter(OutputMessagePriority::Debug, L"P2 Create Successfully.", L"", OutputMessageAlert::None);
#endif // LOG
	return SUCCESS;
	
}

BentleyStatus Waterway::Lock::NativeElementByProxyObjsBuilder::Create_Isolated_P3(const NativeProxyObjInfo& info)
{

	return SUCCESS;
}

BentleyStatus Waterway::Lock::NativeElementByProxyObjsBuilder::Create_Continuous_P1(const NativeProxyObjInfo& info)
{

	return SUCCESS;
}



List<ProxyObjsInfo^>^ ElementByProxyObjsBuilder::PopulateProxyObjsListFromActiveModel()
{
	List<ProxyObjsInfo^>^ _retList = gcnew List<ProxyObjsInfo ^>();
	bvector<NativeProxyObjInfo> _infos;
	if (SUCCESS == NativeElementByProxyObjsBuilder::PopulateProxyObjsListFromActiveModel(_infos))
	{
		for each (auto _info in _infos)
		{
			ProxyObjsInfo^ _managedProxyObjs = gcnew ProxyObjsInfo();
			_managedProxyObjs->ProxyType = static_cast<ProxyObjectType>(_info.proxyType);
			_managedProxyObjs->SolidType = static_cast<ProxySolidType>(_info.solidType);
			_managedProxyObjs->Parameters = gcnew String(_info.parameters.GetWCharCP());
			_managedProxyObjs->ProxyElementId = _info.proxyElementId;
			_retList->Add(_managedProxyObjs);
		}
	}

	return _retList;
}

void PDIWT::Waterway::Lock::ElementByProxyObjsBuilder::CreateSolidByProxyObject(List<ProxyObjsInfo^>^ proxyObjInfoList)
{
	bvector<NativeProxyObjInfo> _proxyInfoList;
	for each (auto _managedProxyInfo in proxyObjInfoList)
	{
		pin_ptr<const WChar> _parameters = PtrToStringChars(_managedProxyInfo->Parameters);
		WString _parametersStr(_parameters);
		NativeProxyObjInfo _info{ static_cast<NativeProxyObjectType>(_managedProxyInfo->ProxyType),static_cast<NativeProxySolidType>(_managedProxyInfo->SolidType), _parameters, _managedProxyInfo->ProxyElementId };
		_proxyInfoList.push_back(_info);
	}
	NativeElementByProxyObjsBuilder::CreateSolidEntityByProxyInfo(_proxyInfoList);

}

PDIWT_WATERWAY_LOCK_NAMESPACE_END



