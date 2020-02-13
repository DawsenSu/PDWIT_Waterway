#include "ECHelper.h"

PDIWT_NAMESPACE_BEGIN

BentleyStatus ECHelper::ImportECSChemaInActiveDgnBasedOnDefinedVariable(WString ecschemaFullName, WString pathByDefinedVariable)
{
	DgnECManagerR _ecMgr = DgnECManager::GetManager();

	WString _pdiwtSchemaName;
	uint32_t _versionMajor, _versionMinor;
	if (ECOBJECTS_STATUS_Success != ECSchema::ParseSchemaFullName(_pdiwtSchemaName, _versionMajor, _versionMinor, ecschemaFullName))
		return ERROR;

	if (!ConfigurationManager::IsVariableDefined(pathByDefinedVariable.GetWCharCP()))
		return ERROR;

	// Get Schema paths defined by variable at Organization Level
	WString _folderPaths;
	T_WStringVector _ecSchemaXMLFilePaths;
	if (SUCCESS != ConfigurationManager::GetVariable(_folderPaths, pathByDefinedVariable.GetWCharCP(), ConfigurationVariableLevel::Organization)||
		WString::IsNullOrEmpty(_folderPaths.GetWCharCP()))
		return ERROR;
	BeStringUtilities::Split(_folderPaths.GetWCharCP(), L";", _ecSchemaXMLFilePaths);

	// If dgnfile doesn't contain the designated schema, Import it.
	SchemaInfo _pdiwtSchemaInfo(SchemaKey(_pdiwtSchemaName.GetWCharCP(), _versionMajor, _versionMinor), *ISessionMgr::GetActiveDgnFile());
	if (!_ecMgr.IsSchemaContainedWithinFile(_pdiwtSchemaInfo, SCHEMAMATCHTYPE_LatestCompatible))
	{
		ECSchemaPtr _pdiwtSchemaImportPtr = nullptr;
		for each (auto _filepath in _ecSchemaXMLFilePaths)
		{
			if (SchemaReadStatus::SCHEMA_READ_STATUS_Success 
				==_ecMgr.ReadSchemaFromXmlFile(_pdiwtSchemaImportPtr, WString(_filepath + ecschemaFullName).GetWCharCP(), ISessionMgr::GetActiveDgnFile()))
				break;
			return ERROR;
		}
		if (SchemaImportStatus::SCHEMAIMPORT_Success !=  _ecMgr.ImportSchema(*_pdiwtSchemaImportPtr, *ISessionMgr::GetActiveDgnFile()) )
			return ERROR;
	}
	return SUCCESS;
}

BentleyStatus ECHelper::SetECPropertyValuesWithManagedObject(IECInstanceR instance, Object^ dataObj)
{
	if (nullptr == dataObj) return ERROR;
	Type^ objType = dataObj->GetType();

	//array<Object^>^ classAttrs = objType->GetCustomAttributes(ECClassAttribute::typeid, false);
	//if (classAttrs->Length == 0) return ERROR;
	//ECClassAttribute^ classAttrObj = dynamic_cast<ECClassAttribute^>(classAttrs[0]);
	//if (nullptr == classAttrObj) return ERROR;
	//pin_ptr<const WCHAR> _ecSchemaName =  PtrToStringChars(classAttrObj->SchemaName);
	//pin_ptr<const WCHAR> _ecClassName = PtrToStringChars(classAttrObj->ClassName);

	for each (auto prop in objType->GetProperties())
	{
		array<Object^>^ propAttrs = prop->GetCustomAttributes(PDIWT::ECPropertyAttribute::typeid, false);
		if (0 == propAttrs->Length) continue;
		ECPropertyAttribute^ propAttrObj = dynamic_cast<ECPropertyAttribute^>(propAttrs[0]);
		pin_ptr<const WCHAR> _accessStr = PtrToStringChars(propAttrObj->AccessString);

		double _scale = 1;
		array<Object^>^ unitAttrs = prop->GetCustomAttributes(PDIWT::UnitScaleAttribute::typeid, false);
		if (0 != unitAttrs->Length)
		{
			UnitScaleAttribute^ unitAttrObj = dynamic_cast<UnitScaleAttribute^>(unitAttrs[0]);
			_scale = unitAttrObj->ToMeterScale();
		}


		Type^ propType = prop->PropertyType;
		if (String::typeid == propType)
		{
			pin_ptr<const WCHAR> _propResultStr = PtrToStringChars(dynamic_cast<String^>(prop->GetValue(dataObj)));
			if (ECN::ECObjectsStatus::ECOBJECTS_STATUS_Success != instance.SetValue(_accessStr, ECValue(_propResultStr))) return ERROR;
		}
		else if (int::typeid == propType)
		{
			if (ECN::ECObjectsStatus::ECOBJECTS_STATUS_Success != instance.SetValue(_accessStr, ECValue(int((int)(prop->GetValue(dataObj))/_scale)))) return ERROR;
		}
		else if (long::typeid == propType)
		{
			if (ECN::ECObjectsStatus::ECOBJECTS_STATUS_Success != instance.SetValue(_accessStr, ECValue(long((long)(prop->GetValue(dataObj))/_scale)))) return ERROR;
		}
		else if (bool::typeid == propType)
		{
			if (ECN::ECObjectsStatus::ECOBJECTS_STATUS_Success != instance.SetValue(_accessStr, ECValue((bool)(prop->GetValue(dataObj))))) return ERROR;
		}
		else if (double::typeid == propType)
		{
			if (ECN::ECObjectsStatus::ECOBJECTS_STATUS_Success != instance.SetValue(_accessStr, ECValue((double)(prop->GetValue(dataObj))/_scale))) return ERROR;
		}
		else
		{
			pin_ptr<const WCHAR> _typeName = PtrToStringChars(propType->Name);
			WPrintfString errorStr(L"[%s], this type is not supported in Bentley EC.", _typeName);
			AString _localStr;
			BeStringUtilities::WCharToCurrentLocaleChar(_localStr, errorStr.GetWCharCP());
			throw std::exception(_localStr.c_str());
		}
	}
	return SUCCESS;
}


BentleyStatus PDIWT::ECHelper::SchedulePDIWTBaseECInstanceOnElement(EditElementHandleR eeh, WStringCR name, WStringCR code)
{
	const WString _ecSchemaFullName(L"PDWT_Waterway.01.00.ecschema.xml");
	const WString _ecSchemaPathDefinedVariable(L"PDIWT_ORGANIZATION_ECSCHEMAPATH");
	const WString _ecClassName(L"Base");
	const WString _ecSchemaName(L"PDWT_Waterway");

	if (SUCCESS != ECHelper::ImportECSChemaInActiveDgnBasedOnDefinedVariable(_ecSchemaFullName, _ecSchemaPathDefinedVariable))
		return ERROR;

	DgnECInstanceEnablerPtr _ecMainInstanceEnabler = DgnECManager::GetManager().ObtainInstanceEnablerByName(_ecSchemaName.GetWCharCP(), _ecClassName.GetWCharCP(), *eeh.GetDgnFileP());
	StandaloneECInstanceR _wipInstance = _ecMainInstanceEnabler->GetSharedWipInstance();
	_wipInstance.SetValue(L"Name", ECValue(name.GetWCharCP()));
	_wipInstance.SetValue(L"Code", ECValue(code.GetWCharCP()));
	if (DgnECInstanceStatus::DGNECINSTANCESTATUS_Success != _ecMainInstanceEnabler->ScheduleInstanceOnElement(nullptr, _wipInstance, eeh))
		return ERROR;

	return SUCCESS;
}

BentleyStatus PDIWT::ECHelper::GetPDIWTBaseNameAndCode(EditElementHandleR eeh, WStringR name, WStringR code)
{

	const WString _ecSchemaFullName(L"PDWT_Waterway.01.00.ecschema.xml");
	const WString _ecClassName(L"Base");

	DgnECManagerR _ecMgr = DgnECManager::GetManager();

	SchemaKey _schemaKey;
	if (SUCCESS != SchemaKey::ParseSchemaFullName(_schemaKey, _ecSchemaFullName.GetWCharCP()))
		return ERROR;
	SchemaInfo _schemaInfo(_schemaKey, *eeh.GetDgnFileP());
	ECSchemaPtr _ecSchemaPtr = _ecMgr.LocateSchemaInDgnFile(_schemaInfo,SchemaMatchType::SCHEMAMATCHTYPE_LatestCompatible);
	if (!_ecSchemaPtr.IsValid())
		return ERROR;

	//FindInstancesScopeOption _findInstanceScopeOption(DgnECHostType::Element, false);
	//FindInstancesScopePtr _findInstancScopPtr = FindInstancesScope::CreateScope(eeh, _findInstanceScopeOption);
	//ECQueryPtr	_queryPtr = ECQuery::CreateQuery(ECQUERY_PROCESS_Empty);
	//_queryPtr->AddSearchClass(_ecSchemaName.GetWCharCP(), _ecClassName.GetWCharCP());
	ECClassCP _ecClassCP = _ecSchemaPtr->GetClassCP(_ecClassName.GetWCharCP());
	if (nullptr == _ecClassCP) return ERROR;
	DgnECInstancePtr _baseInstancePtr = _ecMgr.FindInstanceOnElement(eeh, *_ecClassCP);
	if (!_baseInstancePtr.IsValid())
		return ERROR;
	if (ECOBJECTS_STATUS_Success != _baseInstancePtr->GetValueAsString(name, L"Name", false, -1))
		return ERROR;
	if (ECOBJECTS_STATUS_Success != _baseInstancePtr->GetValueAsString(code, L"Code", false, -1))
		return ERROR;
	return SUCCESS;
}

PDIWT_NAMESPACE_END

