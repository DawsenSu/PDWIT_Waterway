#pragma once
#include "stdafx.h"
#include "Program.h"
PDIWT_WATERWAY_LOCK_STRUCTDEF(NativeGuideWallP3Data)

PDIWT_WATERWAY_LOCK_NAMESPACE_BEGIN
//! all the length is in meter, and angle in radian.
struct NativeGuideWallP3Data
{
	double	frontToeHeight;
	double	frontToeWidth;
	double	dolphinHeight;
	double	dolphinTopWidth;
	Angle	firstBackwallSegAngle;
	double	firstBackwallSegHeight;
	Angle	secondBackwallSegAngle;

	double	interval;
	double	gapwidth;
};

struct GuidewallP3Tool : public DgnElementSetTool
{
private:
	BentleyStatus	CreateDolphinProfile(CurveVectorPtr& curveVec, DgnModelRefR model);
	//GuidwallP3Data	m_data{ 0.6,0.3,10.0,2.0,Angle::FromDegrees(0),3,Angle::FromDegrees(45),10, 0.02 };
	NativeGuideWallP3Data	m_data{ 0.6,0.3,15.0,2.0,Angle::FromDegrees(0),3,Angle::FromDegrees(45),10, 0.02 };
	enum ToolState
	{
		Initialized,
		ChosenGuideLine
	} m_toolState;
	gcroot<BMWPF::ToolSettingsHost^> m_wpfform;
protected:
	GuidewallP3Tool() : DgnElementSetTool() { m_toolState = ToolState::Initialized; }

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
	virtual void			_OnCleanup()		override;

	bool					_OnResetButton(DgnButtonEventCR ev) override { _OnRestartTool(); return true; }
	virtual bool			_OnPostLocate(HitPathCP path, WStringR cantAcceptReason) override;
	virtual StatusInt		_OnElementModify(EditElementHandleR el) override { return SUCCESS; }
	virtual void			_OnRestartTool() override { InstallNewInstance(); }

public:
	static void InstallNewInstance();
	void	SetData();
};
PDIWT_WATERWAY_LOCK_NAMESPACE_END
