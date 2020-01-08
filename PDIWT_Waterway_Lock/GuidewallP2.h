#pragma once
#include "stdafx.h"
#include "Components.h"

PDIWT_WATERWAY_LOCK_NAMESPACE_BEIGN
struct GuideWallP2Data : IParametersValidate
{
	// position information
//dolphin postion information	
	BE_PRIVATE_VALUE(double, dolphinTopElevation)
		BE_PRIVATE_VALUE(double, dolphinBottomElevation)

		// pile position information
		BE_PRIVATE_VALUE(double, pileTopElevation)
		BE_PRIVATE_VALUE(double, pileOffsetAlongRiver)
		BE_PRIVATE_VALUE(double, pileOffsetPerpendicularRiver)
};


class GuidewallP2
{
private:
	GuideWallP2Data m_data;
	void BuildComponents();
public:
	void TransformComponents(DgnButtonEventCR ev);
	// composition parts
	BE_DATA_REFER(Dolphin, dolphin)
	BE_DATA_REFER(Cushioncap, cushioncap)
	BE_DATA_REFER(Pile, pile)
	BE_DATA_REFER(Cushion, cushion)
	BE_DATA_REFER(Wall, wall)
	BE_DATA_REFER(Fender, fender)

};

PDIWT_WATERWAY_LOCK_NAMESPACE_END