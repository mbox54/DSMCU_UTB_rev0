#pragma once

/////////////////////////////////////////////////////////////////////////////
// Includes
/////////////////////////////////////////////////////////////////////////////
#include "GridSFF_CP2112.h"
#include "afxcmn.h"


/////////////////////////////////////////////////////////////////////////////
// Definitions
/////////////////////////////////////////////////////////////////////////////
#define OP_SUCCESS				0x00
#define OP_FAILED				0x01

// slider range
#define VALUE_TEC_GATEC_MIN		0x00
#define VALUE_TEC_GATEC_MAX		0xFA
#define VALUE_TEC_GATEH_MIN		0x00
#define VALUE_TEC_GATEH_MAX		0xFA

#define VALUE_TEMPR_MIN			-250
#define VALUE_TEMPR_MAX			250
#define VALUE_TEMPR_DEF			0x00

#define VALUE_MOD_MIN			0x00
#define VALUE_MOD_MAX			0x80
#define VALUE_MOD_DEF			0x30

#define ADDR_BIAS				0x0F
#define ADDR_MOD				0x0C

#define TIMER_ID_TEC_APC		0x01

// ##########################################################################
// CDS4830A_SFPP_ER_TEC_APC dialog
// ##########################################################################
class CDS4830A_SFPP_ER_TEC_APC : public CDialogEx
{
	DECLARE_DYNAMIC(CDS4830A_SFPP_ER_TEC_APC)

public:
	CDS4830A_SFPP_ER_TEC_APC(CWnd* pParent = NULL);   // standard constructor
	CDS4830A_SFPP_ER_TEC_APC(HID_SMBUS_DEVICE * pHidSmbus, CProgressCtrl * p_cPB_OP, CEdit * p_EDIT_STATUS, st_serviceData * p_service, CWnd* pParent = NULL);

	virtual ~CDS4830A_SFPP_ER_TEC_APC();


// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PROPPAGE_DS4830A_SFPP_ER_TEC_APC };
#endif


protected:
	HID_SMBUS_DEVICE* m_pHidSmbus;

	// service data values
	st_serviceData* p_service;

	// main window status input
	CEdit* p_EDIT_STATUS;

private:
	// dialog internal grid operations
	BYTE uValues[256];


protected:

	// Output sample
	void Trace(LPCTSTR szFmt, ...);

	// slider config
	void CtrlsInit();

	// Device OP
	void ReadDevice();
	void WriteDevice();

	void Set_TEC_APC_Ctrls(unsigned char * v_TEC_APC, WORD uiBias);

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	// Grid events
	afx_msg void OnGridClick(NMHDR *pNotifyStruct, LRESULT* pResult);
	afx_msg void OnGridEndEdit(NMHDR *pNotifyStruct, LRESULT* pResult);

	DECLARE_MESSAGE_MAP()

	// !debug
	BYTE uValues2[256];

	// > Timer	
	UINT_PTR m_nTimer;


public:

	void StartTimer();
	void EditTimer();
	void StopTimer();

	// Grid component
	CGridSFF_CP2112 m_Grid;

	// debug output
	CEdit m_TraceWnd;

	// main window status progress
	CProgressCtrl * p_cPB_OP;

	CSliderCtrl m_Slider_Tempr;
	CSliderCtrl m_Slider_Power;

	// Dialog events
	afx_msg void OnBnClickedButtonConfWrite();
	afx_msg void OnBnClickedButton4();
	afx_msg void OnBnClickedButton5();

	virtual BOOL OnInitDialog();

	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	BOOL m_bCheck_TEC_act;
	BOOL m_bCheck_TEC_heat;
	afx_msg void OnBnClickedCheckTecAct();
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	afx_msg void OnBnClickedButton1();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedButton2();
	CString m_Edit_Tempr_Opt;
	BOOL m_bCheck_Power;
	BOOL m_bCheck_Photo;
	afx_msg void OnBnClickedCheckPowerActive();
	afx_msg void OnBnClickedCheckPhotoActive();
	afx_msg void OnEnChangeEditTemprOpt();
	virtual void OnOK();
	afx_msg void OnBnClickedCheckTecHeat();
	CString m_GateC_P;
	CString m_GateC_N;
	CString m_GateH_P;
	CString m_GateH_N;
	CString m_Edit_Power_Opt;
	CString m_Edit_Power_Gap;
};
