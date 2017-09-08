#pragma once

/////////////////////////////////////////////////////////////////////////////
// Includes
/////////////////////////////////////////////////////////////////////////////
#include "SLABCP2112.h"
#include "GridSFF_CP2112.h"
#include "afxwin.h"
#include "afxcmn.h"

/////////////////////////////////////////////////////////////////////////////
// Definitions
/////////////////////////////////////////////////////////////////////////////

#define OP_SUCCESS				0x00
#define OP_FAILED				0x01

// slider range
#define VALUE_BIAS_MIN			0x00
#define VALUE_BIAS_MAX			0x90
#define VALUE_BIAS_DEF			0x30

#define VALUE_MOD_MIN			0x00
#define VALUE_MOD_MAX			0x80
#define VALUE_MOD_DEF			0x30

#define ADDR_BIAS				0x0F
#define ADDR_MOD				0x0C


// ##########################################################################
// CDS4830A_SFPP_ER_ENGI dialog
// ##########################################################################
class CDS4830A_SFPP_ER_ENGI : public CDialog
{
	DECLARE_DYNAMIC(CDS4830A_SFPP_ER_ENGI)

public:
	CDS4830A_SFPP_ER_ENGI(CWnd* pParent = NULL);   // standard constructor
	CDS4830A_SFPP_ER_ENGI(HID_SMBUS_DEVICE * pHidSmbus, CProgressCtrl * p_cPB_OP, CEdit * p_EDIT_STATUS, st_serviceData * p_service, CWnd* pParent = NULL);

	virtual ~CDS4830A_SFPP_ER_ENGI();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PROPPAGE_DS4830A_SFPP_ER_ENGI };
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
	void SliderInit();

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	// Device OP
	void ReadDevice();
	void WriteDevice();

	// Grid events
	afx_msg void OnGridClick(NMHDR *pNotifyStruct, LRESULT* pResult);
	afx_msg void OnGridEndEdit(NMHDR *pNotifyStruct, LRESULT* pResult);

	DECLARE_MESSAGE_MAP()

	// !debug
	BYTE uValues2[256];


public:
	// Grid component
	CGridSFF_CP2112 m_Grid;

	// debug output
	CEdit m_TraceWnd;

	// main window status progress
	CProgressCtrl * p_cPB_OP;

	// Sliders
	CSliderCtrl m_Slider_BIAS;
	CSliderCtrl m_Slider_MOD;

	CEdit m_Edit_Bias_H;
	CEdit m_Edit_Mod_H;
	//	

	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton4();
	afx_msg void OnBnClickedButton5();

	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButtonConfRead();

	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);

	afx_msg void OnBnClickedButtonBiasSet();
	afx_msg void OnBnClickedButtonModSet2();
	afx_msg void OnBnClickedButtonConfWrite();
	CString m_sEdit_PassValue;
	virtual void OnOK();
};
