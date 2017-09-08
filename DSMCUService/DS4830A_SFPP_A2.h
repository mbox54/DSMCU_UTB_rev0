#pragma once

/////////////////////////////////////////////////////////////////////////////
// Includes
/////////////////////////////////////////////////////////////////////////////
#include "SLABCP2112.h"
#include "GridSFF_CP2112.h"
#include "afxwin.h"
#include "afxcmn.h"


// ##########################################################################
// CDS4830A_SFPP_A2 dialog
// ##########################################################################

class CDS4830A_SFPP_A2 : public CDialog
{
	DECLARE_DYNAMIC(CDS4830A_SFPP_A2)

public:
	CDS4830A_SFPP_A2(CWnd* pParent = NULL);   // standard constructor
	CDS4830A_SFPP_A2(HID_SMBUS_DEVICE* pHidSmbus, CProgressCtrl * p_cPB_OP, CEdit * p_EDIT_STATUS, st_serviceData * p_service, CWnd* pParent = NULL);
	virtual ~CDS4830A_SFPP_A2();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PROPPAGE_DS4830A_SFPP_A2 };
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
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
														// Table procedures
	unsigned char DeviceSlave_Read(BYTE * v_ByteData, BYTE slaveAddr, BYTE startAddr, WORD count);
	unsigned char DeviceSlave_Write(BYTE * v_ByteData, BYTE slaveAddr, BYTE startAddr, WORD count);

	// Output sample
	void Trace(LPCTSTR szFmt, ...);

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

	//	
	virtual BOOL OnInitDialog();

	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton4();
	afx_msg void OnBnClickedButton5();
	virtual void OnOK();
};
