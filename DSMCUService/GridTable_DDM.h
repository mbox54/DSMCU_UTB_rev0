// class CGridTable_DDM header file

// Inherited from CGridTable class
// Class purpose is: 
// - define specific Table for DDM Table input/outpit

#pragma once

/////////////////////////////////////////////////////////////////////////////
// Includes
/////////////////////////////////////////////////////////////////////////////
#include "GridTable.h"

/////////////////////////////////////////////////////////////////////////////
// Definitions
/////////////////////////////////////////////////////////////////////////////

// > Define Edit Set
const unsigned char v_RowSet_Index[6] = { 19, 27, 32, 40, 43, 44 };
const unsigned char v_RowSet_Bytes[6] = { 00, 40, 56, 76, 92, 95 };
const unsigned char v_RowSet_Sizes[6] = { 2, 2, 4, 2, 1, 1 };


class CGridTable_DDM :
	public CGridTable
{
public:
	CGridTable_DDM();
	~CGridTable_DDM();

	unsigned short GetRangeNumber(unsigned short uiRowNumber, unsigned char * err_);
	unsigned short GetRowNumber(unsigned short uiByteNumber, unsigned char * ucRowBytePadding, unsigned char * err_);
	unsigned short GetByteNumber(unsigned short uiRowNumber, unsigned short * index_mst, unsigned char * err_);

	int ProcValue(unsigned short uiNumber, unsigned char modeNumType, unsigned char * v_Values, unsigned char * v_Buf, unsigned char modeOpType);
	int ProcTable(unsigned short uiNumber, unsigned char modeNumType, unsigned char * v_Buf, unsigned char modeOpType);

	// service
	int CheckValidUInt1(int iRow, int iCol);
	int CheckValidUInt2(int iRow, int iCol);


	int WordToBitStr(WORD uByteValue, unsigned char * v_BitStrValue);
	int BitStrToWord(WORD * uByteValue, unsigned char * v_BitStrValue);
	int StrFloatToVarFloat(CString str_Float, float * fValue);

	// Table proc
	int UpdateTable_Val1(unsigned short uiRowNumber, BYTE ucValue);
	int UpdateTable_Val2(unsigned short uiRowNumber, WORD ucValue);
	int UpdateTable_Val4(unsigned short uiRowNumber, UINT ucValue);
	int UpdateTable_Sel(unsigned short uiRowNumber, unsigned short uiColNumber, CString str_cellText);
	int UpdateTable_All(unsigned char * v_Value);

	// Grid events
	afx_msg int OnGridEndEdit(NMHDR * pNotifyStruct, LRESULT * pResult);

};

