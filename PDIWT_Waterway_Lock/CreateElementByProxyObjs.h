#pragma once

#include "stdafx.h"
#include "GuidewallP2.h"

PDIWT_STRUCTDEF(ScanCriteriaEx);

PDIWT_NAMESPACE_BEGIN

public struct ScanCriteriaEx
{
private:
	struct ElementCallBackData
	{
		ElementAgendaR m_agd;
		WStringCR	m_schemanName;
		WStringCR	m_calssName;
	};
public:

	//! GetElementBasedOnECClass, get all elements attached by ECInstance with given schema and class name.
	//! Access:    public static 
	//! @param[out] agd		All elements attached by wanted ECIntance.
	//! @param[in] schemaName	Schema name for search
	//! @param[in] className	class name for search
	//! @return    BentleyStatus SUCCESS if operation succeeds.
	static BentleyStatus GetElementBasedOnECClass(ElementAgendaR agd, WStringCR schemaName, WStringCR className);

};
PDIWT_NAMESPACE_END

PDIWT_WATERWAY_LOCK_NAMESPACE_BEGIN
USING_NAMESPACE_PDIWT

enum class NativeProxyObjectType
{
	Isolated,
	Continuous
};

enum class NativeProxySolidType
{
	Isolated_P1,
	Isolated_P11,
	Isolated_P2,
	Isolated_P3,
	Continous_P1
};

//! struct to store proxy information
public struct NativeProxyObjInfo
{
	NativeProxyObjectType		proxyType;
	NativeProxySolidType		solidType;
	// store xml fragments to parse the parameters
	WString						parameters;
	long long					proxyElementId;
};

struct NativeElementByProxyObjsBuilder
{
	//! Scan active model to get current information, solidType of NativeProxyObjInfo will be set to Isolated_P1 and Continuous_P1 according to its proxyType, respectively.
	//! proxyType will read from current model, but solid Type will be decided by User through UI.
	//! Access:    public static 
	//! @param[out] proxyObjInfos	information list of proxy objects in current active model
	//! @return    BentleyStatus SUCCESS if the operation succeeds.
	static	BentleyStatus	PopulateProxyObjsListFromActiveModel(bvector<NativeProxyObjInfo>& proxyObjInfos);

	static	BentleyStatus	CreateSolidEntityByProxyInfo(const bvector<NativeProxyObjInfo>& proxyObjInfos);
private:
	static	BentleyStatus	Create_Isolated_P1(const NativeProxyObjInfo& info);
	static	BentleyStatus	Create_Isolated_P11(const NativeProxyObjInfo& info);
	static	BentleyStatus	Create_Isolated_P2(const NativeProxyObjInfo& info);
	static	BentleyStatus	Create_Isolated_P3(const NativeProxyObjInfo& info);
	static	BentleyStatus	Create_Continuous_P1(const NativeProxyObjInfo& info);

};



using namespace UI::ViewModel;
using namespace UI::Model;
using namespace System::Collections::Generic;

public ref class ElementByProxyObjsBuilder : public UI::ViewModel::ICreateElementByProxyObjs
{

public:
	virtual List<ProxyObjsInfo^>^ PopulateProxyObjsListFromActiveModel();


	virtual void CreateSolidByProxyObject(List<ProxyObjsInfo^>^ proxyObjInfoList);

};

PDIWT_WATERWAY_LOCK_NAMESPACE_END
