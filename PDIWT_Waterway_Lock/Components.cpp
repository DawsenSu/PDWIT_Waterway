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
	//验证数据
	if (!ValidateParameters())
		return ERROR;
	//根据UOR调整数据
	double uorpermeter = model.GetModelInfoCP()->GetUorPerMeter();
	//UOR_Var(double, m_SaftyHeight, uorpermeter)
	UOR_Var(double, m_DolphinColumnTopElevation, uorpermeter)
		UOR_Var(double, m_DolphinColumnBottomElevation, uorpermeter)
		//UOR_Var(double, m_DolphinColumnTopLength, uorpermeter)
		UOR_Var(double, m_DolphinColumnTopWidth, uorpermeter)
		UOR_Var(double, m_DolphinColumnTopEdgeRadius, uorpermeter)
		UOR_Var(double, m_DolphinColumnSideEdgeRadius, uorpermeter)
		UOR_Var(double, m_DolphinColumnBottomLength, uorpermeter)
		UOR_Var(double, m_DolphinColumnBottomWidth, uorpermeter)
		UOR_Var(double, m_DolphinColumnHaunchHeight, uorpermeter)
		UOR_Var(double, m_DolphinColumnHaunchLength, uorpermeter)
		UOR_Var(double, m_AngleOfFirstPolylineWall, uorpermeter)
		UOR_Var(double, m_HeightOfFirstPolylineWall, uorpermeter)
		//UOR_Var(double, m_AngleOfSecondPolylineWall, uorpermeter)
		//UOR_Var(double, m_HeightOfSecondPolylineWall, uorpermeter)
		//UOR_Var(double, m_CushionCapTopElevation, uorpermeter)
		//创建点
		DPoint3d p1 = DPoint3d::From(uor_m_DolphinColumnBottomLength / 2, 0, 0);
	DPoint3d p2 = DPoint3d::From(uor_m_DolphinColumnBottomLength / 2, uor_m_DolphinColumnHaunchLength, uor_m_DolphinColumnHaunchHeight);
	DPoint3d p3 = DPoint3d::From(uor_m_DolphinColumnBottomLength / 2, uor_m_DolphinColumnHaunchLength, (uor_m_DolphinColumnTopElevation - uor_m_DolphinColumnBottomElevation));
	DPoint3d p4 = DPoint3d::From(uor_m_DolphinColumnBottomLength / 2, (uor_m_DolphinColumnHaunchLength + uor_m_DolphinColumnTopWidth), (uor_m_DolphinColumnTopElevation - uor_m_DolphinColumnBottomElevation));
	DPoint3d p5 = DPoint3d::From(uor_m_DolphinColumnBottomLength / 2, (uor_m_DolphinColumnHaunchLength + uor_m_DolphinColumnTopWidth + (std::tan(uor_m_AngleOfFirstPolylineWall / uorpermeter * PI / 180.0f)*uor_m_HeightOfFirstPolylineWall)), (uor_m_DolphinColumnTopElevation - uor_m_HeightOfFirstPolylineWall));
	DPoint3d p6 = DPoint3d::From(uor_m_DolphinColumnBottomLength / 2, (uor_m_DolphinColumnHaunchLength + uor_m_DolphinColumnBottomWidth), 0);
	//创建线
	CurveVectorPtr  cv1 = CurveVector::Create(CurveVector::BOUNDARY_TYPE_Outer);
	DSegment3d dolphinColumnSegment;
	dolphinColumnSegment.Init(p1, p2);
	cv1->push_back(ICurvePrimitive::CreateLine(dolphinColumnSegment));
	dolphinColumnSegment.Init(p2, p3);
	cv1->push_back(ICurvePrimitive::CreateLine(dolphinColumnSegment));
	dolphinColumnSegment.Init(p3, p4);
	cv1->push_back(ICurvePrimitive::CreateLine(dolphinColumnSegment));
	dolphinColumnSegment.Init(p4, p5);
	cv1->push_back(ICurvePrimitive::CreateLine(dolphinColumnSegment));
	dolphinColumnSegment.Init(p5, p6);
	cv1->push_back(ICurvePrimitive::CreateLine(dolphinColumnSegment));
	dolphinColumnSegment.Init(p6, p1);
	cv1->push_back(ICurvePrimitive::CreateLine(dolphinColumnSegment));
	//创建拉伸
	DPoint3d origin;
	DPoint3d xPt = DPoint3d::From(-uor_m_DolphinColumnBottomLength, 0.0, 0.0);
	DVec3d xVec = DVec3d::FromStartEnd(origin, xPt);
	DgnExtrusionDetail extrusionData(cv1, xVec, true);
	ISolidPrimitivePtr extrusion = ISolidPrimitive::CreateDgnExtrusion(extrusionData);
	if (SUCCESS != DraftingElementSchema::ToElement(eeh, *extrusion, nullptr, model))
		return ERROR;

	//构建位于X轴正方向的切割轮廓
	DPoint3d ptsCv1[3];
	DPoint3d p7 = DPoint3d::From((uor_m_DolphinColumnBottomLength / 2 - uor_m_DolphinColumnSideEdgeRadius), uor_m_DolphinColumnHaunchLength, (uor_m_DolphinColumnTopElevation - uor_m_DolphinColumnBottomElevation));
	DPoint3d midp1 = DPoint3d::From((uor_m_DolphinColumnBottomLength / 2 - uor_m_DolphinColumnSideEdgeRadius + std::sin(45 * PI / 180.0f)*uor_m_DolphinColumnSideEdgeRadius), (uor_m_DolphinColumnHaunchLength + uor_m_DolphinColumnSideEdgeRadius - std::cos(45 * PI / 180.0f)*uor_m_DolphinColumnSideEdgeRadius), (uor_m_DolphinColumnTopElevation - uor_m_DolphinColumnBottomElevation));
	DPoint3d p8 = DPoint3d::From(uor_m_DolphinColumnBottomLength / 2, (uor_m_DolphinColumnHaunchLength + uor_m_DolphinColumnSideEdgeRadius), (uor_m_DolphinColumnTopElevation - uor_m_DolphinColumnBottomElevation));
	DEllipse3d edgeCutArc1 = DEllipse3d::FromPointsOnArc(p8, midp1, p7);
	ptsCv1[0] = p7;
	ptsCv1[1] = p3;
	ptsCv1[2] = p8;
	CurveVectorPtr  cv2 = CurveVector::Create(CurveVector::BOUNDARY_TYPE_Outer);
	cv2->push_back(ICurvePrimitive::CreateArc(edgeCutArc1));
	cv2->push_back(ICurvePrimitive::CreateLineString(ptsCv1, 3));

	//构建位于X轴负方向的切割轮廓
	DPoint3d ptsCv2[3];
	DPoint3d pCenter2 = DPoint3d::From((-uor_m_DolphinColumnBottomLength / 2 + uor_m_DolphinColumnSideEdgeRadius), (uor_m_DolphinColumnHaunchLength + uor_m_DolphinColumnSideEdgeRadius), (uor_m_DolphinColumnTopElevation - uor_m_DolphinColumnBottomElevation));
	DPoint3d p9 = DPoint3d::From((-uor_m_DolphinColumnBottomLength / 2 + uor_m_DolphinColumnSideEdgeRadius), uor_m_DolphinColumnHaunchLength, (uor_m_DolphinColumnTopElevation - uor_m_DolphinColumnBottomElevation));
	DPoint3d p10 = DPoint3d::From(-uor_m_DolphinColumnBottomLength / 2, (uor_m_DolphinColumnHaunchLength + uor_m_DolphinColumnSideEdgeRadius), (uor_m_DolphinColumnTopElevation - uor_m_DolphinColumnBottomElevation));
	DPoint3d p3mirror = DPoint3d::From(-uor_m_DolphinColumnBottomLength / 2, uor_m_DolphinColumnHaunchLength, (uor_m_DolphinColumnTopElevation - uor_m_DolphinColumnBottomElevation));
	DEllipse3d edgeCutArc2 = DEllipse3d::FromArcCenterStartEnd(pCenter2, p9, p10);
	ptsCv2[0] = p10;
	ptsCv2[1] = p3mirror;
	ptsCv2[2] = p9;
	CurveVectorPtr  cv3 = CurveVector::Create(CurveVector::BOUNDARY_TYPE_Outer);
	cv3->push_back(ICurvePrimitive::CreateArc(edgeCutArc2));
	cv3->push_back(ICurvePrimitive::CreateLineString(ptsCv2, 3));

	//切割生成两个侧边的圆角
	ISolidKernelEntityPtr target;
	SolidUtil::Convert::ElementToBody(target, eeh);
	if (
		(SUCCESS != SolidUtil::Modify::BooleanCut(target, *cv2, SolidUtil::Modify::CutDirectionMode::Both,
			SolidUtil::Modify::CutDepthMode::Blind, (uor_m_DolphinColumnTopElevation - uor_m_DolphinColumnBottomElevation - uor_m_DolphinColumnHaunchHeight), false, NULL, 0))
		||
		(SUCCESS != SolidUtil::Modify::BooleanCut(target, *cv3, SolidUtil::Modify::CutDirectionMode::Both,
			SolidUtil::Modify::CutDepthMode::Blind, (uor_m_DolphinColumnTopElevation - uor_m_DolphinColumnBottomElevation - uor_m_DolphinColumnHaunchHeight), false, NULL, 0))
		)
		return ERROR;


	//绘制顶边切割轮廓
	DPoint3d ptsCv3[3];
	DPoint3d p11 = DPoint3d::From(p8.x-uor_m_DolphinColumnTopEdgeRadius,p8.y,p8.z);
	DPoint3d p12 = DPoint3d::From(p8.x,p8.y,p8.z- uor_m_DolphinColumnTopEdgeRadius);
	DPoint3d midp2 = DPoint3d::From(p8.x- (uor_m_DolphinColumnTopEdgeRadius-std::sin(45 * PI / 180.0f)*uor_m_DolphinColumnTopEdgeRadius),p8.y,p8.z- (uor_m_DolphinColumnTopEdgeRadius - std::sin(45 * PI / 180.0f)*uor_m_DolphinColumnTopEdgeRadius));
	DEllipse3d edgeCutArc3 = DEllipse3d::FromPointsOnArc(p12, midp2, p11);
	ptsCv3[0] = p11;
	ptsCv3[1] = p8;
	ptsCv3[2] = p12;
	CurveVectorPtr  cv4 = CurveVector::Create(CurveVector::BOUNDARY_TYPE_Outer);
	cv4->push_back(ICurvePrimitive::CreateArc(edgeCutArc3));
	cv4->push_back(ICurvePrimitive::CreateLineString(ptsCv3, 3));

	//绘制顶边切割实体路径
	DPoint3d ptsCv4[2];
	CurveVectorPtr  cv5 = CurveVector::Create(CurveVector::BOUNDARY_TYPE_Open);
	ptsCv4[0] = p7;
	ptsCv4[1] = p9;
	cv5->push_back(ICurvePrimitive::CreateArc(edgeCutArc1));
	cv5->push_back(ICurvePrimitive::CreateLineString(ptsCv4, 2));
	cv5->push_back(ICurvePrimitive::CreateArc(edgeCutArc2));
	
	ISolidKernelEntityPtr target2;
	if (SUCCESS != SolidUtil::Create::BodyFromSweep(target2, *cv4, *cv5, model, false, false, false, NULL, 0, 0, NULL))
		return ERROR;

	if (SUCCESS == SolidUtil::Modify::BooleanSubtract(target,&target2,1))
		SolidUtil::Convert::BodyToElement(eeh, *target, nullptr, model);

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
	if (m_length <= 0
		|| m_width <= 0
		|| m_height <= 0
		|| m_topBoardThickness <= 0
		|| m_supportWidth <= 0
		|| m_chamferWidth <= 0
		)
		return false;
	if (m_height <= m_topBoardThickness + m_chamferWidth
		|| m_width <= 2 * (m_supportWidth + m_chamferWidth)
		)
		return false;
	return true;
}

BentleyStatus PDIWT::Waterway::Lock::Bridge::CreateBridge(EditElementHandleR eeh, DgnModelRefR model)
{
	if (!ValidateParameters())
		return ERROR;
	double uorpermeter = model.GetModelInfoCP()->GetUorPerMeter();
	UOR_Var(double, m_length, uorpermeter)
		UOR_Var(double, m_width, uorpermeter)
		UOR_Var(double, m_height, uorpermeter)
		UOR_Var(double, m_topBoardThickness, uorpermeter)
		UOR_Var(double, m_supportWidth, uorpermeter)
		UOR_Var(double, m_chamferWidth, uorpermeter)
		bvector<DVec3d> dvecVec = { DVec3d::From(0,0,uor_m_height),
			DVec3d::From(0,uor_m_width),
			DVec3d::From(0,0,-uor_m_height),
			DVec3d::From(0,-uor_m_supportWidth),
			DVec3d::From(0,0,uor_m_height - uor_m_topBoardThickness - uor_m_chamferWidth),
			DVec3d::From(0,-uor_m_chamferWidth,uor_m_chamferWidth),
			DVec3d::From(0,-(uor_m_width - 2 * uor_m_supportWidth - 2 * uor_m_chamferWidth)),
			DVec3d::From(0,-uor_m_chamferWidth,-uor_m_chamferWidth),
			DVec3d::From(0,0,-(uor_m_height - uor_m_topBoardThickness - uor_m_chamferWidth)),
			DVec3d::From(0,-uor_m_supportWidth) };
	bvector<DPoint3d> pts = { DPoint3d() };
	for (size_t i = 0; i < dvecVec.size(); i++)
		pts.push_back(pts.back() + dvecVec[i]);
	pts.push_back(DPoint3d());
	CurveVectorPtr profile = CurveVector::CreateLinear(pts, CurveVector::BOUNDARY_TYPE_Inner, true);

	ISolidPrimitivePtr bridgeSolidPtr = ISolidPrimitive::CreateDgnExtrusion(DgnExtrusionDetail(profile, DVec3d::From(uor_m_length, 0), true));
	if (SUCCESS != DraftingElementSchema::ToElement(eeh, *bridgeSolidPtr, nullptr, model))
		return ERROR;

	return SUCCESS;
}
