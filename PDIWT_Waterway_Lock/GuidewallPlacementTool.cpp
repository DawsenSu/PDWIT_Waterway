#include "GuidewallPlacementTool.h"

PDIWT_WATERWAY_LOCK_NAMESPACE_BEGIN
bool GuidewallPlacementTool::_OnPostLocate(HitPathCP path, WStringR cantAcceptReason)
{
	if (!DgnElementSetTool::_OnPostLocate(path, cantAcceptReason))
		return false;

	EditElementHandle eeh(path->GetHeadElem(), path->GetRoot());

	CurveVectorPtr curve = ICurvePathQuery::ElementToCurveVector(eeh);
	if (!curve.IsValid())
	{
		cantAcceptReason = L"This element cannot be converted to CurveVector";
		return false;
	}
	if (!curve->HasSingleCurvePrimitive())
	{
		cantAcceptReason = L"This element is an complex element";
		return false;
	}
	ICurvePrimitive::CurvePrimitiveType curvePriType = curve->at(0)->GetCurvePrimitiveType();
	if (curvePriType != ICurvePrimitive::CURVE_PRIMITIVE_TYPE_Line &&
		curvePriType != ICurvePrimitive::CURVE_PRIMITIVE_TYPE_LineString)
	{
		cantAcceptReason = L"This element is neither line nor linestring";
		return false;
	}
	return true;
}

bool GuidewallPlacementTool::_OnInstall()
{
	if (!__super::_OnInstall()) return false;
	if (!ISessionMgr::GetActiveDgnModelP()->Is3d()) return false;
	return true;
}

void PDIWT::Waterway::Lock::GuidewallPlacementTool::_OnPostInstall()
{
	m_wpfform = gcnew BMWPF::ToolSettingsHost();
	m_wpfform->Width = 250;
	m_wpfform->Height = 80;
	m_wpfform->Title = L"Guide wall P2 Create Tool";
	m_wpfform->Content = gcnew UI::DolphinGuidewallPlacementView();
	m_wpfform->Attach(Program::Addin);

	//AccuSnap::GetInstance().EnableSnap(true);
	//AccuSnap::GetInstance().EnableLocate(true);
	//AccuDraw::GetInstance().SetContext(ACCUDRAW_SetModePolar);

	//DVec3d uz = DVec3d::UnitZ();
	//AccuDraw::GetInstance().SetContext(ACCUDRAW_SetNormal, NULL, &uz, NULL, NULL, NULL, NULL);
	_SetupAndPromptForNextAction();
	__super::_OnPostInstall();
}

void PDIWT::Waterway::Lock::GuidewallPlacementTool::_OnCleanup()
{
	m_wpfform->Detach();
	delete m_wpfform;
	__super::_OnCleanup();
}

void GuidewallPlacementTool::_SetupAndPromptForNextAction()
{
	mdlOutput_command(L"GuidewalPlacementTool");
	switch (m_nextToolState)
	{
	case None:
		break;
	case Initialized:
		mdlOutput_prompt(L"Pick up the guide Line");
		break;
	case ChosenGuideLine:
		mdlOutput_prompt(L"Pick up Start Point");
		break;
	case PickStartPoint:
		mdlOutput_prompt(L"Pick up End Point");
		break;
	case PickEndPoint:
		mdlOutput_prompt(L"Accept/Reject");
		break;
	default:
		break;
	}
}

void GuidewallPlacementTool::_OnRestartTool()
{
	InstallNewInstance();
}

bool GuidewallPlacementTool::_OnDataButton(DgnButtonEventCR ev)
{
	CurveLocationDetail _curveLocationDetail;

	switch (m_nextToolState)
	{
	case None:
		break;
	case Initialized:
	{
		_LocateOneElement(ev, true);
		m_axis = ICurvePathQuery::ElementToCurveVector(*GetElementAgenda().begin())->front();
		_BeginDynamics();
		m_nextToolState = ChosenGuideLine;
		SetData();
		break;
	}
	case ChosenGuideLine:
	{
		if (m_axis->ClosestPointBounded(*ev.GetPoint(), _curveLocationDetail))
		{
			m_startPointFraction = _curveLocationDetail.fraction;
		}
		m_nextToolState = PickStartPoint;
		break;
	}
	case PickStartPoint:
	{
		if (m_axis->ClosestPointBounded(*ev.GetPoint(), _curveLocationDetail))
		{
			m_endPointFraction = _curveLocationDetail.fraction;
		}
		m_nextToolState = PickEndPoint;
		break;
	}
	case PickEndPoint:
	{
		//double _interval = m_data->WallLength;
		//double _distanceBetweenPoints;
		//double _uorpermetr = ISessionMgr::GetActiveDgnModelP()->GetModelInfo().GetUorPerMeter();
		//m_axis->SignedDistanceBetweenFractions(m_startPointFraction, m_endPointFraction, _distanceBetweenPoints);
		//mdlDialog_dmsgsPrint(WPrintfString(L"The distance between two fraction [%.2f, %0.2f] is : %.2f", m_startPointFraction, m_endPointFraction, _distanceBetweenPoints));
		//mdlDialog_dmsgsPrint(WPrintfString(L"Number of Guidewall is %d", static_cast<int>(_distanceBetweenPoints / _uorpermetr / _interval)));
		m_nextToolState = None;
		CallOnRestartTool();
		break;
	}
	default:
		break;
	}

	_SetupAndPromptForNextAction();
	return false;
}

bool PDIWT::Waterway::Lock::GuidewallPlacementTool::_OnResetButton(DgnButtonEventCR ev)
{
	CallOnRestartTool();
	return false;
}

void GuidewallPlacementTool::_OnDynamicFrame(DgnButtonEventCR ev)
{
	ElementAgenda		_indicators; //Indicators for Dynamic
	CurveLocationDetail _curveLocationDetail;

	switch (m_nextToolState)
	{
	case None:
		break;
	case Initialized:
		break;
	case ChosenGuideLine:
	{
		if (m_axis->ClosestPointBounded(*ev.GetPoint(), _curveLocationDetail))
		{
			DPoint3d _closestPoint = _curveLocationDetail.point;
			EditElementHandle _markEeh;
			CreateIndicatorElementForDynamic(_markEeh, *ACTIVEMODEL, _closestPoint);
			_indicators.Insert(_markEeh);
		}
		break;
	}
	case PickStartPoint:
	{
		EditElementHandle _startPointEeh;
		DPoint3d _startPoint;
		m_axis->FractionToPoint(m_startPointFraction, _startPoint);
		CreateIndicatorElementForDynamic(_startPointEeh, *ACTIVEMODEL, _startPoint);
		_indicators.Insert(_startPointEeh);
		if (m_axis->ClosestPointBounded(*ev.GetPoint(), _curveLocationDetail))
		{
			DPoint3d _closestPoint = _curveLocationDetail.point;
			EditElementHandle _markEeh;
			CreateIndicatorElementForDynamic(_markEeh, *ACTIVEMODEL, _closestPoint, 2); //green dot
			_indicators.Insert(_markEeh);
		}
		break;
	}
	case PickEndPoint:
		break;
	default:
		break;
	}
	_indicators.DrawInAllViews(DRAW_MODE_TempDraw, DrawPurpose::Dynamics, nullptr, nullptr, nullptr);
}

BentleyStatus PDIWT::Waterway::Lock::GuidewallPlacementTool::CreateIndicatorElementForDynamic(EditElementHandleR indicatorEeh, DgnModelRefR model, DPoint3dCR point, int colorInt)
{
	if (SUCCESS != LineHandler::CreateLineElement(indicatorEeh, nullptr, DSegment3d::From(point, point), false, model))
		return ERROR;
	ElementPropertiesSetterPtr _epSetter = ElementPropertiesSetter::Create();
	_epSetter->SetColor(colorInt);
	_epSetter->SetWeight(7);
	_epSetter->Apply(indicatorEeh);
	return SUCCESS;
}

void PDIWT::Waterway::Lock::GuidewallPlacementTool::SetData()
{
	UI::DolphinGuidewallPlacementView^ view = dynamic_cast<UI::DolphinGuidewallPlacementView^>(m_wpfform->Content);
	UI::Model::DolphinGuidewallPlacementData^ data = view->GetData();
	m_data = data;
}

void GuidewallPlacementTool::InstallNewInstance()
{
	GuidewallPlacementTool* tool = new GuidewallPlacementTool();
	tool->InstallTool();
}
PDIWT_WATERWAY_LOCK_NAMESPACE_END

