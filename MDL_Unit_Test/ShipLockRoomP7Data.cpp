#include "ShipLockRoomP7Data.h"
#include "EleOperHelper.h"
#include "EleCreatorHelper.h"

bool ShipLockRoomP7Data::Create(
	EditElementHandleR reeh, void* data)
{
	ShipLockRoomP7Data& d = *(ShipLockRoomP7Data*)data;
	double uorparameter = (*ACTIVEMODEL).GetModelInfoCP()->GetUorPerMeter();
	const DPoint3d dOriPoint = { 0,0,0 };
	Transform tran = Transform::FromFixedPointAndScaleFactors(
		dOriPoint, uorparameter, uorparameter, uorparameter);
	TransformInfo tri(tran);
	ElementAgenda elems;
	//Create Water Way Wall
	double shoulderHeight = d.ShoulderTopElevation -
		d.BottomElevation;
	//double SumHeight = d.TopElevation - d.BottomElevation;
	DPoint3d p0 = { 0, 0, d.BottomElevation };
	DPoint3d p1 = { d.WallBottomWidth, 0, d.BottomElevation };
	double shoulderOutExtend = shoulderHeight * d.DigRatio;
	DPoint3d p2 = { d.WallBottomWidth + shoulderOutExtend, 0,
		d.BottomElevation + shoulderHeight };
	DPoint3d p3 = { d.FrontToeWidth + d.SideWallBottomWidth, 0,
		d.BottomElevation + shoulderHeight };
	DPoint3d p4 = { d.FrontToeWidth + d.WallTopWidth, 0,
		d.TopElevation - d.NeckHeight };
	DPoint3d p5 = { d.FrontToeWidth + d.WallTopWidth, 0, 
		d.TopElevation};
	DPoint3d p6 = { d.FrontToeWidth, 0,
		d.TopElevation };
	DPoint3d p7 = { d.FrontToeWidth, 0, d.BottomElevation +
		d.BottomBoardThickness + d.FrontSideRAngleHeight };
	DPoint3d p8 = { d.FrontToeWidth - d.FrontSideRAngleWidth, 0,
	d.BottomElevation + d.BottomBoardThickness };
	DPoint3d p9 = { 0, 0, d.BottomElevation + d.BottomBoardThickness };
	
	//DPoint3d dirPoint = DPoint3d::From(0, d.StructLen, 0.0);
	//DVec3d dirExtrude = dirPoint - dOriPoint;

	bvector<DPoint3d> points = { p0,p1,p2,p3,p4,p5,p6,p7,p8,p9 };
	StatusInt rtl = 1;
	EditElementHandle eehWWW;
	rtl = EleCreatorHelper::CreateExtruction(eehWWW, d.StructLen, points.data(), (int)points.size());
	if (rtl != SUCCESS)
		return false;
	//Scale The model.
	
	rtl = eehWWW.GetHandler().ApplyTransform(eehWWW, tri);
	if (rtl != SUCCESS)
		return rtl;
	elems.Insert(eehWWW);
	//Create the Cap.
	p3 = p1;
	p1 = { 0, 0, d.BottomElevation - d.CussionThickness };
	p2 = { d.WallBottomWidth, 0, d.BottomElevation - d.CussionThickness };
	points.clear();
	points = { p0,p1,p2,p3 };
	EditElementHandle eehCussion;
	rtl = EleCreatorHelper::CreateExtruction(eehCussion, d.StructLen, points.data(), (int)points.size());
	if (rtl != SUCCESS)
		return false;
	//Scale The model.
	rtl = eehCussion.GetHandler().ApplyTransform(eehCussion, tri);
	if (rtl != SUCCESS)
		return rtl;

	elems.Insert(eehCussion);

	//Construct the cell.
	if (SUCCESS != NormalCellHeaderHandler::
		CreateGroupCellElement(reeh, elems, L"dolphinP11"))
		return false;
	return true;
}


void ShipLockRoomP7Data::SetECInstWithData(IECInstanceR ECInst,
	const ShipLockRoomP7Data&  data)
{
	ECPropSetter(ECInst, SafeSuperElvation, data);
	ECPropSetter(ECInst, TopElevation, data);
	ECPropSetter(ECInst, ShoulderTopElevation, data);
	ECPropSetter(ECInst, BottomElevation, data);
	ECPropSetter(ECInst, WallTopWidth, data);
	ECPropSetter(ECInst, SideWallBottomWidth, data);
	ECPropSetter(ECInst, WallBottomWidth, data);
	ECPropSetter(ECInst, FrontToeWidth, data);
	ECPropSetter(ECInst, FrontSideRAngleWidth, data);
	ECPropSetter(ECInst, FrontSideRAngleHeight, data);
	ECPropSetter(ECInst, NeckHeight, data);
	ECPropSetter(ECInst, DigRatio, data);
	ECPropSetter(ECInst, BottomBoardThickness, data);
	ECPropSetter(ECInst, CussionThickness, data);
	ECPropSetter(ECInst, StructLen, data);
}

void ShipLockRoomP7Data::ReadECInst2Data(const DgnECInstancePtr& ECInst,
	ShipLockRoomP7Data&  data)
{
	ECPropGetter(ECInst, SafeSuperElvation, data);
	ECPropGetter(ECInst, TopElevation, data);
	ECPropGetter(ECInst, ShoulderTopElevation, data);
	ECPropGetter(ECInst, BottomElevation, data);
	ECPropGetter(ECInst, WallTopWidth, data);
	ECPropGetter(ECInst, SideWallBottomWidth, data);
	ECPropGetter(ECInst, WallBottomWidth, data);
	ECPropGetter(ECInst, FrontToeWidth, data);
	ECPropGetter(ECInst, FrontSideRAngleWidth, data);
	ECPropGetter(ECInst, FrontSideRAngleHeight, data);
	ECPropGetter(ECInst, NeckHeight, data);
	ECPropGetter(ECInst, DigRatio, data);
	ECPropGetter(ECInst, BottomBoardThickness, data);
	ECPropGetter(ECInst, CussionThickness, data);
	ECPropGetter(ECInst, StructLen, data);
}

void ShipLockRoomP7Data::SetECOffPoint(const IECInstanceR& ECInst, EditElementHandleR eeh)
{
	DPoint3d dpOff = CaculateOff(eeh);
	ECInst.SetValue(L"OffsetInsertPoint", ECValue(dpOff));
}

DPoint3d ShipLockRoomP7Data::ReadECOffPoint(const DgnECInstancePtr& ECInst)
{
	ECValue ev;
	if (ECOBJECTS_STATUS_Success != 
		ECInst->GetValue(ev, L"OffsetInsertPoint"))
	{
		mdlDialog_dmsgsPrint(L"ShipLockRoomP7Data ReadOffPoint GetValue False.");
		return DPoint3d();
	}
	else
	{
		return ev.GetPoint3D();
	}
}

DPoint3d ShipLockRoomP7Data::CaculateOff(const EditElementHandleR eeh)
{
	DisplayHandler& dh = DisplayHandler::GetInstance();
	DPoint3d dpOff;
	dh.GetTransformOrigin(eeh, dpOff);
	double uorparameter = (*ACTIVEMODEL).GetModelInfoCP()->GetUorPerMeter();
	dpOff.x /= uorparameter;
	dpOff.y /= uorparameter;
	dpOff.z /= uorparameter;
	return dpOff;
}
