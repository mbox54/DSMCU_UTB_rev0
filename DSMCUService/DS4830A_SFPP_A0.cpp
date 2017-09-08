// DS4830A_SFPP_A0.cpp : implementation file
//

#include "stdafx.h"
#include "DSMCUService.h"
#include "DS4830A_SFPP_A0.h"
#include "afxdialogex.h"


// CDS4830A_SFPP_A0 dialog

IMPLEMENT_DYNAMIC(CDS4830A_SFPP_A0, CDialog)

CDS4830A_SFPP_A0::CDS4830A_SFPP_A0(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_PROPPAGE_DS4830A_SFPP_A0, pParent)
{

}

CDS4830A_SFPP_A0::CDS4830A_SFPP_A0(HID_SMBUS_DEVICE * pHidSmbus, CProgressCtrl * p_cPB_OP, CEdit * p_EDIT_STATUS, st_serviceData * p_service, CWnd* pParent  /*=NULL*/)
	: CDialog(IDD_PROPPAGE_DS4830A_SFPP_A0, pParent)
	, m_pHidSmbus(pHidSmbus)
	, p_EDIT_STATUS(p_EDIT_STATUS)
	, p_cPB_OP(p_cPB_OP)
	, p_service(p_service)
	, m_Grid(m_pHidSmbus, p_cPB_OP, p_EDIT_STATUS, p_service)
{

}

CDS4830A_SFPP_A0::~CDS4830A_SFPP_A0()
{
}

void CDS4830A_SFPP_A0::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_GRID, m_Grid);
	DDX_Control(pDX, IDC_GRID2, m_GridASCII);
	DDX_Control(pDX, IDC_TRACE, m_TraceWnd);
	
}


// -------------------------------------------------------------------
// Supporting procedures
// -------------------------------------------------------------------
void CDS4830A_SFPP_A0::Trace(LPCTSTR szFmt, ...)
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

// -------------------------------------------------------------------
// Grid Events
// -------------------------------------------------------------------
void CDS4830A_SFPP_A0::OnGridClick(NMHDR * pNotifyStruct, LRESULT * pResult)
{
	NM_GRIDVIEW* pItem = (NM_GRIDVIEW*)pNotifyStruct;
	Trace(_T("Clicked on row %d, col %d\n"), pItem->iRow, pItem->iColumn);
	Trace(_T("Text: %s\n"), m_Grid.GetItemText(pItem->iRow, pItem->iColumn));

	CString str_cellText = m_Grid.GetItemText(pItem->iRow, pItem->iColumn);
	BYTE byte_cellText = (BYTE)_tcstoul(str_cellText, NULL, 16);

	Trace(_T("ASCII: %c\n"), byte_cellText);
	//Item.strText

	m_GridASCII.SetSelectedRange(pItem->iRow, pItem->iColumn, pItem->iRow, pItem->iColumn);
}

void CDS4830A_SFPP_A0::OnGrid2Click(NMHDR * pNotifyStruct, LRESULT * pResult)
{
	NM_GRIDVIEW* pItem = (NM_GRIDVIEW*)pNotifyStruct;
	Trace(_T("Clicked on row %d, col %d\n"), pItem->iRow, pItem->iColumn);
	Trace(_T("Text: %s\n"), m_GridASCII.GetItemText(pItem->iRow, pItem->iColumn));

	CString str_cellText = m_GridASCII.GetItemText(pItem->iRow, pItem->iColumn);
	BYTE byte_cellText = (BYTE)_tcstoul(str_cellText, NULL, 16);

	Trace(_T("ASCII: %c\n"), byte_cellText);
	//Item.strText

	m_Grid.SetSelectedRange(pItem->iRow, pItem->iColumn, pItem->iRow, pItem->iColumn);
}

void CDS4830A_SFPP_A0::OnGridEndEdit(NMHDR * pNotifyStruct, LRESULT * pResult)
{
	NM_GRIDVIEW* pItem = (NM_GRIDVIEW*)pNotifyStruct;
	Trace(_T("End Edit on row %d, col %d\n"), pItem->iRow, pItem->iColumn);

	// > check Valid Input
	bool bInvalid = m_Grid.CheckValidHex(pItem->iRow, pItem->iColumn);

	*pResult = (bInvalid || m_Grid.m_bRejectEditChanges) ? -1 : 0;

	// > update Grid ASCII
	if (*pResult == 0)
	{
		// read
		CString str_cellText = m_Grid.GetItemText(pItem->iRow, pItem->iColumn);

		// transfer
		BYTE byte_cellText = (BYTE)_tcstoul(str_cellText, NULL, 16);
	
		CString str;
		str.Format(_T("%c"), byte_cellText);

		// write
		m_GridASCII.SetItemText(pItem->iRow, pItem->iColumn, str);

		// refresh component
		m_GridASCII.Invalidate(0);
	}


	Trace(_T("Res: %d\n"), bInvalid && m_Grid.m_bRejectEditChanges);
}

void CDS4830A_SFPP_A0::OnGrid2EndEdit(NMHDR * pNotifyStruct, LRESULT * pResult)
{
	NM_GRIDVIEW* pItem = (NM_GRIDVIEW*)pNotifyStruct;
	Trace(_T("End Edit ACSII on row %d, col %d\n"), pItem->iRow, pItem->iColumn);

	// > check Valid Input
	bool bInvalid = m_GridASCII.CheckValidASCII(pItem->iRow, pItem->iColumn);

	*pResult = (bInvalid || m_GridASCII.m_bRejectEditChanges) ? -1 : 0;

	// > update Grid Hex
	if (*pResult == 0)
	{
		CString item_text = m_GridASCII.GetItemText(pItem->iRow, pItem->iColumn);
		BYTE itemData = item_text[0];

		CString str;
		str.Format(_T("%02X"), itemData);
		m_Grid.SetItemText(pItem->iRow, pItem->iColumn, str);

		// refresh component
		m_Grid.Invalidate(0);
	}
}

// -------------------------------------------------------------------
// MESSAGE_MAP
// -------------------------------------------------------------------
BEGIN_MESSAGE_MAP(CDS4830A_SFPP_A0, CDialog)

	// Grid event functions
	ON_NOTIFY(NM_CLICK, IDC_GRID, OnGridClick)
	ON_NOTIFY(NM_CLICK, IDC_GRID2, OnGrid2Click)
	ON_NOTIFY(GVN_ENDLABELEDIT, IDC_GRID, OnGridEndEdit)
	ON_NOTIFY(GVN_ENDLABELEDIT, IDC_GRID2, OnGrid2EndEdit)

	ON_BN_CLICKED(IDC_BUTTON1, &CDS4830A_SFPP_A0::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CDS4830A_SFPP_A0::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CDS4830A_SFPP_A0::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4, &CDS4830A_SFPP_A0::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON5, &CDS4830A_SFPP_A0::OnBnClickedButton5)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// -------------------------------------------------------------------
// Timer Procedures
// -------------------------------------------------------------------
void CDS4830A_SFPP_A0::StartTimer()
{
	m_nTimer = SetTimer(1, 100, NULL);

}

void CDS4830A_SFPP_A0::StopTimer()
{
	KillTimer(m_nTimer);
}

void CDS4830A_SFPP_A0::OnTimer(UINT_PTR nIDEvent)
{
	unsigned char uTablValues[256];

	if (p_service->activeState == SERVICE_STATE_DISABLE)
	{
		if (!m_bWasReading)
		{
			m_bWasReading = TRUE;
		}		
	}
	else
	{
		if (p_service->activeState == SERVICE_STATE_ENABLE)
		{
			if (m_bWasReading)	// Grid ASCII update check
			{
				// > Update ASCII Grid
				// read
				m_Grid.GridSFF_Read(uTablValues, 0, 256);

				// write
				m_GridASCII.GridSFF_Write_Byte(uTablValues, 0, 256);

				// disable op
				m_bWasReading = FALSE;
			}
		}
	}

	CDialog::OnTimer(nIDEvent);
}

// -------------------------------------------------------------------
// CDS4830A_SFPP_A0 message handlers
// -------------------------------------------------------------------
BOOL CDS4830A_SFPP_A0::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	m_Grid.Init();
	m_Grid.SetTableColor();

	m_GridASCII.Init();
	m_GridASCII.SetTableColor();

	// init var
	m_bWasReading = 0;

	// start service timer
	this->StartTimer();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}


// -------------------------------------------------------------------
// Buttons Test
// -------------------------------------------------------------------
void CDS4830A_SFPP_A0::OnBnClickedButton1()
{
	// TODO: Add your control notification handler code here
	BYTE uValues[256];

	// fill with probe
	for (int k = 0; k < 256; k++)
	{
		uValues[k] = k;
	}

	//	UpdateData(TRUE);
	m_Grid.GridSFF_Write(uValues, 0, 256);

	UpdateData(FALSE);
}


void CDS4830A_SFPP_A0::OnBnClickedButton2()
{
	// TODO: Add your control notification handler code here

	m_Grid.GridSFF_Read(uValues, 0, 256);

}


void CDS4830A_SFPP_A0::OnBnClickedButton3()
{
	// TODO: Add your control notification handler code here

	m_Grid.GridSFF_Write(uValues, 0, 256);
}
// *******************************************************************


// -------------------------------------------------------------------
// Table op call events
// -------------------------------------------------------------------

// Read Table Procedure
void CDS4830A_SFPP_A0::OnBnClickedButton4()
{
	// Read op
	// NOTE: update INTERFACE included in DeviceSlave_ReadTimer

	// NOTE: debug from ver1.1 'DDM-auto' need more stability to Timer_R/W, so it replaces to previous
	//m_Grid.DeviceSlave_ReadTimer(uValues, 0, SLAVEADDR_A0, 0, 256, 0, 0);

	//
	//Trace(_T("trying: READ\n"));
	//Trace(_T("params: A0, 256 bytes\n"));

	// prepare Table output
	// clear Table
	m_Grid.ClearTable();


	Trace(_T("ïîïûòêà: ×ÒÅÍÈÅ\n"));
	Trace(_T("ïàðàìåòðû: A0, 256 áàéò\n"));

	// temp buffer for OP
	BYTE v_Values[256];

	// get Data from Device
	BYTE retVal = m_Grid.DeviceSlave_Read(v_Values, SLAVEADDR_A0, 0, 256);
	
	if (retVal != HID_SMBUS_SUCCESS)
	{
		// error: Device Read
		Trace(_T("ÎØÈÁÊÀ. [êîä: %02d] \n"), retVal);

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

	//Trace(_T("SUCCESS! \n"));
	Trace(_T("ÓÑÏÅØÍÎ.\n"));
	Trace(_T("-----------------------\n"));

// !debug
	/*
	BYTE uValues[256];

	// fill with probe /!debug
	for (int k = 0; k < 256; k++)
	{
		uValues[k] = k;
	}
	*/
}

// Write Table Procedure
void CDS4830A_SFPP_A0::OnBnClickedButton5()
{
	// BYTE uValues[256]; ! Need Global Var for Timer proc
	// NOTE: update VALUES  included in DeviceSlave_WriteTimer 

	// NOTE: debug from ver1.1 'DDM-auto' need more stability to Timer_R/W, so it replaces to previous

	Trace(_T("ïîïûòêà: ÇÀÏÈÑÜ\n"));
	Trace(_T("ïàðàìåòðû: A0, 256 áàéò\n"));

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
		Trace(_T("ÎØÈÁÊÀ. [êîä: %02d] \n"), retVal);
		return;
	}

	Trace(_T("ÓÑÏÅØÍÎ.\n"));
	Trace(_T("-----------------------\n"));
}



void CDS4830A_SFPP_A0::OnOK()
{
	// TODO: Add your specialized code here and/or call the base class

	// CDialog::OnOK();
}
