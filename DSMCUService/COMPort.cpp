// COMPort.cpp
// Based on Cypress USB-Serial Windows realisation (uart.cpp)
//
// NOTE:
//
// Half-Duplex realisation, Synchronous, non-Overlapped
//

#include "stdafx.h"
#include "COMPort.h" 
 //

/////////////////////////////////////////////////////////////////////////////
// Functions
/////////////////////////////////////////////////////////////////////////////
int COMPort_Open(hSerialCDC * hPort, DWORD dwComNum)
{
	// > Open PORT
	char szPort[COM_PORT_STRING_LEN];
	char * szPort_fileStr = "\\\\.\\COM";

	// > Form COM Port filename
	// String Part
	for (UINT k = 0; k < 7; k++)
	{
		szPort[k] = szPort_fileStr[k];
	}

	// Digit Part
	char chBuf[7];
	_itoa_s(dwComNum, chBuf, 7);
	
	UCHAR ucDigitLen = 0;
	while (chBuf[ucDigitLen] != '\0')
	{
		ucDigitLen++;
	}
	
	for (UINT k = 0; k < ucDigitLen; k++)
	{
		szPort[7 + k] = chBuf[k];
	}

	// String Ender
	szPort[7 + ucDigitLen] = '\0';

	// > Define Communication Regime
	// This creates a synchronous handle. So that only a read or write can be performed
	// for this at a particular point of time.
	// For performing Read and Write operation together, please refer to OVERLAPPED COM
	// PORT USAGE example.
	*hPort = CreateFileA(szPort, GENERIC_READ | GENERIC_WRITE, 0, 0, OPEN_EXISTING, 0, NULL);
	if (*hPort == INVALID_HANDLE_VALUE) return -1;


	// > Set the Read and Write API time outs.
	// These Values will be over ridden during read/write API timeout.
	COMMTIMEOUTS objTimeout;
	GetCommTimeouts(*hPort, &objTimeout);
	objTimeout.WriteTotalTimeoutConstant = DEFAULT_WRITE_TIMEOUT;
	objTimeout.ReadTotalTimeoutConstant = DEFAULT_READ_TIMEOUT;

	// Set up the time out value for ReadFile and WriteFile API.
	SetCommTimeouts(*hPort, &objTimeout);


	return COM_PORT_OP_SUCCESS;
}


int COMPort_SetConfig(hSerialCDC * hPort, DWORD dwBaudRate, UCHAR ucByteSize, UCHAR ucStopBits, UCHAR ucParity, BOOL bCTS_flow_ctrl, BOOL bDSR_flow_ctrl)
{
	// NOTE:
	// NAMES:
	// Data Terminal Ready (DTR) - Data Set Ready (DSR)
	// Clear To Send (CTS) - Request To Send (RTS)
	//
	// RESTRICTIONS:
	// 8 or 7 bits are supported.
	// Device supports one or two stop bits.
	// Device supports no parity, odd, even, mark and space.
	//
	// COMMENT:
	// Flow control happen based on option used. 
	// CTS flow control is popularly used.
	// - CTS Flow control.Please interconnect RTS to CTS of the same device.
	// - DSR Flow Control.Please interconnect DTR to DSR of the same device.
	// Between UART, both UART is expected to understand Hardware Flow control.


	// TODO:
	// check COM_Handler for NULL


	DCB dcb;

	// > Get COM Port curr Config
	if (GetCommState(*hPort, &dcb) == FALSE)
	{
		// [ERROR: PORT OP FAILURE]

		return COM_PORT_OP_FAILURE;
	}

	// > Update Config Values
	dcb.BaudRate = dwBaudRate;
	dcb.ByteSize = ucByteSize;				// 8 or 7 bits are supported.
	dcb.StopBits = ucStopBits;				// Device supports one or two stop bits.
	dcb.Parity = ucParity;					// device supports no parity, odd, even, mark and space.

	dcb.fOutxCtsFlow = bCTS_flow_ctrl;		// This will enable RTS-CTS flow control.
	dcb.fOutxDsrFlow = bDSR_flow_ctrl;		// This will disable DTR-DCR Flow control. 

											// > Set COM Port Config
	if (SetCommState(*hPort, &dcb) == FALSE)
	{
		// [ERROR: PORT OP FAILURE]

		return COM_PORT_OP_FAILURE;
	}



	return COM_PORT_OP_SUCCESS;

}


int COMPort_Read(hSerialCDC * hPort, UCHAR * v_ReadBuffer, DWORD * dwNumBytesRead)
{
	// TODO:
	// check COM_Handler for NULL

	BYTE errNumber = 0;
	DWORD dwNumBytesRead_Cyc;
	DWORD dwNumBytesRead_All = 0;
	while ((dwNumBytesRead_All < COM_PORT_OP_FAILURE) && (errNumber == 0))
	{

		BOOL bReadStatus = ReadFile(*hPort, v_ReadBuffer + dwNumBytesRead_All, USBUART_BUFFER_SIZE, &dwNumBytesRead_Cyc, NULL);

		// > Check Valid PROC
		if (bReadStatus == FALSE)
		{
			// [ERROR: PORT OP FAILURE]

			*dwNumBytesRead = GetLastError();

			errNumber = COM_PORT_OP_FAILURE;
		}
		else
		{
			// NOTE: BIG_Buffer MUST be [Times x USBUART_BUFFER_SIZE] or equal.
			// or this PROTECTOR consequencesed to incorrected work!
			if (dwNumBytesRead_Cyc != USBUART_BUFFER_SIZE)
			{
				// [ERROR: TRANSFER MISMATCH]

				errNumber = COM_PORT_OP_MISMATCH;
			}
		}

		if (errNumber != 0)
		{
			// [ERROR CASE]

			CloseHandle(*hPort);

			return errNumber;
		}
		else
		{
			// [SUCCESS READ]

			dwNumBytesRead_All += USBUART_BUFFER_SIZE;

			*dwNumBytesRead = dwNumBytesRead_All;
		}

	} //while ((dwNumBytesRead_All < COM_PORT_OP_FAILURE) && (errNumber == 0)) 	


	return COM_PORT_OP_SUCCESS;
}


int COMPort_Write(hSerialCDC * hPort, UCHAR * v_WriteBuffer, DWORD * dwNumBytesWritten)
{
	// TODO:
	// check COM_Handler for NULL

	*dwNumBytesWritten = 0;

	BOOL bWriteStatus = WriteFile(*hPort, v_WriteBuffer, USBUART_BUFFER_SIZE, dwNumBytesWritten, NULL);

	// > Check Valid PROC
	BYTE errNumber = 0;
	if (bWriteStatus == FALSE)
	{
		// [ERROR: PORT OP FAILURE]

		*dwNumBytesWritten = GetLastError();

		errNumber = COM_PORT_OP_FAILURE;
	}
	else
	{
		if (*dwNumBytesWritten != USBUART_BUFFER_SIZE)
		{
			// [ERROR: TRANSFER MISMATCH]

			errNumber = COM_PORT_OP_MISMATCH;
		}
	}

	if (errNumber != 0)
	{
		// [ERROR CASE]

		CloseHandle(*hPort);

		return errNumber;
	}


	return COM_PORT_OP_SUCCESS;
}


int COMPort_Close(hSerialCDC * hPort)
{
	// TODO:
	// check COM_Handler for NULL

	// > Set COM Port Config
	if (CloseHandle(*hPort) == FALSE)
	{
		// [ERROR: PORT OP FAILURE]

		return COM_PORT_OP_FAILURE;
	}


	return COM_PORT_OP_SUCCESS;
}


