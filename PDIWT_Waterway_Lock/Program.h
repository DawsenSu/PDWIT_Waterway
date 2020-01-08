#pragma once
#include "stdafx.h"
#include "GuidewallP2Tool.h"

PDIWT_WATERWAY_LOCK_NAMESPACE_BEIGN
//////////////////////////////////////////////////////////////////////////
typedef BM::AddIn Base;

// Enter point class
[assembly:Bentley::MstnPlatformNET::AddInClassAttribute(ClassName = "PDIWT.Waterway.Lock.Program")];
[BM::AddInAttribute(MdlTaskID = "PDIWT_Waterway_Lock")]
public ref class Program sealed : public Base
{
private:
	Program(System::IntPtr mdlDesc) : Base(mdlDesc) { Addin = this; }
public:
	virtual int Run(array<System::String^>^ commandLine) override { return 0; }
	static Program^ Addin = nullptr;
};

//////////////////////////////////////////////////////////////////////////
//! command class
public ref class KeyinCommands
{
public:
	//! �޷�
	static	void	CreateDolphinP1(System::String^ unparsed);
	//! ��һ��
	static	void	CreateDolphinP11(System::String^ unparsed);
	//! �ն���
	static	void	CreateGuidewallP2(System::String^ unparsed);
	//! �¹���
	static	void	CreateGuidewallP3(System::String^ unparsed);
};

PDIWT_WATERWAY_LOCK_NAMESPACE_END