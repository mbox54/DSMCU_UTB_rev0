// DS4830A_SFPP_DDM.cpp : implementation file
//

#include "stdafx.h"
#include "DSMCUService.h"
#include "DS4830A_SFPP_DDM.h"
#include "afxdialogex.h"


// CDS4830A_SFPP_DDM dialog

IMPLEMENT_DYNAMIC(CDS4830A_SFPP_DDM, CDialogEx)

CDS4830A_SFPP_DDM::CDS4830A_SFPP_DDM(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_PROPPAGE_DS4830A_SFPP_DDM, pParent)
{

}

CDS4830A_SFPP_DDM::CDS4830A_SFPP_DDM(HID_SMBUS_DEVICE * pHidSmbus, CProgressCtrl * p_cPB_OP, CEdit * p_EDIT_STATUS, st_serviceData * p_service, CWnd * pParent)
	: CDialogEx(IDD_PROPPAGE_DS4830A_SFPP_DDM, pParent)
	, m_pHidSmbus(pHidSmbus)
	, p_EDIT_STATUS(p_EDIT_STATUS)
	, p_cPB_OP(p_cPB_OP)
	, p_service(p_service)
	, m_Grid(m_pHidSmbus, p_cPB_OP, p_EDIT_STATUS, p_service)
{
}

CDS4830A_SFPP_DDM::~CDS4830A_SFPP_DDM()
{
}

void CDS4830A_SFPP_DDM::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_GRID, m_Grid);
	DDX_Control(pDX, IDC_GRID_DESCR, m_GridDescr);
}


BEGIN_MESSAGE_MAP(CDS4830A_SFPP_DDM, CDialogEx)
	// Grid event functions
	ON_NOTIFY(GVN_ENDLABELEDIT, IDC_GRID_DESCR, OnGridDescrEndEdit)

	ON_BN_CLICKED(IDC_BUTTON1, &CDS4830A_SFPP_DDM::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON4, &CDS4830A_SFPP_DDM::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON5, &CDS4830A_SFPP_DDM::OnBnClickedButton5)
	ON_BN_CLICKED(IDC_BUTTON6, &CDS4830A_SFPP_DDM::OnBnClickedButton6)
	ON_BN_CLICKED(IDC_BUTTON7, &CDS4830A_SFPP_DDM::OnBnClickedButton7)
END_MESSAGE_MAP()


// CDS4830A_SFPP_DDM message handlers


void CDS4830A_SFPP_DDM::OnBnClickedButton1()
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


void CDS4830A_SFPP_DDM::OnBnClickedButton4()
{
	// TODO: Add your control notification handler code here
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
	Trace(_T("ïàðàìåòðû: A2, 96 áàéò\n"));

	// temp buffer for OP
	BYTE v_Values[256];

	// get Data from Device
	BYTE retVal = m_Grid.DeviceSlave_Read(v_Values, SLAVEADDR_A2, 0x00, 96);
	
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

	}
	else
	{
		// output to Grid
		m_Grid.GridSFF_Write(v_Values, 0, 96);

		m_GridDescr.UpdateTable_All(v_Values);
		m_GridDescr.Invalidate(0);

		Trace(_T("ÓÑÏÅØÍÎ.\n"));
	}

	Trace(_T("-----------------------\n"));
}


void CDS4830A_SFPP_DDM::OnBnClickedButton5()
{
	// write op
	//m_Grid.DeviceSlave_WriteTimer(uValues, 0, SLAVEADDR_A2, 0, 256, 0, 0);

	BYTE v_DDMValues[96];

	// read from Grid
	m_Grid.GridSFF_Read(v_DDMValues, 0, 96);

	// Write to Device
	BYTE retVal = m_Grid.DeviceSlave_Write(v_DDMValues, SLAVEADDR_A2, 0, 96);


	// NOTE: debug from ver1.1 'DDM-auto' need more stability to Timer_R/W, so it replaces to previous

	Trace(_T("ïîïûòêà: ÇÀÏÈÑÜ\n"));
	Trace(_T("ïàðàìåòðû: A2, 96 áàéò\n"));

	/*

	// temp buffer for OP
	BYTE v_Values[256];

	// input from Grid
	m_Grid.GridSFF_Read(v_Values, 0, 256);

	// write op
	//m_Grid.DeviceSlave_WriteTimer(uValues, 0, SLAVEADDR_A0, 0, 256, 0, 0);

	// set Data to Device
	BYTE retVal = m_Grid.DeviceSlave_Write(v_Values, SLAVEADDR_A2, 0, 256);

	if (retVal != HID_SMBUS_SUCCESS)
	{
	// error: Device Write
	Trace(_T("ÎØÈÁÊÀ. [êîä: %02d] \n"), retVal);
	return;
	}


	*/

	if (retVal != HID_SMBUS_SUCCESS)
	{
		// error: Device Write
		Trace(_T("ÎØÈÁÊÀ. [êîä: %02d] \n"), retVal);
	}
	else
	{
		Trace(_T("ÓÑÏÅØÍÎ.\n"));
	}

	
	Trace(_T("-----------------------\n"));



}


void CDS4830A_SFPP_DDM::Trace(LPCTSTR szFmt, ...)
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

void CDS4830A_SFPP_DDM::OnGridEndEdit(NMHDR * pNotifyStruct, LRESULT * pResult)
{

}

void CDS4830A_SFPP_DDM::OnGridDescrEndEdit(NMHDR * pNotifyStruct, LRESULT * pResult)
{
	// NOTE: parent event is used. No need to implement here.
	int bChanged;
	bChanged = this->m_GridDescr.OnGridEndEdit(pNotifyStruct, pResult);

	if (bChanged == 0)
	{
		// update SFF Base Grid
		NM_GRIDVIEW* pItem = (NM_GRIDVIEW*)pNotifyStruct;

		// > Define Row Type
		// NOTE: Table is NOT uniform
		// FORMAT:
		// Bytes 00 - 39:	Fields: 00 - 19		 /	size: 2, count: 20	/ Diagnostic Flag Alarm and Warning Thresholds (see Table 3.15)
		// Bytes 40 - 55:	Fields: 20 - 27		 /	size: 2, count: 08	/ Unlocated
		// Bytes 56 - 75:	Fields: 28 - 32		 /	size: 4, count: 05	/ Rx PWR Diagnostic calibration constants for optional External Calibration
		// Bytes 76 - 91:	Fields: 33 - 40		 /	size: 2, count: 08	/ Other  Diagnostic calibration constants for optional External Calibration
		// Bytes 92 - 94:	Fields: 41 - 42		 /	size: 1, count: 03	/ Unlocated
		// Bytes 95:		Fields: 43			 /	size: 1, count: 01	/ CHK of 00-94

		// get Range number, Byte number
		unsigned short index_mst = 0;
		unsigned short byte_number = 0;
		unsigned char err = 0;

		byte_number = m_GridDescr.GetByteNumber(pItem->iRow - 1, &index_mst, &err);

		// > Proceed Selected Rows
		// get value
		CString str_cellText = m_GridDescr.GetItemText(pItem->iRow, 2);
		CString str_HexByte;

		BYTE v_setBytes[4];
		BYTE byte_cellText;

		unsigned char uGridRow;
		unsigned char uGridCol;

		switch (v_RowSet_Sizes[index_mst])
		{
		case 1: // 1 Bytes 
			// transfer
			// // 1 symbol
			str_HexByte.Truncate(0);
			str_HexByte.AppendChar(str_cellText[0]);
			str_HexByte.AppendChar(str_cellText[1]);

			byte_cellText = (BYTE)_tcstoul(str_HexByte, NULL, 16);
			v_setBytes[0] = byte_cellText;

			break;

		case 2: // 2 Bytes 

			// transfer
			// // 1 symbol
			str_HexByte.Truncate(0);
			str_HexByte.AppendChar(str_cellText[0]);
			str_HexByte.AppendChar(str_cellText[1]);

			byte_cellText = (BYTE)_tcstoul(str_HexByte, NULL, 16);
			v_setBytes[0] = byte_cellText;

			// // 2 symbol
			str_HexByte.Truncate(0);
			str_HexByte.AppendChar(str_cellText[2]);
			str_HexByte.AppendChar(str_cellText[3]);

			byte_cellText = (BYTE)_tcstoul(str_HexByte, NULL, 16);
			v_setBytes[1] = byte_cellText;

			break;

		case 4: // 4 Bytes 

			// transfer
			for (unsigned char k = 0; k < 4; k++)
			{
				str_HexByte.Truncate(0);
				str_HexByte.AppendChar(str_cellText[2*k]);
				str_HexByte.AppendChar(str_cellText[1 + 2*k]);

				byte_cellText = (BYTE)_tcstoul(str_HexByte, NULL, 16);
				v_setBytes[k] = byte_cellText;

			}

			break;
			
		default:
			// err
			return;

			break;

		} //switch RowSet

		// set
		// // place Value
		m_Grid.GridSFF_Write(v_setBytes, byte_number, v_RowSet_Sizes[index_mst]);

		// place selector
		uGridRow = byte_number / 16;
		uGridCol = byte_number - uGridRow * 16;

		m_Grid.SetSelectedRange(uGridRow + 1, uGridCol + 1, uGridRow + 1, uGridCol + 1);

		// refresh component
		m_Grid.Invalidate(0);

	} //(bChanged == 0)

}

BOOL CDS4830A_SFPP_DDM::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_GridDescr.Init("DDM_tablDef.txt");
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
	unsigned short uValueTemp[4];


	// > Set subordinate Fields
	unsigned char v_temp[4];

	for (unsigned short k = 0; k < 45; k++)
	{
		// load value (main)
		m_GridDescr.ProcTable(k, 0, v_temp, 1);

		// convert 
		CString cellText_str;

		// // define var Type (4/2/1)
		unsigned char k2 = 3;
		unsigned char bZeroVal = 0;
		unsigned char ACT = 1;
		while (ACT)
		{
			if (v_temp[k2] == 0)
			{
				// [proceed adjust]
				if (k2 > 0)
				{
					// [next check]
					k2--;
				}
				else
				{
					// [cancel on zero]

					bZeroVal = 1;
					ACT = 0;
				}
			}
			else
			{
				// [cancel adjust]
				ACT = 0;
			}
		} // define var Type (4/2/1)


		if (!bZeroVal)
		{
			// [construct String]
			unsigned char k3 = 0;
			unsigned char ACT = 1;
			while (ACT)
			{
				cellText_str.AppendFormat(_T("%02X"), v_temp[k3]);

				if (k3 == k2)
				{
					ACT = 0;
				}
				else
				{
					k3++;
				}
			}
		}
		else
		{
			// [set 00h Value]
			cellText_str.Append(_T("00"));
		}

	

		// update subordinate values
		m_GridDescr.UpdateTable_Sel(k, 2, cellText_str);
	}


	unsigned char v_buf[4];

	unsigned char row_byte_padding = 0;
	m_GridDescr.GetRowNumber(63, &row_byte_padding, v_buf);

	unsigned char temp1 = v_buf[0];

	//m_GridDescr.UpdateTable_All(uValues);

	unsigned char v_byteDescr[160];

	// > Fill SFF Table
	// values from DDM file
	for (unsigned short k = 0; k < 96; k++)
	{
		m_GridDescr.ProcTable(k, 1, v_temp, 1);

		v_byteDescr[k] = v_temp[0];
	}
	
	// set
	m_Grid.GridSFF_Write(v_byteDescr, 0, 96);

	// filler values
	for (unsigned short k = 0; k < 160; k++)
	{
		v_byteDescr[k] = 255;
	}

	// set
	m_Grid.GridSFF_Write(v_byteDescr, 96, 160);

	// update output control
	m_GridDescr.Invalidate(0);


	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}


void CDS4830A_SFPP_DDM::OnBnClickedButton6()
{
	// > test CheckSum
	BYTE uValues[128];
	BYTE v_CHK_curr[1];

	CString str;

	// CHK95
	// // get data
	m_Grid.GridSFF_Read(uValues, 0, 95);
	m_Grid.GridSFF_Read(v_CHK_curr, 95, 1);

	// // check // compare
	unsigned char ucCHK_check = CheckCHK1(v_CHK_curr[0], uValues, 95);

	// > set controls output
	if (ucCHK_check)
	{
		// [SUCCESS]

		str.AppendFormat(_T("Ïðîâåðêà ïðîéäåíà."));
	}
	else
	{
		// [FAILURE]

		str.AppendFormat(_T("×åêñóììà íåâåðíà"));
	}

	CWnd *pStaticCHK63 = this->GetDlgItem(IDC_STATIC_CHK_95);
	pStaticCHK63->SetWindowTextW(str);


}


// Update CHK
void CDS4830A_SFPP_DDM::OnBnClickedButton7()
{
	// > test CheckSum
	BYTE uValues[128];
	BYTE v_CHK_curr[1];

	CString str;

	// CHK63
	// // get data
	m_Grid.GridSFF_Read(uValues, 0, 95);

	// // calculate
	v_CHK_curr[0] = CalculateHash1(uValues, 95);

	// // set
	m_Grid.GridSFF_Write(v_CHK_curr, 95, 1);


}


void CDS4830A_SFPP_DDM::OnOK()
{
	// TODO: Add your specialized code here and/or call the base class

	// CDialogEx::OnOK();
}
