// class CGridTable_MSA header file

// Inherited from CGridTable class
// Class purpose is: 
// - define specific Table for MSA Table input/outpit

#pragma once

/////////////////////////////////////////////////////////////////////////////
// Includes
/////////////////////////////////////////////////////////////////////////////
#include "GridTable.h"

/////////////////////////////////////////////////////////////////////////////
// Definitions
/////////////////////////////////////////////////////////////////////////////


class CGridTable_MSA : public CGridTable
{
public:
	CGridTable_MSA();
	~CGridTable_MSA();

	// Table proc
	void SetTableColor();

	int UpdateTable_Val(unsigned short uiRowNumber, BYTE ucValue);
	int UpdateTable_Sel(unsigned short uiStartNumber, unsigned short uiCount, unsigned char * v_Value);
	int UpdateTable_All(unsigned char * v_Value);

	// Grid events
	afx_msg int OnGridEndEdit(NMHDR * pNotifyStruct, LRESULT * pResult);



};

