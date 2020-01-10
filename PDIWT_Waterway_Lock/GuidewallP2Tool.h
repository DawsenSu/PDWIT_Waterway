#pragma once
#include "stdafx.h"
#include "Program.h"
#include "GuidewallP2.h"

PDIWT_WATERWAY_LOCK_NAMESPACE_BEIGN
class GuidewallP2Tool :  DgnPrimitiveTool
{
private:
	BE_DATA_VALUE(GuideWallP2Data, data)

	bvector<DPoint3d>				m_pts;
	//gcroot<UI::GuidewallP2WinForm^> m_form;
	gcroot<BMWPF::ToolSettingsHost^> m_wpfform;
protected:
	GuidewallP2Tool() : DgnPrimitiveTool(0,0) 
	{
		m_data.SetdolphinTopElevation(10);
		m_data.SetdolphinBottomElevation(0);
		m_data.SetdolphinTopLength(1.5);
		m_data.SetdolphinTopWidth(1.8);
		m_data.SetdolphinBottomLength(1.5);
		m_data.SetdolphinBottomWidth(3.6);

		m_data.SetcushioncapBottomElevation(-2.5);
		m_data.SetcushioncapFrontToeLength(1);
		m_data.SetcushioncapRearToeLength(1);
		m_data.SetcushioncapExtraSideLength(0.25);
		m_data.SetcushioncapChamferLength(0.3);


		m_data.SetcushionThickness(0.1);

		m_data.SetpileTopElevation(-2.4);
		m_data.SetpileBottomElevation(-10);
		m_data.SetpileDiameter(1);
		m_data.SetpilewallThickness(0.2);
		m_data.SetpileOffsetPerpendicularRiver(1.5);

		m_data.SetwallTopElevation(10.1);
		m_data.SetwallBottomElevation(3);
		m_data.SetwallThickness(0.5);
		m_data.SetwallLength(10);
		m_data.SethasLeftWall(true);
		m_data.SethasRightWall(true);
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
public:
	static void	InstallNewInstance();
};
PDIWT_WATERWAY_LOCK_NAMESPACE_END

