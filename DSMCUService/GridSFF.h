// class CGridSFF header file

// Inherited from CGridCtrl class
// Class purpose is: 
// - define specific Table for SFF output

#pragma once

#include "GridRes\GridCtrl.h"

class CGridSFF : public CGridCtrl
{
public:
	// standard constructor
	CGridSFF();
	~CGridSFF();

	// methods
	void Init();
	void Config();
	void SetTableColor();

	void ClearCells(BYTE startAddr, WORD count);
	void ClearTable();
	void GridSFF_Write_UpdateRange(BYTE * v_ByteData, BYTE startAddr, WORD count);
	void GridSFF_Write(BYTE * v_ByteData, BYTE startAddr, WORD count);
	void GridSFF_Write_Byte(BYTE * v_ByteData, BYTE startAddr, WORD count);
	void GridSFF_Read_UpdateRange(BYTE * v_ByteData, BYTE startAddr, WORD count);
	void GridSFF_Read(BYTE * v_ByteData, BYTE startAddr, WORD count);
	int	 CheckValidHex(int iRow, int iCol);
	int	 CheckValidASCII(int iRow, int iCol);

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


