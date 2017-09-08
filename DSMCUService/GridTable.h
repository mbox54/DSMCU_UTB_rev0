// class CGridTable header file

// Inherited from CGridCtrl class
// Class purpose is: 
// - define specific Table for custom input/outpit

#pragma once

/////////////////////////////////////////////////////////////////////////////
// Includes
/////////////////////////////////////////////////////////////////////////////
#include "GridRes\GridCtrl.h"

/////////////////////////////////////////////////////////////////////////////
// Definitions
/////////////////////////////////////////////////////////////////////////////

#define OP_SUCCESS				0x00
#define OP_FAILED				0x01
#define OP_FILE_WRONG			0x02
#define OP_FILE_NODATA			0x03


class CGridTable : public CGridCtrl
{
public:
	// standard constructor
	CGridTable();
	~CGridTable();

	// methods
	int TableConfig_LoadFromFile(char * file_name);
	void Init(char * fileName, unsigned char ucType = 0);
	void Config();

	void GridTable_Write_UpdateRange(BYTE * v_ByteData, BYTE startAddr, WORD count);
	void GridTable_Write(BYTE * v_ByteData, BYTE startAddr, WORD count);
	void GridTable_Write_Byte(BYTE * v_ByteData, BYTE startAddr, WORD count);
	void GridTable_Read_UpdateRange(BYTE * v_ByteData, BYTE startAddr, WORD count);
	void GridTable_Read(BYTE * v_ByteData, BYTE startAddr, WORD count);

	// Supporting procedures
	// // check
	int	CheckValidHex(int iRow, int iCol);
	int CheckValidHex2(int iRow, int iCol);

	int	CheckValidASCII(int iRow, int iCol);

	int CheckValidChar(int iRow, int iCol);

	int CheckValidBitStr8(int iRow, int iCol);
	int CheckValidBitStr16(int iRow, int iCol);

	// // convert
	int ByteToBitStr(BYTE uByteValue, unsigned char * v_BitStrValue);
	int BitStrToByte(BYTE * uByteValue, unsigned char * v_BitStrValue);

	// size
	CSize m_OldSize;

	// Cells
	int		m_nFixCols;
	int		m_nFixRows;
	int		m_nCols;
	int		m_nRows;

	// Properties
	BOOL	m_bEditable;
	BOOL	m_bHorzLines;
	BOOL	m_bListMode;
	BOOL	m_bVertLines;
	BOOL	m_bSelectable;
	BOOL	m_bAllowColumnResize;
	BOOL	m_bAllowRowResize;
	BOOL	m_bHeaderSort;
	BOOL	m_bItalics;
	BOOL	m_btitleTips;
	BOOL	m_bSingleSelMode;
	BOOL    m_bSingleColSelMode;
	BOOL	m_bTrackFocus;
	BOOL	m_bFrameFocus;
	BOOL	m_bVirtualMode;
	BOOL	m_bCallback;
	BOOL    m_bVertical;
	BOOL    m_bExpandUseFixed;
	BOOL    m_bRejectEditAttempts;
	BOOL    m_bRejectEditChanges;
	BOOL    m_bRow2Col2Hidden;

};


