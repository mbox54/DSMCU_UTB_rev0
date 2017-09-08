#pragma once

/////////////////////////////////////////////////////////////////////////////
// Includes
/////////////////////////////////////////////////////////////////////////////
#include "SLABCP2112.h"
#include "GridSFF_CP2112.h"
#include "GridTable_DDM.h"
#include "LibHashFunct.h"
#include "afxwin.h"


// ##########################################################################
// CDS4830A_SFPP_DDM dialog
// ##########################################################################


class CDS4830A_SFPP_DDM : public CDialogEx
{
	DECLARE_DYNAMIC(CDS4830A_SFPP_DDM)

public:
	// standard constructor
	CDS4830A_SFPP_DDM(CWnd* pParent = NULL);   

	CDS4830A_SFPP_DDM(HID_SMBUS_DEVICE * pHidSmbus, CProgressCtrl * p_cPB_OP, CEdit * p_EDIT_STATUS, st_serviceData * p_service, CWnd * pParent = NULL);
	virtual ~CDS4830A_SFPP_DDM();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PROPPAGE_DS4830A_SFPP_DDM };
#endif

protected:
	HID_SMBUS_DEVICE* m_pHidSmbus;

	// service data values
	st_serviceData* p_service;

	// main window status input
	CEdit * p_EDIT_STATUS;

	// main window progress bar
	CProgressCtrl * p_cPB_OP;

	// Tables
	CGridSFF_CP2112 m_Grid;					// Table A2h LOWER HEX 
	CGridTable_DDM m_GridDescr;				// Table A2h LOWER Edit and Description

private:
	// dialog internal grid operations
	BYTE uValues[256];

	// Grid events
	void Trace(LPCTSTR szFmt, ...);

	afx_msg void OnGridEndEdit(NMHDR *pNotifyStruct, LRESULT* pResult);
	afx_msg void OnGridDescrEndEdit(NMHDR *pNotifyStruct, LRESULT* pResult);

	// dialog common controls
	virtual BOOL OnInitDialog();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:

	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton4();
	afx_msg void OnBnClickedButton5();

	afx_msg void OnBnClickedButton6();
	afx_msg void OnBnClickedButton7();
	virtual void OnOK();
};
