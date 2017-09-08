#pragma once


/////////////////////////////////////////////////////////////////////////////
// Includes
/////////////////////////////////////////////////////////////////////////////
#include "SLABCP2112.h"
#include "afxwin.h"
#include "afxcmn.h"


/////////////////////////////////////////////////////////////////////////////
// Definitions
/////////////////////////////////////////////////////////////////////////////
#define TIMER_ID_TB_MON		0x01



// NOTE:
// 
// GIPO.0 – Tx Toggle config.
// GPIO.1 – Rx Toggle config.
// GPIO.2 – Input.Tx Fault. 1 – ошибка в работе передатчика, 0 – все нормально.
// GPIO.3 – Output.Tx Disable.Open - Drain. 1 – отключение лазера, 0 – включение.
// GPIO.4 – Input.Mod ABS. 1 – модуль отсутствует, 0 – модуль присутствует.
// GPIO.5 – Output.RS0.Open - Drain.Два состояния 1 и 0. Реализовать галочку.
// GPIO.6 – Input.Rx LOS. 1 – сигнал на приеме ниже дозволенного уровня, 0 – все нормально.
// GPIO.7 – Output.RS1.Open - Drain.Два состояния 1 и 0. Реализовать галочку.


// NOTE:
/*
1) Tx Fault is a CMOS output. When high, output indicates a laser fault of some kind.Low indicates normal
operation.Tx Fault is asserted when bias current of laser exceeds the factory - calibrated threshold level.

2) Tx Disable is an input that is used to shut down the transmitter optical output.It is pulled up within the module
with a 7.5kΩ resistor.

3) Mod - Def 0, 1, 2. These are the module definition pins.They should be pulled up with a 4.7k - 10kΩ resistor on
the host board.The pull - up voltage shall be VccT.

Mod - Def 0 indicates that the module is present
Mod - Def 1 is the clock line of two wire serial interface for serial ID
Mod - Def 2 is the data line of two wire serial interface for serial ID

4) LOS(Loss of Signal) is an open collector output that shall be pulled up with a 4.7k - 10kΩ resistor.Pull up
voltage between 2.0V and VccR + 0.3V.Low indicates normal operation.

5) RD - / +: These are the differential receiver outputs.They are AC coupled 100Ω differential lines which should
be terminated with 100Ω(differential) at the user SERDES.The AC coupling is done inside the module and
is thus not required on the host board.

6) TD - / +: These are the differential transmitter inputs.They are AC - coupled, differential lines with 100Ω differential
termination inside the module.The AC coupling is done inside the module and is thus not required on the
host board.
*/


// ##########################################################################
// CTB_SFP_R2 dialog
// ##########################################################################
class CTB_SFP_R2 : public CDialogEx
{
	DECLARE_DYNAMIC(CTB_SFP_R2)

public:
	// standard constructor
	CTB_SFP_R2(CWnd* pParent = NULL);   
	CTB_SFP_R2(HID_SMBUS_DEVICE* pHidSmbus, CWnd* pParent = NULL);

	virtual ~CTB_SFP_R2();


// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PROPPAGE_TB_SFP_R2 };
#endif

protected:
	HID_SMBUS_DEVICE* m_pHidSmbus;



	// > Timer	
	UINT_PTR m_nTimer;



	// Control Variables


	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

public:
	// Read CP2112 Latch: Modude PROC Config
	void ProcLatchState();

	// Timer PROCs
	void StartTimer();
	void EditTimer();
	void StopTimer();

	CEdit m_TraceWnd;
	CProgressCtrl m_cPB_OP;

	// Button Controls
	afx_msg void OnBnClickedButtonMonStart();
	afx_msg void OnBnClickedButtonMonStop();
	afx_msg void OnBnClickedButtonTxDisable();
	afx_msg void OnBnClickedButtonRs0();
	afx_msg void OnBnClickedButtonRs1();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
};
