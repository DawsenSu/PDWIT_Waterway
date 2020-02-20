#include "stdafx.h"
#include "Examples.h"

#include "ShipLockRoomP7Data.h"
#include "EleCreatorHelper.h"
#include "ECHelper.h"
#include "PickTool.h"

using namespace EleCreatorHelper;

void Examples::SetPara2CeateEle(WCharCP unparsed)
{
	EditElementHandle eeh;
	ShipLockRoomP7Data data;
	ShipLockRoomP7Data::Create(eeh, &data);
	DisplayHandler& dh = DisplayHandler::GetInstance();
	Transform tr;
	dh.GetBasisTransform(eeh, tr);
	eeh.AddToModel();
	//CreateTestHelper<ShipLockRoomP7Data>(unparsed, ShipLockRoomP7Data::Create);
}

ECSchemaPtr g_ec;
void Examples::AddEC2Ele(WCharCP unparsed)
{
	if(false == CreatorHelper<ShipLockRoomP7Data>::
		ParserPara(unparsed))
		return;
	CreatorHelper<ShipLockRoomP7Data>::SetField();
	EditElementHandle eeh;
	ShipLockRoomP7Data& data = 
		CreatorHelper<ShipLockRoomP7Data>::
		CreateHelperInOrigin(eeh, ShipLockRoomP7Data::Create);

	DisplayHandler& dh = DisplayHandler::GetInstance();
	DPoint3d dpOff;
	dh.GetTransformOrigin(eeh, dpOff);
	double uorparameter = (*ACTIVEMODEL).GetModelInfoCP()->GetUorPerMeter();
	dpOff.x /= uorparameter;
	dpOff.y /= uorparameter;
	dpOff.z /= uorparameter;
	
	CreatorHelper<ShipLockRoomP7Data>::Mov2SpecPos(eeh);
	eeh.AddToModel();

	//EditElementHandle eeh;
	//DPoint3d dpOff;
	//ShipLockRoomP7Data& data = CreatorHelper<ShipLockRoomP7Data>::CreateTestHelper(unparsed,
	//ShipLockRoomP7Data::Create, eeh);

	DgnECManagerR ecMgr = DgnECManager::GetManager();
	DgnFileP pDf = ISessionMgr::GetActiveDgnFile();

	bool rtl = ECHelper::GetECSchema(g_ec, pDf,
		L"PDWT_Waterway.01.00.ecschema.xml",
		L"D:\\BentleyDevConfigs\\");
	if (rtl == false)
	{
		mdlDialog_dmsgsPrint(L"AddEC2Ele GetECSchema false");
		return;
	}

	ECClassCP inEclass = g_ec->GetClassCP(L"ShipLockRoomP7");
	DgnECInstanceEnablerPtr ecMainInstanceEnabler =
		ecMgr.ObtainInstanceEnabler(*inEclass, *pDf);
	StandaloneECInstanceR wipInstance =
		ecMainInstanceEnabler->GetSharedWipInstance();
	ShipLockRoomP7Data::SetECInstWithData(wipInstance, data);

	wipInstance.SetValue(L"OffsetInsertPoint", ECValue(dpOff));

	DgnElementECInstancePtr outECInst;
	DgnECInstanceStatus ecRtl =
		ecMainInstanceEnabler->CreateInstanceOnElement(
			&outECInst, wipInstance, eeh);

	//rtl = ECHelper::Data2EC_AddEC2Ele
	//	<ShipLockRoomP7Data>
	//	(L"ShipLockRoomP7", g_ec, pDf, eeh, data
	//		, ShipLockRoomP7Data::SetECInstWithData);
	if (rtl == false)
	{
		mdlDialog_dmsgsPrint(L"AddEC2Ele Data2EC_AddEC2Ele false");
	}
}

void Examples::ReplaceEle(WCharCP unparsed)
{
	DgnECManagerR ecMgr = DgnECManager::GetManager();
	DgnFileP pDf = ISessionMgr::GetActiveDgnFile();
	
	bool rtl = ECHelper::GetECSchema(g_ec, pDf,
		L"PDWT_Waterway.01.00.ecschema.xml",
		L"D:\\BentleyDevConfigs\\");
	if (rtl == false)
	{
		mdlDialog_dmsgsPrint(L"AddEC2Ele GetECSchema false");
		return;
	}

	PickTool::InstallNewInstance(121, g_ec);
}

void Examples::DeleteECSchema5DgnFile()
{
	g_ec->RemoveReferencedSchema(*g_ec);
}