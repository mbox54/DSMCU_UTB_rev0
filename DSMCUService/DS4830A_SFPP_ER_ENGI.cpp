// DS4830A_SFF_T10.cpp : implementation file
//

#include "stdafx.h"
#include "DSMCUService.h"
#include "DS4830A_SFPP_ER_ENGI.h"
#include "afxdialogex.h"

// ##########################################################################
// CDS4830A_SFF_T10 dialog
// ##########################################################################
IMPLEMENT_DYNAMIC(CDS4830A_SFPP_ER_ENGI, CDialog)

CDS4830A_SFPP_ER_ENGI::CDS4830A_SFPP_ER_ENGI(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_PROPPAGE_DS4830A_SFPP_ER_ENGI, pParent)
	, m_sEdit_PassValue(_T(""))
{

}

CDS4830A_SFPP_ER_ENGI::CDS4830A_SFPP_ER_ENGI(HID_SMBUS_DEVICE * pHidSmbus, CProgressCtrl * p_cPB_OP, CEdit * p_EDIT_STATUS, st_serviceData * p_service, CWnd * pParent)
	: CDialog(IDD_PROPPAGE_DS4830A_SFPP_ER_ENGI, pParent)
	, m_pHidSmbus(pHidSmbus)
	, p_EDIT_STATUS(p_EDIT_STATUS)
	, p_cPB_OP(p_cPB_OP)
	, p_service(p_service)
	, m_Grid(m_pHidSmbus, p_cPB_OP, p_EDIT_STATUS, p_service)
{

}

CDS4830A_SFPP_ER_ENGI::~CDS4830A_SFPP_ER_ENGI()
{

}

void CDS4830A_SFPP_ER_ENGI::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_GRID, m_Grid);
	DDX_Control(pDX, IDC_TRACE, m_TraceWnd);
	DDX_Control(pDX, IDC_SLIDER_BIAS, m_Slider_BIAS);
	DDX_Control(pDX, IDC_SLIDER_MOD, m_Slider_MOD);
	DDX_Control(pDX, IDC_EDIT_BIAS_H, m_Edit_Bias_H);
	DDX_Control(pDX, IDC_EDIT_MOD_H, m_Edit_Mod_H);
	DDX_Text(pDX, IDC_EDIT_PASSVALUE, m_sEdit_PassValue);
}

// -------------------------------------------------------------------
// MESSAGE_MAP
// -------------------------------------------------------------------
BEGIN_MESSAGE_MAP(CDS4830A_SFPP_ER_ENGI, CDialog)
	// Grid event functions
	ON_NOTIFY(NM_CLICK, IDC_GRID, OnGridClick)
	ON_NOTIFY(GVN_ENDLABELEDIT, IDC_GRID, OnGridEndEdit)

	ON_BN_CLICKED(IDC_BUTTON1, &CDS4830A_SFPP_ER_ENGI::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CDS4830A_SFPP_ER_ENGI::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CDS4830A_SFPP_ER_ENGI::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4, &CDS4830A_SFPP_ER_ENGI::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON5, &CDS4830A_SFPP_ER_ENGI::OnBnClickedButton5)
	ON_BN_CLICKED(IDC_BUTTON_CONF_READ, &CDS4830A_SFPP_ER_ENGI::OnBnClickedButtonConfRead)

	ON_WM_HSCROLL()
	ON_WM_VSCROLL()

	ON_BN_CLICKED(IDC_BUTTON_BIAS_SET, &CDS4830A_SFPP_ER_ENGI::OnBnClickedButtonBiasSet)
	ON_BN_CLICKED(IDC_BUTTON_MOD_SET2, &CDS4830A_SFPP_ER_ENGI::OnBnClickedButtonModSet2)
	ON_BN_CLICKED(IDC_BUTTON_CONF_WRITE, &CDS4830A_SFPP_ER_ENGI::OnBnClickedButtonConfWrite)
END_MESSAGE_MAP()


// ===================================================================
// Procedures
// ===================================================================
void CDS4830A_SFPP_ER_ENGI::SliderInit()
{
	// var for label OP
	CString str;

	// BIAS
	// slider
	m_Slider_BIAS.SetRange(VALUE_BIAS_MIN, VALUE_BIAS_MAX);
	m_Slider_BIAS.SetPos(VALUE_BIAS_DEF);

	// label
	// get static components
	CWnd *pStaticBIAS = this->GetDlgItem(IDC_STATIC_BIAS);
	CWnd *pStaticBIAS_MIN = this->GetDlgItem(IDC_STATIC_BIAS_MIN);
	CWnd *pStaticBIAS_MAX = this->GetDlgItem(IDC_STATIC_BIAS_MAX);

	// set Values
	str.Format(L"%d", VALUE_BIAS_MIN);			// Set & Convert
	pStaticBIAS_MIN->SetWindowTextW(str);		// Out to Interface control 

	str.Format(L"%d", VALUE_BIAS_MAX);			// Set & Convert
	pStaticBIAS_MAX->SetWindowTextW(str);		// Out to Interface control 

	str.Format(L"%d", VALUE_BIAS_DEF);			// Set & Convert
	pStaticBIAS->SetWindowTextW(str);			// Out to Interface control 

												// edit 
												// config
	this->m_Edit_Bias_H.LimitText(2);

	// fill with Value
	// // Get ByteH and ByteL from WORD
	unsigned short _word;
	unsigned char byte_H;
	unsigned char byte_L;

	byte_H = VALUE_BIAS_DEF;

	// // place Values
	CString str_hex;
	str_hex.Format(_T("%02X"), byte_H);
	this->m_Edit_Bias_H.SetWindowTextW(str_hex);

	str_hex.Truncate(0);

	// MOD
	// slider
	m_Slider_MOD.SetRange(VALUE_MOD_MIN, VALUE_MOD_MAX);
	m_Slider_MOD.SetPos(VALUE_MOD_DEF);

	// get static components
	CWnd *pStaticMOD = this->GetDlgItem(IDC_STATIC_MOD);
	CWnd *pStaticMOD_MIN = this->GetDlgItem(IDC_STATIC_MOD_MIN);
	CWnd *pStaticMOD_MAX = this->GetDlgItem(IDC_STATIC_MOD_MAX);

	// set Values
	str.Format(L"%d", VALUE_MOD_MIN);			// Set & Convert
	pStaticMOD_MIN->SetWindowTextW(str);		// Out to Interface control 

	str.Format(L"%d", VALUE_MOD_MAX);			// Set & Convert
	pStaticMOD_MAX->SetWindowTextW(str);		// Out to Interface control 

	str.Format(L"%d", VALUE_MOD_DEF);			// Set & Convert
	pStaticMOD->SetWindowTextW(str);			// Out to Interface control 

												// edit 
												// config
	this->m_Edit_Mod_H.LimitText(2);

	// fill with Value
	// // Get ByteH and ByteL from WORD
	//unsigned int _word;
	//unsigned char byte_H;
	//unsigned char byte_L;
	byte_H = VALUE_MOD_DEF;

	// // place Values
	//CString str_hex;
	str_hex.Format(_T("%02X"), byte_H);
	this->m_Edit_Mod_H.SetWindowTextW(str_hex);


	// > Update interface for Write controls
	UpdateData(FALSE);
}


// -------------------------------------------------------------------
// Device Operations
// -------------------------------------------------------------------
void CDS4830A_SFPP_ER_ENGI::ReadDevice()
{
	// set password
	// Valid ConfigTable Values
	unsigned char v_TablName[1] = { 0x10 };
//	unsigned char v_TablPass[4] = { 'O', 'P', 'W', 'Y' };
	unsigned char v_TablPass[4]; // = { 0x00, 0x11, 0x22, 0x33 };


								 // get Password 4Bytes
	UpdateData(TRUE);

	CString strHex;

	for (unsigned char k = 0; k < 4; k++)
	{
		char cPassLetter[2];
		cPassLetter[0] = m_sEdit_PassValue[k * 2];
		cPassLetter[1] = m_sEdit_PassValue[k * 2 + 1];

		strHex.AppendChar(cPassLetter[0]);
		strHex.AppendChar(cPassLetter[1]);

		// convert to Byte
		unsigned char byte_passLetter;
		byte_passLetter = (BYTE)_tcstoul(strHex, NULL, 16);

		v_TablPass[k] = byte_passLetter;

		strHex.Truncate(0);
	}

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
	for (unsigned char k = 0; k < 4; k++)
	{
		v_WrByte[0] = v_TablPass[k];
		m_Grid.DeviceSlave_Write(v_WrByte, SLAVEADDR_A2, 0x7B + k, 1);

		Sleep(10);

		p_cPB_OP->SetPos(30 + 5 * k);
	}


	// Read op
	Sleep(50);

	p_cPB_OP->SetPos(80);

	//m_Grid.DeviceSlave_ReadTimer(uValues2, 0, SLAVEADDR_A2, 0, 256, 0, 0);
	m_Grid.DeviceSlave_Read(uValues2, SLAVEADDR_A2, 0, 256);

	m_Grid.GridSFF_Write(uValues2, 0, 256);

	p_cPB_OP->SetPos(100);
}


void CDS4830A_SFPP_ER_ENGI::WriteDevice()
{
	// write op
	//m_Grid.DeviceSlave_WriteTimer(uValues, 1, SLAVEADDR_A2, 0, 256, 0x7F, 0x10);

	// Valid ConfigTable Values
	unsigned char v_TablName[1] = { 0x10 };
//	unsigned char v_TablPass[4] = { 'O', 'P', 'W', 'Y' };
	unsigned char v_TablPass[4]; // = { 0x00, 0x11, 0x22, 0x33 };


								 // get Password 4Bytes
	UpdateData(TRUE);

	CString strHex;

	for (unsigned char k = 0; k < 4; k++)
	{
		char cPassLetter[2];
		cPassLetter[0] = m_sEdit_PassValue[k * 2];
		cPassLetter[1] = m_sEdit_PassValue[k * 2 + 1];

		strHex.AppendChar(cPassLetter[0]);
		strHex.AppendChar(cPassLetter[1]);

		// convert to Byte
		unsigned char byte_passLetter;
		byte_passLetter = (BYTE)_tcstoul(strHex, NULL, 16);

		v_TablPass[k] = byte_passLetter;

		strHex.Truncate(0);
	}

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
	unsigned char v_GridVal_T10[128];
	unsigned char v_WrByte8[8];

	// get Values
	m_Grid.GridSFF_Read(v_GridVal_T10, 0x80, 128);

	// Write to Device
	for (int k = 0; k < 16; k++)
	{
		// prepare write buffer
		for (int k2 = 0; k2 < 8; k2++)
		{
			v_WrByte8[k2] = v_GridVal_T10[k * 8 + k2];
		}

		// i2c write
		// NOTE: 8byte mode
		m_Grid.DeviceSlave_Write(v_WrByte8, SLAVEADDR_A2, 0x80 + k * 8, 8);

		// output progress
		p_cPB_OP->SetPos(40 + k * 7);

		Sleep(10);
	}

	p_cPB_OP->SetPos(100);
	//	m_Grid.DeviceSlave_WriteTimer(uValues2, 0, SLAVEADDR_A2, 128 + 8, 8, 0, 0);
}


// -------------------------------------------------------------------
// Supporting procedures
// -------------------------------------------------------------------
void CDS4830A_SFPP_ER_ENGI::Trace(LPCTSTR szFmt, ...)
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

// -------------------------------------------------------------------
// Grid Events
// -------------------------------------------------------------------
void CDS4830A_SFPP_ER_ENGI::OnGridClick(NMHDR * pNotifyStruct, LRESULT * pResult)
{
	NM_GRIDVIEW* pItem = (NM_GRIDVIEW*)pNotifyStruct;
	Trace(_T("Clicked on row %d, col %d\n"), pItem->iRow, pItem->iColumn);
	Trace(_T("Text: %s\n"), m_Grid.GetItemText(pItem->iRow, pItem->iColumn));
	//Item.strText
}

void CDS4830A_SFPP_ER_ENGI::OnGridEndEdit(NMHDR * pNotifyStruct, LRESULT * pResult)
{
	NM_GRIDVIEW* pItem = (NM_GRIDVIEW*)pNotifyStruct;
	Trace(_T("End Edit on row %d, col %d\n"), pItem->iRow, pItem->iColumn);

	// > check Valid Input
	bool bInvalid = m_Grid.CheckValidHex(pItem->iRow, pItem->iColumn);

	*pResult = (bInvalid || m_Grid.m_bRejectEditChanges) ? -1 : 0;

	Trace(_T("Res: %d\n"), bInvalid && m_Grid.m_bRejectEditChanges);
}


// -------------------------------------------------------------------
// CDS4830A_SFF_T10 message handlers
// -------------------------------------------------------------------
BOOL CDS4830A_SFPP_ER_ENGI::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	m_Grid.Init();
	//m_Grid.SetTableColor();

	// > Sliders
	SliderInit();

	// > Password
	m_sEdit_PassValue = (CString)"00112233";

	// > Update interface for Write controls
	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}


// -------------------------------------------------------------------
// Buttons Test
// -------------------------------------------------------------------
void CDS4830A_SFPP_ER_ENGI::OnBnClickedButton1()
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


void CDS4830A_SFPP_ER_ENGI::OnBnClickedButton2()
{
	// TODO: Add your control notification handler code here

	m_Grid.GridSFF_Read(uValues2, 0, 256);
}


void CDS4830A_SFPP_ER_ENGI::OnBnClickedButton3()
{
	// TODO: Add your control notification handler code here
	m_Grid.GridSFF_Write(uValues2, 0, 256);
}


// -------------------------------------------------------------------
// Table op call events
// -------------------------------------------------------------------

// Read Table Procedure
void CDS4830A_SFPP_ER_ENGI::OnBnClickedButton4()
{
	this->ReadDevice();

}

// Write Table Procedure
void CDS4830A_SFPP_ER_ENGI::OnBnClickedButton5()
{
	this->WriteDevice();

}


void CDS4830A_SFPP_ER_ENGI::OnBnClickedButtonConfRead()
{
	// TODO: Add your control notification handler code here
}


void CDS4830A_SFPP_ER_ENGI::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: Add your message handler code here and/or call default
	CWnd *pSliderBIAS = this->GetDlgItem(IDC_SLIDER_BIAS);
	CWnd *pSliderMOD = this->GetDlgItem(IDC_SLIDER_MOD);

	if (pScrollBar == pSliderBIAS)			//  BIAS Slider
	{
		// get static components
		CWnd *pStaticBIAS = this->GetDlgItem(IDC_STATIC_BIAS);

		// > Update contents for Read controls
		UpdateData(TRUE);

		// > Control MAIN slider
		// Get current Slider position
		unsigned char ucPos = this->m_Slider_BIAS.GetPos();

		// convert to str
		CString str1;
		str1.Format(L"%d", ucPos);

		// Out to Interface control Label
		pStaticBIAS->SetWindowTextW(str1);

		// Out to Interface control Edits

		// // Convert to String Hex
		// // Out in Edits
		CString str_hex;
		str_hex.Format(_T("%02X"), ucPos);
		this->m_Edit_Bias_H.SetWindowTextW(str_hex);

		// > Update interface for Write controls
		UpdateData(FALSE);
	}

	if (pScrollBar == pSliderMOD)			//  MOD Slider
	{
		// get static components
		CWnd *pStaticMOD = this->GetDlgItem(IDC_STATIC_MOD);

		// > Update contents for Read controls
		UpdateData(TRUE);

		// > Control MAIN slider
		// Get current Slider position
		unsigned char ucPos = this->m_Slider_MOD.GetPos();

		// convert to str
		CString str1;
		str1.Format(L"%d", ucPos);

		// Out to Interface control Label
		pStaticMOD->SetWindowTextW(str1);

		// Out to Interface control Edits

		// // Convert to String Hex
		// // Out in Edits
		CString str_hex;
		str_hex.Format(_T("%02X"), ucPos);
		this->m_Edit_Mod_H.SetWindowTextW(str_hex);

		// > Update interface for Write controls
		UpdateData(FALSE);
	}


	if (nSBCode == TB_THUMBTRACK)
	{

	}

	// update output controls on Event
	if (nSBCode == SB_ENDSCROLL)
	{

		if (pScrollBar == pSliderBIAS)			//  BIAS Slider
		{
			this->OnBnClickedButtonBiasSet();

		}

		if (pScrollBar == pSliderMOD)			//  MOD Slider
		{
			this->OnBnClickedButtonModSet2();

		}

	}


	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}


void CDS4830A_SFPP_ER_ENGI::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: Add your message handler code here and/or call default

	CDialog::OnVScroll(nSBCode, nPos, pScrollBar);
}


void CDS4830A_SFPP_ER_ENGI::OnBnClickedButtonBiasSet()
{
	// > Get Value
	CString str;
	CString str_edit;

	// > proceed FIRST byte
	this->m_Edit_Bias_H.GetWindowTextW(str_edit);

	// > > Check null Value
	if (str_edit.GetLength() == 0)
	{
		// raise warning
		int msgboxID = MessageBox(
			(LPCWSTR)L"Операция не выполнена!\nОдно из полей ввода пусто.",
			(LPCWSTR)L"Set Value",
			MB_ICONWARNING | MB_OK | MB_DEFBUTTON2
		);

		// abort
		return;
	}

	// convert to Byte
	unsigned char byte_H;
	byte_H = (BYTE)_tcstoul(str_edit, NULL, 16);

	// > > align Hex String to Valid format
	if (str_edit.GetLength() == 1)
	{
		CString strNew;
		strNew.AppendChar('0');

		strNew.Append(str_edit);
	}

	// > > Add to string
	str.Append(str_edit);

	/*
	// > proceed SECOND byte
	this->m_Edit_Bias_L.GetWindowTextW(str_edit);

	// > > Check null Value
	if (str_edit.GetLength() == 0)
	{
		// raise warning
		int msgboxID = MessageBox(
			(LPCWSTR)L"Операция не выполнена!\nОдно из полей ввода пусто.",
			(LPCWSTR)L"Set Value",
			MB_ICONWARNING | MB_OK | MB_DEFBUTTON2
		);

		// abort
		return;
	}
	

	// convert to Byte
	unsigned char byte_L;
	byte_L = (BYTE)_tcstoul(str_edit, NULL, 16);

	// > > align Hex String to Valid format
	if (str_edit.GetLength() == 1)
	{
		CString strNew;
		strNew.AppendChar('0');

		strNew.Append(str_edit);
	}

	// > > Add to string
	str.Append(str_edit);

	// > Make INT from String HEX
	unsigned int _word;

	_word = (WORD)_tcstoul(str, NULL, 16);
	*/

	// Check Valid range
	if (byte_H < VALUE_BIAS_MIN)
	{
		// raise warning
		int msgboxID = MessageBox(
			(LPCWSTR)L"Операция не выполнена!\nУстановленное значение выходит за рамки допустимого диапазона.",
			(LPCWSTR)L"Set Value",
			MB_ICONWARNING | MB_OK | MB_DEFBUTTON2
		);

		// abort
		return;
	}

	if (byte_H > VALUE_BIAS_MAX)
	{
		// raise warning
		int msgboxID = MessageBox(
			(LPCWSTR)L"Операция не выполнена!\nУстановленное значение выходит за рамки допустимого диапазона.",
			(LPCWSTR)L"Set Value",
			MB_ICONWARNING | MB_OK | MB_DEFBUTTON2
		);

		// abort
		return;
	}

	// > Write Value in Grid
	// get Grid Values
	unsigned char uValue[256];

	this->m_Grid.GridSFF_Read(uValue, 0, 256);

	// put Value
	uValue[0x80 + ADDR_BIAS] = byte_H;

	// set Grid Values
	this->m_Grid.GridSFF_Write(uValue, 0, 256);

}


void CDS4830A_SFPP_ER_ENGI::OnBnClickedButtonModSet2()
{
	// > Get Value
	CString str;
	CString str_edit;

	// > proceed FIRST byte
	this->m_Edit_Mod_H.GetWindowTextW(str_edit);

	// > > Check null Value
	if (str_edit.GetLength() == 0)
	{
		// raise warning
		int msgboxID = MessageBox(
			(LPCWSTR)L"Операция не выполнена!\nОдно из полей ввода пусто.",
			(LPCWSTR)L"Set Value",
			MB_ICONWARNING | MB_OK | MB_DEFBUTTON2
		);

		// abort
		return;
	}

	// convert to Byte
	unsigned char byte_H;
	byte_H = (BYTE)_tcstoul(str_edit, NULL, 16);

	// > > align Hex String to Valid format
	if (str_edit.GetLength() == 1)
	{
		CString strNew;
		strNew.AppendChar('0');

		strNew.Append(str_edit);
	}

	// > > Add to string
	str.Append(str_edit);

	/*
	// > proceed SECOND byte
	this->m_Edit_Bias_L.GetWindowTextW(str_edit);

	// > > Check null Value
	if (str_edit.GetLength() == 0)
	{
	// raise warning
	int msgboxID = MessageBox(
	(LPCWSTR)L"Операция не выполнена!\nОдно из полей ввода пусто.",
	(LPCWSTR)L"Set Value",
	MB_ICONWARNING | MB_OK | MB_DEFBUTTON2
	);

	// abort
	return;
	}


	// convert to Byte
	unsigned char byte_L;
	byte_L = (BYTE)_tcstoul(str_edit, NULL, 16);

	// > > align Hex String to Valid format
	if (str_edit.GetLength() == 1)
	{
	CString strNew;
	strNew.AppendChar('0');

	strNew.Append(str_edit);
	}

	// > > Add to string
	str.Append(str_edit);

	// > Make INT from String HEX
	unsigned int _word;

	_word = (WORD)_tcstoul(str, NULL, 16);
	*/

	// Check Valid range
	if (byte_H < VALUE_MOD_MIN)
	{
		// raise warning
		int msgboxID = MessageBox(
			(LPCWSTR)L"Операция не выполнена!\nУстановленное значение выходит за рамки допустимого диапазона.",
			(LPCWSTR)L"Set Value",
			MB_ICONWARNING | MB_OK | MB_DEFBUTTON2
		);

		// abort
		return;
	}

	if (byte_H > VALUE_MOD_MAX)
	{
		// raise warning
		int msgboxID = MessageBox(
			(LPCWSTR)L"Операция не выполнена!\nУстановленное значение выходит за рамки допустимого диапазона.",
			(LPCWSTR)L"Set Value",
			MB_ICONWARNING | MB_OK | MB_DEFBUTTON2
		);

		// abort
		return;
	}

	// > Write Value in Grid
	// get Grid Values
	unsigned char uValue[256];

	this->m_Grid.GridSFF_Read(uValue, 0, 256);

	// put Value
	uValue[0x80 + ADDR_MOD] = byte_H;

	// set Grid Values
	this->m_Grid.GridSFF_Write(uValue, 0, 256);

}


void CDS4830A_SFPP_ER_ENGI::OnBnClickedButtonConfWrite()
{
	// TODO: Add your control notification handler code here
}


void CDS4830A_SFPP_ER_ENGI::OnOK()
{
	// TODO: Add your specialized code here and/or call the base class

	// CDialog::OnOK();
}
