#include "GuidewallP2.h"

bool PDIWT::Waterway::Lock::GuideWallP2Data::ValidateParameters()
{
	if (m_dolphinTopElevation <= 0
		//|| m_dolphinBottomElevation <= 0
		|| m_dolphinTopLength <= 0
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
		|| m_pileOffsetAlongRiver <= 0
		|| m_pileOffsetPerpendicularRiver <= 0
		|| m_cushionThickness <= 0
		//|| m_wallTopElevation <= 0
		//|| m_wallBottomElevation <= 0
		|| m_wallThickness <= 0
		|| m_wallLength <= 0)
		return false;
	if (m_dolphinBottomElevation >= m_dolphinTopElevation
		|| m_cushioncapBottomElevation >= m_dolphinBottomElevation
		|| m_pileTopElevation >= m_dolphinBottomElevation
		|| m_pileTopElevation <= m_cushioncapBottomElevation
		|| m_pileBottomElevation >= m_pileTopElevation)
		return false;
	return true;
}

BentleyStatus PDIWT::Waterway::Lock::GuidewallP2::InitializeComponents()
{
	//if (!m_data.ValidateParameters())
	//	return ERROR;

	m_dolphin = DolphinColumnP2(m_data.GetdolphinTopLength(), m_data.GetdolphinTopWidth(),m_data.GetdolphinBottomLength(), m_data.GetdolphinBottomWidth(), 
		m_data.GetdolphinTopElevation() - m_data.GetdolphinBottomElevation() - bridgeHeigth, 
		m_data.GetwallThickness() + gapWidth, 
		m_data.GetdolphinTopElevation() - m_data.GetwallBottomElevation() - bridgeHeigth + gapWidth);


	m_cushioncap = Cushioncap(m_data.GetcushioncapFrontToeLength() + m_data.GetdolphinBottomWidth() +m_data.GetcushioncapRearToeLength(), 
		m_data.GetcushioncapFrontToeLength() + m_data.GetdolphinBottomWidth() + m_data.GetcushioncapRearToeLength() + m_data.GetcushioncapChamferLength(), 
		m_data.GetdolphinBottomElevation() - m_data.GetcushioncapBottomElevation(), 
		m_data.GetdolphinBottomLength() + 2 * m_data.GetcushioncapExtraSideLength());

	Pile pile = Pile(m_data.GetpileTopElevation() - m_data.GetpileBottomElevation(), m_data.GetpileDiameter(), m_data.GetpilewallThickness());
	m_piles.push_back(pile);
	m_piles.push_back(Pile(pile));
	m_piles.push_back(Pile(pile));
	m_piles.push_back(Pile(pile));


	m_cushion = Cushion(m_data.GetdolphinBottomLength() + 2 * m_data.GetcushioncapExtraSideLength() + 2 * cushionExtraLength,
		m_data.GetcushioncapFrontToeLength() + m_data.GetdolphinBottomWidth() + m_data.GetcushioncapRearToeLength() + m_data.GetcushioncapChamferLength() + 2 * cushionExtraLength,
		m_data.GetcushionThickness());

	Wall wall = Wall(m_data.GetwallLength(), m_data.GetdolphinTopElevation() - m_data.GetwallBottomElevation(), m_data.GetpilewallThickness());
	if (m_data.GethasLeftWall())
	{		
		m_walls.push_back(Wall(wall));
	}
	if (m_data.GethasRightWall())
		m_walls.push_back(Wall(wall));
	return SUCCESS;
}

BentleyStatus PDIWT::Waterway::Lock::GuidewallP2::CreateGuidewallP2(EditElementHandleR eeh, DgnModelRefR model)
{
	if (SUCCESS != InitializeComponents())
		return ERROR;

	double uorpermeter = model.GetModelInfoCP()->GetUorPerMeter();
	//Dolphin
	EditElementHandle dolphinEeh;
	if (SUCCESS != m_dolphin.CrerateDolphin(dolphinEeh, model))
		return ERROR;

	//cushioncap
	EditElementHandle cushioncapEeh;
	if (SUCCESS != m_cushioncap.CreateCushioncap(cushioncapEeh, model))
		return ERROR;

	DVec3d cushioncapDVec = DVec3d::From(0, -(m_data.GetcushioncapFrontToeLength() + m_data.GetcushioncapChamferLength()), m_data.GetcushioncapBottomElevation() - m_data.GetdolphinBottomElevation());
	cushioncapDVec.Scale(uorpermeter);	
	Transform cushioncapTrans = Transform::From(cushioncapDVec);
	if (SUCCESS != cushioncapEeh.GetHandler().ApplyTransform(cushioncapEeh, TransformInfo(cushioncapTrans)))
		return ERROR;

	//cushion
	EditElementHandle cushionEeh;
	if (SUCCESS != m_cushion.CreateCushion(cushionEeh, model))
		return ERROR;
	DVec3d cushionDVecRelateToCushionCapDVec = DVec3d::From(0, -cushionExtraLength, -m_data.GetcushionThickness());
	cushionDVecRelateToCushionCapDVec.Scale(uorpermeter);
	if (SUCCESS != cushionEeh.GetHandler().ApplyTransform(cushionEeh, TransformInfo(Transform::From(cushioncapDVec + cushionDVecRelateToCushionCapDVec))))
		return ERROR;

	//pile


	//cell Header
	NormalCellHeaderHandler::CreateCellElement(eeh, L"CushionwallP2", DPoint3d::FromZero(), RotMatrix::FromIdentity(), true, model);
	NormalCellHeaderHandler::AddChildElement(eeh, dolphinEeh);
	NormalCellHeaderHandler::AddChildElement(eeh, cushioncapEeh);
	NormalCellHeaderHandler::AddChildElement(eeh, cushionEeh);
	NormalCellHeaderHandler::AddChildComplete(eeh);

	return SUCCESS;
}

PDIWT::Waterway::Lock::GuidewallP2::~GuidewallP2()
{

}
