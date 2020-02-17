#include "MountPierP1.h"
#include "EleOperHelper.h"

using namespace PDI_MDL_Common;

#pragma unmanaged

PDIWT::Waterway::Lock::MountPierData::MountPierData()
{
	SaftyHeight = 1.5;
	DolphinColumnTopElevation = 10;
	DolphinColumnBottomElevation = 0;
	DolphinColumnTopLength = 3.5;
	DolphinColumnTopWidth = 3.5;
	DolphinColumnTopEdgeRadius = 0.1;
	DolphinColumnSideEdgeRadius = 0.3;
	DolphinColumnBottomLength = 3.5;
	DolphinColumnBottomWidth = 5;
	DolphinColumnHaunchHeight = 1;
	DolphinColumnHaunchLength = 0.5;
	AngleOfFirstPolylineWall = 0;
	HeightOfFirstPolylineWall = 4;
	AngleOfSecondPolylineWall = 45;
	HeightOfSecondPolylineWall = 4;

	CushionCapTopElevation = 0;
	CushionCapBottomElevation = -1.5;
	FrontToeLength = 1;
	BackToeLength = 1;
	CushionCapExtraWideLength = 1;
	CapThickness = 0.1;

	PileDiameterOut = 1.0;
	PileWallThick = 0.1;
	PileSideDistH = 1.2;
	PileSideDistV = 1.2;
	PileTopElevation = CushionCapBottomElevation + 0.1;
	PileBottomElevation = PileTopElevation - 15.0;
}

//bool PDIWT::Waterway::Lock::MountPierData::ValidateParameters()
//{
//	return true;
//}

PDIWT::Waterway::Lock::MountPierP1::MountPierP1(const MountPierData& data) : 
	m_data(data) 
{

}


BentleyStatus PDIWT::Waterway::Lock::MountPierP1::InitializeComponents()
{
	//if (!m_data.ValidateParameters())
	//	return ERROR;

	m_dolphin = DolphinColumnP1_P11(
		m_data.SaftyHeight,
		m_data.DolphinColumnTopElevation,
		m_data.DolphinColumnBottomElevation,
		m_data.DolphinColumnTopLength,
		m_data.DolphinColumnTopWidth,
		m_data.DolphinColumnTopEdgeRadius,
		m_data.DolphinColumnSideEdgeRadius,
		m_data.DolphinColumnBottomLength,
		m_data.DolphinColumnBottomWidth,
		m_data.DolphinColumnHaunchHeight,
		m_data.DolphinColumnHaunchLength,
		m_data.AngleOfFirstPolylineWall,
		m_data.HeightOfFirstPolylineWall,
		m_data.AngleOfSecondPolylineWall,
		m_data.HeightOfSecondPolylineWall,
		m_data.CushionCapTopElevation
	);

	m_cushioncap = Cushioncap(m_data.FrontToeLength + m_data.DolphinColumnBottomWidth + m_data.BackToeLength,
		m_data.FrontToeLength + m_data.DolphinColumnBottomWidth + m_data.BackToeLength,
		m_data.DolphinColumnBottomElevation - m_data.CushionCapBottomElevation,
		m_data.DolphinColumnBottomLength + 2 * m_data.CushionCapExtraWideLength);

	m_cushion = Cushion(m_data.DolphinColumnBottomLength + 2 * m_data.CushionCapExtraWideLength + 2 * cushionExtraLength,
		m_data.FrontToeLength + m_data.DolphinColumnBottomWidth + m_data.BackToeLength + 2 * cushionExtraLength,
		m_data.CapThickness);

	return SUCCESS;
}

BentleyStatus CreateEmptyCenterCone(EditElementHandleR eeh, DgnModelRefR model,
	double uorLength, double uorDiameter, double uorThickness)
{
	DPoint3d topCenter = { 0,0,0 };
	DPoint3d bottomCenter = topCenter + DVec3d::From(0, 0, -uorLength);

	// Outer Cylinder
	DgnConeDetail outerCylinder = DgnConeDetail(topCenter, bottomCenter, uorDiameter / 2, uorDiameter / 2, true);
	ISolidPrimitivePtr outerCylinderPtr = ISolidPrimitive::CreateDgnCone(outerCylinder);
	DgnConeDetail innerCylinder = DgnConeDetail(topCenter, bottomCenter, uorDiameter / 2 - uorThickness, uorDiameter / 2 - uorThickness, true);
	ISolidPrimitivePtr innerCylinderPtr = ISolidPrimitive::CreateDgnCone(innerCylinder);
	ISolidKernelEntityPtr outerCylinderEntityPtr, innerCylinderEntityPtr;
	if (SUCCESS != SolidUtil::Create::BodyFromSolidPrimitive(outerCylinderEntityPtr, *outerCylinderPtr, model))
		return ERROR;
	if (SUCCESS != SolidUtil::Create::BodyFromSolidPrimitive(innerCylinderEntityPtr, *innerCylinderPtr, model))
		return ERROR;
	if (SUCCESS != SolidUtil::Modify::BooleanSubtract(outerCylinderEntityPtr, &innerCylinderEntityPtr, 1))
		return ERROR;
	if (SUCCESS != DraftingElementSchema::ToElement(eeh, *outerCylinderEntityPtr, nullptr, model))
		return ERROR;

	return SUCCESS;
}


BentleyStatus PDIWT::Waterway::Lock::MountPierP1::CreateMountPier(
	EditElementHandleR eeh, DgnModelRefR model)
{
	if (SUCCESS != InitializeComponents())
		return ERROR;

	ElementAgenda elems;

	double uorpermeter = model.GetModelInfoCP()->GetUorPerMeter();
	//DolphinColumn
	EditElementHandle dolphinEeh;
	if (SUCCESS != m_dolphin.CreateDolphin(dolphinEeh, model))
		return ERROR;
	elems.Insert(dolphinEeh);

	//cushioncap
	EditElementHandle cushioncapEeh;
	if (SUCCESS != m_cushioncap.CreateCushioncap(cushioncapEeh, model))
		return ERROR;

	DVec3d cushioncapDVec = DVec3d::From(0, (m_data.DolphinColumnHaunchLength - m_data.FrontToeLength), m_data.CushionCapBottomElevation - m_data.DolphinColumnBottomElevation);
	cushioncapDVec.Scale(uorpermeter);
	Transform cushioncapTrans = Transform::From(cushioncapDVec);
	if (SUCCESS != cushioncapEeh.GetHandler().ApplyTransform(cushioncapEeh, TransformInfo(cushioncapTrans)))
		return ERROR;
	elems.Insert(cushioncapEeh);

	//cushion
	EditElementHandle cushionEeh;
	if (SUCCESS != m_cushion.CreateCushion(cushionEeh, model))
		return ERROR;
	DVec3d cushionRelateToCushionCapDVec = DVec3d::From(0, -cushionExtraLength, -m_data.CapThickness);
	cushionRelateToCushionCapDVec.Scale(uorpermeter);
	if (SUCCESS != cushionEeh.GetHandler().ApplyTransform(cushionEeh, TransformInfo(Transform::From(cushioncapDVec + cushionRelateToCushionCapDVec))))
		return ERROR;
	elems.Insert(cushionEeh);

	//Piers
	double pileLen = m_data.PileTopElevation -
		m_data.PileBottomElevation;
	double pileDia = m_data.PileDiameterOut;
	double pileThick = m_data.PileWallThick;

	double dcushionCapWidth = m_data.FrontToeLength + m_data.DolphinColumnBottomWidth + m_data.BackToeLength;
	//double dcushionCapLenth = m_data.DolphinColumnBottomLength + 2 * m_data.CushionCapExtraWideLength;
	double cushionYFrontSide = 0 - m_data.FrontToeLength;
	double cushionCapLengthHarf = (m_data.DolphinColumnBottomLength + 2 * m_data.CushionCapExtraWideLength)
		/ 2;
	double pile03x = cushionCapLengthHarf - m_data.PileSideDistH - pileThick;
	double pile12x = -cushionCapLengthHarf + m_data.PileSideDistH + pileThick;
	double pile01y = cushionYFrontSide + m_data.PileSideDistV + pileThick;
	double pile23y = cushionYFrontSide + dcushionCapWidth - m_data.PileSideDistV - pileThick;

	double pileTop = m_data.PileTopElevation;

	pileLen *= uorpermeter;
	pileDia *= uorpermeter;
	pileThick *= uorpermeter;
	pile03x *= uorpermeter;
	pile12x *= uorpermeter;
	pile01y *= uorpermeter;
	pile23y *= uorpermeter;
	pileTop *= uorpermeter;
	//DPoint3d pile0 = { pile03x, pile01y, pileTop };
	//DPoint3d pile1 = { pile12x, pile01y, pileTop };
	//DPoint3d pile2 = { pile12x, pile23y, pileTop };
	//DPoint3d pile3 = { pile03x, pile23y, pileTop };

	EditElementHandle eehPile0, eehPile1, eehPile2, eehPile3;
	CreateEmptyCenterCone(eehPile0, model, 
		pileLen, pileDia, pileThick);
	CreateEmptyCenterCone(eehPile1, model,
		pileLen, pileDia, pileThick);
	CreateEmptyCenterCone(eehPile2, model,
		pileLen, pileDia, pileThick);
	CreateEmptyCenterCone(eehPile3, model,
		pileLen, pileDia, pileThick);

	stEleOperHelper::MoveAbsPos(eehPile0,
		0, 0, 0, pile03x, pile01y, pileTop);
	stEleOperHelper::MoveAbsPos(eehPile1,
		0, 0, 0, pile12x, pile01y, pileTop);
	stEleOperHelper::MoveAbsPos(eehPile2,
		0, 0, 0, pile12x, pile23y, pileTop);
	stEleOperHelper::MoveAbsPos(eehPile3,
		0, 0, 0, pile03x, pile23y, pileTop);

	elems.Insert(eehPile0);
	elems.Insert(eehPile1);
	elems.Insert(eehPile2);
	elems.Insert(eehPile3);
	//cell Header
	if (SUCCESS != NormalCellHeaderHandler::CreateGroupCellElement(eeh, elems, L"MountPierP1"))
		return ERROR;
	return SUCCESS;
}