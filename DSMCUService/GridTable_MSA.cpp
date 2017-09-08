// class CGridTable_MSA implementation file



#include "stdafx.h"
#include "GridTable_MSA.h"

// ###################################################################
// CGridTable_MSA GridTable_MSA
// ###################################################################
CGridTable_MSA::CGridTable_MSA()
{
}


CGridTable_MSA::~CGridTable_MSA()
{
}

// NOTE:
// FORMAT:
// # Orange:	/Vendor Name/	:		20-35	/254, 232, 185
// # Blue:		/Module Type/   :		40-55	/174, 238, 238
// # Red:		/Serial Number/ :		68-83	/255, 193, 194
// # Magenta:	/Data Code/		:		84-91	/238, 174, 235
// # Green:		/Checksum/		:		63, 95	/147, 243, 146
// #			/Length/		:		14-15
// # 

//
// Set Cell Colours
void CGridTable_MSA::SetTableColor()
{
	//GV_ITEM * pItem = NULL;
		
	// > Get Cell Resource
	//this->GetItem(pItem);

	//pItem->col = 2;
	//pItem->row = 2;

	// > Define Colours
	COLORREF clr_Red_head = RGB(255, 193, 194);
	COLORREF clr_Red_body = RGB(255, 223, 224);

	COLORREF clr_Orange_head = RGB(254, 232, 185);
	COLORREF clr_Orange_body = RGB(254, 252, 225);

	COLORREF clr_Blue_head = RGB(174, 238, 238);
	COLORREF clr_Blue_body = RGB(204, 255, 255);

	COLORREF clr_Magenta_head = RGB(238, 174, 235);
	COLORREF clr_Magenta_body = RGB(255, 234, 255);

	COLORREF clr_Green_head = RGB(167, 243, 166);
	COLORREF clr_Green_body = RGB(207, 250, 206);

	// > Set Cell Resource

	// # Orange:	/Vendor Name/	:		20-35	/254, 232, 185
	for (unsigned char k = 0; k < 16; k++)
	{
		// header
		this->SetItemBkColour(k + 20 + 1, 0, clr_Orange_head);

		// body
		for (unsigned char kk = 1; kk < 6; kk++)
		{
			this->SetItemBkColour(k + 20 + 1, kk, clr_Orange_body);
		}
	}
	
	// # Blue:		/Module Type/   :		40-55	/174, 238, 238
	for (unsigned char k = 0; k < 16; k++)
	{
		// header
		this->SetItemBkColour(k + 40 + 1, 0, clr_Blue_head);

		// body
		for (unsigned char kk = 1; kk < 6; kk++)
		{
			this->SetItemBkColour(k + 40 + 1, kk, clr_Blue_body);
		}
	}

	// # Red:		/Serial Number/ :		68-83	/255, 193, 194
	for (unsigned char k = 0; k < 16; k++)
	{
		// header
		this->SetItemBkColour(k + 68 + 1, 0, clr_Red_head);

		// body
		for (unsigned char kk = 1; kk < 6; kk++)
		{
			this->SetItemBkColour(k + 68 + 1, kk, clr_Red_body);
		}
	}

	// # Magenta:	/Data Code/		:		84-91	/238, 174, 235
	for (unsigned char k = 0; k < 8; k++)
	{
		// header
		this->SetItemBkColour(k + 84 + 1, 0, clr_Magenta_head);

		// body
		for (unsigned char kk = 1; kk < 6; kk++)
		{
			this->SetItemBkColour(k + 84 + 1, kk, clr_Magenta_body);
		}
	}

	// # Green:		/Checksum/		:		63, 95	/147, 243, 146
	// CHK63
	// header
	this->SetItemBkColour(63 + 1, 0, clr_Green_head);

	// body
	for (unsigned char kk = 1; kk < 6; kk++)
	{
		this->SetItemBkColour(63 + 1, kk, clr_Green_body);
	}

	// CHK95
	// header
	this->SetItemBkColour(95 + 1, 0, clr_Green_head);

	// body
	for (unsigned char kk = 1; kk < 6; kk++)
	{
		this->SetItemBkColour(95 + 1, kk, clr_Green_body);
	}

	//pItem->crBkClr = clr;             // or - m_Grid.SetItemBkColour(row, col, clr);
	// Item.crFgClr = RGB(255, 0, 0);    // or - m_Grid.SetItemFgColour(row, col, RGB(255,0,0));
	//pItem->mask |= (GVIF_BKCLR | GVIF_FGCLR);
	

	
//	SetItem(pItem);

}

// Table procedures
int CGridTable_MSA::UpdateTable_Val(unsigned short uiRowNumber, BYTE ucValue)
{
	// > set Values
	CString str_cellText;
	CString str;
	BYTE byte_cellText;
	int iProcSuccess;

	// Column 2 - HEX Value
	str.Format(_T("%02X"), ucValue);

	SetItemText(uiRowNumber + 1, 2, str);

	// Column 3 - Bit Value
	unsigned char v_BitStr[8]; // = { 0, 0, 0, 0, 0, 0, 0, 0 };

	iProcSuccess = this->ByteToBitStr(ucValue, v_BitStr);

	str.Truncate(0);
	for (unsigned char k = 0; k < 8; k++)
	{
		str.AppendChar(v_BitStr[k]);
	}

	SetItemText(uiRowNumber + 1, 3, str);

	// Column 4 - ASCII Value
	str.Truncate(0);
	str.Format(_T("%c"), ucValue);

	SetItemText(uiRowNumber + 1, 4, str);


	return 0;
}

int CGridTable_MSA::UpdateTable_Sel(unsigned short uiStartNumber, unsigned short uiCount, unsigned char * v_Value)
{
	for (unsigned short k = 0; k < uiCount; k++)
	{
		this->UpdateTable_Val(uiStartNumber + k, v_Value[k]);
	}

	return 0;
}

int CGridTable_MSA::UpdateTable_All(unsigned char * v_Value)
{
	this->UpdateTable_Sel(0, 128, v_Value);

	return 0;
}




// Grid events
int CGridTable_MSA::OnGridEndEdit(NMHDR * pNotifyStruct, LRESULT * pResult)
{
	// check global enable flag
	if (this->m_bRejectEditChanges)
	{
		*pResult = -1;
		return 1;
	}

	NM_GRIDVIEW* pItem = (NM_GRIDVIEW*)pNotifyStruct;
	//Trace(_T("End Edit on row %d, col %d\n"), pItem->iRow, pItem->iColumn);

	// > check Column
	// NOTE: we may edit next Columns:
	// FORMAT:
	// Column 0 - Byte Number		| disable
	// Column 1 - Byte Name			| disable in this rev. (discussed)
	// Column 2 - Byte Value		| edited, HEX value of 1 byte data, HH
	// Column 3 - Bit  Value		| edited, Bit value of 1 byte data, bbbbbbbb
	// Column 4 - Byte Value(ASCII)	| edited, CHAR value for 1 symbol, C 

	// when Col2 is edited, Col3 updates its value
	// when Col3 is edited, Col2 updates its value 

	// > check Valid Input 
	bool bInvalid = 0;
	switch (pItem->iColumn)
	{
	case 0:
		bInvalid = 1;

		break;

	case 1:
		bInvalid = 1;

		break;

	case 2:	// HEX Value
		bInvalid = this->CheckValidHex(pItem->iRow, pItem->iColumn);

		break;

	case 3:
		bInvalid = this->CheckValidBitStr8(pItem->iRow, pItem->iColumn);

		break;

	case 4:	// ASCII Value
		bInvalid = this->CheckValidChar(pItem->iRow, pItem->iColumn);

		break;

	default:
		bInvalid = 1;

		break;
	}

	// check valid input
	if (bInvalid)
	{
		*pResult = -1;
		return 1;
	}

	// Approve valid editing for Column
	*pResult = 0;

	// > proceed OP 
	CString str_cellText;
	CString str;
	BYTE byte_cellText;
	unsigned char v_BitStr[8] = {0, 0, 0, 0, 0, 0, 0, 0};

	switch (pItem->iColumn)
	{
	case 2:	// HEX Value
		// > update Col3 - ASCII Value
		// read
		str_cellText = this->GetItemText(pItem->iRow, pItem->iColumn);

		// transfer
		byte_cellText = (BYTE)_tcstoul(str_cellText, NULL, 16);

		// set
		UpdateTable_Val(pItem->iRow - 1, byte_cellText);

		//str.Format(_T("%c"), byte_cellText);

		// set
		//SetItemText(pItem->iRow, 4, str);

		break;

	case 3:	// Bit Value
			// read
		str_cellText = GetItemText(pItem->iRow, pItem->iColumn);

		// transfer
		for (unsigned char k = 0; k < 8; k++)
		{
			v_BitStr[k] = str_cellText[k];
		}

		this->BitStrToByte(&byte_cellText, v_BitStr);

		// set
		UpdateTable_Val(pItem->iRow - 1, byte_cellText);

		break;

	case 4:	// ASCII Value
		// read
		str_cellText = GetItemText(pItem->iRow, pItem->iColumn);

		// transfer
		// set
		UpdateTable_Val(pItem->iRow - 1, str_cellText[0]);

		break;

	default:

		break;
	}

	// refresh component
	Invalidate(0);

	return 0;
}

