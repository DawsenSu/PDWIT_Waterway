#pragma once
#include "stdafx.h"
#include "Components.h"

PDIWT_WATERWAY_LOCK_NAMESPACE_BEIGN
struct GuideWallP2Data : IParametersValidate
{
	// position information
	//dolphin
	BE_DATA_VALUE(double, dolphinTopElevation)
		BE_DATA_VALUE(double, dolphinBottomElevation)
		BE_DATA_VALUE(double, dolphinTopLength)
		BE_DATA_VALUE(double, dolphinTopWidth)
		BE_DATA_VALUE(double, dolphinBottomLength)
		BE_DATA_VALUE(double, dolphinBottomWidth)

		//cushioncap
		BE_DATA_VALUE(double, cushioncapBottomElevation)
		BE_DATA_VALUE(double, cushioncapFrontToeLength)
		BE_DATA_VALUE(double, cushioncapRearToeLength)
		BE_DATA_VALUE(double, cushioncapExtraSideLength)
		BE_DATA_VALUE(double, cushioncapChamferLength)

		// pile
		BE_DATA_VALUE(double, pileTopElevation)
		BE_DATA_VALUE(double, pileBottomElevation)
		BE_DATA_VALUE(double, pileDiameter)
		BE_DATA_VALUE(double, pilewallThickness)
		//BE_DATA_VALUE(double, pileOffsetAlongRiver)
		BE_DATA_VALUE(double, pileOffsetPerpendicularRiver)
		
		//cushion
		BE_DATA_VALUE(double, cushionThickness)

		//wall
		BE_DATA_VALUE(double, wallTopElevation)
		BE_DATA_VALUE(double, wallBottomElevation)
		BE_DATA_VALUE(double, wallThickness)
		BE_DATA_VALUE(double, wallLength)
		BE_DATA_VALUE(bool, hasLeftWall)
		BE_DATA_VALUE(bool, hasRightWall)

		//Bridge
		BE_DATA_VALUE(double, bridgeLength)
		BE_DATA_VALUE(double, bridgeHeight)
		BE_DATA_VALUE(double, bridgeWidth)
		BE_DATA_VALUE(double, bridgeTopBoardThickness)
		BE_DATA_VALUE(double, bridgeSupportWidth)
		BE_DATA_VALUE(double, bridgeChamferWidth)
public:
		virtual bool ValidateParameters() override;

};

//typedef RefCountedPtr<DolphinColumnP2> DolphinColumnP2Ptr;
//typedef RefCountedPtr<Cushioncap> CushioncapPtr;
//typedef RefCountedPtr<Pile> PilePtr;
//typedef RefCountedPtr<Cushion> CushionPtr;
//typedef RefCountedPtr<Wall> WallPtr;

class GuidewallP2 
{
private:
	double gapWidth = 0.02; // unit: m
	double bridgeHeigth = 0.57; //m
	double cushionExtraLength = 0.1; //m

	GuideWallP2Data			m_data;

	DolphinColumnP2		m_dolphin;
	Cushioncap			m_cushioncap;
	Pile				m_pile;
	Cushion				m_cushion;
	Wall				m_wall;


	BentleyStatus		InitializeComponents();
public:
	GuidewallP2(const GuideWallP2Data& data) : m_data(data) {  }
	BentleyStatus		CreateGuidewallP2(EditElementHandleR eeh, DgnModelRefR model);
};

PDIWT_WATERWAY_LOCK_NAMESPACE_END