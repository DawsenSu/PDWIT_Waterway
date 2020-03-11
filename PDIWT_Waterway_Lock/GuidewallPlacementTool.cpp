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
	delete m_data;
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
		mdlOutput_prompt(L"Pick up End Point and accept");
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
		ElemAgendaEntry* _firstElem = GetElementAgenda().begin();
		if (_firstElem == nullptr)
			return false;

		
		m_axis = ICurvePathQuery::ElementToCurveVector(*GetElementAgenda().begin())->front();
		SetData();

		AccuSnapR _accuSnap = AccuSnap::GetInstance();
		_accuSnap.EnableLocate(true);
		_accuSnap.EnableSnap(true);

		m_nextToolState = ChosenGuideLine;
		_BeginDynamics();
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
		ElementAgenda _proxyObjs;
		if (SUCCESS != CreateProxyObjects(_proxyObjs, *ev.GetPoint(), false))
		{
			mdlOutput_messageCenter(OutputMessagePriority::Error, L"Can't create proxy object", L"", OutputMessageAlert::Balloon);
			return false;
		}
		for (auto ele : _proxyObjs)
		{
			ele.AddToModel();
		}
		CallOnRestartTool();
		break;
	}
	case PickEndPoint:
	{
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
	DPoint3d _hoverPt = *ev.GetPoint();
	//DPoint3d _axisStartPt;
	//m_axis->GetStartPoint(_axisStartPt);
	//_hoverPt.z = _axisStartPt.z;
	CurveLocationDetail _currentBtnClosestPtCurveLocationDetail;
	m_axis->ClosestPointBounded(_hoverPt, _currentBtnClosestPtCurveLocationDetail);
	//if (ICurvePrimitive::CurvePrimitiveType::CURVE_PRIMITIVE_TYPE_Line == m_axis->GetCurvePrimitiveType())
	//{
	//	double _projectPointFraction;
	//	m_axis->GetLineCP()->ProjectPointBounded(_projectPoint, _projectPointFraction, *ev.GetPoint());
	//}
	//else if (ICurvePrimitive::CurvePrimitiveType::CURVE_PRIMITIVE_TYPE_LineString == m_axis->GetCurvePrimitiveType())
	//{
	//	//Get the nearest projected points as projected point
	//	const bvector<DPoint3d>* _linestringPtsP = m_axis->GetLineStringCP();
	//	bvector<Dpoint3d>	_projectsPts;
	//	
	//	for (size_t i =0; i < _linestringPtsP->size()-1; i++)
	//	{
	//		DSegment3d _lineSeg = DSegment3d::From(_linestringPtsP->at(i), _linestringPtsP->at(i + 1));
	//		Dpoint3d _tempPt;
	//		double _tempFraction;
	//		_lineSeg.ProjectPointBounded(_tempPt, _tempFraction, *ev.GetPoint());
	//		_projectsPts.push_back(_tempPt);
	//	}
	//	auto _maxPtIter = std::max_element(_projectsPts.begin(), _projectsPts.end(), [&ev](DPoint3d x, DPoint3d y)->bool
	//	{
	//		return x.DistanceSquared(*ev.GetPoint()) < y.DistanceSquared(*ev.GetPoint());
	//	});
	//	_projectPoint = *_maxPtIter;
	//	
	//}

	switch (m_nextToolState)
	{
	case None:
		break;
	case Initialized:
		break;
	case ChosenGuideLine:
	{
		EditElementHandle _markEeh, _dotEeh;
		DVec3d _direction = _currentBtnClosestPtCurveLocationDetail.PointAndUnitTangent().Value().direction;
		if (0 == m_data->ProxyObjType) // if proxy object is dolphin like
		{
			CreateIsolatedProxyObject(_markEeh, *ACTIVEMODEL, _currentBtnClosestPtCurveLocationDetail.point, DVec2d::From(_direction.x, _direction.y));
			_indicators.Insert(_markEeh);
		}
		CreateIndicatorElementForDynamic(_dotEeh, *ACTIVEMODEL, _currentBtnClosestPtCurveLocationDetail.point);
		_indicators.Insert(_dotEeh);

		break;
	}
	case PickStartPoint:
	{
		if (SUCCESS != CreateProxyObjects(_indicators, _hoverPt))
			return;

		break;
	}
	case PickEndPoint:
	{
		break;
	}
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

BentleyStatus PDIWT::Waterway::Lock::GuidewallPlacementTool::CreateIsolatedProxyObject(EditElementHandleR eeh, DgnModelRefR model, DPoint3dCR insertPt, DVec2dCR direction)
{
	DVec2d _normalizedDirection(direction);
	_normalizedDirection.Normalize();
	ICurvePrimitivePtr _proxyObjPtr = ICurvePrimitive::CreateRectangle(-25000, 0, 25000, 50000, insertPt.z);
	_proxyObjPtr->TransformInPlace(Transform::FromOriginAndXVector(DPoint2d{ insertPt.x,insertPt.y }, _normalizedDirection));
	return DraftingElementSchema::ToElement(eeh, *_proxyObjPtr, nullptr, true, model);
}

BentleyStatus PDIWT::Waterway::Lock::GuidewallPlacementTool::CreateContinuouslyProxyObject(EditElementHandleR eeh, DgnModelRefR model, DPoint3dCR startPt, DPoint3dCR endPt)
{
	DVec3d _dvec3d = endPt - startPt;
	_dvec3d.z = 0;
	ICurvePrimitivePtr _rectProxyObjPtr = ICurvePrimitive::CreateRectangle(0, 0, _dvec3d.Magnitude(), 50000, startPt.z);
	_rectProxyObjPtr->TransformInPlace(Transform::From(RotMatrix::From1Vector(_dvec3d, 0, true), startPt));
	return DraftingElementSchema::ToElement(eeh, *_rectProxyObjPtr, nullptr, true, model);
}

BentleyStatus PDIWT::Waterway::Lock::GuidewallPlacementTool::CreateProxyObjects(ElementAgendaR eleAgenda, DPoint3dCR secondInsertPt, bool isForDynamic /*= true*/)
{
	if (!m_axis.IsValid())
		return ERROR;
	//set variables for ecschema manipulation
	DgnFileR _dgnFile = *ISessionMgr::GetActiveDgnFile();
	DgnECManagerR _ecMgr = DgnECManager::GetManager();
	const WString _ecSchemaFullName(L"PDWT_Waterway.01.00.ecschema.xml");
	const WString _ecClassName(L"ProxyObject");
	const WString _definedECSchemaFilePathVariable(L"PDIWT_ORGANIZATION_ECSCHEMAPATH");
	SchemaKey _schemakey;
	if (SUCCESS != SchemaKey::ParseSchemaFullName(_schemakey, _ecSchemaFullName.GetWCharCP())) return ERROR;
	SchemaInfo _schemaInfo(_schemakey, _dgnFile);

	//Import ecschema File
	if (!isForDynamic)
	{
		//Use the ECHelper Method
		if (SUCCESS != ECHelper::ImportECSChemaInActiveDgnBasedOnDefinedVariable(_ecSchemaFullName, _definedECSchemaFilePathVariable)) return ERROR;
	}
	// obtain closest curvelocatindetail information on axis
	CurveLocationDetail _currentBtnClosestPtCurveLocationDetail;
	m_axis->ClosestPointBounded(secondInsertPt, _currentBtnClosestPtCurveLocationDetail);
	double _uorper = ISessionMgr::GetActiveDgnModelRefP()->GetModelInfoCP()->GetUorPerMeter();
	// if proxy object is dolphin like
	if (0 == m_data->ProxyObjType)
	{
		double _uor_interval = m_data->Interval * _uorper;
		if (_currentBtnClosestPtCurveLocationDetail.fraction < m_startPointFraction)
			_uor_interval = -_uor_interval;
		bvector<CurveLocationDetail> _curveLocationDetails;

		CurveLocationDetail _forwardCurveLocation;
		// if the user chose to place by interval
		if (0 == m_data->Method)
		{
			bool _isWithinCurrentRange = true;
			size_t _numberIndex = 0;
			while (_isWithinCurrentRange)
			{
				if (m_axis->PointAtSignedDistanceFromFraction(m_startPointFraction, _uor_interval * _numberIndex, false, _forwardCurveLocation))
					_curveLocationDetails.push_back(CurveLocationDetail(_forwardCurveLocation));
				_numberIndex++;
				if (_currentBtnClosestPtCurveLocationDetail.fraction < m_startPointFraction)
				{
					_isWithinCurrentRange = _forwardCurveLocation.fraction > _currentBtnClosestPtCurveLocationDetail.fraction;
				}
				else
				{
					_isWithinCurrentRange = _forwardCurveLocation.fraction < _currentBtnClosestPtCurveLocationDetail.fraction;
				}
			}
			_curveLocationDetails.pop_back(); // to remove last one
		}
		else if (1 == m_data->Method) //if the user chose to place by number
		{
			for (size_t i = 0; i < m_data->Number; i++)
			{
				if (m_axis->PointAtSignedDistanceFromFraction(m_startPointFraction, _uor_interval * i, true, _forwardCurveLocation))
					_curveLocationDetails.push_back(CurveLocationDetail(_forwardCurveLocation));
			}
		}
		// draw proxy object and indicator if it is for dynamic.
		for each (auto _locationDetail in _curveLocationDetails)
		{
			EditElementHandle _markEeh, _dotEeh;
			DVec3d _direction = _locationDetail.PointAndUnitTangent().Value().direction;

			CreateIsolatedProxyObject(_markEeh, *ACTIVEMODEL, _locationDetail.point, DVec2d::From(_direction.x, _direction.y));

			// to determine the location relative to current line segment
			DVec3d _currentBtnToClosestAxisPointDVec = secondInsertPt - _currentBtnClosestPtCurveLocationDetail.point;
			if (_currentBtnClosestPtCurveLocationDetail.PointAndUnitTangent().Value().direction.SignedAngleTo(_currentBtnToClosestAxisPointDVec, DVec3d::UnitZ()) < 0)
			{
				Transform _mirTrans;
				_mirTrans.InitFromMirrorPlane(_locationDetail.point, DVec3d::FromNormalizedCrossProduct(DVec3d::UnitZ(), _direction));
				_markEeh.GetHandler().ApplyTransform(_markEeh, TransformInfo(_mirTrans));
			}
			if (!isForDynamic)
			{
				DgnECInstanceEnablerP _ecMainInstanceEnabler = _ecMgr.ObtainInstanceEnablerByName(_schemaInfo.GetSchemaName(), _ecClassName.GetWCharCP(), _dgnFile);
				if (!_ecMainInstanceEnabler->IsEnablerValidForDgnFile(_dgnFile))
					return ERROR;
				StandaloneECInstanceR _wipInstance = _ecMainInstanceEnabler->GetSharedWipInstance();

				_wipInstance.SetValue(L"Id", ECValue(L"ProxyObject"));
				_wipInstance.SetValue(L"FirstInsertPoint", ECValue(_locationDetail.point));
				_wipInstance.SetValue(L"SecondInsertPoint", ECValue(_locationDetail.point));
				_wipInstance.SetValue(L"Type", ECValue(m_data->ProxyObjType));

				if (!_ecMainInstanceEnabler->SupportsCreateInstanceOnElement())
					return ERROR;
				if (DgnECInstanceStatus::DGNECINSTANCESTATUS_Success != _ecMainInstanceEnabler->ScheduleInstanceOnElement(nullptr, _wipInstance, _markEeh))
					return ERROR;
			}

			eleAgenda.Insert(_markEeh);

			if (isForDynamic)
			{
				CreateIndicatorElementForDynamic(_dotEeh, *ACTIVEMODEL, _locationDetail.point);
				eleAgenda.Insert(_dotEeh);
			}
		}
	}
	else if (1 == m_data->ProxyObjType) // if proxy object is gravitational structure like
	{
		EditElementHandle _markEeh;
		DPoint3d _startPoint;
		m_axis->FractionToPoint(m_startPointFraction, _startPoint);
		if (m_startPointFraction < _currentBtnClosestPtCurveLocationDetail.fraction)
		{
			CreateContinuouslyProxyObject(_markEeh, *ACTIVEMODEL, _startPoint, _currentBtnClosestPtCurveLocationDetail.point);
		}
		else
		{
			CreateContinuouslyProxyObject(_markEeh, *ACTIVEMODEL, _currentBtnClosestPtCurveLocationDetail.point, _startPoint);
		}

		DVec3d _currentBtnToClosestAxisPointDVec = secondInsertPt - _startPoint;
		DVec3d _direction = _currentBtnClosestPtCurveLocationDetail.point - _startPoint;
		if (m_startPointFraction >= _currentBtnClosestPtCurveLocationDetail.fraction)
		{
			_direction.Negate();
		}
		if (_direction.SignedAngleTo(_currentBtnToClosestAxisPointDVec, DVec3d::UnitZ()) < 0)
		{
			Transform _mirTrans;
			_mirTrans.InitFromMirrorPlane(_currentBtnClosestPtCurveLocationDetail.point, DVec3d::FromNormalizedCrossProduct(DVec3d::UnitZ(), _direction));
			_markEeh.GetHandler().ApplyTransform(_markEeh, TransformInfo(_mirTrans));
		}

		if (!isForDynamic)
		{
			DgnECInstanceEnablerP _ecMainInstanceEnabler = _ecMgr.ObtainInstanceEnablerByName(_schemaInfo.GetSchemaName(), _ecClassName.GetWCharCP(), _dgnFile);
			StandaloneECInstanceR _wipInstance = _ecMainInstanceEnabler->GetSharedWipInstance();
			if (!_ecMainInstanceEnabler->IsEnablerValidForDgnFile(_dgnFile))
				return ERROR;

			_wipInstance.SetValue(L"Id", ECValue(L"ProxyObject"));
			_wipInstance.SetValue(L"FirstInsertPoint", ECValue(_startPoint));
			_wipInstance.SetValue(L"SecondInsertPoint", ECValue(secondInsertPt));
			_wipInstance.SetValue(L"Type", ECValue(m_data->ProxyObjType));

			if (!_ecMainInstanceEnabler->SupportsCreateInstanceOnElement())
				return ERROR;
			if (DgnECInstanceStatus::DGNECINSTANCESTATUS_Success != _ecMainInstanceEnabler->ScheduleInstanceOnElement(nullptr, _wipInstance, _markEeh))
				return ERROR;
		}

		eleAgenda.Insert(_markEeh);
	}
	// to draw second insert point indicator
	if (isForDynamic)
	{
		//Create end mark
		EditElementHandle _endMarkEeh;
		CreateIndicatorElementForDynamic(_endMarkEeh, *ACTIVEMODEL, _currentBtnClosestPtCurveLocationDetail.point, 2); //green dot
		eleAgenda.Insert(_endMarkEeh);
	}
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

