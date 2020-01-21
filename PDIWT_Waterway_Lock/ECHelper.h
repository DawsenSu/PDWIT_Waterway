#pragma once
#ifndef ECHelper_h__
#define ECHelper_h__
#include"stdafx.h"
using namespace System::Reflection;

PDIWT_NAMESPACE_BEGIN
public class ECHelper
{
public:
	//! Import ESChema from organization level directory. If the schema already exists in Active DgnFile, no action will be taken.
	//! @param[in]	ecschemaFullName	FullName of Schema to import, with .xml extension
	//! @param[in]	pathByDefinedVariable	The defined system variable which define the path to search ecschema file.
	//!										This defined variable must be in organization level.
	//! @return SUCCESS if import ecSchema file without error
	static BentleyStatus ImportECSChemaInActiveDgnBasedOnDefinedVariable(WString ecschemaFullName, WString pathByDefinedVariable);
	//!	Write the information in dataObj into ECIstance.
	//! @param[in]	instance			The ECinstance Which to be written into.
	//! @param[in]	dataObj				The managed object containing the data which will be use to write into ecinstance
	//! @return	SUCCESS if write operation is successful.
	//! @remark		The dataObj should contain property/Properties attaching ECPropertyAttribute class. The ECPropertyClass indicate which Ecproperty accessstring in instance object should this 
	//!				class property look for. If the class property has UnitScaleAttribute, the property value in dataObjec will be multiply the ratio from meter to its assigned unit. If doesn't have
	//!				UnitScaleAttribute, the property value will remain unchanged.
	//!@see ECPropertyAttribute, UnitScaleAttribute.
	static BentleyStatus SetECPropertyValuesWithManagedObject(IECInstanceR instance, Object^ dataObj);
	//! Schedule Base ECClass on Element, which identify each solid as assigned mark
	//!	@param[in]	eeh					The editelementhandle Base ECClass attached on
	//! @param[in]	name				The name to identify the element
	//! @param[in]	code				The code to mark different element in same category
	//! @return	 SUCCESS if write operation is successful.
	static BentleyStatus SchedulePDIWTBaseECInstanceOnElement(EditElementHandleR eeh, WStringCR name, WStringCR code);


	//! Get the name and code from editElementHandle which is attached a PDIWTBase instance.
	//!	@param[in]	eeh					The editelementhandle is looked for, the eeh must be the one has been added to Model.
	//! @param[out] name				The element name in PDIWTBase
	//! @param[out] code				The element code in PDIWTBase
	//! @return		SUCCESS if get operation is successful.
	static BentleyStatus GetPDIWTBaseNameAndCode(EditElementHandleR eeh, WStringR name, WStringR code);
};
PDIWT_NAMESPACE_END

#endif // ECHelper_h__