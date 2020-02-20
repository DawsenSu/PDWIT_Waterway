#pragma once

#include "stdafx.h"
#include "ECHelper.h"

struct ShipLockRoomP7Data
{
	//There for the moment not used.
	double SafeSuperElvation = 1.5; //Num 0
	double TopElevation = 10.6; //Num 1
	//double BottomBoardTopElevation = 
	// > BottomBoardTopElevation and < (TopElevation-NeckHeight)
	double ShoulderTopElevation = 4.0;//Num 2
	// < (ShoulderTopElevation-BottomBoardThickness)
	double BottomElevation = 0;//Num 3
	//< SideWallBottomWidth 
	double WallTopWidth = 1.8;//Num 4
	//< WallBottomWidth
	double SideWallBottomWidth = 3.5;//Num 5
	//> WallBottomWidth
	double WallBottomWidth = 6; //Num 6
	//< WallBottomWidth
	double FrontToeWidth = 2.5;//Num 7
	//< FrontToeWidth
	double FrontSideRAngleWidth = 0.3;//Num 8
	//< (TopElevation-BottomElevation-BottomBoardThickness)
	double FrontSideRAngleHeight = 0.6;//Num 9
	//< (TopElevation-ShoulderTopElevation)
	double NeckHeight = 3;//Num 10
	//double WallBackHeight1 = ;
	//double WallBackHeight2;
	//1/2 < DigRatio < 1/1
	double DigRatio = 1 / 1.5;//Num 11
	//< (ShoulderTopElevation - BottomElevation)
	double BottomBoardThickness = 3;//Num 12
	//<0.5
	double CussionThickness = 0.2;//Num 13
	//>0
	double StructLen = 9.98;//Num 14

	static bool Create(EditElementHandleR reeh, void* data);

	static void SetECInstWithData(IECInstanceR ECInst,
		const ShipLockRoomP7Data& data);
	static void ReadECInst2Data(const DgnECInstancePtr& ECInst,
		ShipLockRoomP7Data&  data);
	
	static DPoint3d ReadECOffPoint(const DgnECInstancePtr& ECInst);
	static DPoint3d CaculateOff(const EditElementHandleR eeh);
	static void SetECOffPoint(const IECInstanceR& ECInst, const EditElementHandleR eeh);
};
