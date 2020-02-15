#pragma once
#ifndef DolphinP11_h__
#define DolphinP11_h__

#include "stdafx.h"
#include "Components.h"
#include "ECHelper.h"

PDIWT_WATERWAY_LOCK_NAMESPACE_BEGIN

class DolphinP11
{
public:
	static const double cushionExtraLength; //m
	static const WString componentName;
private:
	gcroot<UI::Model::DolphinP11Data^> m_data;
	DolphinColumnP1_P11		m_dolphin;
	Cushioncap			m_cushioncap;
	Cushion				m_cushion;
	BentleyStatus		InitializeComponents();
public:
	DolphinP11(UI::Model::DolphinP11Data^ data);
	BentleyStatus		CreateDolphinP11(EditElementHandleR eeh, DgnModelRefR model, bool isForDynamic = false);
};

PDIWT_WATERWAY_LOCK_NAMESPACE_END
#endif // DolphinP11_h__