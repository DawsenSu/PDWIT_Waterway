#pragma once
#include "stdafx.h"
#include "Components.h"

PDIWT_WATERWAY_LOCK_NAMESPACE_BEIGN

struct DolphinP11Data : IParametersValidate
{
	BE_DATA_VALUE(double, SaftyHeight)
		BE_DATA_VALUE(double, DolphinColumnTopElevation)
		BE_DATA_VALUE(double, DolphinColumnBottomElevation)
		BE_DATA_VALUE(double, DolphinColumnTopLength)
		BE_DATA_VALUE(double, DolphinColumnTopWidth)
		BE_DATA_VALUE(double, DolphinColumnTopEdgeRadius)
		BE_DATA_VALUE(double, DolphinColumnSideEdgeRadius)
		BE_DATA_VALUE(double, DolphinColumnBottomLength)
		BE_DATA_VALUE(double, DolphinColumnBottomWidth)
		BE_DATA_VALUE(double, DolphinColumnHaunchHeight)
		BE_DATA_VALUE(double, DolphinColumnHaunchLength)
		BE_DATA_VALUE(double, AngleOfFirstPolylineWall)
		BE_DATA_VALUE(double, HeightOfFirstPolylineWall)
		BE_DATA_VALUE(double, AngleOfSecondPolylineWall)
		BE_DATA_VALUE(double, HeightOfSecondPolylineWall)
		BE_DATA_VALUE(double, CushionCapTopElevation)
		BE_DATA_VALUE(double, CushionCapBottomElevation)
		BE_DATA_VALUE(double, FrontToeLength)
		BE_DATA_VALUE(double, BackToeLength)
		BE_DATA_VALUE(double, CushionCapExtraWideLength)
		BE_DATA_VALUE(double, CushionCapLength)
		BE_DATA_VALUE(double, CushionCapWidth)
		BE_DATA_VALUE(double, CapThickness)
public:
	virtual bool ValidateParameters() override;

};


class DolphinP11
{
private:
	double cushionExtraLength = 0.1; //m

	DolphinP11Data			m_data;
	DolphinColumnP1_P11		m_dolphin;
	Cushioncap			m_cushioncap;
	Cushion				m_cushion;

	BentleyStatus		InitializeComponents();
public:
	DolphinP11(const DolphinP11Data& data) : m_data(data) {  }
	BentleyStatus		CreateDolphinP11(EditElementHandleR eeh, DgnModelRefR model);
};

PDIWT_WATERWAY_LOCK_NAMESPACE_END