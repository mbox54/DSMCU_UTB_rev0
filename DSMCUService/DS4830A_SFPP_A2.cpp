// DS4830A_SFPP_A2.cpp : implementation file
//

#include "stdafx.h"
#include "DSMCUService.h"
#include "DS4830A_SFPP_A2.h"
#include "afxdialogex.h"


// CDS4830A_SFPP_A2 dialog

IMPLEMENT_DYNAMIC(CDS4830A_SFPP_A2, CDialog)

CDS4830A_SFPP_A2::CDS4830A_SFPP_A2(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_PROPPAGE_DS4830A_SFPP_A2, pParent)
{

}

CDS4830A_SFPP_A2::CDS4830A_SFPP_A2(HID_SMBUS_DEVICE * pHidSmbus, CProgressCtrl * p_cPB_OP, CEdit * p_EDIT_STATUS, st_serviceData * p_service, CWnd * pParent /*=NULL*/)
	: CDialog(IDD_PROPPAGE_DS4830A_SFPP_A2, pParent)
	, m_pHidSmbus(pHidSmbus)
	, p_EDIT_STATUS(p_EDIT_STATUS)
	, p_cPB_OP(p_cPB_OP)
	, p_service(p_service)
	, m_Grid(m_pHidSmbus, p_cPB_OP, p_EDIT_STATUS, p_service)
{
}

CDS4830A_SFPP_A2::~CDS4830A_SFPP_A2()
{
}

void CDS4830A_SFPP_A2::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_GRID, m_Grid);
	DDX_Control(pDX, IDC_TRACE, m_TraceWnd);
}

// -------------------------------------------------------------------
// Table procedures
// -------------------------------------------------------------------
// TODO:
// remove from this class

unsigned char CDS4830A_SFPP_A2::DeviceSlave_Read(BYTE * v_ByteData, BYTE slaveAddr, BYTE startAddr, WORD count)
{
	BOOL				opened;
	HID_SMBUS_STATUS	status;
	HID_SMBUS_S0		status0;

	BYTE				buffer_cycle[HID_SMBUS_MAX_READ_RESPONSE_SIZE];
	BYTE				buffer_store[256];

	BYTE				numBytesRead = 0;

	// > SafeCheck
	// Make sure that the device is opened
	BOOL tryDevice = HidSmbus_IsOpened(*m_pHidSmbus, &opened);										// check, return to "opened"
																									// > Check connection																								
	if (opened == 0)
	{
		return MAXQBL_NO_DEVICE;
	}
	// Make sure that the device is operating
	if (tryDevice != HID_SMBUS_SUCCESS)																// check error
	{
		return MAXQBL_DEV_INACCESSIBLE;
	}
	// Make sure that parameters is in Valid Range
	if ((count == 0) || (startAddr + count > 256))
	{
		return MAXQBL_OP_WRONG;
	}

	// > Get Data from device
	BYTE	targetAddress[HID_SMBUS_MAX_TARGET_ADDRESS_SIZE];
	targetAddress[0] = startAddr;

	// > Read from Device
	// addressed Read
	status = HidSmbus_AddressReadRequest(*m_pHidSmbus, slaveAddr, count, 1, targetAddress);
	// wait for i2c proceed
	Sleep(100);

	// > Init read parameters
	// define one Read OP cycle size
	// NOTE: CP2112 provide /61 bytes back maximum/ at 1 Read OP
	// so cycle_size Value must be < 61
	const unsigned char cycle_size = 32;

	unsigned int uRounds = count / cycle_size;
	unsigned int uRemain = count - uRounds * cycle_size;
	unsigned char iCount;
	unsigned int uPos = 0;

	do
	{
		// > Define Read count
		if (uRounds > 0)
		{
			uRounds--;
			iCount = cycle_size;
		}
		else
		{
			iCount = uRemain;
		}

		// Notify device that it should send a read response back
		status = HidSmbus_ForceReadResponse(*m_pHidSmbus, iCount);
		// Load data to buffer
		status = HidSmbus_GetReadResponse(*m_pHidSmbus, &status0, buffer_cycle, HID_SMBUS_MAX_READ_RESPONSE_SIZE, &numBytesRead);

		// Check OP proceed
		if (status != HID_SMBUS_SUCCESS)
		{
			return MAXQBL_OP_FAILED;
		}
		// Check OP result
		if (numBytesRead != iCount)
		{
			return MAXQBL_OP_FAILED;
		}

		// > Copy this segment to the Store
		for (unsigned int k = 0; k < iCount; k++)
		{
			buffer_store[uPos + k] = buffer_cycle[k];
		}

		uPos += iCount;

	} while (uPos < count);


	// Form Result
	for (unsigned int k = 0; k < count; k++)
	{
		v_ByteData[startAddr + k] = buffer_store[k];
	}

	return HID_SMBUS_SUCCESS;
}

unsigned char CDS4830A_SFPP_A2::DeviceSlave_Write(BYTE * v_ByteData, BYTE slaveAddr, BYTE startAddr, WORD count)
{
	BOOL				opened;
	HID_SMBUS_STATUS	status;

	BYTE				buffer_cycle[HID_SMBUS_MAX_WRITE_REQUEST_SIZE];

	// > SafeCheck
	// Make sure that the device is opened
	BOOL tryDevice = HidSmbus_IsOpened(*m_pHidSmbus, &opened);										// check, return to "opened"
																									// > Check connection																								
	if (opened == 0)
	{
		return MAXQBL_NO_DEVICE;
	}
	// Make sure that the device is operating
	if (tryDevice != HID_SMBUS_SUCCESS)																// check error
	{
		return MAXQBL_DEV_INACCESSIBLE;
	}
	// Make sure that parameters is in Valid Range
	if ((count == 0) || (startAddr + count > 256))
	{
		return MAXQBL_OP_WRONG;
	}

	// > Set Data to device
	// define one Write OP cycle size
	// NOTE: CP2112 provide /61 bytes forward maximum/ at 1 Write OP
	// so cycle_size Value must be < 61
	const unsigned char cycle_size = 32;

	unsigned int uRounds = count / cycle_size;
	unsigned int uRemain = count - uRounds * cycle_size;
	unsigned char iCount;
	unsigned int uPos = 0;

	buffer_cycle[0] = startAddr;			// Set Addr, write OP starts from 

	do
	{
		// > Define Read count
		if (uRounds > 0)
		{
			uRounds--;
			iCount = cycle_size;
		}
		else
		{
			iCount = uRemain;
		}

		// > Load segment to transfer
		for (unsigned int k = 0; k < iCount; k++)
		{
			buffer_cycle[1 + k] = v_ByteData[uPos + k];
		}

		status = HidSmbus_WriteRequest(*m_pHidSmbus, slaveAddr, buffer_cycle, iCount + 1);

		// wait for i2c proceed
		Sleep(40);

		// Check OP proceed
		if (status != HID_SMBUS_SUCCESS)
		{
			return MAXQBL_OP_FAILED;
		}

		uPos += iCount;
		buffer_cycle[0] = uPos;

	} while (uPos < count);


	return HID_SMBUS_SUCCESS;
}

// -------------------------------------------------------------------
// Supporting procedures
// -------------------------------------------------------------------
void CDS4830A_SFPP_A2::Trace(LPCTSTR szFmt, ...)
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
void CDS4830A_SFPP_A2::OnGridClick(NMHDR * pNotifyStruct, LRESULT * pResult)
{
	NM_GRIDVIEW* pItem = (NM_GRIDVIEW*)pNotifyStruct;
	Trace(_T("Clicked on row %d, col %d\n"), pItem->iRow, pItem->iColumn);
	Trace(_T("Text: %s\n"), m_Grid.GetItemText(pItem->iRow, pItem->iColumn));
	//Item.strText
}

void CDS4830A_SFPP_A2::OnGridEndEdit(NMHDR * pNotifyStruct, LRESULT * pResult)
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
BEGIN_MESSAGE_MAP(CDS4830A_SFPP_A2, CDialog)	
	// Grid event functions
	ON_NOTIFY(NM_CLICK, IDC_GRID, OnGridClick)
	ON_NOTIFY(GVN_ENDLABELEDIT, IDC_GRID, OnGridEndEdit)

	ON_BN_CLICKED(IDC_BUTTON1, &CDS4830A_SFPP_A2::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CDS4830A_SFPP_A2::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CDS4830A_SFPP_A2::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON5, &CDS4830A_SFPP_A2::OnBnClickedButton5)
	ON_BN_CLICKED(IDC_BUTTON4, &CDS4830A_SFPP_A2::OnBnClickedButton4)
END_MESSAGE_MAP()

// -------------------------------------------------------------------
// CDS4830A_SFPP_A2 message handlers
// -------------------------------------------------------------------
BOOL CDS4830A_SFPP_A2::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	m_Grid.Init();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}


// -------------------------------------------------------------------
// Buttons Test
// -------------------------------------------------------------------
void CDS4830A_SFPP_A2::OnBnClickedButton1()
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


void CDS4830A_SFPP_A2::OnBnClickedButton2()
{
	// TODO: Add your control notification handler code here

	m_Grid.GridSFF_Read(uValues2, 0, 256);
}


void CDS4830A_SFPP_A2::OnBnClickedButton3()
{
	// TODO: Add your control notification handler code here
	m_Grid.GridSFF_Write(uValues2, 0, 256);
}

// -------------------------------------------------------------------
// Table op call events
// -------------------------------------------------------------------

// Read Table Procedure
void CDS4830A_SFPP_A2::OnBnClickedButton4()
{
	// Read op
	//m_Grid.DeviceSlave_ReadTimer(uValues, 0, SLAVEADDR_A2, 0, 256, 0, 0);

	// Read op
	//m_Grid.DeviceSlave_ReadTimer(uValues2, 0, 0xA2, 0, 256, 0, 0);

	// NOTE: debug from ver1.1 'DDM-auto' need more stability to Timer_R/W, so it replaces to previous
	//m_Grid.DeviceSlave_ReadTimer(uValues, 0, SLAVEADDR_A0, 0, 256, 0, 0);

	//
	//Trace(_T("trying: READ\n"));
	//Trace(_T("params: A0, 256 bytes\n"));

	// prepare Table output
	// clear Table
	m_Grid.ClearTable();

	Trace(_T("ïîïûòêà: ×ÒÅÍÈÅ\n"));
	Trace(_T("ïàðàìåòðû: A2, 256 áàéò\n"));

	// temp buffer for OP
	BYTE v_Values[256];

	// get Data from Device
	BYTE retVal = m_Grid.DeviceSlave_Read(v_Values, SLAVEADDR_A2, 0, 256);

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

	UpdateData(FALSE);


}

// Write Table Procedure
void CDS4830A_SFPP_A2::OnBnClickedButton5()
{
	// write op
	m_Grid.DeviceSlave_WriteTimer(uValues, 0, SLAVEADDR_A2, 0, 256, 0, 0);

	// NOTE: debug from ver1.1 'DDM-auto' need more stability to Timer_R/W, so it replaces to previous

	Trace(_T("ïîïûòêà: ÇÀÏÈÑÜ\n"));
	Trace(_T("ïàðàìåòðû: A2, 256 áàéò\n"));

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
	Trace(_T("ÓÑÏÅØÍÎ.\n"));
	Trace(_T("-----------------------\n"));

}





void CDS4830A_SFPP_A2::OnOK()
{
	// TODO: Add your specialized code here and/or call the base class

	// CDialog::OnOK();
}
