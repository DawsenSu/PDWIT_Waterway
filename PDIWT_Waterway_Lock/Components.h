#pragma once
#include "stdafx.h"

PDIWT_WATERWAY_LOCK_NAMESPACE_BEIGN

//////////////////////////////////////////////////////////////////////////
//
//	P1		桩基式靠船墩
//	P11		重力式靠船墩、重力式隔流墙
//	P2		桩基挂板式导航墙
//	P3		重力式导航墙
//////////////////////////////////////////////////////////////////////////
#define UOR_Var(type, var, factor) type uor_##var = var * factor;
class IParametersValidate {
public:
	virtual bool ValidateParameters() = 0;
	virtual ~IParametersValidate() = default;
};

// Su
class DolphinColumnP2 : IParametersValidate
{
private:
	// Dolphin
	BE_PRIVATE_VALUE(double, dolphinTopLength)
		BE_PRIVATE_VALUE(double, dolphinTopWidth)
		BE_PRIVATE_VALUE(double, dolphinBottomLength)
		BE_PRIVATE_VALUE(double, dolphinBottomWidth)
		BE_PRIVATE_VALUE(double, dolphinHeight)

		BE_PRIVATE_VALUE(double, dolphinSubtractWallThickness)
		BE_PRIVATE_VALUE(double, dolphinSubtractWallHeight)
public:
	DolphinColumnP2(double topLength, double topWidth, double bottomLength, double bottomWidth, double height, double subtractWallThickness, double subtractWallHeight)
		:m_dolphinTopLength(topLength), m_dolphinTopWidth(topWidth), m_dolphinBottomLength(bottomLength), m_dolphinBottomWidth(bottomWidth), m_dolphinHeight(height),
		m_dolphinSubtractWallThickness(subtractWallThickness), m_dolphinSubtractWallHeight(subtractWallHeight) {}
	bool ValidateParameters() override;
	BentleyStatus CrerateDolphin(EditElementHandleR ele, DgnModelRefR model);
};

/************************************************************************/
/* Cui & Duan										*/
/* The dolphin of P1 and P11											*/
/************************************************************************/
class DolphinColumnP1_P11 : IParametersValidate
{
private:

public:
	bool ValidateParameters() override;
	BentleyStatus CreateDolphin(EditElementHandleR eeh, DgnModelRefR model);
};

// Nan
class DolphinColumnP3 : IParametersValidate
{
private:

public:
	bool ValidateParameters() override;
	BentleyStatus CreateDolphin(EditElementHandleR eeh, DgnModelRefR model);
};

// Su
class Cushioncap : IParametersValidate
{
private:
	//cushioncap
	BE_PRIVATE_VALUE(double, cushioncapBottomElevation)
		BE_PRIVATE_VALUE(double, cushioncapFrontToeLength)
		BE_PRIVATE_VALUE(double, cushioncapRearToeLength)
		BE_PRIVATE_VALUE(double, cushioncapExtraSideLength)
		BE_PRIVATE_VALUE(double, cushioncapChamferLength)

		//cushioncap
		BE_PRIVATE_VALUE(double, cushioncapTopWidth)
		BE_PRIVATE_VALUE(double, cushioncapTopLength)
		BE_PRIVATE_VALUE(double, cushioncapBottomWidth)
		BE_PRIVATE_VALUE(double, cushioncapBottomLength)
		BE_PRIVATE_VALUE(double, cushioncapHeight)
public:
	bool ValidateParameters() override;
};

// Su
// The unit of this pile is meter
class Pile : IParametersValidate
{
private:
	//pile
	BE_PRIVATE_VALUE(double, pileLength)
		BE_PRIVATE_VALUE(double, pileDiameter)
		BE_PRIVATE_VALUE(double, pileThickness)
public:
	Pile() : m_pileLength(10), m_pileDiameter(0.8), m_pileThickness(0.1) {}
	Pile(double length, double diameter, double thickness)
		: m_pileLength(length), m_pileDiameter(diameter), m_pileThickness(thickness) {}
	bool ValidateParameters() override;
	BentleyStatus CreatePile(EditElementHandleR eeh, DgnModelRefR modelToInsert);
};

// nan
class Cushion : IParametersValidate
{
private:
	//chushion
	BE_PRIVATE_VALUE(double, cushionThickness)
		BE_PRIVATE_VALUE(double, cushionLength)
		BE_PRIVATE_VALUE(double, cushionWidth)
public:
	Cushion(double length, double width, double thickness)
		: m_cushionLength(length), m_cushionWidth(width), m_cushionThickness(thickness) {}
	bool ValidateParameters() override;
	BentleyStatus CreateCushion(EditElementHandleR eeh, DgnModelRefR modelToInsert);
};
// su
class Wall : IParametersValidate
{
	//wall
	BE_DATA_VALUE(double, wallTopElevation)
		BE_DATA_VALUE(double, wallBottomElevation)
		BE_DATA_VALUE(double, wallThickness)
		BE_DATA_VALUE(double, wallLength)
		BE_DATA_VALUE(bool, hasLeftWall)
		BE_DATA_VALUE(bool, hasRightWall)
public:
	bool ValidateParameters() override;
};
// su
class Fender : IParametersValidate
{
	//fender
	BE_DATA_VALUE(double, fenderTopElevation)
		BE_DATA_VALUE(double, fenderBottomElevation)
		BE_DATA_VALUE(double, fenderThickness)
public:
	bool ValidateParameters() override;
};

PDIWT_WATERWAY_LOCK_NAMESPACE_END

