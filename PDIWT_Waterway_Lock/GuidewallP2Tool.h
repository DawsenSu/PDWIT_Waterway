#pragma once
#ifndef GuidewallP2Tool_h__
#define GuidewallP2Tool_h__

#include "stdafx.h"
#include "Program.h"
#include "GuidewallP2.h"
#pragma warning(disable:4700)
#pragma warning(disable:4564)

PDIWT_WATERWAY_LOCK_NAMESPACE_BEGIN
class GuidewallP2Tool : DgnPrimitiveTool
{
private:
	//The data struct which represented in Cache, all unite are in meters;
	gcroot<UI::Model::GuidewallP2Data^> m_data;
	bvector<DPoint3d>				m_pts;
	//gcroot<UI::GuidewallP2WinForm^> m_form;
	gcroot<BMWPF::ToolSettingsHost^> m_wpfform;
protected:
	GuidewallP2Tool() : DgnPrimitiveTool(0, 0)	{}

	virtual bool _OnInstall() override;
	virtual void _OnPostInstall() override;
	virtual void _OnRestartTool() override;
	virtual bool _OnResetButton(DgnButtonEventCR ev) override;
	virtual bool _OnDataButton(DgnButtonEventCR ev) override;
	virtual void _OnDynamicFrame(DgnButtonEventCR ev) override;
	virtual void _OnCleanup() override;
private:
	void SetupForNextAction();
	void SetData();
public:
	static void	InstallNewInstance();
};
PDIWT_WATERWAY_LOCK_NAMESPACE_END
#endif // GuidewallP2Tool_h__
