#include "GuidewallP3Tool.h"
PDIWT_WATERWAY_LOCK_NAMESPACE_BEGIN

BentleyStatus PDIWT::Waterway::Lock::GuidewallP3Tool::CreateDolphinProfile(CurveVectorPtr& curveVec, DgnModelRefR model)
{
	double _uorperM = model.GetModelInfoCP()->GetUorPerMeter();
	bvector<DVec3d> _segmentsDVec3d =
	{
		DVec3d::From(m_data.frontToeWidth,m_data.frontToeHeight),
		DVec3d::From(0,m_data.dolphinHeight - m_data.frontToeHeight),
		DVec3d::From(m_data.dolphinTopWidth,0),
		DVec3d::From(m_data.firstBackwallSegHeight*m_data.firstBackwallSegAngle.Sin(),-m_data.firstBackwallSegHeight),
		DVec3d::From((m_data.dolphinHeight - m_data.firstBackwallSegHeight) * m_data.secondBackwallSegAngle.Sin(),-(m_data.dolphinHeight - m_data.firstBackwallSegHeight))
	};
	bvector<DPoint3d> _pts = { DPoint3d::From(0,0) };
	for (size_t i = 0; i < _segmentsDVec3d.size(); i++)
	{
		_segmentsDVec3d.at(i).Scale(_uorperM);
		_pts.push_back(_pts.at(i) + _segmentsDVec3d.at(i));
	}
	curveVec = CurveVector::CreateLinear(_pts, CurveVector::BOUNDARY_TYPE_Outer);

	return SUCCESS;
}


bool GuidewallP3Tool::_OnInstall()
{
	if (!__super::_OnInstall()) return false;
	if (!ISessionMgr::GetActiveDgnModelP()->Is3d()) return false;
	return true;
}

void GuidewallP3Tool::_OnPostInstall()
{
	m_wpfform = gcnew BMWPF::ToolSettingsHost();
	m_wpfform->Width = 250;
	m_wpfform->Height = 300;
	m_wpfform->Title = L"Guide wall P3 Create Tool";
	m_wpfform->Content = gcnew UI::GuidewallP3View(Program::Addin);
	m_wpfform->Attach(Program::Addin);
	__super::_OnPostInstall();
}

bool GuidewallP3Tool::_OnDataButton(DgnButtonEventCR ev)
{
	double _uorperMeter = ISessionMgr::GetActiveDgnModelRefP()->GetModelInfoCP()->GetUorPerMeter(); 
	size_t _numberOfSolidCreationFail = 0;
	switch (m_toolState)
	{
	case PDIWT::Waterway::Lock::GuidewallP3Tool::Initialized:
	{
		//! Set the data from UI
		SetData();

		_LocateOneElement(ev, true);

		ElemAgendaEntry* _firstEle = GetElementAgenda().begin();
		if (_firstEle == nullptr) return false;
		CurveVectorPtr _axis = ICurvePathQuery::ElementToCurveVector(*_firstEle);

#ifdef LOG
		mdlDialog_dmsgsPrint(WPrintfString(L"Number of Primitives are %d", _axis->size()));
#endif // LOG		
		CurveVectorWithDistanceIndexPtr _CVWDIPtr = CurveVectorWithDistanceIndex::Create();
		_CVWDIPtr->SetPath(_axis);

		CurveVectorPtr _profile;
		CreateDolphinProfile(_profile, *ACTIVEMODEL);
		//_profile->TransformInPlace(*_trans.GetTransform());
		bvector < std::tuple<int, double, int, double>> _pathIndexFractionTuples;
		size_t _searchIndex = 1;
		PathLocationDetail _pathLocationDetail;
		PathLocationDetail _pathEndLocationDetail;
		int _startIndex = 0, _endIndex = 0;
		double  _startFraction = 0, _endFraction = 0;

		while (_CVWDIPtr->SearchByDistanceFromPathStart(m_data.interval * _uorperMeter * _searchIndex, _pathLocationDetail) && !(_pathLocationDetail.GetPrimitiveIndex() == _axis->size() && _pathLocationDetail.GetCurveLocationDetail().fraction >= 1))
		{
			_CVWDIPtr->SearchByDistanceFromPathStart((m_data.interval*_searchIndex - m_data.gapwidth) * _uorperMeter, _pathEndLocationDetail);
			_endIndex = _pathEndLocationDetail.GetPrimitiveIndex() -1;
			_endFraction = _pathEndLocationDetail.GetCurveLocationDetail().fraction;
			_pathIndexFractionTuples.push_back(std::make_tuple(_startIndex, _startFraction, _endIndex, _endFraction));
			_startIndex = _pathLocationDetail.GetPrimitiveIndex() -1;
			_startFraction = _pathLocationDetail.GetCurveLocationDetail().fraction;
			_searchIndex++;
		}
		_pathIndexFractionTuples.push_back(std::make_tuple(_startIndex, _startFraction, (int)_axis->size(), 1.0));

		for each (auto tuple in _pathIndexFractionTuples)
		{
			std::tie(_startIndex, _startFraction, _endIndex, _endFraction) = tuple;

#ifdef LOG
			mdlDialog_dmsgsPrint(WPrintfString(L"[%d,%f |%d,%f ]", _startIndex, _startFraction, _endIndex, _endFraction));
#endif // LOG
			CurveVectorPtr _subPathVec = _axis->CloneBetweenDirectedFractions(_startIndex, _startFraction, _endIndex, _endFraction, false);

			//! Get the right transformed profile
			DPoint3d _startPoint, _endPoint;
			DVec3d _startTan, _endTan;
			_subPathVec->GetStartEnd(_startPoint, _endPoint, _startTan, _endTan);
			if (_startTan.z != 0)
			{
#ifdef LOG
				mdlOutput_messageCenter(OutputMessagePriority::Fatal, L"The start point tangent of selected wire element is not parallel to xy plane", L"", OutputMessageAlert::Balloon);
#endif // LOG
				continue;
			}
			DVec3d _subPath_xvec = DVec3d::FromCrossProduct(DVec3d::UnitZ(), _startTan);
			RotMatrix _profileRot = RotMatrix::From2Vectors(_subPath_xvec, DVec3d::UnitZ());

			CurveVectorPtr _subprofile = _profile->Clone();
			_subprofile->TransformInPlace(Transform::From(_profileRot, _startPoint));

			//! create solid entity.
			ISolidKernelEntityPtr _sweepSolid;
			//EditElementHandle _profileEEh;
			//DraftingElementSchema::ToElement(_profileEEh, *_subprofile, nullptr, true, *ACTIVEMODEL);
			//_profileEEh.AddToModel();
			if (SUCCESS != SolidUtil::Create::BodyFromSweep(_sweepSolid, *_subprofile, *_subPathVec, *ACTIVEMODEL, false, true, false))
			{
#ifdef LOG
				mdlOutput_messageCenter(OutputMessagePriority::Warning, WPrintfString(L"SolidKernelEntity [Start Fraction: %f, End Fraction: %f] can't be created!", _startFraction, _endFraction), WPrintfString(L"[%d,%f |%d,%f ]", _startIndex, _startFraction, _endIndex, _endFraction), OutputMessageAlert::None);
#endif // LOG
				continue;
			}
			EditElementHandle _eeh;
			if (SUCCESS != DraftingElementSchema::ToElement(_eeh, *_sweepSolid, nullptr, *ACTIVEMODEL))
			{
#ifdef LOG
				mdlOutput_messageCenter(OutputMessagePriority::Warning, WPrintfString(L"Solid Between [Start Fraction: %f, End Fraction: %f] can't be created!", _startFraction, _endFraction), WPrintfString(L"[%d,%f |%d,%f ]", _startIndex, _startFraction, _endIndex, _endFraction), OutputMessageAlert::None);
#endif // LOG
				_numberOfSolidCreationFail++;
				continue;
			}
			_eeh.AddToModel();
		}
#ifdef LOG
		mdlOutput_messageCenter(OutputMessagePriority::Info, WPrintfString(L"Solid Creation process finished: %d success, %d fail.", _pathIndexFractionTuples.size() - _numberOfSolidCreationFail, _numberOfSolidCreationFail), L"", OutputMessageAlert::None);
#endif // LOG
		

	}
	case PDIWT::Waterway::Lock::GuidewallP3Tool::ChosenGuideLine:
		break;
	default:
		break;
	}
	return true;
}


void PDIWT::Waterway::Lock::GuidewallP3Tool::_OnCleanup()
{
	m_wpfform->Detach();
	delete m_wpfform;
	__super::_OnCleanup();
}

bool PDIWT::Waterway::Lock::GuidewallP3Tool::_OnPostLocate(HitPathCP path, WStringR cantAcceptReason)
{
	if (!DgnElementSetTool::_OnPostLocate(path, cantAcceptReason))
		return false;

	EditElementHandle eeh(path->GetHeadElem(), path->GetRoot());

	CurveVectorPtr curve = ICurvePathQuery::ElementToCurveVector(eeh);
	if (!curve.IsValid())
	{
		cantAcceptReason = L"This element can't be converted to curveVector";
		return false;
	}
	return true;
}

void PDIWT::Waterway::Lock::GuidewallP3Tool::InstallNewInstance()
{
	GuidewallP3Tool* _tool = new GuidewallP3Tool();
	_tool->InstallTool();
}

void PDIWT::Waterway::Lock::GuidewallP3Tool::SetData()
{
	UI::GuidewallP3View^ _view = dynamic_cast<UI::GuidewallP3View^>(m_wpfform->Content);
	UI::Model::GuidewallP3Data^ _data = _view->GetData();
	m_data = NativeGuideWallP3Data{ _data->FrontToeHeight,_data->FrontToeWidth,_data->DolphinHeight,_data->DolphinTopWidth, Angle::FromDegrees(_data->FirstBackwallSegAngle),_data->FirstBackwallSegHeight,Angle::FromDegrees(_data->SecondBackwallAngle),_data->Interval,_data->GapWidth };
}


PDIWT_WATERWAY_LOCK_NAMESPACE_END