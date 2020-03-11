#pragma once
#include "stdafx.h"
#include "Components.h"

PDIWT_WATERWAY_LOCK_STRUCTDEF(TypeARailData)
PDIWT_WATERWAY_LOCK_STRUCTDEF(TypeBRailData)


PDIWT_WATERWAY_LOCK_NAMESPACE_BEGIN
struct TypeARailData 
{
	
};

//unit all in meter
struct TypeBRailData 
{
	double postRailDiameter;
	double postSpan;
	double postHeight;
	double middleRailDiameter;
	double mideleRailHeight;

	double reinforcePlateBottomWidth;
	double reinforcePlateTopWidth;
	double reinforcePlateHeight;
	double reinforceThickness;
	double basePlateThickness;
	void Scale(double scale)
	{
		postRailDiameter *= scale;
		postSpan *= scale;
		postHeight *= scale;
		middleRailDiameter *= scale;
		mideleRailHeight *= scale;

		reinforcePlateBottomWidth *= scale;
		reinforcePlateTopWidth *= scale;
		reinforcePlateHeight *= scale;
		reinforceThickness *= scale;
		basePlateThickness *= scale;
	}
};

struct CurveLocationCompare
{
	bool operator()(CurveLocationDetailCR lhs, CurveLocationDetailCR rhs)
	{
		return lhs.fraction < rhs.fraction;
	}
};

struct RailTool : public DgnElementSetTool
{
private:
	BentleyStatus	CreateTypeBRail(ICurvePrimitivePtr axis, TypeBRailDataCR data, DgnModelRefR model);
protected:
	RailTool() : DgnElementSetTool() { }

	virtual bool	_OnInstall() override;
	virtual void	_OnPostInstall() override;
	virtual bool	_OnDataButton(DgnButtonEventCR ev) override;

	virtual bool	_WantAccuSnap() override { return true; }
	virtual bool	_WantDynamics() override { return false; }
	virtual UsesDragSelect	_AllowDragSelect() override { return DgnElementSetTool::USES_DRAGSELECT_None; }
	virtual UsesFence		_AllowFence() override { return DgnElementSetTool::USES_FENCE_None; }
	virtual UsesSelection	_AllowSelection() override { return DgnElementSetTool::USES_SS_None; }
	virtual bool			_DoGroups()			override { return false; }
	virtual bool			_IsModifyOriginal() override { return false; }
	//virtual void			_OnCleanup()		override;

	bool					_OnResetButton(DgnButtonEventCR ev) override { _OnRestartTool(); return true; }
	virtual bool			_OnPostLocate(HitPathCP path, WStringR cantAcceptReason) override;
	virtual StatusInt		_OnElementModify(EditElementHandleR el) override { return SUCCESS; }
	virtual void			_OnRestartTool() override { InstallNewInstance(); }

public:
	static void InstallNewInstance();
};

PDIWT_WATERWAY_LOCK_NAMESPACE_END