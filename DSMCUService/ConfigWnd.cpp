// GPIOConfig.cpp: файл реализации
//

#include "stdafx.h"
#include "ConfigWnd.h"
#include "afxdialogex.h"

// диалоговое окно CGPIOConfig

IMPLEMENT_DYNAMIC(CConfigWnd, CDialogEx)


/////////////////////////////////////////////////////////////////////////////
// Static Global Variables
// Set status bar pane 0 and 1 indicators
static UINT BASED_CODE StatusBarIndicators[] =
{
	ID_INDICATOR_DEVICE,
	ID_INDICATOR_STATUS
};

//CGPIOConfig::CGPIOConfig(CWnd* pParent /*=NULL*/)
//	: CDialogEx(IDD_GPIO_CONFIG, pParent)
//{}
//CGPIOConfig::CGPIOConfig(HID_SMBUS_DEVICE* pHidSmbus, CWnd* pParent /*=NULL*/)
//	: CDialogEx(IDD_GPIO_CONFIG, pParent), 	m_pHidSmbus(pHidSmbus)
//{
//}

// volatile sig_atomic_t flag = 0;

/////////////////////////////////////////////////////////////////////////////
// Static Global Functions
/////////////////////////////////////////////////////////////////////////////

// Given a input string containing hex byte values,
// return a vector of the numeric values
std::vector<BYTE> GetHexArray(CString hexString)
{
	std::vector<BYTE> retVal;

	// Remove whitespace
	hexString.Replace(_T(" "), _T(""));
	hexString.Replace(_T(","), _T(""));
	hexString.Replace(_T("\t"), _T(""));
	hexString.Replace(_T("\r"), _T(""));
	hexString.Replace(_T("\n"), _T(""));

	// Grab valid hexadecimal characters
	hexString = hexString.SpanIncluding(_T("0123456789abcdefABCDEF"));

	// Make sure that there are an even number of hex characters
	if (hexString.GetLength() % 2 == 1)
	{
		// Insert an extra zero before the first character
		hexString = _T("0") + hexString;
	}

	// Iterate through each byte (2 hex characters)
	for (int i = 0; i < hexString.GetLength(); i += 2)
	{
		// Insert the decimal value into the vector
		CString byteString = hexString.Mid(i, 2);
		retVal.push_back((BYTE)_tcstoul(byteString, NULL, 16));
	}

	return retVal;
}

// convert Alphabetic Hex symbols to Byte
std::vector<BYTE> StrHexToByte(BYTE * v_HexLetters, int count, unsigned char * errCode)
{
	// NOTE:
	// errCode definition: 0 - success; 1 - string op error; 2 - invalid hex symbol.

	std::vector<BYTE> retVal;										// temp vector

	try
	{	// Srting operations

		// Get Hex Letter String
		CString hexString(v_HexLetters);

		// Grab valid hexadecimal characters
		hexString.Truncate(count * 2);
		CString hexString_valid = hexString.SpanIncluding(_T("0123456789abcdefABCDEF"));

		if (hexString != hexString_valid)
		{	// invalit symbols
			throw 2;

		}

		// Iterate through each byte (2 hex characters)
		for (int i = 0; i < count * 2; i += 2)
		{
			// Insert the decimal value into the vector
			CString byteString = hexString.Mid(i, 2);
			retVal.push_back((BYTE)_tcstoul(byteString, NULL, 16));		// _tcstoul: Str, EndPtr, Radix 
		}
	}
	catch (const std::exception&)
	{	// String op error
		*errCode = 1;
	}

	catch (int iErr)
	{
		*errCode = iErr;
	}

	return retVal;
}

// convert Byte to Alphabetic Hex symbols 
CString ByteToStrHex(BYTE * v_ByteData, int count, unsigned char * errCode)
{
	CString StrHex;
	for (int i = 0; i < count; i++)
	{
		StrHex.AppendFormat(_T("%02X"), v_ByteData[i]);
	}

	return StrHex;
}


CConfigWnd::CGPIOConfig(CWnd * pParent)
	: m_TextOutput(_T(""))
	, m_TextFilename(_T(""))
{
}

/*
_crt_signal_t CGPIOConfig::signalHandler(int signum)
{
	Sleep(10);
	// cleanup and close up stuff here  
	// terminate program  
	flag = 1;

	exit(signum);

}
*/

/*
CGPIOConfig::CGPIOConfig(HID_SMBUS_DEVICE * pHidSmbus, CWnd * pParent)
	: CDialogEx(IDD_GPIO_CONFIG, pParent), m_pHidSmbus(pHidSmbus)
{

	// register signal SIGINT and signal handler  
	signal(SIGINT, signalHandler(signum));

}
*/

CConfigWnd::~CConfigWnd()
{
}


void CConfigWnd::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PROGRESS1, m_cPB_OP);
	DDX_Text(pDX, IDC_EDIT_OUTPUT, m_TextOutput);
	DDX_Text(pDX, IDC_EDIT_FILENAME, m_TextFilename);
}


BEGIN_MESSAGE_MAP(CConfigWnd, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_RESET_LATCH, &CConfigWnd::OnBnClickedButtonResetLatch)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_CHECK_LATCH_GPIO_6, &CConfigWnd::OnBnClickedCheckLatch)
	ON_BN_CLICKED(IDC_CHECK_LATCH_GPIO_7, &CConfigWnd::OnBnClickedCheckLatch)
	ON_BN_CLICKED(IDC_CHECK_LATCH_GPIO_5, &CConfigWnd::OnBnClickedCheckLatch)
	ON_BN_CLICKED(IDC_BUTTON2, &CConfigWnd::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CConfigWnd::OnBnClickedButton3)
END_MESSAGE_MAP()



// Initialize the status bar
// - Create the status bar object
// - Set number of panes to 2
// - Make pane 0 a fixed width
// - Make pane 1 fit the rest of the dialog
// - Draw the status bar
// - Set default status bar text to "Not Connected" and ""
void CConfigWnd::InitStatusBar()
{
	// Create the status bar
	m_statusBar.Create(this);

	// Set number of panes
	m_statusBar.SetIndicators(StatusBarIndicators, sizeof(StatusBarIndicators) / sizeof(StatusBarIndicators[0]));

	// Make the device pane a fixed width
	m_statusBar.SetPaneInfo(0, ID_INDICATOR_DEVICE, SBPS_NORMAL, 100);

	// Make the status pane fit the rest of the status bar
	m_statusBar.SetPaneInfo(1, ID_INDICATOR_STATUS, SBPS_STRETCH, 0);

	// Draw the status bar
	RepositionBars(AFX_IDW_CONTROLBAR_FIRST, AFX_IDW_CONTROLBAR_LAST, ID_INDICATOR_STATUS);

	SetStatusText0(_T(""));
}

void CGPIOConfig::SetStatusText0(const CString & text)
{
	// Set the status bar text
	m_statusBar.SetPaneText(1, text);
}

// Set status bar text for specified pane 1
void CConfigWnd::SetStatusText1(const CString & text)
{
	// Set the status bar text
	m_statusBar.SetPaneText(1, text);
}

int CGPIOConfig::GetGPIOini(BYTE * gpio_val)
{
	FILE *fs;
	char* file_name = "cpiodirection.cfg";

	errno_t err;
	if ((err = fopen_s(&fs, file_name, "r")) != 0)
	{
		// "can't read file" 
		return 1;
	}
	else
	{
		char load_str[9] = "00000000";
		fgets(load_str, 9, fs);
		fclose(fs);

		int file_val = 0;

		int k = 1;
		for (int i = 0; i < 8; i++)
		{
			if (load_str[7 - i] == '0')
			{
				//	
			}
			else
			{
				if (load_str[7 - i] == '1')
				{
					file_val += k;
				}
				else
				{
					//bad file
					return 2;
				}
			}

			k = k * 2;
		}

		if (file_val < 256) {
			*gpio_val = (BYTE)file_val;
		}
		else {
			return 3;
		}
	}

	return 0;
}

void CGPIOConfig::LatchController()
{
	//check & set output GPIO pins
	//GP02 in  - SFP, GP03 in  - XFP, GP04 in  - QSFP	/ 0 - active, 1 - none
	//GP05 out - SFP, GP06 out - XFP, GP07 out - QSFP	/ 0 - none,   1 - active

	BOOL opened;

	// Make sure that the device is opened

	if (HidSmbus_IsOpened(*m_pHidSmbus, &opened) == HID_SMBUS_SUCCESS && opened)
	{
		BYTE latchValue;

		// Read GPIO latch value
		HID_SMBUS_STATUS statusR = HidSmbus_ReadLatch(*m_pHidSmbus, &latchValue);

		if (statusR == HID_SMBUS_SUCCESS)
		{

			if (m_gpio_latch != latchValue)
			{
				//iterrupt timer
				this->StopTimer();

				BOOL bPluggedOut = 0;	//in case where we get plug out event with one remain deactive device

				BYTE infomsg = 0;
				//SFP check
				if ((HID_SMBUS_MASK_GPIO_2 & m_gpio_latch) != (HID_SMBUS_MASK_GPIO_2 & latchValue))
				{
					if ((HID_SMBUS_MASK_GPIO_2 & latchValue) == 0)	//SFP has being plagged in
					{
						//config latch, set GP05 
						HID_SMBUS_STATUS statusW = HidSmbus_WriteLatch(*m_pHidSmbus, 255, HID_SMBUS_MASK_GPIO_5);

						if (statusW == HID_SMBUS_SUCCESS)
						{
							//update for reflect latch value on screen
							latchValue = (latchValue | HID_SMBUS_MASK_GPIO_5);
							//info
							infomsg = 11;
						}
					}
					else											//SFP has being plagged out
					{
						//config latch, set GP05 
						HID_SMBUS_STATUS statusW = HidSmbus_WriteLatch(*m_pHidSmbus, 0, HID_SMBUS_MASK_GPIO_5);
						//info
						if (statusW == HID_SMBUS_SUCCESS)
						{
							//update for reflect latch value on screen
							latchValue = (latchValue & 0xDF);
							//info
							infomsg = 12;

							bPluggedOut = TRUE;
						}
					}
				}
				//XFP check
				if ((HID_SMBUS_MASK_GPIO_3 & m_gpio_latch) != (HID_SMBUS_MASK_GPIO_3 & latchValue))
				{
					if ((HID_SMBUS_MASK_GPIO_3 & latchValue) == 0)	//XFP has being plagged in
					{
						//config latch, set GP06 
						HID_SMBUS_STATUS statusW = HidSmbus_WriteLatch(*m_pHidSmbus, 255, HID_SMBUS_MASK_GPIO_6);

						if (statusW == HID_SMBUS_SUCCESS)
						{
							//update for reflect latch value on screen
							latchValue = (latchValue | HID_SMBUS_MASK_GPIO_6);
							//info
							infomsg = 21;
						}
					}
					else											//XFP has being plagged out
					{
						//config latch, set GP06 
						HID_SMBUS_STATUS statusW = HidSmbus_WriteLatch(*m_pHidSmbus, 0, HID_SMBUS_MASK_GPIO_6);
						//info
						if (statusW == HID_SMBUS_SUCCESS)
						{
							//update for reflect latch value on screen
							latchValue = (latchValue & 0xBF);
							//info
							infomsg = 22;

							bPluggedOut = TRUE;
						}
					}
				}
				//QSFP check
				if ((HID_SMBUS_MASK_GPIO_4 & m_gpio_latch) != (HID_SMBUS_MASK_GPIO_4 & latchValue))
				{
					if ((HID_SMBUS_MASK_GPIO_4 & latchValue) == 0)	//QSFP has being plagged in
					{
						//config latch, set GP07 
						HID_SMBUS_STATUS statusW = HidSmbus_WriteLatch(*m_pHidSmbus, 255, HID_SMBUS_MASK_GPIO_7);

						if (statusW == HID_SMBUS_SUCCESS)
						{
							//update for reflect latch value on screen
							latchValue = (latchValue | HID_SMBUS_MASK_GPIO_7);
							//info
							infomsg = 31;
						}
					}
					else											//QSFP has being plagged out
					{
						//config latch, set GP07 
						HID_SMBUS_STATUS statusW = HidSmbus_WriteLatch(*m_pHidSmbus, 0, HID_SMBUS_MASK_GPIO_7);
						//info
						if (statusW == HID_SMBUS_SUCCESS)
						{
							//update for reflect latch value on screen
							latchValue = (latchValue & 0x7F);
							//info
							infomsg = 32;

							bPluggedOut = TRUE;
						}
					}
				}

				m_gpio_latch = latchValue;

				if (infomsg != 0) {

					CString statusText;

					switch (infomsg) {
					case 11: statusText = "SFP has plagged in!";
						//AfxMessageBox((CString)"SFP has plagged in!", MB_OK, MB_ICONASTERISK); 
						break;

					case 12: statusText = "SFP has plagged out!";
						//AfxMessageBox((CString)"SFP has plagged out!", MB_OK, MB_ICONASTERISK);
						break;

					case 21: statusText = "XFP has plagged in!";
						//AfxMessageBox((CString)"XFP has plagged in!", MB_OK, MB_ICONASTERISK);
						break;

					case 22: statusText = "XFP has plagged out!";
						//AfxMessageBox((CString)"XFP has plagged out!", MB_OK, MB_ICONASTERISK);
						break;

					case 31: statusText = "QSFP has plagged in!";
						//AfxMessageBox((CString)"QSFP has plagged in!", MB_OK, MB_ICONASTERISK);
						break;

					case 32: statusText = "QSFP has plagged out!";
						//AfxMessageBox((CString)"QSFP has plagged out!", MB_OK, MB_ICONASTERISK);
						break;
					} // case

					  //status info
					SetStatusText1(statusText);

				} // info

				  //check multiple plugging
				BYTE sum = 0;
				infomsg = 0;	//for active/deactive informing

				if ((HID_SMBUS_MASK_GPIO_2 & latchValue) == 0) { sum++; }
				if ((HID_SMBUS_MASK_GPIO_3 & latchValue) == 0) { sum++; }
				if ((HID_SMBUS_MASK_GPIO_4 & latchValue) == 0) { sum++; }

				if (sum > 1)
				{
					// user need to define active device
					CDeviceSelect deviceSelectDlg(&this->m_gpio_latch);
					deviceSelectDlg.DoModal();

					//config latch

					HID_SMBUS_STATUS statusW = HidSmbus_WriteLatch(*m_pHidSmbus, m_gpio_latch, 255);

					//renew for reflect latch value on screen
					latchValue = m_gpio_latch;

					//info_msg active device
					if ((HID_SMBUS_MASK_GPIO_5 & latchValue) != 0)
					{
						infomsg = 11;
					}
					else
					{
						if ((HID_SMBUS_MASK_GPIO_6 & latchValue) != 0)
						{
							infomsg = 21;
						}
						else
						{
							infomsg = 31;
						}
					}
				}	//multiple plugging

					// activate last remain device event
				if ((sum == 1) && (bPluggedOut))
				{
					if ((HID_SMBUS_MASK_GPIO_2 & latchValue) == 0)
					{
						m_gpio_latch = m_gpio_latch | HID_SMBUS_MASK_GPIO_5;
						infomsg = 11;
					}
					else
					{
						if ((HID_SMBUS_MASK_GPIO_3 & latchValue) == 0)
						{
							m_gpio_latch = m_gpio_latch | HID_SMBUS_MASK_GPIO_6;
							infomsg = 21;
						}
						else
						{
							m_gpio_latch = m_gpio_latch | HID_SMBUS_MASK_GPIO_7;
							infomsg = 31;
						}
					}


					//config latch, set output pin
					HID_SMBUS_STATUS statusW = HidSmbus_WriteLatch(*m_pHidSmbus, m_gpio_latch, 255);

					//renew for reflect latch value on screen
					latchValue = m_gpio_latch;
				}	//activate last remain device event

					// Update controls to reflect latch value
				SetLatchValue(latchValue);

				if (infomsg != 0)
				{
					CString statusText;

					switch (infomsg) {
					case 11: statusText = "SFP power on!";
						//AfxMessageBox((CString)"SFP has plagged in!", MB_OK, MB_ICONASTERISK); 
						break;

					case 12: statusText = "SFP power off!";
						//AfxMessageBox((CString)"SFP has plagged out!", MB_OK, MB_ICONASTERISK);
						break;

					case 21: statusText = "XFP power on!";
						//AfxMessageBox((CString)"XFP has plagged in!", MB_OK, MB_ICONASTERISK);
						break;

					case 22: statusText = "XFP power off!";
						//AfxMessageBox((CString)"XFP has plagged out!", MB_OK, MB_ICONASTERISK);
						break;

					case 31: statusText = "QSFP power on!";
						//AfxMessageBox((CString)"QSFP has plagged in!", MB_OK, MB_ICONASTERISK);
						break;

					case 32: statusText = "QSFP power off!";
						//AfxMessageBox((CString)"QSFP has plagged out!", MB_OK, MB_ICONASTERISK);

					case 40: statusText = "Another device has already activated!";
						//AfxMessageBox((CString)"QSFP has plagged out!", MB_OK, MB_ICONASTERISK);
						break;
					} // case

					  //status info
					SetStatusText1(statusText);
				}	//infomsg

					//restore timer
				this->StartTimer();


			} // m_gpio_latch != latchValue 	
		} // statusR == HID_SMBUS_SUCCESS
	}
}

void CGPIOConfig::LatchReset(BOOL bRest)
{
	// read hardware gpio pins / 00 01 02 03 04 needed
	BYTE latchValue = 0;
	HID_SMBUS_STATUS status = HidSmbus_ReadLatch(*m_pHidSmbus, &latchValue);

	if (status != HID_SMBUS_SUCCESS)
	{
		//err read
	}

	// prepare state for LatchControl procedure //deactivate all output
	//write GPOI 05 06 07 pins default (000)
	status = HidSmbus_WriteLatch(*m_pHidSmbus, 0, 0xE0);
	// renew for reflect
	latchValue = (latchValue & 0x1F);

	if (status != HID_SMBUS_SUCCESS)
	{
		//err write
	}

	if (bRest)
	{
		Sleep(10);	//rest between
	}

	//prepare GPOI 02 03 04 pins default (111)
	latchValue = (latchValue | 0x1C);
	// show renew outut
	SetLatchValue(latchValue);

	// initial status
	m_gpio_latch = latchValue;
}

void CGPIOConfig::SetLatchValue(BYTE latchValue)
{
	if (latchValue & HID_SMBUS_MASK_GPIO_2)		SetDlgItemText(IDC_CHECK_LATCH_GPIO_2, _T("Empty"));
	else										SetDlgItemText(IDC_CHECK_LATCH_GPIO_2, _T("Plugged"));
	if (latchValue & HID_SMBUS_MASK_GPIO_3)		SetDlgItemText(IDC_CHECK_LATCH_GPIO_3, _T("Empty"));
	else										SetDlgItemText(IDC_CHECK_LATCH_GPIO_3, _T("Plugged"));
	if (latchValue & HID_SMBUS_MASK_GPIO_4)		SetDlgItemText(IDC_CHECK_LATCH_GPIO_4, _T("Empty"));
	else										SetDlgItemText(IDC_CHECK_LATCH_GPIO_4, _T("Plugged"));
	if (latchValue & HID_SMBUS_MASK_GPIO_5)		SetDlgItemText(IDC_CHECK_LATCH_GPIO_5, _T("On"));
	else										SetDlgItemText(IDC_CHECK_LATCH_GPIO_5, _T("Off"));
	if (latchValue & HID_SMBUS_MASK_GPIO_6)		SetDlgItemText(IDC_CHECK_LATCH_GPIO_6, _T("On"));
	else										SetDlgItemText(IDC_CHECK_LATCH_GPIO_6, _T("Off"));
	if (latchValue & HID_SMBUS_MASK_GPIO_7)		SetDlgItemText(IDC_CHECK_LATCH_GPIO_7, _T("On"));
	else										SetDlgItemText(IDC_CHECK_LATCH_GPIO_7, _T("Off"));
}


void CGPIOConfig::StartTimer()
{
	m_nTimer = SetTimer(1, 1000, 0);
}

void CGPIOConfig::StopTimer()
{
	KillTimer(m_nTimer);
}

void CGPIOConfig::SetDefaults()
{
	// m_gpio_latch = 0;
}

void CGPIOConfig::SetFromDevice()
{
	// Set default control values
	SetDefaults();

	// # Set logics /to RAM from ini
	// (pins haven't got any onchip memory)
	BYTE direction = 0;
	if (GetGPIOini(&direction) != 0)
	{
		//err
	}

	BYTE mode = 0x00;
	BYTE function = 0x00;

	// Set GPIO direction and mode bitmasks
	HID_SMBUS_STATUS status = HidSmbus_SetGpioConfig(*m_pHidSmbus, direction, mode, function, 0);

	// starting checking plugged devices process
	LatchReset();
	StartTimer();

	UpdateData(FALSE);
}


/////////////////////////////////////////////////////////////////////////////
// Maxim DS4830a Loader procedures
/////////////////////////////////////////////////////////////////////////////
unsigned char CConfigWnd::MaxQ_EnterBoot()
{
	BOOL				opened;
	HID_SMBUS_STATUS	status;

	BYTE				writeData[HID_SMBUS_MAX_WRITE_REQUEST_SIZE];

	// Make sure that the device is opened
	BOOL tryDevice = HidSmbus_IsOpened(*m_pHidSmbus, &opened);										// check, return to "opened"

																									// > Check connection
	if (opened == 0)
	{
		return MAXQBL_NO_DEVICE;
	}

	if (tryDevice != HID_SMBUS_SUCCESS)																// check error
	{
		return MAXQBL_DEV_INACCESSIBLE;
	}

	// > Enter Boot	
	writeData[0] = SET_I2C_SPE;			// Send Slave Boot Address + Write + SPE_flag cmd
	status = HidSmbus_WriteRequest(*m_pHidSmbus, I2C_BOOTLOADER_SLAVE_ADDR, writeData, 1);

	// > Result report
	if (status == HID_SMBUS_SUCCESS)
	{
		return MAXQBL_SUCCESS;
	}
	else
	{
		return MAXQBL_OP_FAILED;
	}

}

unsigned char CGPIOConfig::MaxQ_SoftReset()
{
	BOOL				opened;
	HID_SMBUS_STATUS	status;

	BYTE				writeData[HID_SMBUS_MAX_WRITE_REQUEST_SIZE];

	// Make sure that the device is opened
	BOOL tryDevice = HidSmbus_IsOpened(*m_pHidSmbus, &opened);										// check, return to "opened"

																									// > Check connection																								
	if (opened == 0)
	{
		return MAXQBL_NO_DEVICE;
	}

	if (tryDevice != HID_SMBUS_SUCCESS)																// check error
	{
		return MAXQBL_DEV_INACCESSIBLE;
	}

	// > Soft Reset
	writeData[0] = I2C_SW_RESET;		// Send Slave Boot Address + Write + I2C_reset cmd
	status = HidSmbus_WriteRequest(*m_pHidSmbus, I2C_BOOTLOADER_SLAVE_ADDR, writeData, 1);

	// > Result report
	if (status == HID_SMBUS_SUCCESS)
	{
		return MAXQBL_SUCCESS;
	}
	else
	{
		return MAXQBL_OP_FAILED;
	}
}

unsigned char CGPIOConfig::MaxQ_MasterErase()
{
	BOOL				opened;
	HID_SMBUS_STATUS	status;

	BYTE				writeData[HID_SMBUS_MAX_WRITE_REQUEST_SIZE];

	// Make sure that the device is opened
	BOOL tryDevice = HidSmbus_IsOpened(*m_pHidSmbus, &opened);										// check, return to "opened"

																									// > Check connection																								
	if (opened == 0)
	{
		return MAXQBL_NO_DEVICE;
	}

	if (tryDevice != HID_SMBUS_SUCCESS)																// check error
	{
		return MAXQBL_DEV_INACCESSIBLE;
	}

	// > Master Erase
	writeData[0] = MASTER_ERASE;		// Send Slave Boot Address + Write + I2C_reset cmd
	status = HidSmbus_WriteRequest(*m_pHidSmbus, I2C_DEVICE_SLAVE_ADDR, writeData, 1);

	// > Result report
	if (status == HID_SMBUS_SUCCESS)
	{
		return MAXQBL_SUCCESS;
	}
	else
	{
		return MAXQBL_OP_FAILED;
	}
}

unsigned char CGPIOConfig::MaxQ_Poll3Eh(unsigned char *MAXQBL_RET_3EH)
{
	BOOL				opened;
	HID_SMBUS_STATUS	status;
	HID_SMBUS_S0		status0;

	BYTE				buffer[HID_SMBUS_MAX_READ_RESPONSE_SIZE];

	BYTE				numBytesRead = 0;

	// Make sure that the device is opened
	BOOL tryDevice = HidSmbus_IsOpened(*m_pHidSmbus, &opened);										// check, return to "opened"

																									// > Check connection																								
	if (opened == 0)
	{
		return MAXQBL_NO_DEVICE;
	}

	if (tryDevice != HID_SMBUS_SUCCESS)																// check error
	{
		return MAXQBL_DEV_INACCESSIBLE;
	}

	// > Get Data from device
	status = HidSmbus_ReadRequest(*m_pHidSmbus, I2C_DEVICE_SLAVE_ADDR, 1);	// simple Read / 1 byte
	Sleep(100);																// wait for i2c proceed
	status = HidSmbus_ForceReadResponse(*m_pHidSmbus, 1);					// Notify device that it should send a read response back
	status = HidSmbus_GetReadResponse(*m_pHidSmbus, &status0, buffer, HID_SMBUS_MAX_READ_RESPONSE_SIZE, &numBytesRead);	// Load data to buffer

																														// > Device return parameter check
	if (buffer[0] == 0x3E)
	{
		*MAXQBL_RET_3EH = MAXQBL_RET_IS_3E;
	}
	else
		if (buffer[0] == 0xB7)
		{
			*MAXQBL_RET_3EH = MAXQBL_RET_IS_B7;
		}
		else
		{
			*MAXQBL_RET_3EH = MAXQBL_RET_NO_3E;
		}

	// > Result report																														// > Result report
	if (status == HID_SMBUS_SUCCESS)
	{
		return MAXQBL_SUCCESS;
	}
	else
	{
		return MAXQBL_OP_FAILED;
	}
}


/*************************************************************************************
Command					Name Command	LSB						MSB
Get Status				04h				Flags					Status
Get Code Size (flash)	06h				SizeЦlower byte			SizeЦhigher byte
Get Data Size (SRAM)	07h				SizeЦlower byte			SizeЦhigher byte
Get Loader Version		08h				Lower version number	Higher version number
Get Utility ROM Version 09h				Lower version number	Higher version number
**************************************************************************************/

/*************************************************************************************
*** CMD possible values ***
#define GET_STATUS							0x04
#define GET_CODE_SIZE						0x06
#define GET_DATA_SIZE						0x07
#define GET_LOADER_VERSION					0x08
#define GET_UROM_VERSION					0x09
**************************************************************************************/
unsigned char CConfigWnd::MaxQ_CMD_2B(unsigned char CMD, unsigned char *MSB, unsigned char *LSB, unsigned char *MAXQBL_RET_3EH, unsigned char MODE)
{
	BOOL				opened;
	HID_SMBUS_STATUS	status;
	HID_SMBUS_S0		status0;

	BYTE				writeData[HID_SMBUS_MAX_WRITE_REQUEST_SIZE];
	BYTE				buffer[HID_SMBUS_MAX_READ_RESPONSE_SIZE];

	BYTE				numBytesRead = 0;

	// TODO: need electrical capacity CLK - GND to avoid chargind. If not - prevent with OP delay.
	Sleep(20);

	// Make sure that the device is opened
	BOOL tryDevice = HidSmbus_IsOpened(*m_pHidSmbus, &opened);										// check, return to "opened"

																									// > Check connection																								
	if (opened == 0)
	{
		return MAXQBL_NO_DEVICE;
	}

	if (tryDevice != HID_SMBUS_SUCCESS)																// check error
	{
		return MAXQBL_DEV_INACCESSIBLE;
	}

	// > Check availability of Command
	if (!((CMD > 0x03) && (CMD < 0x0A) && (CMD != 0x05)))
	{
		return MAXQBL_OP_WRONG;
	}

	// > Set Command to device
	writeData[0] = CMD;	// Send Slave Device Address + Write + cmd
	status = HidSmbus_WriteRequest(*m_pHidSmbus, I2C_DEVICE_SLAVE_ADDR, writeData, 1);

	// > Get Data from device 
	int readCount = 3;

	// NOTE: MODE = 1 - proceed electrical circuit bug, that appears in: 
	// xx X1 .. .. X2 X3. /X1 - bad byte staffed, X2 - shifted byte from staf, X3 - last byte corrupted 
	if (MODE == 1)
	{
		readCount += 2;
	}

	status = HidSmbus_ReadRequest(*m_pHidSmbus, I2C_DEVICE_SLAVE_ADDR, readCount);	// simple Read /read 3 bytes (2b + "3Eh")
	Sleep(30);																// wait for i2c proceed
	status = HidSmbus_ForceReadResponse(*m_pHidSmbus, readCount);					// Notify device that it should send a read response back
	status = HidSmbus_GetReadResponse(*m_pHidSmbus, &status0, buffer, HID_SMBUS_MAX_READ_RESPONSE_SIZE, &numBytesRead);	// Wait for a read response

	if (numBytesRead != readCount)
	{
		return MAXQBL_OP_FAILED;
	}

	if (MODE == 1)
	{
		for (int k = 1; k < readCount - 2; k++)
		{
			buffer[k] = buffer[k + 1];
		}

		numBytesRead -= 2;
	}

	// proceed buffer
	*MSB = buffer[0];
	*LSB = buffer[1];

	if (buffer[2] == 0x3E)
	{
		*MAXQBL_RET_3EH = MAXQBL_RET_IS_3E;
	}
	else
	{
		*MAXQBL_RET_3EH = MAXQBL_RET_NO_3E;
	}

	// > Result report																														// > Result report
	if (status == HID_SMBUS_SUCCESS)
	{
		return MAXQBL_SUCCESS;
	}
	else
	{
		return MAXQBL_OP_FAILED;
	}
}

/*************************************************************************************
*** Password Lock ***
FLAG = 0
0 Ц The password is unlocked or had a default value; password - protected commands may be used.
1 Ц The password is locked.Password - protected commands may not be used.

STATUS
00 - No Error. The last command completed successfully.
03 - No Password Match. An attempt was made to use a password-protected command without first
matching a valid password. Or, the Password Match command was called with an incorrect
password value.
**************************************************************************************/
unsigned char CConfigWnd::MaxQ_PasswordMatch(unsigned char * password)
{
	BOOL				opened;
	HID_SMBUS_STATUS	status;

	BYTE				writeData[HID_SMBUS_MAX_WRITE_REQUEST_SIZE];

	// Make sure that the device is opened
	BOOL tryDevice = HidSmbus_IsOpened(*m_pHidSmbus, &opened);										// check, return to "opened"

																									// > Check connection
	if (opened == 0)
	{
		return MAXQBL_NO_DEVICE;
	}

	if (tryDevice != HID_SMBUS_SUCCESS)																// check error
	{
		return MAXQBL_DEV_INACCESSIBLE;
	}

	// > Form password command	
	writeData[0] = PASSWORD_MATCH;

	for (int k = 0; k < 32; k++)
	{
		writeData[k + 1] = password[k];
	}

	status = HidSmbus_WriteRequest(*m_pHidSmbus, I2C_DEVICE_SLAVE_ADDR, writeData, 33);

	// > Result report
	if (status == HID_SMBUS_SUCCESS)
	{
		return MAXQBL_SUCCESS;
	}
	else
	{
		return MAXQBL_OP_FAILED;
	}

}

unsigned char CGPIOConfig::MaxQ_VerifyCode(unsigned char LENGTH, unsigned char addrL, unsigned char addrH, unsigned char * v_code)
{
	BOOL				opened;
	HID_SMBUS_STATUS	status;

	BYTE				writeData[HID_SMBUS_MAX_WRITE_REQUEST_SIZE];

	// Make sure that the device is opened
	BOOL tryDevice = HidSmbus_IsOpened(*m_pHidSmbus, &opened);										// check, return to "opened"

																									// > Check connection
	if (opened == 0)
	{
		return MAXQBL_NO_DEVICE;
	}

	if (tryDevice != HID_SMBUS_SUCCESS)																// check error
	{
		return MAXQBL_DEV_INACCESSIBLE;
	}

	// > Form Verify command	
	writeData[0] = VERIFY_CODE;
	writeData[1] = LENGTH;
	writeData[2] = addrL;
	writeData[3] = addrH;

	for (int k = 0; k < LENGTH; k++)
	{
		writeData[k + 4] = v_code[k];
	}

	status = HidSmbus_WriteRequest(*m_pHidSmbus, I2C_DEVICE_SLAVE_ADDR, writeData, 4 + LENGTH);

	// > Result report
	if (status == HID_SMBUS_SUCCESS)
	{
		return MAXQBL_SUCCESS;
	}
	else
	{
		return MAXQBL_OP_FAILED;
	}

}



unsigned char CGPIOConfig::MaxQ_DumpCode(unsigned char addrL, unsigned char addrH,
	unsigned char lengthL, unsigned char lengthH,
	unsigned char * v_code, unsigned char * numBytesRead,
	unsigned char * MAXQBL_RET_3EH, unsigned char MODE)
{
	BOOL				opened;
	HID_SMBUS_STATUS	status;
	HID_SMBUS_S0		status0;

	BYTE				writeData[HID_SMBUS_MAX_WRITE_REQUEST_SIZE];
	BYTE				buffer[HID_SMBUS_MAX_READ_RESPONSE_SIZE];

	BYTE				_numBytesRead = 0;

	// Make sure that the device is opened
	BOOL tryDevice = HidSmbus_IsOpened(*m_pHidSmbus, &opened);										// check, return to "opened"

																									// > Check connection																								
	if (opened == 0)
	{
		return MAXQBL_NO_DEVICE;
	}

	if (tryDevice != HID_SMBUS_SUCCESS)																// check error
	{
		return MAXQBL_DEV_INACCESSIBLE;
	}

	// > Form Dump command	
	writeData[0] = DUMP_CODE;
	writeData[1] = 0x02;

	writeData[2] = addrL;
	writeData[3] = addrH;
	writeData[4] = lengthL;
	writeData[5] = lengthH;

	// > Set Command to device
	status = HidSmbus_WriteRequest(*m_pHidSmbus, I2C_DEVICE_SLAVE_ADDR, writeData, 6);

	// > Get Data from device 
	int readLength = lengthH * 256 + lengthL;
	// For Polling answer
	readLength++;

	// NOTE: MODE = 1 - proceed electrical circuit bug, that appears in: 
	// xx X1 .. .. X2 X3. /X1 - bad byte staffed, X2 - shifted byte from staf, X3 - safe byte reading (for most stability) 
	if (readLength == 1)	// SafeCheck
	{
		MODE = 0;
	}

	if (MODE == 1)
	{
		readLength += 2;
	}

	status = HidSmbus_ReadRequest(*m_pHidSmbus, I2C_DEVICE_SLAVE_ADDR, readLength);	// simple Read /read Length + 1 (Length + "3Eh")
																					//	Sleep(100);	
	Sleep(25);// wait for i2c proceed
	status = HidSmbus_ForceReadResponse(*m_pHidSmbus, readLength);					// Notify device that it should send a read response back
	status = HidSmbus_GetReadResponse(*m_pHidSmbus, &status0, buffer, HID_SMBUS_MAX_READ_RESPONSE_SIZE, &_numBytesRead);	// Wait for a read response

	if (_numBytesRead != readLength)
	{
		return MAXQBL_OP_FAILED;
	}

	if (MODE == 1)
	{
		for (int k = 1; k < readLength - 2; k++)
		{
			buffer[k] = buffer[k + 1];
		}

		_numBytesRead -= 2;
	}

	*numBytesRead = _numBytesRead - 1;

	// > Load Code from buffer
	for (int k = 0; k < _numBytesRead - 1; k++)
	{
		v_code[k] = buffer[k];
	}

	// > Check device answer
	if (buffer[_numBytesRead - 1] == 0x3E)
	{
		*MAXQBL_RET_3EH = MAXQBL_RET_IS_3E;
	}
	else
	{
		*MAXQBL_RET_3EH = MAXQBL_RET_NO_3E;
	}

	// > Result report																														// > Result report
	if (status == HID_SMBUS_SUCCESS)
	{
		return MAXQBL_SUCCESS;
	}
	else
	{
		return MAXQBL_OP_FAILED;
	}
}

unsigned char CGPIOConfig::MaxQ_LoadVerifyCode(unsigned char LENGTH, unsigned char addrL, unsigned char addrH, unsigned char * v_code)
{
	BOOL				opened;
	HID_SMBUS_STATUS	status;

	BYTE				writeData[HID_SMBUS_MAX_WRITE_REQUEST_SIZE];

	// Make sure that the device is opened
	BOOL tryDevice = HidSmbus_IsOpened(*m_pHidSmbus, &opened);										// check, return to "opened"

																									// > Check connection
	if (opened == 0)
	{
		return MAXQBL_NO_DEVICE;
	}

	if (tryDevice != HID_SMBUS_SUCCESS)																// check error
	{
		return MAXQBL_DEV_INACCESSIBLE;
	}

	// > Form Load & Verify command	
	writeData[0] = LOAD_VERIFY_CODE;
	writeData[1] = LENGTH;
	writeData[2] = addrL;
	writeData[3] = addrH;

	for (int k = 0; k < LENGTH; k++)
	{
		writeData[k + 4] = v_code[k];
	}

	status = HidSmbus_WriteRequest(*m_pHidSmbus, I2C_DEVICE_SLAVE_ADDR, writeData, 4 + LENGTH);

	// > Result report
	if (status == HID_SMBUS_SUCCESS)
	{
		return MAXQBL_SUCCESS;
	}
	else
	{
		return MAXQBL_OP_FAILED;
	}

}



int CGPIOConfig::LoadVerifyCode_HexFile(int *errLine, int *errSymb)
{
	// init locals
	*errLine = 0;						// error line. Defined exit-line number when load had been failed. / -1 = can't open file 
	*errSymb = 0;						// error symbol. Defined exit-symbol number when load had been failed.

	unsigned char errCode;				// error Code for unsafe operations
	unsigned char devicePolling;
	
	// output text log
	UpdateData(TRUE);
	m_TextOutput.Truncate(0);

	m_TextOutput += "*** Load Program operation *** \r\n";
	UpdateData(FALSE);

	//MessageBox(_T("ok."), _T("ok"), MB_ICONERROR | MB_OK);
	UpdateData(TRUE);
	m_TextOutput += "# initialization....... \r\n";
	
	UpdateData(FALSE);
	
	// > interr
	// raise(SIGINT);

	CEdit* pEdit = (CEdit*)GetDlgItem(IDC_EDIT_OUTPUT);
	pEdit->SetActiveWindow();

	m_TextOutput += "> checking ";
	m_TextOutput.Append(m_TextFilename);
	m_TextOutput += "file \r\n";
	UpdateData(FALSE);

	

	FILE *fp;

	// [#] FILE OP CHECK
	// > Define Filename
	char *file_name = "test.hex";

	// > Open File try
	errno_t err;
	err = fopen_s(&fp, file_name, "r");

	if (err != 0)
	{	// "can't open file" 
		*errLine = -1;
		return MAXQBL_OP_FAILED;
	}

	// output
	UpdateData(TRUE);
	m_TextOutput += "- File opened successfull. \r\n";
	UpdateData(FALSE);

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
				*errLine = -2;
				return MAXQBL_OP_FAILED;
				break;
			}
		}
	}

	// output
	UpdateData(TRUE);
	m_TextOutput.AppendFormat(_T("%04X"), fileLineNum);
	m_TextOutput += " lines to proceed \r\n";
	UpdateData(FALSE);		

	// reopen FILE
	fclose(fp);

	err = fopen_s(&fp, file_name, "r");

	if (err != 0)
	{	// "can't open file" 
		*errLine = -1;
		return MAXQBL_OP_FAILED;
	}

	// progress component
	m_cPB_OP.SetPos(0);
	m_cPB_OP.SetStep(1);

	// progress bar
	double fstepTrigger = fileLineNum;
	fstepTrigger /= 100;
	double stepValue = 0;


	// output
	UpdateData(TRUE);
	m_TextOutput += "> Open connection with DS4830a... \r\n";
	UpdateData(FALSE);

	// [#] DEVICE OP CHECK
	// > Enter Boot	
	errCode = MaxQ_EnterBoot();
	if (errCode != MAXQBL_SUCCESS)
	{
		*errLine = -3;
		*errSymb = errCode;
		return MAXQBL_OP_FAILED;
	}

	UpdateData(TRUE);
	m_TextOutput += "- Enter boot... OK \r\n";
	UpdateData(FALSE);

	// > Soft Reset
	errCode = MaxQ_SoftReset();
	if (errCode != MAXQBL_SUCCESS)
	{
		*errLine = -4;
		*errSymb = errCode;
		return MAXQBL_OP_FAILED;
	}

	// Reset delay
	Sleep(50);

	// > Poll 3Eh
	errCode = MaxQ_Poll3Eh(&devicePolling);
	if (errCode != MAXQBL_SUCCESS)
	{
		*errLine = -5;
		*errSymb = errCode;
		return MAXQBL_OP_FAILED;
	}

	if (devicePolling != MAXQBL_RET_IS_3E)
	{
		*errLine = -6;
		return MAXQBL_OP_FAILED;
	}

	UpdateData(TRUE);
	m_TextOutput += "- Reseting... OK \r\n";
	UpdateData(FALSE);

	// > Master Erase
	errCode = MaxQ_MasterErase();
	if (errCode != MAXQBL_SUCCESS)
	{
		*errLine = -7;
		*errSymb = errCode;
		return MAXQBL_OP_FAILED;
	}

	// Master Erase delay
	Sleep(100);

	// > Poll 3Eh
	errCode = MaxQ_Poll3Eh(&devicePolling);
	if (errCode != MAXQBL_SUCCESS)
	{
		*errLine = -8;
		*errSymb = errCode;
		return MAXQBL_OP_FAILED;
	}

	if (devicePolling != MAXQBL_RET_IS_3E)
	{
		*errLine = -9;
		return MAXQBL_OP_FAILED;
	}

	m_TextOutput += "- Erasing memory... OK \r\n";

	// SOLUTION: while CP2112 do not provide i2c-restart message (2bytes-CMD bugged), 2B-CMD was deactevated.
	// > Get Status
	/*	unsigned char MSB, LSB;
	errCode = MaxQ_CMD_2B(GET_STATUS, &MSB, &LSB, &devicePolling, 1);
	if (errCode != MAXQBL_SUCCESS)
	{
	*errLine = -9;
	*errSymb = errCode;
	return MAXQBL_OP_FAILED;
	}

	if ((LSB & 0x07) != 0)		// 8 - erase failure, 0 - ok.
	{
	*errLine = -10;
	return MAXQBL_OP_FAILED;
	}
	*/

	// [#] PROCEED DUMP OP	

	// > Proceed IntelHex File	
	// NOTE: CMD has format:	:			10			01 00			00		01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F 10		40			\n		.. 0D 0A
	//					Descr	Delimeter	CodeLength	FlashStartAddr	Type	Code Data (opcode hex)								Checksum	EOL		.. EOF
	//						k	00			00-02		03-06			07-08	09-[08 + 2*n]										[10 + 2*n]-[11 + 2*n]

	m_TextOutput += "> Start Loading... \r\n";

	const unsigned int SYMBOLS_COUNT_DLMT = 1;
	const unsigned int SYMBOLS_COUNT_CODE = 2;
	const unsigned int SYMBOLS_COUNT_ADDR = 4;
	const unsigned int SYMBOLS_COUNT_TYPE = 2;
	const unsigned int SYMBOLS_COUNT_CHKS = 2;

	unsigned int l = 0;					// file line numeration
	unsigned int k = 0;					// file line symbol numeration
	unsigned int n = 0;					// line opcode bytes count

	unsigned char flashStartAddr[SYMBOLS_COUNT_ADDR / 2];	// flash start addr number
	unsigned char T;					// line type	//00 - data, 01 - end, 04 - ext
	unsigned char CHK;					// checksum

	unsigned char v_symb[32];			// symbol buffer vector
	unsigned char v_code[16];			// code data buffer vector
	std::vector<BYTE> vByteVector;		// temp conversion buffer	

	int c;								// symbol code (text letter code)

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
										// NOP						 /init realised in ':' case 
			break;

		case ':':						// [#] new Intel Hex op-line started
			k = SYMBOLS_COUNT_DLMT;

			break;

		default:						// [#] all Common text letter symbols
										// [#] Parce Intel Hex line
			if (k < SYMBOLS_COUNT_DLMT + SYMBOLS_COUNT_CODE)						// %VALUE: OPCODE: BYTE NUMBER	(n)
			{
				v_symb[k - SYMBOLS_COUNT_DLMT] = c;

				if (k == SYMBOLS_COUNT_CODE)										// %VALUE: end of current val
				{
					// Hex_Letter To Byte convertion
					vByteVector.clear();
					unsigned char errCode = 0;
					vByteVector = StrHexToByte(v_symb, 1, &errCode);

					if (errCode != 0)
					{	// abort procedure
						*errLine = l;
						*errSymb = k;

						return MAXQBL_OP_FAILED;
					}

					// Get line opcode bytes number (n)
					n = vByteVector[0];

				} // end val
			} // k < SYMBOLS_COUNT_DLMT + SYMBOLS_COUNT_CODE
			else
				if (k < SYMBOLS_COUNT_DLMT + SYMBOLS_COUNT_CODE + SYMBOLS_COUNT_ADDR)	// %VALUE: OPCODE: FLASH START	(flashStartAddr[SYMBOLS_COUNT_ADDR /2])
				{
					v_symb[k - SYMBOLS_COUNT_DLMT - SYMBOLS_COUNT_CODE] = c;

					if (k == SYMBOLS_COUNT_CODE + SYMBOLS_COUNT_ADDR)					// %VALUE: end of current val
					{
						// Hex_Letter To Byte convertion
						vByteVector.clear();
						unsigned char errCode = 0;
						vByteVector = StrHexToByte(v_symb, SYMBOLS_COUNT_ADDR / 2, &errCode);

						if (errCode != 0)
						{	// abort procedure
							*errLine = l;
							*errSymb = k;

							return MAXQBL_OP_FAILED;
						}

						// Get line opcode Address info (flashStartAddr[0] - AddrH, flashStartAddr[1] - AddrL)
						for (int i = 0; i < SYMBOLS_COUNT_ADDR / 2; i++)
						{
							flashStartAddr[i] = vByteVector[i];
						}

					} // end val

				} // k < SYMBOLS_COUNT_DLMT + SYMBOLS_COUNT_CODE + SYMBOLS_COUNT_ADDR
				else
					if (k < SYMBOLS_COUNT_DLMT + SYMBOLS_COUNT_CODE + SYMBOLS_COUNT_ADDR + SYMBOLS_COUNT_TYPE)	// %VALUE: LINE TYPE	(T)
					{
						v_symb[k - SYMBOLS_COUNT_DLMT - SYMBOLS_COUNT_CODE - SYMBOLS_COUNT_ADDR] = c;

						if (k == SYMBOLS_COUNT_CODE + SYMBOLS_COUNT_ADDR + SYMBOLS_COUNT_TYPE)					// %VALUE: end of current val
						{
							// Hex_Letter To Byte convertion
							vByteVector.clear();
							unsigned char errCode = 0;
							vByteVector = StrHexToByte(v_symb, SYMBOLS_COUNT_TYPE / 2, &errCode);

							if (errCode != 0)
							{	// abort procedure
								*errLine = l;
								*errSymb = k;

								return MAXQBL_OP_FAILED;
							}

							// Get line opcode bytes number (flashStartAddr[2])
							T = vByteVector[0];

							// > Proceed EndLine of HexFile 
							if (T == 0x01)
							{
								ACT = FALSE;

								// progress bar
								stepValue += 1;
								if (stepValue > fstepTrigger)
								{
									stepValue -= fstepTrigger;

									//UpdateData(TRUE);
									m_cPB_OP.StepIt();
									//UpdateData(FALSE);

									// output
									m_TextOutput += " Loading operation successfull! \r\n";
									m_TextOutput += " *** END *** \r\n \r\n";
								}
							}

						} // end val
					} // k < SYMBOLS_COUNT_DLMT + SYMBOLS_COUNT_CODE + SYMBOLS_COUNT_ADDR + SYMBOLS_COUNT_TYPE
					else
						if (k < SYMBOLS_COUNT_DLMT + SYMBOLS_COUNT_CODE + SYMBOLS_COUNT_ADDR + SYMBOLS_COUNT_TYPE + n * 2)	// %VALUE: OPCODE: OP DATABYTE	(v_code[16])
						{
							v_symb[k - SYMBOLS_COUNT_DLMT - SYMBOLS_COUNT_CODE - SYMBOLS_COUNT_ADDR - SYMBOLS_COUNT_TYPE] = c;

							if (k == SYMBOLS_COUNT_CODE + SYMBOLS_COUNT_ADDR + SYMBOLS_COUNT_TYPE + n * 2)					// %VALUE: end of current val
							{
								// Hex_Letter To Byte convertion
								vByteVector.clear();
								unsigned char errCode = 0;
								vByteVector = StrHexToByte(v_symb, n, &errCode);

								if (errCode != 0)
								{	// abort procedure
									*errLine = l;
									*errSymb = k;

									return MAXQBL_OP_FAILED;
								}

								// Get line opcode bytes number (flashStartAddr[2])
								for (int i = 0; i < n; i++)
								{
									v_code[i] = vByteVector[i];
								}

							} // end val
						} // k < SYMBOLS_COUNT_DLMT + SYMBOLS_COUNT_CODE + SYMBOLS_COUNT_ADDR + SYMBOLS_COUNT_TYPE + n * 2
						else
							if (k < SYMBOLS_COUNT_DLMT + SYMBOLS_COUNT_CODE + SYMBOLS_COUNT_ADDR + SYMBOLS_COUNT_TYPE + n * 2 + SYMBOLS_COUNT_CHKS)	// %VALUE: CHECKSUM	(CHK])
							{
								v_symb[k - SYMBOLS_COUNT_DLMT - SYMBOLS_COUNT_CODE - SYMBOLS_COUNT_ADDR - SYMBOLS_COUNT_TYPE - n * 2] = c;

								if (k == SYMBOLS_COUNT_CODE + SYMBOLS_COUNT_ADDR + SYMBOLS_COUNT_TYPE + n * 2 + SYMBOLS_COUNT_CHKS)					// %VALUE: end of current val
								{
									// Hex_Letter To Byte convertion
									vByteVector.clear();
									unsigned char errCode = 0;
									vByteVector = StrHexToByte(v_symb, SYMBOLS_COUNT_CHKS / 2, &errCode);

									if (errCode != 0)
									{	// abort procedure
										*errLine = l;
										*errSymb = k;

										return MAXQBL_OP_FAILED;
									}

									// Get line opcode bytes number (flashStartAddr[2])
									// TODO: calculate self CHK and compare CHK's
									CHK = vByteVector[0];


									// > Load OPCODE line to Device & Verify OPCODE Load result
									errCode = MaxQ_LoadVerifyCode(n, flashStartAddr[1], flashStartAddr[0], v_code);
									Sleep(25);

									// > Poll 3Eh
									errCode = MaxQ_Poll3Eh(&devicePolling);

									if (devicePolling != MAXQBL_RET_IS_3E)
									{	// device fail
										*errLine = l;	// TODO: ? define this type of error to user
										return MAXQBL_OP_FAILED;
									}

									// progress bar
									stepValue += 1;
									if (stepValue > fstepTrigger)
									{
										stepValue -= fstepTrigger;

										//UpdateData(TRUE);
										m_cPB_OP.StepIt();
										//UpdateData(FALSE);
									}

									// output
									m_TextOutput.AppendFormat(_T("%04X"), l);
									m_TextOutput += " OPCODE line loading... OK \r\n";

									//
									// > Check Status of Verification
									/*
									errCode = MaxQ_CMD_2B(GET_STATUS, &MSB, &LSB, &devicePolling, 1);

									if (devicePolling != MAXQBL_RET_IS_3E)
									{	// device fail
									*errLine = l;	// TODO: ? define this type of error to user
									return MAXQBL_OP_FAILED;
									}

									LSB = LSB & 0x7F;
									if (LSB != 0)
									{
									*errLine = l;
									return MAXQBL_OP_FAILED;
									}
									*/
								} // end val
							} // k < SYMBOLS_COUNT_DLMT + SYMBOLS_COUNT_CODE + SYMBOLS_COUNT_ADDR + SYMBOLS_COUNT_TYPE + n * 2 + SYMBOLS_COUNT_CHKS


							  // Step next symbol
			k++;

			break;
		} // switch

	} // while ACT

	  // > Close Intel Hex File
	fclose(fp);

	// progress bar
	m_cPB_OP.SetPos(0);

	return MAXQBL_SUCCESS;
}

// Dump code from device /Maxim DS4830a type/ to file /Intel Hex format/
int CConfigWnd::DumpCode_HexFile(int *errLine, int *errSymb)
{
	// init locals
	*errLine = 0;						// error line. Defined exit-line number when load had been failed.		
										// NOTE:							/fail:	-1 = open file; -2 = Enter Boot; -3 = Soft Reset; -4 = Poll OP _Rst; -5 = PollBack ACK _Rst; 
										//											-6 = Pass Match OP; -7 = Poll OP _PWD; -8 = PollBack ACK _PWD; -9 = GetSTAT _PWD; -10 = pass unlock
	*errSymb = 0;						// error symbol. Defined exit-symbol number when load had been failed.	/ when l < 0  = errType 
	unsigned char errCode;
	unsigned char devicePolling;

	FILE *fp;

	// [#] FILE OP CHECK
	// > Define Filename
	char* file_name = "test.hex";

	// > Try to Create File
	errno_t err;
	err = fopen_s(&fp, file_name, "w");

	if (err != 0)
	{	// "can't write file"  
		*errLine = -1;
		return MAXQBL_OP_FAILED;
	}

	// [#] DEVICE OP CHECK
	// > Enter Boot	
	errCode = MaxQ_EnterBoot();
	if (errCode != MAXQBL_SUCCESS)
	{
		*errLine = -2;
		*errSymb = errCode;
		return MAXQBL_OP_FAILED;
	}

	// > Soft Reset
	errCode = MaxQ_SoftReset();
	if (errCode != MAXQBL_SUCCESS)
	{
		*errLine = -3;
		*errSymb = errCode;
		return MAXQBL_OP_FAILED;
	}

	// > Poll 3Eh
	errCode = MaxQ_Poll3Eh(&devicePolling);
	if (errCode != MAXQBL_SUCCESS)
	{
		*errLine = -4;
		*errSymb = errCode;
		return MAXQBL_OP_FAILED;
	}

	if (devicePolling != MAXQBL_RET_IS_3E)
	{
		*errLine = -5;
		return MAXQBL_OP_FAILED;
	}

	// > Password Match
	errCode = MaxQ_PasswordMatch(passData);
	if (errCode != MAXQBL_SUCCESS)
	{
		*errLine = -6;
		*errSymb = errCode;
		return MAXQBL_OP_FAILED;
	}

	Sleep(50);

	// > Poll 3Eh
	errCode = MaxQ_Poll3Eh(&devicePolling);
	if (errCode != MAXQBL_SUCCESS)
	{
		*errLine = -7;
		*errSymb = errCode;
		return MAXQBL_OP_FAILED;
	}

	if (devicePolling != MAXQBL_RET_IS_3E)
	{
		*errLine = -8;
		return MAXQBL_OP_FAILED;
	}

	// > Get Status
	unsigned char MSB, LSB;
	errCode = MaxQ_CMD_2B(GET_STATUS, &MSB, &LSB, &devicePolling, 1);
	if (errCode != MAXQBL_SUCCESS)
	{
		*errLine = -9;
		*errSymb = errCode;
		return MAXQBL_OP_FAILED;
	}

	if ((MSB & 0x01) != 0)		// 0 = Pass Unlocked
	{
		*errLine = -10;
		return MAXQBL_OP_FAILED;
	}


	// [#] PROCEED DUMP OP	
	// NOTE: CMD has format:	:			10			01 00			00		01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F 10		40			\n		.. 0D 0A
	//					Descr	Delimeter	CodeLength	FlashStartAddr	Type	Code Data (opcode hex)								Checksum	EOL		.. EOF
	//						k	00			00-02		03-06			07-08	09-[08 + 2*n]										[10 + 2*n]-[11 + 2*n]

	// format const
	const unsigned int SYMBOLS_COUNT_DLMT = 1;
	const unsigned int SYMBOLS_COUNT_CODE = 2;
	const unsigned int SYMBOLS_COUNT_ADDR = 4;
	const unsigned int SYMBOLS_COUNT_TYPE = 2;
	const unsigned int SYMBOLS_COUNT_CHKS = 2;

	// device dump stricts const
	const unsigned int CODE_FLSH_LENGTH = 0xFFFF;		// TODO: define from status _CODE_LENGTH cmd
	const unsigned int CODE_READ_LENGTH = 16;			// TODO: define as function parameter

														// line type format const
	const CString DELIMETER(":");
	const CString HEX_FORMAT_TYPE_USUAL("00");
	const CString HEX_FORMAT_TYPE_END("01");
	const CString HEX_FORMAT_END_STRING("00000001FF");

	// in-cycle
	unsigned int l = 0;					// file line numeration
	unsigned int k = 0;					// file line symbol numeration
	unsigned int n = 0;					// line opcode bytes count

										// string format
	unsigned char flashStartAddr[SYMBOLS_COUNT_ADDR / 2];	// flash start addr number
	unsigned char T;					// line type	//00 - data, 01 - end, 04 - ext
	unsigned char CHK;					// checksum

										// vector buf
	unsigned char v_symb[32]; // = { '0','9','A','0','C','3','B','7','F','F' };			// symbol buffer vector
	unsigned char v_code[16]; // = { 0, 11, 32, 12, 58, 218, 0, 11, 49, 12, 0, 0, 0, 0, 0, 0 };			// code data buffer vector 
	std::vector<BYTE> vByteVector;		// temp conversion buffer

										// progress component
	m_cPB_OP.SetPos(0);
	m_cPB_OP.SetStep(1);

	// progress bar
	double fstepTrigger = CODE_FLSH_LENGTH / 100;
	fstepTrigger /= CODE_READ_LENGTH;
	double stepValue = 0;

	// > Init locals
	for (int i = 0; i < SYMBOLS_COUNT_ADDR / 2; i++)
	{
		flashStartAddr[i] = 0;
	}

	unsigned char numBytesRead = 0;
	CString hexFormatLine("");

	// > Proceed device flash memory
	bool ACT = 1;						// writing file op is active
	while (ACT)
	{
		if (256 * flashStartAddr[1] + flashStartAddr[0] < CODE_FLSH_LENGTH)
		{
			// progress bar
			stepValue += 1;
			if (stepValue > fstepTrigger)
			{
				stepValue -= fstepTrigger;

				//				UpdateData(TRUE);
				m_cPB_OP.StepIt();
				//				UpdateData(FALSE);
			}

			// > Dump OPCODE Line (Bytes) = 8[count] x 1[8bit] x 2[MCU architect] = 16 Bytes
			// dump op proc (addr[0], addr[1], 16)

			errCode = MaxQ_DumpCode(flashStartAddr[0], flashStartAddr[1], CODE_READ_LENGTH, 0, v_code, &numBytesRead, &devicePolling, 1);
			if (errCode != MAXQBL_SUCCESS)
			{	// OP fail
				*errLine = l;
				return MAXQBL_OP_FAILED;
			}

			if (devicePolling != MAXQBL_RET_IS_3E)
			{	// device fail
				*errLine = l;	// TODO: ? define this type of error to user
				return MAXQBL_OP_FAILED;
			}

			// > Proceed OPCODE data
			// init CHK
			CHK = 0;

			// find zeros to cut in forehand
			k = 0;
			while ((v_code[CODE_READ_LENGTH - 1 - k] == 0xFF) && (k < CODE_READ_LENGTH))
			{
				k++;
			}

			// skip writing empty OPCODE line, trim empty end of OPCODE line
			if (k != CODE_READ_LENGTH)
			{
				// > Form String Hex lines

				// init locals
				hexFormatLine.Truncate(0);
				unsigned char errCode;

				hexFormatLine.Append(DELIMETER);											// [#] X . xx . xxxx . xx . xx xx xx xx xx .. xx . xx

																							// prepare line OPCODE bytes count
				v_symb[0] = CODE_READ_LENGTH - k;

				CString strHexOPCODECount = ByteToStrHex(v_symb, 1, &errCode);
				hexFormatLine.Append(strHexOPCODECount);									// [#] x . XX . xxxx . xx . xx xx xx xx xx .. xx . xx

				CHK += v_symb[0];	// Checksum

									// prepare line ADDR flash mem value
				v_symb[0] = flashStartAddr[1];
				v_symb[1] = flashStartAddr[0];

				CString strHexAddr = ByteToStrHex(v_symb, 2, &errCode);
				hexFormatLine.Append(strHexAddr);											// [#] x . xx . XXXX . xx . xx xx xx xx xx .. xx . xx

				CHK += v_symb[0];	// Checksum
				CHK += v_symb[1];

				// prepare line TYPE value
				hexFormatLine.Append(HEX_FORMAT_TYPE_USUAL);								// [#] x . xx . xxxx . XX . xx xx xx xx xx .. xx . xx

																							// prepare line OPCODE byte consequence
				CString strHexOPCODEline = ByteToStrHex(v_code, CODE_READ_LENGTH - k, &errCode);
				hexFormatLine.Append(strHexOPCODEline);										// [#] x . xx . xxxx . xx . XX XX XX XX XX .. XX . xx

				for (int i = 0; i < CODE_READ_LENGTH - k; i++)
				{
					CHK += v_code[i];	// Checksum
				}

				// prepare line CHK value
				CHK = 256 - CHK;
				v_symb[0] = CHK;

				CString strHexCHK = ByteToStrHex(v_symb, 1, &errCode);
				hexFormatLine.Append(strHexCHK);											// [#] x . xx . xxxx . xx . xx xx xx xx xx .. xx . XX		

			} // k != CODE_READ_LENGTH

			  // > Update ADDR information
			  // SOLUTION: accumulate Low and High registers of Addr /0 = Low; 1 = High
			if (flashStartAddr[0] + CODE_READ_LENGTH > 0xFF)
			{
				flashStartAddr[0] = CODE_READ_LENGTH - (256 - flashStartAddr[0]);
				flashStartAddr[1] ++;

				if (flashStartAddr[1] == 0xFF)		// x16 memory ends
				{
					// > Form String Hex lines
					// init locals
					hexFormatLine.Truncate(0);

					hexFormatLine.Append(DELIMETER);												// [#] X . xx . xxxx . xx . xx xx xx xx xx .. xx . xx
					hexFormatLine.Append(HEX_FORMAT_END_STRING);									// [#] x . XX . XXXX . XX . XX XX XX XX XX .. XX . XX

																									// cancel another step
					ACT = 0;
				}
			}
			else
			{
				flashStartAddr[0] += CODE_READ_LENGTH;
			}

		} // 256 * flashStartAddr[1] + flashStartAddr[0] < CODE_FLSH_LENGTH
		else // end line
		{
			// > Form String Hex lines
			// init locals
			hexFormatLine.Truncate(0);

			hexFormatLine.Append(DELIMETER);												// [#] X . xx . xxxx . xx . xx xx xx xx xx .. xx . xx
			hexFormatLine.Append(HEX_FORMAT_END_STRING);									// [#] x . XX . XXXX . XX . XX XX XX XX XX .. XX . XX

																							// cancel another step
			ACT = 0;
		}

		// prepare to write line to FILE
		// TODO: probably in future FILE may has string length wider than 1024 symbols
		// 'k' (number of OPCODE symbols) has 0..255 now
		if ((k != CODE_READ_LENGTH) || (ACT == 0))	// block empty lines. (Write not empty line and end line).
		{
			if (hexFormatLine.GetLength() < 256)
			{
				char c[256];

				for (int i = 0; i < hexFormatLine.GetLength(); i++)
				{
					c[i] = hexFormatLine[i];
				}

				int finalizeStr = hexFormatLine.GetLength();
				c[finalizeStr] = '\n';
				c[finalizeStr + 1] = '\0';

				// write line to FILE
				fputs(c, fp);
			}
			else // ** reserved for future work
				if (hexFormatLine.GetLength() < 1024)
				{
					char c[1024];
					memcpy(c, hexFormatLine, hexFormatLine.GetLength());

					// write line to FILE
					fputs(c, fp);
				}
				else
				{	// abort: too wide string line 
					*errLine = l;
					return MAXQBL_OP_FAILED;
				}

			// line counter
			l++;
		}


	} // ACT

	  // close FILE
	fclose(fp);

	m_cPB_OP.SetPos(0);

	return MAXQBL_SUCCESS;
}

/////////////////////////////////////////////////////////////////////////////
// Input Element event procedures
/////////////////////////////////////////////////////////////////////////////
void CConfigWnd::OnBnClickedButtonResetLatch()
{
	//

}


void CGPIOConfig::OnTimer(UINT_PTR nIDEvent)
{
	LatchController();

	CDialogEx::OnTimer(nIDEvent);
}


BOOL CGPIOConfig::OnInitDialog()
{
	CDialogEx::OnInitDialog();
// TODO: 

	//statusbar
	InitStatusBar();
	//def
	SetDefaults();
	//device init write
	//SetFromDevice();






	return TRUE;  // return TRUE unless you set the focus to a control
				  // Исключение: страница свойств OCX должна возвращать значение FALSE
}


void CGPIOConfig::OnBnClickedCheckLatch()
{



	/*
	// TODO: 
	const MSG* pMsg = GetCurrentMessage();

	if (pMsg->message == WM_COMMAND)
	{
		int infomsg = 0;	//info

							// Get control ID
		int nID = (int)pMsg->wParam;

		switch (nID)	//button clicked
		{
		case IDC_CHECK_LATCH_GPIO_5:	//SFP
			if ((this->m_gpio_latch & HID_SMBUS_MASK_GPIO_2) == 0)	//SFP plugged
			{
				if ((this->m_gpio_latch & HID_SMBUS_MASK_GPIO_5) != 0)	//SFP active
				{
					//deactivating
					HID_SMBUS_STATUS statusW = HidSmbus_WriteLatch(*m_pHidSmbus, 0, HID_SMBUS_MASK_GPIO_5);

					//save new status
				//	this->m_gpio_latch = (this->m_gpio_latch & 0xDF);
					//reflect in controls
					SetLatchValue(this->m_gpio_latch);

					infomsg = 12;
				}
				else													//SFP none
				{
					//check multiple plugging
					BYTE bMultiple = FALSE;
					if ((HID_SMBUS_MASK_GPIO_3 & this->m_gpio_latch) == 0) { bMultiple = TRUE; }
					else
					{
						if ((HID_SMBUS_MASK_GPIO_4 & this->m_gpio_latch) == 0) { bMultiple = TRUE; }
					}

					if (bMultiple)	//other devise is active
					{
						infomsg = 40;
					}
					else
					{
						//activating
						HID_SMBUS_STATUS statusW = HidSmbus_WriteLatch(*m_pHidSmbus, 255, HID_SMBUS_MASK_GPIO_5);

						//save new status
				//		this->m_gpio_latch = (this->m_gpio_latch | HID_SMBUS_MASK_GPIO_5);
						//reflect in controls
						SetLatchValue(this->m_gpio_latch);

						infomsg = 11;
					}
				}
			}
			break;

		case IDC_CHECK_LATCH_GPIO_6:	//SFP
			if ((this->m_gpio_latch & HID_SMBUS_MASK_GPIO_3) == 0)	//SFP plugged
			{
				if ((this->m_gpio_latch & HID_SMBUS_MASK_GPIO_6) != 0)	//SFP active
				{
					//deactivating
					HID_SMBUS_STATUS statusW = HidSmbus_WriteLatch(*m_pHidSmbus, 0, HID_SMBUS_MASK_GPIO_6);

					//save new status
				//	this->m_gpio_latch = (this->m_gpio_latch & 0xBF);
					//reflect in controls
					SetLatchValue(this->m_gpio_latch);

					infomsg = 22;
				}
				else													//SFP none
				{
					//check multiple plugging
					BYTE bMultiple = FALSE;
					if ((HID_SMBUS_MASK_GPIO_2 & this->m_gpio_latch) == 0) { bMultiple = TRUE; }
					else
					{
						if ((HID_SMBUS_MASK_GPIO_4 & this->m_gpio_latch) == 0) { bMultiple = TRUE; }
					}

					if (bMultiple)	//other devise is active
					{
						infomsg = 40;
					}
					else
					{
						//activating
						HID_SMBUS_STATUS statusW = HidSmbus_WriteLatch(*m_pHidSmbus, 255, HID_SMBUS_MASK_GPIO_6);

						//save new status
					//	this->m_gpio_latch = (this->m_gpio_latch | HID_SMBUS_MASK_GPIO_6);
						//reflect in controls
						SetLatchValue(this->m_gpio_latch);

						infomsg = 21;
					}
				}
			}
			break;

		case IDC_CHECK_LATCH_GPIO_7:	//SFP
			if ((this->m_gpio_latch & HID_SMBUS_MASK_GPIO_4) == 0)	//SFP plugged
			{
				if ((this->m_gpio_latch & HID_SMBUS_MASK_GPIO_7) != 0)	//SFP active
				{
					//deactivating
					HID_SMBUS_STATUS statusW = HidSmbus_WriteLatch(*m_pHidSmbus, 0, HID_SMBUS_MASK_GPIO_7);

					//save new status
				//	this->m_gpio_latch = (this->m_gpio_latch & 0x7F);
					//reflect in controls
					SetLatchValue(this->m_gpio_latch);

					infomsg = 32;
				}
				else													//SFP none
				{
					//check multiple plugging
					BYTE bMultiple = FALSE;
					if ((HID_SMBUS_MASK_GPIO_2 & this->m_gpio_latch) == 0) { bMultiple = TRUE; }
					else
					{
						if ((HID_SMBUS_MASK_GPIO_3 & this->m_gpio_latch) == 0) { bMultiple = TRUE; }
					}

					if (bMultiple)	//other devise is active
					{
						infomsg = 40;
					}
					else
					{
						//activating
						HID_SMBUS_STATUS statusW = HidSmbus_WriteLatch(*m_pHidSmbus, 255, HID_SMBUS_MASK_GPIO_7);

						//save new status
				//		this->m_gpio_latch = (this->m_gpio_latch | HID_SMBUS_MASK_GPIO_7);
						//reflect in controls
						SetLatchValue(this->m_gpio_latch);

						infomsg = 31;
					}
				}
			}
			break;

		default:
			break;
		}

		if (infomsg != 0)
		{
			CString statusText;

			switch (infomsg) {
			case 11: statusText = "SFP power on!";
				//AfxMessageBox((CString)"SFP has plagged in!", MB_OK, MB_ICONASTERISK); 
				break;

			case 12: statusText = "SFP power off!";
				//AfxMessageBox((CString)"SFP has plagged out!", MB_OK, MB_ICONASTERISK);
				break;

			case 21: statusText = "XFP power on!";
				//AfxMessageBox((CString)"XFP has plagged in!", MB_OK, MB_ICONASTERISK);
				break;

			case 22: statusText = "XFP power off!";
				//AfxMessageBox((CString)"XFP has plagged out!", MB_OK, MB_ICONASTERISK);
				break;

			case 31: statusText = "QSFP power on!";
				//AfxMessageBox((CString)"QSFP has plagged in!", MB_OK, MB_ICONASTERISK);
				break;

			case 32: statusText = "QSFP power off!";
				//AfxMessageBox((CString)"QSFP has plagged out!", MB_OK, MB_ICONASTERISK);

			case 40: statusText = "Another device has already activated!";
				//AfxMessageBox((CString)"QSFP has plagged out!", MB_OK, MB_ICONASTERISK);
				break;
			} // case

			  //status info
			SetStatusText1(statusText);

		} // info
	} //pMsg->message == WM_COMMAND 

	*/
}



void CGPIOConfig::OnBnClickedButton2()
{
	// TODO: Add your control notification handler code here
	// TODO: добавьте свой код обработчика уведомлений
	int					errCode = 0;
	BOOL				opened;

	HID_SMBUS_STATUS	status;
	HID_SMBUS_S0		status0;

	BYTE				writeData[HID_SMBUS_MAX_WRITE_REQUEST_SIZE];
	BYTE				targetAddress[HID_SMBUS_MAX_TARGET_ADDRESS_SIZE];
	BYTE				buffer[HID_SMBUS_MAX_READ_RESPONSE_SIZE];

	BYTE				numBytesToWrite = 0;
	BYTE				numBytesRead = 0;

	// # Perforn simple 2byte command

	/*
	// Make sure that the device is opened
	BOOL tryDevice = HidSmbus_IsOpened(*m_pHidSmbus, &opened);										// check, return to "opened"

	if (tryDevice != HID_SMBUS_SUCCESS)																// check error
	{	// abort / device inaccesible
	errCode = 1;
	exit;
	}


	// > Enter Boot
	writeData[0] = SET_I2C_SPE;			// Send Slave Boot Address + Write + SPE_flag cmd
	status = HidSmbus_WriteRequest(*m_pHidSmbus, I2C_BOOTLOADER_SLAVE_ADDR, writeData, 1);
	*/

	// > Enter Boot	
	errCode = MaxQ_EnterBoot();

	// > Soft Reset
	errCode = MaxQ_SoftReset();

	// > Poll 3Eh
	unsigned char devicePolling;
	errCode = MaxQ_Poll3Eh(&devicePolling);

	// > Password Match
	errCode = MaxQ_PasswordMatch(passData);
	Sleep(50);
	// > Poll 3Eh
	errCode = MaxQ_Poll3Eh(&devicePolling);
	//	Sleep(100);

	/*	// > Get Status
	unsigned char MSB, LSB;
	errCode = MaxQ_CMD_2B(GET_CODE_SIZE, &MSB, &LSB, &devicePolling, 1);


	// test output
	CString receiveString;
	receiveString.AppendFormat(_T("%02X "), MSB);
	receiveString.AppendFormat(_T("%02X "), LSB);
	receiveString.AppendFormat(_T("%02X "), devicePolling);
	*/
	// > Verify Code
	unsigned char v_code[8] = { 0x00, 0x0B, 0x20, 0x0C, 0x06, 0x08, 0x29, 0xE9 };

	errCode = MaxQ_VerifyCode(5, 0x00, 0x00, v_code);


	Sleep(50);
	// > Poll 3Eh
	errCode = MaxQ_Poll3Eh(&devicePolling);

	CString receiveString;
	unsigned char LSB;
	unsigned char MSB;

	int do_valid = 1;
	int tryCount = 0;
	/*	while (do_valid)
	{
	// > Get Status
	errCode = MaxQ_CMD_2B(GET_STATUS, &MSB, &LSB, &devicePolling, 1);

	if ((MSB > 0x07) || (tryCount > 10))
	{
	do_valid = 0;
	}
	}

	if (tryCount > 10)
	{
	// read failure
	}
	*/
	errCode = MaxQ_CMD_2B(GET_STATUS, &MSB, &LSB, &devicePolling, 1);
	LSB = LSB & 0x7F;
	if (LSB == 0)
	{
		// good result
	}

	// test output
	receiveString.AppendFormat(_T("%02X "), MSB);
	receiveString.AppendFormat(_T("%02X "), LSB);
	receiveString.AppendFormat(_T("%02X "), devicePolling);

	/*

	// > Dump Code
	unsigned char v_code[256];
	Sleep(50);

	errCode = MaxQ_DumpCode(0x00, 0, 16, 0, v_code, &numBytesRead, &devicePolling, 1);

	// output
	//	CString receiveString;
	for (int k = 0; k < numBytesRead; k++)
	{
	receiveString.AppendFormat(_T("%02X "), v_code[k]);
	}

	*/
	/*
	// > Password Match /test for wrong
	errCode = MaxQ_PasswordMatch(passDataWr);

	// > Poll 3Eh
	errCode = MaxQ_Poll3Eh(&devicePolling);

	// > Get Status
	errCode = MaxQ_CMD_2B(GET_STATUS, &MSB, &LSB, &devicePolling);

	// test output
	receiveString.AppendFormat(_T("%02X "), MSB);
	receiveString.AppendFormat(_T("%02X "), LSB);
	receiveString.AppendFormat(_T("%02X "), devicePolling);
	*/
	// try dump

	//

	UpdateData(TRUE);
	//m_receiveData = receiveString;
	UpdateData(FALSE);

	// Display the ProgressBar control.
	//	.Visible = true;
	// Set Minimum to 1 to represent the first file being copied.
	m_cPB_OP.SetPos(0);
	m_cPB_OP.SetStep(1);


	// Loop through all files to copy.
	for (int x = 0; x < 100; x++)
	{
		// Perform the increment on the ProgressBar.
		m_cPB_OP.StepIt();
		Sleep(10);
	}

	m_cPB_OP.SetPos(0);
	/*
	unsigned char MSB, LSB;
	CString receiveString;

	// > Perform command
	errCode = MaxQ_CMD_2B(GET_CODE_SIZE, &MSB, &LSB, &devicePolling);

	// test output
	receiveString.AppendFormat(_T("%02X "), MSB);
	receiveString.AppendFormat(_T("%02X "), LSB);
	receiveString.AppendFormat(_T("%02X "), devicePolling);

	// > Perform command
	errCode = MaxQ_CMD_2B(GET_DATA_SIZE, &MSB, &LSB, &devicePolling);

	// test output
	receiveString.AppendFormat(_T("%02X "), MSB);
	receiveString.AppendFormat(_T("%02X "), LSB);
	receiveString.AppendFormat(_T("%02X "), devicePolling);
	*/

	// >Testing output

	/*
	// Show the received data in the receive data editbox
	if (status == HID_SMBUS_SUCCESS)
	{
	for (int i = 0; i < numBytesRead; i++)
	{
	receiveString.AppendFormat(_T("%02X "), buffer[i]);
	}
	}
	*/



	// >Exit Boot
	writeData[0] = EXIT_BOOTLOADER;	// Send Slave Device Address + Write + Exit_boot cmd
	status = HidSmbus_WriteRequest(*m_pHidSmbus, I2C_DEVICE_SLAVE_ADDR, writeData, 1);

}


void CGPIOConfig::OnBnClickedButton3()
{
	// TODO: Add your control notification handler code here
	int errLine = 0;
	int errSymb = 0;
	unsigned char errCode = 0;

	//errCode = LoadCode_HexFile(&errLine, &errSymb);

	errCode = LoadVerifyCode_HexFile(&errLine, &errSymb);

}
