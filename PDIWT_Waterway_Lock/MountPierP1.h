#pragma once
#include "stdafx.h"

#include "Components.h"

PDIWT_WATERWAY_LOCK_NAMESPACE_BEIGN

struct MountPierData// : IParametersValidate
{
	double SaftyHeight;
	double DolphinColumnTopElevation;
	double DolphinColumnBottomElevation;
	double DolphinColumnTopLength;
	double DolphinColumnTopWidth;
	double DolphinColumnTopEdgeRadius;
	double DolphinColumnSideEdgeRadius;
	double DolphinColumnBottomLength;
	double DolphinColumnBottomWidth;
	double DolphinColumnHaunchHeight;
	double DolphinColumnHaunchLength;

	double AngleOfFirstPolylineWall;
	double HeightOfFirstPolylineWall;
	double AngleOfSecondPolylineWall;
	double HeightOfSecondPolylineWall;

	double CushionCapTopElevation;
	double CushionCapBottomElevation;
	double FrontToeLength;
	double BackToeLength;
	double CushionCapExtraWideLength;
	double CapThickness;

	double PileDiameterOut;
	double PileWallThick;
	double PileSideDistV;
	double PileSideDistH;
	double PileTopElevation;
	double PileBottomElevation;

public:
	MountPierData();
	//virtual bool ValidateParameters() override;
};

class MountPierP1
{
private:
	double cushionExtraLength = 0.1; //m

	MountPierData			m_data;
	DolphinColumnP1_P11		m_dolphin;
	Cushioncap			m_cushioncap;
	Cushion				m_cushion;
	Pile m_pile;

	BentleyStatus		InitializeComponents();
public:
	MountPierP1(const MountPierData& data);
	BentleyStatus		CreateMountPier(EditElementHandleR eeh, DgnModelRefR model);
};

PDIWT_WATERWAY_LOCK_NAMESPACE_END