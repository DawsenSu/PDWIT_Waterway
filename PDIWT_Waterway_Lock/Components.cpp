#include "Components.h"

bool PDIWT::Waterway::Lock::DolphinColumnP2::ValidateParameters()
{
	if (m_dolphinTopWidth <= 0 || m_dolphinTopLength <= 0 || m_dolphinBottomWidth <= 0 || m_dolphinBottomLength <= 0
		|| m_dolphinHeight <= 0 || m_dolphinSubtractWallThickness <= 0 || m_dolphinSubtractWallHeight <= 0)
		return false;
	if (m_dolphinSubtractWallThickness >= min(m_dolphinTopWidth, m_dolphinBottomWidth) || m_dolphinSubtractWallHeight >= m_dolphinHeight)
		return false;
	return true;
}

BentleyStatus PDIWT::Waterway::Lock::DolphinColumnP2::CrerateDolphin(EditElementHandleR eeh, DgnModelRefR model)
{
	if (!ValidateParameters())
		return ERROR;
	double uorpermeter = model.GetModelInfoCP()->GetUorPerMeter();
	UOR_Var(double, m_dolphinTopWidth, uorpermeter)
		UOR_Var(double, m_dolphinTopLength, uorpermeter)
		UOR_Var(double, m_dolphinBottomWidth, uorpermeter)
		UOR_Var(double, m_dolphinBottomLength, uorpermeter)
		UOR_Var(double, m_dolphinHeight, uorpermeter)
		UOR_Var(double, m_dolphinSubtractWallHeight, uorpermeter)
		UOR_Var(double, m_dolphinSubtractWallThickness, uorpermeter)

		DgnBoxDetail mainbodyDetail = DgnBoxDetail(DPoint3d{ -uor_m_dolphinBottomLength / 2, 0,0 }, DPoint3d{ -uor_m_dolphinTopLength / 2,0,uor_m_dolphinHeight },
			DVec3d::UnitX(), DVec3d::UnitY(),
			uor_m_dolphinBottomLength, uor_m_dolphinBottomWidth,
			uor_m_dolphinTopLength, uor_m_dolphinTopWidth, true);
	ISolidPrimitivePtr mainbodyPtr = ISolidPrimitive::CreateDgnBox(mainbodyDetail);

	double uor_dolphinSubstractWallLength = max(uor_m_dolphinTopLength, uor_m_dolphinBottomLength);
	DgnBoxDetail subtractbodyDetail = DgnBoxDetail(DPoint3d{ -uor_dolphinSubstractWallLength / 2, 0, uor_m_dolphinHeight - uor_m_dolphinSubtractWallHeight },
		DPoint3d{ -uor_dolphinSubstractWallLength / 2, 0,uor_m_dolphinHeight }, DVec3d::UnitX(), DVec3d::UnitY(),
		uor_dolphinSubstractWallLength, uor_m_dolphinSubtractWallThickness,
		uor_dolphinSubstractWallLength, uor_m_dolphinSubtractWallThickness, true);
	ISolidPrimitivePtr subtractbodyPtr = ISolidPrimitive::CreateDgnBox(subtractbodyDetail);

	ISolidKernelEntityPtr mainbodyEntityPtr, subtractbodyEntityPtr;
	if (ERROR == SolidUtil::Create::BodyFromSolidPrimitive(mainbodyEntityPtr, *mainbodyPtr, model))
		return ERROR;
	if (ERROR == SolidUtil::Create::BodyFromSolidPrimitive(subtractbodyEntityPtr, *subtractbodyPtr, model))
		return ERROR;
	if (ERROR == SolidUtil::Modify::BooleanSubtract(mainbodyEntityPtr, &subtractbodyEntityPtr, 1))
		return ERROR;
	if (ERROR == DraftingElementSchema::ToElement(eeh, *mainbodyEntityPtr, nullptr, model))
		return ERROR;
	return SUCCESS;
}

#pragma region Others
bool PDIWT::Waterway::Lock::DolphinColumnP1_P11::ValidateParameters()
{
	return true;
}

BentleyStatus PDIWT::Waterway::Lock::DolphinColumnP1_P11::CreateDolphin(EditElementHandleR eeh, DgnModelRefR model)
{
	return SUCCESS;
}

bool PDIWT::Waterway::Lock::DolphinColumnP3::ValidateParameters()
{
	return true;
}

BentleyStatus PDIWT::Waterway::Lock::DolphinColumnP3::CreateDolphin(EditElementHandleR eeh, DgnModelRefR model)
{
	return SUCCESS;
}

#pragma endregion Others

bool PDIWT::Waterway::Lock::Cushioncap::ValidateParameters()
{
	if (m_cushioncapTopWidth <= 0 || m_cushioncapBottomWidth <= 0 || m_cushioncapHeight <= 0 || m_cushioncapLength <= 0)
		return false;
	return true;
}

BentleyStatus PDIWT::Waterway::Lock::Cushioncap::CreateCushioncap(EditElementHandleR eeh, DgnModelRefR model)
{
	if (!ValidateParameters())
		return ERROR;
	double uorpermeter = model.GetModelInfoCP()->GetUorPerMeter();
	UOR_Var(double, m_cushioncapTopWidth, uorpermeter)
		UOR_Var(double, m_cushioncapBottomWidth, uorpermeter)
		UOR_Var(double, m_cushioncapHeight, uorpermeter)
		UOR_Var(double, m_cushioncapLength, uorpermeter)

		DgnBoxDetail cushioncapDetail = DgnBoxDetail(DPoint3d{ -uor_m_cushioncapLength / 2,0,0 },
			DPoint3d{ -uor_m_cushioncapLength / 2,uor_m_cushioncapBottomWidth - uor_m_cushioncapTopWidth, uor_m_cushioncapHeight },
			DVec3d::UnitX(), DVec3d::UnitY(),
			uor_m_cushioncapLength, uor_m_cushioncapBottomWidth,
			uor_m_cushioncapLength, uor_m_cushioncapTopWidth, true);
	ISolidPrimitivePtr cushioncapSolidPtr = ISolidPrimitive::CreateDgnBox(cushioncapDetail);
	if (ERROR == DraftingElementSchema::ToElement(eeh, *cushioncapSolidPtr, nullptr, model))
		return ERROR;
	return SUCCESS;
}

bool PDIWT::Waterway::Lock::Pile::ValidateParameters()
{
	if (m_pileDiameter <= 0 || m_pileLength <= 0 || m_pileThickness <= 0)
		return false;
	if (m_pileThickness * 2 >= m_pileDiameter)
		return false;
	return true;
}

BentleyStatus PDIWT::Waterway::Lock::Pile::CreatePile(EditElementHandleR eeh, DgnModelRefR model)
{
	if (!ValidateParameters())
		return ERROR;
	double uorpermeter = model.GetModelInfoCP()->GetUorPerMeter();
	double uorLength = m_pileLength * uorpermeter;
	double uorDiameter = m_pileDiameter * uorpermeter;
	double uorThickness = m_pileThickness * uorpermeter;

	DPoint3d topCenter;
	DPoint3d bottomCenter = topCenter + DVec3d::From(0, 0, -uorLength);

	// Outer Cylinder
	DgnConeDetail outerCylinder = DgnConeDetail(topCenter, bottomCenter, uorDiameter / 2, uorDiameter / 2, true);
	ISolidPrimitivePtr outerCylinderPtr = ISolidPrimitive::CreateDgnCone(outerCylinder);
	DgnConeDetail innerCylinder = DgnConeDetail(topCenter, bottomCenter, uorDiameter / 2 - uorThickness, uorDiameter / 2 - uorThickness, true);
	ISolidPrimitivePtr innerCylinderPtr = ISolidPrimitive::CreateDgnCone(innerCylinder);
	ISolidKernelEntityPtr outerCylinderEntityPtr, innerCylinderEntityPtr;
	if (ERROR == SolidUtil::Create::BodyFromSolidPrimitive(outerCylinderEntityPtr, *outerCylinderPtr, model))
		return ERROR;
	if (ERROR == SolidUtil::Create::BodyFromSolidPrimitive(innerCylinderEntityPtr, *innerCylinderPtr, model))
		return ERROR;
	if (ERROR == SolidUtil::Modify::BooleanSubtract(outerCylinderEntityPtr, &innerCylinderEntityPtr, 1))
		return ERROR;
	if (ERROR == DraftingElementSchema::ToElement(eeh, *outerCylinderEntityPtr, nullptr, model))
		return ERROR;
	return SUCCESS;
}


bool PDIWT::Waterway::Lock::Cushion::ValidateParameters()
{
	if (m_cushionLength <= 0 || m_cushionWidth <= 0 || m_cushionThickness <= 0)
		return false;
	return true;
}

BentleyStatus PDIWT::Waterway::Lock::Cushion::CreateCushion(EditElementHandleR eeh, DgnModelRefR model)
{
	if (!ValidateParameters())
		return ERROR;
	double uorpermeter = model.GetModelInfoCP()->GetUorPerMeter();
	UOR_Var(double, m_cushionLength, uorpermeter)
		UOR_Var(double, m_cushionWidth, uorpermeter)
		UOR_Var(double, m_cushionThickness, uorpermeter)

		DgnBoxDetail cushionDetail = DgnBoxDetail(DPoint3d{ -uor_m_cushionLength / 2,0,0 }, DPoint3d{
		-uor_m_cushionLength / 2,0,uor_m_cushionThickness }, DVec3d::UnitX(), DVec3d::UnitY(), uor_m_cushionLength,
		uor_m_cushionWidth, uor_m_cushionLength, uor_m_cushionWidth, true);
	ISolidPrimitivePtr cushionSolidPtr = ISolidPrimitive::CreateDgnBox(cushionDetail);
	if (ERROR == DraftingElementSchema::ToElement(eeh, *cushionSolidPtr, NULL, model))
		return ERROR;
	return SUCCESS;
}

bool PDIWT::Waterway::Lock::Wall::ValidateParameters()
{
	if (m_wallLength <= 0 || m_wallHeight <= 0 || m_wallThickness <= 0)
		return false;
	return true;
}

BentleyStatus PDIWT::Waterway::Lock::Wall::CreateWall(EditElementHandleR eeh, DgnModelRefR model)
{
	if (!ValidateParameters())
		return ERROR;
	double uorpermeter = model.GetModelInfoCP()->GetUorPerMeter();
	UOR_Var(double, m_wallLength, uorpermeter)
		UOR_Var(double, m_wallHeight, uorpermeter)
		UOR_Var(double, m_wallThickness, uorpermeter)

		DgnBoxDetail cushionDetail = DgnBoxDetail(DPoint3d{ -uor_m_wallLength / 2,0,0 }, DPoint3d{
		-uor_m_wallLength / 2,0,uor_m_wallHeight }, DVec3d::UnitX(), DVec3d::UnitY(), uor_m_wallLength,
		uor_m_wallThickness, uor_m_wallLength, uor_m_wallThickness, true);
	ISolidPrimitivePtr wallSolidPtr = ISolidPrimitive::CreateDgnBox(cushionDetail);
	if (ERROR == DraftingElementSchema::ToElement(eeh, *wallSolidPtr, NULL, model))
		return ERROR;
	return SUCCESS;
}

bool PDIWT::Waterway::Lock::Bridge::ValidateParameters()
{
	return true;
}
