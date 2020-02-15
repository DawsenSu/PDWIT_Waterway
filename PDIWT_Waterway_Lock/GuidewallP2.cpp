#include "GuidewallP2.h"

PDIWT_WATERWAY_LOCK_NAMESPACE_BEGIN
/************************************************************************/
/* Static member Initialization
/* Author: DawsenSu						2020/01/17
/************************************************************************/
const double GuidewallP2::gapWidth = 0.02;
const double GuidewallP2::cushionExtraLength = 0.1;
const double GuidewallP2::bridgePavementThickness = 0.06;
const WString GuidewallP2::componentName = L"GuideWallP2";

/************************************************************************/
/* The implement of Other methods
/* Author: DawsenSu						2020/01/20
/************************************************************************/
PDIWT::Waterway::Lock::GuidewallP2::GuidewallP2(UI::Model::GuidewallP2Data^ data)
{
	m_data = data;
}

BentleyStatus GuidewallP2::InitializeComponents()
{
	if (!m_data->ValidateParameters())
		return ERROR;

	m_dolphin = DolphinColumnP2(m_data->DolphinTopLength, m_data->DolphinTopWidth, m_data->DolphinBottomLength, m_data->DolphinBottomWidth,
		m_data->DolphinTopElevation - m_data->DolphinBottomElevation - bridgePavementThickness - m_data->BridgeHeight - gapWidth / 2,			 //height
		m_data->WallThickness + gapWidth,																		 //subtract wall thickness
		m_data->DolphinTopElevation - m_data->WallBottomElevation + gapWidth - bridgePavementThickness - m_data->BridgeHeight - gapWidth / 2);	 // subtract wall height

	m_cushioncap = Cushioncap(m_data->CushioncapFrontToeLength + m_data->DolphinBottomWidth + m_data->CushioncapRearToeLength - m_data->CushioncapChamferLength,
		m_data->CushioncapFrontToeLength + m_data->DolphinBottomWidth + m_data->CushioncapRearToeLength,
		m_data->DolphinBottomElevation - m_data->CushioncapBottomElevation,
		m_data->DolphinBottomLength + 2 * m_data->CushioncapExtraSideLength);

	m_pile = Pile(m_data->PileTopElevation - m_data->PileBottomElevation, m_data->PileDiameter, m_data->PileWallThickness);

	m_cushion = Cushion(m_data->DolphinBottomLength + 2 * m_data->CushioncapExtraSideLength + 2 * cushionExtraLength,
		m_data->CushioncapFrontToeLength + m_data->DolphinBottomWidth + m_data->CushioncapRearToeLength + 2 * cushionExtraLength,
		m_data->CushionThickness);

	m_wall = Wall(m_data->WallLength, m_data->DolphinTopElevation - m_data->WallBottomElevation, m_data->WallThickness);

	m_bridge = Bridge(m_data->BridgeLength, m_data->BridgeWidth, m_data->BridgeHeight, m_data->BridgeTopBoardThickness,
		m_data->BridgeSupportWidth, m_data->BridgeChamferWidth);

	m_bridgeCushion = Cushion(m_data->BridgeLength, 2 * m_data->BridgeWidth + gapWidth, bridgePavementThickness);

	return SUCCESS;
}

BentleyStatus GuidewallP2::CreateGuidewallP2(EditElementHandleR eeh, DgnModelRefR model, bool isForDynamic)
{
	// prepare for creation
	if (SUCCESS != InitializeComponents())
		return ERROR;

	ElementAgenda _elems;
	double uorpermeter = model.GetModelInfoCP()->GetUorPerMeter();
	const bvector<WString> _subElementNames{ L"DolphinColumnP2",L"Cushioncap",L"Pile",L"Cushion",L"Wall",L"Bridge",L"BridgeCushion" };

	//Dolphin
	EditElementHandle dolphinEeh;
	if (SUCCESS != m_dolphin.CrerateDolphin(dolphinEeh, model))
		return ERROR;
	if (SUCCESS != ECHelper::SchedulePDIWTBaseECInstanceOnElement(dolphinEeh, L"DolphinColumnP2", L"")) return ERROR;
	_elems.Insert(dolphinEeh); //This operation will invalidate the inserted EditElementHandle, if needed, use the return value instead.

	//cushioncap
	EditElementHandle cushioncapEeh;
	if (SUCCESS != m_cushioncap.CreateCushioncap(cushioncapEeh, model))
		return ERROR;

	DVec3d cushioncapDVec = DVec3d::From(0, -m_data->CushioncapFrontToeLength, m_data->CushioncapBottomElevation - m_data->DolphinBottomElevation);
	cushioncapDVec.Scale(uorpermeter);
	Transform cushioncapTrans = Transform::From(cushioncapDVec);
	if (SUCCESS != cushioncapEeh.GetHandler().ApplyTransform(cushioncapEeh, TransformInfo(cushioncapTrans)))
		return ERROR;
	if (SUCCESS != ECHelper::SchedulePDIWTBaseECInstanceOnElement(cushioncapEeh, L"Cushioncap", L"")) return ERROR;

	_elems.Insert(cushioncapEeh);
	//pile
	EditElementHandle pileFrontEeh;
	if (SUCCESS != m_pile.CreatePile(pileFrontEeh, model))
		return ERROR;
	DVec3d pileRelateToCushionCapDVec = DVec3d::From(0, (m_data->CushioncapFrontToeLength + m_data->DolphinBottomWidth + m_data->CushioncapRearToeLength) / 2 - m_data->PileOffsetPerpendicularRiver,
		m_data->PileTopElevation - m_data->CushioncapBottomElevation);
	pileRelateToCushionCapDVec.Scale(uorpermeter);
	if (SUCCESS != pileFrontEeh.GetHandler().ApplyTransform(pileFrontEeh, TransformInfo(Transform::From(cushioncapDVec + pileRelateToCushionCapDVec))))
		return ERROR;

	EditElementHandle pileRearEeh;
	if (SUCCESS != m_pile.CreatePile(pileRearEeh, model))
		return ERROR;
	DVec3d pileRelateToFront = DVec3d::From(0, 2 * m_data->PileOffsetPerpendicularRiver, 0);
	pileRelateToFront.Scale(uorpermeter);
	if (SUCCESS != pileRearEeh.GetHandler().ApplyTransform(pileRearEeh, TransformInfo(Transform::From(cushioncapDVec + pileRelateToCushionCapDVec + pileRelateToFront))))
		return ERROR;
	if (SUCCESS != ECHelper::SchedulePDIWTBaseECInstanceOnElement(pileFrontEeh, L"Pile", L"")) return ERROR;
	if (SUCCESS != ECHelper::SchedulePDIWTBaseECInstanceOnElement(pileRearEeh, L"Pile", L"")) return ERROR;
	_elems.Insert(pileFrontEeh);
	_elems.Insert(pileRearEeh);

	//cushion
	EditElementHandle cushionEeh;
	if (SUCCESS != m_cushion.CreateCushion(cushionEeh, model))
		return ERROR;
	DVec3d cushionRelateToCushionCapDVec = DVec3d::From(0, -cushionExtraLength, -m_data->CushionThickness);
	cushionRelateToCushionCapDVec.Scale(uorpermeter);
	if (SUCCESS != cushionEeh.GetHandler().ApplyTransform(cushionEeh, TransformInfo(Transform::From(cushioncapDVec + cushionRelateToCushionCapDVec))))
		return ERROR;
	if (SUCCESS != ECHelper::SchedulePDIWTBaseECInstanceOnElement(cushionEeh, L"Cushion", L"")) return ERROR;
	_elems.Insert(cushionEeh);

	//walls
	if (m_data->HasRightWall)
	{
		EditElementHandle rightWallEeh;
		if (SUCCESS != m_wall.CreateWall(rightWallEeh, model))
			return ERROR;
		DVec3d wallDVec = DVec3d::From(gapWidth / 2 + m_data->WallLength / 2, 0, m_data->WallBottomElevation - m_data->DolphinBottomElevation);
		wallDVec.Scale(uorpermeter);
		if (SUCCESS != rightWallEeh.GetHandler().ApplyTransform(rightWallEeh, TransformInfo(Transform::From(wallDVec))))
			return ERROR;
		if (SUCCESS != ECHelper::SchedulePDIWTBaseECInstanceOnElement(rightWallEeh, L"Wall", L"")) return ERROR;
		_elems.Insert(rightWallEeh);
	}
	if (m_data->HasLeftWall)
	{
		EditElementHandle leftWallEeh;
		if (SUCCESS != m_wall.CreateWall(leftWallEeh, model))
			return ERROR;
		DVec3d wallDVec = DVec3d::From(-(gapWidth / 2 + m_data->WallLength / 2), 0, m_data->WallBottomElevation - m_data->DolphinBottomElevation);
		wallDVec.Scale(uorpermeter);
		if (SUCCESS != leftWallEeh.GetHandler().ApplyTransform(leftWallEeh, TransformInfo(Transform::From(wallDVec))))
			return ERROR;
		if (SUCCESS != ECHelper::SchedulePDIWTBaseECInstanceOnElement(leftWallEeh, L"Wall", L"")) return ERROR;
		_elems.Insert(leftWallEeh);
	}

	//Bridges
	if (m_data->HasRightWall)
	{
		EditElementHandle bridgeEehFront, bridgeEehRear;
		if (SUCCESS != m_bridge.CreateBridge(bridgeEehFront, model))
			return ERROR;
		if (SUCCESS != m_bridge.CreateBridge(bridgeEehRear, model))
			return ERROR;
		DVec3d frontBridgeVec = DVec3d::From(gapWidth / 2, m_data->WallThickness + gapWidth, m_data->DolphinTopElevation - m_data->DolphinBottomElevation - bridgePavementThickness - m_data->BridgeHeight);
		frontBridgeVec.Scale(uorpermeter);
		if (SUCCESS != bridgeEehFront.GetHandler().ApplyTransform(bridgeEehFront, TransformInfo(Transform::From(frontBridgeVec))))
			return ERROR;
		DVec3d rearBridgeVec = DVec3d::From(0, m_data->BridgeWidth + gapWidth);
		rearBridgeVec.Scale(uorpermeter);
		if (SUCCESS != bridgeEehRear.GetHandler().ApplyTransform(bridgeEehRear, TransformInfo(Transform::From(frontBridgeVec + rearBridgeVec))))
			return ERROR;
		if (SUCCESS != ECHelper::SchedulePDIWTBaseECInstanceOnElement(bridgeEehFront, L"Bridge", L"")) return ERROR;
		if (SUCCESS != ECHelper::SchedulePDIWTBaseECInstanceOnElement(bridgeEehRear, L"Bridge", L"")) return ERROR;
		_elems.Insert(bridgeEehFront);
		_elems.Insert(bridgeEehRear);
	}
	if (m_data->HasLeftWall)
	{
		Transform mirrorTrans;
		mirrorTrans.InitFromMirrorPlane(DPoint3d(), DVec3d::UnitX());
		EditElementHandle bridgeEehFront, bridgeEehRear;
		if (SUCCESS != m_bridge.CreateBridge(bridgeEehFront, model))
			return ERROR;
		if (SUCCESS != m_bridge.CreateBridge(bridgeEehRear, model))
			return ERROR;
		DVec3d frontBridgeVec = DVec3d::From(gapWidth / 2, m_data->WallThickness + gapWidth, m_data->DolphinTopElevation - m_data->DolphinBottomElevation - bridgePavementThickness - m_data->BridgeHeight);
		frontBridgeVec.Scale(uorpermeter);
		if (SUCCESS != bridgeEehFront.GetHandler().ApplyTransform(bridgeEehFront, TransformInfo(mirrorTrans * Transform::From(frontBridgeVec))))
			return ERROR;
		DVec3d rearBridgeVec = DVec3d::From(0, m_data->BridgeWidth + gapWidth);
		rearBridgeVec.Scale(uorpermeter);
		if (SUCCESS != bridgeEehRear.GetHandler().ApplyTransform(bridgeEehRear, TransformInfo(mirrorTrans * Transform::From(frontBridgeVec + rearBridgeVec))))
			return ERROR;
		if (SUCCESS != ECHelper::SchedulePDIWTBaseECInstanceOnElement(bridgeEehFront, L"Bridge", L"")) return ERROR;
		if (SUCCESS != ECHelper::SchedulePDIWTBaseECInstanceOnElement(bridgeEehRear, L"Bridge", L"")) return ERROR;
		_elems.Insert(bridgeEehFront);
		_elems.Insert(bridgeEehRear);
	}

	//bridge Cushion
	if (m_data->HasRightWall)
	{
		EditElementHandle bridgeCushionEeh;
		if (SUCCESS != m_bridgeCushion.CreateCushion(bridgeCushionEeh, model))
			return ERROR;
		DVec3d bridgeCushionVec = DVec3d::From(m_data->BridgeLength / 2 + gapWidth / 2, m_data->WallThickness + gapWidth, m_data->DolphinTopElevation - m_data->DolphinBottomElevation - bridgePavementThickness);
		bridgeCushionVec.Scale(uorpermeter);
		if (SUCCESS != bridgeCushionEeh.GetHandler().ApplyTransform(bridgeCushionEeh, TransformInfo(Transform::From(bridgeCushionVec))))
			return ERROR;
		if (SUCCESS != ECHelper::SchedulePDIWTBaseECInstanceOnElement(bridgeCushionEeh, L"BridgeCushion", L"")) return ERROR;
		_elems.Insert(bridgeCushionEeh);
	}
	if (m_data->HasLeftWall)
	{
		Transform mirrorTrans;
		mirrorTrans.InitFromMirrorPlane(DPoint3d(), DVec3d::UnitX());

		EditElementHandle bridgeCushionEeh;
		if (SUCCESS != m_bridgeCushion.CreateCushion(bridgeCushionEeh, model))
			return ERROR;

		DVec3d bridgeCushionVec = DVec3d::From(m_data->BridgeLength / 2 + gapWidth / 2, m_data->WallThickness + gapWidth, m_data->DolphinTopElevation - m_data->DolphinBottomElevation - bridgePavementThickness);
		bridgeCushionVec.Scale(uorpermeter);
		if (SUCCESS != bridgeCushionEeh.GetHandler().ApplyTransform(bridgeCushionEeh, TransformInfo(mirrorTrans * Transform::From(bridgeCushionVec))))
			return ERROR;
		if (SUCCESS != ECHelper::SchedulePDIWTBaseECInstanceOnElement(bridgeCushionEeh, L"BridgeCushion", L"")) return ERROR;
		_elems.Insert(bridgeCushionEeh);
	}

	//cell Header
	if (SUCCESS != NormalCellHeaderHandler::CreateGroupCellElement(eeh, _elems, L"guidewallp2"))
		return ERROR;

	// if it's not for dynamic purpose, attach its ECClass;
	if (!isForDynamic)
	{
		DgnFileR _dgnFile = *model.GetDgnFileP();

		DgnECManagerR _ecMgr = DgnECManager::GetManager();
		const WString _ecSchemaFullName(L"PDWT_Waterway.01.00.ecschema.xml");
		const WString _ecClassName(L"GuidewallP2");
		const WString _definedECSchemaFilePathVariable(L"PDIWT_ORGANIZATION_ECSCHEMAPATH");

		SchemaKey _schemakey;
		if (SUCCESS != SchemaKey::ParseSchemaFullName(_schemakey, _ecSchemaFullName.GetWCharCP())) return ERROR;
		SchemaInfo _schemaInfo(_schemakey, _dgnFile);
		//Use the ECHelper Method
		if (SUCCESS != ECHelper::ImportECSChemaInActiveDgnBasedOnDefinedVariable(_ecSchemaFullName, _definedECSchemaFilePathVariable)) return ERROR;

		DgnECInstanceEnablerPtr _ecMainInstanceEnabler = _ecMgr.ObtainInstanceEnablerByName(_schemaInfo.GetSchemaName(), _ecClassName.GetWCharCP(), _dgnFile);
		StandaloneECInstanceR _wipInstance = _ecMainInstanceEnabler->GetSharedWipInstance();

		//! Base part assignment
		_wipInstance.SetValue(L"Name", ECValue(_ecClassName.GetWCharCP()));
		//! Parameters part assignment
		if (SUCCESS != ECHelper::SetECPropertyValuesWithManagedObject(_wipInstance, m_data)) return ERROR;
		//! Volume part assignment
		//bmap<WString, double>	_volumeMap;
		//for each (auto subEeh in _elems)
		//{
		//	WString _subEehName, _subEehCode;
		//	if (SUCCESS != ECHelper::GetPDIWTBaseNameAndCode(subEeh, _subEehName, _subEehName)) continue;
		//	double _volume;
		//	if (SUCCESS != mdlMeasure_volumeProperties(&_volume, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, subEeh.GetElementDescrP(), 0)) continue;

		//	if (_volumeMap.end() == _volumeMap.find(_subEehName))
		//	{
		//		_volumeMap.Insert(_subEehName, _volume);
		//	}
		//	else
		//	{
		//		_volumeMap[_subEehName] += _volume;
		//	}
		//}
		//_wipInstance.SetValue(L"BridgesCushionVolume", ECValue(_volumeMap[L"BridgeCushion"]));
		//_wipInstance.SetValue(L"BridgesVolume", ECValue(_volumeMap[L"Bridge"]));
		//_wipInstance.SetValue(L"WallVolum", ECValue(_volumeMap[L"Wall"]));
		//_wipInstance.SetValue(L"CushionVolume", ECValue(_volumeMap[L"Cushion"]));
		//_wipInstance.SetValue(L"PilesVolume", ECValue(_volumeMap[L"Pile"]));
		//_wipInstance.SetValue(L"CushioncapVolume", ECValue(_volumeMap[L"Cushioncap"]));
		//_wipInstance.SetValue(L"DolphinVolume", ECValue(_volumeMap[L"DolphinColumnP2"]));

		if (!_ecMainInstanceEnabler->SupportsCreateInstanceOnElement())
			return ERROR;
		if (DgnECInstanceStatus::DGNECINSTANCESTATUS_Success != _ecMainInstanceEnabler->ScheduleInstanceOnElement(nullptr, _wipInstance, eeh))
			return ERROR;
	}

	return SUCCESS;
}

PDIWT_WATERWAY_LOCK_NAMESPACE_END