#pragma once

/////////////////////////////////////////////////////////////////////////////
// Includes
/////////////////////////////////////////////////////////////////////////////
#include "SLABCP2112.h"
#include "GridSFF_CP2112.h"
#include "GridTable_MSA.h"
#include "LibHashFunct.h"
#include "afxwin.h"


// ##########################################################################
// CDS4830A_SFPP_MSA dialog
// ##########################################################################


class CDS4830A_SFPP_MSA : public CDialogEx
{
	DECLARE_DYNAMIC(CDS4830A_SFPP_MSA)

public:
	CDS4830A_SFPP_MSA(CWnd* pParent = NULL);   // standard constructor
	CDS4830A_SFPP_MSA(HID_SMBUS_DEVICE * pHidSmbus, CProgressCtrl * p_cPB_OP, CEdit * p_EDIT_STATUS, st_serviceData * p_service, CWnd * pParent = NULL);

	virtual ~CDS4830A_SFPP_MSA();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PROPPAGE_DS4830A_SFPP_MSA };
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
	CGridSFF_CP2112 m_Grid;					// Table A0h HEX 
	CGridTable_MSA m_GridDescr;				// Table A0h Edit and Description

private:
	// dialog internal grid operations
	BYTE uValues[256];

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support


	DECLARE_MESSAGE_MAP()
public:

	// Output sample
	void Trace(LPCTSTR szFmt, ...);

	// Grid events
	afx_msg void OnGridEndEdit(NMHDR *pNotifyStruct, LRESULT* pResult);
	afx_msg void OnGridDescrEndEdit(NMHDR *pNotifyStruct, LRESULT* pResult);

	// dialog common controls
	virtual BOOL OnInitDialog();

	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();

	afx_msg void OnBnClickedButton4();
	afx_msg void OnBnClickedButton5();

	afx_msg void OnBnClickedButton6();
	virtual void OnOK();
};
