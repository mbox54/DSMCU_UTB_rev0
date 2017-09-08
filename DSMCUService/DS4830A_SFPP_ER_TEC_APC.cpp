// DS4830A_SFPP_ER_TEC_APC.cpp : implementation file
//

/////////////////////////////////////////////////////////////////////////////
// Includes
/////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "DSMCUService.h"
#include "DS4830A_SFPP_ER_TEC_APC.h"
#include "afxdialogex.h"


// ##########################################################################
// CDS4830A_SFPP_ER_TEC_APC dialog
// ##########################################################################
IMPLEMENT_DYNAMIC(CDS4830A_SFPP_ER_TEC_APC, CDialogEx)

CDS4830A_SFPP_ER_TEC_APC::CDS4830A_SFPP_ER_TEC_APC(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_PROPPAGE_DS4830A_SFPP_ER_TEC_APC, pParent)
	, m_bCheck_TEC_act(FALSE)
	, m_bCheck_TEC_heat(FALSE)
	, m_Edit_Tempr_Opt(_T(""))
	, m_bCheck_Power(FALSE)
	, m_bCheck_Photo(FALSE)
	, m_GateC_P(_T(""))
	, m_GateC_N(_T(""))
	, m_GateH_P(_T(""))
	, m_GateH_N(_T(""))
	, m_Edit_Power_Opt(_T(""))
	, m_Edit_Power_Gap(_T(""))
{

}


CDS4830A_SFPP_ER_TEC_APC::CDS4830A_SFPP_ER_TEC_APC(HID_SMBUS_DEVICE * pHidSmbus, CProgressCtrl * p_cPB_OP, CEdit * p_EDIT_STATUS, st_serviceData * p_service, CWnd * pParent)
	: CDialogEx(IDD_PROPPAGE_DS4830A_SFPP_ER_TEC_APC, pParent)
, m_pHidSmbus(pHidSmbus)
, p_EDIT_STATUS(p_EDIT_STATUS)
, p_cPB_OP(p_cPB_OP)
, p_service(p_service)
, m_Grid(m_pHidSmbus, p_cPB_OP, p_EDIT_STATUS, p_service)
{

}


CDS4830A_SFPP_ER_TEC_APC::~CDS4830A_SFPP_ER_TEC_APC()
{

}

// -------------------------------------------------------------------
// MESSAGE_MAP
// -------------------------------------------------------------------
BEGIN_MESSAGE_MAP(CDS4830A_SFPP_ER_TEC_APC, CDialogEx)
	// Grid event functions
	ON_NOTIFY(NM_CLICK, IDC_GRID, OnGridClick)
	ON_NOTIFY(GVN_ENDLABELEDIT, IDC_GRID, OnGridEndEdit)

	ON_BN_CLICKED(IDC_BUTTON_CONF_WRITE, &CDS4830A_SFPP_ER_TEC_APC::OnBnClickedButtonConfWrite)
	ON_BN_CLICKED(IDC_BUTTON4, &CDS4830A_SFPP_ER_TEC_APC::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON5, &CDS4830A_SFPP_ER_TEC_APC::OnBnClickedButton5)
	ON_WM_HSCROLL()
	ON_BN_CLICKED(IDC_CHECK_TEC_ACT, &CDS4830A_SFPP_ER_TEC_APC::OnBnClickedCheckTecAct)
	ON_WM_ACTIVATE()
	ON_BN_CLICKED(IDC_BUTTON1, &CDS4830A_SFPP_ER_TEC_APC::OnBnClickedButton1)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON2, &CDS4830A_SFPP_ER_TEC_APC::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_CHECK_POWER_ACTIVE, &CDS4830A_SFPP_ER_TEC_APC::OnBnClickedCheckPowerActive)
	ON_BN_CLICKED(IDC_CHECK_PHOTO_ACTIVE, &CDS4830A_SFPP_ER_TEC_APC::OnBnClickedCheckPhotoActive)
	ON_EN_CHANGE(IDC_EDIT_TEMPR_OPT, &CDS4830A_SFPP_ER_TEC_APC::OnEnChangeEditTemprOpt)
	ON_BN_CLICKED(IDC_CHECK_TEC_HEAT, &CDS4830A_SFPP_ER_TEC_APC::OnBnClickedCheckTecHeat)
END_MESSAGE_MAP()


void CDS4830A_SFPP_ER_TEC_APC::Trace(LPCTSTR szFmt, ...)
{
	CString str;

	// Format the message text
	va_list argptr;
	va_start(argptr, szFmt);
	str.FormatV(szFmt, argptr);
	va_end(argptr);

	str.Replace(_T("\n"), _T("\r\n"));

	CString strWndText;
	p_EDIT_STATUS->GetWindowText(strWndText);
	strWndText += str;
	p_EDIT_STATUS->SetWindowText(strWndText);

	//	m_TraceWnd.SetSel(str.GetLength()-1, str.GetLength()-2, FALSE);
	p_EDIT_STATUS->LineScroll(-p_EDIT_STATUS->GetLineCount());
	p_EDIT_STATUS->LineScroll(p_EDIT_STATUS->GetLineCount() - 4);
}

void CDS4830A_SFPP_ER_TEC_APC::CtrlsInit()
{
	// var for label OP
	CString str;

	// BIAS
	// slider
	m_Slider_Tempr.SetRange(VALUE_TEMPR_MIN, VALUE_TEMPR_MAX);

	m_Slider_Tempr.SetPos(-1);	// need for init updatedata this control bug
//	UpdateData(TRUE);

	m_Slider_Tempr.SetPos(VALUE_TEMPR_DEF);

	UpdateData(FALSE);
}


// -------------------------------------------------------------------
// Device Operations
// -------------------------------------------------------------------
void CDS4830A_SFPP_ER_TEC_APC::ReadDevice()
{
	// set password
	// Valid ConfigTable Values
	unsigned char v_TablName[1] = { 0x10 };
	unsigned char v_TablPass[4] = { 0x00, 0x11, 0x22, 0x33 };

	unsigned char v_WrByte[1];

	// progress component
	p_cPB_OP->SetPos(0);

	// send Table
	// write tabl in Device
	m_Grid.DeviceSlave_Write(v_TablName, SLAVEADDR_A2, 0x7F, 1);
	Sleep(60);

	p_cPB_OP->SetPos(30);

	// send password
	// write pass in Device
	/*	// !debug // DESICION: 1Byte mode
	
	for (unsigned char k = 0; k < 4; k++)
	{
		v_WrByte[0] = v_TablPass[k];
		m_Grid.DeviceSlave_Write(v_WrByte, SLAVEADDR_A2, 0x7B + k, 1);

		Sleep(10);

		p_cPB_OP->SetPos(30 + 5 * k);
	}
*/

	// send 4Byte immediately
	m_Grid.DeviceSlave_Write(v_TablPass, SLAVEADDR_A2, 0x7B, 4);

	p_cPB_OP->SetPos(50);

	Sleep(50);

	// > Read op
	unsigned char v_TableOutput[256];
	unsigned char v_TEC_APC[32];

	p_cPB_OP->SetPos(80);

	// fill Table other fields with None 
	for (unsigned char k = 0; k < 0xC0; k++)
	{
		v_TableOutput[k] = 0xFF;
	}

	m_Grid.DeviceSlave_Read(v_TEC_APC, SLAVEADDR_A2, 0xC0, 32);

	// fill Table data fields with Values 
	for (unsigned char k = 0; k < 32; k++)
	{
		v_TableOutput[k + 0xC0] = v_TEC_APC[k];
	}

	// fill Table other fields with None 
	unsigned char k_end = 0xC0 + 32;
	for (unsigned int k = k_end; k < 256; k++)
	{
		v_TableOutput[k] = 0xFF;
	}

	// > Output to Grid control
	m_Grid.GridSFF_Write(v_TableOutput, 0, 256);

	p_cPB_OP->SetPos(100);

	// > Get ONET Bias Value
	unsigned char v_Bias[2];
	m_Grid.DeviceSlave_Read(v_Bias, SLAVEADDR_A2, 0x8F, 2);

	// format Value
	WORD uiBiasVal;

	uiBiasVal = v_Bias[0];
	uiBiasVal = uiBiasVal << 2;
	uiBiasVal += (v_Bias[1] & 0x03);


	// > Config other output Controls
	Set_TEC_APC_Ctrls(v_TEC_APC, uiBiasVal);

}


void CDS4830A_SFPP_ER_TEC_APC::WriteDevice()
{
	// write op
	//m_Grid.DeviceSlave_WriteTimer(uValues, 1, SLAVEADDR_A2, 0, 256, 0x7F, 0x10);

	// Valid ConfigTable Values
	unsigned char v_TablName[1] = { 0x10 };
	unsigned char v_TablPass[4] = { 0x00, 0x11, 0x22, 0x33 };


	// progress component
	p_cPB_OP->SetPos(0);

	// send Table
	// write tabl in Device
	m_Grid.DeviceSlave_Write(v_TablName, SLAVEADDR_A2, 0x7F, 1);
	Sleep(30);

	p_cPB_OP->SetPos(20);

	// send password
	// write pass in Device
	m_Grid.DeviceSlave_Write(v_TablPass, SLAVEADDR_A2, 0x7B, 4);
	Sleep(30);

	p_cPB_OP->SetPos(40);

	// write op
	//m_Grid.GridSFF_Read(uValues2, 128, 128);

	// NOTE:
	// it is must to:
	// write ALL 128 bytes of TABLE
	unsigned char v_TEC_APC[32];
	unsigned char v_WrByte8[8];

	// get Values
	m_Grid.GridSFF_Read(v_TEC_APC, 0xC0, 32);

	// Write to Device
	m_Grid.DeviceSlave_Write(v_TEC_APC, SLAVEADDR_A2, 0xC0, 32);

	p_cPB_OP->SetPos(100);

}

void CDS4830A_SFPP_ER_TEC_APC::Set_TEC_APC_Ctrls(unsigned char * v_TEC_APC, WORD uiBias)
{
	// > TEC Controls PROC
	unsigned char ucTECStatus = 0;

	// > Get TEC Active Status
	// get Static components
	CWnd *pStaticTEMPR = this->GetDlgItem(IDC_STATIC_TEMPR);

	// get Slider components
	CWnd *pSliderTEMPR = this->GetDlgItem(IDC_SLIDER_TEMPR);

	// get Values
	unsigned char ucTEC_Act = v_TEC_APC[0x00];
	ucTEC_Act &= 0x01;
	m_bCheck_TEC_act = ucTEC_Act;

	ucTEC_Act = v_TEC_APC[0x00];
	ucTEC_Act &= 0x02;
	m_bCheck_TEC_heat = ucTEC_Act;

	// Get TEC COOL/HEAT DAC Values
	unsigned char ucTEC_COOLP = v_TEC_APC[0x01];
	unsigned char ucTEC_COOLN = v_TEC_APC[0x02];
	unsigned char ucTEC_HEATP = v_TEC_APC[0x03];
	unsigned char ucTEC_HEATN = v_TEC_APC[0x04];

	// set Edit output control
	CString strGates;

	// Gates Cool
	strGates.Format(L"%d", ucTEC_COOLP);
	this->m_GateC_P = strGates;

	strGates.Truncate(0);
	strGates.Format(L"%d", ucTEC_COOLN);
	this->m_GateC_N = strGates;

	// Gates Heat
	strGates.Truncate(0);
	strGates.Format(L"%d", ucTEC_HEATP);
	this->m_GateH_P = strGates;

	strGates.Truncate(0);
	strGates.Format(L"%d", ucTEC_HEATN);
	this->m_GateH_N = strGates;

	// Set Slider output control
	// // invert P_GATES Values (P-MOSFET open when U-)
	ucTEC_COOLP = VALUE_TEC_GATEC_MAX - ucTEC_COOLP;
	ucTEC_HEATP = VALUE_TEC_GATEH_MAX - ucTEC_HEATP;

	int iPos1 = 0;			// Slider TEMPR position

	if ((ucTEC_COOLP == VALUE_TEC_GATEC_MIN) && (ucTEC_COOLN == VALUE_TEC_GATEC_MIN))
	{
		// [COOL OFF]

		// check HEAT circiut
		if ((ucTEC_HEATP == VALUE_TEC_GATEH_MIN) && (ucTEC_HEATN == VALUE_TEC_GATEH_MIN))
		{
			// [HEAT OFF]

			// Valid State
			iPos1 = 0;

		}
		else
		{
			// [HEAT ON]

			// Valid State
			iPos1 = -(ucTEC_HEATP);
		}
	}
	else
	{
		// [COOL ON]

		// check HEAT circiut
		if ((ucTEC_HEATP == VALUE_TEC_GATEH_MIN) && (ucTEC_HEATN == VALUE_TEC_GATEH_MIN))
		{
			// [HEAT OFF]

			// Valid State 
			iPos1 = ucTEC_COOLP;
		}
		else
		{
			// [HEAT OFF]

			//error State
			ucTECStatus = 1;

		}
	} // check OP

	// output Slider
	if (ucTECStatus == 0)
	{
		// [VALID STATUS]
		this->m_Slider_Tempr.SetPos(iPos1);

		// output Static control
		// // convert to percent
		int iPosPercent = iPos1 * 100 / VALUE_TEMPR_MAX;

		// // convert to str
		CString str1;
		str1.Format(L"%d", iPosPercent);
		str1.AppendChar('%');

		// Out to Interface control Label
		pStaticTEMPR->SetWindowTextW(str1);
	}
	else
	{
		// [INVALID STATUS]

	}

	// > Tempr output
	// get Static components
	CWnd *pStaticTEMPR_Cur = this->GetDlgItem(IDC_STATIC_TEMPR_CUR);

	// Get Tempr Values
	unsigned char ucTEMPR_Cur = v_TEC_APC[0x0B];
	unsigned char ucTEMPR_Opt = v_TEC_APC[0x05];

	// output Current
	// output Static
	// // convert to str
	CString str2;
	str2.Format(L"%d", ucTEMPR_Cur);

	// // Out to Interface control Label
	pStaticTEMPR_Cur->SetWindowTextW(str2);

	// output Optimum
	// output Edit
	str2.Truncate(0);
	str2.Format(L"%d", ucTEMPR_Opt);

	this->m_Edit_Tempr_Opt = str2;


	// > APD Controls PROC
	unsigned char ucAPDStatus = 0;

	// > Get TEC Active Status
	// get Static components
	CWnd *pStaticPOWER = this->GetDlgItem(IDC_STATIC_POWER);

	// get Slider components
	CWnd *pSliderPOWER = this->GetDlgItem(IDC_SLIDER_POWER);

	// get Values
	// Get TEC COOL/HEAT DAC Values
	unsigned char ucAPC_PD_MON_ANODE =	 v_TEC_APC[0x14];
	unsigned char ucAPC_PD_THRESHOLD01 = v_TEC_APC[0x16];
	unsigned char ucAPC_PD_THRESHOLD02 = v_TEC_APC[0x17];
	unsigned char ucAPC_PD_CORRECT_GAP = v_TEC_APC[0x18];

	unsigned char ucAPC_Act = v_TEC_APC[0x10];
	ucAPC_Act &= 0x01;
	m_bCheck_Power = ucAPC_Act;

	ucAPC_Act = v_TEC_APC[0x10];
	ucAPC_Act &= 0x02;
	m_bCheck_Photo = ucAPC_Act;

	// output Current Power Value
	CString strThreshold;

	// get Static components
	CWnd *pStaticPOWER_Cur = this->GetDlgItem(IDC_STATIC_POWER_CUR);
	
	WORD uiAPC_PD_MON_ANODE_Formatted = ucAPC_PD_MON_ANODE * 200;

	strThreshold.Format(L"%d", uiAPC_PD_MON_ANODE_Formatted);
	pStaticPOWER_Cur->SetWindowTextW(strThreshold);

	// output Threshold Value
	WORD threshold_word;
	threshold_word = ucAPC_PD_THRESHOLD01;
	threshold_word = threshold_word << 8;
	threshold_word += ucAPC_PD_THRESHOLD02;

	// // set Edit output control
	strThreshold.Format(L"%d", threshold_word);
	this->m_Edit_Power_Opt = strThreshold;

	// output CorrectGap Value
	strThreshold.Truncate(0);
	strThreshold.Format(L"%d", ucAPC_PD_CORRECT_GAP);
	this->m_Edit_Power_Gap = strThreshold;;

	// NOTE: Bit0 = APC_EN, Bit1 = APD_EN
	// get Static components
	CWnd *pStaticPOWER_Cur_Bias = this->GetDlgItem(IDC_STATIC_POWER_CUR_BIAS);

	strThreshold.Truncate(0);
	strThreshold.Format(L"%d", uiBias);
	pStaticPOWER_Cur_Bias->SetWindowTextW(strThreshold);

	UpdateData(FALSE);

}


void CDS4830A_SFPP_ER_TEC_APC::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_GRID, m_Grid);
	DDX_Control(pDX, IDC_SLIDER_TEMPR, m_Slider_Tempr);
	DDX_Control(pDX, IDC_SLIDER_POWER, m_Slider_Power);
	DDX_Check(pDX, IDC_CHECK_TEC_ACT, m_bCheck_TEC_act);
	DDX_Check(pDX, IDC_CHECK_TEC_HEAT, m_bCheck_TEC_heat);
	DDX_Text(pDX, IDC_EDIT_TEMPR_OPT, m_Edit_Tempr_Opt);
	DDX_Check(pDX, IDC_CHECK_POWER_ACTIVE, m_bCheck_Power);
	DDX_Check(pDX, IDC_CHECK_PHOTO_ACTIVE, m_bCheck_Photo);
	DDX_Text(pDX, IDC_EDIT_GATEC_P, m_GateC_P);
	DDX_Text(pDX, IDC_EDIT_GATEC_N, m_GateC_N);
	DDX_Text(pDX, IDC_EDIT_GATEH_P, m_GateH_P);
	DDX_Text(pDX, IDC_EDIT_GATEH_N, m_GateH_N);
	DDX_Text(pDX, IDC_EDIT_POWER_OPT, m_Edit_Power_Opt);
	DDX_Text(pDX, IDC_EDIT_POWER_GAP, m_Edit_Power_Gap);
}


void CDS4830A_SFPP_ER_TEC_APC::OnGridClick(NMHDR * pNotifyStruct, LRESULT * pResult)
{
	/*
	NM_GRIDVIEW* pItem = (NM_GRIDVIEW*)pNotifyStruct;
	Trace(_T("Clicked on row %d, col %d\n"), pItem->iRow, pItem->iColumn);
	Trace(_T("Text: %s\n"), m_Grid.GetItemText(pItem->iRow, pItem->iColumn));
	//Item.strText
	
	*/
}

void CDS4830A_SFPP_ER_TEC_APC::OnGridEndEdit(NMHDR * pNotifyStruct, LRESULT * pResult)
{
	NM_GRIDVIEW* pItem = (NM_GRIDVIEW*)pNotifyStruct;
	Trace(_T("End Edit on row %d, col %d\n"), pItem->iRow, pItem->iColumn);

	// > check Valid Input
	bool bInvalid = m_Grid.CheckValidHex(pItem->iRow, pItem->iColumn);

	*pResult = (bInvalid || m_Grid.m_bRejectEditChanges) ? -1 : 0;

	Trace(_T("Res: %d\n"), bInvalid && m_Grid.m_bRejectEditChanges);
}

// -------------------------------------------------------------------
// Timer Procedures
// -------------------------------------------------------------------
void CDS4830A_SFPP_ER_TEC_APC::StartTimer()
{
	m_nTimer = SetTimer(TIMER_ID_TEC_APC, 3000, NULL);

	// Set controls
	CWnd *pButton1 = this->GetDlgItem(IDC_BUTTON1);
	CWnd *pButton2 = this->GetDlgItem(IDC_BUTTON2);

	pButton1->EnableWindow(FALSE);
	pButton2->EnableWindow(TRUE);

	// Proc Timer OP
	this->OnBnClickedButton4();
}

void CDS4830A_SFPP_ER_TEC_APC::EditTimer()
{

}

void CDS4830A_SFPP_ER_TEC_APC::StopTimer()
{
	KillTimer(m_nTimer);

	// Set controls
	CWnd *pButton1 = this->GetDlgItem(IDC_BUTTON1);
	CWnd *pButton2 = this->GetDlgItem(IDC_BUTTON2);

	pButton2->EnableWindow(FALSE);
	pButton1->EnableWindow(TRUE);
}


// CDS4830A_SFPP_ER_TEC_APC message handlers

void CDS4830A_SFPP_ER_TEC_APC::OnBnClickedButtonConfWrite()
{
	// TODO: Add your control notification handler code here
}


// -------------------------------------------------------------------
// Table op call events
// -------------------------------------------------------------------

// Read Table Procedure
void CDS4830A_SFPP_ER_TEC_APC::OnBnClickedButton4()
{
	// TODO: Add your control notification handler code here
	ReadDevice();

}

// Write Table Procedure
void CDS4830A_SFPP_ER_TEC_APC::OnBnClickedButton5()
{
	// TODO: Add your control notification handler code here
	WriteDevice();

}


BOOL CDS4830A_SFPP_ER_TEC_APC::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here
	m_Grid.Init();

	// > Ctrls
	CtrlsInit();


	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}


void CDS4830A_SFPP_ER_TEC_APC::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// get Slider components
	CWnd *pSliderTEMPR = this->GetDlgItem(IDC_SLIDER_TEMPR);

	// NOTE: it's only one slider control in this Page, but maybe it will go another way in the future, so that is how it is.
	if (pScrollBar == pSliderTEMPR)			//  TEMPR Slider
	{
		// get Static components
		CWnd *pStaticTEMPR = this->GetDlgItem(IDC_STATIC_TEMPR);

		// > Update contents for Read controls
		UpdateData(TRUE);

		// > Control MAIN slider
		// Get current Slider position
		int iPos1 = this->m_Slider_Tempr.GetPos();

		// > Convert Value to double-sided slider position
		if (iPos1 > 0)
		{
			// [COOL SIDE]
			// NOTE: correlation is non linear!
		}
		else
		{
			// [HEAT SIDE]
		}

		// output Static control
		// // convert to percent
		int iPosPercent = iPos1 * 100 / VALUE_TEMPR_MAX;

		// // convert to str
		CString str1;
		str1.Format(L"%d", iPosPercent);
		str1.AppendChar('%');

		// Out to Interface control Label
		pStaticTEMPR->SetWindowTextW(str1);

	} //(pScrollBar == pSliderTEMPR)

	UpdateData(FALSE);

	CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);
}


// Set output control CHECK / [TEC EN]
void CDS4830A_SFPP_ER_TEC_APC::OnBnClickedCheckTecAct()
{
	unsigned char v_TEC_act[1];

	// Get Control Value
	UpdateData(TRUE);

	m_Grid.GridSFF_Read(v_TEC_act, 0xC0, 1);

	if (this->m_bCheck_TEC_act)
	{
		// [TEC ENABLING]

		// correct Value
		v_TEC_act[0] |= 0x01;		// Set bit0 = 1

	}
	else
	{
		// [TEC DISABLING]

		// correct Value
		v_TEC_act[0] &= 0xFE;		// Set bit0 = 0
	}

	// Set Value to Grid
	m_Grid.GridSFF_Write(v_TEC_act, 0xC0, 1);

	UpdateData(FALSE);

}


// Set output control CHECK / [TEC HT]
void CDS4830A_SFPP_ER_TEC_APC::OnBnClickedCheckTecHeat()
{
	unsigned char v_TEC_act[1];

	// Get Control Value
	UpdateData(TRUE);

	m_Grid.GridSFF_Read(v_TEC_act, 0xC0, 1);

	if (this->m_bCheck_TEC_heat)
	{
		// [TEC ENABLING]

		// correct Value
		v_TEC_act[0] |= 0x02;		// Set bit0 = 1


	}
	else
	{
		// [TEC DISABLING]

		// correct Value
		v_TEC_act[0] &= 0xFD;		// Set bit0 = 0
	}

	// Set Value to Grid
	m_Grid.GridSFF_Write(v_TEC_act, 0xC0, 1);

	UpdateData(FALSE);

}



void CDS4830A_SFPP_ER_TEC_APC::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	CDialogEx::OnActivate(nState, pWndOther, bMinimized);

	// TODO: Add your message handler code here

}


void CDS4830A_SFPP_ER_TEC_APC::OnBnClickedButton1()
{
	// Start Timer
	this->StartTimer();

}


void CDS4830A_SFPP_ER_TEC_APC::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	//m_service.activeState = SERVICE_STATE_DISABLING;

	this->OnBnClickedButton4();

	//m_service.activeState = SERVICE_STATE_ENABLING;


	CDialogEx::OnTimer(nIDEvent);
}


void CDS4830A_SFPP_ER_TEC_APC::OnBnClickedButton2()
{
	this->StopTimer();

}


// Set output control CHECK / [APC]
void CDS4830A_SFPP_ER_TEC_APC::OnBnClickedCheckPowerActive()
{
	unsigned char v_APC_act[1];

	// Get Control Value
	UpdateData(TRUE);

	m_Grid.GridSFF_Read(v_APC_act, 0xD0, 1);

	if (this->m_bCheck_Power)
	{
		// [APC ENABLING]

		// correct Value
		v_APC_act[0] |= 0x01;		// Set bit0 = 1

	}
	else
	{
		// [APC DISABLING]

		// correct Value
		v_APC_act[0] &= 0xFE;		// Set bit0 = 0
	}

	// Set Value to Grid
	m_Grid.GridSFF_Write(v_APC_act, 0xD0, 1);

	UpdateData(FALSE);

}

// Set output control CHECK / [APD]
void CDS4830A_SFPP_ER_TEC_APC::OnBnClickedCheckPhotoActive()
{
	unsigned char v_APC_act[1];

	// Get Control Value
	UpdateData(TRUE);

	m_Grid.GridSFF_Read(v_APC_act, 0xD0, 1);

	if (this->m_bCheck_Photo)
	{
		// [APD ENABLING]

		// correct Value
		v_APC_act[0] |= 0x02;		// Set bit1 = 1

	}
	else
	{
		// [APD DISABLING]

		// correct Value
		v_APC_act[0] &= 0xFD;		// Set bit1 = 0
	}

	// Set Value to Grid
	m_Grid.GridSFF_Write(v_APC_act, 0xD0, 1);

	UpdateData(FALSE);

}


void CDS4830A_SFPP_ER_TEC_APC::OnEnChangeEditTemprOpt()
{
	// Get Value current
	CString str_TemprOpt_curr = m_Edit_Tempr_Opt;
	//MessageBox(str_TemprOpt_curr);
	
	// > Check Valid changes
	// get Edit components
	CWnd *pEditTEMPR = this->GetDlgItem(IDC_EDIT_TEMPR_OPT);

	// Update contents for Read controls
	UpdateData(TRUE);

	// Get Value new 
	CString str_TemprOpt_new;
	pEditTEMPR->GetWindowTextW(str_TemprOpt_new);

	WORD uiTemprOpt = (WORD)_tcstoul(str_TemprOpt_new, NULL, 10);

	// > check Valid Value
	if ((uiTemprOpt > 255) || (uiTemprOpt < 0))
	{
		// [invalid Value]
		// restore Value
		m_Edit_Tempr_Opt = str_TemprOpt_curr;
	}

	UpdateData(FALSE);

	//MessageBox(str_TemprOpt_new);

}


void CDS4830A_SFPP_ER_TEC_APC::OnOK()
{
	// TODO: Add your specialized code here and/or call the base class

	// CDialogEx::OnOK();
}

// TODO:
// Get BIAL Value from ONET
// Write Threshold(1-2)
// Write Gap
// Monitor Slider Output controlles