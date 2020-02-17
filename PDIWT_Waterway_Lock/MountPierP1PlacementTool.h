#pragma once
#include "stdafx.h"
#include "Program.h"
#include "MountPierP1.h"
#pragma warning(disable:4700)
#pragma warning(disable:4564)

PDIWT_WATERWAY_LOCK_NAMESPACE_BEIGN
class MountPierP1PlacementTool : DgnPrimitiveTool
{
private:
	BE_DATA_VALUE(MountPierData, data)

		bvector<DPoint3d>				m_pts;
	////gcroot<UI::GuidewallP2WinForm^> m_form;
	gcroot<BMWPF::ToolSettingsHost^> m_wpfform;
protected:
	MountPierP1PlacementTool() : DgnPrimitiveTool(0, 0)
	{

	}

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