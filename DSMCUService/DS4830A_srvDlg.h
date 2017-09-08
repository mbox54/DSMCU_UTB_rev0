#pragma once

/////////////////////////////////////////////////////////////////////////////
// Includes
/////////////////////////////////////////////////////////////////////////////

#include "afxcmn.h"

#include "DSMCUServiceDlg.h"
#include "DlgTabCtrl.h"

// Tab Dialogs
#include "DSBootLoader.h"
#include "DS4830A_SFPP_A0.h"
#include "DS4830A_SFPP_A2.h"
#include "DS4830A_SFPP_ER_ENGI.h"
#include "DS4830A_SFF_Custom.h"
#include "DS4830A_SFPP_LR_CONF_OPER.h"
#include "DS4830A_SFPP_LR_CONF_ENGI.h"
#include "DS4830A_SFPP_MSA.h"
#include "DS4830A_SFPP_DDM.h"
#include "DS4830A_SFPP_ER_TEC_APC.h"
#include "TB_SFP_R2.h"

// System Grid
#include "SLABCP2112.h"
#include "GridSFF_CP2112.h"

#include "afxwin.h"


/////////////////////////////////////////////////////////////////////////////
// Constants /aliases
/////////////////////////////////////////////////////////////////////////////

// Timers
#define TIMER_ID_SYSTEM						0x01
#define TIMER_ID_DDM						0x02

#define BITMASK_BIT0						0x01
#define BITMASK_BIT1						0x02
#define BITMASK_BIT2						0x04
#define BITMASK_BIT3						0x08
#define BITMASK_BIT4						0x10
#define BITMASK_BIT5						0x20
#define BITMASK_BIT6						0x40
#define BITMASK_BIT7						0x80


// typedef for A2 DDM
typedef struct
{
	unsigned short Temp_High_Alarm;				// 00-01  2  Temp High Alarm   MSB at low address
	unsigned short Temp_Low_Alarm;				// 02-03  2  Temp Low Alarm MSB at low address
	unsigned short Temp_High_Warning;			// 04-05  2  Temp High Warning MSB at low address
	unsigned short Temp_Low_Warning;			// 06-07  2  Temp Low Warning MSB at low address
	unsigned short Voltage_High_Alarm;			// 08-09  2  Voltage High Alarm MSB at low address
	unsigned short Voltage_Low_Alarm;			// 10-11  2  Voltage Low Alarm MSB at low address
	unsigned short Voltage_High_Warning;		// 12-13  2  Voltage High Warning MSB at low address
	unsigned short Voltage_Low_Warning;			// 14-15  2  Voltage Low Warning MSB at low address
	unsigned short Bias_High_Alarm;				// 16-17  2  Bias High Alarm MSB at low address
	unsigned short Bias_Low_Alarm;				// 18-19  2  Bias Low Alarm MSB at low address
	unsigned short Bias_High_Warning;			// 20-21  2  Bias High Warning MSB at low address
	unsigned short Bias_Low_Warning;			// 22-23  2  Bias Low Warning MSB at low address
	unsigned short TX_Power_High_Alarm;			// 24-25  2  TX Power High Alarm MSB at low address
	unsigned short TX_Power_Low_Alarm;			// 26-27  2  TX Power Low Alarm MSB at low address
	unsigned short TX_Power_High_Warning;		// 28-29  2  TX Power High Warning MSB at low address
	unsigned short TX_Power_Low_Warning;		// 30-31  2  TX Power Low Warning MSB at low address
	unsigned short RX_Power_High_Alarm;			// 32-33  2  RX Power High Alarm MSB at low address
	unsigned short RX_Power_Low_Alarm;			// 34-35  2  RX Power Low Alarm MSB at low address
	unsigned short RX_Power_High_Warning;		// 36-37  2  RX Power High Warning MSB at low address
	unsigned short RX_Power_Low_Warning;		// 38-39  2  RX Power Low Warning MSB at low address	
} st_AW_THRESHOLDS;								// 0-39		0x00-0x27	40	A/W Thresholds		Diagnostic Flag Alarm and Warning Thresholds (see Table 3.15)

typedef struct
{
	unsigned char TEMPERATURE_MSB;				// 96
	unsigned char TEMPERATURE_LSB;				// 97
	unsigned char VCC[2];						// 98-99
	unsigned char TX_BIAS[2];					// 100-101
	unsigned char TX_POWER[2];					// 102-103
	unsigned char RX_POWER[2];					// 104-105
} st_DDM_ACTIVE;

typedef struct
{
	// DIAGNOSTIC AND CONTROL/STATUS FIELDS
	/*
	struct
	{
	unsigned int Temp_High_Alarm;			// 00-01  2  Temp High Alarm MSB at low address
	unsigned int Temp_Low_Alarm;			// 02-03  2  Temp Low Alarm MSB at low address
	unsigned int Temp_High_Warning;			// 04-05  2  Temp High Warning MSB at low address
	unsigned int Temp_Low_Warning;			// 06-07  2  Temp Low Warning MSB at low address
	unsigned int Voltage_High_Alarm;		// 08-09  2  Voltage High Alarm MSB at low address
	unsigned int Voltage_Low_Alarm;			// 10-11  2  Voltage Low Alarm MSB at low address
	unsigned int Voltage_High_Warning;		// 12-13  2  Voltage High Warning MSB at low address
	unsigned int Voltage_Low_Warning;		// 14-15  2  Voltage Low Warning MSB at low address
	unsigned int Bias_High_Alarm;			// 16-17  2  Bias High Alarm MSB at low address
	unsigned int Bias_Low_Alarm;			// 18-19  2  Bias Low Alarm MSB at low address
	unsigned int Bias_High_Warning;			// 20-21  2  Bias High Warning MSB at low address
	unsigned int Bias_Low_Warning;			// 22-23  2  Bias Low Warning MSB at low address
	unsigned int TX_Power_High_Alarm;		// 24-25  2  TX Power High Alarm MSB at low address
	unsigned int TX_Power_Low_Alarm;		// 26-27  2  TX Power Low Alarm MSB at low address
	unsigned int TX_Power_High_Warning;		// 28-29  2  TX Power High Warning MSB at low address
	unsigned int TX_Power_Low_Warning;		// 30-31  2  TX Power Low Warning MSB at low address
	unsigned int RX_Power_High_Alarm;		// 32-33  2  RX Power High Alarm MSB at low address
	unsigned int RX_Power_Low_Alarm;		// 34-35  2  RX Power Low Alarm MSB at low address
	unsigned int RX_Power_High_Warning;		// 36-37  2  RX Power High Warning MSB at low address
	unsigned int RX_Power_Low_Warning;		// 38-39  2  RX Power Low Warning MSB at low address
	} AW_THRESHOLDS;					// 00-39	0x00-0x27	40	A/W Thresholds		Diagnostic Flag Alarm and Warning Thresholds (see Table 3.15)
	*/

	st_AW_THRESHOLDS AW_THRESHOLDS;			// 0-39		0x00-0x27	40	A/W Thresholds		Diagnostic Flag Alarm and Warning Thresholds (see Table 3.15)

	unsigned char RESERVED3[16];			// 40-55	0x28-0x37	16	Unallocated
	unsigned char EXT_CAL_CONST[36];		// 56-91	0x38-0x5B	36	Ext Cal Constants	Diagnostic calibration constants for optional External Calibration (see Table 3.16)
	unsigned char RESERVED4[3];				// 92-94	0x5C-0x5E	3	Unallocated
	unsigned char CC_DMI;					// 95		0x5F		1	CC_DMI				Check code for Base Diagnostic Fields (addresses 0 to 94)

											// unsigned char DIAGNOSTICS[10];		// 96-105	0x60-0x69	10	Diagnostics			Diagnostic Monitor Data (internally or externally calibrated) (see Table 3.17)
											// Converted analog values. Calibrated 16 bit data. 
											/*
											96   All  Temperature MSB  Internally measured module temperature.
											97   All  Temperature LSB
											98   All  Vcc MSB  Internally measured supply voltage in transceiver.
											99   All  Vcc LSB
											100  All  TX Bias MSB  Internally measured TX Bias C urrent.
											101  All  TX Bias LSB
											102  All  TX Power MSB  Measured TX output power.
											103  All  TX Power LSB
											104  All  RX Power MSB  Measured RX input power.
											105  All  RX Power LSB
											106-109  All  Unallocated  Reserved for future diagnostic definitions
											*/

	st_DDM_ACTIVE DDM_ACTIVE;

	/*
	struct
	{
	unsigned char TEMPERATURE_MSB;			// 96		0x60
	unsigned char TEMPERATURE_LSB;			// 97		0x61
	unsigned int  VCC;						// 98-99	0x62-0x63
	unsigned int  TX_BIAS;					// 100-101
	unsigned int  TX_POWER;					// 102-103
	unsigned int  RX_POWER;					// 104-105
	} DDM_ACTIVE;
	*/

	unsigned char RESERVED5[4];				// 106-109	0x6A-0x6D	4	Unallocated

											/*
											struct
											{
											unsigned char Data_Ready_Bar_State		: 1;    // / 0 /
											unsigned char Rx_LOS_State				: 1;    // / 1 /
											unsigned char TX_Fault_State			: 1;    // / 2 /
											unsigned char Soft_Rate_Select			: 1;    //  3 [aka. “RS(0)”] /
											unsigned char Rate_Select_State			: 1;    //  4 [aka. “RS(0)”] /
											unsigned char RS_1_State				: 1;    //  5 /
											unsigned char Soft_TX_Disable_Select	: 1;    //  6 /
											unsigned char TX_Disable_State			: 1;    //  7 /
											} STATUS_CONTROL_bit;				// 110		0x6E		1	Status/Control		Optional Status and Control Bits (see Table 3.17)
											*/

	// st_STATUS_CONTROL_bit STATUS_CONTROL_bit;
	unsigned char STATUS_CONTROL_bit;

	unsigned char RESERVED6;				// 111		0x6F		1	Reserved			Reserved for SFF-8079

											/*
											struct
											{
											unsigned char TX_Power_Low_Alarm		: 1;     / 0 Set when TX output power is below low alarm level.  /
											unsigned char TX_Power_High_Alarm		: 1;     / 1 Set when TX output power exceeds high alarm level. 	 /
											unsigned char TX_Bias_Low_Alarm			: 1;     / 2 Set when TX Bias current is below low alarm level.  /
											unsigned char TX_Bias_High_Alarm		: 1;     / 3 Set when TX Bias current exceeds high alarm level. /
											unsigned char Vcc_Low_Alarm				: 1;     / 4 Set when internal supply voltage is below low alarm level. /
											unsigned char Vcc_High_Alarm			: 1;     / 5 Set when internal supply voltage exceeds high alarm level. /
											unsigned char Temp_Low_Alarm			: 1;     / 6 Set when internal temperature is below low alarm level. /
											unsigned char Temp_High_Alarm			: 1;     / 7 Set when internal temperature exceeds high alarm level. /
											} ALARM_FLAGS_1_bit;				// 112		0x70	Alarm Flags		Diagnostic Alarm Flag Status Bits (see Table 3.18)
											*/

	// st_ALARM_FLAGS_1_bit ALARM_FLAGS_1_bit;
	unsigned char ALARM_FLAGS_1_bit;

	/*
	struct
	{
	unsigned char 							: 6;    / 0-5 Reserved Alarm /
	unsigned char RX_Power_Low_Alarm		: 1;    / 6 Set when Received Power is below low alarm level. /
	unsigned char RX_Power_High_Alarm		: 1;    / 7 Set when Received Power exceeds high alarm level. /
	} ALARM_FLAGS_2_bit;				// 113		0x71	Alarm Flags		Diagnostic Alarm Flag Status Bits (see Table 3.18)
	*/

	// st_ALARM_FLAGS_2_bit ALARM_FLAGS_2_bit;
	unsigned char ALARM_FLAGS_2_bit;

	unsigned char RESERVED7[2];				// 114-115	0x72-0x73	2	Unallocated

											/*
											struct
											{
											unsigned char TX_Power_Low_Warning		: 1;    / 0 Set when TX output power is below l ow warning level. /
											unsigned char TX_Power_High_Warning		: 1;    / 1 Set when TX output power exceeds high warning level.  /
											unsigned char TX_Bias_Low_Warning		: 1;    / 2 Set when TX Bias current is below low warning level.  /
											unsigned char TX_Bias_High_Warning		: 1;    / 3 Set when TX Bias cu rrent exceeds high warning level. /
											unsigned char Vcc_Low_Warning			: 1;    / 4 Set when internal supply voltage is below low warning level. /
											unsigned char Vcc_High_Warning			: 1;    / 5 Set when internal supply voltage exceeds high warning level.  /
											unsigned char Temp_Low_Warning			: 1;    / 6 Set when intern al temperature is below low warning level. 	 /
											unsigned char Temp_High_Warning			: 1;    / 7 Set when internal temperature exceeds high warning level. /
											} WARNING_FLAGS_1_bit;				// 116		0x74	Warning Flags		Diagnostic Warning Flag Status Bits (see Table 3.18)
											*/

	// st_WARNING_FLAGS_1_bit WARNING_FLAGS_1_bit;
	unsigned char WARNING_FLAGS_1_bit;

	/*
	struct
	{
	unsigned char 							: 6;    / 0-5 Reserved Alarm /
	unsigned char RX_Power_Low_Warning		: 1;    / 6 Set when Received Power is below low Warning level. /
	unsigned char RX_Power_High_Warning		: 1;    / 7 Set when Received Power exceeds high Warning level. /
	} WARNING_FLAGS_2_bit;				// 117		0x75	Warning Flags		Diagnostic Warning Flag Status Bits (see Table 3.18)
	*/

	// st_WARNING_FLAGS_2_bit WARNING_FLAGS_2_bit;
	unsigned char WARNING_FLAGS_2_bit;

	/*
	struct
	{
	unsigned char Power_Level_Select 		: 1;    / 0 /
	unsigned char Power_Level_Op_State		: 1;    / 1 /
	unsigned char 							: 1;    / 2 /
	unsigned char Soft_RS_1_Select			: 1;    / 3 /
	unsigned char Reserved					: 4;    / 4-7 /
	} EXT_STATUS_CONTROL_bit;			// 118		0x76	Ext Status/Control	Extended module control and status bytes (see Table 3.18a)
	*/

	// st_EXT_STATUS_CONTROL_bit EXT_STATUS_CONTROL_bit;
	unsigned char EXT_STATUS_CONTROL_bit;

	unsigned char RESERVED8;				// 119		0x77		1	

											// GENERAL USE FIELDS
	unsigned char VENDOR_SPECIFIC[3];		// 120-122	0x78-0x7A	3	Vendor Specific		Vendor specific memory addresses (see Table 3.19)

											//Password
	unsigned char PASSWORD[4];				// 123-126	0x7B-0x7E	4

											//Page
	unsigned char PAGE;						// 127		0x7F		1

											//unsigned char USER_EEPROM[120];		// 128-247	0x80-0xF7	120	User EEPROM			User writable non-volatile memory (see Table 3.20)
											//unsigned char VENDOR_CONTROL[8];		// 248-255	0xF8-0xFF	8	Vendor Control		Vendor specific control addresses (see Table 3.21)	
} MEMORY_MAP_A2_LOWER;

// Registers _named_bits
typedef union
{
	unsigned char v_MEMORY_MAP_A2_LOWER[128];

	MEMORY_MAP_A2_LOWER st_MEMORY_MAP_A2_LOWER;

} un_MEMORY_MAP_A2_LOWER;


typedef struct st_AWFlags
{
	BYTE isAW;
	BYTE bWL, bWH, bAL, bAH;
};


// ##########################################################################
// CDS4830A_srvDlg dialog
// ##########################################################################
class CDS4830A_srvDlg : public CDialog
{
	DECLARE_DYNAMIC(CDS4830A_srvDlg)

public:
	CDS4830A_srvDlg(CWnd* pParent = NULL);
	CDS4830A_srvDlg(hSerialCDC* phSerialCDC, BYTE mode, DWORD CP2112_activeDeviceNum , st_CP2112_GPConf CP2112_GPConf, CWnd* pParent = NULL);

	virtual ~CDS4830A_srvDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DS4830A };
#endif

	// Protected Members
protected:

	// define users Tab List
	struct st_UserTabOrder
	{
		unsigned char v_Tabs_Programmer[15] =	{ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
		unsigned char v_Tabs_Engineer[15] =		{ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
		unsigned char v_Tabs_Operator[15] =		{ 0, 3, 4, 0, 0, 0, 0, 0, 0, 0, 0 };
	} m_UserTabOrder;
	
	// define COMPortMsg Arbiter
	struct st_OID_SHOW_PARAMS
	{
		BYTE ucTableNum;
		BYTE ucCounterDivider;
		BYTE ucCounterValue;
	};

	struct st_COMPortMsgQue
	{
		BYTE ucMsgCount;
		st_OID_SHOW_PARAMS v_OID_SHOW[32];

	} m_COMPortMsgQue;


	HID_SMBUS_DEVICE* m_pHidSmbus;
	hSerialCDC* m_phSerialCDC;

	channelFrame m_frame1;

	// Controls
	// Tab Dialogs
	CDSBootLoader					m_pageDSBootLoader;
	CDS4830A_SFPP_A0				m_DS4830A_SFPP_A0;
	CDS4830A_SFPP_A2				m_DS4830A_SFPP_A2;
	CDS4830A_SFPP_ER_ENGI			m_DS4830A_SFPP_T10;
	CDS4830A_SFF_Custom				m_DS4830A_SFPP_Custom;
	CDS4830A_SFPP_CONF				m_DS4830A_SFPP_LR_CONF_OPER;
	CDS4830A_SFPP_LR_CONF_ENGI		m_DS4830A_SFPP_LR_CONF_ENGI;
	CDS4830A_SFPP_MSA				m_DS4830A_SFPP_MSA;
	CDS4830A_SFPP_DDM				m_DS4830A_SFPP_DDM;
	CDS4830A_SFPP_ER_TEC_APC		m_DS4830A_SFPP_TEC_APC;
	CTB_SFP_R2						m_TB_SFP_R2;



	// Service data operations/options
	st_serviceData m_service;				// R/W status
	BYTE m_Mode;							// Dialog Mode
	DWORD mc_CP2112_activeDeviceNum;		// copy params
	st_CP2112_GPConf mc_CP2112_GPConf;		// copy params

// Implementation
	// Variables
	HICON					m_hIcon;
	HICON					m_hSmallIcon;

	// > Timer	
	UINT_PTR m_nTimer;
	UINT_PTR m_nTimerDDM;

	// Procedures
	void InitializeDialog();
	void InitDlgTabs();

	void Trace(LPCTSTR szFmt, ...);

	// Timer
	void StartTimer();
	void StopTimer();

	void StartTimerDDM();
	void EditTimerDDM();
	void StopTimerDDM();

	

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	// tabs control
	CDlgTabCtrl m_tabCtrl_DS4830A;

	CEdit m_EDIT_INFO;

	// main window status input
	CEdit m_EDIT_STATUS;

	// main progress
	CProgressCtrl m_cPB_OP;

	// system Grid
	CGridSFF_CP2112 m_GridSystem;

	BOOL m_bCheck_Autoscan;
	CStatic m_Static_Logo;

	// > Procedures
	// init
	void COMPortMsgQue_Init();

	// service
	void DDM_ConstructStateStr(st_AWFlags st_AWFlagsTemp, CString * str);

	// table ops
	void DDM_Proceed();

//	unsigned char DeviceSlave_Read(BYTE * v_ByteData, BYTE slaveAddr, BYTE startAddr, WORD count);

	virtual BOOL OnInitDialog();

	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedButtonRead();
	afx_msg void OnBnClickedButtonWrite();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedOk();
	
	afx_msg void OnBnClickedButton1();
	//  auto scanning state

	afx_msg void OnPaint();
	afx_msg void OnNMClickTabDs4830a(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButtonTestboardReset();
};



// typedef struct
// {
// 	unsigned char Data_Ready_Bar_State : 1;			/* 0 */
// 	unsigned char Rx_LOS_State : 1;					/* 1 */
// 	unsigned char TX_Fault_State : 1;				/* 2 */
// 	unsigned char Soft_Rate_Select : 1;				/* 3 [aka. “RS(0)”] */
// 	unsigned char Rate_Select_State : 1;			/* 4 [aka. “RS(0)”] */
// 	unsigned char RS_1_State : 1;					/* 5 */
// 	unsigned char Soft_TX_Disable_Select : 1;		/* 6 */
// 	unsigned char TX_Disable_State : 1;				/* 7 */
// } st_STATUS_CONTROL_bit;					// 110		0x6E		1	Status/Control		Optional Status and Control Bits (see Table 3.17)

// typedef struct
// {
// 	unsigned char TX_Power_Low_Alarm : 1;			/* 0 Set when TX output power is below low alarm level.  */
// 	unsigned char TX_Power_High_Alarm : 1;			/* 1 Set when TX output power exceeds high alarm level. 	 */
// 	unsigned char TX_Bias_Low_Alarm : 1;			/* 2 Set when TX Bias current is below low alarm level.  */
// 	unsigned char TX_Bias_High_Alarm : 1;			/* 3 Set when TX Bias current exceeds high alarm level. */
// 	unsigned char Vcc_Low_Alarm : 1;				/* 4 Set when internal supply voltage is below low alarm level. */
// 	unsigned char Vcc_High_Alarm : 1;				/* 5 Set when internal supply voltage exceeds high alarm level. */
// 	unsigned char Temp_Low_Alarm : 1;				/* 6 Set when internal temperature is below low alarm level. */
// 	unsigned char Temp_High_Alarm : 1;				/* 7 Set when internal temperature exceeds high alarm level. */
// } st_ALARM_FLAGS_1_bit;						// 112		0x70	Alarm Flags		Diagnostic Alarm Flag Status Bits (see Table 3.18)	

// typedef struct
// {
// 	unsigned char : 6;								/* 0-5 Reserved Alarm */
// 	unsigned char RX_Power_Low_Alarm : 1;			/* 6 Set when Received Power is below low alarm level. */
// 	unsigned char RX_Power_High_Alarm : 1;			/* 7 Set when Received Power exceeds high alarm level. */
// } st_ALARM_FLAGS_2_bit;						// 113		0x71	Alarm Flags		Diagnostic Alarm Flag Status Bits (see Table 3.18)		

// typedef struct
// {
// 	unsigned char TX_Power_Low_Warning : 1;			/* 0 Set when TX output power is below l ow warning level. */
// 	unsigned char TX_Power_High_Warning : 1;		/* 1 Set when TX output power exceeds high warning level.  */
// 	unsigned char TX_Bias_Low_Warning : 1;			/* 2 Set when TX Bias current is below low warning level.  */
// 	unsigned char TX_Bias_High_Warning : 1;			/* 3 Set when TX Bias cu rrent exceeds high warning level. */
// 	unsigned char Vcc_Low_Warning : 1;				/* 4 Set when internal supply voltage is below low warning level. */
// 	unsigned char Vcc_High_Warning : 1;				/* 5 Set when internal supply voltage exceeds high warning level.  */
// 	unsigned char Temp_Low_Warning : 1;				/* 6 Set when intern al temperature is below low warning level. 	 */
// 	unsigned char Temp_High_Warning : 1;			/* 7 Set when internal temperature exceeds high warning level. */
// } st_WARNING_FLAGS_1_bit;					// 116		0x74	Warning Flags		Diagnostic Warning Flag Status Bits (see Table 3.18)

// typedef struct
// {
// 	unsigned char : 6;								/* 0-5 Reserved Alarm */
// 	unsigned char RX_Power_Low_Warning : 1;			/* 6 Set when Received Power is below low Warning level. */
// 	unsigned char RX_Power_High_Warning : 1;		/* 7 Set when Received Power exceeds high Warning level. */
// } st_WARNING_FLAGS_2_bit;					// 117		0x75	Warning Flags		Diagnostic Warning Flag Status Bits (see Table 3.18)	

// typedef struct
// {
// 	unsigned char Power_Level_Select : 1;			/* 0 */
// 	unsigned char Power_Level_Op_State : 1;			/* 1 */
// 	unsigned char : 1;								/* 2 */
// 	unsigned char Soft_RS_1_Select : 1;				/* 3 */
// 	unsigned char Reserved : 4;						/* 4-7 */
// } st_EXT_STATUS_CONTROL_bit;				// 118		0x76	Ext Status/Control	Extended module control and status bytes (see Table 3.18a)


											// > [#]Slave A2 LOWER Memory Map
											// Diagnostics: Data Fields – Address A2h