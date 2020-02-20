#pragma once

#include"stdafx.h"

USING_NAMESPACE_BENTLEY_DGNPLATFORM;
USING_NAMESPACE_BENTLEY_MSTNPLATFORM;
USING_NAMESPACE_BENTLEY_MSTNPLATFORM_ELEMENT;

namespace PDI_MDL_Common
{
	namespace stEleOperHelper
	{
		void GetTransform2Origin(EditElementHandleR eeh,
			TransformR outTr2Origin);
		void GetTransform2Origin(EditElementHandleR eeh,
			TransformR outTr2Origin,
			DPoint3dCR dpCur, RotMatrixR rotCur);
		StatusInt MoveAbsPos(
			EditElementHandle & eeh, 
			double xEle, double yEle, double zEle,
			double xDest, double yDest, double  zDest);
		StatusInt MoveAbsPos(
			EditElementHandle & eeh,
			DPoint3dCR rpEle, DPoint3dCR rpDest);
		StatusInt ApplyTransform(ElementRefP erp, Transform& tr);
		DPoint3d QueryEleOriginPos(ElementRefP erp);
		DPoint3d QueryEleOriginPos(ElementHandleCR el);
	};
}
