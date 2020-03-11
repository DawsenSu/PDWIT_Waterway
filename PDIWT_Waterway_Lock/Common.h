#pragma once
#include "stdafx.h"

PDIWT_NAMESPACE_BEGIN

struct CuvreVectorExtension
{
	static CurveVectorPtr CreateFilletArcCurve(bvector<DPoint3d>& pointsOfLineString, bvector<size_t>& FilletArcPointIndex, double radius);
};

PDIWT_NAMESPACE_END

