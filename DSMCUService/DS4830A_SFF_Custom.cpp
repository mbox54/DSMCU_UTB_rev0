// DS4830A_SFF_Custom.cpp : implementation file
//

#include "stdafx.h"
#include "DSMCUService.h"
#include "DS4830A_SFF_Custom.h"
#include "afxdialogex.h"

// ##########################################################################
// CDS4830A_SFF_Custom dialog
// ##########################################################################
IMPLEMENT_DYNAMIC(CDS4830A_SFF_Custom, CDialog)

CDS4830A_SFF_Custom::CDS4830A_SFF_Custom(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_PROPPAGE_DS4830A_SFPP_CUSTOM, pParent)
	, m_SLA_ADDR(_T(""))
	, m_TABL_ADDR(_T(""))
	, m_TABL_NAME(_T(""))
	, m_sEdit_PasValue(_T(""))
	, m_sEdit_PasAddr(_T(""))
	, m_bCheck_PasEnable(FALSE)
	, m_bCheck_SelRange(FALSE)
	, m_SelAddr(_T(""))
	, m_SelCount(_T(""))
	, m_bCheck_TabSelect(FALSE)
{

}

CDS4830A_SFF_Custom::CDS4830A_SFF_Custom(HID_SMBUS_DEVICE * pHidSmbus, CProgressCtrl * p_cPB_OP, CEdit * p_EDIT_STATUS, st_serviceData * p_service, CWnd * pParent)
	: CDialog(IDD_PROPPAGE_DS4830A_SFPP_CUSTOM, pParent)
	, m_pHidSmbus(pHidSmbus)
	, p_EDIT_STATUS(p_EDIT_STATUS)
	, p_cPB_OP(p_cPB_OP)
	, p_service(p_service)
	, m_Grid(m_pHidSmbus, p_cPB_OP, p_EDIT_STATUS, p_service)

{
}

CDS4830A_SFF_Custom::~CDS4830A_SFF_Custom()
{
}

void CDS4830A_SFF_Custom::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_GRID, m_Grid);
	DDX_Control(pDX, IDC_TRACE, m_TraceWnd);
	DDX_Text(pDX, IDC_EDIT_SLAADDR, m_SLA_ADDR);
	DDX_Text(pDX, IDC_EDIT_TABLADDR, m_TABL_ADDR);
	DDX_Text(pDX, IDC_EDIT_TABLNAME, m_TABL_NAME);
	//  DDX_Text(pDX, IDC_EDIT_PASADDR, m_Edit_PasAddr);
	//  DDX_Control(pDX, IDC_EDIT_PASVALUE, m_Edit_PasValue);
	DDX_Text(pDX, IDC_EDIT_PASVALUE, m_sEdit_PasValue);
	//  DDX_Control(pDX, IDC_EDIT_PASADDR, m_sEdit_PasAddr);
	DDX_Text(pDX, IDC_EDIT_PASADDR, m_sEdit_PasAddr);
	DDX_Check(pDX, IDC_CHECK_PASENABLE, m_bCheck_PasEnable);
	DDX_Control(pDX, IDC_EDIT_SLAADDR, m_Edit_SlaveAddr);
	DDX_Control(pDX, IDC_EDIT_TABLADDR, m_Edit_TableAddr);
	DDX_Control(pDX, IDC_EDIT_TABLNAME, m_Edit_TableName);
	DDX_Control(pDX, IDC_EDIT_PASADDR, m_Edit_PasAddr);
	DDX_Control(pDX, IDC_EDIT_PASVALUE, m_Edit_PasValue);
	DDX_Check(pDX, IDC_CHECK_SELENABLE, m_bCheck_SelRange);
	DDX_Text(pDX, IDC_EDIT_SELADDR, m_SelAddr);
	//  DDV_MaxChars(pDX, m_SelAddr, 2);
	//  DDX_Control(pDX, IDC_EDIT_SELCNT, m);
	DDX_Text(pDX, IDC_EDIT_SELCNT, m_SelCount);
	DDV_MaxChars(pDX, m_SelCount, 3);
	DDX_Check(pDX, IDC_CHECK_TABENABLE, m_bCheck_TabSelect);
}


// -------------------------------------------------------------------
// Supporting procedures
// -------------------------------------------------------------------
void CDS4830A_SFF_Custom::Trace(LPCTSTR szFmt, ...)
{
	CString str;

	// Format the message text
	va_list argptr;
	va_start(argptr, szFmt);
	str.FormatV(szFmt, argptr);
	va_end(argptr);

	str.Replace(_T("\n"), _T("\r\n"));

	CString strWndText;
	m_TraceWnd.GetWindowText(strWndText);
	strWndText += str;
	m_TraceWnd.SetWindowText(strWndText);

	//	m_TraceWnd.SetSel(str.GetLength()-1, str.GetLength()-2, FALSE);
	m_TraceWnd.LineScroll(-m_TraceWnd.GetLineCount());
	m_TraceWnd.LineScroll(m_TraceWnd.GetLineCount() - 4);
}

void CDS4830A_SFF_Custom::EditHexControl(CEdit * pEdit)
{
	CString str;

	pEdit->GetWindowTextW(str);

	// valid symbol checking
	CString hexTextValid = str.SpanIncluding(_T("0123456789abcdefABCDEF"));

	// check for invalid hex symbol
	if (hexTextValid.Compare(str) != 0)
	{
		// clear control Text
		pEdit->SetWindowTextW(_T(""));
	}

}

void CDS4830A_SFF_Custom::EditInit()
{

	// init EditBoxes

	// set default text values
	m_SLA_ADDR = (CString)"A2";
	m_TABL_ADDR = (CString)"7F";
	m_TABL_NAME = (CString)"00";

	m_sEdit_PasAddr = (CString)"7B";
	m_sEdit_PasValue = (CString)"4F505759";

	// set Edit control parameter
	m_Edit_SlaveAddr.SetLimitText(2);
	m_Edit_TableAddr.SetLimitText(2);
	m_Edit_TableName.SetLimitText(2);

	m_Edit_PasAddr.SetLimitText(2);

}

void CDS4830A_SFF_Custom::OnGridClick(NMHDR * pNotifyStruct, LRESULT * pResult)
{
	NM_GRIDVIEW* pItem = (NM_GRIDVIEW*)pNotifyStruct;
	Trace(_T("Clicked on row %d, col %d\n"), pItem->iRow, pItem->iColumn);
	Trace(_T("Text: %s\n"), m_Grid.GetItemText(pItem->iRow, pItem->iColumn));
	//Item.strText
}

void CDS4830A_SFF_Custom::OnGridEndEdit(NMHDR * pNotifyStruct, LRESULT * pResult)
{
	NM_GRIDVIEW* pItem = (NM_GRIDVIEW*)pNotifyStruct;
	Trace(_T("End Edit on row %d, col %d\n"), pItem->iRow, pItem->iColumn);

	// > check Valid Input
	bool bInvalid = m_Grid.CheckValidHex(pItem->iRow, pItem->iColumn);

	*pResult = (bInvalid || m_Grid.m_bRejectEditChanges) ? -1 : 0;

	Trace(_T("Res: %d\n"), bInvalid && m_Grid.m_bRejectEditChanges);
}

// -------------------------------------------------------------------
// MESSAGE_MAP
// -------------------------------------------------------------------
BEGIN_MESSAGE_MAP(CDS4830A_SFF_Custom, CDialog)
	// Grid event functions
	ON_NOTIFY(NM_CLICK, IDC_GRID, OnGridClick)
	ON_NOTIFY(GVN_ENDLABELEDIT, IDC_GRID, OnGridEndEdit)

	ON_BN_CLICKED(IDC_BUTTON1, &CDS4830A_SFF_Custom::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CDS4830A_SFF_Custom::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CDS4830A_SFF_Custom::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4, &CDS4830A_SFF_Custom::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON5, &CDS4830A_SFF_Custom::OnBnClickedButton5)
	ON_EN_CHANGE(IDC_EDIT_SLAADDR, &CDS4830A_SFF_Custom::OnEnChangeEditSlaaddr)
	ON_EN_CHANGE(IDC_EDIT_TABLADDR, &CDS4830A_SFF_Custom::OnEnChangeEditTabladdr)
	ON_EN_CHANGE(IDC_EDIT_TABLNAME, &CDS4830A_SFF_Custom::OnEnChangeEditTablname)
	ON_EN_CHANGE(IDC_EDIT_PASADDR, &CDS4830A_SFF_Custom::OnEnChangeEditPasaddr)
	ON_EN_CHANGE(IDC_EDIT_PASVALUE, &CDS4830A_SFF_Custom::OnEnChangeEditPasvalue)
END_MESSAGE_MAP()

// -------------------------------------------------------------------
// CDS4830A_SFF_Custom message handlers
// -------------------------------------------------------------------
BOOL CDS4830A_SFF_Custom::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_Grid.Init();

	// init default Device options
	this->EditInit();

	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}

// -------------------------------------------------------------------
// Buttons Test
// -------------------------------------------------------------------
void CDS4830A_SFF_Custom::OnBnClickedButton1()
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


void CDS4830A_SFF_Custom::OnBnClickedButton2()
{
	// TODO: Add your control notification handler code here

	m_Grid.GridSFF_Read(uValues2, 0, 256);
}


void CDS4830A_SFF_Custom::OnBnClickedButton3()
{
	// TODO: Add your control notification handler code here
	m_Grid.GridSFF_Write(uValues2, 0, 256);
}

// -------------------------------------------------------------------
// Table op call events
// -------------------------------------------------------------------

// Read Table Procedure
void CDS4830A_SFF_Custom::OnBnClickedButton4()
{
	UpdateData(TRUE);

	// get options
	BYTE uSLA_ADDR = (BYTE)_tcstoul(m_SLA_ADDR, NULL, 16);
	BYTE uTABL_ADDR = (BYTE)_tcstoul(m_TABL_ADDR, NULL, 16);
	BYTE uTABL_NAME = (BYTE)_tcstoul(m_TABL_NAME, NULL, 16);

	BYTE uPAS_ADDR = (BYTE)_tcstoul(m_sEdit_PasAddr, NULL, 16);
	//BYTE uPAS_VALUE = (BYTE)_tcstoul(m_SLA_ADDR, NULL, 16);

	BYTE uSEL_ADDR = (BYTE)_tcstoul(m_SelAddr, NULL, 16);
	WORD uSEL_CNT = (WORD)_tcstoul(m_SelCount, NULL, 10);

	// check Table Select
	if (m_bCheck_TabSelect)
	{
		// send Table
		// write tabl in Device
		BYTE v_TablName[1] = { uTABL_NAME };

		m_Grid.DeviceSlave_Write(v_TablName, uSLA_ADDR, uTABL_ADDR, 1);

		Sleep(30);
	}


	// Check for Password 
	if (this->m_bCheck_PasEnable)
	{
		unsigned char v_PassSymbols[4];
		// get Password Bytes
		CString strHex;

		for (unsigned char k = 0; k < 4; k++ )
		{
			char cPassLetter[2];
			cPassLetter[0] = m_sEdit_PasValue[k * 2];
			cPassLetter[1] = m_sEdit_PasValue[k * 2 + 1];

			strHex.AppendChar(cPassLetter[0]);
			strHex.AppendChar(cPassLetter[1]);

			// convert to Byte
			unsigned char byte_passLetter;
			byte_passLetter = (BYTE)_tcstoul(strHex, NULL, 16);

			v_PassSymbols[k] = byte_passLetter;

			strHex.Truncate(0);
		}

		// send password
		// write pass in Device
		m_Grid.DeviceSlave_Write(v_PassSymbols, uSLA_ADDR, uPAS_ADDR, 4);

	}

	// check values
	unsigned char check_result = 0;

	if (uSLA_ADDR > 0xF0)
	{
		check_result = 0x11;
	}

	BYTE ucAddrStart = 0;
	WORD uiAddrCount = 256;

	// check Address Select
	if (this->m_bCheck_SelRange)
	{
		// SafeCheck
		if (uSEL_ADDR < 0x100)
		{
			ucAddrStart = uSEL_ADDR;
		}

		if (uSEL_CNT <= 256)
		{
			if (uSEL_ADDR + uSEL_CNT > 0x100)
			{
				uiAddrCount = 0x100 - ucAddrStart;
			}
			else
			{
				uiAddrCount = uSEL_CNT;
			}
		}
	}

	// Read op
	Sleep(10);

	p_cPB_OP->SetPos(0);

	BYTE v_ReadBytes[256];

	// init with previous
//	m_Grid.GridSFF_Read(v_ReadBytes, 0, 256);

	// Read Selected
	m_Grid.DeviceSlave_Read(v_ReadBytes, uSLA_ADDR, ucAddrStart, uiAddrCount);

	p_cPB_OP->SetPos(100);

	// > Set output
	// clear Table
	m_Grid.ClearTable();

	// output Data
	m_Grid.GridSFF_Write(v_ReadBytes, ucAddrStart, uiAddrCount);

	UpdateData(FALSE);

}

// Write Table Procedure
void CDS4830A_SFF_Custom::OnBnClickedButton5()
{
	UpdateData(TRUE);

	// get options
	BYTE uSLA_ADDR = (BYTE)_tcstoul(m_SLA_ADDR, NULL, 16);
	BYTE uTABL_ADDR = (BYTE)_tcstoul(m_TABL_ADDR, NULL, 16);
	BYTE uTABL_NAME = (BYTE)_tcstoul(m_TABL_NAME, NULL, 16);

	BYTE uPAS_ADDR = (BYTE)_tcstoul(m_sEdit_PasAddr, NULL, 16);
	//BYTE uPAS_VALUE = (BYTE)_tcstoul(m_SLA_ADDR, NULL, 16);

	BYTE uSEL_ADDR = (BYTE)_tcstoul(m_SelAddr, NULL, 16);
	WORD uSEL_CNT = (WORD)_tcstoul(m_SelCount, NULL, 10);

	// check Table Select
	if (m_bCheck_TabSelect)
	{
		// send Table
		// write tabl in Device
		BYTE v_TablName[1] = { uTABL_NAME };

		m_Grid.DeviceSlave_Write(v_TablName, uSLA_ADDR, uTABL_ADDR, 1);

		Sleep(30);
	}

	// Check for Password 
	if (this->m_bCheck_PasEnable)
	{
		unsigned char v_PassSymbols[4];
		// get Password Bytes
		CString strHex;

		for (unsigned char k = 0; k < 4; k++)
		{
			char cPassLetter[2];
			cPassLetter[0] = m_sEdit_PasValue[k * 2];
			cPassLetter[1] = m_sEdit_PasValue[k * 2 + 1];

			strHex.AppendChar(cPassLetter[0]);
			strHex.AppendChar(cPassLetter[1]);

			// convert to Byte
			unsigned char byte_passLetter;
			byte_passLetter = (BYTE)_tcstoul(strHex, NULL, 16);

			v_PassSymbols[k] = byte_passLetter;

			strHex.Truncate(0);
		}

		// send password
		// write pass in Device
		m_Grid.DeviceSlave_Write(v_PassSymbols, uSLA_ADDR, uPAS_ADDR, 4);

	}

	// > Write
	unsigned char v_WrByte8[8];
	BYTE v_WriteBytes[256];

	BYTE ucAddrStart = 0;
	WORD uiAddrCount = 256;

	// check Select
	if (this->m_bCheck_SelRange)
	{
		// SafeCheck
		if (uSEL_ADDR < 0x100)
		{
			ucAddrStart = uSEL_ADDR;
		}

		if (uSEL_CNT <= 256)
		{
			if (uSEL_ADDR + uSEL_CNT > 0x100)
			{
				uiAddrCount = 0x100 - ucAddrStart;
			}
			else
			{
				uiAddrCount = uSEL_CNT;
			}
		}
	}

	p_cPB_OP->SetPos(40);

	// get Values
	m_Grid.GridSFF_Read(v_WriteBytes, ucAddrStart, uiAddrCount);

	BYTE byteBlock_cnt = uiAddrCount / 8;
	BYTE byteBlock_lst = uiAddrCount - byteBlock_cnt * 8;

	// Write to Device
	if (byteBlock_cnt > 0)
	{
		for (int k = 0; k < byteBlock_cnt; k++)
		{
			// prepare write buffer
			for (int k2 = 0; k2 < 8; k2++)
			{
				v_WrByte8[k2] = v_WriteBytes[k * 8 + k2];
			}

			// i2c write
			// NOTE: 8byte mode
			m_Grid.DeviceSlave_Write(v_WrByte8, uSLA_ADDR, ucAddrStart + k * 8, 8);

			// output progress
			p_cPB_OP->SetPos(40 + k * 7);

			Sleep(10);
		}
	}

	if (byteBlock_lst > 0)
	{
		// prepare write buffer
		for (int k2 = 0; k2 < byteBlock_lst; k2++)
		{
			v_WrByte8[k2] = v_WriteBytes[byteBlock_cnt * 8 + k2];
		}

		// i2c write
		// NOTE: 8byte mode
		m_Grid.DeviceSlave_Write(v_WrByte8, uSLA_ADDR, ucAddrStart + byteBlock_cnt * 8, byteBlock_lst);

	}

	p_cPB_OP->SetPos(100);

}





void CDS4830A_SFF_Custom::OnEnChangeEditSlaaddr()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	this->EditHexControl(&m_Edit_SlaveAddr);
}


void CDS4830A_SFF_Custom::OnEnChangeEditTabladdr()
{
	this->EditHexControl(&m_Edit_TableAddr);		
}


void CDS4830A_SFF_Custom::OnEnChangeEditTablname()
{
	this->EditHexControl(&m_Edit_TableName);
}


void CDS4830A_SFF_Custom::OnEnChangeEditPasaddr()
{
	this->EditHexControl(&m_Edit_PasAddr);
}


void CDS4830A_SFF_Custom::OnEnChangeEditPasvalue()
{
	this->EditHexControl(&m_Edit_PasValue);
}


void CDS4830A_SFF_Custom::OnOK()
{
	// TODO: Add your specialized code here and/or call the base class

	// CDialog::OnOK();
}
