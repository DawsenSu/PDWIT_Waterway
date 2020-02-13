#pragma once
#include "stdafx.h"
#include "Program.h"
#include "GuidewallP2.h"

PDIWT_WATERWAY_LOCK_NAMESPACE_BEGIN
struct GuidewallPlacementTool : DgnElementSetTool
{
public:
	enum ToolState
	{
		None,
		Initialized,
		ChosenGuideLine,
		PickStartPoint,
		PickEndPoint
	};

private:
	ToolState				m_nextToolState;
	ICurvePrimitivePtr		m_axis;		
	double					m_startPointFraction, m_endPointFraction;
	//The data struct which represented in Cache, all unite are in meters;
	gcroot<UI::Model::DolphinGuidewallPlacementData^> m_data;
	gcroot<BMWPF::ToolSettingsHost^> m_wpfform;
protected:
	GuidewallPlacementTool() : DgnElementSetTool() { m_nextToolState = Initialized; }
	//only allow to pick up the guide line, disable the fence and selection set
	virtual UsesFence		_AllowFence()		override { return DgnElementSetTool::USES_FENCE_None; }
	virtual UsesSelection	_AllowSelection()	override { return DgnElementSetTool::USES_SS_None; }
	virtual UsesDragSelect	_AllowDragSelect()	override { return DgnElementSetTool::USES_DRAGSELECT_None; }
	virtual bool			_DoGroups()			override { return false; }
	virtual bool			_IsModifyOriginal() override { return false; }

	//If current model is not 3D model don't launch the tool
	virtual bool			_OnInstall()		override;
	//To initialize the UI
	virtual void			_OnPostInstall()	override;
	//To release the UI resources
	virtual void			_OnCleanup()		override;
	//If the element to be selected is not line, line string, prompt it cannot be chosen.
	virtual bool			_OnPostLocate(HitPathCP path, WStringR cantAcceptReason) override;
	//Not intend to modify the guide line
	virtual StatusInt		_OnElementModify(EditElementHandleR el) override { return SUCCESS; }
	virtual void			_OnRestartTool() override;
	virtual void			_SetupAndPromptForNextAction() override;
	virtual	bool			_OnDataButton(DgnButtonEventCR ev) override;
	virtual bool			_OnResetButton(DgnButtonEventCR ev) override;
	virtual void			_OnDynamicFrame(DgnButtonEventCR ev) override;

private:
	BentleyStatus			CreateIndicatorElementForDynamic(EditElementHandleR indicatorEeh, DgnModelRefR model, DPoint3dCR point, int colorInt = 0);
	void					SetData();
public:
	static void InstallNewInstance();

};
PDIWT_WATERWAY_LOCK_NAMESPACE_END
