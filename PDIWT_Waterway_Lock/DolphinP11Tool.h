#pragma once
#include "stdafx.h"
#include "Program.h"
#include "DolphinP11.h"
#pragma warning(disable:4700)
#pragma warning(disable:4564)

PDIWT_WATERWAY_LOCK_NAMESPACE_BEIGN
class DolphinP11Tool : DgnPrimitiveTool
{
private:
	BE_DATA_VALUE(DolphinP11Data, data)

		bvector<DPoint3d>				m_pts;
	////gcroot<UI::GuidewallP2WinForm^> m_form;
	gcroot<BMWPF::ToolSettingsHost^> m_wpfform;
protected:
	DolphinP11Tool() : DgnPrimitiveTool(0, 0)
	{
		m_data.SetSaftyHeight(1.5);
		m_data.SetDolphinColumnTopElevation(10);
		m_data.SetDolphinColumnBottomElevation(0);
		m_data.SetDolphinColumnTopLength(3.5);
		m_data.SetDolphinColumnTopWidth(3.5);
		m_data.SetDolphinColumnTopEdgeRadius(0.1);
		m_data.SetDolphinColumnSideEdgeRadius(0.3);
		m_data.SetDolphinColumnBottomLength(3.5);
		m_data.SetDolphinColumnBottomWidth(5);
		m_data.SetDolphinColumnHaunchHeight(1);
		m_data.SetDolphinColumnHaunchLength(0.5);
		m_data.SetAngleOfFirstPolylineWall(0);
		m_data.SetHeightOfFirstPolylineWall(4);
		m_data.SetAngleOfSecondPolylineWall(45);
		m_data.SetHeightOfSecondPolylineWall(4);

		m_data.SetCushionCapTopElevation(0);
		m_data.SetCushionCapBottomElevation(-1.5);
		m_data.SetFrontToeLength(1);
		m_data.SetBackToeLength(1);
		m_data.SetCushionCapExtraWideLength(1);
		m_data.SetCapThickness(0.1);
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