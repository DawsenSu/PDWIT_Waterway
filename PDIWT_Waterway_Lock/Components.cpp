#include "Components.h"

bool PDIWT::Waterway::Lock::Pile::ValidateParameters()
{
	if (m_pileDiameter <= 0 || m_pileLength <= 0 || m_pileThickness <= 0)
		return false;
	if (m_pileThickness * 2 >= m_pileDiameter)
		return false;
	return true;
}

BentleyStatus PDIWT::Waterway::Lock::Pile::CreatePile(EditElementHandleR eeh, DgnModelRefR modelToInsert)
{
	if (!ValidateParameters())
		return ERROR;
	double uorpermeter = modelToInsert.GetModelInfoCP()->GetUorPerMeter();
	double uorLength = GetpileLength() * uorpermeter;
	double uorDiameter = GetpileDiameter() * uorpermeter;
	double uorThickness = GetpileThickness() * uorpermeter;

	DPoint3d topCenter;
	DPoint3d bottomCenter = topCenter + DVec3d::From(0, 0, -uorLength);

	// Outer Cylinder
	DgnConeDetail outerCylinder = DgnConeDetail(topCenter, bottomCenter, uorDiameter / 2, uorDiameter / 2, true);
	ISolidPrimitivePtr outerCylinderPtr = ISolidPrimitive::CreateDgnCone(outerCylinder);
	DgnConeDetail innerCylinder = DgnConeDetail(topCenter, bottomCenter, uorDiameter / 2 - uorThickness, uorDiameter / 2 - uorThickness, true);
	ISolidPrimitivePtr innerCylinderPtr = ISolidPrimitive::CreateDgnCone(innerCylinder);
	ISolidKernelEntityPtr outerCylinderEntityPtr, innerCylinderEntityPtr;
	if (ERROR == SolidUtil::Create::BodyFromSolidPrimitive(outerCylinderEntityPtr, *outerCylinderPtr, modelToInsert))
		return ERROR;
	if (ERROR == SolidUtil::Create::BodyFromSolidPrimitive(innerCylinderEntityPtr, *innerCylinderPtr, modelToInsert))
		return ERROR;
	if (ERROR == SolidUtil::Modify::BooleanSubtract(outerCylinderEntityPtr, &innerCylinderEntityPtr, 1))
		return ERROR;
	if (ERROR == SolidUtil::Convert::BodyToElement(eeh, *outerCylinderEntityPtr, nullptr, modelToInsert))
		return ERROR;
	return SUCCESS;
}


bool PDIWT::Waterway::Lock::Cushion::ValidateParameters()
{
	if (m_cushionLength <= 0 || m_cushionWidth <= 0 || m_cushionThickness <= 0)
		return false;
	return true;
}

BentleyStatus PDIWT::Waterway::Lock::Cushion::CreateCushion(EditElementHandleR eeh, DgnModelRefR modelToInsert)
{
	if (!ValidateParameters())
		return ERROR;
	double uorpermeter = modelToInsert.GetModelInfoCP()->GetUorPerMeter();
	UOR_Var(double, m_cushionLength, uorpermeter)
		UOR_Var(double, m_cushionWidth, uorpermeter)
		UOR_Var(double, m_cushionThickness, uorpermeter)
		DgnBoxDetail cushionDetail = DgnBoxDetail(DPoint3d{ 0,0,0 }, DPoint3d{
		0,0,-uor_m_cushionThickness }, DVec3d::UnitX(), DVec3d::UnitY(), uor_m_cushionLength,
		uor_m_cushionLength, uor_m_cushionWidth, uor_m_cushionWidth, true);
	ISolidPrimitivePtr cushionSolidPtr = ISolidPrimitive::CreateDgnBox(cushionDetail);
	if (ERROR == DraftingElementSchema::ToElement(eeh, *cushionSolidPtr, NULL, modelToInsert))
		return ERROR;
	return SUCCESS;
}
