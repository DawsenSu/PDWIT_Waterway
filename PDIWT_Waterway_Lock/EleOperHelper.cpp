#include "EleOperHelper.h"
#include <DgnPlatform\DgnPlatformApi.h>

namespace PDI_MDL_Common
{
	namespace stEleOperHelper
	{
		StatusInt MoveAbsPos(
			EditElementHandle & eeh,
			double xEle, double yEle, double zEle,
			double xDest, double yDest, double  zDest)
		{
			DPoint3d dEle = { xEle, yEle, zEle };
			DPoint3d dDest = { xDest,yDest,zDest };
			//mdlDialog_dmsgsPrint(L"Create Counter = 0");
			StatusInt rtl = MoveAbsPos(eeh,  dEle, dDest);
			return rtl;
		}

		StatusInt MoveAbsPos(
			EditElementHandle & eeh,
			DPoint3dCR rpEle, DPoint3dCR rpDest)
		{
			//mdlDialog_dmsgsPrint(L"Create Counter = 0");
			DVec3d      vec;
			// Setup translation from accept point to this button event point.
			vec.DifferenceOf(rpDest, rpEle);

			Transform tran;
			tran.InitFrom(vec);
			TransformInfo tri(tran);
			StatusInt rtl = eeh.GetHandler().ApplyTransform(eeh, tri);
			//trans.From(vec);
			return rtl;
		}

		StatusInt ApplyTransform(ElementRefP erp, Transform& tr)
		{
			TransformInfo tri(tr);
			DgnModelRefP mr = erp->GetDgnModelP();
			EditElementHandle eeh(erp, mr);
			StatusInt rtl = eeh.GetHandler().ApplyTransform(eeh, tri);
			if (rtl != SUCCESS)
				return rtl;
			rtl = eeh.ReplaceInModel(erp);
			return rtl;
		}

		DPoint3d QueryEleOriginPos(ElementRefP erp)
		{
			ElementHandle eh(erp);
			return QueryEleOriginPos(eh);
		}

		DPoint3d QueryEleOriginPos(ElementHandleCR el)
		{
			DPoint3d d3dOrigin;
			DisplayHandler& dh = DisplayHandler::GetInstance();
			dh.GetTransformOrigin(el, d3dOrigin);

			return d3dOrigin;
		}
	}
}