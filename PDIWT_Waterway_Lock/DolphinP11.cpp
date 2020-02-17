#include "DolphinP11.h"

bool PDIWT::Waterway::Lock::DolphinP11Data::ValidateParameters()
{
	return true;
}

BentleyStatus PDIWT::Waterway::Lock::DolphinP11::InitializeComponents()
{
	if (!m_data.ValidateParameters())
		return ERROR;

	m_dolphin = DolphinColumnP1_P11(
		m_data.GetSaftyHeight(),
		m_data.GetDolphinColumnTopElevation(),
		m_data.GetDolphinColumnBottomElevation(),
		m_data.GetDolphinColumnTopLength(),
		m_data.GetDolphinColumnTopWidth(),
		m_data.GetDolphinColumnTopEdgeRadius(),
		m_data.GetDolphinColumnSideEdgeRadius(),
		m_data.GetDolphinColumnBottomLength(),
		m_data.GetDolphinColumnBottomWidth(),
		m_data.GetDolphinColumnHaunchHeight(),
		m_data.GetDolphinColumnHaunchLength(),
		m_data.GetAngleOfFirstPolylineWall(),
		m_data.GetHeightOfFirstPolylineWall(),
		m_data.GetAngleOfSecondPolylineWall(),
		m_data.GetHeightOfSecondPolylineWall(),
		m_data.GetCushionCapTopElevation()
	);

	m_cushioncap = Cushioncap(m_data.GetFrontToeLength() + m_data.GetDolphinColumnBottomWidth() + m_data.GetBackToeLength(),
		m_data.GetFrontToeLength() + m_data.GetDolphinColumnBottomWidth() + m_data.GetBackToeLength(),
		m_data.GetDolphinColumnBottomElevation() - m_data.GetCushionCapBottomElevation(),
		m_data.GetDolphinColumnBottomLength() + 2 * m_data.GetCushionCapExtraWideLength());

	m_cushion = Cushion(m_data.GetDolphinColumnBottomLength() + 2 * m_data.GetCushionCapExtraWideLength() + 2 * cushionExtraLength,
		m_data.GetFrontToeLength() + m_data.GetDolphinColumnBottomWidth() + m_data.GetBackToeLength() + 2 * cushionExtraLength,
		m_data.GetCapThickness());

	return SUCCESS;
}

BentleyStatus PDIWT::Waterway::Lock::DolphinP11::CreateDolphinP11(EditElementHandleR eeh, DgnModelRefR model)
{
	if (SUCCESS != InitializeComponents())
		return ERROR;

	ElementAgenda elems;

	double uorpermeter = model.GetModelInfoCP()->GetUorPerMeter();
	//DolphinColumn
	EditElementHandle dolphinEeh;
	if (SUCCESS != m_dolphin.CreateDolphin(dolphinEeh, model))
		return ERROR;
	elems.Insert(dolphinEeh);

	//cushioncap
	EditElementHandle cushioncapEeh;
	if (SUCCESS != m_cushioncap.CreateCushioncap(cushioncapEeh, model))
		return ERROR;

	DVec3d cushioncapDVec = DVec3d::From(0, (m_data.GetDolphinColumnHaunchLength() - m_data.GetFrontToeLength()), m_data.GetCushionCapBottomElevation() - m_data.GetDolphinColumnBottomElevation());
	cushioncapDVec.Scale(uorpermeter);
	Transform cushioncapTrans = Transform::From(cushioncapDVec);
	if (SUCCESS != cushioncapEeh.GetHandler().ApplyTransform(cushioncapEeh, TransformInfo(cushioncapTrans)))
		return ERROR;
	elems.Insert(cushioncapEeh);

	//cushion
	EditElementHandle cushionEeh;
	if (SUCCESS != m_cushion.CreateCushion(cushionEeh, model))
		return ERROR;
	DVec3d cushionRelateToCushionCapDVec = DVec3d::From(0, -cushionExtraLength, -m_data.GetCapThickness());
	cushionRelateToCushionCapDVec.Scale(uorpermeter);
	if (SUCCESS != cushionEeh.GetHandler().ApplyTransform(cushionEeh, TransformInfo(Transform::From(cushioncapDVec + cushionRelateToCushionCapDVec))))
		return ERROR;
	elems.Insert(cushionEeh);

	//cell Header
	if (SUCCESS != NormalCellHeaderHandler::CreateGroupCellElement(eeh, elems, L"dolphinP11"))
		return ERROR;
	return SUCCESS;
}