// TB_SFP_R2.cpp : implementation file
//

/////////////////////////////////////////////////////////////////////////////
// Includes
/////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "afxdialogex.h"

#include "DSMCUService.h"
#include "TB_SFP_R2.h"



// ##########################################################################
// CTB_SFP_R2 dialog
// ##########################################################################
IMPLEMENT_DYNAMIC(CTB_SFP_R2, CDialogEx)

CTB_SFP_R2::CTB_SFP_R2(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_PROPPAGE_TB_SFP_R2, pParent)
{

}


CTB_SFP_R2::CTB_SFP_R2(HID_SMBUS_DEVICE * pHidSmbus, CWnd * pParent)
	: CDialogEx(IDD_PROPPAGE_BOOTLOAD, pParent)
	, m_pHidSmbus(pHidSmbus)
{

}

CTB_SFP_R2::~CTB_SFP_R2()
{

}


// -------------------------------------------------------------------
// MESSAGE_MAP
// -------------------------------------------------------------------
BEGIN_MESSAGE_MAP(CTB_SFP_R2, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_MON_START, &CTB_SFP_R2::OnBnClickedButtonMonStart)
	ON_BN_CLICKED(IDC_BUTTON_MON_STOP, &CTB_SFP_R2::OnBnClickedButtonMonStop)

	ON_BN_CLICKED(IDC_BUTTON_TX_DISABLE, &CTB_SFP_R2::OnBnClickedButtonTxDisable)
	ON_BN_CLICKED(IDC_BUTTON_RS_0, &CTB_SFP_R2::OnBnClickedButtonRs0)
	ON_BN_CLICKED(IDC_BUTTON_RS_1, &CTB_SFP_R2::OnBnClickedButtonRs1)
	ON_WM_TIMER()
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


void CTB_SFP_R2::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_OUTPUT, m_TraceWnd);
	DDX_Control(pDX, IDC_PB_OP, m_cPB_OP);
}


void CTB_SFP_R2::ProcLatchState()
{
	// > Get Latch Values from CP2112
	BYTE latchValue = 0;
	HID_SMBUS_STATUS status = HidSmbus_ReadLatch(*m_pHidSmbus, &latchValue);

	if (status != HID_SMBUS_SUCCESS)
	{
		//err read
	}

	// FORMAT:
	// GIPO.0 Ц Tx Toggle config.
	// GPIO.1 Ц Rx Toggle config.
	// GPIO.2 Ц Input.Tx Fault. 1 Ц ошибка в работе передатчика, 0 Ц все нормально.
	// GPIO.3 Ц Output.Tx Disable.Open - Drain. 1 Ц отключение лазера, 0 Ц включение.
	// GPIO.4 Ц Input.Mod ABS. 1 Ц модуль отсутствует, 0 Ц модуль присутствует.
	// GPIO.5 Ц Output.RS0.Open - Drain.ƒва состо€ни€ 1 и 0. –еализовать галочку.
	// GPIO.6 Ц Input.Rx LOS. 1 Ц сигнал на приеме ниже дозволенного уровн€, 0 Ц все нормально.
	// GPIO.7 Ц Output.RS1.Open - Drain.ƒва состо€ни€ 1 и 0. –еализовать галочку.

	// > Set Controls output
	// Get Label Controls
	CWnd *pStaticModAbs =		this->GetDlgItem(IDC_STATIC_MODABS_STATE);

	CWnd *pStaticTxDisable =	this->GetDlgItem(IDC_STATIC_TX_DISABLE_STATE);
	CWnd *pStaticRS0 =			this->GetDlgItem(IDC_STATIC_RS_0_STATE);
	CWnd *pStaticRS1 =			this->GetDlgItem(IDC_STATIC_RS_1_STATE);

	CWnd *pStaticTxFault =		this->GetDlgItem(IDC_STATIC_TX_FAULT_STATE);
	CWnd *pStaticRxLos =		this->GetDlgItem(IDC_STATIC_RX_LOS_STATE);

	// Get Button Controls
	CWnd *pButtonTxDisable =	this->GetDlgItem(IDC_BUTTON_TX_DISABLE);
	CWnd *pButtonRS0 =			this->GetDlgItem(IDC_BUTTON_RS_0);
	CWnd *pButtonRS1 =			this->GetDlgItem(IDC_BUTTON_RS_1);

	// Set State
	// [ModAbs]
	CString strStateText;
	if (latchValue & HID_SMBUS_MASK_GPIO_4)		strStateText.AppendFormat(L"[1]: модуль отсутствует");
	else										strStateText.AppendFormat(L"[0]: модуль присутствует");
	pStaticModAbs->SetWindowTextW(strStateText);

	// [TxDisable]
	strStateText.Truncate(0);
	CString strButtonText;

	if (latchValue & HID_SMBUS_MASK_GPIO_3)
	{
		strStateText.AppendFormat(L"[1]: лазер отключен");
		strStateText.AppendFormat(L"¬кл.");
	}
	else
	{
		strStateText.AppendFormat(L"[0]: лазер включен");
		strStateText.AppendFormat(L"¬ыкл.");
	}

	pStaticTxDisable->SetWindowTextW(strStateText);
	pButtonTxDisable->SetWindowTextW(strButtonText);


	// [RS0]
	strStateText.Truncate(0);
	if (latchValue & HID_SMBUS_MASK_GPIO_5)		strStateText.AppendFormat(L"[1]: высокий уровень");
	else										strStateText.AppendFormat(L"[0]: низкий уровень");
	pStaticRS0->SetWindowTextW(strStateText);

	// [RS1]
	strStateText.Truncate(0);
	if (latchValue & HID_SMBUS_MASK_GPIO_7)		strStateText.AppendFormat(L"[1]: высокий уровень");
	else										strStateText.AppendFormat(L"[0]: низкий уровень");
	pStaticRS1->SetWindowTextW(strStateText);

	// [TxFault]
	strStateText.Truncate(0);
	if (latchValue & HID_SMBUS_MASK_GPIO_2)		strStateText.AppendFormat(L"[1]: ошибка работы передатчика");
	else										strStateText.AppendFormat(L"[0]: передатчик исправен");
	pStaticTxFault->SetWindowTextW(strStateText);

	// [RxLos]
	strStateText.Truncate(0);
	if (latchValue & HID_SMBUS_MASK_GPIO_6)		strStateText.AppendFormat(L"[1]: слабый сигнал на приЄме");
	else										strStateText.AppendFormat(L"[0]: приЄм исправный");
	pStaticRxLos->SetWindowTextW(strStateText);

	Invalidate();
	/*
	
	// prepare state for LatchControl procedure //deactivate all output
	//write GPOI 05 06 07 pins default (000)
	status = HidSmbus_WriteLatch(*m_pHidSmbus, 0, 0xE0);
	// renew for reflect
	latchValue = (latchValue & 0x1F);

	if (status != HID_SMBUS_SUCCESS)
	{
	//err write
	}

	
	*/

}


// -------------------------------------------------------------------
// Timer Procedures
// -------------------------------------------------------------------
void CTB_SFP_R2::StartTimer()
{
	m_nTimer = SetTimer(TIMER_ID_TB_MON, 1000, NULL);

	// Set controls
	CWnd *pButton_MonStart = this->GetDlgItem(IDC_BUTTON_MON_START);
	CWnd *pButton_MonStop = this->GetDlgItem(IDC_BUTTON_MON_STOP);

	pButton_MonStart->EnableWindow(FALSE);
	pButton_MonStop->EnableWindow(TRUE);

	// Proc Timer OP

}


void CTB_SFP_R2::EditTimer()
{

}


void CTB_SFP_R2::StopTimer()
{
	KillTimer(m_nTimer);

	// Set controls
	CWnd *pButton_MonStart = this->GetDlgItem(IDC_BUTTON_MON_START);
	CWnd *pButton_MonStop = this->GetDlgItem(IDC_BUTTON_MON_STOP);

	pButton_MonStart->EnableWindow(TRUE);
	pButton_MonStop->EnableWindow(FALSE);
}


// -------------------------------------------------------------------
// CTB_SFP_R2 message handlers
// -------------------------------------------------------------------

void CTB_SFP_R2::OnBnClickedButtonMonStart()
{
	this->StartTimer();

}


void CTB_SFP_R2::OnBnClickedButtonMonStop()
{
	this->StopTimer();

}

// Set Custom State /TxDisable/
void CTB_SFP_R2::OnBnClickedButtonTxDisable()
{
	// Stop Timer PROC
	this->StopTimer();

	// > Get Previous State
	// Read CP2112 Latch Values
	BYTE latchValue = 0;
	HID_SMBUS_STATUS status = HidSmbus_ReadLatch(*m_pHidSmbus, &latchValue);

	if (status != HID_SMBUS_SUCCESS)
	{
		//err read
	}

	// CP2112 PROC Time
	Sleep(10);

	// Define TxDisable State from Values
	BYTE bTxDisable = latchValue & HID_SMBUS_MASK_GPIO_3;
	bTxDisable = bTxDisable >> 3;

	// > Set Toggle New State	
	bTxDisable = !bTxDisable;

	latchValue &= ~(HID_SMBUS_MASK_GPIO_3);
	latchValue |= (bTxDisable * HID_SMBUS_MASK_GPIO_3);


	// > Write State
	//write GPOI 05 06 07 pins default (000)
	status = HidSmbus_WriteLatch(*m_pHidSmbus, latchValue, 0xFF);

	if (status != HID_SMBUS_SUCCESS)
	{
		//err write
	}

	// CP2112 PROC Time
	Sleep(10);

	// > Update Controls
	this->ProcLatchState();

	// Restore Timer PROC
	this->StartTimer();

}



void CTB_SFP_R2::OnBnClickedButtonRs0()
{
	// Stop Timer PROC
	this->StopTimer();

	// > Get Previous State
	// Read CP2112 Latch Values
	BYTE latchValue = 0;
	HID_SMBUS_STATUS status = HidSmbus_ReadLatch(*m_pHidSmbus, &latchValue);

	if (status != HID_SMBUS_SUCCESS)
	{
		//err read
	}

	// CP2112 PROC Time
	Sleep(10);

	// Define TxDisable State from Values
	BYTE bTxDisable = latchValue & HID_SMBUS_MASK_GPIO_5;
	bTxDisable = bTxDisable >> 5;

	// > Set Toggle New State	
	bTxDisable = !bTxDisable;

	latchValue &= ~(HID_SMBUS_MASK_GPIO_5);
	latchValue |= (bTxDisable * HID_SMBUS_MASK_GPIO_5);


	// > Write State
	//write GPOI 05 06 07 pins default (000)
	status = HidSmbus_WriteLatch(*m_pHidSmbus, latchValue, 0xFF);

	if (status != HID_SMBUS_SUCCESS)
	{
		//err write
	}

	// CP2112 PROC Time
	Sleep(10);

	// > Update Controls
	this->ProcLatchState();

	// Restore Timer PROC
	this->StartTimer();

}


void CTB_SFP_R2::OnBnClickedButtonRs1()
{
	// Stop Timer PROC
	this->StopTimer();

	// > Get Previous State
	// Read CP2112 Latch Values
	BYTE latchValue = 0;
	HID_SMBUS_STATUS status = HidSmbus_ReadLatch(*m_pHidSmbus, &latchValue);

	if (status != HID_SMBUS_SUCCESS)
	{
		//err read
	}

	// CP2112 PROC Time
	Sleep(10);

	// Define TxDisable State from Values
	BYTE bTxDisable = latchValue & HID_SMBUS_MASK_GPIO_7;
	bTxDisable = bTxDisable >> 7;

	// > Set Toggle New State	
	bTxDisable = !bTxDisable;

	latchValue &= ~(HID_SMBUS_MASK_GPIO_7);
	latchValue |= (bTxDisable * HID_SMBUS_MASK_GPIO_7);


	// > Write State
	//write GPOI 05 06 07 pins default (000)
	status = HidSmbus_WriteLatch(*m_pHidSmbus, latchValue, 0xFF);

	if (status != HID_SMBUS_SUCCESS)
	{
		//err write
	}

	// CP2112 PROC Time
	Sleep(10);

	// > Update Controls
	this->ProcLatchState();

	// Restore Timer PROC
	this->StartTimer();
}


void CTB_SFP_R2::OnTimer(UINT_PTR nIDEvent)
{
	this->ProcLatchState();

	CDialogEx::OnTimer(nIDEvent);
}


HBRUSH CTB_SFP_R2::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// Create Temp Brush
	CBrush brushStatic = brushStatic.CreateStockObject(DEFAULT_PALETTE);

	// Get Static Controls
	CWnd *pStaticModAbs = this->GetDlgItem(IDC_STATIC_MODABS_STATE);

	CWnd *pStaticTxDisable = this->GetDlgItem(IDC_STATIC_TX_DISABLE_STATE);
	CWnd *pStaticRS0 = this->GetDlgItem(IDC_STATIC_RS_0_STATE);
	CWnd *pStaticRS1 = this->GetDlgItem(IDC_STATIC_RS_1_STATE);

	CWnd *pStaticTxFault = this->GetDlgItem(IDC_STATIC_TX_FAULT_STATE);
	CWnd *pStaticRxLos = this->GetDlgItem(IDC_STATIC_RX_LOS_STATE);

	// Define Colors
	COLORREF clr_Red = RGB(195, 13, 14);
	COLORREF clr_Green = RGB(13, 155, 14);
	COLORREF clr_Set;

	// Proceed Coloring

	// [ModAbs]
	if (pWnd->GetSafeHwnd() == pStaticModAbs->GetSafeHwnd())
	{
		CString strText;
		pStaticModAbs->GetWindowTextW(strText);

		if (strText[1] == '1') 	{ clr_Set = clr_Red; }
		else					{ clr_Set = clr_Green; 	}

		pDC->SetTextColor(clr_Set);
		pDC->SetBkMode(TRANSPARENT);

		return brushStatic;
	}
	
	// [TxDisable]
	if (pWnd->GetSafeHwnd() == pStaticTxDisable->GetSafeHwnd())
	{
		CString strText;
		pStaticTxDisable->GetWindowTextW(strText);

		if (strText[1] == '1')	{ clr_Set = clr_Red; }
		else					{ clr_Set = clr_Green; }

		pDC->SetTextColor(clr_Set);
		pDC->SetBkMode(TRANSPARENT);

		return brushStatic;
	}

	// [TxFault]
	if (pWnd->GetSafeHwnd() == pStaticTxFault->GetSafeHwnd())
	{
		CString strText;
		pStaticTxFault->GetWindowTextW(strText);

		if (strText[1] == '1')	{ clr_Set = clr_Red; }
		else					{ clr_Set = clr_Green; }

		pDC->SetTextColor(clr_Set);
		pDC->SetBkMode(TRANSPARENT);

		return brushStatic;
	}

	// [RxLos]
	if (pWnd->GetSafeHwnd() == pStaticRxLos->GetSafeHwnd())
	{
		CString strText;
		pStaticRxLos->GetWindowTextW(strText);

		if (strText[1] == '1')	{ clr_Set = clr_Red; }
		else					{ clr_Set = clr_Green; }

		pDC->SetTextColor(clr_Set);
		pDC->SetBkMode(TRANSPARENT);

		return brushStatic;
	}
	
	// TODO:  Change any attributes of the DC here
	// TODO:  Return a different brush if the default is not desired

	return hbr;
}
