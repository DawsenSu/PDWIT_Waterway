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
	DolphinColumnP2() {}
	DolphinColumnP2(double topLength, double topWidth, double bottomLength, double bottomWidth, double height, double subtractWallThickness, double subtractWallHeight)
		:m_dolphinTopLength(topLength), m_dolphinTopWidth(topWidth), m_dolphinBottomLength(bottomLength), m_dolphinBottomWidth(bottomWidth), m_dolphinHeight(height),
		m_dolphinSubtractWallThickness(subtractWallThickness), m_dolphinSubtractWallHeight(subtractWallHeight) {}
	bool ValidateParameters() override;
	BentleyStatus CrerateDolphin(EditElementHandleR eeh, DgnModelRefR model);
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
	BE_PRIVATE_VALUE(double, cushioncapTopWidth)
		BE_PRIVATE_VALUE(double, cushioncapBottomWidth)
		BE_PRIVATE_VALUE(double, cushioncapHeight)
		BE_PRIVATE_VALUE(double, cushioncapLength)

public:
	Cushioncap() {}
	Cushioncap(double topWidth, double bottomWidth, double height, double length)
		: m_cushioncapTopWidth(topWidth), m_cushioncapBottomWidth(bottomWidth), m_cushioncapHeight(height), m_cushioncapLength(length) {}
	Cushioncap(double width,double height, double length)
		: m_cushioncapTopWidth(width), m_cushioncapBottomWidth(width),m_cushioncapHeight(height), m_cushioncapLength(length) {}
	bool ValidateParameters() override;
	BentleyStatus CreateCushioncap(EditElementHandleR eeh, DgnModelRefR model);
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
	Pile() {}
	Pile(double length, double diameter, double thickness)
		: m_pileLength(length), m_pileDiameter(diameter), m_pileThickness(thickness) {}
	bool ValidateParameters() override;
	BentleyStatus CreatePile(EditElementHandleR eeh, DgnModelRefR mdoel);
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
	Cushion() {}
	Cushion(double length, double width, double thickness)
		: m_cushionLength(length), m_cushionWidth(width), m_cushionThickness(thickness) {}
	bool ValidateParameters() override;
	BentleyStatus CreateCushion(EditElementHandleR eeh, DgnModelRefR model);
};

// su
class Wall : IParametersValidate
{
private:
	//wall
	BE_DATA_VALUE(double, wallLength)
		BE_DATA_VALUE(double, wallHeight)
		BE_DATA_VALUE(double, wallThickness)

public:
	Wall() {}
	Wall(double length, double height, double thickness)
		:m_wallLength(length), m_wallHeight(height), m_wallThickness(thickness) {}
	bool ValidateParameters() override;
	BentleyStatus CreateWall(EditElementHandleR eeh, DgnModelRefR model);
};

class Bridge : IParametersValidate
{
	BE_DATA_VALUE(double, length)
		BE_DATA_VALUE(double, width)
		BE_DATA_VALUE(double, height)
		BE_DATA_VALUE(double, topBoardThickness)
		BE_DATA_VALUE(double, supportWidth)
		BE_DATA_VALUE(double, chamferWidth)
public :
	Bridge() {}
	Bridge(double length, double width, double height, double topBoardThickness, double supportWidth, double chamferWidth)
		: m_length(length), m_width(width), m_height(height), m_topBoardThickness(topBoardThickness), m_supportWidth(supportWidth), m_chamferWidth(chamferWidth) {}
	bool ValidateParameters() override;
	BentleyStatus CreateBridge(EditElementHandleR eeh, DgnModelRefR model);
	
};
// su
//class Fender : IParametersValidate
//{
//	//fender
//	BE_DATA_VALUE(double, fenderTopElevation)
//		BE_DATA_VALUE(double, fenderBottomElevation)
//		BE_DATA_VALUE(double, fenderThickness)
//public:
//	bool ValidateParameters() override;
//};

PDIWT_WATERWAY_LOCK_NAMESPACE_END

