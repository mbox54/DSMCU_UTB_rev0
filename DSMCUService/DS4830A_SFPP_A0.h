#pragma once

/////////////////////////////////////////////////////////////////////////////
// Includes
/////////////////////////////////////////////////////////////////////////////
#include "SLABCP2112.h"
#include "GridSFF_CP2112.h"
#include "afxwin.h"
#include "afxcmn.h"



// ##########################################################################
// CDS4830A_SFPP_A0 dialog
// ##########################################################################
class CDS4830A_SFPP_A0 : public CDialog
{
	DECLARE_DYNAMIC(CDS4830A_SFPP_A0)

public:
	CDS4830A_SFPP_A0(CWnd* pParent = NULL);   // standard constructor
	CDS4830A_SFPP_A0(HID_SMBUS_DEVICE * pHidSmbus, CProgressCtrl * p_cPB_OP, CEdit * p_EDIT_STATUS, st_serviceData * p_service, CWnd * pParent = NULL);
	virtual ~CDS4830A_SFPP_A0();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PROPPAGE_DS4830A_SFPP_A0 };
#endif

// > Resources from Owner dialog
protected:
	HID_SMBUS_DEVICE* m_pHidSmbus;

	// service data values
	st_serviceData* p_service;

	// main window status input
	CEdit * p_EDIT_STATUS;


// > Controls
public:
	// Grid component main
	CGridSFF_CP2112 m_Grid;

	// ASCII I/O
	CGridSFF_CP2112 m_GridASCII;

	// main window status progress
	CProgressCtrl * p_cPB_OP;

	// Grid for ASCII I/O
	//	CGridSFF_CP2112 m_GridASCII;

	// debug output
	CEdit m_TraceWnd;


private:
	// dialog internal grid operations
	BYTE uValues[256];

	// Var to control Event appearing
	bool m_bWasReading;	// Reading

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	// Output sample
	void Trace(LPCTSTR szFmt, ...);

	// Grid events
	afx_msg void OnGridClick(NMHDR * pNotifyStruct, LRESULT * pResult);
	afx_msg void OnGrid2Click(NMHDR * pNotifyStruct, LRESULT * pResult);
	afx_msg void OnGridEndEdit(NMHDR * pNotifyStruct, LRESULT * pResult);
	afx_msg void OnGrid2EndEdit(NMHDR * pNotifyStruct, LRESULT * pResult);

	virtual BOOL OnInitDialog();

	afx_msg void OnTimer(UINT_PTR nIDEvent);

	//timer controler
	UINT_PTR m_nTimer;

	// timer
	void StartTimer();
	void StopTimer();

public:
	// Buttons
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton4();
	afx_msg void OnBnClickedButton5();

	DECLARE_MESSAGE_MAP()


	virtual void OnOK();
};
