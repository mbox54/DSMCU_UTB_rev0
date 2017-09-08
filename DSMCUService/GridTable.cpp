// class CGridTable implementation file

/////////////////////////////////////////////////////////////////////////////
// Includes
/////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "GridTable.h"

// ###################################################################
// CGridTable GridCtrl
// ###################################################################
CGridTable::CGridTable()
{

}

CGridTable::~CGridTable()
{
}

// Service
// NOTE:
// FORMAT of file: 
// ---------------------------------------------------------------
// File consist set of Lines. 
// Lines has many Types: *starter descript*, *null*, *label*, *value*, *space delimeter*
// /.../ - comment

// /0 Line/  *starter descript* *value* - File Type /MSA Table Description File/

// .. *space delimeter* ..

// *label* /[version]/
// /next Line/ *value* - version of File /ver 1.0/

// .. *space delimeter* ..

// *label* /[info]/
// /next Line/ *value* 1: column count
// /next Line/ *value* 2: fixed columns
// /next Line/ *value* 3: column headers, column width
// .. *(cols count) Times*
// /next Line/ *value* 4: rows count

// .. *space delimeter* ..

// *label* /[data]/
// /next Line/ *value* : rows values(col1 \n col2 \n col3 \n col4 \n)
// .. *(cols count) Times*
// .. .. *(rows count) Times*
// ---------------------------------------------------------------
int CGridTable::TableConfig_LoadFromFile(char * file_name)
{
	// > Read config from File

	FILE * fs;

	unsigned short uiRow;				// Table Cell Row number
	unsigned char  ucCol;				// Table Cell Col number

	// > Open File / Try
	errno_t err;
	if ((err = fopen_s(&fs, file_name, "r")) != 0)
	{
		// "can't read file" 
		return OP_FAILED;
	}

	// > Read File with Config and Data
	const unsigned short MAX_LINE_LENGTH = 256;
	char v_loadLine[MAX_LINE_LENGTH];

	CString cellText;					// Cell Value var

	// NOTE: MAX Line Length is MAX_LINE_LENGTH

	unsigned char  ucColCount = 0;		// Table Cell Col count
	unsigned short uiRowCount = 0;		// Table Cell Row count


		// # Proceed Line routine
		unsigned short k;			// line symbol var

		// > /Line 00/  *starter descript* *value* - File Type /MSA Table Description File/
		fgets(v_loadLine, MAX_LINE_LENGTH, fs);

		// (proc) NOP

		// .. *space delimeter* ..
		fgets(v_loadLine, MAX_LINE_LENGTH, fs);

		// > /Line NN/  *label* /[version]/
		fgets(v_loadLine, MAX_LINE_LENGTH, fs);

		// /next Line/ *value* - ver. NN
		fgets(v_loadLine, MAX_LINE_LENGTH, fs);

		// (proc) NOP

		// .. *space delimeter* ..
		fgets(v_loadLine, MAX_LINE_LENGTH, fs);

		// > /Line NN/  *label* / [info] /
		CString strLineInfo;
		fgets(v_loadLine, MAX_LINE_LENGTH, fs);

		// /next Line/ *comment* -  1: column count
		fgets(v_loadLine, MAX_LINE_LENGTH, fs);

		// /next Line/ *value*
		fgets(v_loadLine, MAX_LINE_LENGTH, fs);
		k = 0;
		strLineInfo.Truncate(0);
		while (v_loadLine[k] != '\n')
		{
			// SafeCheck
			if (v_loadLine[k] == '\0')
			{
				break;
			}

			strLineInfo.AppendChar(v_loadLine[k]);

			k++;
		}

		ucColCount = (WORD)_tcstoul(strLineInfo, NULL, 10);

		m_nFixRows = 1;
		m_nCols = ucColCount;	

		// /next Line/ *comment* -  2: fixed columns
		fgets(v_loadLine, MAX_LINE_LENGTH, fs);

		// /next Line/ *value* 
		fgets(v_loadLine, MAX_LINE_LENGTH, fs);
		k = 0;
		strLineInfo.Truncate(0);
		while (v_loadLine[k] != '\n')
		{
			// SafeCheck
			if (v_loadLine[k] == '\0')
			{
				break;
			}

			strLineInfo.AppendChar(v_loadLine[k]);

			k++;
		}

		m_nFixCols = (WORD)_tcstoul(strLineInfo, NULL, 10);

		m_nRows = 2;						// 1 Fix + 1 vacant

		// Set Table Size
		TRY
		{
			this->SetRowCount(m_nRows);
			this->SetColumnCount(m_nCols);
			this->SetFixedRowCount(m_nFixRows);
			this->SetFixedColumnCount(m_nFixCols);
		}
			CATCH(CMemoryException, e)
		{
			e->ReportError();
			return OP_FAILED;
		}
		END_CATCH

		// /next Line/ *comment* -  3: column headers, column width
		fgets(v_loadLine, MAX_LINE_LENGTH, fs);

		// /next Line/ *value* 
		CString strLineColHead;
		unsigned short uiColHeadLength;

		uiRow = 0;			// Set Header Row

		for (unsigned char k = 0; k < ucColCount; k++)
		{
			ucCol = k;

			// - column header
			if (fgets(v_loadLine, MAX_LINE_LENGTH, fs) == NULL)
			{
				fclose(fs);
				return OP_FILE_NODATA;
			}

			unsigned short kk = 0;			// local line symbol number var

			// build value string
			strLineColHead.Truncate(0);
			while (v_loadLine[kk] != '\n')
			{
				// SafeCheck
				if (v_loadLine[kk] == '\0')
				{
					break;
				}

				strLineColHead.AppendChar(v_loadLine[kk]);

				kk++;
			}

			// adapt to set Value
			cellText.Truncate(0);
			cellText.Append(strLineColHead);

			// set in Table
			GV_ITEM Item;

			Item.mask = GVIF_TEXT;
			Item.row = uiRow;
			Item.col = ucCol;
			Item.strText = cellText;

			this->SetItem(&Item);
			//this->SetItemText(uiRow, ucCol, cellText);

			// - column length
			if (fgets(v_loadLine, MAX_LINE_LENGTH, fs) == NULL)
			{
				fclose(fs);
				return OP_FILE_NODATA;
			}

			// build value string
			kk = 0;
			strLineColHead.Truncate(0);
			while (v_loadLine[kk] != '\n')
			{
				// SafeCheck
				if (v_loadLine[kk] == '\0')
				{
					break;
				}

				strLineColHead.AppendChar(v_loadLine[kk]);

				kk++;
			}

			// adapt to set Value
			uiColHeadLength = (WORD)_tcstoul(strLineColHead, NULL, 10);

			// set in Table
			this->SetColumnWidth(k, uiColHeadLength);
		}	// for: column parse

		// /next Line/ *comment* - 4: rows count
		fgets(v_loadLine, MAX_LINE_LENGTH, fs);

		// /next Line/ *value* 
		fgets(v_loadLine, MAX_LINE_LENGTH, fs);
		k = 0;
		strLineInfo.Truncate(0);
		while (v_loadLine[k] != '\n')
		{
			// SafeCheck
			if (v_loadLine[k] == '\0')
			{
				break;
			}

			strLineInfo.AppendChar(v_loadLine[k]);

			k++;
		}

		uiRowCount = (WORD)_tcstoul(strLineInfo, NULL, 10);

		m_nRows = uiRowCount + 1;

		// Set Table Size
		TRY
		{
			this->SetRowCount(m_nRows);
			//this->SetColumnCount(m_nCols);
			//this->SetFixedRowCount(m_nFixRows);
			//this->SetFixedColumnCount(m_nFixCols);
		}
			CATCH(CMemoryException, e)
		{
			e->ReportError();
			return OP_FAILED;
		}
		END_CATCH

		// .. *space delimeter* ..
		fgets(v_loadLine, MAX_LINE_LENGTH, fs);


		// search Pattern string
		// // get line str
		CString strPattern = _T("[data]");

		unsigned char ucPatternLength = strPattern.GetLength();

		unsigned char ACT = 1;
		while (ACT)		// Read until key
		{
			// > /Line NN/  *label* / [data]
			if (fgets(v_loadLine, MAX_LINE_LENGTH, fs) == NULL)
			{
				// DECISION: alternative to "return" is var that keep error code at the last end.
				// ACT = 0;
				fclose(fs);
				return OP_FILE_NODATA;
			}
			

			// > compare to pattern
			CString strLine;

			for (unsigned char k = 0; k < ucPatternLength; k++)
			{
				strLine.AppendChar(v_loadLine[k]);
			}

			if (strPattern.Compare(strLine) == 0)
			{
				// pattern successed
				ACT = 0;
			}
		} // while [data]

		// /next Line/ *value* x(RowCount) : rows values(col1, col2, col3, col4)
		for (unsigned short k = 0; k < uiRowCount; k++)
		{
			uiRow = k + 1;

			// > Parse string set
			for (unsigned short kk = 0; kk < ucColCount; kk++)
			{
				ucCol = kk;

				if (fgets(v_loadLine, MAX_LINE_LENGTH, fs) == NULL)
				{
					fclose(fs);
					return OP_FILE_NODATA;
				}

				unsigned short k2 = 0;
				CString strLineCol;
				while (v_loadLine[k2] != '\n')
				{
					// SafeCheck
					if (v_loadLine[k2] == '\0')
					{
						break;
					}

					strLineCol.AppendChar(v_loadLine[k2]);

					k2++;
				}

				// adapt to set Value
				cellText.Truncate(0);
				cellText.Append(strLineCol);

				// set in Table
				GV_ITEM Item;

				Item.mask = GVIF_TEXT;
				Item.row = uiRow;
				Item.col = ucCol;
				Item.strText = cellText;

				this->SetItem(&Item);

			} // for Parse 
			
			// Line \n ender
			if (fgets(v_loadLine, MAX_LINE_LENGTH, fs) == NULL)
			{
				fclose(fs);
				return OP_FILE_NODATA;
			}

		} // for rows values(col1 \n col2 \n col3 \n col4 \n)
	
	fclose(fs);


	return OP_SUCCESS;
}


void CGridTable::Init(char * fileName, unsigned char ucType)
{
	if (ucType == 0)
	{
		// Define Grid table common parameters 
		Config();
		TableConfig_LoadFromFile(fileName);
	}

}

void CGridTable::Config()
{
	m_OldSize = CSize(-1, -1);

	//{{AFX_DATA_INIT(CGridCtrlDemoDlg)

	m_bEditable = TRUE;
	m_bHorzLines = FALSE;
	m_bVertLines = FALSE;
	m_bListMode = TRUE;
	m_bHeaderSort = FALSE;
	m_bSingleSelMode = TRUE;
	m_bSingleColSelMode = TRUE;
	m_bSelectable = FALSE;
	m_bAllowColumnResize = FALSE;
	m_bAllowRowResize = FALSE;
	m_bItalics = FALSE;
	m_btitleTips = FALSE;
	m_bTrackFocus = FALSE;
	m_bFrameFocus = FALSE;
	m_bVirtualMode = FALSE;
	m_bCallback = TRUE;
	m_bVertical = TRUE;
	//m_bExpandUseFixed = FALSE;
	m_bExpandUseFixed = TRUE;

	m_bRejectEditAttempts = TRUE;
	m_bRejectEditChanges = FALSE;
	m_bRow2Col2Hidden = FALSE;

	// Set
	this->SetVirtualMode(m_bVirtualMode);
	this->SetEditable(m_bEditable);

	this->SetAutoSizeStyle();

}

// Use: Update Grid[0xFF x 0xFF] Interface 
// FROM: startAddr TO: startAddr + count 
// with Memory Values[0xFF x 0xFF] 
// FROM: startAddr TO: startAddr + count
void CGridTable::GridTable_Write_UpdateRange(BYTE * v_ByteData, BYTE startAddr, WORD count)
{
	// Safe check
	if (startAddr + count > 256)
	{
		// out of range
		return;
	}
	else
	{
		// define table start cell
		unsigned char stRow = startAddr / 0x10;				// start Row
		unsigned char stCol = startAddr - stRow * 0x10;		// start Col

															// fill in table
		unsigned char uRow = stRow + 1;						// +1 is for Fixed Cell
		unsigned char uCol = stCol + 1;						// +1 is for Fixed Cell

		unsigned int k = 0;

		bool act = 1;
		do
		{
			// forming Cell Text
			CString cellText;
			cellText.Format(_T("%02X"), v_ByteData[startAddr + k]);
			this->SetItemText(uRow, uCol, cellText);

			// prepare next cell coord	/NOTE: useless at last cycle
			uCol++;

			if (uCol > 0x0F + 1)							// +1 is for Fixed Cell
			{
				uCol = 0 + 1;								// +1 is for Fixed Cell
				uRow++;										// can't exceed 0x0F val cause of #SafeCheck
			}

			k++;

		} while (k < count);

		this->Invalidate();
	} //SafeCheck
}

void CGridTable::GridTable_Write(BYTE * v_ByteData, BYTE startAddr, WORD count)
{
	// Safe check
	if (startAddr + count > 256)
	{
		// out of range
		return;
	}
	else
	{
		// define table start cell
		unsigned char stRow = startAddr / 0x10;				// start Row
		unsigned char stCol = startAddr - stRow * 0x10;		// start Col

															// fill in table
		unsigned char uRow = stRow + 1;						// +1 is for Fixed Cell
		unsigned char uCol = stCol + 1;						// +1 is for Fixed Cell

		unsigned int k = 0;

		bool act = 1;
		do
		{
			// forming Cell Text
			CString cellText;
			cellText.Format(_T("%02X"), v_ByteData[k]);
			this->SetItemText(uRow, uCol, cellText);

			// prepare next cell coord	/NOTE: useless at last cycle
			uCol++;

			if (uCol > 0x0F + 1)							// +1 is for Fixed Cell
			{
				uCol = 0 + 1;								// +1 is for Fixed Cell
				uRow++;										// can't exceed 0x0F val cause of #SafeCheck
			}

			k++;

		} while (k < count);

		this->Invalidate();
	} //SafeCheck
}

void CGridTable::GridTable_Write_Byte(BYTE * v_ByteData, BYTE startAddr, WORD count)
{
	// Safe check
	if (startAddr + count > 256)
	{
		// out of range
		return;
	}
	else
	{
		// define table start cell
		unsigned char stRow = startAddr / 0x10;				// start Row
		unsigned char stCol = startAddr - stRow * 0x10;		// start Col

															// fill in table
		unsigned char uRow = stRow + 1;						// +1 is for Fixed Cell
		unsigned char uCol = stCol + 1;						// +1 is for Fixed Cell

		unsigned int k = 0;

		bool act = 1;
		do
		{
			// forming Cell Text
			CString cellText;
			cellText.Format(_T("%c"), v_ByteData[startAddr + k]);
			this->SetItemText(uRow, uCol, cellText);

			// prepare next cell coord	/NOTE: useless at last cycle
			uCol++;

			if (uCol > 0x0F + 1)							// +1 is for Fixed Cell
			{
				uCol = 0 + 1;								// +1 is for Fixed Cell
				uRow++;										// can't exceed 0x0F val cause of #SafeCheck
			}

			k++;

		} while (k < count);

		this->Invalidate();
	} //SafeCheck
}

// Use: Update Memory Values
// FROM: startAddr TO: startAddr + count
// with Grid Values
// FROM: startAddr TO: startAddr + count
void CGridTable::GridTable_Read_UpdateRange(BYTE * v_ByteData, BYTE startAddr, WORD count)
{
	// Safe check
	if (startAddr + count > 256)
	{
		// out of range
		return;
	}
	else
	{
		// define table start cell
		unsigned char stRow = startAddr / 0x10;				// start Row
		unsigned char stCol = startAddr - stRow * 0x10;		// start Col

															// get from Table
		unsigned char uRow = stRow + 1;						// +1 is for Fixed Cell
		unsigned char uCol = stCol + 1;						// +1 is for Fixed Cell

		unsigned int k = 0;

		bool act = 1;
		do
		{
			// extract Cell Text
			CString cellText = this->GetItemText(uRow, uCol);
			v_ByteData[startAddr + k] = (BYTE)_tcstoul(cellText, NULL, 16);

			// prepare next cell coord	/NOTE: useless at last cycle
			uCol++;

			if (uCol > 0x0F + 1)							// +1 is for Fixed Cell
			{
				uCol = 0 + 1;								// +1 is for Fixed Cell
				uRow++;										// can't exceed 0x0F val cause of #SafeCheck
			}

			k++;

		} while (k < count);

	}
}

// Use: simple Read Grid OP
// FROM: startAddr TO: startAddr + count
// in Memory
// FROM: 0 TO: count
void CGridTable::GridTable_Read(BYTE * v_ByteData, BYTE startAddr, WORD count)
{
	// Safe check
	if (startAddr + count > 256)
	{
		// out of range
		return;
	}
	else
	{
		// define table start cell
		unsigned char stRow = startAddr / 0x10;				// start Row
		unsigned char stCol = startAddr - stRow * 0x10;		// start Col

															// get from Table
		unsigned char uRow = stRow + 1;						// +1 is for Fixed Cell
		unsigned char uCol = stCol + 1;						// +1 is for Fixed Cell

		unsigned int k = 0;

		bool act = 1;
		do
		{
			// extract Cell Text
			CString cellText = this->GetItemText(uRow, uCol);
			v_ByteData[k] = (BYTE)_tcstoul(cellText, NULL, 16);

			// prepare next cell coord	/NOTE: useless at last cycle
			uCol++;

			if (uCol > 0x0F + 1)							// +1 is for Fixed Cell
			{
				uCol = 0 + 1;								// +1 is for Fixed Cell
				uRow++;										// can't exceed 0x0F val cause of #SafeCheck
			}

			k++;

		} while (k < count);

	}
}

// -------------------------------------------------------------------
// Supporting procedures
// -------------------------------------------------------------------

// Check PROCs


int CGridTable::CheckValidHex(int iRow, int iCol)
{
	// error symbol at first
	bool act = 1;

	CString cellText = this->GetItemText(iRow, iCol);

	// check valid Length
	if (cellText.GetLength() == 2)
	{
		// check valid Symbols
		CString hexString = cellText.SpanIncluding(_T("0123456789abcdefABCDEF"));

		if (hexString.Compare(cellText) == 0)
		{
			act = 0;

			// Change to upper case
			this->SetItemText(iRow, iCol, cellText.MakeUpper());
		}
	}

	return act;
}

int CGridTable::CheckValidHex2(int iRow, int iCol)
{
	// error symbol at first
	bool act = 1;

	CString cellText = this->GetItemText(iRow, iCol);

	// check valid Length
	if (cellText.GetLength() == 4)
	{
		// check valid Symbols
		CString hexString = cellText.SpanIncluding(_T("0123456789abcdefABCDEF"));

		if (hexString.Compare(cellText) == 0)
		{
			act = 0;

			// Change to upper case
			this->SetItemText(iRow, iCol, cellText.MakeUpper());
		}
	}

	return act;
}

int CGridTable::CheckValidASCII(int iRow, int iCol)
{
	bool act = 0;

	CString cellText = this->GetItemText(iRow, iCol);

	// check valid Length
	if (cellText.GetLength() != 1)
	{
		// error symbol
		act = 1;
	}

	return act;
}

int CGridTable::ByteToBitStr(BYTE uByteValue, unsigned char * v_BitStrValue)
{
	BYTE byte_var = 128;			// 2^(8-1)

	for (int k = 0; k < 8; k++)
	{
		if (uByteValue >= byte_var)
		{
			v_BitStrValue[k] = '1';
			uByteValue -= byte_var;
		}
		else
		{
			v_BitStrValue[k] = '0';
		}

		byte_var /= 2;
	}

	return 0;

}

int CGridTable::BitStrToByte(BYTE * uByteValue, unsigned char * v_BitStrValue)
{

	BYTE byte_var = 1;
	BYTE byteValue_loc = 0;

	for (int k = 0; k < 8; k++)
	{
		if (v_BitStrValue[7 - k] == '0')
		{
			//
		}
		else
		{
			if (v_BitStrValue[7 - k] == '1')
			{
				byteValue_loc += byte_var;
			}
			else
			{
				// bad parameter value
				return 2;
			}
		}

		byte_var *= 2;
	}

	*uByteValue = byteValue_loc;


	return 0;
}

int CGridTable::CheckValidChar(int iRow, int iCol)
{
	// set error flag at first
	bool act = 1;

	CString cellText = this->GetItemText(iRow, iCol);

	// check valid Length
	if (cellText.GetLength() == 1)
	{
		act = 0;
	}

	return act;
}

int CGridTable::CheckValidBitStr8(int iRow, int iCol)
{
	// set error flag at first
	bool act = 1;

	CString cellText = this->GetItemText(iRow, iCol);

	// check valid Length
	if (cellText.GetLength() == 8)
	{
		// valid Length
		// check valid Symbol
		act = 0;

		for (unsigned char k = 0; k < 8; k++)
		{
			if (!((cellText[k] == '0') || (cellText[k] == '1')))
			{
				// invalid symbol
				act = 1;
				break;
			}

		}
	}

	return act;
}

int CGridTable::CheckValidBitStr16(int iRow, int iCol)
{
	// set error flag at first
	bool act = 1;

	CString cellText = this->GetItemText(iRow, iCol);

	// check valid Length
	if (cellText.GetLength() == 16)
	{
		// valid Length
		// check valid Symbol
		act = 0;

		for (unsigned char k = 0; k < 16; k++)
		{
			if (!((cellText[k] == '0') || (cellText[k] == '1')))
			{
				// invalid symbol
				act = 1;
				break;
			}

		}
	}

	return act;
}


/*

// Get GPIO vals from ini 
int CPinConfigurationPage::GetGPIOini(BYTE *gpio_val)
{
	FILE *fs;
	char* file_name = "cpiodirection.cfg";

	errno_t err;
	if ((err = fopen_s(&fs, file_name, "r")) != 0)
	{
		// "can't read file" 
		return 1;
	}
	else
	{
		char load_str[9] = "00000000";
		fgets(load_str, 9, fs);
		fclose(fs);

		int file_val = 0;

		int k = 1;
		for (int i = 0; i < 8; i++)
		{
			if (load_str[7 - i] == '0')
			{
				//	
			}
			else
			{
				if (load_str[7 - i] == '1')
				{
					file_val += k;
				}
				else
				{
					//bad file
					return 2;
				}
			}

			k = k * 2;
		}

		if (file_val < 256) {
			*gpio_val = (BYTE)file_val;
		}
		else {
			return 3;
		}
	}

	return 0;
}



*/