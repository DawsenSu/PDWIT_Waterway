#include "RailTool.h"

bool PDIWT::Waterway::Lock::RailTool::_OnInstall()
{
	if (!__super::_OnInstall()) return false;
	if (!ISessionMgr::GetActiveDgnModelP()->Is3d()) return false;
	return true;
}

void PDIWT::Waterway::Lock::RailTool::_OnPostInstall()
{
	__super::_OnPostInstall();
}

bool PDIWT::Waterway::Lock::RailTool::_OnDataButton(DgnButtonEventCR ev)
{
	_LocateOneElement(ev, true);
	ElemAgendaEntry* _firstEle = GetElementAgenda().begin();
	if (_firstEle == nullptr) return false;
	CurveVectorPtr _axis = ICurvePathQuery::ElementToCurveVector(*_firstEle);
	_axis->ConsolidateAdjacentPrimitives();
	TypeBRailData _data{ 0.045, 2, 1.2, 0.038, 0.6, 0.075, 0.02, 0.14, 0.008, 0.01 };

	CreateTypeBRail(_axis->front(), _data, *ACTIVEMODEL);

	return true;
}

bool PDIWT::Waterway::Lock::RailTool::_OnPostLocate(HitPathCP path, WStringR cantAcceptReason)
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
	Transform _localToWorld, _worldToLocal;
	DRange3d _range;
	if (!curve->IsPlanar(_localToWorld, _worldToLocal, _range))
	{
		cantAcceptReason = L"This curve is not planar";
		return false;
	}
	return true;
}

void PDIWT::Waterway::Lock::RailTool::InstallNewInstance()
{
	RailTool* _tool = new RailTool();
	_tool->InstallTool();
}



BentleyStatus PDIWT::Waterway::Lock::RailTool::CreateTypeBRail(ICurvePrimitivePtr axis, TypeBRailDataCR data, DgnModelRefR model)
{
	double _uorperMeter = model.GetModelInfoCP()->GetUorPerMeter();
	TypeBRailData _uorData = data;
	_uorData.Scale(_uorperMeter);


	std::set <CurveLocationDetail, PDIWT::Waterway::Lock::CurveLocationCompare> _postSet;

	CurveLocationDetail _locationDetail;
	size_t _index = 0;
	while (axis->PointAtSignedDistanceFromFraction(0, _uorData.postSpan*_index, false, _locationDetail))
	{
		_postSet.insert(_locationDetail);
		_index++;
		if (_locationDetail.fraction == 1) break;
	}

	if (ICurvePrimitive::CurvePrimitiveType::CURVE_PRIMITIVE_TYPE_LineString == axis->GetCurvePrimitiveType())
	{
		bvector<DPoint3d> const* _vertexs = axis->GetLineStringCP();
		for each (auto _vertex in *_vertexs)
		{
			CurveLocationDetail _vertexLocation;
			axis->ClosestPointBounded(_vertex, _vertexLocation);
			_postSet.insert(_vertexLocation);
		}
	}

	//! filter the location where the distance of adjacent posts is less than diagonal distance of base plate, corner post will remain.
	std::set <CurveLocationDetail, PDIWT::Waterway::Lock::CurveLocationCompare> _filteredPostSet;
	double _uorBasePlateDiagonalDistance = sqrt(2) *_uorData.postRailDiameter + 2 * _uorData.reinforcePlateBottomWidth;
	for (auto _itr = _postSet.begin(); _itr != _postSet.end(); _itr++)
	{
		auto _itrnext = _itr; 		_itrnext++;
		if (_itrnext != _postSet.end())
		{
			double _postdistance = _itr->Distance(*_itrnext);
			if (_postdistance >= _uorBasePlateDiagonalDistance)
			{
				_filteredPostSet.emplace(*_itr);
			}
			else
			{
				if (_itr->componentFraction == 1)
				{
					_filteredPostSet.emplace(*_itr);
					_itr = _itrnext;
				}
			}
		}
		else
		{
			_filteredPostSet.emplace(*_itr);
		}
	}


#ifdef DEBUG
	mdlDialog_dmsgsPrint(WPrintfString(L"Number of set is: %d", _filteredPostSet.size()));
	for each (auto _info in _filteredPostSet)
	{
		mdlDialog_dmsgsPrint(WPrintfString(L"Fraction: %f, componentIndex: %d, componentFraction: %f", _info.fraction, _info.componentIndex, _info.componentFraction));
	}
#endif // DEBUG

	//! Create a Rail Post prototype
	DVec3d _postTopDev = DVec3d::From(0, 0, _uorData.postHeight);
	ReinforePlate _reinforcePlate(data.reinforcePlateBottomWidth, data.reinforcePlateTopWidth, data.reinforcePlateHeight, data.reinforceThickness);

	DgnConeDetail _coneDetail(DPoint3d::FromZero(), DPoint3d::From(0, 0, _uorData.postHeight), _uorData.postRailDiameter / 2, _uorData.postRailDiameter / 2, true);
	ISolidPrimitivePtr _postPrimtive = ISolidPrimitive::CreateDgnCone(_coneDetail);
	ISolidKernelEntityPtr _postEntity;
	if (SUCCESS != SolidUtil::Create::BodyFromSolidPrimitive(_postEntity, *_postPrimtive, model))
	{
#ifdef DEBUG
		mdlDialog_dmsgsPrint(WPrintfString(L"Can't create post"));
#endif // DEBUG
		return ERROR;
	}

	EditElementHandle _reinforcePlateEeh, _reinforcePlateEeh1, _reinforcePlateEeh2, _reinforcePlateEeh3;
	if (SUCCESS != _reinforcePlate.Create(_reinforcePlateEeh, model) || SUCCESS != _reinforcePlate.Create(_reinforcePlateEeh1, model) || SUCCESS != _reinforcePlate.Create(_reinforcePlateEeh2, model) || SUCCESS != _reinforcePlate.Create(_reinforcePlateEeh3, model))
	{
#ifdef DEBUG
		mdlDialog_dmsgsPrint(WPrintfString(L"Can't create reinforce plate"));
#endif // DEBUG
		return ERROR;
	}
	_reinforcePlateEeh.GetHandler().ApplyTransform(_reinforcePlateEeh, TransformInfo(Transform::From(DPoint3d::From(_uorData.postRailDiameter / 2, 0, 0))));
	_reinforcePlateEeh1.GetHandler().ApplyTransform(_reinforcePlateEeh1, TransformInfo(Transform::From(RotMatrix::FromAxisAndRotationAngle(2, Angle::PiOver2()))*Transform::From(DPoint3d::From(_uorData.postRailDiameter / 2, 0, 0))));
	_reinforcePlateEeh2.GetHandler().ApplyTransform(_reinforcePlateEeh2, TransformInfo(Transform::From(RotMatrix::FromAxisAndRotationAngle(2, Angle::Pi()))*Transform::From(DPoint3d::From(_uorData.postRailDiameter / 2, 0, 0))));
	_reinforcePlateEeh3.GetHandler().ApplyTransform(_reinforcePlateEeh3, TransformInfo(Transform::From(RotMatrix::FromAxisAndRotationAngle(2, Angle::FromDegrees(-90).Radians()))*Transform::From(DPoint3d::From(_uorData.postRailDiameter / 2, 0, 0))));

	ISolidKernelEntityPtr _reinforcePlateEntity, _reinforcePlateEntity1, _reinforcePlateEntity2, _reinforcePlateEntity3;
	if (SUCCESS != SolidUtil::Convert::ElementToBody(_reinforcePlateEntity, _reinforcePlateEeh) || SUCCESS != SolidUtil::Convert::ElementToBody(_reinforcePlateEntity1, _reinforcePlateEeh1) || SUCCESS != SolidUtil::Convert::ElementToBody(_reinforcePlateEntity2, _reinforcePlateEeh2) || SUCCESS != SolidUtil::Convert::ElementToBody(_reinforcePlateEntity3, _reinforcePlateEeh3))
	{
#ifdef DEBUG
		mdlDialog_dmsgsPrint(WPrintfString(L"Can't convert reinforce plate to solid entity"));
#endif // DEBUG
		return ERROR;
	}

	double _uorBasePlateSideLength = _uorData.postRailDiameter + 2 * _uorData.reinforcePlateBottomWidth;
	DgnBoxDetail _basePlateDetial(DPoint3d::From(-_uorBasePlateSideLength / 2, -_uorBasePlateSideLength / 2, -_uorData.basePlateThickness), DPoint3d::From(-_uorBasePlateSideLength / 2, -_uorBasePlateSideLength / 2, 0), DVec3d::UnitX(), DVec3d::UnitY(), _uorBasePlateSideLength, _uorBasePlateSideLength, _uorBasePlateSideLength, _uorBasePlateSideLength, true);
	ISolidPrimitivePtr _basePlatePrimitivePtr = ISolidPrimitive::CreateDgnBox(_basePlateDetial);
	ISolidKernelEntityPtr _basePlateEntity;
	if (SUCCESS != SolidUtil::Create::BodyFromSolidPrimitive(_basePlateEntity, *_basePlatePrimitivePtr, model))
	{
#ifdef DEBUG
		mdlDialog_dmsgsPrint(WPrintfString(L"Can't create base plate"));
#endif // DEBUG
		return ERROR;
	}
	bvector<ISolidKernelEntityPtr> _tools = { _reinforcePlateEntity,_reinforcePlateEntity1,_reinforcePlateEntity2,_reinforcePlateEntity3, };

	if (SUCCESS != SolidUtil::Modify::BooleanUnion(_basePlateEntity, _tools.data(), _tools.size()))
	{
#ifdef DEBUG
		mdlDialog_dmsgsPrint(WPrintfString(L"Can't create union post"));
#endif // DEBUG
		return ERROR;
	}

	EditElementHandle _postEeh, _basePlateEeh;
	if (SUCCESS != DraftingElementSchema::ToElement(_postEeh, *_postEntity, nullptr, model) || SUCCESS != DraftingElementSchema::ToElement(_basePlateEeh, *_basePlateEntity, nullptr, model))
	{
#ifdef DEBUG
		mdlDialog_dmsgsPrint(WPrintfString(L"Can't convert post and base plate"));
#endif // DEBUG
		return ERROR;
	}

	//ElementCopyContext _elementCopyContext = ElementCopyContext(&model);
	if (_filteredPostSet.size() > 2)
	{
		for (auto _itr = ++_filteredPostSet.begin(); _itr != --_filteredPostSet.end(); _itr++)
		{
			DPoint3dCR _basePt = _itr->point;
			DVec3d _locationUnitTangent = _itr->PointAndUnitTangent().Value().direction;
			_locationUnitTangent.z = 0;
			TransformInfo _trans = TransformInfo(Transform::From(_basePt) * Transform::From(RotMatrix::From1Vector(_locationUnitTangent, 0, true)));

			EditElementHandle _currentPostEeh(_postEeh, true), _currentBasePlate(_basePlateEeh, true);
			_currentPostEeh.GetHandler().ApplyTransform(_currentPostEeh, _trans);
			_currentBasePlate.GetHandler().ApplyTransform(_currentBasePlate, _trans);
			_currentPostEeh.AddToModel();
			_currentBasePlate.AddToModel();
		}
	}
	//! Build middle rail
	ICurvePrimitivePtr _middleRailAxis = axis->Clone();
	_middleRailAxis->TransformInPlace(Transform::From(DVec3d::From(0, 0, _uorData.mideleRailHeight)));

	CurveVectorPtr _middleRailPath = CurveVector::Create(_middleRailAxis);

	auto _firstLocation = _filteredPostSet.begin(), _lastLocation = --_filteredPostSet.end();

	CurveVectorPtr _middleRailProfile = CurveVector::CreateDisk(DEllipse3d::FromCenterNormalRadius(_firstLocation->point + DVec3d::From(0, 0, _uorData.mideleRailHeight), _firstLocation->PointAndUnitTangent().Value().direction, _uorData.middleRailDiameter / 2));

	ISolidKernelEntityPtr _middleRailEntity;
	if (SUCCESS != SolidUtil::Create::BodyFromSweep(_middleRailEntity, *_middleRailProfile, *_middleRailPath, model, false, true, false))
	{
#ifdef DEBUG
		mdlDialog_dmsgsPrint(L"Can't create middle rail");
#endif // DEBUG
		return ERROR;
	}
	EditElementHandle _middleRailEeh;
	if (SUCCESS != DraftingElementSchema::ToElement(_middleRailEeh, *_middleRailEntity, nullptr, model))
	{
#ifdef DEBUG
		mdlDialog_dmsgsPrint(L"Can't Convert middle rail to editelementhandle");
#endif // DEBUG
		return ERROR;
	}
	_middleRailEeh.AddToModel();
	//! Build handrail

	double _cornerAr_Radius_railDiameter_ratio = 3.6;
	DVec3d _firstLocationTrangent_WithScale = _firstLocation->PointAndUnitTangent().Value().direction;
	_firstLocationTrangent_WithScale.Scale(_uorData.postRailDiameter*_cornerAr_Radius_railDiameter_ratio);
	Dpoint3d _firstPostControlPt = _firstLocation->point + DVec3d::From(0, 0, _uorData.postHeight - _uorData.postRailDiameter * _cornerAr_Radius_railDiameter_ratio);
	DPoint3d _firstRailControlPt = _firstLocation->point + DVec3d::From(0, 0, _uorData.postHeight) + _firstLocationTrangent_WithScale;
	DEllipse3d _firstCornerArc;
	_firstCornerArc.InitArcFromPointTangentPoint(_firstPostControlPt, DVec3d::UnitZ(), _firstRailControlPt);

	DVec3d _lastLocationTrangent_WithScale = _lastLocation->PointAndUnitTangent().Value().direction;
	_lastLocationTrangent_WithScale.Scale(_uorData.postRailDiameter * _cornerAr_Radius_railDiameter_ratio);
	DPoint3d _lastPostControlPt = _lastLocation->point + DVec3d::From(0, 0, _uorData.postHeight - _uorData.postRailDiameter * _cornerAr_Radius_railDiameter_ratio);
	DPoint3d _lastRailControlPt = _lastLocation->point + DVec3d::From(0, 0, _uorData.postHeight) - _lastLocationTrangent_WithScale;
	DEllipse3d _lastCornerArc;
	_lastCornerArc.InitArcFromPointTangentPoint(_lastRailControlPt, _lastLocation->PointAndUnitTangent().Value().direction, _lastPostControlPt);

	bvector<DPoint3d> _railControlPts;
	if (ICurvePrimitive::CurvePrimitiveType::CURVE_PRIMITIVE_TYPE_Line == axis->GetCurvePrimitiveType())
	{
		_railControlPts.push_back(_firstRailControlPt);
		_railControlPts.push_back(_lastRailControlPt);
	}
	else if (ICurvePrimitive::CurvePrimitiveType::CURVE_PRIMITIVE_TYPE_LineString == axis->GetCurvePrimitiveType())
	{
		_railControlPts.push_back(_firstRailControlPt);
		bvector<DPoint3d> const* _lingstrPts = axis->GetLineStringCP();
		for (size_t i = 1; i < _lingstrPts->size() - 1; i++)
		{
			_railControlPts.push_back(_lingstrPts->at(i) + DVec3d::From(0, 0, _uorData.postHeight));
		}
		_railControlPts.push_back(_lastRailControlPt);
	}
	else
	{
#ifdef DEBUG
		mdlDialog_dmsgsPrint(L"The chose guide line is not supported yet");
#endif // DEBUG
		return ERROR;
	}
	ICurvePrimitivePtr _firstPostAxis = ICurvePrimitive::CreateLine(DSegment3d::From(_firstLocation->point, _firstPostControlPt)),
		_firstArcAxis = ICurvePrimitive::CreateArc(_firstCornerArc);
	ICurvePrimitivePtr _railHorizontal = ICurvePrimitive::CreateLineString(_railControlPts);

	ICurvePrimitivePtr _lastPostAxis = ICurvePrimitive::CreateLine(DSegment3d::From(_lastPostControlPt, _lastLocation->point)),
		_lastArcAxis = ICurvePrimitive::CreateArc(_lastCornerArc);

	CurveVectorPtr _railPath = CurveVector::Create(_firstPostAxis);

	_railPath->Add(_firstArcAxis);
	_railPath->Add(_railHorizontal);
	_railPath->Add(_lastArcAxis);
	_railPath->Add(_lastPostAxis);

	//double _filletsArcRaidus = _uorData.postRailDiameter * _cornerAr_Radius_railDiameter_ratio / 2;
	//ICurvePrimitivePtr _firstPostPrimitive = ICurvePrimitive::CreateLineString({ _firstLocation->point, _firstLocation->point + DVec3d::From(0,0,_uorData.postHeight) });
	//ICurvePrimitivePtr _lastPostPrimitive = ICurvePrimitive::CreateLineString({ _lastLocation->point + DVec3d::From(0,0,_uorData.postHeight),_lastLocation->point });
	//ICurvePrimitivePtr _railPrimitive = axis->Clone();
	//_railPrimitive->TransformInPlace(Transform::From(DPoint3d::From(0, 0, _uorData.postHeight)));

	//bvector<CurveCurve::FilletDetail> _firstFilletDetails;
	//CurveCurve::CollectFilletArcs(*_firstPostPrimitive, *_railPrimitive, _filletsArcRaidus, false, _firstFilletDetails);
	//for each (auto _detail in _firstFilletDetails)
	//{
	//	CurveVectorPtr _curve = CurveVector::Create(CurveVector::BOUNDARY_TYPE_Open);
	//	_curve->Add(_detail.detailA.curve->CloneBetweenFractions(0, _detail.detailA.fraction, false));
	//	_curve->Add(ICurvePrimitive::CreateArc(_detail.arc));
	//	_curve->Add(_detail.detailB.curve->CloneBetweenFractions(_detail.detailB.fraction,1, false));

	//	EditElementHandle _testEEH;
	//	DraftingElementSchema::ToElement(_testEEH, *_curve, nullptr, true, model);
	//	_testEEH.AddToModel();
	//}


	CurveVectorPtr _railProfile = CurveVector::CreateDisk(DEllipse3d::FromCenterNormalRadius(_firstLocation->point, DVec3d::UnitZ(), _uorData.postRailDiameter / 2));
	ISolidKernelEntityPtr _railEntity;
	if (SUCCESS != SolidUtil::Create::BodyFromSweep(_railEntity, *_railProfile, *_railPath, model, false, true, false))
	{
#ifdef DEBUG
		mdlDialog_dmsgsPrint(L"Can't create rail");
#endif // DEBUG
		return ERROR;
	}
	EditElementHandle _railEeh;
	if (SUCCESS != DraftingElementSchema::ToElement(_railEeh, *_railEntity, nullptr, model))
	{
#ifdef DEBUG
		mdlDialog_dmsgsPrint(L"Can't Convert rail to editelementhandle");
#endif // DEBUG
		return ERROR;
	}
	_railEeh.AddToModel();
	//! First and last posts' basePlates
	DVec3d _firstlocationUnitTangent = _firstLocation->PointAndUnitTangent().Value().direction;
	_firstlocationUnitTangent.z = 0;
	TransformInfo _firstPostBasePlateTrans = TransformInfo(Transform::From(_firstLocation->point) * Transform::From(RotMatrix::From1Vector(_firstlocationUnitTangent, 0, true)));
	DVec3d _LastlocationUnitTangent = _lastLocation->PointAndUnitTangent().Value().direction;
	_LastlocationUnitTangent.z = 0;
	TransformInfo _lastPostBasePlateTrans = TransformInfo(Transform::From(_lastLocation->point) * Transform::From(RotMatrix::From1Vector(_LastlocationUnitTangent, 0, true)));

	EditElementHandle  _firstPostBasePlate(_basePlateEeh, true), _lastPostBasePlate(_basePlateEeh, true);
	_firstPostBasePlate.GetHandler().ApplyTransform(_firstPostBasePlate, _firstPostBasePlateTrans);
	_lastPostBasePlate.GetHandler().ApplyTransform(_lastPostBasePlate, _lastPostBasePlateTrans);
	_firstPostBasePlate.AddToModel();
	_lastPostBasePlate.AddToModel();
	return SUCCESS;
}
