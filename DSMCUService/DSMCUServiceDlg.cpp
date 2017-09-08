
// DSMCUServiceDlg.cpp : implementation file
//

#include "stdafx.h"
#include "afxdialogex.h"

#include "DSMCUServiceDlg.h"
#include "DS4830A_srvDlg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

/////////////////////////////////////////////////////////////////////////////
// Library Dependencies
#pragma comment(lib, "SLABHIDtoSMBus.lib")


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
	virtual void OnOK();
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CDSMCUServiceDlg dialog



CDSMCUServiceDlg::CDSMCUServiceDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DSMCUSERVICE_DIALOG, pParent)
	, m_iRadio_DeviceType(0)
	, m_sEdit_COM_NUM(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CDSMCUServiceDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	//  DDX_CBString(pDX, IDC_COMBO_DEVICE_LIST, m_comboDeviceList);
	DDX_Control(pDX, IDC_COMBO_DEVICE_LIST, m_comboDeviceList);
	DDX_Control(pDX, IDC_STATIC_CONNECT, m_static_connect);
	DDX_Control(pDX, IDC_STATIC_LOGO, m_Static_Logo);
	DDX_Radio(pDX, IDC_RADIO_TEST_BOARD, m_iRadio_DeviceType);
	DDX_Text(pDX, IDC_EDIT_COM_NUM, m_sEdit_COM_NUM);
	DDV_MaxChars(pDX, m_sEdit_COM_NUM, 2);
}

void CDSMCUServiceDlg::UpdateDeviceList()
{
	// Only update the combo list when the drop down list is closed
	if (!m_comboDeviceList.GetDroppedState())
	{
		int						sel;
		CString					serial;
		DWORD					numDevices;
		HID_SMBUS_DEVICE_STR	deviceString;

		// Get previous combobox string selection
		GetSelectedDevice(serial);

		// Remove all strings from the combobox
		m_comboDeviceList.ResetContent();

		// Get number of HID devices with matching VID/PID (0/0 means not filtered)
		if (HidSmbus_GetNumDevices(&numDevices, VID, PID) == HID_SMBUS_SUCCESS)
		{
			// Iterate through each HID device with matching VID/PID
			for (DWORD i = 0; i < numDevices; i++)
			{
				// Add serial strings to the combobox
				if (HidSmbus_GetString(i, VID, PID, deviceString, HID_SMBUS_GET_SERIAL_STR) == HID_SMBUS_SUCCESS)
				{
					m_comboDeviceList.AddString(CString(deviceString));
				}
			}
		}

		sel = m_comboDeviceList.FindStringExact(-1, serial);

		// Select first combobox string
		if (sel == CB_ERR)
		{
			m_comboDeviceList.SetCurSel(0);
		}
		// Restore previous combobox selection
		else
		{
			m_comboDeviceList.SetCurSel(sel);
		}
	}

}

// Get the combobox device selection
// If a device is selected, return TRUE and return the serial string
// Otherwise, return FALSE
BOOL CDSMCUServiceDlg::GetSelectedDevice(CString & serial)
{
	BOOL selected = FALSE;

	int			sel;
	CString		selText;

	// Get current selection index or CB_ERR(-1)
	// if no device is selected
	sel = m_comboDeviceList.GetCurSel();

	if (sel != CB_ERR)
	{
		// Get the selected device string
		m_comboDeviceList.GetLBText(sel, selText);
		selected = TRUE;
		serial = selText;
	}

	return selected;
}

// Search for an HID device with a matching device serial string
// If the device was found return TRUE and return the device number
// in deviceNumber
// Otherwise return FALSE
BOOL CDSMCUServiceDlg::FindDevice(CString serial, DWORD & deviceNum)
{
	BOOL					found = FALSE;
	DWORD					numDevices;
	HID_SMBUS_DEVICE_STR	deviceString;

	if (HidSmbus_GetNumDevices(&numDevices, VID, PID) == HID_SMBUS_SUCCESS)
	{
		for (DWORD i = 0; i < numDevices; i++)
		{
			if (HidSmbus_GetString(i, VID, PID, deviceString, HID_SMBUS_GET_SERIAL_STR) == HID_SMBUS_SUCCESS)
			{
				if (serial.CompareNoCase(CString(deviceString)) == 0)
				{
					found = TRUE;
					deviceNum = i;
					break;
				}
			}
		}
	}

	return found;
}

BOOL CDSMCUServiceDlg::Connect()
{
	BOOL		connected = FALSE;
	CString		serial;
	DWORD		deviceNum;

	// Get selected device serial string
	if (GetSelectedDevice(serial))
	{
		// Find the selected device number
		if (FindDevice(serial, deviceNum))
		{
			HID_SMBUS_STATUS status = HidSmbus_Open(&m_hidSmbus, deviceNum, VID, PID);
			Sleep(50);

			CString statusText;
			//			statusText.Format(_T("HidSmbus_Open(): %s"), HidSmbus_DecodeErrorStatus(status).GetString());
			//			SetStatusText(statusText);

			// Attempt to open the device
			if (status == HID_SMBUS_SUCCESS)
			{
				connected = TRUE;
			}
			else
			{
				CString msg;
				//				msg.Format(_T("Failed to connect to %s: %s"), serial.GetString(), HidSmbus_DecodeErrorStatus(status).GetString());
				MessageBox(msg, 0, MB_ICONWARNING);
			}
		}
	}

	// Connected
	if (connected)
	{
		// Check/press the connect button
		CheckDlgButton(IDC_CHECK_CONNECT, TRUE);

		// Update Connect/Disconnect button caption
		SetDlgItemText(IDC_CHECK_CONNECT, _T("&ќтключить"));

		// Update connection status text
		//		SetConnectionText(serial);

		// Disable the device combobox
		m_comboDeviceList.EnableWindow(FALSE);

		// Enable all device controls when connected
		//EnableDeviceCtrls(TRUE);

		// > Set GPIO Config
		// GPIO3 = 0 / GND
//		HID_SMBUS_STATUS status = HidSmbus_WriteLatch(*&m_hidSmbus, 1, 0xE8);


		UpdateData(TRUE);
		BYTE ucDeviceType = m_iRadio_DeviceType;


		st_CP2112_GPConf conf_curr;
		switch (ucDeviceType)
		{
		case 0:		// Test Board
			// ### Config for FT SFPp TestBoard rev.2 ###
			conf_curr.direction = 0xA8;			// NOTE: 0 = input, 1 = output
			conf_curr.mode = 0x00;				// NOTE: 0 = open-drain, 1 = push-pull
			conf_curr.function = 0x06;			// NOTE: 000 = no specFunct, 111 = all Enabled ([1]Rx, [0]Tx, [7]Clk)

			break;

		case 1:		// Universal Reader
			// ### Config for FT Universal Reader rev.1 ###
			conf_curr.direction = 0xE2;			// NOTE: 0 = input, 1 = output
			conf_curr.mode = 0x00;				// NOTE: 0 = open-drain, 1 = push-pull
			conf_curr.function = 0x02;			// NOTE: 000 = no specFunct, 111 = all Enabled  ([1]Rx, [0]Tx, [7]Clk)

			break;


		default:	// err state

			break;
		}

		// Set GPIO direction and mode bitmasks
		HID_SMBUS_STATUS status = HidSmbus_SetGpioConfig(m_hidSmbus, conf_curr.direction, conf_curr.mode, conf_curr.function, 0);

		// init Latch
		HidSmbus_WriteLatch(m_hidSmbus, 0x00, 0xFF);

		// store active params
		CP2112_activeDeviceNum = deviceNum;
		CP2112_GPConf = conf_curr;

	}
	// Disconnected
	else
	{
		// Uncheck/unpress the connect button
		CheckDlgButton(IDC_CHECK_CONNECT, FALSE);

		// Update Connect/Disconnect button caption
		SetDlgItemText(IDC_CHECK_CONNECT, _T("&ѕодключитьс€"));

		// Update connection status text
		//		SetConnectionText(_T("Not Connected"));

		// Enable the device combobox
		m_comboDeviceList.EnableWindow(TRUE);

		// Disable all device controls when disconnected
		//EnableDeviceCtrls(FALSE);
	}
	
	// Update the device information now that we are connected to it
	// (this will give us the part number and version if connected)
	//UpdateDeviceInformation(TRUE);

	// Update all device settings for all property pages
	// ! Parameters reading also when it can't connect properly
	//	m_pageConfiguration.SetFromDevice();
	//	m_pageDataTransfer.SetFromDevice();
	//	m_pagePinConfiguration.SetFromDevice();
	//	m_pageCustomization.SetFromDevice();

	return connected;
}

BOOL CDSMCUServiceDlg::Disconnect()
{
	BOOL disconnected = FALSE;

	// Disconnect from the current device
	HID_SMBUS_STATUS status = HidSmbus_Close(m_hidSmbus);
	m_hidSmbus = NULL;

	// CString statusText;
	// statusText.Format(_T("HidSmbus_Close(): %s"), HidSmbus_DecodeErrorStatus(status).GetString());
	// SetStatusText(statusText);

	// Output an error message if the close failed
	if (status != HID_SMBUS_SUCCESS)
	{
		// CString msg;
		// msg.Format(_T("Failed to disconnect: %s"), HidSmbus_DecodeErrorStatus(status).GetString());
		// MessageBox(msg, 0, MB_ICONWARNING);
	}
	else
	{
		disconnected = TRUE;
	}

	// Uncheck/unpress the connect button
	// CheckDlgButton(IDC_CHECK_CONNECT, FALSE);

	// Update Connect/Disconnect button caption
	SetDlgItemText(IDC_CHECK_CONNECT, _T("&ѕодключитьс€"));

	// Update connection status text
	// SetConnectionText(_T("Not Connected"));

	// Enable the device combobox
	m_comboDeviceList.EnableWindow(TRUE);

	// Disable all device controls when disconnected
	EnableDeviceCtrls(FALSE);

	// Update the device information (clear)
	UpdateDeviceInformation(TRUE);

	return disconnected;
}


BOOL CDSMCUServiceDlg::Connect_MCU()
{
	// > Get COM Number
	UpdateData(TRUE);

	BYTE dwComNum = (BYTE)_tcstoul(this->m_sEdit_COM_NUM, NULL, 10);

	// > Connect
	int iResult = m_UTBDevice.Connect(dwComNum);


	return iResult;
}


BOOL CDSMCUServiceDlg::Disconnect_MCU()
{

	// > Disconnect
	int iResult = COMPort_Close(&m_hSerialCDC);


	return iResult;
}


void CDSMCUServiceDlg::EnableDeviceCtrls(BOOL enable)
{
//	GetDlgItem(IDC_BUTTON_RESET)->EnableWindow(enable);

//	EnableDlgControls(m_pageConfiguration.GetSafeHwnd(), enable);
//	EnableDlgControls(m_pageDataTransfer.GetSafeHwnd(), enable);
//	EnableDlgControls(m_pagePinConfiguration.GetSafeHwnd(), enable);
//	EnableDlgControls(m_pageCustomization.GetSafeHwnd(), enable);

//	if (enable)
//	{
//		m_pagePinConfiguration.UpdateControlStates();
//	}
}

void CDSMCUServiceDlg::UpdateDeviceInformation(BOOL connected)
{
	BOOL					opened;
	HID_SMBUS_DEVICE_STR	deviceString;
	WORD					vid;
	WORD					pid;
	WORD					releaseNumber;
	BYTE					partNumber;
	BYTE					version;

	CString					vidString;
	CString					pidString;
	CString					releaseNumberString;
	CString					partNumberString;
	CString					versionString;
	CString					serialString;
	CString					pathString;
	CString					manufacturerString;
	CString					productString;

/*

	// If we're already connected to the device, then we can call the
	// opened version of the device information functions
	if (connected == TRUE &&
		HidSmbus_IsOpened(m_hidSmbus, &opened) == HID_SMBUS_SUCCESS &&
		opened == TRUE)
	{
		// Update device information (opened)

		if (HidSmbus_GetOpenedAttributes(m_hidSmbus, &vid, &pid, &releaseNumber) == HID_SMBUS_SUCCESS)
		{
			vidString.Format(_T("%04X"), vid);
			pidString.Format(_T("%04X"), pid);
			releaseNumberString.Format(_T("%X.%02X"), HIBYTE(releaseNumber), LOBYTE(releaseNumber));
		}
		if (HidSmbus_GetPartNumber(m_hidSmbus, &partNumber, &version) == HID_SMBUS_SUCCESS)
		{
			partNumberString.Format(_T("%d"), partNumber);
			versionString.Format(_T("%d"), version);
		}
		if (HidSmbus_GetOpenedString(m_hidSmbus, deviceString, HID_SMBUS_GET_SERIAL_STR) == HID_SMBUS_SUCCESS)
		{
			serialString = deviceString;
		}
		if (HidSmbus_GetOpenedString(m_hidSmbus, deviceString, HID_SMBUS_GET_PATH_STR) == HID_SMBUS_SUCCESS)
		{
			pathString = deviceString;
		}
		if (HidSmbus_GetOpenedString(m_hidSmbus, deviceString, HID_SMBUS_GET_MANUFACTURER_STR) == HID_SMBUS_SUCCESS)
		{
			manufacturerString = deviceString;
		}
		if (HidSmbus_GetOpenedString(m_hidSmbus, deviceString, HID_SMBUS_GET_PRODUCT_STR) == HID_SMBUS_SUCCESS)
		{
			productString = deviceString;
		}
	}
	else
	{
		CString		serial;
		DWORD		deviceNum;

		// Get selected device serial string
		if (GetSelectedDevice(serial))
		{
			// Find the selected device number
			if (FindDevice(serial, deviceNum))
			{
				// Update device information

				if (HidSmbus_GetAttributes(deviceNum, VID, PID, &vid, &pid, &releaseNumber) == HID_SMBUS_SUCCESS)
				{
					vidString.Format(_T("%04X"), vid);
					pidString.Format(_T("%04X"), pid);
					releaseNumberString.Format(_T("%X.%02X"), HIBYTE(releaseNumber), LOBYTE(releaseNumber));
				}
				if (HidSmbus_GetString(deviceNum, VID, PID, deviceString, HID_SMBUS_GET_SERIAL_STR) == HID_SMBUS_SUCCESS)
				{
					serialString = deviceString;
				}
				if (HidSmbus_GetString(deviceNum, VID, PID, deviceString, HID_SMBUS_GET_PATH_STR) == HID_SMBUS_SUCCESS)
				{
					pathString = deviceString;
				}
				if (HidSmbus_GetString(deviceNum, VID, PID, deviceString, HID_SMBUS_GET_MANUFACTURER_STR) == HID_SMBUS_SUCCESS)
				{
					manufacturerString = deviceString;
				}
				if (HidSmbus_GetString(deviceNum, VID, PID, deviceString, HID_SMBUS_GET_PRODUCT_STR) == HID_SMBUS_SUCCESS)
				{
					productString = deviceString;
				}
			}
		}
	}
	*/

	// Update device information on the dialog
/*	SetDlgItemText(IDC_EDIT_VID, vidString);
	SetDlgItemText(IDC_EDIT_PID, pidString);
	SetDlgItemText(IDC_EDIT_RELEASE_NUMBER, releaseNumberString);
	SetDlgItemText(IDC_EDIT_PART_NUMBER, partNumberString);
	SetDlgItemText(IDC_EDIT_VERSION, versionString);
	SetDlgItemText(IDC_EDIT_PATH, pathString);
	SetDlgItemText(IDC_EDIT_SERIAL, serialString);
	SetDlgItemText(IDC_EDIT_MANUFACTURER, manufacturerString);
	SetDlgItemText(IDC_EDIT_PRODUCT, productString);
	*/
}

BEGIN_MESSAGE_MAP(CDSMCUServiceDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_CBN_DROPDOWN(IDC_COMBO_DEVICE_LIST, &CDSMCUServiceDlg::OnDropdownComboDeviceList)
	ON_CBN_CLOSEUP(IDC_COMBO_DEVICE_LIST, &CDSMCUServiceDlg::OnCloseupComboDeviceList)
	ON_CBN_SELCHANGE(IDC_COMBO_DEVICE_LIST, &CDSMCUServiceDlg::OnSelchangeComboDeviceList)
	ON_BN_CLICKED(IDOK, &CDSMCUServiceDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_CHECK_CONNECT, &CDSMCUServiceDlg::OnBnClickedCheckConnect)
	ON_STN_CLICKED(IDC_STATIC_CONNECT, &CDSMCUServiceDlg::OnStnClickedStaticConnect)
	ON_BN_CLICKED(IDC_BUTTON_OPERATOR, &CDSMCUServiceDlg::OnBnClickedButtonOperator)
	ON_BN_CLICKED(IDC_BUTTON_ADMIN, &CDSMCUServiceDlg::OnBnClickedButtonAdmin)
END_MESSAGE_MAP()


// CDSMCUServiceDlg message handlers

BOOL CDSMCUServiceDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// Bitmap
	m_static_connect.ModifyStyle(0, SS_BITMAP);

	// init 
	m_connected_flag = 0;
	m_iRadio_DeviceType = 0;

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CDSMCUServiceDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CDSMCUServiceDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		// Load connection status picture
		CRect rect;
		m_static_connect.GetClientRect(&rect);

		CBitmap bmp;

		// set connection status picture
		if (m_connected_flag)
		{
			bmp.LoadBitmap(IDB_BITMAP1);
		}
		else
		{
			bmp.LoadBitmap(IDB_BITMAP2);
		}
		
		BITMAP b;
		bmp.GetObject(sizeof(BITMAP), &b);

		CPaintDC dc(&m_static_connect);

		CDC memdc;
		memdc.CreateCompatibleDC(&dc);
		memdc.SelectObject(&bmp);

		dc.StretchBlt(0, 0, rect.Width(), rect.Height(), &memdc,
			0, 0, b.bmWidth, b.bmHeight, SRCCOPY);



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
		CPaintDC dc2(&m_Static_Logo);

		CDC memdcLogo;
		memdcLogo.CreateCompatibleDC(&dc2);
		memdcLogo.SelectObject(&bitmapLogo);

		// Place stratch
		CRect rectLogo;
		m_Static_Logo.GetClientRect(&rectLogo);

		dc2.StretchBlt(0, 0, rectLogo.Width(), rectLogo.Height(), &memdcLogo,
			0, 0, imgLogo.bmWidth, imgLogo.bmHeight, SRCCOPY);



		/* // !debug
		dc.MoveTo(0, 0);
		dc.LineTo(100, 100);
		*/

		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CDSMCUServiceDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


// COMBO_BOX_MESSAGES

void CDSMCUServiceDlg::OnDropdownComboDeviceList()
{
	// TODO: Add your control notification handler code here
	// TODO: 
	// Automatically update the device list when the
	// list is opened/dropped down
	UpdateDeviceList();

	// Update device information for selected device (not connected)
	//UpdateDeviceInformation(FALSE);

}


void CDSMCUServiceDlg::OnCloseupComboDeviceList()
{
	// TODO: Add your control notification handler code here
	// TODO: 
	CString		serial;
	DWORD		deviceNum;

	if (GetSelectedDevice(serial))
	{
		// If the selected device has been removed
		if (!FindDevice(serial, deviceNum))
		{
			// Then update the device list
			UpdateDeviceList();
			//UpdateDeviceInformation(FALSE);
		}
	}

}


void CDSMCUServiceDlg::OnSelchangeComboDeviceList()
{
	// TODO: Add your control notification handler code here

}


void CDSMCUServiceDlg::OnClickedCheckConnect()
{
	// TODO: Add your control notification handler code here
}


void CDSMCUServiceDlg::OnBnClickedOk()
{
//	CWnd * pWnd = this->GetWindow(GW_OWNER);
//		GetSafeHwnd();


	// open module dialog 
    CDS4830A_srvDlg DS4830A_srvDlg(&this->m_hidSmbus, MD_ENGINEER, CP2112_activeDeviceNum, CP2112_GPConf);
	DS4830A_srvDlg.DoModal();


	CDialogEx::OnOK();
}



// NOTE:
// 
// GIPO.0 Ц Tx Toggle config.
// GPIO.1 Ц Rx Toggle config.
// GPIO.2 Ц Input.Tx Fault. 1 Ц ошибка в работе передатчика, 0 Ц все нормально.
// GPIO.3 Ц Output.Tx Disable.Open - Drain. 1 Ц отключение лазера, 0 Ц включение.
// GPIO.4 Ц Input.Mod ABS. 1 Ц модуль отсутствует, 0 Ц модуль присутствует.
// GPIO.5 Ц Output.RS0.Open - Drain.ƒва состо€ни€ 1 и 0. –еализовать галочку.
// GPIO.6 Ц Input.Rx LOS. 1 Ц сигнал на приеме ниже дозволенного уровн€, 0 Ц все нормально.
// GPIO.7 Ц Output.RS1.Open - Drain.ƒва состо€ни€ 1 и 0. –еализовать галочку.

void CDSMCUServiceDlg::OnBnClickedCheckConnect()
{
	// TODO: Add your control notification handler code here
	CWnd *pBtnOper = this->GetDlgItem(IDC_BUTTON_OPERATOR);
	CWnd *pBtnAdmn = this->GetDlgItem(IDC_BUTTON_ADMIN);

	if (m_connected_flag)
	{
		// [Disconnecting]
		Disconnect_MCU();

		// Disable Controls
		pBtnOper->EnableWindow(FALSE);
		pBtnAdmn->EnableWindow(FALSE);

		m_connected_flag = 0;
	}
	else
	{
		// [Connecting]

		// try to connect
		int iResult = Connect_MCU();

		if (iResult)
		{
			// success

			// Enable Controls
			pBtnOper->EnableWindow(TRUE);
			pBtnAdmn->EnableWindow(TRUE);

			m_connected_flag = 1;
		}
		
	}

	// Rewrite Window graphic
	OnPaint();

	// Update Window grapics
	Invalidate(0);
	
	//open gpio config
	//CGPIOConfig GPIOConfigDlg(&this->m_hidSmbus);
	//GPIOConfigDlg.DoModal();
	// }

}


void CDSMCUServiceDlg::OnStnClickedStaticConnect()
{
	// TODO: Add your control notification handler code here
}


void CDSMCUServiceDlg::OnBnClickedButtonOperator()
{
	// # set Mode MD_OPERATOR

	// open module dialog 
	CDS4830A_srvDlg DS4830A_srvDlg(&this->m_hSerialCDC, MD_OPERATOR, CP2112_activeDeviceNum, CP2112_GPConf);
	DS4830A_srvDlg.DoModal();

	CDialogEx::OnOK();

}


void CDSMCUServiceDlg::OnBnClickedButtonAdmin()
{
	// open module dialog 
	CDS4830A_srvDlg DS4830A_srvDlg(&this->m_hidSmbus, MD_ENGINEER, CP2112_activeDeviceNum, CP2112_GPConf);
	DS4830A_srvDlg.DoModal();

	CDialogEx::OnOK();
}


void CAboutDlg::OnOK()
{
	// TODO: Add your specialized code here and/or call the base class

	// CDialogEx::OnOK();
}
