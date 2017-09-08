#pragma once

/////////////////////////////////////////////////////////////////////////////
// Includes
/////////////////////////////////////////////////////////////////////////////

#include "SLABCP2112.h"
// #include "DeviceSelect.h"
#include <stdio.h>
#include <stdlib.h>
#include "afxcmn.h"

#include <vector>

/////////////////////////////////////////////////////////////////////////////
// Constants /aliases
/////////////////////////////////////////////////////////////////////////////

#define MAXQBL_SUCCESS						0x00
#define MAXQBL_NO_DEVICE					0x01
#define MAXQBL_DEV_INACCESSIBLE				0x02
#define MAXQBL_OP_FAILED					0x03
#define MAXQBL_OP_WRONG						0x04
#define MAXQBL_PASS_MISMATCH				0x05

#define MAXQBL_RET_IS_3E					0x00
#define MAXQBL_RET_IS_B7					0x01
#define MAXQBL_RET_NO_3E					0x02


/////////////////////////////////////////////////////////////////////////////
// Constants /alliases
/////////////////////////////////////////////////////////////////////////////

// Maxim DS4830a I2C interface SLAVE addresses
#define I2C_BOOTLOADER_SLAVE_ADDR			0x34		// Bootloader addr
#define I2C_DEVICE_SLAVE_ADDR				0x36		// default MCU addr

// Maxim DS4830a Bootloader I2C commands
// service:
#define SET_I2C_SPE							0xF0		// enter I2C boot
#define I2C_SW_RESET						0xBB		// soft reset

#define NO_OP								0x00
#define EXIT_BOOTLOADER						0x01
#define MASTER_ERASE						0x02
#define PASSWORD_MATCH						0x03
#define GET_STATUS							0x04
#define GET_SUPPORTED_CMD					0x05
#define GET_CODE_SIZE						0x06
#define GET_DATA_SIZE						0x07
#define GET_LOADER_VERSION					0x08
#define GET_UROM_VERSION					0x09
#define PRINT_BANNER						0x0D
#define PAGE_ERASE							0xE0

// load and check:
#define LOAD_CODE							0x10
#define LOAD_DATA							0x11

#define DUMP_CODE							0x20
#define DUMP_DATA							0x21

#define CRC_CODE							0x30
#define CRC_DATA							0x31

#define VERIFY_CODE							0x40
#define VERIFY_DATA							0x41

#define LOAD_VERIFY_CODE					0x50
#define LOAD_VERIFY_DATA					0x51

/////////////////////////////////////////////////////////////////////////////
// Static Global Function Prototypes
/////////////////////////////////////////////////////////////////////////////

static std::vector<BYTE> GetHexArray(CString hexString);
std::vector<BYTE> StrHexToByte(BYTE * v_HexLetters, int count, unsigned char * errCode);
CString ByteToStrHex(BYTE * v_ByteData, int count, unsigned char * errCode);


/////////////////////////////////////////////////////////////////////////////
// DS4830a Loader dialog
/////////////////////////////////////////////////////////////////////////////


class CConfigWnd : public CDialogEx
{
	DECLARE_DYNAMIC(CConfigWnd)

public:
	CConfigWnd(CWnd* pParent = NULL);   // ����������� �����������
	CConfigWnd(HID_SMBUS_DEVICE* pHidSmbus, CWnd* pParent = NULL);   

	virtual ~CConfigWnd();

// ������ ����������� ����
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CONFIG_WND };
#endif

	// User Variables
protected:
	BYTE	passData[32] = { 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22,
		0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22,
		0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22,
		0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22 };

	BYTE	passDataWr[32] = { 0x33, 0x33, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22,
		0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22,
		0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22,
		0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x33 };


// Control Variables
protected:
	//plugged devices
	BYTE m_gpio_latch;

	//timer controler
	UINT_PTR m_nTimer;


/////////////
//	status bar
	CStatusBar	m_statusBar;

	void InitStatusBar();

	// Set the status bar text
	void SetStatusText0(const CString& text);
	void SetStatusText1(const CString& text);


// Protected Members
protected:
	HID_SMBUS_DEVICE* m_pHidSmbus;

// Protected Methods
protected:
	//read direction
	int  GetGPIOini(BYTE *gpio_val);

	//latch operations
	void LatchController();
	void LatchReset(BOOL bRest = FALSE);

	//reflect on dialog
	void SetLatchValue(BYTE latchValue);

	// timer
	void StartTimer();
	void StopTimer();

	// init
	void SetDefaults();
	void SetFromDevice();


	// # Maxim DS Bootloader functionality
	unsigned char MaxQ_EnterBoot();
	unsigned char MaxQ_SoftReset();
	unsigned char MaxQ_MasterErase();
	unsigned char MaxQ_Poll3Eh(unsigned char *MAXQBL_RET_3EH);
	unsigned char MaxQ_CMD_2B(unsigned char CMD, unsigned char *MSB, unsigned char *LSB, unsigned char *MAXQBL_RET_3EH, unsigned char MODE);
	unsigned char MaxQ_PasswordMatch(unsigned char *password);
	unsigned char MaxQ_VerifyCode(unsigned char LENGTH, unsigned char addrL, unsigned char AddrH, unsigned char * v_code);

	unsigned char MaxQ_DumpCode(unsigned char addrL, unsigned char addrH,
		unsigned char lengthL, unsigned char lengthH,
		unsigned char * v_code, unsigned char * numBytesRead,
		unsigned char * MAXQBL_RET_3EH, unsigned char MODE);
	unsigned char MaxQ_LoadVerifyCode(unsigned char LENGTH, unsigned char addrL, unsigned char addrH, unsigned char * v_code);

	// # Service procedured
	int LoadVerifyCode_HexFile(int *errLine, int *errSymb);
	int DumpCode_HexFile(int *errLine, int *errSymb);



//	_crt_signal_t signalHandler(int signum);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // ��������� DDX/DDV

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonResetLatch();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedCheckLatch();
	// progress OP status
	CProgressCtrl m_cPB_OP;
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();
	CString m_TextOutput;
	CString m_TextFilename;
};
