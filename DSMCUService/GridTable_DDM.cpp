// class CGridTable_DDM implementation file



#include "stdafx.h"
#include "GridTable_DDM.h"
#include <locale.h> 


CGridTable_DDM::CGridTable_DDM()
{
}


CGridTable_DDM::~CGridTable_DDM()
{
}

// Define Row Range Number from RowNumber
unsigned short CGridTable_DDM::GetRangeNumber(unsigned short uiRowNumber, unsigned char * err_)
{
	// init error Value
	*err_ = 0;
	
	// define Set intersect
	unsigned short index_var = 0;
	unsigned short index_mst = 0;		// milestone

	unsigned char ACT = 1;
	while (ACT)
	{
		// check set intersect
		if (uiRowNumber <= v_RowSet_Index[index_mst])		// +1 need for FixHeader 1 Row padding
		{
			// [OP ready]
			ACT = 0;

			return index_mst;
		}
		else
		{
			// [OP continue]
			index_mst++;

			// SafeCheck
			if (index_mst > 5)
			{
				// err
				*err_ = 1;

				// exit
				ACT = 0;
			}
		}

	} //while // > Define Edit Set

	return 0;
}

// Get Row Number from Byte Number
// option: get byte paddind from Row starting value
unsigned short CGridTable_DDM::GetRowNumber(unsigned short uiByteNumber, unsigned char * ucRowBytePadding, unsigned char * err_)
{
	unsigned char ucByteRangeLeftMst = 0;
	unsigned char index_mst = 0;
	unsigned short byte_mst = 0;
	unsigned short row_number = 0;

	unsigned char ACT = 1;
	unsigned char ucRowRange = v_RowSet_Index[0] + 1;			// +1 to shift from 00 addr

	byte_mst = ucRowRange * v_RowSet_Sizes[0];

	while (ACT)
	{
		// define Byte Range	 

		if (uiByteNumber <= byte_mst)
		{
			// [range defined]

			ACT = 0;
		}
		else
		{
			// [next range]
			index_mst++;

			// update from last cycle
			row_number += ucRowRange;
			ucByteRangeLeftMst = byte_mst;

			// calc range row value
			ucRowRange = v_RowSet_Index[index_mst] - v_RowSet_Index[index_mst - 1];
	
			// new cycle calc 
			byte_mst += ucRowRange * v_RowSet_Sizes[index_mst];

		} // if (uiNumber <= byte_number)

	} // while (ACT)

	  // define Row number	
	unsigned char ucByteLeftMst = 0;
	unsigned char byte_residual = 0;

	byte_residual = uiByteNumber - ucByteRangeLeftMst;

	
	unsigned char row_residual = byte_residual / v_RowSet_Sizes[index_mst];
	unsigned char ucRowBytePadding_ = 0;

	row_number += row_residual;

	ucRowBytePadding_ = byte_residual - row_residual * v_RowSet_Sizes[index_mst];
	*ucRowBytePadding = ucRowBytePadding_;

	return row_number;
}

// Get BYTE Number for RowNumber
// optional: get index_mst
unsigned short CGridTable_DDM::GetByteNumber(unsigned short uiRowNumber, unsigned short * index_mst, unsigned char * err_)
{
	// get Range Number
	unsigned char err = 0;
	unsigned short uiIndex_mst = this->GetRangeNumber(uiRowNumber, &err);

	*index_mst = uiIndex_mst;

	unsigned short row_number = 0;
	unsigned short byte_mst = 0;
	//unsigned char ucByteRangeLeftMst = 0;

	// Byte proceed
	unsigned short range_curr = 0;
	unsigned char ACT = 1;
	while (ACT)
	{
		// define Byte Range
		unsigned char ucByteRange = 0;

		// check exit
		if (uiIndex_mst <= range_curr)
		{
			// [range defined]

			ACT = 0;
		}
		else
		{
			// [next range]

			// perform byte accumulation
			if (range_curr == 0)
			{
				ucByteRange = v_RowSet_Index[range_curr] - 0 + 1;			// +1 to shift from 00 addr
			}
			else
			{
				ucByteRange = v_RowSet_Index[range_curr] - v_RowSet_Index[range_curr - 1];
			}

			byte_mst += ucByteRange * v_RowSet_Sizes[range_curr];

			range_curr++;

		} // if (uiNumber <= byte_number)

	} // while (ACT)

	// define Row number	
	unsigned char ucRowLeftMst = 0;
	unsigned char row_residual = 0;

	if (uiIndex_mst == 0)
	{
		ucRowLeftMst = 0;

		row_residual = uiRowNumber - ucRowLeftMst;
	}
	else
	{
		ucRowLeftMst = v_RowSet_Index[uiIndex_mst - 1];

		row_residual = uiRowNumber - ucRowLeftMst - 1;			// -1 for shift to Range start addr
	}

	byte_mst += row_residual * v_RowSet_Sizes[uiIndex_mst];

	return byte_mst;
}

// Proceed Structure Value
// NOTE:
// param description:
// uiNumber:		Row/Byte number
// modeTypeNum:		numeration mode (0 = Row, 1 = Byte) 
// modeOpType:		direction (0 = save, 1 = load)
int CGridTable_DDM::ProcValue(unsigned short uiNumber, unsigned char modeNumType, unsigned char * v_Values, unsigned char * v_Buf, unsigned char modeOpType)
{

	return 0;
}

// Proceed Table Value
// NOTE:
// param description:
// uiNumber:		Row/Byte number // !numeration from 0
// modeTypeNum:		numeration mode (0 = Row, 1 = Byte) 
// modeOpType:		direction (0 = save, 1 = load)
// NOTE:
// when modeNumType = 0, OUTPUT size is in accordance with calced RowType, placed in v_Buf[0 - NN] 
// when modeNumType = 1, 
// // OUTPUT size is 1 BYTE, placed in v_Buf[0], 
// // Row number placed in v_Buf[1-2], v_Buf[1] = LSB, v_Buf[2] = MSB,
// // Range number placed in v_Buf[3]. 

int CGridTable_DDM::ProcTable(unsigned short uiNumber, unsigned char modeNumType, unsigned char * v_Buf, unsigned char modeOpType)
{
	// > Define Row Type
	// NOTE: Table is NOT uniform
	// FORMAT:
	// Bytes 00 - 39:	Fields: 00 - 19		 /	size: 2, count: 20	/ Diagnostic Flag Alarm and Warning Thresholds (see Table 3.15)
	// Bytes 40 - 55:	Fields: 20 - 27		 /	size: 2, count: 08	/ Unlocated
	// Bytes 56 - 75:	Fields: 28 - 32		 /	size: 4, count: 05	/ Rx PWR Diagnostic calibration constants for optional External Calibration
	// Bytes 76 - 91:	Fields: 33 - 40		 /	size: 2, count: 08	/ Other  Diagnostic calibration constants for optional External Calibration
	// Bytes 92 - 94:	Fields: 41 - 43		 /	size: 1, count: 03	/ Unlocated
	// Bytes 95:		Fields: 44			 /	size: 1, count: 01	/ CHK of 00-94

	// define Set intersect
	unsigned short index_var = 0;
	unsigned short index_mst = 0;		// milestone
	unsigned short byte_mst = 0;
	unsigned short row_number = 0;

	if (modeNumType == 0)
	{
		// [Row mode]
		unsigned char err = 0;
		index_mst = GetRangeNumber(uiNumber, &err);

		if (err != 0)
		{
			// err, abort
			return 1;
		}

		// > Select OP Type
		if (modeOpType == 0)
		{
			// [Save OP]
		}
		else
		{
			if (modeOpType == 1)
			{
				// [Load OP]

				// get cell Text
				CString cellText = this->GetItemText(uiNumber + 1, 2);			// NOTE: +1 need for Header Row padding
				BYTE byte_value;
				WORD word_value;
				WORD word_temp;
				UINT uint_value;
				UINT uint_temp;

				// convert to Output Format
				switch (v_RowSet_Sizes[index_mst])
				{
				case 1: // 1 Byte var

					v_Buf[3] = 0;
					v_Buf[2] = 0;
					v_Buf[1] = 0;

					byte_value = (BYTE)_tcstoul(cellText, NULL, 16);

					v_Buf[0] = byte_value;

					break;

				case 2:	// 2 Bytes var

					v_Buf[3] = 0;
					v_Buf[2] = 0;

					word_value = (WORD)_tcstoul(cellText, NULL, 16);

					word_temp = word_value;
					word_temp &= 0x00FF;

					v_Buf[1] = word_temp;

					word_temp = word_value;
					word_temp &= 0xFF00;
					word_temp = word_temp >> 8;

					v_Buf[0] = word_temp;

					break;

				case 4: // 4 Bytes var

					uint_value = (UINT)_tcstoul(cellText, NULL, 16);

					uint_temp = uint_value;
					uint_temp &= 0x000000FF;

					v_Buf[3] = uint_temp;

					uint_temp = uint_value;
					uint_temp &= 0x0000FF00;
					uint_temp = uint_temp >> 8;

					v_Buf[2] = uint_temp;

					uint_temp = uint_value;
					uint_temp &= 0x00FF0000;
					uint_temp = uint_temp >> 16;

					v_Buf[1] = uint_temp;

					uint_temp = uint_value;
					uint_temp &= 0xFF000000;
					uint_temp = uint_temp >> 24;

					v_Buf[0] = uint_temp;

					break;

				default:
					// err

					break;
				}
			} // [Load OP]
		} // OP Select

	} // [Row mode]
	else
	{
		if (modeNumType == 1)
		{
			// [Byte mode]
			unsigned char err;
			unsigned char row_byteNumber = 0;
			unsigned char row_number = this->GetRowNumber(uiNumber, &row_byteNumber, &err);

			// > Select OP Type

			// set non-usage null Values
			v_Buf[3] = index_mst;

			// TODO: MSB byte now is deactive. So Row number is in [0 .. 255] Value range.
			// need to activate MSB to provide WORD [0 .. 65535] Values
			v_Buf[2] = 0;
			v_Buf[1] = row_number;

			if (modeOpType == 0)
			{
				// [Save OP]
				// get cell Text
				CString cellText = this->GetItemText(row_number + 1, 2);			// NOTE: +1 need for Header Row padding

				// define Range Number
				unsigned char err = 0;
				index_mst = GetRangeNumber(row_number, &err);

				// place Value
				BYTE byte_value;
				WORD word_value;
				WORD word_temp;
				WORD word_tempA;
				UINT uint_value;
				UINT uint_temp;
				UINT uint_tempA;

				CString str_value;

				switch (v_RowSet_Sizes[index_mst])
				{
				case 1: // 1 Byte var

					byte_value = v_Buf[0];

					// convert
					str_value.Truncate(0);
					str_value.Format(_T("%02X"), byte_value);

					break;

				case 2:	// 2 Bytes var
					// NOTE:
					// FORMAT: [ 0 | 1 ]
					word_value = (WORD)_tcstoul(cellText, NULL, 16);

					switch (row_byteNumber)
					{
					case 1: // [second Byte pos in Word]

						word_temp = word_value;
						word_temp &= 0xFF00;

						word_temp += v_Buf[0];

						break;

					case 0: // [first Byte pos in Word]

						word_temp = v_Buf[0];
						word_temp = word_temp << 8;

						word_tempA = word_value;
						word_tempA &= 0x00FF;

						word_temp += word_tempA;

						break;

					default:
						// err

						break;

					} // switch (row_byteNumber)

					// convert
					str_value.Truncate(0);
					str_value.Format(_T("%04X"), word_temp);

					break;

				case 4: // 4 Bytes var
						// NOTE:
						// FORMAT: [ 0 | 1 | 2 | 3 ]
					uint_value = (UINT)_tcstoul(cellText, NULL, 16);

					switch (row_byteNumber)
					{
					case 3: // [forth Byte pos in Uint]

						uint_temp = uint_value;
						uint_temp &= 0xFFFFFF00;

						uint_temp += v_Buf[0];

						break;

					case 2: // [third Byte pos in Uint]

						uint_temp = 0x00000000;

						uint_tempA = uint_value;
						uint_tempA &= 0xFFFF0000;
						uint_tempA = uint_tempA >> 16;

						uint_temp += uint_tempA;
						uint_temp = uint_temp << 8;

						uint_temp += v_Buf[0];
						uint_temp = uint_temp << 8;

						uint_tempA = uint_value;
						uint_tempA &= 0x000000FF;

						uint_temp += uint_tempA;

						break;

					case 1: // [second Byte pos in Uint]

						uint_temp = 0x00000000;

						uint_tempA = uint_value;
						uint_tempA &= 0xFF000000;
						uint_tempA = uint_tempA >> 24;

						uint_temp += uint_tempA;
						uint_temp = uint_temp << 8;

						uint_temp += v_Buf[0];
						uint_temp = uint_temp << 8;

						uint_tempA = uint_value;
						uint_tempA &= 0x0000FFFF;

						uint_temp += uint_tempA;

						break;

					case 0: // [first Byte pos in Uint]

						uint_temp = uint_value;
						uint_temp &= 0x000000FF;

						v_Buf[0] = uint_temp;

						break;

					default:
						// err

						break;

					} // switch (row_byteNumber)

					break;

				default:
					// err

					break;

				// convert
				str_value.Truncate(0);
				str_value.Format(_T("%08X"), word_temp);

				} // convert to Output Format

				// set
				UpdateTable_Sel(row_number, 2, str_value);				// Column 2 - HEX Value

			}
			else
			{
				if (modeOpType == 1)
				{
					// [Load OP]

					// get cell Text
					CString cellText = this->GetItemText(row_number + 1, 2);			// NOTE: +1 need for Header Row padding
					
					// define Range Number
					unsigned char err = 0;
					index_mst = GetRangeNumber(row_number, &err);

					// convert to Output Format
					BYTE byte_value;
					WORD word_value;
					WORD word_temp;
					UINT uint_value;
					UINT uint_temp;

					switch (v_RowSet_Sizes[index_mst])
					{
					case 1: // 1 Byte var

						byte_value = (BYTE)_tcstoul(cellText, NULL, 16);

						v_Buf[0] = byte_value;

						break;

					case 2:	// 2 Bytes var

						word_value = (WORD)_tcstoul(cellText, NULL, 16);

						switch (row_byteNumber)
						{
						case 1: // [second Byte pos in Word]
							
							word_temp = word_value;
							word_temp &= 0x00FF;

							v_Buf[0] = word_temp;

							break;

						case 0: // [first Byte pos in Word]

							word_temp = word_value;
							word_temp &= 0xFF00;
							word_temp = word_temp >> 8;

							v_Buf[0] = word_temp;

							break;

						default:
							// err

							break;

						} // switch (row_byteNumber)

						break;

					case 4: // 4 Bytes var

						uint_value = (UINT)_tcstoul(cellText, NULL, 16);

						switch (row_byteNumber)
						{
						case 3: // [forth Byte pos in Uint]

							uint_temp = uint_value;
							uint_temp &= 0xFF000000;
							uint_temp = uint_temp >> 24;

							v_Buf[0] = uint_temp;

							break;

						case 2: // [third Byte pos in Uint]

							uint_temp = uint_value;
							uint_temp &= 0x00FF0000;
							uint_temp = uint_temp >> 16;

							v_Buf[0] = uint_temp;

							break;

						case 1: // [second Byte pos in Uint]

							uint_temp = uint_value;
							uint_temp &= 0x0000FF00;
							uint_temp = uint_temp >> 8;

							v_Buf[0] = uint_temp;

							break;

						case 0: // [first Byte pos in Uint]

							uint_temp = uint_value;
							uint_temp &= 0x000000FF;

							v_Buf[0] = uint_temp;

							break;

						default:
							// err

							break;

						} // switch (row_byteNumber)

						break;

					default:
						// err

						break;

					} // convert to Output Format

				} // [Load OP]
			} // > Select OP Type


		} // if [Byte mode]
	}

	return 0;
}


int CGridTable_DDM::CheckValidUInt1(int iRow, int iCol)
{
	// error symbol at first
	bool act = 1;

	CString cellText = this->GetItemText(iRow, iCol);

	// check valid Length
	if (cellText.GetLength() < 4)
	{
		// check valid Symbols
		CString intString = cellText.SpanIncluding(_T("0123456789"));

		if (intString.Compare(cellText) == 0)
		{
			// check [unsigned int] valid range
			int intValue = (int)_tcstoul(intString, NULL, 10);

			if ((intValue >= 0) && (intValue <= 255))
			{
				act = 0;
			}
		}
	}

	return act;
}

int CGridTable_DDM::CheckValidUInt2(int iRow, int iCol)
{
	// error symbol at first
	bool act = 1;

	CString cellText = this->GetItemText(iRow, iCol);

	// check valid Length
	if (cellText.GetLength() < 6)
	{
		// check valid Symbols
		CString intString = cellText.SpanIncluding(_T("0123456789"));

		if (intString.Compare(cellText) == 0)
		{
			// check [unsigned int] valid range
			int intValue = (int)_tcstoul(intString, NULL, 10);

			if ((intValue >= 0) && (intValue <= 65535))
			{
				act = 0;
			}		
		}
	}

	return act;
}


int CGridTable_DDM::WordToBitStr(WORD uByteValue, unsigned char * v_BitStrValue)
{
	WORD word_var = 32768;		// 2^(16-1)

	for (int k = 0; k < 16; k++)
	{
		if (uByteValue >= word_var)
		{
			v_BitStrValue[k] = '1';
			uByteValue -= word_var;
		}
		else
		{
			v_BitStrValue[k] = '0';
		}

		word_var /= 2;
	}

	return 0;
}


int CGridTable_DDM::BitStrToWord(WORD * uWordValue, unsigned char * v_BitStrValue)
{

	WORD word_var = 1;
	WORD wordValue_loc = 0;

	for (int k = 0; k < 16; k++)
	{
		if (v_BitStrValue[15 - k] == '0')
		{
			//
		}
		else
		{
			if (v_BitStrValue[15 - k] == '1')
			{
				wordValue_loc += word_var;
			}
			else
			{
				// bad parameter value
				return 2;
			}
		}

		word_var *= 2;
	}

	*uWordValue = wordValue_loc;

	return 0;

}

// Convert String of scientific definition of Float Value to Float VarType Value
int CGridTable_DDM::StrFloatToVarFloat(CString str_Float, float * fValue)
{
	// transfer to char vector
	unsigned char str_length = str_Float.GetLength();

	char v_str_Float[50];

//	char * v_str_Float = "  -2.309e-25";

	for (unsigned char k = 0; k < str_length; k++)
	{
		v_str_Float[k] = str_Float[k];
	}

	v_str_Float[str_length] = '\0';

	_locale_t en = _create_locale(LC_NUMERIC, "en-EN");
	*fValue = _atof_l(v_str_Float, en);

	return 0;
}


int CGridTable_DDM::UpdateTable_Val1(unsigned short uiRowNumber, BYTE ucValue)
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

	// Column 4 - Num Value
	str.Truncate(0);
	str.Format(_T("%d"), ucValue);

	SetItemText(uiRowNumber + 1, 4, str);


	return 0;
}


int CGridTable_DDM::UpdateTable_Val2(unsigned short uiRowNumber, WORD ucValue)
{
	// > set Values
	CString str_cellText;
	CString str;
	BYTE byte_cellText;
	int iProcSuccess;

	// Column 2 - HEX Value
	str.Format(_T("%04X"), ucValue);

	SetItemText(uiRowNumber + 1, 2, str);

	// Column 3 - Bit Value
	unsigned char v_BitStr[16]; // = { 0, 0, 0, 0, 0, 0, 0, 0 };

	iProcSuccess = this->WordToBitStr(ucValue, v_BitStr);

	str.Truncate(0);
	for (unsigned char k = 0; k < 16; k++)
	{
		str.AppendChar(v_BitStr[k]);
	}

	SetItemText(uiRowNumber + 1, 3, str);

	// Column 4 - Num Value
	str.Truncate(0);
	str.Format(_T("%d"), ucValue);

	SetItemText(uiRowNumber + 1, 4, str);


	return 0;
}


int CGridTable_DDM::UpdateTable_Val4(unsigned short uiRowNumber, UINT ucValue)
{
	// > set Values
	CString str_cellText;
	CString str;
	BYTE byte_cellText;
	int iProcSuccess;

	// Column 2 - HEX Value
	str.Format(_T("%08X"), ucValue);

	SetItemText(uiRowNumber + 1, 2, str);

	// Column 3 - Bit Value
	//unsigned char v_BitStr[16]; // = { 0, 0, 0, 0, 0, 0, 0, 0 };

	SetItemText(uiRowNumber + 1, 3, _T("*"));

	// Column 4 - Float Value

	// convert
	union {
		float float_Value;
		UINT uint_Value;
		unsigned char bytes_array[4];
	} union_temp;

	union_temp.uint_Value = ucValue;

	/*
	float value = 0; 
	char str1[16]= "  -2,309e-25";
	

	_locale_t fr = _create_locale(LC_NUMERIC, "fr-FR");
	value = _atof_l(str1, fr);
	*/

	char str_float[20];
	sprintf_s(str_float, "%G", union_temp.float_Value);

	str_cellText.Truncate(0);
	for (unsigned char k = 0; k < 20; k++)
	{
		str_cellText.AppendChar(str_float[k]);
	}

	SetItemText(uiRowNumber + 1, 4, str_cellText);

/*
	my_union.float_variable = value;

	str_cellText.Truncate(0);
	for (unsigned char k = 0; k < 3; k++)
	{
		str_cellText.AppendFormat(_T("%02X"), my_union.bytes_array[k]);
	}

	SetItemText(uiRowNumber + 1, 2, str_cellText);
*/

	return 0;
}

// Decision: UpdateSelected bytes performing in excessively hard to use/provide, so All 
// Update: UpdateSelected is now used to update 1 selected Row
int CGridTable_DDM::UpdateTable_Sel(unsigned short uiRowNumber, unsigned short uiColNumber, CString str_cellText)
{

	// define Set intersect
	unsigned short index_var = 0;
	unsigned short index_mst = 0;		// milestone
	unsigned short byte_mst = 0;
	unsigned short row_number = 0;

	// [Row mode]

	// > Define range

	unsigned char err = 0;
	index_mst = GetRangeNumber(uiRowNumber, &err);

	// > Update

	// > proceed OP 
	int res = 0;

	CString str;
	BYTE byte_cellText;
	WORD word_cellText;
	UINT uint_cellText;
	unsigned char v_BitStr[16]; // = { 0, 0, 0, 0, 0, 0, 0, 0 };

	switch (uiColNumber)
	{
	case 2:	// HEX Value

		switch (index_mst)
		{
		case 0: // Bytes 00 - 39 / size: 2 / Diagnostic Flag Alarm and Warning Thresholds 
				// transfer
			word_cellText = (WORD)_tcstoul(str_cellText, NULL, 16);

			// set /all/
			UpdateTable_Val2(uiRowNumber, word_cellText);

			break;

		case 1: // Bytes 40 - 55 /	size: 2, count: 08	/ Unlocated
				// transfer
			word_cellText = (WORD)_tcstoul(str_cellText, NULL, 16);

			// set /all/
			UpdateTable_Val2(uiRowNumber, word_cellText);
			break;

		case 2: // Bytes 56 - 75 /	size: 4, count: 05	/ Rx PWR Diagnostic calibration constants for optional External Calibration
				// Real Value
			uint_cellText = (UINT)_tcstoul(str_cellText, NULL, 16);

			UpdateTable_Val4(uiRowNumber, uint_cellText);

			break;

		case 3: // Bytes 76 - 91 /	size: 2, count: 08	/ Other  Diagnostic calibration constants for optional External Calibration
				// transfer
			word_cellText = (WORD)_tcstoul(str_cellText, NULL, 16);

			// set /all/
			UpdateTable_Val2(uiRowNumber, word_cellText);

			break;

		case 4: // Bytes 92 - 94 /	size: 1, count: 03	/ Unlocated
				// transfer
			word_cellText = (BYTE)_tcstoul(str_cellText, NULL, 16);

			// set /all/
			UpdateTable_Val1(uiRowNumber, word_cellText);

			break;

		case 5: // Bytes 95		/	size: 1, count: 01	/ CHK of 00-94
				// transfer
			word_cellText = (BYTE)_tcstoul(str_cellText, NULL, 16);

			// set /all/
			UpdateTable_Val1(uiRowNumber, word_cellText);

			break;

		case 6:
			// err case

			break;

		default:
			break;

		} //switch RowSet


		break;

	case 3:	// Bit Value

		switch (index_mst)
		{
		case 0: // Bytes 00 - 39 / size: 2 / Diagnostic Flag Alarm and Warning Thresholds 
				// transfer
			for (unsigned char k = 0; k < 16; k++)
			{
				v_BitStr[k] = str_cellText[k];
			}

			this->BitStrToWord(&word_cellText, v_BitStr);

			// set
			UpdateTable_Val2(uiRowNumber, word_cellText);

			break;

		case 1: // Bytes 40 - 55 /	size: 2, count: 08	/ Unlocated
				// transfer
			for (unsigned char k = 0; k < 16; k++)
			{
				v_BitStr[k] = str_cellText[k];
			}

			this->BitStrToWord(&word_cellText, v_BitStr);

			// set
			UpdateTable_Val2(uiRowNumber, word_cellText);

			break;

		case 2: // Bytes 56 - 75 /	size: 4, count: 05	/ Rx PWR Diagnostic calibration constants for optional External Calibration
				// Real4
			// NOP - has no Column BitStr 
			break;

		case 3: // Bytes 76 - 91 /	size: 2, count: 08	/ Other  Diagnostic calibration constants for optional External Calibration
				// transfer
			for (unsigned char k = 0; k < 16; k++)
			{
				v_BitStr[k] = str_cellText[k];
			}

			this->BitStrToWord(&word_cellText, v_BitStr);

			// set
			UpdateTable_Val2(uiRowNumber, word_cellText);

			break;

		case 4: // Bytes 92 - 94 /	size: 1, count: 03	/ Unlocated
				// transfer
			for (unsigned char k = 0; k < 8; k++)
			{
				v_BitStr[k] = str_cellText[k];
			}

			this->BitStrToByte(&byte_cellText, v_BitStr);

			// set
			UpdateTable_Val1(uiRowNumber, byte_cellText);

			break;

		case 5: // Bytes 95		/	size: 1, count: 01	/ CHK of 00-94
				// transfer
			for (unsigned char k = 0; k < 8; k++)
			{
				v_BitStr[k] = str_cellText[k];
			}

			this->BitStrToByte(&byte_cellText, v_BitStr);

			// set
			UpdateTable_Val1(uiRowNumber, byte_cellText);

			break;

		case 6:
			// err case

			break;

		default:

			break;

		} //switch RowSet


		break;

	case 4:	// Dec Value

		switch (index_mst)
		{
		case 0: // Bytes 00 - 39 / size: 2 / Diagnostic Flag Alarm and Warning Thresholds 
				// transfer
			word_cellText = (WORD)_tcstoul(str_cellText, NULL, 10);

			// set /all/
			UpdateTable_Val2(uiRowNumber, word_cellText);

			break;

		case 1: // Bytes 40 - 55 /	size: 2, count: 08	/ Unlocated
				// transfer
			word_cellText = (WORD)_tcstoul(str_cellText, NULL, 10);

			// set /all/
			UpdateTable_Val2(uiRowNumber, word_cellText);

			break;

		case 2: // Bytes 56 - 75 /	size: 4, count: 05	/ Rx PWR Diagnostic calibration constants for optional External Calibration
			// convert
			union {
				float float_Value;
				UINT uint_Value;
				unsigned char bytes_array[4];
			} union_temp;

			res = StrFloatToVarFloat(str_cellText, &union_temp.float_Value);

			uint_cellText = union_temp.uint_Value;

			UpdateTable_Val4(uiRowNumber, uint_cellText);

			break;

		case 3: // Bytes 76 - 91 /	size: 2, count: 08	/ Other  Diagnostic calibration constants for optional External Calibration
				// transfer
			word_cellText = (WORD)_tcstoul(str_cellText, NULL, 10);

			// set /all/
			UpdateTable_Val2(uiRowNumber, word_cellText);

			break;

		case 4: // Bytes 92 - 94 /	size: 1, count: 03	/ Unlocated
				// transfer
			byte_cellText = (WORD)_tcstoul(str_cellText, NULL, 10);

			// set /all/
			UpdateTable_Val1(uiRowNumber, byte_cellText);

			break;

		case 5: // Bytes 95		/	size: 1, count: 01	/ CHK of 00-94
				// transfer
			byte_cellText = (WORD)_tcstoul(str_cellText, NULL, 10);

			// set /all/
			UpdateTable_Val1(uiRowNumber, byte_cellText);

			break;

		case 6:
			// err case

			break;

		default:
			break;

		} //switch RowSet


		break;

	default:

		break;
	}

	return 0;
}


int CGridTable_DDM::UpdateTable_All(unsigned char * v_Value)
{
	// NOTE: Table is NOT uniform
	// FORMAT:
	// Bytes 00 - 39:	Fields: 00 - 19		 /	size: 2, count: 20	/ Diagnostic Flag Alarm and Warning Thresholds (see Table 3.15)
	// Bytes 40 - 55:	Fields: 20 - 27		 /	size: 2, count: 08	/ Unlocated
	// Bytes 56 - 75:	Fields: 28 - 32		 /	size: 4, count: 05	/ Rx PWR Diagnostic calibration constants for optional External Calibration
	// Bytes 76 - 91:	Fields: 33 - 40		 /	size: 2, count: 08	/ Other  Diagnostic calibration constants for optional External Calibration
	// Bytes 92 - 94:	Fields: 41 - 43		 /	size: 1, count: 03	/ Unlocated
	// Bytes 95:		Fields: 44			 /	size: 1, count: 01	/ CHK of 00-94	

	unsigned char v_Buf[4];
	CString str;

	for (unsigned char k = 0; k < 96; k++)
	{
		v_Buf[0] = v_Value[k];

		ProcTable(k, 1, v_Buf, 0);
	}
	

	/*
	WORD word_value = 0;

	UINT uint_value = 0;

	unsigned char ACT = 1;
	while (ACT)
	{
		// place Value /all/
		switch (v_RowSet_Sizes[index_mst])
		{
		case 1: // 1 Byte var

			UpdateTable_Val1(byte_number, v_Value[byte_number]);

			break;

		case 2:	// 2 Bytes var
			// get Value
			word_value = v_Value[byte_number];
			word_value << 8;
			word_value += v_Value[byte_number + 1];

			UpdateTable_Val2(byte_number, word_value);

			break;

		case 4: // 4 Bytes var
			uint_value = v_Value[byte_number];
			uint_value << 8;
			uint_value += v_Value[byte_number + 1];
			uint_value << 8;
			uint_value += v_Value[byte_number + 2];
			uint_value << 8;
			uint_value += v_Value[byte_number + 3];

			UpdateTable_Val4(byte_number, uint_value);
			
			break;

		default:
			// err

			break;
		}

		// > Proceed range structure
		if (index_var < v_RowSet_Index[index_mst])
		{
			// [current range]

			// proceed further
			index_var++;
		}
		else
		{
			// [next range]
			
			// Check Final Range 
			if (index_mst > 5)
			{
				ACT = 0;
			}
			else
			{
				// change to next range
				index_mst++;
				index_var = 0;

				// set next Var Byte Count
				count_curr = v_RowSet_Sizes[index_mst];
			}
			
		} // if // proceed range structure

		// exit condition
		if (byte_number > 96)
		{
			// [EXIT]

			ACT = 0;
		}
		else
		{
			// [CONTINUE]

			// calc 
			byte_number += count_curr;
		}

	} // while (ACT)
	*/

	return 0;
}


// Grid events
int CGridTable_DDM::OnGridEndEdit(NMHDR * pNotifyStruct, LRESULT * pResult)
{
	// check global enable flag
	if (this->m_bRejectEditChanges)
	{
		*pResult = -1;
		return 1;
	}

	NM_GRIDVIEW* pItem = (NM_GRIDVIEW*)pNotifyStruct;
	//Trace(_T("End Edit on row %d, col %d\n"), pItem->iRow, pItem->iColumn);

	// > Define Row Type
	// NOTE: Table is NOT uniform
	// FORMAT:
	// Bytes 00 - 39:	Fields: 00 - 19		 /	size: 2, count: 20	/ Diagnostic Flag Alarm and Warning Thresholds (see Table 3.15)
	// Bytes 40 - 55:	Fields: 20 - 27		 /	size: 2, count: 08	/ Unlocated
	// Bytes 56 - 75:	Fields: 28 - 32		 /	size: 4, count: 05	/ Rx PWR Diagnostic calibration constants for optional External Calibration
	// Bytes 76 - 91:	Fields: 33 - 40		 /	size: 2, count: 08	/ Other  Diagnostic calibration constants for optional External Calibration
	// Bytes 92 - 94:	Fields: 41 - 43		 /	size: 1, count: 03	/ Unlocated
	// Bytes 95:		Fields: 44			 /	size: 1, count: 01	/ CHK of 00-94

	unsigned char err = 0;

	unsigned short index_mst = 0;

	// get Row Range Value
	index_mst = this->GetRangeNumber(pItem->iRow - 1, &err);

	// check valid row OP
	if (err != 0)
	{
		*pResult = -1;
		return 1;
	}

	// > Proceed Selected Cell

	// > Check Column
	// NOTE: we may edit next Columns:
	// FORMAT:
	// Column 0 - Byte Number		| disable
	// Column 1 - Byte Name			| disable in this rev. (discussed)
	// Column 2 - Byte Value		| edited, HEX value of 1 byte data, HH
	// Column 3 - Bit  Value		| edited, Bit value of 1 byte data, bbbbbbbb
	// Column 4 - Byte Value(ASCII)	| edited, CHAR value for 1 symbol, C 

	// when Col2 is edited, Col3 updates its value
	// when Col3 is edited, Col2 updates its value 

	// > Check Valid Input 
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
		switch (index_mst)
		{
		case 0: // Bytes 00 - 39 / size: 2 / Diagnostic Flag Alarm and Warning Thresholds 
			bInvalid = this->CheckValidHex2(pItem->iRow, pItem->iColumn);

			break;

		case 1: // Bytes 40 - 55 /	size: 2, count: 08	/ Unlocated
			bInvalid = this->CheckValidHex2(pItem->iRow, pItem->iColumn);

			break;

		case 2: // Bytes 56 - 75 /	size: 4, count: 05	/ Rx PWR Diagnostic calibration constants for optional External Calibration
			// 4 BYTEs

			break;

		case 3: // Bytes 76 - 91 /	size: 2, count: 08	/ Other  Diagnostic calibration constants for optional External Calibration
			bInvalid = this->CheckValidHex2(pItem->iRow, pItem->iColumn);

			break;

		case 4: // Bytes 92 - 94 /	size: 1, count: 03	/ Unlocated
			bInvalid = this->CheckValidHex(pItem->iRow, pItem->iColumn);

			break;

		case 5: // Bytes 95		/	size: 1, count: 01	/ CHK of 00-94
			bInvalid = this->CheckValidHex(pItem->iRow, pItem->iColumn);

			break;

		case 6:
			// err case

			break;

		default:
			break;
		}


		break;

	case 3: // Bit Value 
		
		switch (index_mst)
		{
		case 0: // Bytes 00 - 39 / size: 2 / Diagnostic Flag Alarm and Warning Thresholds 
			bInvalid = this->CheckValidBitStr16(pItem->iRow, pItem->iColumn);

			break;

		case 1: // Bytes 40 - 55 /	size: 2, count: 08	/ Unlocated
			bInvalid = this->CheckValidBitStr16(pItem->iRow, pItem->iColumn);

			break;

		case 2: // Bytes 56 - 75 /	size: 4, count: 05	/ Rx PWR Diagnostic calibration constants for optional External Calibration
				// 4 BYTEs

			break;

		case 3: // Bytes 76 - 91 /	size: 2, count: 08	/ Other  Diagnostic calibration constants for optional External Calibration
			bInvalid = this->CheckValidHex2(pItem->iRow, pItem->iColumn);

			break;

		case 4: // Bytes 92 - 94 /	size: 1, count: 03	/ Unlocated
			bInvalid = this->CheckValidHex(pItem->iRow, pItem->iColumn);

			break;

		case 5: // Bytes 95		/	size: 1, count: 01	/ CHK of 00-94
			bInvalid = this->CheckValidHex(pItem->iRow, pItem->iColumn);

			break;

		case 6:
			// err case

			break;

		default:
			break;
		}

		break;

	case 4:	// Int Value
		
		switch (index_mst)
		{
		case 0: // Bytes 00 - 39 / size: 2 / Diagnostic Flag Alarm and Warning Thresholds 
			bInvalid = this->CheckValidUInt2(pItem->iRow, pItem->iColumn);

			break;

		case 1: // Bytes 40 - 55 /	size: 2, count: 08	/ Unlocated
			bInvalid = this->CheckValidUInt2(pItem->iRow, pItem->iColumn);

			break;

		case 2: // Bytes 56 - 75 /	size: 4, count: 05	/ Rx PWR Diagnostic calibration constants for optional External Calibration
				// 4 BYTEs // Real4

			break;

		case 3: // Bytes 76 - 91 /	size: 2, count: 08	/ Other  Diagnostic calibration constants for optional External Calibration
			bInvalid = this->CheckValidUInt2(pItem->iRow, pItem->iColumn);

			break;

		case 4: // Bytes 92 - 94 /	size: 1, count: 03	/ Unlocated
			bInvalid = this->CheckValidUInt1(pItem->iRow, pItem->iColumn);

			break;

		case 5: // Bytes 95		/	size: 1, count: 01	/ CHK of 00-94
			bInvalid = this->CheckValidUInt1(pItem->iRow, pItem->iColumn);

			break;

		case 6:
			// err case

			break;

		default:
			break;
		}


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

	// > Update Row Values
	CString str_cellText = this->GetItemText(pItem->iRow, pItem->iColumn);			// NOTE: +1 need for Header Row padding
	UpdateTable_Sel(pItem->iRow - 1, pItem->iColumn, str_cellText);

	// refresh component
	Invalidate(0);

	return 0;
}
