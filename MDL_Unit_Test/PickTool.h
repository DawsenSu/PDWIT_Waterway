#pragma once

#include "stdafx.h"
#include "ShipLockRoomP7Data.h"

USING_NAMESPACE_BENTLEY_DGNPLATFORM;
USING_NAMESPACE_BENTLEY_MSTNPLATFORM;
USING_NAMESPACE_BENTLEY_MSTNPLATFORM_ELEMENT;
USING_NAMESPACE_BENTLEY_ECOBJECT

class PickTool : DgnElementSetTool
{
private:
	ShipLockRoomP7Data m_dataPick;
	//DPoint3d m_offPick;
	//Transform m_tr;
	//EditElementHandle m_eeh;
	int m_iCounter = 0;
	ECSchemaPtr& m_ec;
	WCharCP m_strECName = L"ShipLockRoomP7";
public:
	static void InstallNewInstance(int toolId, ECSchemaPtr& ec);
	PickTool(int toolId, ECSchemaPtr& ec);
	~PickTool();
	virtual bool _OnDataButton(DgnButtonEventCR ev) override;
	virtual bool _OnResetButton(DgnButtonEventCR ev) override;//override right button.
	virtual StatusInt _OnElementModify(EditElementHandleR element) override;
	virtual void _OnRestartTool() override;
};

