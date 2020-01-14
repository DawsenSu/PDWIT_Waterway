#include "GuidewallP2.h"

bool PDIWT::Waterway::Lock::GuideWallP2Data::ValidateParameters()
{
	if (
		//m_dolphinTopElevation <= 0
		//|| m_dolphinBottomElevation <= 0
		m_dolphinTopLength <= 0
		|| m_dolphinTopWidth <= 0
		|| m_dolphinBottomLength <= 0
		|| m_dolphinBottomWidth <= 0
		//|| m_cushioncapBottomElevation <= 0
		|| m_cushioncapFrontToeLength <= 0
		|| m_cushioncapRearToeLength <= 0
		|| m_cushioncapExtraSideLength <= 0
		|| m_cushioncapChamferLength <= 0
		//|| m_pileTopElevation <= 0
		//|| m_pileBottomElevation <= 0
		|| m_pileDiameter <= 0
		|| m_pilewallThickness <= 0
		//|| m_pileOffsetAlongRiver <= 0
		|| m_pileOffsetPerpendicularRiver <= 0
		|| m_cushionThickness <= 0
		//|| m_wallTopElevation <= 0
		//|| m_wallBottomElevation <= 0
		|| m_wallThickness <= 0
		|| m_wallLength <= 0
		|| m_bridgeLength <= 0
		|| m_bridgeHeight <= 0
		|| m_bridgeWidth <= 0
		|| m_bridgeTopBoardThickness <= 0
		|| m_bridgeSupportWidth <= 0
		|| m_bridgeChamferWidth <= 0)
		return false;
	double cushioncapHalfBottomWidth = (m_cushioncapRearToeLength + m_dolphinBottomWidth + m_cushioncapFrontToeLength) / 2;
	if (m_dolphinBottomElevation >= m_dolphinTopElevation
		|| m_cushioncapBottomElevation >= m_dolphinBottomElevation
		|| m_pileTopElevation >= m_dolphinBottomElevation
		|| m_pileTopElevation <= m_cushioncapBottomElevation
		|| m_pileBottomElevation >= m_pileTopElevation
		|| m_pileOffsetPerpendicularRiver <= m_pileDiameter / 2
		|| m_pileOffsetPerpendicularRiver >= cushioncapHalfBottomWidth - m_pileDiameter / 2)
		return false;
	return true;
}

BentleyStatus PDIWT::Waterway::Lock::GuidewallP2::InitializeComponents()
{
	if (!m_data.ValidateParameters())
		return ERROR;

	m_dolphin = DolphinColumnP2(m_data.GetdolphinTopLength(), m_data.GetdolphinTopWidth(), m_data.GetdolphinBottomLength(), m_data.GetdolphinBottomWidth(),
		m_data.GetdolphinTopElevation() - m_data.GetdolphinBottomElevation() - bridgePavementThickness - m_data.GetbridgeHeight() - gapWidth / 2,			 //height
		m_data.GetwallThickness() + gapWidth,																		 //subtract wall thickness
		m_data.GetdolphinTopElevation() - m_data.GetwallBottomElevation() - m_data.GetbridgeHeight() + gapWidth);	 // subtract wall height

	m_cushioncap = Cushioncap(m_data.GetcushioncapFrontToeLength() + m_data.GetdolphinBottomWidth() + m_data.GetcushioncapRearToeLength() - m_data.GetcushioncapChamferLength(),
		m_data.GetcushioncapFrontToeLength() + m_data.GetdolphinBottomWidth() + m_data.GetcushioncapRearToeLength(),
		m_data.GetdolphinBottomElevation() - m_data.GetcushioncapBottomElevation(),
		m_data.GetdolphinBottomLength() + 2 * m_data.GetcushioncapExtraSideLength());

	m_pile = Pile(m_data.GetpileTopElevation() - m_data.GetpileBottomElevation(), m_data.GetpileDiameter(), m_data.GetpilewallThickness());

	m_cushion = Cushion(m_data.GetdolphinBottomLength() + 2 * m_data.GetcushioncapExtraSideLength() + 2 * cushionExtraLength,
		m_data.GetcushioncapFrontToeLength() + m_data.GetdolphinBottomWidth() + m_data.GetcushioncapRearToeLength() + 2 * cushionExtraLength,
		m_data.GetcushionThickness());

	m_wall = Wall(m_data.GetwallLength(), m_data.GetdolphinTopElevation() - m_data.GetwallBottomElevation(), m_data.GetwallThickness());

	m_bridge = Bridge(m_data.GetbridgeLength(), m_data.GetbridgeWidth(), m_data.GetbridgeHeight(), m_data.GetbridgeTopBoardThickness(),
		m_data.GetbridgeSupportWidth(), m_data.GetbridgeChamferWidth());

	m_bridgeCushion = Cushion(m_data.GetbridgeLength(), 2 * m_data.GetbridgeWidth() + gapWidth, bridgePavementThickness);

	return SUCCESS;
}

BentleyStatus PDIWT::Waterway::Lock::GuidewallP2::CreateGuidewallP2(EditElementHandleR eeh, DgnModelRefR model)
{
	if (SUCCESS != InitializeComponents())
		return ERROR;

	ElementAgenda elems;

	double uorpermeter = model.GetModelInfoCP()->GetUorPerMeter();
	//Dolphin
	EditElementHandle dolphinEeh;
	if (SUCCESS != m_dolphin.CrerateDolphin(dolphinEeh, model))
		return ERROR;
	elems.Insert(dolphinEeh);

	//cushioncap
	EditElementHandle cushioncapEeh;
	if (SUCCESS != m_cushioncap.CreateCushioncap(cushioncapEeh, model))
		return ERROR;

	DVec3d cushioncapDVec = DVec3d::From(0, -m_data.GetcushioncapFrontToeLength(), m_data.GetcushioncapBottomElevation() - m_data.GetdolphinBottomElevation());
	cushioncapDVec.Scale(uorpermeter);
	Transform cushioncapTrans = Transform::From(cushioncapDVec);
	if (SUCCESS != cushioncapEeh.GetHandler().ApplyTransform(cushioncapEeh, TransformInfo(cushioncapTrans)))
		return ERROR;
	elems.Insert(cushioncapEeh);

	//pile
	EditElementHandle pileFrontEeh;
	if (SUCCESS != m_pile.CreatePile(pileFrontEeh, model))
		return ERROR;
	DVec3d pileRelateToCushionCapDVec = DVec3d::From(0, (m_data.GetcushioncapFrontToeLength() + m_data.GetdolphinBottomWidth() + m_data.GetcushioncapRearToeLength()) / 2 - m_data.GetpileOffsetPerpendicularRiver(),
		m_data.GetpileTopElevation() - m_data.GetcushioncapBottomElevation());
	pileRelateToCushionCapDVec.Scale(uorpermeter);
	if (SUCCESS != pileFrontEeh.GetHandler().ApplyTransform(pileFrontEeh, TransformInfo(Transform::From(cushioncapDVec + pileRelateToCushionCapDVec))))
		return ERROR;
	elems.Insert(pileFrontEeh);

	EditElementHandle pileRearEeh;
	if (SUCCESS != m_pile.CreatePile(pileRearEeh, model))
		return ERROR;
	DVec3d pileRelateToFront = DVec3d::From(0, 2 * m_data.GetpileOffsetPerpendicularRiver(), 0);
	pileRelateToFront.Scale(uorpermeter);
	if (SUCCESS != pileRearEeh.GetHandler().ApplyTransform(pileRearEeh, TransformInfo(Transform::From(cushioncapDVec + pileRelateToCushionCapDVec + pileRelateToFront))))
		return ERROR;
	elems.Insert(pileRearEeh);

	//cushion
	EditElementHandle cushionEeh;
	if (SUCCESS != m_cushion.CreateCushion(cushionEeh, model))
		return ERROR;
	DVec3d cushionRelateToCushionCapDVec = DVec3d::From(0, -cushionExtraLength, -m_data.GetcushionThickness());
	cushionRelateToCushionCapDVec.Scale(uorpermeter);
	if (SUCCESS != cushionEeh.GetHandler().ApplyTransform(cushionEeh, TransformInfo(Transform::From(cushioncapDVec + cushionRelateToCushionCapDVec))))
		return ERROR;
	elems.Insert(cushionEeh);


	//walls
	if (m_data.GethasRightWall())
	{
		EditElementHandle leftWallEeh;
		if (SUCCESS != m_wall.CreateWall(leftWallEeh, model))
			return ERROR;
		DVec3d wallDVec = DVec3d::From(gapWidth / 2 + m_data.GetwallLength() / 2, 0, m_data.GetwallBottomElevation() - m_data.GetdolphinBottomElevation() + gapWidth);
		wallDVec.Scale(uorpermeter);
		if (SUCCESS != leftWallEeh.GetHandler().ApplyTransform(leftWallEeh, TransformInfo(Transform::From(wallDVec))))
			return ERROR;
		elems.Insert(leftWallEeh);
	}
	if (m_data.GethasLeftWall())
	{
		EditElementHandle rightWallEeh;
		if (SUCCESS != m_wall.CreateWall(rightWallEeh, model))
			return ERROR;
		DVec3d wallDVec = DVec3d::From(-(gapWidth / 2 + m_data.GetwallLength() / 2), 0, m_data.GetwallBottomElevation() - m_data.GetdolphinBottomElevation() + gapWidth);
		wallDVec.Scale(uorpermeter);
		if (SUCCESS != rightWallEeh.GetHandler().ApplyTransform(rightWallEeh, TransformInfo(Transform::From(wallDVec))))
			return ERROR;
		elems.Insert(rightWallEeh);
	}

	//Bridges
	if (m_data.GethasRightWall())
	{
		EditElementHandle bridgeEehFront, bridgeEehRear;
		if (SUCCESS != m_bridge.CreateBridge(bridgeEehFront, model))
			return ERROR;
		if (SUCCESS != m_bridge.CreateBridge(bridgeEehRear, model))
			return ERROR;
		DVec3d frontBridgeVec = DVec3d::From(gapWidth / 2, m_data.GetwallThickness() + gapWidth, m_data.GetdolphinTopElevation() - m_data.GetdolphinBottomElevation() - bridgePavementThickness - m_data.GetbridgeHeight() + gapWidth);
		frontBridgeVec.Scale(uorpermeter);
		if (SUCCESS != bridgeEehFront.GetHandler().ApplyTransform(bridgeEehFront, TransformInfo(Transform::From(frontBridgeVec))))
			return ERROR;
		elems.Insert(bridgeEehFront);
		DVec3d rearBridgeVec = DVec3d::From(0, m_data.GetbridgeWidth() + gapWidth);
		rearBridgeVec.Scale(uorpermeter);
		if (SUCCESS != bridgeEehRear.GetHandler().ApplyTransform(bridgeEehRear, TransformInfo(Transform::From(frontBridgeVec + rearBridgeVec))))
			return ERROR;
		elems.Insert(bridgeEehRear);
	}
	if (m_data.GethasLeftWall())
	{
		Transform mirrorTrans;
		mirrorTrans.InitFromMirrorPlane(DPoint3d(), DVec3d::UnitX());
		EditElementHandle bridgeEehFront, bridgeEehRear;
		if (SUCCESS != m_bridge.CreateBridge(bridgeEehFront, model))
			return ERROR;
		if (SUCCESS != m_bridge.CreateBridge(bridgeEehRear, model))
			return ERROR;
		DVec3d frontBridgeVec = DVec3d::From(gapWidth / 2, m_data.GetwallThickness() + gapWidth, m_data.GetdolphinTopElevation() - m_data.GetdolphinBottomElevation() - bridgePavementThickness - m_data.GetbridgeHeight() + gapWidth);
		frontBridgeVec.Scale(uorpermeter);
		if (SUCCESS != bridgeEehFront.GetHandler().ApplyTransform(bridgeEehFront, TransformInfo(mirrorTrans * Transform::From(frontBridgeVec))))
			return ERROR;
		elems.Insert(bridgeEehFront);
		DVec3d rearBridgeVec = DVec3d::From(0, m_data.GetbridgeWidth() + gapWidth);
		rearBridgeVec.Scale(uorpermeter);
		if (SUCCESS != bridgeEehRear.GetHandler().ApplyTransform(bridgeEehRear, TransformInfo(mirrorTrans * Transform::From(frontBridgeVec + rearBridgeVec))))
			return ERROR;
		elems.Insert(bridgeEehRear);
	}

	//bridge Cushion
	if (m_data.GethasRightWall())
	{
		EditElementHandle bridgeCushionEeh;
		if (SUCCESS != m_bridgeCushion.CreateCushion(bridgeCushionEeh, model))
			return ERROR;
		DVec3d bridgeCushionVec = DVec3d::From(m_data.GetbridgeLength() / 2 + gapWidth / 2, m_data.GetwallThickness() + gapWidth, m_data.GetdolphinTopElevation() - m_data.GetdolphinBottomElevation() - bridgePavementThickness + gapWidth);
		bridgeCushionVec.Scale(uorpermeter);
		if (SUCCESS != bridgeCushionEeh.GetHandler().ApplyTransform(bridgeCushionEeh, TransformInfo(Transform::From(bridgeCushionVec))))
			return ERROR;
		elems.Insert(bridgeCushionEeh);
	}
	if (m_data.GethasLeftWall())
	{
		Transform mirrorTrans;
		mirrorTrans.InitFromMirrorPlane(DPoint3d(), DVec3d::UnitX());

		EditElementHandle bridgeCushionEeh;
		if (SUCCESS != m_bridgeCushion.CreateCushion(bridgeCushionEeh, model))
			return ERROR;

		DVec3d bridgeCushionVec = DVec3d::From(m_data.GetbridgeLength() / 2 + gapWidth / 2, m_data.GetwallThickness() + gapWidth, m_data.GetdolphinTopElevation() - m_data.GetdolphinBottomElevation() - bridgePavementThickness + gapWidth);
		bridgeCushionVec.Scale(uorpermeter);
		if (SUCCESS != bridgeCushionEeh.GetHandler().ApplyTransform(bridgeCushionEeh, TransformInfo(mirrorTrans * Transform::From(bridgeCushionVec))))
			return ERROR;
		elems.Insert(bridgeCushionEeh);
	}

	//cell Header
	if (SUCCESS != NormalCellHeaderHandler::CreateGroupCellElement(eeh, elems, L"guidewallp2"))
		return ERROR;
	return SUCCESS;
}
