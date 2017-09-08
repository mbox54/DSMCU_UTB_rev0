// DS4830A_SFPP_CONF.cpp : implementation file
//

#include "stdafx.h"
#include "DSMCUService.h"
#include "DS4830A_SFPP_LR_CONF_OPER.h"
#include "afxdialogex.h"


// CDS4830A_SFPP_CONF dialog

IMPLEMENT_DYNAMIC(CDS4830A_SFPP_CONF, CDialogEx)


void CDS4830A_SFPP_CONF::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_GRID, m_Grid);
	DDX_Text(pDX, IDC_EDIT_FILENAME, m_Edit_Filename);
	DDX_Control(pDX, IDC_SLIDER_BIAS, m_Slider_BIAS);
	DDX_Control(pDX, IDC_SLIDER_MOD, m_Slider_MOD);
	DDX_Control(pDX, IDC_EDIT_BIAS_H, m_Edit_Bias_H);
	DDX_Control(pDX, IDC_EDIT_BIAS_L, m_Edit_Bias_L);
	DDX_Control(pDX, IDC_EDIT_MOD_H, m_Edit_Mod_H);
	DDX_Control(pDX, IDC_EDIT_MOD_L, m_Edit_Mod_L);
	DDX_Control(pDX, IDC_SLIDER_HORISON, m_Slider_HORISON);
	DDX_Control(pDX, IDC_EDIT_HOR, m_Edit_Horison);
	DDX_Control(pDX, IDC_SLIDER_VERTICAL, m_Slider_VERTICAL);
	DDX_Control(pDX, IDC_EDIT_VERTICAL, m_Edit_Vertical);
	DDX_Check(pDX, IDC_CHECK_VERTICAL, m_bCheck_Vertical);
	DDX_Check(pDX, IDC_CHECK_BIAS_MOD_DEPENDANCE, m_bCheck_BIAS_MOD_Dependancy);
	DDX_Check(pDX, IDC_CHECK_SAFERANGE, m_bCheck_SafeRange);
	DDX_Text(pDX, IDC_EDIT_PASSVALUE, m_sEdit_PassValue);
}


CDS4830A_SFPP_CONF::CDS4830A_SFPP_CONF(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_PROPPAGE_DS4830A_SFPP_LR_CONF_OPER, pParent)
	, m_Edit_Filename(_T(""))
	, m_bCheck_Vertical(FALSE)
	, m_bCheck_BIAS_MOD_Dependancy(FALSE)
	, m_bCheck_SafeRange(FALSE)
	, m_sEdit_PassValue(_T(""))
{

}

CDS4830A_SFPP_CONF::CDS4830A_SFPP_CONF(HID_SMBUS_DEVICE * pHidSmbus, CProgressCtrl * p_cPB_OP, CEdit * p_EDIT_STATUS, st_serviceData * p_service, CWnd * pParent)
	: CDialogEx(IDD_PROPPAGE_DS4830A_SFPP_LR_CONF_OPER, pParent)
	, m_pHidSmbus(pHidSmbus)
	, p_EDIT_STATUS(p_EDIT_STATUS)
	, p_cPB_OP(p_cPB_OP)
	, p_service(p_service)
	, m_Grid(m_pHidSmbus, p_cPB_OP, p_EDIT_STATUS, p_service)
{

}

CDS4830A_SFPP_CONF::~CDS4830A_SFPP_CONF()
{
}

void CDS4830A_SFPP_CONF::Trace(LPCTSTR szFmt, ...)
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

void CDS4830A_SFPP_CONF::SliderInit()
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
	this->m_Edit_Bias_L.LimitText(2);

	// fill with Value
	// // Get ByteH and ByteL from WORD
	unsigned short _word;
	unsigned char byte_H;
	unsigned char byte_L;

	// // // ByteH
	_word = VALUE_BIAS_DEF;
	_word &= 0xFF00;
	_word = _word >> 8;
	byte_H = (BYTE)_word;

	// // // ByteL
	_word = VALUE_BIAS_DEF;
	_word &= 0x00FF;
	byte_L = (BYTE)_word;

	// // place Values
	CString str_hex;
	str_hex.Format(_T("%02X"), byte_H);
	this->m_Edit_Bias_H.SetWindowTextW(str_hex);

	str_hex.Truncate(0);

	str_hex.Format(_T("%02X"), byte_L);
	this->m_Edit_Bias_L.SetWindowTextW(str_hex);
	

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
	this->m_Edit_Mod_L.LimitText(2);

	// fill with Value
	// // Get ByteH and ByteL from WORD
	//unsigned int _word;
	//unsigned char byte_H;
	//unsigned char byte_L;

	// // // ByteH
	_word = VALUE_MOD_DEF;
	_word &= 0xFF00;
	_word = _word >> 8;
	byte_H = (BYTE)_word;

	// // // ByteL
	_word = VALUE_MOD_DEF;
	_word &= 0x00FF;
	byte_L = (BYTE)_word;

	// // place Values
	//CString str_hex;
	str_hex.Format(_T("%02X"), byte_H);
	this->m_Edit_Mod_H.SetWindowTextW(str_hex);

	str_hex.Truncate(0);

	str_hex.Format(_T("%02X"), byte_L);
	this->m_Edit_Mod_L.SetWindowTextW(str_hex);

	// checkbox
	this->m_bCheck_BIAS_MOD_Dependancy = TRUE;
	UpdateData(FALSE);
	

	// HORISONTAL
	// slider
	m_Slider_HORISON.SetRange(VALUE_HORISON_MIN, VALUE_HORISON_MAX);
	m_Slider_HORISON.SetPos(VALUE_HORISON_DEF);

	// get static components
	CWnd *pStaticHOR = this->GetDlgItem(IDC_STATIC_HOR);
	CWnd *pStaticHOR_MIN = this->GetDlgItem(IDC_STATIC_HOR_MIN2);
	CWnd *pStaticHOR_MAX = this->GetDlgItem(IDC_STATIC_HOR_MAX);

	// set Values
	str.Format(L"%d", VALUE_HORISON_MIN);		// Set & Convert
	pStaticHOR_MIN->SetWindowTextW(str);		// Out to Interface control 

	str.Format(L"%d", VALUE_HORISON_MAX);		// Set & Convert
	pStaticHOR_MAX->SetWindowTextW(str);		// Out to Interface control 

	str.Format(L"%d", VALUE_HORISON_DEF);		// Set & Convert
	pStaticHOR->SetWindowTextW(str);			// Out to Interface control 

	// edit 
	// config
	this->m_Edit_Horison.LimitText(2);

	// Out to Interface control Edits
	// Format Value
	// FORMAT:
	// 1Byte = bits:: data:7654321, direction:0

	// define direction
	// define Value
	char direction;
	unsigned char iPosFormat;

	char iPos = VALUE_HORISON_DEF;

	if (iPos < 0)
	{
		direction = 1;
		iPosFormat = iPos * (-1);
	}
	else
	{
		direction = 0;
		iPosFormat = iPos;
	}

	iPosFormat = iPosFormat << 1;
	iPosFormat &= 0xFE;
	iPosFormat += direction;

	// Out to Interface control Edits
	// // Convert to String Hex
	// // Out in Edits
	//CString str_hex;
	str_hex.Format(_T("%02X"), iPosFormat);
	this->m_Edit_Horison.SetWindowTextW(str_hex);

	// ! bug correct: init with bad position
	m_Slider_HORISON.SetPos(VALUE_HORISON_DEF -20);

	// VERTICAL
	// slider
	m_Slider_VERTICAL.SetRange(VALUE_VERTICAL_MIN, VALUE_VERTICAL_MAX);
	m_Slider_VERTICAL.SetPos(VALUE_VERTICAL_DEF);

	// get static components
	CWnd *pStaticVERT = this->GetDlgItem(IDC_STATIC_VERT);
	CWnd *pStaticVERT_MIN = this->GetDlgItem(IDC_STATIC_VERT_MIN);
	CWnd *pStaticVERT_MAX = this->GetDlgItem(IDC_STATIC_VERT_MAX);

	// set Values
	str.Format(L"%d", VALUE_VERTICAL_MIN);		// Set & Convert
	pStaticVERT_MIN->SetWindowTextW(str);		// Out to Interface control 

	str.Format(L"%d", VALUE_VERTICAL_MAX);		// Set & Convert
	pStaticVERT_MAX->SetWindowTextW(str);		// Out to Interface control 

	str.Format(L"%d", VALUE_VERTICAL_DEF);		// Set & Convert
	pStaticVERT->SetWindowTextW(str);			// Out to Interface control 

	// checkbox
	this->m_bCheck_Vertical = TRUE;
	this->m_bCheck_SafeRange = TRUE;
	UpdateData(FALSE);

	// edit 
	// config
	this->m_Edit_Vertical.LimitText(2);

	// Out to Interface control Edits
	// Format Value
	// FORMAT:
	// 1Byte = bits:: data:7654321, active:0

	// define Status
	unsigned char iStatus;

	UpdateData(TRUE);
	if (m_bCheck_Vertical)
	{
		iStatus = 1;
	}
	else
	{
		iStatus = 0;
	}

	// define Value
	//unsigned char iPosFormat;

	iPosFormat = iPos;
	iPosFormat = iPosFormat << 1;
	iPosFormat &= 0xFE;
	iPosFormat += iStatus;

	// Out to Interface control Edits
	// // Convert to String Hex
	// // Out in Edits
	//CString str_hex;
	str_hex.Format(_T("%02X"), iPosFormat);
	this->m_Edit_Vertical.SetWindowTextW(str_hex);

	// ! bug correct: init with bad position
	m_Slider_HORISON.SetPos(VALUE_VERTICAL_DEF + 1);
	m_Slider_HORISON.SetPos(VALUE_HORISON_DEF + 1);

	// > Update interface for Write controls
	UpdateData(FALSE);
}

void CDS4830A_SFPP_CONF::EditHexControl(CEdit * pEdit)
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

int CDS4830A_SFPP_CONF::TableValues_LoadFromFile(BYTE * v_ByteData)
{
	// CONST
	const unsigned char uLineCount = 16;
	const unsigned char uByteCount = 16;
	const unsigned char uLetterCount = 2;

	// err status
	unsigned char errLine;

	// position
	unsigned char l;				// active Line
	unsigned char k;				// active Byte
	unsigned char i;				// active Letter (Hex)

									// str Hex buffer
	unsigned char chex_buf[2];

	FILE *fp;


	// [#] FILE OP CHECK
	// > Define Filename
	CString str_filename("test.conf");
	char *file_name = "test.conf";

	// > Open File try
	errno_t err;

	char ch_fname[200];
	for (int k = 0; k < str_filename.GetLength(); k++)
	{
		ch_fname[k] = str_filename[k];
	}

	ch_fname[str_filename.GetLength()] = '\0';

	err = fopen_s(&fp, ch_fname, "r");

	if (err != 0)
	{	// "can't open file" 
		errLine = -1;
		return OP_FAILED;
	}

	// [STATUS OUTPUT]
	Trace(_T("File %s  \n"), str_filename);

	// > Define parameters for probress bar
	// count number of Intel Hex file lines
	unsigned int fileLineNum = 0;
	char *e_str;
	char IHline[256];
	while (TRUE)
	{
		e_str = fgets(IHline, sizeof(IHline), fp);
		fileLineNum++;

		// exit condition
		if (e_str == NULL)
		{
			// check line read condition
			if (feof(fp) != 0)
			{	// end of file

				break;
			}
			else
			{	// file corrupted
				errLine = -2;
				//		return MAXQBL_OP_FAILED;
				break;
			}
		}
	}

	// reopen FILE
	fclose(fp);

	err = fopen_s(&fp, file_name, "r");

	if (err != 0)
	{	// "can't open file" 
		errLine = -1;
		
		return OP_FAILED;
	}

	// [STATUS OUTPUT]
	Trace(_T("- lines: %03d \n"), fileLineNum);


	// > Read Data

	// init cycle Read var
	l = 0;
	k = 0;
	i = 0;
	chex_buf[0] = '0';
	chex_buf[1] = '0';


	// Read OP

	// NOTE:
	// FORMAT of file: 
	// H0 H1 H2 H3 H4 H5 H6 H7 H8 H9 HA HB HC HD HE HF // (Line 0x00)
	// ...
	// H0 H1 H2 H3 H4 H5 H6 H7 H8 H9 HA HB HC HD HE HF // (Line 0x0F)
	// where HN - Hex String symbols

	bool ACT = 1;						// reading file op is active
	while (ACT)
	{	// proceed symbol routine

		// > Get symbol
		int c = fgetc(fp);

		// > Select type
		switch (c)
		{
		case EOF:						// [#] cancel on End of File
			ACT = FALSE;
			break;

		case '\n':						// [#] prepare to New Line

			l++;						// line number
										 
			if (l > uLineCount - 1)		// check End of Reading OP
			{
				ACT = FALSE;
			}
			else
			{
				// reset byte number in Line
				k = 0;
			}
			break;

		case ' ':						// [#] prepare to New Byte

			k++;						// Byte number
			
			if (k > uByteCount - 1)		// check End of Line
			{
				// NOP
				// !probably set error here
			}

			break;


		default:						// [#] all Common text letter symbols
										// [#] Parce Intel Hex line
										// read letter
			chex_buf[i] = c;

			if (i == 0)
			{
				// FIRST letter
				i++;
			}
			else
			{
				// SECOND letter

				// > check valid Symbols
				// make hex string
				CString fileText;
				fileText.AppendChar(chex_buf[0]);
				fileText.AppendChar(chex_buf[1]);

				// valid symbol checking
				CString hexTextValid = fileText.SpanIncluding(_T("0123456789abcdefABCDEF"));

				if (hexTextValid.Compare(fileText) == 0)
				{
					// convert to Byte
					// place to uValues[] Table
					v_ByteData[l * uByteCount + k] = (BYTE)_tcstoul(hexTextValid, NULL, 16);

					// reset letter number
					i = 0;
				}
				else
				{
					// set err number equal to Line number
					errLine = l;

					return OP_FAILED;
				} // if compare
			}

			break;

		} // switch c
	} // while ACT


	// > Close Intel Hex File
	fclose(fp);


	return OP_SUCCESS;
}

int CDS4830A_SFPP_CONF::TableValues_SaveToFile(BYTE * v_ByteData)
{
	// CONST
	// # define COUNT of Byte in Line
	const unsigned char LINE_BYTE_COUNT = 16;	

	const unsigned char uLineCount = 16;
	const unsigned char uByteCount = 16;
	const unsigned char uLetterCount = 2;

	// err status
	unsigned char errLine;

	// position
	unsigned char l;				// active Line
	unsigned char k;				// active Byte
	unsigned char i;				// active Letter (Hex)

									// str Hex buffer
	unsigned char chex_buf[2];

	FILE *fp;


	// [#] FILE OP CHECK
	// > Define Filename
	CString str_filename("test.conf");
	char *file_name = "test.conf";

	// > Open File try
	errno_t err;

	char ch_fname[200];
	for (int k = 0; k < str_filename.GetLength(); k++)
	{
		ch_fname[k] = str_filename[k];
	}

	ch_fname[str_filename.GetLength()] = '\0';

	err = fopen_s(&fp, ch_fname, "w");

	if (err != 0)
	{	// "can't open file" 
		errLine = -1;
		return OP_FAILED;
	}

	// [STATUS OUTPUT]
	Trace(_T("File %s  \n"), str_filename);


	// > Write Data

	// NOTE:
	// FORMAT of file: 
	// H0 H1 H2 H3 H4 H5 H6 H7 H8 H9 HA HB HC HD HE HF // (Line 0x00)
	// ...
	// H0 H1 H2 H3 H4 H5 H6 H7 H8 H9 HA HB HC HD HE HF // (Line 0x0F)
	// where HN - Hex String symbols

	// init cycle Write var
	l = 0;
	k = 0;

	//
	unsigned char cbyte_line[LINE_BYTE_COUNT];

	bool ACT = 1;							// writing file op is active
	while (ACT)
	{
		// fill the Line
		cbyte_line[k] = v_ByteData[l * uByteCount + k];

		if (k > uByteCount - 2)				// check End of Line
		{
			// > forming Line

			// make string of hex
			CString fileHexSymbol;
			CString fileHexLine;

			for (unsigned char ik = 0; ik < uByteCount; ik++)
			{
				fileHexSymbol.Format(_T("%02X"), cbyte_line[ik]);

				fileHexLine.Append(fileHexSymbol);
				fileHexLine.AppendChar(' ');
			}
			
			// make null_string Line_of_char to write in file
			// NOTE:
			// FORMAT: 'H' 'H' ' ', ... , 'H' 'H' \n \0
			char chex_line[uByteCount * 3 + 1];			
			
			for (unsigned char ik = 0; ik < uByteCount * 3 - 1; ik++)
			{
				chex_line[ik] = fileHexLine[ik];
			}

			// add line ender
			chex_line[uByteCount * 3 - 1] = '\n';
			chex_line[uByteCount * 3] = '\0';

			// write string to file
			fputs(chex_line, fp);


			if (l > uLineCount - 2)			// check End of Reading OP
			{
				ACT = FALSE;
			}
			else
			{
				// prepare new Line
				l++;

				// reset Byte number in Line
				k = 0;
			}
		} 
		else
		{
			k++;
		}
	} // 


	// > Close Intel Hex File
	fclose(fp);


	return OP_SUCCESS;
}


void CDS4830A_SFPP_CONF::OnGridClick(NMHDR * pNotifyStruct, LRESULT * pResult)
{
	NM_GRIDVIEW* pItem = (NM_GRIDVIEW*)pNotifyStruct;
	Trace(_T("Clicked on row %d, col %d\n"), pItem->iRow, pItem->iColumn);
	Trace(_T("Text: %s\n"), m_Grid.GetItemText(pItem->iRow, pItem->iColumn));
	//Item.strText

}

void CDS4830A_SFPP_CONF::OnGridEndEdit(NMHDR * pNotifyStruct, LRESULT * pResult)
{
	NM_GRIDVIEW* pItem = (NM_GRIDVIEW*)pNotifyStruct;
	Trace(_T("End Edit on row %d, col %d\n"), pItem->iRow, pItem->iColumn);

	// > check Valid Input
	bool bInvalid = m_Grid.CheckValidHex(pItem->iRow, pItem->iColumn);

	*pResult = (bInvalid || m_Grid.m_bRejectEditChanges) ? -1 : 0;

	Trace(_T("Res: %d\n"), bInvalid && m_Grid.m_bRejectEditChanges);
}

void CDS4830A_SFPP_CONF::OnSliderBIASMove(NMHDR * pNotifyStruct, LRESULT * pResult)
{
	//this->m_Slider_BIAS.GetPos()
	//	Trace(_T("Text: %s\n",  ) );

	Trace(_T("Text: \n"));
}


BEGIN_MESSAGE_MAP(CDS4830A_SFPP_CONF, CDialogEx)
	// Grid event functions
	ON_NOTIFY(NM_CLICK, IDC_GRID, OnGridClick)
//	ON_NOTIFY(GVN_ENDLABELEDIT, IDC_GRID, OnGridEndEdit)

	// Slider event functions
	ON_WM_HSCROLL()
	// ON_WM_VSCROLL() // not yet



	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_BIAS, &CDS4830A_SFPP_CONF::OnNMCustomdrawSliderBias)

	ON_BN_CLICKED(IDC_BUTTON4, &CDS4830A_SFPP_CONF::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON5, &CDS4830A_SFPP_CONF::OnBnClickedButton5)
	ON_BN_CLICKED(IDC_BUTTON_CONF_READ, &CDS4830A_SFPP_CONF::OnBnClickedButtonConfRead)
	ON_BN_CLICKED(IDC_BUTTON_CONF_WRITE, &CDS4830A_SFPP_CONF::OnBnClickedButtonConfWrite)
	ON_BN_CLICKED(IDC_BUTTON1, &CDS4830A_SFPP_CONF::OnBnClickedButton1)

	ON_WM_HSCROLL()
	ON_WM_VSCROLL()

	ON_BN_CLICKED(IDC_BUTTON_BIAS_SET, &CDS4830A_SFPP_CONF::OnBnClickedButtonBiasSet)

	ON_EN_CHANGE(IDC_EDIT_BIAS_H, &CDS4830A_SFPP_CONF::OnEnChangeEditBiasH)

	ON_EN_CHANGE(IDC_EDIT_BIAS_L, &CDS4830A_SFPP_CONF::OnEnChangeEditBiasL)
	ON_BN_CLICKED(IDC_BUTTON_MOD_SET, &CDS4830A_SFPP_CONF::OnBnClickedButtonModSet)
	ON_BN_CLICKED(IDC_BUTTON_HOR_SET, &CDS4830A_SFPP_CONF::OnBnClickedButtonHorSet)
	ON_BN_CLICKED(IDC_CHECK_VERTICAL, &CDS4830A_SFPP_CONF::OnBnClickedCheckVertical)
	ON_BN_CLICKED(IDC_BUTTON_VERTICAL_SET, &CDS4830A_SFPP_CONF::OnBnClickedButtonVerticalSet)
	ON_BN_CLICKED(IDC_CHECK_SAFERANGE, &CDS4830A_SFPP_CONF::OnBnClickedCheckSaferange)
	ON_BN_CLICKED(IDC_CHECK_BIAS_MOD_DEPENDANCE, &CDS4830A_SFPP_CONF::OnBnClickedCheckBiasModDependance)
END_MESSAGE_MAP()


// CDS4830A_SFPP_CONF message handlers

// -------------------------------------------------------------------
// Table op call events
// -------------------------------------------------------------------

// Read Table Procedure
void CDS4830A_SFPP_CONF::OnBnClickedButton4()
{
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

// Write Table Procedure
void CDS4830A_SFPP_CONF::OnBnClickedButton5()
{
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
			v_WrByte8[k2] = v_GridVal_T10[k*8 + k2];
		}

		// i2c write
		m_Grid.DeviceSlave_Write(v_WrByte8, SLAVEADDR_A2, 0x80 + k * 8, 8);

		p_cPB_OP->SetPos(40 + k*7);

		Sleep(10);
	}

	p_cPB_OP->SetPos(100);
//	m_Grid.DeviceSlave_WriteTimer(uValues2, 0, SLAVEADDR_A2, 128 + 8, 8, 0, 0);
}


BOOL CDS4830A_SFPP_CONF::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here
	m_Grid.Init();

	// > Sliders
	SliderInit();

	// > Password
	m_sEdit_PassValue = (CString)"00112233";

	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}


void CDS4830A_SFPP_CONF::OnBnClickedButtonConfRead()
{
	// loading Values
	this->TableValues_LoadFromFile(this->uValues2);

	m_Grid.GridSFF_Write(uValues2, 0, 256);

	// place in Grid

}


void CDS4830A_SFPP_CONF::OnBnClickedButtonConfWrite()
{
	// TODO: Add your control notification handler code here
	m_Grid.GridSFF_Read(uValues2, 0, 256);

	this->TableValues_SaveToFile(uValues2);
}


void CDS4830A_SFPP_CONF::OnBnClickedButton1()
{
	// Valid ConfigTable Values
	unsigned char v_TablName[1] = { 0x10 };
	unsigned char v_TablPass[4] = { 'O', 'P', 'W', 'Y' };

	unsigned char v_WrByte[1];

	// send Table
	// write tabl in Device
	m_Grid.DeviceSlave_Write(v_TablName, SLAVEADDR_A2, 0x7F, 1);
	Sleep(60);

	// send password
	// write pass in Device
	for (unsigned char k = 0; k < 4; k++)
	{
		v_WrByte[0] = v_TablPass[k];
		m_Grid.DeviceSlave_Write(v_WrByte, SLAVEADDR_A2, 0x7B + k, 1);

		Sleep(10);
	}
	

/*	// *** CHECK BEFORE SEND *** 
	unsigned char v_TablSymbol[1];
	unsigned char v_PassSymbols[4];

	// check Table Value
	// get Table Value
	m_Grid.DeviceSlave_Read(v_TablSymbol, SLAVEADDR_A2, 0x7F, 1);

	if (v_TablSymbol[0] != v_TablName[0])
	{
		// send Table
		// write tabl in Device
		m_Grid.DeviceSlave_Write(v_TablName, SLAVEADDR_A2, 0x7F, 1);

		Sleep(100);
	}

	// check Password Value
	// get Table Value
	m_Grid.DeviceSlave_Read(v_PassSymbols, SLAVEADDR_A2, 0x7B, 4);

	bool act = 0;
	// check valid
	for (unsigned char k = 0; k < 4; k++)
	{
		if (v_PassSymbols[k] != v_TablPass[k])
		{
			act = 1;

			break;
		}
	}

	if (act)
	{
		// send password
		// write pass in Device
		m_Grid.DeviceSlave_Write(v_TablPass, SLAVEADDR_A2, 0x7B, 4);

		Sleep(100);
	}

*/
	// Read op
	Sleep(100);
	m_Grid.DeviceSlave_ReadTimer(uValues2, 0, SLAVEADDR_A2, 0, 256, 0, 0);
}

// Slider Event Functions
void CDS4830A_SFPP_CONF::OnNMCustomdrawSliderBias(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	
	// TODO: Add your control notification handler code here

	*pResult = 0;
}


void CDS4830A_SFPP_CONF::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: Add your message handler code here and/or call default
	CWnd *pSliderBIAS = this->GetDlgItem(IDC_SLIDER_BIAS);
	CWnd *pSliderMOD = this->GetDlgItem(IDC_SLIDER_MOD);
	CWnd *pSliderHOR = this->GetDlgItem(IDC_SLIDER_HORISON);
	CWnd *pSliderVERT = this->GetDlgItem(IDC_SLIDER_VERTICAL);


	if (pScrollBar == pSliderBIAS)			//  BIAS Slider
	{
		// get static components
		CWnd *pStaticBIAS = this->GetDlgItem(IDC_STATIC_BIAS);
		CWnd *pStaticMOD = this->GetDlgItem(IDC_STATIC_MOD);

		// > Update contents for Read controls
		UpdateData(TRUE);

		// > Control MAIN slider
		// Get current Slider position
		int iPos1 = this->m_Slider_BIAS.GetPos();

		// convert to str
		CString str1;
		str1.Format(L"%d", iPos1);

		// Out to Interface control Label
		pStaticBIAS->SetWindowTextW(str1);

		// Out to Interface control Edits
		// // Get ByteH and ByteL from WORD
		unsigned short _word;
		unsigned char byte_H;
		unsigned char byte_L;

		// // // ByteH
		_word = iPos1;
		_word &= 0xFF00;
		_word = _word >> 8;
		byte_H = (BYTE)_word;

		// // // ByteL
		_word = iPos1;
		_word &= 0x00FF;
		byte_L = (BYTE)_word;

		// // Convert to String Hex
		// // Out in Edits
		CString str_hex;
		str_hex.Format(_T("%02X"), byte_H);
		this->m_Edit_Bias_H.SetWindowTextW(str_hex);

		str_hex.Truncate(0);

		str_hex.Format(_T("%02X"), byte_L);
		this->m_Edit_Bias_L.SetWindowTextW(str_hex);

		// > Control SECONDARY slider
		if (this->m_bCheck_BIAS_MOD_Dependancy)
		{
			// check limit event
			int iMin = this->m_Slider_MOD.GetRangeMin();
			int iMax = this->m_Slider_MOD.GetRangeMax();

			// set SECONDARY Slider position
			UpdateData(TRUE);

			// Define Valid Range, Pos variation
			int iPos2 = 0;

			if (iPos1 < iMin)
			{
				iPos2 = iMin;
			}
			else
			{
				if (iPos1 * 1.5 > iMax)
				{
					iPos2 = iMax;
				}
				else
				{
					iPos2 = iPos1 * 1.5;
				}
			}

			// Set Controls
			m_Slider_MOD.SetPos(iPos2);

			// convert to str
			CString str2;
			str2.Format(L"%d", iPos2);

			// Out to Interface control 
			pStaticMOD->SetWindowTextW(str2);

			// Out to Interface control Edits
			// // Get ByteH and ByteL from WORD
			//unsigned int _word;
			//unsigned char byte_H;
			//unsigned char byte_L;

			// // // ByteH
			_word = iPos2;
			_word &= 0xFF00;
			_word = _word >> 8;
			byte_H = (BYTE)_word;

			// // // ByteL
			_word = iPos2;
			_word &= 0x00FF;
			byte_L = (BYTE)_word;

			// // Convert to String Hex
			// // Out in Edits
			//CString str_hex;
			str_hex.Truncate(0);

			str_hex.Format(_T("%02X"), byte_H);
			this->m_Edit_Mod_H.SetWindowTextW(str_hex);

			str_hex.Truncate(0);

			str_hex.Format(_T("%02X"), byte_L);
			this->m_Edit_Mod_L.SetWindowTextW(str_hex);
		}


		// > Update interface for Write controls
		UpdateData(FALSE);
	}

	if (pScrollBar == pSliderMOD)			//  MOD Slider
	{
		CWnd *pStaticMOD = this->GetDlgItem(IDC_STATIC_MOD);

		// Update contents for Read controls
		UpdateData(TRUE);

		// Get current Slider position
		int iPos = this->m_Slider_MOD.GetPos();

		// convert to str
		CString str;
		str.Format(L"%d", iPos);

		// Out to Interface control 
		pStaticMOD->SetWindowTextW(str);

		// Out to Interface control Edits
		// // Get ByteH and ByteL from WORD
		unsigned int _word;
		unsigned char byte_H;
		unsigned char byte_L;

		// // // ByteH
		_word = iPos;
		_word &= 0xFF00;
		_word = _word >> 8;
		byte_H = (BYTE)_word;

		// // // ByteL
		_word = iPos;
		_word &= 0x00FF;
		byte_L = (BYTE)_word;

		// // Convert to String Hex
		// // Out in Edits
		CString str_hex;
		str_hex.Format(_T("%02X"), byte_H);
		this->m_Edit_Mod_H.SetWindowTextW(str_hex);

		str_hex.Truncate(0);

		str_hex.Format(_T("%02X"), byte_L);
		this->m_Edit_Mod_L.SetWindowTextW(str_hex);

		// Update interface for Write controls
		UpdateData(FALSE);


	}

	if (pScrollBar == pSliderHOR)			//  HOR Slider
	{
		CWnd *pStaticHOR = this->GetDlgItem(IDC_STATIC_HOR);

		// Update contents for Read controls
		UpdateData(TRUE);

		// Get current Slider position
		char iPos = this->m_Slider_HORISON.GetPos();

		// convert to str
		CString str;
		str.Format(L"%d", iPos);

		// Out to Interface control Label
		pStaticHOR->SetWindowTextW(str);

		// Out to Interface control Edits
		// Format Value
		// FORMAT:
		// 1Byte = bits:: data:7654321, direction:0

		// define direction
		// define Value
		char direction;
		unsigned char iPosFormat;

		if (iPos < 0)
		{
			direction = 1;
			iPosFormat = iPos * (-1);
		}
		else
		{
			direction = 0;
			iPosFormat = iPos;
		}
		
		iPosFormat = iPosFormat << 1;
		iPosFormat &= 0xFE;
		iPosFormat += direction;

		// Out to Interface control Edits
		// // Convert to String Hex
		// // Out in Edits
		CString str_hex;
		str_hex.Format(_T("%02X"), iPosFormat);
		this->m_Edit_Horison.SetWindowTextW(str_hex);

		// Update interface for Write controls
		UpdateData(FALSE);

	}

	if (pScrollBar == pSliderVERT)			//  VERT Slider
	{
		CWnd *pStaticVERT = this->GetDlgItem(IDC_STATIC_VERT);

		// Update contents for Read controls
		UpdateData(TRUE);

		// Get current Slider position
		unsigned char iPos = this->m_Slider_VERTICAL.GetPos();

		// convert to str
		CString str;
		str.Format(L"%d", iPos);

		// Out to Interface control Label
		pStaticVERT->SetWindowTextW(str);

		// Out to Interface control Edits
		// Format Value
		// FORMAT:
		// 1Byte = bits:: data:7654321, active:0

		// define Status
		unsigned char iStatus;

		UpdateData(TRUE);
		if (m_bCheck_Vertical)
		{
			iStatus = 1;
		}
		else
		{
			iStatus = 0;
		}

		// define Value
		unsigned char iPosFormat;

		iPosFormat = iPos;
		iPosFormat = iPosFormat << 1;
		iPosFormat &= 0xFE;
		iPosFormat += iStatus;

		// Out to Interface control Edits
		// // Convert to String Hex
		// // Out in Edits
		CString str_hex;
		str_hex.Format(_T("%02X"), iPosFormat);
		this->m_Edit_Vertical.SetWindowTextW(str_hex);

		// Update interface for Write controls
		UpdateData(FALSE);

	}

	if (nSBCode == TB_THUMBTRACK)
	{
		

	}

	//Trace(_T("y"));

	CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);
}


void CDS4830A_SFPP_CONF::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: Add your message handler code here and/or call default

	CDialogEx::OnVScroll(nSBCode, nPos, pScrollBar);
}


void CDS4830A_SFPP_CONF::OnBnClickedButtonBiasSet()
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

	// Check Valid range
	if (_word < VALUE_BIAS_MIN)
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

	if (_word > VALUE_BIAS_MAX)
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
	uValue[0x80 + ADDR_BIAS + 1] = byte_L;

	// set Grid Values
	this->m_Grid.GridSFF_Write(uValue, 0, 256);

}

// -------------------------------------------------------------------
// Edit Control Event Functions
// -------------------------------------------------------------------
void CDS4830A_SFPP_CONF::OnEnChangeEditBiasH()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialogEx::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	this->EditHexControl(&m_Edit_Bias_H);
}


void CDS4830A_SFPP_CONF::OnEnChangeEditBiasL()
{
	this->EditHexControl(&m_Edit_Bias_L);
}


void CDS4830A_SFPP_CONF::OnBnClickedButtonModSet()
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

	// > proceed SECOND byte
	this->m_Edit_Mod_L.GetWindowTextW(str_edit);

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

	// Check Valid range
	if (_word < VALUE_MOD_MIN)
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

	if (_word > VALUE_MOD_MAX)
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
	uValue[0x80 + ADDR_MOD + 1] = byte_L;

	// set Grid Values
	this->m_Grid.GridSFF_Write(uValue, 0, 256);

}


void CDS4830A_SFPP_CONF::OnBnClickedButtonHorSet()
{
	// > Get Value
	CString str;

	// > proceed FIRST byte
	this->m_Edit_Horison.GetWindowTextW(str);

	// > > Check null Value
	// no check: edit disabled

	// convert to Byte
	// no convert: The Value has been already converted

	// > > align Hex String to Valid format
	// no align: The Value has been already aligned

	// > Make INT from String HEX
	unsigned char _byte;

	_byte = (BYTE)_tcstoul(str, NULL, 16);

	// TODO: simplify. No need to read/write all 256 Values
	// > Write Value in Grid
	// get Grid Values
	unsigned char uValue[256];

	this->m_Grid.GridSFF_Read(uValue, 0, 256);

	// put Value
	uValue[0x80 + ADDR_HORISON] = _byte;

	// set Grid Values
	this->m_Grid.GridSFF_Write(uValue, 0, 256);
}


void CDS4830A_SFPP_CONF::OnBnClickedCheckVertical()
{

	CWnd *pStaticVERT = this->GetDlgItem(IDC_STATIC_VERT);

	// Update contents for Read controls
	UpdateData(TRUE);

	// Get current Slider position
	unsigned char iPos = this->m_Slider_VERTICAL.GetPos();

	// convert to str
	CString str;
	str.Format(L"%d", iPos);

	// Out to Interface control Label
	pStaticVERT->SetWindowTextW(str);

	// Out to Interface control Edits
	// Format Value
	// FORMAT:
	// 1Byte = bits:: data:7654321, active:0

	// define Status
	// set Slidebar Enable State
	unsigned char iStatus;
	
	CWnd *wndSlide = this->GetDlgItem(IDC_SLIDER_VERTICAL);
	
	UpdateData(TRUE);
	if (m_bCheck_Vertical)
	{
		iStatus = 1;

		// enadle Control
		wndSlide->EnableWindow(TRUE);
		
	}
	else
	{
		iStatus = 0;
		
		// disable Control
		wndSlide->EnableWindow(FALSE);
	}

	// define Value
	unsigned char iPosFormat;

	iPosFormat = iPos;
	iPosFormat = iPosFormat << 1;
	iPosFormat &= 0xFE;
	iPosFormat += iStatus;

	// Out to Interface control Edits
	// // Convert to String Hex
	// // Out in Edits
	CString str_hex;
	str_hex.Format(_T("%02X"), iPosFormat);
	this->m_Edit_Vertical.SetWindowTextW(str_hex);

	// Update interface for Write controls
	UpdateData(FALSE);

}


void CDS4830A_SFPP_CONF::OnBnClickedButtonVerticalSet()
{
	// > Get Value
	CString str;

	// > proceed FIRST byte
	this->m_Edit_Vertical.GetWindowTextW(str);

	// > > Check null Value
	// no check: edit disabled

	// convert to Byte
	// no convert: The Value has been already converted

	// > > align Hex String to Valid format
	// no align: The Value has been already aligned

	// > Make INT from String HEX
	unsigned char _byte;

	_byte = (BYTE)_tcstoul(str, NULL, 16);

	// TODO: simplify. No need to read/write all 256 Values
	// > Write Value in Grid
	// get Grid Values
	unsigned char uValue[256];

	this->m_Grid.GridSFF_Read(uValue, 0, 256);

	// put Value
	uValue[0x80 + ADDR_VERTICAL] = _byte;

	// set Grid Values
	this->m_Grid.GridSFF_Write(uValue, 0, 256);
}


void CDS4830A_SFPP_CONF::OnBnClickedCheckSaferange()
{
	// var for label OP
	CString str;

	// get static components
	// // BIAS
	CWnd *pStaticBIAS = this->GetDlgItem(IDC_STATIC_BIAS);
	CWnd *pStaticBIAS_MIN = this->GetDlgItem(IDC_STATIC_BIAS_MIN);
	CWnd *pStaticBIAS_MAX = this->GetDlgItem(IDC_STATIC_BIAS_MAX);

	// // MOD
	CWnd *pStaticMOD = this->GetDlgItem(IDC_STATIC_MOD);
	CWnd *pStaticMOD_MIN = this->GetDlgItem(IDC_STATIC_MOD_MIN);
	CWnd *pStaticMOD_MAX = this->GetDlgItem(IDC_STATIC_MOD_MAX);

	// get Control Value
	UpdateData(TRUE);

	if (this->m_bCheck_SafeRange)
	{
		// [SAFE mode]

		// BIAS
		// slider
		m_Slider_BIAS.SetRange(VALUE_BIAS_MIN, VALUE_BIAS_MAX);
		m_Slider_BIAS.SetPos(VALUE_BIAS_DEF);

		// label
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
		this->m_Edit_Bias_L.LimitText(2);

		// fill with Value
		// // Get ByteH and ByteL from WORD
		unsigned short _word;
		unsigned char byte_H;
		unsigned char byte_L;

		// // // ByteH
		_word = VALUE_BIAS_DEF;
		_word &= 0xFF00;
		_word = _word >> 8;
		byte_H = (BYTE)_word;

		// // // ByteL
		_word = VALUE_BIAS_DEF;
		_word &= 0x00FF;
		byte_L = (BYTE)_word;

		// // place Values
		CString str_hex;
		str_hex.Format(_T("%02X"), byte_H);
		this->m_Edit_Bias_H.SetWindowTextW(str_hex);

		str_hex.Truncate(0);

		str_hex.Format(_T("%02X"), byte_L);
		this->m_Edit_Bias_L.SetWindowTextW(str_hex);


		// MOD
		// slider
		m_Slider_MOD.SetRange(VALUE_MOD_MIN, VALUE_MOD_MAX);
		m_Slider_MOD.SetPos(VALUE_MOD_DEF);

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
		this->m_Edit_Mod_L.LimitText(2);

		// fill with Value
		// // Get ByteH and ByteL from WORD
		//unsigned int _word;
		//unsigned char byte_H;
		//unsigned char byte_L;

		// // // ByteH
		_word = VALUE_MOD_DEF;
		_word &= 0xFF00;
		_word = _word >> 8;
		byte_H = (BYTE)_word;

		// // // ByteL
		_word = VALUE_MOD_DEF;
		_word &= 0x00FF;
		byte_L = (BYTE)_word;

		// // place Values
		//CString str_hex;
		str_hex.Format(_T("%02X"), byte_H);
		this->m_Edit_Mod_H.SetWindowTextW(str_hex);

		str_hex.Truncate(0);

		str_hex.Format(_T("%02X"), byte_L);
		this->m_Edit_Mod_L.SetWindowTextW(str_hex);

	}
	else
	{
		// [DANGER mode]

		// BIAS
		// slider
		m_Slider_BIAS.SetRange(VALUE_BIAS_MIN_OC, VALUE_BIAS_MAX_OC);

		m_Slider_BIAS.SetPos(VALUE_BIAS_DEF_OC);
		//int iPos = this->m_Slider_BIAS.GetPos();
		//m_Slider_BIAS.SetPos(iPos);

		// label
		// set Values
		str.Format(L"%d", VALUE_BIAS_MIN_OC);			// Set & Convert
		pStaticBIAS_MIN->SetWindowTextW(str);		// Out to Interface control 

		str.Format(L"%d", VALUE_BIAS_MAX_OC);			// Set & Convert
		pStaticBIAS_MAX->SetWindowTextW(str);		// Out to Interface control 

		str.Format(L"%d", VALUE_BIAS_DEF_OC);			// Set & Convert
		pStaticBIAS->SetWindowTextW(str);			// Out to Interface control 

		// fill with Value
		// // Get ByteH and ByteL from WORD
		unsigned short _word;
		unsigned char byte_H;
		unsigned char byte_L;

		// // // ByteH
		_word = VALUE_BIAS_DEF_OC;
		_word &= 0xFF00;
		_word = _word >> 8;
		byte_H = (BYTE)_word;

		// // // ByteL
		_word = VALUE_BIAS_DEF_OC;
		_word &= 0x00FF;
		byte_L = (BYTE)_word;

		// // place Values
		CString str_hex;
		str_hex.Format(_T("%02X"), byte_H);
		this->m_Edit_Bias_H.SetWindowTextW(str_hex);

		str_hex.Truncate(0);

		str_hex.Format(_T("%02X"), byte_L);
		this->m_Edit_Bias_L.SetWindowTextW(str_hex);


		// MOD
		// slider
		m_Slider_MOD.SetRange(VALUE_MOD_MIN_OC, VALUE_MOD_MAX_OC);

		m_Slider_MOD.SetPos(VALUE_MOD_DEF_OC);
		//iPos = this->m_Slider_BIAS.GetPos();
		//m_Slider_BIAS.SetPos(iPos);

		// set Values
		str.Format(L"%d", VALUE_MOD_MIN_OC);		// Set & Convert
		pStaticMOD_MIN->SetWindowTextW(str);		// Out to Interface control 

		str.Format(L"%d", VALUE_MOD_MAX_OC);		// Set & Convert
		pStaticMOD_MAX->SetWindowTextW(str);		// Out to Interface control 

		str.Format(L"%d", VALUE_MOD_DEF_OC);		// Set & Convert
		pStaticMOD->SetWindowTextW(str);			// Out to Interface control 

		// fill with Value
		// // Get ByteH and ByteL from WORD
		//unsigned int _word;
		//unsigned char byte_H;
		//unsigned char byte_L;

		// // // ByteH
		_word = VALUE_MOD_DEF_OC;
		_word &= 0xFF00;
		_word = _word >> 8;
		byte_H = (BYTE)_word;

		// // // ByteL
		_word = VALUE_MOD_DEF_OC;
		_word &= 0x00FF;
		byte_L = (BYTE)_word;

		// // place Values
		//CString str_hex;
		str_hex.Format(_T("%02X"), byte_H);
		this->m_Edit_Mod_H.SetWindowTextW(str_hex);

		str_hex.Truncate(0);

		str_hex.Format(_T("%02X"), byte_L);
		this->m_Edit_Mod_L.SetWindowTextW(str_hex);

	}

	UpdateData(FALSE);
}


void CDS4830A_SFPP_CONF::OnBnClickedCheckBiasModDependance()
{
	// TODO: Add your control notification handler code here
}


void CDS4830A_SFPP_CONF::OnOK()
{
	// TODO: Add your specialized code here and/or call the base class

	// CDialogEx::OnOK();
}
