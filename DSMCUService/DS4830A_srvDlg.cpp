// DS4830A_srvDlg.cpp : implementation file
//

#include "stdafx.h"
#include "afxdialogex.h"

#include "DS4830A_srvDlg.h"



// CDS4830A_srvDlg dialog

IMPLEMENT_DYNAMIC(CDS4830A_srvDlg, CDialog)


CDS4830A_srvDlg::CDS4830A_srvDlg(CWnd * pParent)
	: CDialog(IDD_DS4830A, pParent)
	, m_bCheck_Autoscan(FALSE)
{
}

CDS4830A_srvDlg::CDS4830A_srvDlg(hSerialCDC* phSerialCDC, BYTE mode, DWORD CP2112_activeDeviceNum, st_CP2112_GPConf CP2112_GPConf, CWnd* pParent /*=NULL*/)
	: CDialog(IDD_DS4830A, pParent)
	, m_phSerialCDC(phSerialCDC)
	, m_Mode(mode)
	, mc_CP2112_activeDeviceNum(CP2112_activeDeviceNum)
	, mc_CP2112_GPConf(CP2112_GPConf)
	// cp2112 grid
	, m_GridSystem(m_pHidSmbus, &m_cPB_OP, &m_EDIT_STATUS, &m_service)
	// pages
	, m_pageDSBootLoader(m_pHidSmbus)
	, m_DS4830A_SFPP_A0(m_pHidSmbus, &m_cPB_OP, &m_EDIT_STATUS, &m_service)
	, m_DS4830A_SFPP_A2(m_pHidSmbus, &m_cPB_OP, &m_EDIT_STATUS, &m_service)
	, m_DS4830A_SFPP_T10(m_pHidSmbus, &m_cPB_OP, &m_EDIT_STATUS, &m_service)
	, m_DS4830A_SFPP_Custom(m_pHidSmbus, &m_cPB_OP, &m_EDIT_STATUS, &m_service)
	, m_DS4830A_SFPP_LR_CONF_OPER(m_pHidSmbus, &m_cPB_OP, &m_EDIT_STATUS, &m_service)
	, m_DS4830A_SFPP_LR_CONF_ENGI(m_pHidSmbus, &m_cPB_OP, &m_EDIT_STATUS, &m_service)
	, m_DS4830A_SFPP_MSA(m_pHidSmbus, &m_cPB_OP, &m_EDIT_STATUS, &m_service)
	, m_DS4830A_SFPP_DDM(m_pHidSmbus, &m_cPB_OP, &m_EDIT_STATUS, &m_service)
	, m_DS4830A_SFPP_TEC_APC(m_pHidSmbus, &m_cPB_OP, &m_EDIT_STATUS, &m_service)
	, m_TB_SFP_R2(m_pHidSmbus)


{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_hSmallIcon = (HICON)LoadImage(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_MAINFRAME), IMAGE_ICON, 16, 16, 0);

}


CDS4830A_srvDlg::~CDS4830A_srvDlg()
{
}

/////////////////////////////////////////////////////////////////////////////
// DS4830A_svrDlg - Protected Methods
/////////////////////////////////////////////////////////////////////////////

void CDS4830A_srvDlg::InitializeDialog()
{
	// place Tabs
	InitDlgTabs();

	// init service
	m_service.activeState = SERVICE_STATE_ENABLE;

	// start service
	StartTimer();

	// start DDM proceed Read
	StartTimerDDM();
}

// Create each of the dialog tabs and add
// tabs to the tab control
void CDS4830A_srvDlg::InitDlgTabs()
{
	// renew pointers
	m_DS4830A_SFPP_A0.p_cPB_OP = &m_cPB_OP;
	m_DS4830A_SFPP_A2.p_cPB_OP = &m_cPB_OP;
	m_DS4830A_SFPP_T10.p_cPB_OP = &m_cPB_OP;
	m_DS4830A_SFPP_LR_CONF_OPER.p_cPB_OP = &m_cPB_OP;

	switch (m_Mode)
	{
	case MD_PROGRAMMER:
		m_tabCtrl_DS4830A.InsertItem(0,  _T("Flasher"), IDD_PROPPAGE_BOOTLOAD, &m_pageDSBootLoader);
		m_tabCtrl_DS4830A.InsertItem(1,  _T("A0"), IDD_PROPPAGE_DS4830A_SFPP_A0, &m_DS4830A_SFPP_A0);
		m_tabCtrl_DS4830A.InsertItem(2,  _T("A2"), IDD_PROPPAGE_DS4830A_SFPP_A2, &m_DS4830A_SFPP_A2);
		m_tabCtrl_DS4830A.InsertItem(3,  _T("ONET1130_ENGI"), IDD_PROPPAGE_DS4830A_SFPP_ER_ENGI, &m_DS4830A_SFPP_T10);
		m_tabCtrl_DS4830A.InsertItem(4,  _T("GN1157"), IDD_PROPPAGE_DS4830A_SFPP_LR_CONF_OPER, &m_DS4830A_SFPP_LR_CONF_OPER);
		m_tabCtrl_DS4830A.InsertItem(5,  _T("GN1157_ENGI"), IDD_PROPPAGE_DS4830A_SFPP_LR_CONF_ENGI, &m_DS4830A_SFPP_LR_CONF_ENGI);
		m_tabCtrl_DS4830A.InsertItem(6,  _T("Custom"), IDD_PROPPAGE_DS4830A_SFPP_CUSTOM, &m_DS4830A_SFPP_Custom);
		m_tabCtrl_DS4830A.InsertItem(7,  _T("MSA"), IDD_PROPPAGE_DS4830A_SFPP_MSA, &m_DS4830A_SFPP_MSA);
		m_tabCtrl_DS4830A.InsertItem(8,  _T("DDM"), IDD_PROPPAGE_DS4830A_SFPP_DDM, &m_DS4830A_SFPP_DDM);
		m_tabCtrl_DS4830A.InsertItem(9,  _T("TEC APC"), IDD_PROPPAGE_DS4830A_SFPP_ER_TEC_APC, &m_DS4830A_SFPP_TEC_APC);
		m_tabCtrl_DS4830A.InsertItem(10, _T("TBSFP+ r2"), IDD_PROPPAGE_TB_SFP_R2, &m_TB_SFP_R2);

		break;

	case MD_ENGINEER:
		m_tabCtrl_DS4830A.InsertItem(0, _T("Flasher"), IDD_PROPPAGE_BOOTLOAD, &m_pageDSBootLoader);
		m_tabCtrl_DS4830A.InsertItem(1, _T("A0"), IDD_PROPPAGE_DS4830A_SFPP_A0, &m_DS4830A_SFPP_A0);
		m_tabCtrl_DS4830A.InsertItem(2, _T("A2"), IDD_PROPPAGE_DS4830A_SFPP_A2, &m_DS4830A_SFPP_A2);
		m_tabCtrl_DS4830A.InsertItem(3, _T("ONET1130_ENGI"), IDD_PROPPAGE_DS4830A_SFPP_ER_ENGI, &m_DS4830A_SFPP_T10);
		m_tabCtrl_DS4830A.InsertItem(4, _T("GN1157"), IDD_PROPPAGE_DS4830A_SFPP_LR_CONF_OPER, &m_DS4830A_SFPP_LR_CONF_OPER);
		m_tabCtrl_DS4830A.InsertItem(5, _T("GN1157_ENGI"), IDD_PROPPAGE_DS4830A_SFPP_LR_CONF_ENGI, &m_DS4830A_SFPP_LR_CONF_ENGI);
		m_tabCtrl_DS4830A.InsertItem(6, _T("Custom"), IDD_PROPPAGE_DS4830A_SFPP_CUSTOM, &m_DS4830A_SFPP_Custom);
		m_tabCtrl_DS4830A.InsertItem(7, _T("MSA"), IDD_PROPPAGE_DS4830A_SFPP_MSA, &m_DS4830A_SFPP_MSA);
		m_tabCtrl_DS4830A.InsertItem(8, _T("DDM"), IDD_PROPPAGE_DS4830A_SFPP_DDM, &m_DS4830A_SFPP_DDM);
		m_tabCtrl_DS4830A.InsertItem(9, _T("TEC APC"), IDD_PROPPAGE_DS4830A_SFPP_ER_TEC_APC, &m_DS4830A_SFPP_TEC_APC);
		m_tabCtrl_DS4830A.InsertItem(10, _T("TBSFP+ r2"), IDD_PROPPAGE_TB_SFP_R2, &m_TB_SFP_R2);

		break;

	case MD_OPERATOR:
		m_tabCtrl_DS4830A.InsertItem(0, _T("ER/ZR Program"), IDD_PROPPAGE_BOOTLOAD, &m_pageDSBootLoader);
		m_tabCtrl_DS4830A.InsertItem(1, _T("ONET1130ec"), IDD_PROPPAGE_DS4830A_SFPP_ER_ENGI, &m_DS4830A_SFPP_T10);
		m_tabCtrl_DS4830A.InsertItem(2, _T("GN1157"), IDD_PROPPAGE_DS4830A_SFPP_LR_CONF_OPER, &m_DS4830A_SFPP_LR_CONF_OPER);
		break;

	default:
		break;
	}


}

void CDS4830A_srvDlg::Trace(LPCTSTR szFmt, ...)
{
	CString str;

	// Format the message text
	va_list argptr;
	va_start(argptr, szFmt);
	str.FormatV(szFmt, argptr);
	va_end(argptr);

	str.Replace(_T("\n"), _T("\r\n"));

	CString strWndText;
	m_EDIT_STATUS.GetWindowText(strWndText);
	strWndText += str;
	m_EDIT_STATUS.SetWindowText(strWndText);

	//	m_TraceWnd.SetSel(str.GetLength()-1, str.GetLength()-2, FALSE);
	m_EDIT_STATUS.LineScroll(-m_EDIT_STATUS.GetLineCount());
	m_EDIT_STATUS.LineScroll(m_EDIT_STATUS.GetLineCount() - 4);
}


void CDS4830A_srvDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB_DS4830A, m_tabCtrl_DS4830A);
	DDX_Control(pDX, IDC_EDIT_INFO, m_EDIT_INFO);
	DDX_Control(pDX, IDC_EDIT_STATUS, m_EDIT_STATUS);
	DDX_Control(pDX, IDC_PB_OP, m_cPB_OP);
	DDX_Control(pDX, IDC_GRID, m_GridSystem);
	DDX_Check(pDX, IDC_CHECK_AUTOSCAN, m_bCheck_Autoscan);
	DDX_Control(pDX, IDC_STATIC_LOGO, m_Static_Logo);
}


BEGIN_MESSAGE_MAP(CDS4830A_srvDlg, CDialog)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_READ, &CDS4830A_srvDlg::OnBnClickedButtonRead)
	ON_BN_CLICKED(IDC_BUTTON_WRITE, &CDS4830A_srvDlg::OnBnClickedButtonWrite)
	ON_BN_CLICKED(IDCANCEL, &CDS4830A_srvDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDOK, &CDS4830A_srvDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON1, &CDS4830A_srvDlg::OnBnClickedButton1)
	ON_WM_PAINT()
	ON_NOTIFY(NM_CLICK, IDC_TAB_DS4830A, &CDS4830A_srvDlg::OnNMClickTabDs4830a)
	ON_BN_CLICKED(IDC_BUTTON_TESTBOARD_RESET, &CDS4830A_srvDlg::OnBnClickedButtonTestboardReset)
END_MESSAGE_MAP()


// CDS4830A_srvDlg message handlers

void CDS4830A_srvDlg::COMPortMsgQue_Init()
{
	// > Init Basic Monitoring Options

	m_frame1[10] = 10;

	// > Set Mon Default Tables
	BYTE ucCount = 0;

	// TABLE 1:1 / Board_Service
	m_COMPortMsgQue.v_OID_SHOW[ucCount].ucTableNum = 0x01;
	m_COMPortMsgQue.v_OID_SHOW[ucCount].ucCounterDivider = 1;
	m_COMPortMsgQue.v_OID_SHOW[ucCount].ucCounterValue = m_COMPortMsgQue.v_OID_SHOW[ucCount].ucCounterDivider - 1;

	ucCount++;
	m_COMPortMsgQue.ucMsgCount = ucCount;

	// TABLE 1:2 / Board_Ports

}


void CDS4830A_srvDlg::DDM_ConstructStateStr(st_AWFlags st_AWFlagsTemp, CString * str)
{
	CString strState;

	if (st_AWFlagsTemp.isAW == 0)
	{
		// [NO AW FLAGS]
		strState.Append(_T("оптимально"));
	}
	else
	{
		// [AW OCCURED]

		// check each AW
		if (st_AWFlagsTemp.bWL)
		{
			strState.Append(_T("[WL] "));
		}

		if (st_AWFlagsTemp.bWH)
		{
			strState.Append(_T("[WH] "));
		}

		if (st_AWFlagsTemp.bAL)
		{
			strState.Append(_T("[AL] "));
		}

		if (st_AWFlagsTemp.bAH)
		{
			strState.Append(_T("[AH] "));
		}
	}

	// set output parameter
	*str = strState;

}

// proceed DDM from Device tranfering by I2C
void CDS4830A_srvDlg::DDM_Proceed()
{
	/*
	typedef struct
	{
		unsigned char t1[5];
		unsigned char t2[5];
		unsigned char t3[5];
		unsigned char t4[5];

	} st_temp1;

	typedef union
	{
		unsigned char v_temp[20];

		st_temp1 st_temp;

	} un_temp1;

	unsigned char v_temp[20] = { 
		0x01, 0x02, 0x03, 0x04, 0x05, 
		0x06, 0x07, 0x08, 0x09, 0x0A, 
		0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 
		0x10, 0x11, 0x12, 0x13, 0x14
	};

	un_temp1 tt1;
	for (int k = 0; k < 20; k++)
	{
		tt1.v_temp[k] = v_temp[k];
	}
	
	unsigned char ttt1 = tt1.st_temp.t4[0];
	*/

	// > Read Device I2C SLAVE A2h

	// progress component
	m_cPB_OP.SetPos(0);

	unsigned char v_SlaveA2_LowerTable[128];

	// read from i2c
	// NOTE: for most compability it is read by 8 BYTE parts
	/*
	for (unsigned char k = 0; k < 16; k++)
	{
		// i2c read: Read FROM: Device[k*8 : k*8 + 8], TO: v_SlaveA2_LowerTable[k*8 : k*8 + 8]
		this->m_GridSystem.DeviceSlave_ReadUpdate(v_SlaveA2_LowerTable, SLAVEADDR_A2, k * 8, 8);

		m_cPB_OP.SetPos(k * 4);
	}
*/
	m_GridSystem.DeviceSlave_Read(v_SlaveA2_LowerTable, SLAVEADDR_A2, 0, 128);


	// > Proceed Values
	// copy Table to structure map
	un_MEMORY_MAP_A2_LOWER x_SlaveA2_LowerTable;

	for (unsigned char k = 0; k < 128; k++)
	{
		x_SlaveA2_LowerTable.v_MEMORY_MAP_A2_LOWER[k] = v_SlaveA2_LowerTable[k];

		// !debug
		//x_SlaveA2_LowerTable.v_MEMORY_MAP_A2_LOWER[k] = k;
	}

	// get Labels
	CWnd *pTEMPERATURE = this->GetDlgItem(IDC_STATIC_TEMPERATURE);
	CWnd *pTEMPR_DDM = this->GetDlgItem(IDC_STATIC_TEMPR_DDM);
	CWnd *pTEMPR_HEX = this->GetDlgItem(IDC_STATIC_TEMPR_HEX);
	CWnd *pTEMPR_STATE = this->GetDlgItem(IDC_STATIC_TEMPR_STATE);
	//	CWnd *pTEMPERATURE_WARN_MIN = this->GetDlgItem(IDC_STATIC_TEMPERATURE_WARN_MIN);
	//	CWnd *pTEMPERATURE_WARN_MAX = this->GetDlgItem(IDC_STATIC_TEMPERATURE_WARN_MAX);
	//	CWnd *pTEMPERATURE_ALERT_MIN = this->GetDlgItem(IDC_STATIC_TEMPERATURE_ALERT_MIN);
	//	CWnd *pTEMPERATURE_ALERT_MAX = this->GetDlgItem(IDC_STATIC_TEMPERATURE_ALERT_MAX);

	CWnd *pVCC = this->GetDlgItem(IDC_STATIC_VCC);
	CWnd *pVCC_DDM = this->GetDlgItem(IDC_STATIC_VCC_DDM);
	CWnd *pVCC_HEX = this->GetDlgItem(IDC_STATIC_VCC_HEX);
	CWnd *pVCC_STATE = this->GetDlgItem(IDC_STATIC_VCC_STATE);
	//	CWnd *pVCC_WARN_MIN = this->GetDlgItem(IDC_STATIC_VCC_WARN_MIN);
	//	CWnd *pVCC_WARN_MAX = this->GetDlgItem(IDC_STATIC_VCC_WARN_MAX);
	//	CWnd *pVCC_ALERT_MIN = this->GetDlgItem(IDC_STATIC_VCC_ALERT_MIN);
	//	CWnd *pVCC_ALERT_MAX = this->GetDlgItem(IDC_STATIC_VCC_ALERT_MAX);

	CWnd *pTX_BIAS = this->GetDlgItem(IDC_STATIC_TX_BIAS);
	CWnd *pTX_BIAS_DDM = this->GetDlgItem(IDC_STATIC_TX_BIAS_DDM);
	CWnd *pTX_BIAS_HEX = this->GetDlgItem(IDC_STATIC_TX_BIAS_HEX);
	CWnd *pTX_BIAS_STATE = this->GetDlgItem(IDC_STATIC_TX_BIAS_STATE);
	//	CWnd *pTX_BIAS_WARN_MIN = this->GetDlgItem(IDC_STATIC_TX_BIAS_WARN_MIN);
	//	CWnd *pTX_BIAS_WARN_MAX = this->GetDlgItem(IDC_STATIC_TX_BIAS_WARN_MAX);
	//	CWnd *pTX_BIAS_ALERT_MIN = this->GetDlgItem(IDC_STATIC_TX_BIAS_ALERT_MIN);
	//	CWnd *pTX_BIAS_ALERT_MAX = this->GetDlgItem(IDC_STATIC_TX_BIAS_ALERT_MAX);

	CWnd *pTX_POWER = this->GetDlgItem(IDC_STATIC_TX_POWER);
	CWnd *pTX_POWER_DDM = this->GetDlgItem(IDC_STATIC_TX_POWER_DDM);
	CWnd *pTX_POWER_HEX = this->GetDlgItem(IDC_STATIC_TX_POWER_HEX);
	CWnd *pTX_POWER_STATE = this->GetDlgItem(IDC_STATIC_TX_POWER_STATE);
	//	CWnd *pTX_POWER_WARN_MIN = this->GetDlgItem(IDC_STATIC_TX_POWER_WARN_MIN);
	//	CWnd *pTX_POWER_WARN_MAX = this->GetDlgItem(IDC_STATIC_TX_POWER_WARN_MAX);
	//	CWnd *pTX_POWER_ALERT_MIN = this->GetDlgItem(IDC_STATIC_TX_POWER_ALERT_MIN);
	//	CWnd *pTX_POWER_ALERT_MAX = this->GetDlgItem(IDC_STATIC_TX_POWER_ALERT_MAX);

	CWnd *pRX_POWER = this->GetDlgItem(IDC_STATIC_RX_POWER);
	CWnd *pRX_POWER_DDM = this->GetDlgItem(IDC_STATIC_RX_POWER_DDM);
	CWnd *pRX_POWER_HEX = this->GetDlgItem(IDC_STATIC_RX_POWER_HEX);
	CWnd *pRX_POWER_STATE = this->GetDlgItem(IDC_STATIC_RX_POWER_STATE);
	//	CWnd *pRX_POWER_WARN_MIN = this->GetDlgItem(IDC_STATIC_RX_POWER_WARN_MIN);
	//	CWnd *pRX_POWER_WARN_MAX = this->GetDlgItem(IDC_STATIC_RX_POWER_WARN_MAX);
	//	CWnd *pRX_POWER_ALERT_MIN = this->GetDlgItem(IDC_STATIC_RX_POWER_ALERT_MIN);
	//	CWnd *pRX_POWER_ALERT_MAX = this->GetDlgItem(IDC_STATIC_RX_POWER_ALERT_MAX);

	unsigned short uiValue;
	int iValue;
	float fValue;

	CString str;

	// > Real-Time Values
	// get [TEMPERATURE]
	// FORMAT:
	// 1Byte MSB = uChar, 1 Byte LSB = SFF_Fract
	// 1Byte MSB 1x = 1Grad_Celc
	// 1Byte LSB 1x = 1 / 256
	uiValue = x_SlaveA2_LowerTable.st_MEMORY_MAP_A2_LOWER.DDM_ACTIVE.TEMPERATURE_MSB;

	// output DDM-format
	// // convert to str
	str.Format(L"%d %d", x_SlaveA2_LowerTable.st_MEMORY_MAP_A2_LOWER.DDM_ACTIVE.TEMPERATURE_MSB, x_SlaveA2_LowerTable.st_MEMORY_MAP_A2_LOWER.DDM_ACTIVE.TEMPERATURE_LSB);

	// // output to control
	pTEMPR_DDM->SetWindowTextW(str);

	// output User-format
	fValue = uiValue;

	float fValueTemp;
	fValueTemp = x_SlaveA2_LowerTable.st_MEMORY_MAP_A2_LOWER.DDM_ACTIVE.TEMPERATURE_LSB;
	fValueTemp /= 256;

	fValue += fValueTemp;

	// // convert to str
	str.Format(L"%02.1f", fValue);

	// // output to control
	pTEMPERATURE->SetWindowTextW(str);

	// output HEX
	str.Format(L"%02X%02X", x_SlaveA2_LowerTable.st_MEMORY_MAP_A2_LOWER.DDM_ACTIVE.TEMPERATURE_MSB, x_SlaveA2_LowerTable.st_MEMORY_MAP_A2_LOWER.DDM_ACTIVE.TEMPERATURE_LSB);

	// // output to control
	pTEMPR_HEX->SetWindowTextW(str);

	// get [VCC]
	// FORMAT:
	// 2Byte Value

	uiValue = (x_SlaveA2_LowerTable.st_MEMORY_MAP_A2_LOWER.DDM_ACTIVE.VCC[0] * 256 + x_SlaveA2_LowerTable.st_MEMORY_MAP_A2_LOWER.DDM_ACTIVE.VCC[1]);
	
	// output DDM-format
	// 1x = 100uV
	// // convert to str
	str.Format(L"%d", uiValue);

	// // output to control
	pVCC_DDM->SetWindowTextW(str);

	// output User-format
	fValue = uiValue;
	fValue /= 10000;

	// // convert to str
	str.Format(L"%01.2f", fValue);

	// // output to control
	pVCC->SetWindowTextW(str);

	// output HEX
	str.Format(L"%02X%02X", x_SlaveA2_LowerTable.st_MEMORY_MAP_A2_LOWER.DDM_ACTIVE.VCC[0], x_SlaveA2_LowerTable.st_MEMORY_MAP_A2_LOWER.DDM_ACTIVE.VCC[1]);

	// // output to control
	pVCC_HEX->SetWindowTextW(str);


	// get TX_BIAS
	// FORMAT:
	// 2Byte Value
	uiValue = (x_SlaveA2_LowerTable.st_MEMORY_MAP_A2_LOWER.DDM_ACTIVE.TX_BIAS[0] * 256 + x_SlaveA2_LowerTable.st_MEMORY_MAP_A2_LOWER.DDM_ACTIVE.TX_BIAS[1]); // *2;
	
	// output DDM-format
	// 1x = 2uA
	// // convert to str
	str.Format(L"%d", uiValue);

	// // output to control
	pTX_BIAS_DDM->SetWindowTextW(str);

	// output User-format
	uiValue *= 2;
	fValue = uiValue;
	fValue /= 1000;

	// // convert to str
	str.Format(L"%3.1f", fValue);

	// // output to control
	pTX_BIAS->SetWindowTextW(str);
																																							
	// output HEX
	str.Format(L"%02X%02X", x_SlaveA2_LowerTable.st_MEMORY_MAP_A2_LOWER.DDM_ACTIVE.TX_BIAS[0], x_SlaveA2_LowerTable.st_MEMORY_MAP_A2_LOWER.DDM_ACTIVE.TX_BIAS[1]);

	// // output to control
	pTX_BIAS_HEX->SetWindowTextW(str);


	// get TX_POWER
	// FORMAT:
	// 2Byte Value
	uiValue = (x_SlaveA2_LowerTable.st_MEMORY_MAP_A2_LOWER.DDM_ACTIVE.TX_POWER[0] * 256 + x_SlaveA2_LowerTable.st_MEMORY_MAP_A2_LOWER.DDM_ACTIVE.TX_POWER[1]);

	// output DDM-format
	// 1x = 0.1uW
	// convert to str
	str.Format(L"%d", uiValue);

	// // output to control
	pTX_POWER_DDM->SetWindowTextW(str);

	// output User-format
	fValue = uiValue;
	fValue /= 10000;

	// // convert to dBm
	fValue = log10(fValue);
	fValue *= 10;

	// // convert to str
	str.Format(L"%02.1f", fValue);

	// // output to control
	pTX_POWER->SetWindowTextW(str);

	// output HEX
	str.Format(L"%02X%02X", x_SlaveA2_LowerTable.st_MEMORY_MAP_A2_LOWER.DDM_ACTIVE.TX_POWER[0], x_SlaveA2_LowerTable.st_MEMORY_MAP_A2_LOWER.DDM_ACTIVE.TX_POWER[1]);

	// // output to control
	pTX_POWER_HEX->SetWindowTextW(str);


	// get RX_POWER
	// FORMAT:
	// 2Byte Value
	uiValue = (x_SlaveA2_LowerTable.st_MEMORY_MAP_A2_LOWER.DDM_ACTIVE.RX_POWER[0] * 256 + x_SlaveA2_LowerTable.st_MEMORY_MAP_A2_LOWER.DDM_ACTIVE.RX_POWER[1]);

	// output DDM-format
	// 1x = 0.1uW
	// convert to str
	str.Format(L"%d", uiValue);

	// // output to control
	pRX_POWER_DDM->SetWindowTextW(str);

	// output User-format
	fValue = uiValue;
	fValue /= 10000;

	// // convert to dBm
	fValue = log10(fValue);
	fValue *= 10;

	// // convert to str
	str.Format(L"%02.1f", fValue);

	// // output to control
	pRX_POWER->SetWindowTextW(str);

	// output HEX
	str.Format(L"%02X%02X", x_SlaveA2_LowerTable.st_MEMORY_MAP_A2_LOWER.DDM_ACTIVE.RX_POWER[0], x_SlaveA2_LowerTable.st_MEMORY_MAP_A2_LOWER.DDM_ACTIVE.RX_POWER[1]);

	// // output to control
	pRX_POWER_HEX->SetWindowTextW(str);


	// > ALARM & WARNING FLAGS
	// NOTE:
	// FORMAT:
	/*
	WARNING_FLAGS_1_bit:
	7 = Temp_High_Warning
	6 = Temp_Low_Warning
	5 = Vcc_High_Warning
	4 = Vcc_Low_Warning
	3 = TX_Bias_High_Warning
	2 = TX_Bias_Low_Warning
	1 = TX_Power_High_Warning
	0 = TX_Power_Low_Warning
	
	WARNING_FLAGS_2_bit:
	7 = RX_Power_High_Warning
	6 = RX_Power_Low_Warning
	5-0 Reserved

	ALARM_FLAGS_1_bit:
	7 = Temp_High_Warning
	6 = Temp_Low_Warning
	5 = Vcc_High_Warning
	4 = Vcc_Low_Warning
	3 = TX_Bias_High_Warning
	2 = TX_Bias_Low_Warning
	1 = TX_Power_High_Warning
	0 = TX_Power_Low_Warning

	ALARM_FLAGS_2_bit:
	7 = RX_Power_High_Warning
	6 = RX_Power_Low_Warning
	5-0 Reserved

	*/ 

	// vars for Temporary Charasteristic Flag State
	st_AWFlags st_AWFlagsTemp;

	// get ALARM & WARNING FLAG Values
	BYTE icWarnings1 = x_SlaveA2_LowerTable.st_MEMORY_MAP_A2_LOWER.WARNING_FLAGS_1_bit;
	BYTE icWarnings2 = x_SlaveA2_LowerTable.st_MEMORY_MAP_A2_LOWER.WARNING_FLAGS_2_bit;

	BYTE icAlarms1 = x_SlaveA2_LowerTable.st_MEMORY_MAP_A2_LOWER.ALARM_FLAGS_1_bit;
	BYTE icAlarms2 = x_SlaveA2_LowerTable.st_MEMORY_MAP_A2_LOWER.ALARM_FLAGS_2_bit;

	// proceed [Tempr] Flags
	st_AWFlagsTemp.isAW = 0;

	// // WH
	if ((icWarnings1 & BITMASK_BIT7) != 0)
	{
		st_AWFlagsTemp.bWH = 1;
		st_AWFlagsTemp.isAW = 1;
	}
	else
	{
		st_AWFlagsTemp.bWH = 0;
	}

	// // WL
	if ((icWarnings1 & BITMASK_BIT6) != 0)
	{
		st_AWFlagsTemp.bWL = 1;
		st_AWFlagsTemp.isAW = 1;
	}
	else
	{
		st_AWFlagsTemp.bWL = 0;
	}

	// // AH
	if ((icAlarms1 & BITMASK_BIT7) != 0)
	{
		st_AWFlagsTemp.bAH = 1;
		st_AWFlagsTemp.isAW = 1;
	}
	else
	{
		st_AWFlagsTemp.bAH = 0;
	}

	// // AL
	if ((icAlarms1 & BITMASK_BIT6) != 0)
	{
		st_AWFlagsTemp.bAL = 1;
		st_AWFlagsTemp.isAW = 1;
	}
	else
	{
		st_AWFlagsTemp.bAL = 0;
	}

	// output State
	CString strState;

	// // proceed str
	this->DDM_ConstructStateStr(st_AWFlagsTemp, &strState);
	
	// // set control
	pTEMPR_STATE->SetWindowTextW(strState);


	// proceed [Voltage] Flags
	st_AWFlagsTemp.isAW = 0;

	// // WH
	if ((icWarnings1 & BITMASK_BIT5) != 0)
	{
		st_AWFlagsTemp.bWH = 1;
		st_AWFlagsTemp.isAW = 1;
	}
	else
	{
		st_AWFlagsTemp.bWH = 0;
	}

	// // WL
	if ((icWarnings1 & BITMASK_BIT4) != 0)
	{
		st_AWFlagsTemp.bWL = 1;
		st_AWFlagsTemp.isAW = 1;
	}
	else
	{
		st_AWFlagsTemp.bWL = 0;
	}

	// // AH
	if ((icAlarms1 & BITMASK_BIT5) != 0)
	{
		st_AWFlagsTemp.bAH = 1;
		st_AWFlagsTemp.isAW = 1;
	}
	else
	{
		st_AWFlagsTemp.bAH = 0;
	}

	// // AL
	if ((icAlarms1 & BITMASK_BIT4) != 0)
	{
		st_AWFlagsTemp.bAL = 1;
		st_AWFlagsTemp.isAW = 1;
	}
	else
	{
		st_AWFlagsTemp.bAL = 0;
	}

	// output State
	strState.Truncate(0);

	// // proceed str
	this->DDM_ConstructStateStr(st_AWFlagsTemp, &strState);

	// // set control
	pVCC_STATE->SetWindowTextW(strState);


	// proceed [Tx Power] Flags
	st_AWFlagsTemp.isAW = 0;

	// // WH
	if ((icWarnings1 & BITMASK_BIT1) != 0)
	{
		st_AWFlagsTemp.bWH = 1;
		st_AWFlagsTemp.isAW = 1;
	}
	else
	{
		st_AWFlagsTemp.bWH = 0;
	}

	// // WL
	if ((icWarnings1 & BITMASK_BIT0) != 0)
	{
		st_AWFlagsTemp.bWL = 1;
		st_AWFlagsTemp.isAW = 1;
	}
	else
	{
		st_AWFlagsTemp.bWL = 0;
	}

	// // AH
	if ((icAlarms1 & BITMASK_BIT1) != 0)
	{
		st_AWFlagsTemp.bAH = 1;
		st_AWFlagsTemp.isAW = 1;
	}
	else
	{
		st_AWFlagsTemp.bAH = 0;
	}

	// // AL
	if ((icAlarms1 & BITMASK_BIT0) != 0)
	{
		st_AWFlagsTemp.bAL = 1;
		st_AWFlagsTemp.isAW = 1;
	}
	else
	{
		st_AWFlagsTemp.bAL = 0;
	}

	// output State
	strState.Truncate(0);

	// // proceed str
	this->DDM_ConstructStateStr(st_AWFlagsTemp, &strState);

	// // set control
	pTX_BIAS_STATE->SetWindowTextW(strState);


	// proceed [Tx Power] Flags
	st_AWFlagsTemp.isAW = 0;

	// // WH
	if ((icWarnings1 & BITMASK_BIT3) != 0)
	{
		st_AWFlagsTemp.bWH = 1;
		st_AWFlagsTemp.isAW = 1;
	}
	else
	{
		st_AWFlagsTemp.bWH = 0;
	}

	// // WL
	if ((icWarnings1 & BITMASK_BIT2) != 0)
	{
		st_AWFlagsTemp.bWL = 1;
		st_AWFlagsTemp.isAW = 1;
	}
	else
	{
		st_AWFlagsTemp.bWL = 0;
	}

	// // AH
	if ((icAlarms1 & BITMASK_BIT3) != 0)
	{
		st_AWFlagsTemp.bAH = 1;
		st_AWFlagsTemp.isAW = 1;
	}
	else
	{
		st_AWFlagsTemp.bAH = 0;
	}

	// // AL
	if ((icAlarms1 & BITMASK_BIT2) != 0)
	{
		st_AWFlagsTemp.bAL = 1;
		st_AWFlagsTemp.isAW = 1;
	}
	else
	{
		st_AWFlagsTemp.bAL = 0;
	}

	// output State
	strState.Truncate(0);

	// // proceed str
	this->DDM_ConstructStateStr(st_AWFlagsTemp, &strState);

	// // set control
	pTX_POWER_STATE->SetWindowTextW(strState);


	// proceed [Rx Bias] Flags
	st_AWFlagsTemp.isAW = 0;

	// // WH
	if ((icWarnings2 & BITMASK_BIT7) != 0)
	{
		st_AWFlagsTemp.bWH = 1;
		st_AWFlagsTemp.isAW = 1;
	}
	else
	{
		st_AWFlagsTemp.bWH = 0;
	}

	// // WL
	if ((icWarnings2 & BITMASK_BIT6) != 0)
	{
		st_AWFlagsTemp.bWL = 1;
		st_AWFlagsTemp.isAW = 1;
	}
	else
	{
		st_AWFlagsTemp.bWL = 0;
	}

	// // AH
	if ((icAlarms2 & BITMASK_BIT7) != 0)
	{
		st_AWFlagsTemp.bAH = 1;
		st_AWFlagsTemp.isAW = 1;
	}
	else
	{
		st_AWFlagsTemp.bAH = 0;
	}

	// // AL
	if ((icAlarms2 & BITMASK_BIT6) != 0)
	{
		st_AWFlagsTemp.bAL = 1;
		st_AWFlagsTemp.isAW = 1;
	}
	else
	{
		st_AWFlagsTemp.bAL = 0;
	}

	// output State
	strState.Truncate(0);

	// // proceed str
	this->DDM_ConstructStateStr(st_AWFlagsTemp, &strState);

	// // set control
	pRX_POWER_STATE->SetWindowTextW(strState);



	/*
	if (st_AWFlagsTemp.isAW == 0)
	{
		// [NO AW FLAGS]
		strState.Append(_T("оптимально"));
	}
	else
	{
		// [AW OCCURED]

		// check each AW
		if (st_AWFlagsTemp.bWL)
		{
			strState.Append(_T("[WL] "));
		}

		if (st_AWFlagsTemp.bWH)
		{
			strState.Append(_T("[WH] "));
		}

		if (st_AWFlagsTemp.bAL)
		{
			strState.Append(_T("[AL] "));
		}

		if (st_AWFlagsTemp.bAH)
		{
			strState.Append(_T("[AH] "));
		}
	}
	*/


/*
	// > Warning Values
	// > > Low
	// get TEMPERATURE
	iValue = x_SlaveA2_LowerTable.st_MEMORY_MAP_A2_LOWER.AW_THRESHOLDS.Temp_Low_Warning;

	iValue *= 0xFF00;			// get MSB Byte
	iValue = iValue >> 8;

	// convert to str
	str.Format(L"%d", iValue);
	// output
	pTEMPERATURE_WARN_MIN->SetWindowTextW(str);

	// get VCC
	iValue = x_SlaveA2_LowerTable.st_MEMORY_MAP_A2_LOWER.AW_THRESHOLDS.Voltage_Low_Warning;
	// convert to str
	str.Format(L"%d", iValue);
	// output
	pVCC_WARN_MIN->SetWindowTextW(str);

	// get TX_BIAS
	iValue = x_SlaveA2_LowerTable.st_MEMORY_MAP_A2_LOWER.AW_THRESHOLDS.Bias_Low_Warning;
	// convert to str
	str.Format(L"%d", iValue);
	// output
	pTX_BIAS_WARN_MIN->SetWindowTextW(str);

	// get TX_POWER
	iValue = x_SlaveA2_LowerTable.st_MEMORY_MAP_A2_LOWER.AW_THRESHOLDS.TX_Power_Low_Warning;
	// convert to str
	str.Format(L"%d", iValue);
	// output
	pTX_POWER_WARN_MIN->SetWindowTextW(str);

	// get RX_POWER
	iValue = x_SlaveA2_LowerTable.st_MEMORY_MAP_A2_LOWER.AW_THRESHOLDS.RX_Power_Low_Warning;
	// convert to str
	str.Format(L"%d", iValue);
	// output
	pRX_POWER_WARN_MIN->SetWindowTextW(str);

	// > > High
	// get TEMPERATURE
	iValue = x_SlaveA2_LowerTable.st_MEMORY_MAP_A2_LOWER.AW_THRESHOLDS.Temp_High_Warning;
	
	iValue *= 0xFF00;			// get MSB Byte
	iValue = iValue >> 8;
	
	// convert to str
	str.Format(L"%d", iValue);
	// output
	pTEMPERATURE_WARN_MAX->SetWindowTextW(str);

	// get VCC
	iValue = x_SlaveA2_LowerTable.st_MEMORY_MAP_A2_LOWER.AW_THRESHOLDS.Voltage_High_Warning;
	// convert to str
	str.Format(L"%d", iValue);
	// output
	pVCC_WARN_MAX->SetWindowTextW(str);

	// get TX_BIAS
	iValue = x_SlaveA2_LowerTable.st_MEMORY_MAP_A2_LOWER.AW_THRESHOLDS.Bias_High_Warning;
	// convert to str
	str.Format(L"%d", iValue);
	// output
	pTX_BIAS_WARN_MAX->SetWindowTextW(str);

	// get TX_POWER
	iValue = x_SlaveA2_LowerTable.st_MEMORY_MAP_A2_LOWER.AW_THRESHOLDS.TX_Power_High_Warning;
	// convert to str
	str.Format(L"%d", iValue);
	// output
	pTX_POWER_WARN_MAX->SetWindowTextW(str);

	// get RX_POWER
	iValue = x_SlaveA2_LowerTable.st_MEMORY_MAP_A2_LOWER.AW_THRESHOLDS.RX_Power_High_Warning;
	// convert to str
	str.Format(L"%d", iValue);
	// output
	pRX_POWER_WARN_MAX->SetWindowTextW(str);


	// > Alert Values
	// > > Low
	// get TEMPERATURE
	iValue = x_SlaveA2_LowerTable.st_MEMORY_MAP_A2_LOWER.AW_THRESHOLDS.Temp_Low_Alarm;
	
	iValue *= 0xFF00;			// get MSB Byte
	iValue = iValue >> 8;
	
	// convert to str
	str.Format(L"%d", iValue);
	// output
	pTEMPERATURE_ALERT_MIN->SetWindowTextW(str);

	// get VCC
	iValue = x_SlaveA2_LowerTable.st_MEMORY_MAP_A2_LOWER.AW_THRESHOLDS.Voltage_Low_Alarm;
	// convert to str
	str.Format(L"%d", iValue);
	// output
	pVCC_ALERT_MIN->SetWindowTextW(str);

	// get TX_BIAS
	iValue = x_SlaveA2_LowerTable.st_MEMORY_MAP_A2_LOWER.AW_THRESHOLDS.Bias_Low_Alarm;
	// convert to str
	str.Format(L"%d", iValue);
	// output
	pTX_BIAS_ALERT_MIN->SetWindowTextW(str);

	// get TX_POWER
	iValue = x_SlaveA2_LowerTable.st_MEMORY_MAP_A2_LOWER.AW_THRESHOLDS.TX_Power_Low_Alarm;
	// convert to str
	str.Format(L"%d", iValue);
	// output
	pTX_POWER_ALERT_MIN->SetWindowTextW(str);

	// get RX_POWER
	iValue = x_SlaveA2_LowerTable.st_MEMORY_MAP_A2_LOWER.AW_THRESHOLDS.RX_Power_Low_Alarm;
	// convert to str
	str.Format(L"%d", iValue);
	// output
	pRX_POWER_ALERT_MIN->SetWindowTextW(str);

	// > > High
	// get TEMPERATURE
	iValue = x_SlaveA2_LowerTable.st_MEMORY_MAP_A2_LOWER.AW_THRESHOLDS.Temp_High_Alarm;
	
	iValue *= 0xFF00;			// get MSB Byte
	iValue = iValue >> 8;

	// convert to str
	str.Format(L"%d", iValue);
	// output
	pTEMPERATURE_ALERT_MAX->SetWindowTextW(str);

	// get VCC
	iValue = x_SlaveA2_LowerTable.st_MEMORY_MAP_A2_LOWER.AW_THRESHOLDS.Voltage_High_Alarm;
	// convert to str
	str.Format(L"%d", iValue);
	// output
	pVCC_ALERT_MAX->SetWindowTextW(str);

	// get TX_BIAS
	iValue = x_SlaveA2_LowerTable.st_MEMORY_MAP_A2_LOWER.AW_THRESHOLDS.Bias_High_Alarm;
	// convert to str
	str.Format(L"%d", iValue);
	// output
	pTX_BIAS_ALERT_MAX->SetWindowTextW(str);

	// get TX_POWER
	iValue = x_SlaveA2_LowerTable.st_MEMORY_MAP_A2_LOWER.AW_THRESHOLDS.TX_Power_High_Alarm;
	// convert to str
	str.Format(L"%d", iValue);
	// output
	pTX_POWER_ALERT_MAX->SetWindowTextW(str);

	// get RX_POWER
	iValue = x_SlaveA2_LowerTable.st_MEMORY_MAP_A2_LOWER.AW_THRESHOLDS.RX_Power_High_Alarm;
	// convert to str
	str.Format(L"%d", iValue);
	// output
	pRX_POWER_ALERT_MAX->SetWindowTextW(str);
*/

	m_cPB_OP.SetPos(100);
	UpdateData(FALSE);

	// Status output
	// Trace(_T("File %s  \n"), str);
	Trace(_T("[ Обновлены значения DDM ]  \n"));

	UpdateData(FALSE);
}

BOOL CDS4830A_srvDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here

	InitializeDialog();

	COMPortMsgQue_Init();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}


// -------------------------------------------------------------------
// Table procedures
// -------------------------------------------------------------------


// -------------------------------------------------------------------
// # Service routine
// -------------------------------------------------------------------

// -------------------------------------------------------------------
// Timer Procedures
// -------------------------------------------------------------------
void CDS4830A_srvDlg::StartTimer()
{
	m_nTimer = SetTimer(TIMER_ID_SYSTEM, 100, NULL);
}

void CDS4830A_srvDlg::StopTimer()
{
	KillTimer(m_nTimer);
}

void CDS4830A_srvDlg::StartTimerDDM()
{
	m_nTimerDDM = SetTimer(TIMER_ID_DDM, 3000, NULL);
}

void CDS4830A_srvDlg::EditTimerDDM()
{
	// TODO: Set new Timer period
}

void CDS4830A_srvDlg::StopTimerDDM()
{
	KillTimer(m_nTimerDDM);
}


void CDS4830A_srvDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	switch (nIDEvent)
	{
	case TIMER_ID_SYSTEM:	// Proceed MCU COM Port Communication Arbitrage

		// NOTE:
		// FORMAT:
		// Send 1 SHOW Request   --->
		// Get  1 SHOW Responce  <---
		
		for (UCHAR k = 0; k < m_COMPortMsgQue.ucMsgCount; k++)
		{
			if (m_COMPortMsgQue.v_OID_SHOW[k].ucCounterValue == 0)
			{
				// [PROCEED]

				// m_UTBDevice


				// restore Counter
				m_COMPortMsgQue.v_OID_SHOW[k].ucCounterValue = m_COMPortMsgQue.v_OID_SHOW[k].ucCounterDivider - 1;
			}
			else
			{
				// [WAIT]

				m_COMPortMsgQue.v_OID_SHOW[k].ucCounterValue--;
			}

		}
		
		



		break;

/*
		if (m_service.activeState == SERVICE_STATE_ENABLE)
		{
			// NOP
		}
		else
			if (m_service.activeState == SERVICE_STATE_DISABLE)
			{
				// NOP
			}
			else
				if (m_service.activeState == SERVICE_STATE_ENABLING)
				{
					// enable Controls
					CWnd *wndTab = this->GetDlgItem(IDC_TAB_DS4830A);
					wndTab->EnableWindow(TRUE);

					CWnd *wndBtn1 = this->GetDlgItem(IDC_BUTTON_READ);
					wndBtn1->EnableWindow(TRUE);
					CWnd *wndBtn2 = this->GetDlgItem(IDC_BUTTON_WRITE);
					wndBtn2->EnableWindow(TRUE);

					// change status state
					m_service.activeState = SERVICE_STATE_ENABLE;
				}
				else
					if (m_service.activeState == SERVICE_STATE_DISABLING)
					{
						// disable Controls
						CWnd *wndTab = this->GetDlgItem(IDC_TAB_DS4830A);
						wndTab->EnableWindow(FALSE);

						CWnd *wndBtn1 = this->GetDlgItem(IDC_BUTTON_READ);
						wndBtn1->EnableWindow(FALSE);
						CWnd *wndBtn2 = this->GetDlgItem(IDC_BUTTON_WRITE);
						wndBtn2->EnableWindow(FALSE);

						// change status state
						m_service.activeState = SERVICE_STATE_DISABLE;
					}
*/		

	case TIMER_ID_DDM:	// Proceed DDM output Status	
		
		// > check State of autoscanning
		// update vars
		UpdateData(TRUE);

		if (m_bCheck_Autoscan)	// if ACTIVATED
		{

			if (m_service.activeState == SERVICE_STATE_ENABLE)	// other Read/Write OP inactive
			{
				m_service.activeState = SERVICE_STATE_DISABLING;
			
				DDM_Proceed();

				m_service.activeState = SERVICE_STATE_ENABLING;
			}
		}	

		break;

	default:
		break;
	}


	CDialog::OnTimer(nIDEvent);
}

// main/global Read Button
void CDS4830A_srvDlg::OnBnClickedButtonRead()
{

	// define User Active Page Value
	BYTE uOrderPage;
	BYTE uUserPage;

	//NOTE: kak govoril Maksim, esli est pamiat, nujno ee ispolzovat.
	uOrderPage = m_tabCtrl_DS4830A.GetCurSel();

	// select from Tab Order
	switch (m_Mode)
	{
	case MD_PROGRAMMER:
		uUserPage = m_UserTabOrder.v_Tabs_Programmer[uOrderPage];
		break;

	case MD_ENGINEER:
		uUserPage = m_UserTabOrder.v_Tabs_Engineer[uOrderPage];
		break;

	case MD_OPERATOR:
		uUserPage = m_UserTabOrder.v_Tabs_Operator[uOrderPage];
		break;

	default:

		// error check
		break;

	}

	// select from User Tab Value
	switch (uUserPage)
	{
	case 0:	//	m_pageDSBootLoader;

		break;

	case 1:	//m_DS4830A_SFPP_A0
		m_DS4830A_SFPP_A0.OnBnClickedButton4();

		break;

	case 2:	//m_DS4830A_SFPP_A2
		m_DS4830A_SFPP_A2.OnBnClickedButton4();

		break;

	case 3:	//m_DS4830A_SFPP_T10
		m_DS4830A_SFPP_T10.OnBnClickedButton4();

		break;

	case 4:	//m_DS4830A_SFPP_Config
		m_service.activeState = SERVICE_STATE_DISABLING;

		m_DS4830A_SFPP_LR_CONF_OPER.OnBnClickedButton4();

		m_service.activeState = SERVICE_STATE_ENABLING;

		break;

	case 5:
		m_service.activeState = SERVICE_STATE_DISABLING;

		m_DS4830A_SFPP_LR_CONF_ENGI.OnBnClickedButton4();

		m_service.activeState = SERVICE_STATE_ENABLING;

		break;

	case 6:	//m_DS4830A_SFPP_Custom
		m_DS4830A_SFPP_Custom.OnBnClickedButton4();
		break;

	case 7:	//m_DS4830A_SFPP_MSA	
		m_service.activeState = SERVICE_STATE_DISABLING;

		m_DS4830A_SFPP_MSA.OnBnClickedButton4();

		m_service.activeState = SERVICE_STATE_ENABLING;

		break;

	case 8:	//m_DS4830A_SFPP_DDM	
		m_service.activeState = SERVICE_STATE_DISABLING;

		m_DS4830A_SFPP_DDM.OnBnClickedButton4();

		m_service.activeState = SERVICE_STATE_ENABLING;

		break;

	case 9:	//m_DS4830A_SFPP_TEC_APC	
		m_service.activeState = SERVICE_STATE_DISABLING;

		m_DS4830A_SFPP_TEC_APC.OnBnClickedButton4();

		m_service.activeState = SERVICE_STATE_ENABLING;

		break;

	default:

		// error check
		break;
	}
	
}

// main/global Write Button
void CDS4830A_srvDlg::OnBnClickedButtonWrite()
{
	// select from Tabs
	switch (m_tabCtrl_DS4830A.GetCurSel())
	{
	case 0:	//	m_pageDSBootLoader;

		break;

	case 1:	//m_DS4830A_SFPP_A0
		m_DS4830A_SFPP_A0.OnBnClickedButton5();

		break;

	case 2:	//m_DS4830A_SFPP_A2
		m_DS4830A_SFPP_A2.OnBnClickedButton5();

		break;

	case 3:	//m_DS4830A_SFPP_T10
		m_service.activeState = SERVICE_STATE_DISABLING;

		m_DS4830A_SFPP_T10.OnBnClickedButton5();

		m_service.activeState = SERVICE_STATE_ENABLING;

		break;

	case 4:	//m_DS4830A_SFPP_Config

		m_service.activeState = SERVICE_STATE_DISABLING;

		// Read OP
		m_DS4830A_SFPP_LR_CONF_OPER.OnBnClickedButton4();

		// Set OP
		m_DS4830A_SFPP_LR_CONF_OPER.OnBnClickedButtonBiasSet();
		m_DS4830A_SFPP_LR_CONF_OPER.OnBnClickedButtonModSet();
		m_DS4830A_SFPP_LR_CONF_OPER.OnBnClickedButtonHorSet();
		m_DS4830A_SFPP_LR_CONF_OPER.OnBnClickedButtonVerticalSet();

		// Write OP
		m_DS4830A_SFPP_LR_CONF_OPER.OnBnClickedButton5();

		m_service.activeState = SERVICE_STATE_ENABLING;
		break;

	case 5:
		m_service.activeState = SERVICE_STATE_DISABLING;

		// Read OP
		//m_DS4830A_SFPP_LR_CONF_ENGI.OnBnClickedButton4();

		// Set OP
		m_DS4830A_SFPP_LR_CONF_ENGI.OnBnClickedButtonBiasSet();
		m_DS4830A_SFPP_LR_CONF_ENGI.OnBnClickedButtonModSet();
		m_DS4830A_SFPP_LR_CONF_ENGI.OnBnClickedButtonHorSet();
		m_DS4830A_SFPP_LR_CONF_ENGI.OnBnClickedButtonVerticalSet();

		// Write OP
		m_DS4830A_SFPP_LR_CONF_ENGI.OnBnClickedButton5();

		m_service.activeState = SERVICE_STATE_ENABLING;

		break;

	case 6:	//m_DS4830A_SFPP_Custom
		m_DS4830A_SFPP_Custom.OnBnClickedButton5();
		break;

	case 7:	//m_DS4830A_SFPP_MSA	
		m_service.activeState = SERVICE_STATE_DISABLING;

		m_DS4830A_SFPP_MSA.OnBnClickedButton5();

		m_service.activeState = SERVICE_STATE_ENABLING;

		break;

	case 8:	//m_DS4830A_SFPP_DDM	
		m_service.activeState = SERVICE_STATE_DISABLING;

		m_DS4830A_SFPP_DDM.OnBnClickedButton5();

		m_service.activeState = SERVICE_STATE_ENABLING;

		break;

	case 9:	//m_DS4830A_SFPP_TEC_APC	
		m_service.activeState = SERVICE_STATE_DISABLING;

		m_DS4830A_SFPP_TEC_APC.OnBnClickedButton5();

		m_service.activeState = SERVICE_STATE_ENABLING;

		break;


	default:
		break;
	}
}


void CDS4830A_srvDlg::OnBnClickedCancel()
{
	int msgboxID = MessageBox(
		(LPCWSTR)L"Желаете выйти из программы?",
		(LPCWSTR)L"Выход из программы обслуживания \n",
		MB_ICONQUESTION | MB_YESNO
	);

	switch (msgboxID)
	{
	case IDYES:
		// Cancel
		CDialog::OnCancel();

		break;

	case IDNO:		
		// NOP

		break;
	}
	
}


void CDS4830A_srvDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	CDialog::OnOK();
}


void CDS4830A_srvDlg::OnBnClickedButton1()
{
	// TODO: Add your control notification handler code here
	DDM_Proceed();
}


void CDS4830A_srvDlg::OnPaint()
{
//	CPaintDC dc(this); // device context for painting
					   // TODO: Add your message handler code here
					   // Do not call CDialog::OnPaint() for painting messages
	// logo

	// Get img
	CImage imageLogo;
	imageLogo.Load(_T("logo_FT.png"));

	CBitmap bitmapLogo;
	bitmapLogo.Attach(imageLogo.Detach());

	// Load to Bitmap
	BITMAP imgLogo;
	bitmapLogo.GetObject(sizeof(BITMAP), &imgLogo);

	// Paint on DC
	CPaintDC dc(&m_Static_Logo);

	CDC memdcLogo;
	memdcLogo.CreateCompatibleDC(&dc);
	memdcLogo.SelectObject(&bitmapLogo);

	// Place stratch
	CRect rectLogo;
	m_Static_Logo.GetClientRect(&rectLogo);

	dc.StretchBlt(0, 0, rectLogo.Width(), rectLogo.Height(), &memdcLogo,
		0, 0, imgLogo.bmWidth, imgLogo.bmHeight, SRCCOPY);

	// return control to dialog
	CDialog::OnPaint();

}


void CDS4830A_srvDlg::OnNMClickTabDs4830a(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: Add your control notification handler code here
/*	
	int msgboxID = MessageBox(
		(LPCWSTR)L"Желаете выйти из программы?",
		(LPCWSTR)L"Выход из программы обслуживания \n",
		MB_ICONQUESTION | MB_YESNO
	);
*/

//	this->OnBnClickedButtonRead();
// define User Active Page Value
	BYTE uOrderPage;
	BYTE uUserPage;

	//NOTE: kak govoril Maksim, esli est pamiat, nujno ee ispolzovat.
	uOrderPage = m_tabCtrl_DS4830A.GetCurSel();

	// select from Tab Order
	switch (m_Mode)
	{
	case MD_PROGRAMMER:
		uUserPage = m_UserTabOrder.v_Tabs_Programmer[uOrderPage];
		break;

	case MD_ENGINEER:
		uUserPage = m_UserTabOrder.v_Tabs_Engineer[uOrderPage];
		break;

	case MD_OPERATOR:
		uUserPage = m_UserTabOrder.v_Tabs_Operator[uOrderPage];
		break;

	default:

		// error check
		break;

	}

	// select from User Tab Value
	switch (uUserPage)
	{


	case 9:	//m_DS4830A_SFPP_TEC_APC	

		m_DS4830A_SFPP_TEC_APC.StartTimer();

		m_TB_SFP_R2.StopTimer();

		break;

	case 10:	//m_DS4830A_SFPP_TEC_APC	

		m_TB_SFP_R2.StartTimer();

		m_DS4830A_SFPP_TEC_APC.StopTimer();

		break;



	default:

		// TEC Monitor Timer OP
		// TestBoard Monitor OP
		if ((m_Mode == MD_PROGRAMMER) || (m_Mode == MD_ENGINEER))
		{
			m_DS4830A_SFPP_TEC_APC.StopTimer();

			m_TB_SFP_R2.StopTimer();
		}

		break;
	}


	*pResult = 0;
}


void CDS4830A_srvDlg::OnBnClickedButtonTestboardReset()
{
	
	// reset CP2112
	BYTE retVal = HidSmbus_Reset(*m_pHidSmbus);
	Sleep(50);

	// open CP2112 Last connection
	retVal = HidSmbus_Open(m_pHidSmbus, mc_CP2112_activeDeviceNum, VID, PID);
	Sleep(50);

	// Set GPIO direction and mode bitmasks
	retVal = HidSmbus_SetGpioConfig(*m_pHidSmbus, mc_CP2112_GPConf.direction, mc_CP2112_GPConf.mode, mc_CP2112_GPConf.function, 0);
	Sleep(50);

	// reinit
	retVal = HidSmbus_WriteLatch(*m_pHidSmbus, 0x00, 0xFF);
	Sleep(50);

	if (retVal != HID_SMBUS_SUCCESS)
	{
		// error: Device Read
		Trace(_T("УСТРОЙСТВО НЕ ОТВЕЧАЕТ. [код: %02d] \n"), retVal);

	}
	else
	{
		Trace(_T("УСТРОЙСТВО ПЕРЕЗАГРУЖЕНО.\n"), retVal);
	}
}
