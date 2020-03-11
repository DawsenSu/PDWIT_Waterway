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
	//! Create Indicator dots for dynamic purpose
	//! @param[in, out]	indicatorEeh	the dot element handler
	//! @param[in]	model		The model to store dot element
	//! @param[in]	point		dot point, in uor
	//! @param[in]	colorInt		dot color, default is black
	//! @return		SUCCESS if the create operation succeed.
	BentleyStatus			CreateIndicatorElementForDynamic(EditElementHandleR indicatorEeh, DgnModelRefR model, DPoint3dCR point, int colorInt = 0);

	//!	Create isolated Proxy Object which is dolphin-like, proxy object will be represented by 5m*5m square in xy plane.
	//! @param[in,out]		eeh	The editelementhandler of isolated proxy object 
	//! @param[in]		model	The model in which the proxy object will be created
	//! @param[in]		insertPt		Insert point for proxy object
	//! @param[in]		direction		Direction of proxy object, in xy plane.
	//! @return	SUCCESS if the operation succeeded.
	BentleyStatus			CreateIsolatedProxyObject(EditElementHandleR eeh, DgnModelRefR model, DPoint3dCR insertPt, DVec2dCR direction);

	//!	Create continuous Proxy Object which is gravitational-like, proxy object will be represented by a rectangle in xy plane with height of 5m.
	//! @param[in,out]		eeh	The editelementhandler of contiunous proxy object 
	//! @param[in]		model	The model in which the proxy object will be created
	//! @param[in]		startPt		First insert point for proxy object
	//! @param[in]		endPt		Second insert Point for proxy object, if z coordinate of endpt is not same as startpt, it will force it to be.
	//! @return	SUCCESS if the operation succeeded.
	BentleyStatus			CreateContinuouslyProxyObject(EditElementHandleR eeh, DgnModelRefR model, DPoint3dCR startPt, DPoint3dCR endPt);

	//! Create proxy Object Based on selected options
	//!	@param[in, out]	eleAgenda		the elementAgenda in which stores proxy objects
	//!	@param[in]		secondInsertPt	the second picked insert point.
	//!	@param[in]		isForDyanmic		indicate if elementagneda stores elemnts for dynamics, for example, the insert point
	//! @return	SUCCESS if the operation succeeded.
	//!	@remark this method is to encapsulate process of proxy objects creation. It's called after first insert point has been determined, ie. class member m_startPoint fraction has been set.
	BentleyStatus			CreateProxyObjects(ElementAgendaR eleAgenda, DPoint3dCR secondInsertPt, bool isForDynamic = true);

	void					SetData();
public:
	static void InstallNewInstance();

};

PDIWT_WATERWAY_LOCK_NAMESPACE_END
