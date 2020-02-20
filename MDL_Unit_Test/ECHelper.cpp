#include "stdafx.h"
#include "ECHelper.h"

namespace ECHelper
{
	bool GetECSchema(ECSchemaPtr& ecSchemaPtr,
		DgnFileP pDgnFile2Import,
		WString ecschemaFullName, WString filePath)
	{
		WString _pdiwtSchemaName;
		uint32_t _versionMajor, _versionMinor;
		if (ECOBJECTS_STATUS_Success != ECSchema::ParseSchemaFullName(
			_pdiwtSchemaName, _versionMajor, _versionMinor, 
			ecschemaFullName))
			return false;
		// If dgnfile doesn't contain the designated schema, Import it.
		DgnECManagerR ecMgr = DgnECManager::GetManager();
		SchemaInfo schemaInfo(SchemaKey(_pdiwtSchemaName.GetWCharCP(),
			_versionMajor, _versionMinor), *ISessionMgr::GetActiveDgnFile());
		if (false == ecMgr.IsSchemaContainedWithinFile(schemaInfo, SCHEMAMATCHTYPE_LatestCompatible))
		{
			if (SchemaReadStatus::SCHEMA_READ_STATUS_Success
				!= ecMgr.ReadSchemaFromXmlFile(ecSchemaPtr,
					WString(filePath + ecschemaFullName).GetWCharCP(), 
					ISessionMgr::GetActiveDgnFile()))
				return false;
			if (SchemaImportStatus::SCHEMAIMPORT_Success != 
				ecMgr.ImportSchema(*ecSchemaPtr, 
					*pDgnFile2Import))
				return false;
		}
		else
		{
			ecSchemaPtr =
				ecMgr.LocateSchemaInDgnFile(schemaInfo, SchemaMatchType::SCHEMAMATCHTYPE_LatestCompatible);
			if (false == ecSchemaPtr.IsValid())
				return false;
		}
		return true;
	}

	StandaloneECInstanceR GetECInstanceInActiveDgnFile(
		ECSchemaPtr & inEcSchemaPtr,
		ECClassCP inEclass)
	{
		DgnFileP pDf = ISessionMgr::GetActiveDgnFile();
		return GetECInstance(inEcSchemaPtr, inEclass, pDf);
	}

	StandaloneECInstanceR GetECInstance(ECSchemaPtr & inEcSchemaPtr, 
		ECClassCP inEclass, DgnFileP inDgnFile)
	{
		DgnECManagerR ecMgr = DgnECManager::GetManager();
		DgnECInstanceEnablerPtr ecMainInstanceEnabler =
			ecMgr.ObtainInstanceEnabler(*inEclass, *inDgnFile);
		StandaloneECInstanceR wipInstance =
			ecMainInstanceEnabler->GetSharedWipInstance();
		return wipInstance;
	}


	BentleyStatus ImportECSChemaInActiveDgnBasedOnDefinedVariable(WString ecschemaFullName, WString pathByDefinedVariable)
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
		if (SUCCESS != ConfigurationManager::GetVariable(_folderPaths, pathByDefinedVariable.GetWCharCP(), ConfigurationVariableLevel::Organization) ||
			WString::IsNullOrEmpty(_folderPaths.GetWCharCP()))
			return ERROR;
		BeStringUtilities::Split(_folderPaths.GetWCharCP(), L";", _ecSchemaXMLFilePaths);

		// If dgnfile doesn't contain the designated schema, Import it.
		SchemaInfo _pdiwtSchemaInfo(SchemaKey(_pdiwtSchemaName.GetWCharCP(), _versionMajor, _versionMinor), *ISessionMgr::GetActiveDgnFile());
		if (!_ecMgr.IsSchemaContainedWithinFile(_pdiwtSchemaInfo, SCHEMAMATCHTYPE_LatestCompatible))
		{
			ECSchemaPtr _pdiwtSchemaImportPtr = nullptr;
			size_t iSize = _ecSchemaXMLFilePaths.size();
			for (int i = 0; i < iSize; ++i)
			{
				Bentley::WStringCR _filepath = _ecSchemaXMLFilePaths[i];
				if (SchemaReadStatus::SCHEMA_READ_STATUS_Success
					== _ecMgr.ReadSchemaFromXmlFile(_pdiwtSchemaImportPtr, WString(_filepath + ecschemaFullName).GetWCharCP(), ISessionMgr::GetActiveDgnFile()))
					break;
				return ERROR;
			}
			if (SchemaImportStatus::SCHEMAIMPORT_Success != _ecMgr.ImportSchema(*_pdiwtSchemaImportPtr, *ISessionMgr::GetActiveDgnFile()))
				return ERROR;
		}
		return SUCCESS;
	}

	BentleyStatus SetECPropertyValuesWithManagedObject(IECInstanceR instance)
	{
		double dEcVal = 5.5;
		if (ECN::ECObjectsStatus::ECOBJECTS_STATUS_Success != 
			instance.SetValue(L"ECPropName", ECValue(dEcVal)))
			return ERROR;

		return SUCCESS;
	}


	BentleyStatus ECHelper::SchedulePDIWTBaseECInstanceOnElement(EditElementHandleR eeh, WStringCR name, WStringCR code)
	{
		const WString _ecSchemaFullName(L"PDWT_Waterway.01.00.ecschema.xml");
		const WString _ecSchemaPathDefinedVariable(L"PDIWT_ORGANIZATION_ECSCHEMAPATH");
		const WString _ecClassName(L"Base");
		const WString _ecSchemaName(L"PDWT_Waterway");

		if (SUCCESS != ECHelper::ImportECSChemaInActiveDgnBasedOnDefinedVariable(_ecSchemaFullName, _ecSchemaPathDefinedVariable))
			return ERROR;

		DgnECInstanceEnablerPtr _ecMainInstanceEnabler = 
			DgnECManager::GetManager().
			ObtainInstanceEnablerByName(_ecSchemaName.GetWCharCP(), 
				_ecClassName.GetWCharCP(), *eeh.GetDgnFileP());
		StandaloneECInstanceR _wipInstance = _ecMainInstanceEnabler->GetSharedWipInstance();
		_wipInstance.SetValue(L"Name", ECValue(name.GetWCharCP()));
		_wipInstance.SetValue(L"Code", ECValue(code.GetWCharCP()));
		if (DgnECInstanceStatus::DGNECINSTANCESTATUS_Success != 
			_ecMainInstanceEnabler->ScheduleInstanceOnElement(
				nullptr, _wipInstance, eeh))
			return ERROR;

		return SUCCESS;
	}

	BentleyStatus ECHelper::GetPDIWTBaseNameAndCode(EditElementHandleR eeh, WStringR name, WStringR code)
	{
		const WString _ecSchemaFullName(L"PDWT_Waterway.01.00.ecschema.xml");
		const WString _ecClassName(L"Base");

		DgnECManagerR _ecMgr = DgnECManager::GetManager();

		SchemaKey _schemaKey;
		if (SUCCESS != SchemaKey::ParseSchemaFullName(_schemaKey, _ecSchemaFullName.GetWCharCP()))
			return ERROR;
		SchemaInfo _schemaInfo(_schemaKey, *eeh.GetDgnFileP());
		ECSchemaPtr _ecSchemaPtr = _ecMgr.LocateSchemaInDgnFile(_schemaInfo, SchemaMatchType::SCHEMAMATCHTYPE_LatestCompatible);
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
}