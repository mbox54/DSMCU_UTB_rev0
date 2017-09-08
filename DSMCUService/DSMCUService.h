// DSMCUService.h : main header file for the PROJECT 'DS MCU Service' application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

/////////////////////////////////////////////////////////////////////////////
// Resource DataBase
/////////////////////////////////////////////////////////////////////////////
#include "resource.h"		// main symbols



/////////////////////////////////////////////////////////////////////////////
// Constants /aliases
/////////////////////////////////////////////////////////////////////////////
// Service states
#define SERVICE_STATE_ENABLE				0x00
#define SERVICE_STATE_DISABLE				0x01
#define SERVICE_STATE_ENABLING				0x02
#define SERVICE_STATE_DISABLING				0x03

// Maxim DS4830a OP Return Codes
#define MAXQBL_SUCCESS						0x00
#define MAXQBL_NO_DEVICE					0x01
#define MAXQBL_DEV_INACCESSIBLE				0x02
#define MAXQBL_OP_FAILED					0x03
#define MAXQBL_OP_WRONG						0x04

#define SLAVEADDR_A0						0xA0
#define SLAVEADDR_A2						0xA2
#define SLAVEADDR_TABLE10					0x10

// User Modes
#define MD_PROGRAMMER						0x00
#define MD_ENGINEER							0x01
#define MD_OPERATOR							0x02

/////////////////////////////////////////////////////////////////////////////
// Project Data Types 
/////////////////////////////////////////////////////////////////////////////
struct st_serviceData
{
	unsigned char activeState;

};




// CDSMCUServiceApp:
// See DSMCUService.cpp for the implementation of this class
//
class CDSMCUServiceApp : public CWinApp
{
public:
	CDSMCUServiceApp();

// Overrides
public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern CDSMCUServiceApp theApp;