#pragma once
#ifndef GuidewallP2_h__
#define GuidewallP2_h__

#include "stdafx.h"
#include "Components.h"
#include "ECHelper.h"

PDIWT_WATERWAY_LOCK_NAMESPACE_BEGIN

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