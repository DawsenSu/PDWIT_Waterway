#include "DolphinP11.h"

PDIWT_WATERWAY_LOCK_NAMESPACE_BEGIN

const double DolphinP11::cushionExtraLength = 0.1;
const WString DolphinP11::componentName = L"DolphinP11";

PDIWT::Waterway::Lock::DolphinP11::DolphinP11(UI::Model::DolphinP11Data^ data)
{
	m_data = data;
}

BentleyStatus PDIWT::Waterway::Lock::DolphinP11::InitializeComponents()
{
	if (!m_data->ValidateParameters())
		return ERROR;

	m_dolphin = DolphinColumnP1_P11(
		m_data->DolphinColumnTopElevation,
		m_data->DolphinColumnBottomElevation,
		m_data->DolphinColumnTopLength,
		m_data->DolphinColumnTopWidth,
		m_data->DolphinColumnTopEdgeRadius,
		m_data->DolphinColumnSideEdgeRadius,
		m_data->DolphinColumnBottomLength,
		m_data->DolphinColumnBottomWidth,
		m_data->DolphinColumnHaunchHeight,
		m_data->DolphinColumnHaunchLength,
		m_data->AngleOfFirstPolylineWall,
		m_data->HeightOfFirstPolylineWall,
		m_data->DolphinColumnBottomElevation
	);

	m_cushioncap = Cushioncap(m_data->FrontToeLength + m_data->DolphinColumnBottomWidth + m_data->BackToeLength,
		m_data->FrontToeLength + m_data->DolphinColumnBottomWidth + m_data->BackToeLength,
		m_data->DolphinColumnBottomElevation - m_data->CushionCapBottomElevation,
		m_data->DolphinColumnBottomLength + 2 * m_data->CushionCapExtraWideLength);

	m_cushion = Cushion(m_data->DolphinColumnBottomLength + 2 * m_data->CushionCapExtraWideLength + 2 * cushionExtraLength,
		m_data->FrontToeLength + m_data->DolphinColumnBottomWidth + m_data->BackToeLength + 2 * cushionExtraLength,
		m_data->CapThickness);

	return SUCCESS;
}

BentleyStatus PDIWT::Waterway::Lock::DolphinP11::CreateDolphinP11(EditElementHandleR eeh, DgnModelRefR model, bool isForDynamic)
{
	if (SUCCESS != InitializeComponents())
		return ERROR;

	ElementAgenda elems;

	double uorpermeter = model.GetModelInfoCP()->GetUorPerMeter();
	const bvector<WString> _subElementNames{ L"DolphinColumnP1_P11",L"Cushioncap",L"Cushion"};

	//DolphinColumn
	EditElementHandle dolphinEeh;
	if (SUCCESS != m_dolphin.CreateDolphin(dolphinEeh, model))
		return ERROR;
	if (SUCCESS != ECHelper::SchedulePDIWTBaseECInstanceOnElement(dolphinEeh, L"DolphinColumnP1_P11", L"")) return ERROR;
	elems.Insert(dolphinEeh);

	//cushioncap
	EditElementHandle cushioncapEeh;
	if (SUCCESS != m_cushioncap.CreateCushioncap(cushioncapEeh, model))
		return ERROR;

	DVec3d cushioncapDVec = DVec3d::From(0, (m_data->DolphinColumnHaunchLength - m_data->FrontToeLength), (m_data->CushionCapBottomElevation - m_data->DolphinColumnBottomElevation));
	cushioncapDVec.Scale(uorpermeter);
	Transform cushioncapTrans = Transform::From(cushioncapDVec);
	if (SUCCESS != cushioncapEeh.GetHandler().ApplyTransform(cushioncapEeh, TransformInfo(cushioncapTrans)))
		return ERROR;
	if (SUCCESS != ECHelper::SchedulePDIWTBaseECInstanceOnElement(cushioncapEeh, L"Cushioncap", L"")) return ERROR;
	elems.Insert(cushioncapEeh);

	//cushion
	EditElementHandle cushionEeh;
	if (SUCCESS != m_cushion.CreateCushion(cushionEeh, model))
		return ERROR;
	DVec3d cushionRelateToCushionCapDVec = DVec3d::From(0, -cushionExtraLength, -m_data->CapThickness);
	cushionRelateToCushionCapDVec.Scale(uorpermeter);
	if (SUCCESS != cushionEeh.GetHandler().ApplyTransform(cushionEeh, TransformInfo(Transform::From(cushioncapDVec + cushionRelateToCushionCapDVec))))
		return ERROR;
	if (SUCCESS != ECHelper::SchedulePDIWTBaseECInstanceOnElement(cushionEeh, L"Cushion", L"")) return ERROR;
	elems.Insert(cushionEeh);

	//cell Header
	if (SUCCESS != NormalCellHeaderHandler::CreateGroupCellElement(eeh, elems, L"dolphinP11"))
		return ERROR;

	// if it's not for dynamic purpose, attach its ECClass;
	if (!isForDynamic)
	{
		DgnFileR _dgnFile = *model.GetDgnFileP();

		DgnECManagerR _ecMgr = DgnECManager::GetManager();
		const WString _ecSchemaFullName(L"PDWT_Waterway.01.00.ecschema.xml");
		const WString _ecClassName(L"DolphinP11");
		const WString _definedECSchemaFilePathVariable(L"PDIWT_ORGANIZATION_ECSCHEMAPATH");

		SchemaKey _schemakey;
		if (SUCCESS != SchemaKey::ParseSchemaFullName(_schemakey, _ecSchemaFullName.GetWCharCP())) return ERROR;
		SchemaInfo _schemaInfo(_schemakey, _dgnFile);
		//Use the ECHelper Method
		if (SUCCESS != ECHelper::ImportECSChemaInActiveDgnBasedOnDefinedVariable(_ecSchemaFullName, _definedECSchemaFilePathVariable)) return ERROR;

		DgnECInstanceEnablerPtr _ecMainInstanceEnabler = _ecMgr.ObtainInstanceEnablerByName(_schemaInfo.GetSchemaName(), _ecClassName.GetWCharCP(), _dgnFile);
		StandaloneECInstanceR _wipInstance = _ecMainInstanceEnabler->GetSharedWipInstance();

		//! Base part assignment
		_wipInstance.SetValue(L"Name", ECValue(_ecClassName.GetWCharCP()));
		//! Parameters part assignment
		if (SUCCESS != ECHelper::SetECPropertyValuesWithManagedObject(_wipInstance, m_data)) return ERROR;

		if (!_ecMainInstanceEnabler->SupportsCreateInstanceOnElement())
			return ERROR;
		if (DgnECInstanceStatus::DGNECINSTANCESTATUS_Success != _ecMainInstanceEnabler->ScheduleInstanceOnElement(nullptr, _wipInstance, eeh))
			return ERROR;
	}
	return SUCCESS;
}

PDIWT_WATERWAY_LOCK_NAMESPACE_END