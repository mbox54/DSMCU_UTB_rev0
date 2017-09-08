// DS4830A_SFPP_MSA.cpp : implementation file
//

#include "stdafx.h"
#include "DSMCUService.h"
#include "DS4830A_SFPP_MSA.h"
#include "afxdialogex.h"


// CDS4830A_SFPP_MSA dialog

IMPLEMENT_DYNAMIC(CDS4830A_SFPP_MSA, CDialogEx)

CDS4830A_SFPP_MSA::CDS4830A_SFPP_MSA(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_PROPPAGE_DS4830A_SFPP_MSA, pParent)
{

}

CDS4830A_SFPP_MSA::CDS4830A_SFPP_MSA(HID_SMBUS_DEVICE * pHidSmbus, CProgressCtrl * p_cPB_OP, CEdit * p_EDIT_STATUS, st_serviceData * p_service, CWnd * pParent)
	: CDialogEx(IDD_PROPPAGE_DS4830A_SFPP_MSA, pParent)
, m_pHidSmbus(pHidSmbus)
, p_EDIT_STATUS(p_EDIT_STATUS)
, p_cPB_OP(p_cPB_OP)
, p_service(p_service)
, m_Grid(m_pHidSmbus, p_cPB_OP, p_EDIT_STATUS, p_service)

{

}


CDS4830A_SFPP_MSA::~CDS4830A_SFPP_MSA()
{

}


BEGIN_MESSAGE_MAP(CDS4830A_SFPP_MSA, CDialogEx)
	// Grid event functions
	ON_NOTIFY(GVN_ENDLABELEDIT, IDC_GRID_DESCR, OnGridDescrEndEdit)

	// Dialog support controls
	ON_BN_CLICKED(IDC_BUTTON1, &CDS4830A_SFPP_MSA::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON4, &CDS4830A_SFPP_MSA::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON5, &CDS4830A_SFPP_MSA::OnBnClickedButton5)
	ON_BN_CLICKED(IDC_BUTTON2, &CDS4830A_SFPP_MSA::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON6, &CDS4830A_SFPP_MSA::OnBnClickedButton6)
END_MESSAGE_MAP()


void CDS4830A_SFPP_MSA::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_GRID, m_Grid);
	DDX_Control(pDX, IDC_GRID_DESCR, m_GridDescr);
}


void CDS4830A_SFPP_MSA::Trace(LPCTSTR szFmt, ...)
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


void CDS4830A_SFPP_MSA::OnGridEndEdit(NMHDR * pNotifyStruct, LRESULT * pResult)
{

}


void CDS4830A_SFPP_MSA::OnGridDescrEndEdit(NMHDR * pNotifyStruct, LRESULT * pResult)
{
	// NOTE: parent event is used. No need to implement here.
	int bChanged;
	bChanged = this->m_GridDescr.OnGridEndEdit(pNotifyStruct, pResult);

	if (bChanged == 0)
	{
		// update SFF Base Grid
		NM_GRIDVIEW* pItem = (NM_GRIDVIEW*)pNotifyStruct;

		// get value
		CString str_cellText = m_GridDescr.GetItemText(pItem->iRow, 2);

		// transfer
		BYTE byte_cellText = (BYTE)_tcstoul(str_cellText, NULL, 16);

		// set
		unsigned char v_temp[1];
		v_temp[0] = byte_cellText;
		m_Grid.GridSFF_Write(v_temp, pItem->iRow - 1, 1);

		// place selector
		unsigned char uGridRow;
		unsigned char uGridCol;

		uGridRow = (pItem->iRow - 1) / 16;
		uGridCol = (pItem->iRow - 1) - uGridRow * 16;

		m_Grid.SetSelectedRange(uGridRow + 1, uGridCol + 1, uGridRow + 1, uGridCol + 1);

		// refresh component
		m_Grid.Invalidate(0);
	}
	
}


// CDS4830A_SFPP_MSA message handlers


void CDS4830A_SFPP_MSA::OnBnClickedButton1()
{
	// > test CheckSum
	BYTE uValues[128];
	BYTE v_CHK_curr[1];

	CString str;

	// CHK63
	// // get data
	m_Grid.GridSFF_Read(uValues, 0, 63);
	m_Grid.GridSFF_Read(v_CHK_curr, 63, 1);

	// // check // compare
	unsigned char ucCHK_check = CheckCHK1(v_CHK_curr[0], uValues, 63);

	// > set controls output
	if (ucCHK_check)
	{
		// [SUCCESS]

		str.AppendFormat(_T("Проверка пройдена."));
	}
	else
	{
		// [FAILURE]

		str.AppendFormat(_T("Чексумма неверна"));
	}

	CWnd *pStaticCHK63 = this->GetDlgItem(IDC_STATIC_CHK_63);
	pStaticCHK63->SetWindowTextW(str);		


	// CHK95
	// // get data
	m_Grid.GridSFF_Read(uValues, 64, 31);
	m_Grid.GridSFF_Read(v_CHK_curr, 95, 1);

	// // check // compare
	ucCHK_check = CheckCHK1(v_CHK_curr[0], uValues, 31);

	// > set controls output
	str.Truncate(0);

	if (ucCHK_check)
	{
		// [SUCCESS]

		str.AppendFormat(_T("Проверка пройдена."));
	}
	else
	{
		// [FAILURE]

		str.AppendFormat(_T("Чексумма неверна"));
	}

	CWnd *pStaticCHK95 = this->GetDlgItem(IDC_STATIC_CHK_95);
	pStaticCHK95->SetWindowTextW(str);


//	unsigned char ucCompRes = CheckCHK1(34, )

	// TODO: Add your control notification handler code here
	/*
	BYTE uValues[256];

	// fill with probe
	for (int k = 0; k < 256; k++)
	{
		uValues[k] = k;
	}

	//	UpdateData(TRUE);
	m_Grid.GridSFF_Write(uValues, 0, 256);

	UpdateData(FALSE);
	*/
}

// Update CHK
void CDS4830A_SFPP_MSA::OnBnClickedButton6()
{
	// > test CheckSum
	BYTE uValues[128];
	BYTE v_CHK_curr[1];

	CString str;

	// CHK63
	// // get data
	m_Grid.GridSFF_Read(uValues, 0, 63);

	// // calculate
	v_CHK_curr[0] = CalculateHash1(uValues, 63);

	// // set
	m_Grid.GridSFF_Write(v_CHK_curr, 63, 1);

	// CHK95
	// // get data
	m_Grid.GridSFF_Read(uValues, 64, 31);

	// // calculate
	v_CHK_curr[0] = CalculateHash1(uValues, 31);

	// // set
	m_Grid.GridSFF_Write(v_CHK_curr, 95, 1);

}


BOOL CDS4830A_SFPP_MSA::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_GridDescr.Init("MSA_tablDef.txt");
	m_Grid.Init();

	// specify SFF Table for preview mode
	// header
	m_Grid.SetColumnWidth(0, 30);
	m_Grid.SetRowHeight(0, 18);

	//body
	for (unsigned char k = 1; k < 17; k++)
	{
		m_Grid.SetColumnWidth(k, 18);
		m_Grid.SetRowHeight(k, 17);
	}
	

	// font
	// Initializes a CFont object with the specified characteristics. 
	CFont font;
	VERIFY(font.CreateFont(
		0,                         // nHeight
		4,                         // nWidth
		0,                         // nEscapement
		0,                         // nOrientation
		FW_NORMAL,                 // nWeight
		FALSE,                     // bItalic
		FALSE,                     // bUnderline
		0,                         // cStrikeOut
		ANSI_CHARSET,              // nCharSet
		OUT_DEFAULT_PRECIS,        // nOutPrecision
		CLIP_DEFAULT_PRECIS,       // nClipPrecision
		DEFAULT_QUALITY,           // nQuality
		DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
		_T("Lucida Fax")));        // lpszFacename

	m_Grid.SetFont(&font);


	// > Fill all subordinate to HEX Fields with HEX init
	// get HEX field Values
	unsigned char uValueTemp[128];

	for (unsigned short k = 0; k < 128; k++)
	{
		CString str_cellText = m_GridDescr.GetItemText(k + 1, 2);
		unsigned char byte_cellText = (BYTE)_tcstoul(str_cellText, NULL, 16);

		uValues[k] = byte_cellText;
	}

	// set subordinate Fields
	m_GridDescr.UpdateTable_All(uValues);
	
	m_GridDescr.SetTableColor();
	m_GridDescr.Invalidate(0);


	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}


// -------------------------------------------------------------------
// Table op call events
// -------------------------------------------------------------------

// Read Table Procedure
void CDS4830A_SFPP_MSA::OnBnClickedButton4()
{
	// TODO: Add your control notification handler code here
	// Read op
	// NOTE: update INTERFACE included in DeviceSlave_ReadTimer

	// NOTE: debug from ver1.1 'DDM-auto' need more stability to Timer_R/W, so it replaces to previous
	//m_Grid.DeviceSlave_ReadTimer(uValues, 0, SLAVEADDR_A0, 0, 256, 0, 0);

	//
	//Trace(_T("trying: READ\n"));
	//Trace(_T("params: A0, 256 bytes\n"));
	Trace(_T("попытка: ЧТЕНИЕ\n"));
	Trace(_T("параметры: A0, 256 байт\n"));

	// prepare Table output
	// clear Table
	m_Grid.ClearTable();

	// temp buffer for OP
	BYTE v_Values[256];

	// get Data from Device
	BYTE retVal = m_Grid.DeviceSlave_Read(v_Values, SLAVEADDR_A0, 0, 256);

	if (retVal != HID_SMBUS_SUCCESS)
	{
		// error: Device Read
		Trace(_T("ОШИБКА. [код: %02d] \n"), retVal);

		CString str_ErrText;
		switch (retVal)
		{
		case 1:
			str_ErrText.AppendFormat(L"(MAXQBL_NO_DEVICE)");

			break;

		case 2:
			str_ErrText.AppendFormat(L"(MAXQBL_DEV_INACCESSIBLE)");

			break;

		case 3:
			str_ErrText.AppendFormat(L"(MAXQBL_OP_FAILED)");

			break;

		case 4:
			str_ErrText.AppendFormat(L"(MAXQBL_OP_WRONG)");

			break;

		default:
			break;
		}

		// err comment output
		Trace(str_ErrText);

		return;

	}
	else
	{
		// output to Grid
		m_Grid.GridSFF_Write(v_Values, 0, 256);
	}


	//m_Grid.GridSFF_Read(v_Values, 0, 256);

	m_GridDescr.UpdateTable_All(v_Values);
	m_GridDescr.Invalidate(0);


	//Trace(_T("SUCCESS! \n"));
	Trace(_T("УСПЕШНО.\n"));
	Trace(_T("-----------------------\n"));
}


// Write Table Procedure
void CDS4830A_SFPP_MSA::OnBnClickedButton5()
{
	// BYTE uValues[256]; ! Need Global Var for Timer proc
	// NOTE: update VALUES  included in DeviceSlave_WriteTimer 

	// NOTE: debug from ver1.1 'DDM-auto' need more stability to Timer_R/W, so it replaces to previous

	Trace(_T("попытка: ЗАПИСЬ\n"));
	Trace(_T("параметры: A0, 256 байт\n"));

	// temp buffer for OP
	BYTE v_Values[256];

	// input from Grid
	m_Grid.GridSFF_Read(v_Values, 0, 256);

	// write op
	//m_Grid.DeviceSlave_WriteTimer(uValues, 0, SLAVEADDR_A0, 0, 256, 0, 0);

	// set Data to Device
	BYTE retVal = m_Grid.DeviceSlave_Write(v_Values, SLAVEADDR_A0, 0, 256);

	if (retVal != HID_SMBUS_SUCCESS)
	{
		// error: Device Write
		Trace(_T("ОШИБКА. [код: %02d] \n"), retVal);
		return;
	}

	Trace(_T("УСПЕШНО.\n"));
	Trace(_T("-----------------------\n"));
}


void CDS4830A_SFPP_MSA::OnBnClickedButton2()
{
	// TODO: Add your control notification handler code here

}



void CDS4830A_SFPP_MSA::OnOK()
{
	// TODO: Add your specialized code here and/or call the base class

	// CDialogEx::OnOK();
}
