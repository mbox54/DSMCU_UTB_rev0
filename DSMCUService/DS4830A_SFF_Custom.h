#pragma once

/////////////////////////////////////////////////////////////////////////////
// Includes
/////////////////////////////////////////////////////////////////////////////
#include "SLABCP2112.h"
#include "GridSFF_CP2112.h"
#include "afxwin.h"


// ##########################################################################
// CDS4830A_SFF_Custom dialog
// ##########################################################################
class CDS4830A_SFF_Custom : public CDialog
{
	DECLARE_DYNAMIC(CDS4830A_SFF_Custom)

public:
	CDS4830A_SFF_Custom(CWnd* pParent = NULL);   // standard constructor
	CDS4830A_SFF_Custom(HID_SMBUS_DEVICE * pHidSmbus, CProgressCtrl * p_cPB_OP, CEdit * p_EDIT_STATUS,  st_serviceData * p_service, CWnd * pParent = NULL);

	virtual ~CDS4830A_SFF_Custom();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PROPPAGE_DS4830A_SFPP_CUSTOM };
#endif

protected:
	HID_SMBUS_DEVICE* m_pHidSmbus;

	// service data values
	st_serviceData* p_service;

	// main window status input
	CEdit * p_EDIT_STATUS;

	// main window progress bar
	CProgressCtrl * p_cPB_OP;

private:
	// dialog internal grid operations
	BYTE uValues[256];

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

														// Table procedures
//	unsigned char DeviceSlave_ReadTable(BYTE * v_ByteData, BYTE slaveAddr, BYTE startAddr, WORD count);
//	unsigned char DeviceSlave_WriteTable(BYTE * v_ByteData, BYTE slaveAddr, BYTE startAddr, WORD count);

	// Output sample
	void Trace(LPCTSTR szFmt, ...);

	void EditHexControl(CEdit *pEdit);

	void EditInit();

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

	//	
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton4();
	afx_msg void OnBnClickedButton5();


	virtual BOOL OnInitDialog();

	// Inputs for CUSTOM parameters
	CString m_SLA_ADDR;
	CString m_TABL_ADDR;
	CString m_TABL_NAME;

	CString m_sEdit_PasValue;
	CString m_sEdit_PasAddr;

	// activate Password Mode
	BOOL m_bCheck_PasEnable;

	CEdit m_Edit_SlaveAddr;
	CEdit m_Edit_TableAddr;
	CEdit m_Edit_TableName;
	CEdit m_Edit_PasAddr;
	CEdit m_Edit_PasValue;
	
	afx_msg void OnEnChangeEditSlaaddr();
	afx_msg void OnEnChangeEditTabladdr();
	afx_msg void OnEnChangeEditTablname();
	afx_msg void OnEnChangeEditPasaddr();
	afx_msg void OnEnChangeEditPasvalue();
	virtual void OnOK();
	BOOL m_bCheck_SelRange;
	CString m_SelAddr;
//	CEdit m;
	CString m_SelCount;
	BOOL m_bCheck_TabSelect;
};
