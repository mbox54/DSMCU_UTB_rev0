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
#define VALUE_BIAS_MIN			0x0080
#define VALUE_BIAS_MAX			0x0300
#define VALUE_BIAS_DEF			0x0200

#define VALUE_MOD_MIN			0x0000
#define VALUE_MOD_MAX			0x0800
#define VALUE_MOD_DEF			0x0300

#define VALUE_HORISON_MIN		-127
#define VALUE_HORISON_MAX		127
#define VALUE_HORISON_DEF		0

#define VALUE_VERTICAL_MIN		0
#define VALUE_VERTICAL_MAX		127
#define VALUE_VERTICAL_DEF		64

#define ADDR_BIAS				0x0A
#define ADDR_MOD				0x06
#define ADDR_HORISON			0x02
#define ADDR_VERTICAL			0x01

// ##########################################################################
// CDS4830A_SFPP_LR_ENGI_ dialog
// ##########################################################################

class CDS4830A_SFPP_LR_CONF_ENGI : public CDialogEx
{
	DECLARE_DYNAMIC(CDS4830A_SFPP_LR_CONF_ENGI)

public:
	CDS4830A_SFPP_LR_CONF_ENGI(CWnd* pParent = NULL);   // standard constructor
	CDS4830A_SFPP_LR_CONF_ENGI(HID_SMBUS_DEVICE * pHidSmbus, CProgressCtrl * p_cPB_OP, CEdit * p_EDIT_STATUS, st_serviceData * p_service, CWnd* pParent = NULL);

	virtual ~CDS4830A_SFPP_LR_CONF_ENGI();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PROPPAGE_DS4830A_SFPP_LR_CONF_ENGI };
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

	void EditHexControl(CEdit *pEdit);

	// Table File OP
	int TableValues_LoadFromFile(char * v_filename, BYTE * v_ByteData);
	int TableValues_SaveToFile(BYTE * v_ByteData);

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

														// Grid events
	afx_msg void OnGridClick(NMHDR *pNotifyStruct, LRESULT* pResult);
	afx_msg void OnGridEndEdit(NMHDR *pNotifyStruct, LRESULT* pResult);

	// Slider events
//	afx_msg void OnSliderBIASMove(NMHDR *pNotifyStruct, LRESULT* pResult);


	DECLARE_MESSAGE_MAP()

	// !debug
	BYTE uValues2[256];


public:
	// Grid component
	CGridSFF_CP2112 m_Grid;

	// debug output
	CEdit m_TraceWnd;

	CEdit m_Edit_Bias_H;
	CEdit m_Edit_Bias_L;

	CEdit m_Edit_Mod_H;
	CEdit m_Edit_Mod_L;

	CEdit m_Edit_Horison_H;
	CEdit m_Edit_Horison_L;
	CEdit m_Edit_Horison;

	CEdit m_Edit_Vertical;

	// main window status progress
	CProgressCtrl * p_cPB_OP;

	// config filename
	CString m_Edit_Filename;

	BOOL m_bCheck_Vertical;


	// Sliders
	CSliderCtrl m_Slider_BIAS;
	CSliderCtrl m_Slider_MOD;
	CSliderCtrl m_Slider_HORISON;
	CSliderCtrl m_Slider_VERTICAL;


public:
	virtual BOOL OnInitDialog();

	afx_msg void OnBnClickedButton4();
	afx_msg void OnBnClickedButton5();

	afx_msg void OnBnClickedButtonConfRead();
	afx_msg void OnBnClickedButtonConfWrite();
	afx_msg void OnBnClickedButton1();

//	afx_msg void OnNMCustomdrawSliderBias(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);

	afx_msg void OnBnClickedButtonBiasSet();
//	afx_msg void OnEnChangeEditBiasH();
//	afx_msg void OnEnChangeEditBiasL();
	afx_msg void OnBnClickedButtonModSet();
	afx_msg void OnBnClickedButtonHorSet();
//	afx_msg void OnBnClickedCheckVertical();
	afx_msg void OnBnClickedButtonVerticalSet();
	CString m_sEdit_PassValue;
	virtual void OnOK();
};
