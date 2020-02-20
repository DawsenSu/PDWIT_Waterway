#include "stdafx.h"
#include "PickTool.h"
#include <DgnPlatform\DgnPlatformApi.h>

#include "EleOperHelper.h"
#include "ShipLockRoomP7Data.h"


void PickTool::InstallNewInstance(int toolId, ECSchemaPtr& ec)
{
	PickTool* pt = new PickTool(toolId, ec);
	pt->InstallTool();
}

PickTool::PickTool(int toolId, ECSchemaPtr& ec) :m_ec(ec)
{
	mdlDialog_dmsgsPrint(L"PickTool Constructor.");
}

PickTool::~PickTool()
{
	mdlDialog_dmsgsPrint(L"PickTool Destructor.");
}

RotMatrix g_rm;
bool PickTool::_OnDataButton(DgnButtonEventCR ev)
{
	HitPathCP hitPath = _DoLocate(ev, 
		true, Bentley::DgnPlatform::ComponentMode::None);
	ElementRefP erp = nullptr;

	if (NULL != hitPath)
		erp = hitPath->GetHeadElem();
	else
		return false;
	if (erp == nullptr)
		return false;

	DgnModelRefP mr = erp->GetDgnModelP();
	EditElementHandle eeh(erp, mr);

	//Read EC Instance.
	DgnECManagerR ecMgr = DgnECManager::GetManager();

	ECClassCP eclass = m_ec->GetClassCP(m_strECName);
	DgnECInstancePtr baseInstancePtr = ecMgr.
		FindInstanceOnElement(eeh, *eclass);

	if (false == baseInstancePtr.IsValid())
	{
		mdlDialog_dmsgsPrint(L"PickTool _OnDataButton EC FindInstanceOnElement False.");
		return false;
	}

	if (m_iCounter == 0)
	{
		++m_iCounter;
		
		//ShipLockRoomP7Data data;
		ShipLockRoomP7Data::ReadECInst2Data(baseInstancePtr, m_dataPick);
		//m_offPick = ShipLockRoomP7Data::ReadECOffPoint(baseInstancePtr);
		/*ECValue ecValSafeHeight;
		if (ECOBJECTS_STATUS_Success !=
			baseInstancePtr->GetValue(ecValSafeHeight, L"SafeSuperElvation"))
		{
			mdlDialog_dmsgsPrint(L"_OnDataButton EC GetValue False.");
			return false;
		}
		*/
		std::wstring wsSafeH = std::to_wstring(m_dataPick.SafeSuperElvation);
		std::wstring wsRtl(L"safeHeight value is ");
		wsRtl.append(wsSafeH);
		mdlDialog_dmsgsPrint(wsRtl.c_str());
	}
	else
	{
		//Create the pcikced Element.
		EditElementHandle eehPicked;
		mdlDialog_dmsgsPrint(L"_OnDataButton ICounter > 1");
		ShipLockRoomP7Data::Create(eehPicked, &m_dataPick);

		DPoint3d dpOffNew = ShipLockRoomP7Data::CaculateOff(eehPicked);
		DPoint3d dpOff2Replaced = ShipLockRoomP7Data::ReadECOffPoint(baseInstancePtr);
		DVec3d vecOff;
		vecOff.DifferenceOf(dpOffNew, dpOff2Replaced);
		double uorparameter = (*ACTIVEMODEL).GetModelInfoCP()->GetUorPerMeter();
		vecOff = vecOff * uorparameter;
		Transform tranOff = Transform::From(vecOff);

		Transform tran2Origin;
		PDI_MDL_Common::stEleOperHelper::GetTransform2Origin(
			eehPicked, tran2Origin);

		DisplayHandler* pdh = eeh.GetDisplayHandler();
		Transform tr2Replace;
		pdh->GetBasisTransform(eeh, tr2Replace);

		Transform tr2Dest = tranOff * tr2Replace * tran2Origin;
		//Transform tr2Dest = tr2Replace * tran2Origin;
		eehPicked.GetHandler().ApplyTransform(eehPicked, TransformInfo(tr2Dest));

		eehPicked.ReplaceInModel(erp);
		//eeh.Invalidate();
		//eehPicked.AddToModel();
		DgnFileP pDf = ISessionMgr::GetActiveDgnFile();
		DgnECInstanceEnablerPtr ecMainInstanceEnabler =
			ecMgr.ObtainInstanceEnabler(*eclass, *pDf);
		StandaloneECInstanceR wipInstance =
			ecMainInstanceEnabler->GetSharedWipInstance();
		ShipLockRoomP7Data::SetECInstWithData(wipInstance, m_dataPick);
		ShipLockRoomP7Data::SetECOffPoint(wipInstance, eehPicked);
		DgnElementECInstancePtr outECInst;
		DgnECInstanceStatus ecRtl =
			ecMainInstanceEnabler->CreateInstanceOnElement(
				&outECInst, wipInstance, eehPicked);

		bool rtl = ECHelper::Data2EC_AddEC2Ele
			<ShipLockRoomP7Data>
			(L"ShipLockRoomP7", m_ec, 
				ISessionMgr::GetActiveDgnFile(), 
				eehPicked, m_dataPick,
				ShipLockRoomP7Data::SetECInstWithData);
		if (false == rtl)
		{
			mdlDialog_dmsgsPrint(L"PickTool _OnDataButton EC AddInstance False.");
		}
	}

	return false;
}

bool PickTool::_OnResetButton(DgnButtonEventCR ev)
{
	mdlDialog_dmsgsPrint(L"_OnResetButton");
	m_iCounter = 0;
	return false;
}

StatusInt PickTool::_OnElementModify(EditElementHandleR element)
{
	return false;
}

void PickTool::_OnRestartTool()
{
}
