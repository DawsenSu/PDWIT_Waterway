#pragma once
#include "stdafx.h"
#include "Program.h"
#include "GuidewallP2.h"

PDIWT_WATERWAY_LOCK_NAMESPACE_BEIGN
class GuidewallP2Tool :  DgnPrimitiveTool
{
private:
	gcroot<UI::GuidewallP2WinForm^> m_form;
	//gcroot<BMWPF::ToolSettingsHost^> m_wpfform;
protected:
	GuidewallP2Tool() : DgnPrimitiveTool(0,0) {}

	virtual bool _OnInstall() override;
	virtual void _OnPostInstall() override;
	virtual void _OnRestartTool() override;
	virtual bool _OnResetButton(DgnButtonEventCR ev) override;
	virtual bool _OnDataButton(DgnButtonEventCR ev) override;
	virtual void _OnDynamicFrame(DgnButtonEventCR ev) override;
	virtual void _OnCleanup() override;

public:
	static void	InstallNewInstance();
};
PDIWT_WATERWAY_LOCK_NAMESPACE_END

