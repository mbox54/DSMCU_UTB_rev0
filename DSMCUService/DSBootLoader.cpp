// DSBootLoader.cpp : implementation file
//

#include "stdafx.h"
#include "DSMCUService.h"
#include "DSBootLoader.h"
#include "afxdialogex.h"


// CDSBootLoader dialog

IMPLEMENT_DYNAMIC(CDSBootLoader, CDialog)


CDSBootLoader::CDSBootLoader(CWnd * pParent)
	: m_Check_OutputProg(FALSE)
{
}


CDSBootLoader::CDSBootLoader(hSerialCDC* phSerialCDC, CWnd* pParent /*=NULL*/)
	: CDialog(IDD_PROPPAGE_BOOTLOAD, pParent)
	, m_phSerialCDC(phSerialCDC)
{
}


CDSBootLoader::~CDSBootLoader()
{
}


void CDSBootLoader::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PB_OP, m_cPB_OP);
	DDX_Control(pDX, IDC_EDIT_OUTPUT, m_TraceWnd);
	DDX_Check(pDX, IDC_CHECK_OUTPUT_PROG, m_Check_OutputProg);
}


BEGIN_MESSAGE_MAP(CDSBootLoader, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_LOAD, &CDSBootLoader::OnBnClickedButtonLoad)

	ON_MESSAGE(WM_MYMESSAGE, OnMyMessage)

	ON_BN_CLICKED(IDC_BUTTON4, &CDSBootLoader::OnBnClickedButton4)
END_MESSAGE_MAP()

// CDSBootLoader message handlers
/////////////////////////////////////////////////////////////////////////////
// Static Global Functions
/////////////////////////////////////////////////////////////////////////////

// Given a input string containing hex byte values,
// return a vector of the numeric values
std::vector<BYTE> CDSBootLoader::GetHexArray(CString hexString)
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
std::vector<BYTE> CDSBootLoader::StrHexToByte(BYTE * v_HexLetters, int count, unsigned char * errCode)
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
CString CDSBootLoader::ByteToStrHex(BYTE * v_ByteData, int count, unsigned char * errCode)
{
	CString StrHex;
	for (int i = 0; i < count; i++)
	{
		StrHex.AppendFormat(_T("%02X"), v_ByteData[i]);
	}

	return StrHex;
}

//---------------------------------------------------------------------------
// Maxim DS Bootloader functionality
//---------------------------------------------------------------------------
unsigned char CDSBootLoader::MaxQ_EnterBoot()
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

unsigned char CDSBootLoader::MaxQ_SoftReset()
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

unsigned char CDSBootLoader::MaxQ_MasterErase()
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

unsigned char CDSBootLoader::MaxQ_Poll3Eh(unsigned char * MAXQBL_RET_3EH)
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
	Sleep(30);																// wait for i2c proceed
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
Get Code Size (flash)	06h				Size–lower byte			Size–higher byte
Get Data Size (SRAM)	07h				Size–lower byte			Size–higher byte
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
unsigned char CDSBootLoader::MaxQ_CMD_2B(unsigned char CMD, unsigned char * MSB, unsigned char * LSB, unsigned char * MAXQBL_RET_3EH, unsigned char MODE)
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
0 – The password is unlocked or had a default value; password - protected commands may be used.
1 – The password is locked.Password - protected commands may not be used.

STATUS
00 - No Error. The last command completed successfully.
03 - No Password Match. An attempt was made to use a password-protected command without first
matching a valid password. Or, the Password Match command was called with an incorrect
password value.
**************************************************************************************/
unsigned char CDSBootLoader::MaxQ_PasswordMatch(unsigned char * password)
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

unsigned char CDSBootLoader::MaxQ_VerifyCode(unsigned char LENGTH, unsigned char addrL, unsigned char addrH, unsigned char * v_code)
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

unsigned char CDSBootLoader::MaxQ_DumpCode(unsigned char addrL, unsigned char addrH, unsigned char lengthL, unsigned char lengthH, unsigned char * v_code, unsigned char * numBytesRead, unsigned char * MAXQBL_RET_3EH, unsigned char MODE)
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

unsigned char CDSBootLoader::MaxQ_LoadVerifyCode(unsigned char LENGTH, unsigned char addrL, unsigned char addrH, unsigned char * v_code)
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

int CDSBootLoader::LoadVerifyCode_HexFile(int * errLine, int * errSymb)
{
	// init locals
	*errLine = 0;						// error line. Defined exit-line number when load had been failed. / -1 = can't open file 
	*errSymb = 0;						// error symbol. Defined exit-symbol number when load had been failed.

	unsigned char errCode;				// error Code for unsafe operations
	unsigned char devicePolling;

	FILE *fp;

	// [#] FILE OP CHECK
	// > Define Filename
	CString str_filename("test.hex");
	char *file_name = "test.hex";

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
		*errLine = -1;
		return MAXQBL_OP_FAILED;
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
				*errLine = -2;
				return MAXQBL_OP_FAILED;
				break;
			}
		}
	}

	// reopen FILE
	fclose(fp);

	err = fopen_s(&fp, file_name, "r");

	if (err != 0)
	{	// "can't open file" 
		*errLine = -1;
		return MAXQBL_OP_FAILED;
	}

	// [STATUS OUTPUT]
	Trace(_T("- lines: %04d \n"), fileLineNum);

	// progress component
	m_cPB_OP.SetPos(0);
	m_cPB_OP.SetStep(1);

	// progress bar
	double fstepTrigger = fileLineNum;
	fstepTrigger /= 100;
	double stepValue = 0;

	// [#] DEVICE OP CHECK
	// > Enter Boot	
	errCode = MaxQ_EnterBoot();
	if (errCode != MAXQBL_SUCCESS)
	{
		*errLine = -3;
		*errSymb = errCode;
		return MAXQBL_OP_FAILED;
	}

	// [STATUS OUTPUT]
	Trace(_T("# Entering device boot ...OK \n"));

	// > Soft Reset
	errCode = MaxQ_SoftReset();
	if (errCode != MAXQBL_SUCCESS)
	{
		*errLine = -4;
		*errSymb = errCode;
		return MAXQBL_OP_FAILED;
	}

	// [STATUS OUTPUT]
	Trace(_T("Reseting device ...OK \n"));


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

	// > Master Erase
	errCode = MaxQ_MasterErase();
	if (errCode != MAXQBL_SUCCESS)
	{
		*errLine = -7;
		*errSymb = errCode;
		return MAXQBL_OP_FAILED;
	}

	// [STATUS OUTPUT]
	Trace(_T("Erasing FLASH ...OK \n"));

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


	// [STATUS OUTPUT]
	Trace(_T("  \n"));
	Trace(_T("~~~ Start CODE loading operation ~~~ \n"));
	Trace(_T("------------------------------------------------ \n"));

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

									// Get line opcode bytes number (flashStartAddr[2])
									// TODO: calculate self CHK and compare CHK's
									CHK = vByteVector[0];


									// > Load OPCODE line to Device & Verify OPCODE Load result
									errCode = MaxQ_LoadVerifyCode(n, flashStartAddr[1], flashStartAddr[0], v_code);
									Sleep(5);

									if (errCode != 0)
									{	// abort procedure
										*errLine = l;
										*errSymb = k;

										// [STATUS OUTPUT]
										Trace(_T("[X] Loading operation failed! \n"));
										Trace(_T("error info: on line %5d %s %3d \n", l, _T(", symbol: "), k));

										return MAXQBL_OP_FAILED;
									}

									// [STATUS OUTPUT]
									// TODO: 'this->m_Check_OutputProg' need to replace to function_parameter
									if (this->m_Check_OutputProg)
									{
										CString str_code;

										for (int k = 0; k < n; k++)
										{
											str_code.AppendFormat(_T("%02X "), v_code[k]);

										}
								
										Trace(_T("loading ADDR: %02X%02X %s %s \n"), flashStartAddr[0], flashStartAddr[1], _T(", DATA: "), str_code);
									}

			

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


	Trace(_T("------------------------------------------------ \n"));
	Trace(_T("[V] Loading operation successfull! \n"));

	return MAXQBL_SUCCESS;
}

// Dump code from device /Maxim DS4830a type/ to file /Intel Hex format/
int CDSBootLoader::DumpCode_HexFile(int * errLine, int * errSymb)
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
	const unsigned int CODE_FLSH_LENGTH = 0x7FFF;		// TODO: define from status _CODE_LENGTH cmd
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
		if (256 * flashStartAddr[1] + flashStartAddr[0] < 0x1000) //CODE_FLSH_LENGTH) //! ###### debug #####!
		{
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
				c[finalizeStr + 1] = '\0';		// null string ender symbol 

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

	return MAXQBL_SUCCESS;
}


void CDSBootLoader::OnBnClickedButtonLoad()
{
	// TODO: Add your control notification handler code here
	int errLine = 0;
	int errSymb = 0;
	unsigned char errCode = 0;

	//errCode = LoadCode_HexFile(&errLine, &errSymb);

	UpdateData(TRUE);
	errCode = LoadVerifyCode_HexFile(&errLine, &errSymb);

}

// -------------------------------------------------------------------
// Supporting procedures
// -------------------------------------------------------------------
void CDSBootLoader::Trace(LPCTSTR szFmt, ...)
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
//	m_TraceWnd.LineScroll(-m_TraceWnd.GetLineCount());
	m_TraceWnd.LineScroll(m_TraceWnd.GetLineCount() -10);
}

LRESULT CDSBootLoader::OnMyMessage(WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(wParam);
	UNREFERENCED_PARAMETER(lParam);
	//



	return LRESULT();
}


void CDSBootLoader::OnBnClickedButton4()
{
	// TODO: Add your control notification handler code here
	for (int k = 0; k < 15; k++)
	{
		//
		Sleep(200);

		// test output
//		CString receiveString;
//		receiveString.AppendFormat(_T("%02X "), k);

//		UpdateData(TRUE);
		Trace(_T("k addr = %5d %s \n"), k, _T("... is OK")); 
//		UpdateData(FALSE);

	}

}
