#pragma once

#include "stdafx.h"
#include "EleOperHelper.h"

using namespace PDI_MDL_Common;

namespace EleCreatorHelper
{
	StatusInt CreateExtruction(EditElementHandle& eeh,
		double extrudeLen, DPoint3d* pSideShapePoints, int num);

	typedef bool(*CallbackCreateElement)(EditElementHandle & eeh,
		void * pData);

	template<class TData>
	class CreatorHelper
	{
		static TData stData;
		static double stPosX4CreateTest;

		static int stIndexPara;
		static double stParaValue;
		static double stXOffStep;

	public:
		static bool ParserPara(WCharCP unparsed)
		{
			if (unparsed == NULL || unparsed == L"" || unparsed == L" ")
			{
				mdlDialog_dmsgsPrint(L"CreateTestHelper: unparsed is NULL");
				return false;
			}

			std::wstring wstrInfo(unparsed);
			size_t iPos1 = wstrInfo.find(',');
			size_t iPos2 = wstrInfo.rfind(',');

			std::wstring strIndexPara = wstrInfo.substr(0, iPos1);
			std::wstring strXOffStep = wstrInfo.substr(iPos1 + 1, iPos2 - iPos1);
			std::wstring strParaValue = wstrInfo.substr(iPos2 + 1, wstrInfo.length() - iPos2);

			stIndexPara = std::stoi(strIndexPara);
			stParaValue = std::stod(strParaValue);
			stXOffStep = std::stod(strXOffStep);

			return true;
		}

		static void SetField()
		{
			int maxIndex = sizeof(TData) / sizeof(double);
			if (stIndexPara >= maxIndex)
			{
				mdlDialog_dmsgsPrint(L"CreateTestHelper: Fail to Set the Filed");
				return;
			}
			//Change Parameter;
			TData* pData = &stData;
			double* parrData = (double*)pData;
			int arrSize = sizeof(TData) / sizeof(double);
			parrData[stIndexPara] = stParaValue;
		}

		static void Mov2SpecPos(EditElementHandle& eeh)
		{
			double uorparameter = (*ACTIVEMODEL).GetModelInfoCP()->GetUorPerMeter();
			//Move Offset the model;
			stPosX4CreateTest += stXOffStep;
			if (SUCCESS != stEleOperHelper::MoveAbsPos(
				eeh, 0, 0, 0, stPosX4CreateTest* uorparameter, 0, 0))
			{
				mdlDialog_dmsgsPrint(L"CreateTestHelper: False to Move Element");
			}
		}

	public:
		static TData& CreateTestHelper(WCharCP unparsed, CallbackCreateElement CreateEle)
		{
			EditElementHandle eeh;
			return CreateTestHelper<TData>(unparsed, CreateEle, eeh);
		}

		static TData& CreateHelperInOrigin(EditElementHandleR eeh, CallbackCreateElement CreateEle)
		{
			if (CreateEle == NULL || false == CreateEle(eeh, &stData))
			{
				mdlDialog_dmsgsPrint(L"CreateTestHelper: False to Create Element");
			}
			return stData;
		}

	public:
		static TData& CreateTestHelper(WCharCP unparsed, CallbackCreateElement CreateEle,
			EditElementHandle& eeh)
		{
			try
			{
				if (false == ParserPara(unparsed))
					return stData;

				SetField();

				//Create the element.
				if (CreateEle == NULL || false == CreateEle(eeh, &stData))
				{
					mdlDialog_dmsgsPrint(L"CreateTestHelper: False to Create Element");
					return stData;
				}

				Mov2SpecPos(eeh);

				eeh.AddToModel();

				mdlDialog_dmsgsPrint(L"CreateTestHelper: Executed Completely");
				return stData;
			}
			catch (...)
			{
#ifdef LOG
				mdlDialog_dmsgsPrint(L"CreateTestHelper Error");
#endif // LOG
				return stData;
			}
		}
	};

	template<typename TData>
	TData CreatorHelper<TData>::stData;
	template<typename TData>
	double CreatorHelper<TData>::stPosX4CreateTest = 0;
	template<typename TData>
	int CreatorHelper<TData>::stIndexPara = 0;
	template<typename TData>
	double CreatorHelper<TData>::stParaValue = 0;
	template<typename TData>
	double CreatorHelper<TData>::stXOffStep = 0;
};

