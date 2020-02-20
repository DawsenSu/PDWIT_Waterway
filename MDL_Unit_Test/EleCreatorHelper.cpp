#include "stdafx.h"
#include "EleCreatorHelper.h"


namespace EleCreatorHelper
{
	StatusInt CreateExtruction(EditElementHandle& eeh, double extrudeLen, DPoint3d* pSideShapePoints, int num)
	{
		const DPoint3d dOriPoint = { 0,0,0 };
		DPoint3d dirPoint = DPoint3d::From(0, extrudeLen, 0.0);
		DVec3d dirExtrude = dirPoint - dOriPoint;
		CurveVectorPtr ccp = CurveVector::Create(CurveVector::BOUNDARY_TYPE_Outer);
		for (int i = 1; i < num; ++i)
		{
			DPoint3dCR pPre = pSideShapePoints[i - 1];
			DPoint3dCR pCur = pSideShapePoints[i];
			DSegment3d ds;
			ds.Init(pPre, pCur);
			ccp->push_back(
				ICurvePrimitive::CreateLine(ds)
			);
		}

		DgnExtrusionDetail exDetail(ccp, dirExtrude, true);
		ISolidPrimitivePtr solidExtrusion = ISolidPrimitive::CreateDgnExtrusion(exDetail);

		EditElementHandle eehProfile;
		if (SUCCESS != DraftingElementSchema::ToElement(eehProfile, *exDetail.m_baseCurve, NULL, true, *ACTIVEMODEL))
			return ERROR;

		bool        capped = true;
		DPoint3d    origin;
		exDetail.m_baseCurve->GetStartPoint(origin); // humbug. The origin arg for CreateProjectionElement is unused, but make it accurate.

		return SurfaceOrSolidHandler::CreateProjectionElement(eeh, nullptr, eehProfile, origin, dirExtrude, NULL, capped, *ACTIVEMODEL);

	}
};