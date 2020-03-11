#include "Common.h"

PDIWT_NAMESPACE_BEGIN
CurveVectorPtr CuvreVectorExtension::CreateFilletArcCurve(bvector<DPoint3d>& pointsOfLineString, bvector<size_t>& FilletArcPointIndex, double radius)
{
	if (pointsOfLineString.size() < 3) return nullptr;

	//make unique index vector
	bvector<size_t> _uniqueSortedIndexs(FilletArcPointIndex.begin(), FilletArcPointIndex.end());
	std::sort(_uniqueSortedIndexs.begin(), _uniqueSortedIndexs.end());
	std::unique(_uniqueSortedIndexs.begin(), _uniqueSortedIndexs.end());
	auto _firstNoErase = std::find_if_not(_uniqueSortedIndexs.begin(), _uniqueSortedIndexs.end(), [](size_t x) { return x < 1; });
	auto _lastNoErase = std::find_if(_uniqueSortedIndexs.begin(), _uniqueSortedIndexs.end(), [&pointsOfLineString](size_t x) {return x > pointsOfLineString.size() - 2; });
	bvector<size_t> _index(_firstNoErase, _lastNoErase);
#ifdef DEBUG
	mdlDialog_dmsgsPrint(WPrintfString(L"Number of index is: %d", _index.size()));
	for each (auto _i in _index)
	{
		mdlDialog_dmsgsPrint(WPrintfString(L"%d", _i));

	}
#endif // DEBUG

	//Prepare all lines
	bvector<ICurvePrimitivePtr> _lineSegments;
	for (size_t i = 0; i < pointsOfLineString.size() - 1; i++)
	{
		_lineSegments.push_back(ICurvePrimitive::CreateLine(DSegment3d::From(pointsOfLineString[i], pointsOfLineString[i + 1])));
	}
#ifdef DEBUG
	mdlDialog_dmsgsPrint(WPrintfString(L"Number of LineString is: %d", _lineSegments.size()));
#endif // DEBUG
	bvector<std::tuple<size_t, ICurvePrimitivePtr>> _addedArcTuples;
	for (size_t i = 0; i < _index.size(); i++)
	{
		bvector<CurveCurve::FilletDetail> _filletDetails;
		CurveCurve::CollectFilletArcs(*_lineSegments[_index[i] - 1], *_lineSegments[_index[i]], radius, false, _filletDetails);
		if (_filletDetails.size() == 1)
		{
			_lineSegments[_index[i] - 1] = _lineSegments[_index[i] - 1]->CloneBetweenFractions(0, _filletDetails.at(0).detailA.fraction, false);
			_lineSegments[_index[i]] = _lineSegments[_index[i]]->CloneBetweenFractions(_filletDetails.at(0).detailB.fraction, 1, false);
			_addedArcTuples.push_back(std::make_tuple(_index[i], ICurvePrimitive::CreateArc(_filletDetails.at(0).arc)));

		}
	}
	
#ifdef DEBUG
	//for each (auto _arc in _addedArcTuples)
	//{
	//	EditElementHandle _eeh;
	//	DraftingElementSchema::ToElement(_eeh, *std::get<1>(_arc), nullptr, true, *ACTIVEMODEL);
	//	_eeh.AddToModel();
	//}
#endif // DEBUG
	CurveVectorPtr _modifiedCurve = CurveVector::Create(CurveVector::BOUNDARY_TYPE_Open);
	for (size_t i = 0; i < _lineSegments.size(); i++)
	{
		_modifiedCurve->Add(_lineSegments[i]);
		auto _arcItr = std::find_if(_addedArcTuples.begin(), _addedArcTuples.end(), [&i](std::tuple<size_t, ICurvePrimitivePtr> arc) {return (i + 1) == std::get<0>(arc); });
		if (_arcItr != _addedArcTuples.end())
			_modifiedCurve->Add(std::get<1>(*_arcItr));
	}
	_modifiedCurve->ConsolidateAdjacentPrimitives();
	return _modifiedCurve;
}
PDIWT_NAMESPACE_END