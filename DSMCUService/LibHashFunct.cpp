// LibHashFunct.cpp

// *******************************************************************
// Library source with service Functions
// Functions for calculate/perform HASH Operations
// *******************************************************************

// implementation file

#include "stdafx.h"
#include "LibHashFunct.h"


// Calculate 1 Byte Checksum
unsigned char CalculateHash1(unsigned char * v_Values, unsigned int udCount)
{
	unsigned char ucCHK = 0;

	for (unsigned int k = 0; k < udCount; k++)
	{
		ucCHK += v_Values[k];
	}

//	ucCHK = 0x100 - ucCHK;

	return ucCHK;
}

// Compare 1Byte CHK with previous exist Value
// FORMAT:
// 0 = FALSE
// 1 TRUE
unsigned char CheckCHK1(unsigned char uc_CHK, unsigned char * v_Values, unsigned int udCount)
{
	// calc CHK
	unsigned char ucCHK_calc = CalculateHash1(v_Values, udCount);

	// compare
	unsigned char res;
	if (uc_CHK == ucCHK_calc)
	{
		res = 1;
	}
	else
	{
		res = 0;
	}
	
	return res;
}



