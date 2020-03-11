#pragma once
#ifndef GuidewallP2_h__
#define GuidewallP2_h__

#include "stdafx.h"
#include "Components.h"
#include "ECHelper.h"
//
//#pragma  managed(push, off)
//#include <rttr\registration>
//struct MyStruct { MyStruct() {}; void func(double) {}; int data; };
//
//RTTR_REGISTRATION
//{
//	rttr::registration::class_<MyStruct>("MyStruct")
//		 .constructor<>()
//		 .property("data", &MyStruct::data)
//		 .method("func", &MyStruct::func);
//}
//
//#pragma managed(pop)

PDIWT_WATERWAY_LOCK_NAMESPACE_BEGIN
struct NativeGuideWallP2Data
{
	double	DolphinTopElevation  = 10;
	double	DolphinBottomElevation;
	double	DolphinTopLength;
	double	DolphinTopWidth;
	double	DolphinBottomLength;
	double	DolphinBottomWidth;
	double	CushioncapBottomElevation;
	double	CushioncapFrontToeLength;
	double	CushioncapRearToeLength;
	double	CushioncapExtraSideLength;
	double	CushioncapChamferLength;
	double	PileTopElevation;
	double	PileBottomElevation;
	double	PileDiameter;
	double	PileWallThickness;
	double	PileOffsetPerpendicularRiver;
	double	CushionThickness;
	double	WallBottomElevation;
	double	WallThickness;
	bool	WallLength = true;
	bool	HasLeftWall = true;
	double	HasRightWall;
	double	BridgeLength;
	double	BridgeHeight;
	double	BridgeWidth;
	double	BridgeTopBoardThickness;
	double	BridgeSupportWidth;
	double	BridgeChamferWidth;
	
};

class GuidewallP2
{
public:
	static const double gapWidth; // unit: m
	static const double cushionExtraLength; //m
	static const double bridgePavementThickness;
	static const WString componentName;
private:
	gcroot<UI::Model::GuidewallP2Data^> m_data;

	DolphinColumnP2		m_dolphin;
	Cushioncap			m_cushioncap;
	Pile				m_pile;
	Cushion				m_cushion;
	Wall				m_wall;
	Bridge				m_bridge;
	Cushion				m_bridgeCushion;
	BentleyStatus		InitializeComponents();

public:
	GuidewallP2(UI::Model::GuidewallP2Data^ data);

	BentleyStatus		CreateGuidewallP2(EditElementHandleR eeh, DgnModelRefR model, bool isForDynamic = false);
};

PDIWT_WATERWAY_LOCK_NAMESPACE_END
#endif // GuidewallP2_h__