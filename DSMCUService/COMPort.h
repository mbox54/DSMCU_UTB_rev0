// COMPort.h : header file
// Based on Cypress USB-Serial Windows realisation (uart.cpp)
//
// NOTE:
//
// Half-Duplex realisation, Synchronous, non-Overlapped
//

#pragma once

/////////////////////////////////////////////////////////////////////////////
// Includes
/////////////////////////////////////////////////////////////////////////////
#include <windows.h>
#include <dbt.h>


/////////////////////////////////////////////////////////////////////////////
// Constants /aliases
/////////////////////////////////////////////////////////////////////////////
#define COM_PORT_STRING_LEN             256
#define DEFAULT_WRITE_TIMEOUT           2000
#define DEFAULT_READ_TIMEOUT            2000

#define DEFAULT_UART_BAUDRATE           500000 //(500K Bauds).
#define UART_3M_BAUDRATE                3000000 // 3 MBbps

#define USBUART_BUFFER_SIZE				64


#define COM_PORT_OP_SUCCESS				0x00
#define COM_PORT_IS_EMPTY				0x01
#define COM_PORT_OP_FAILURE				0x02
#define COM_PORT_OP_MISMATCH			0x03


/////////////////////////////////////////////////////////////////////////////
// typedef
/////////////////////////////////////////////////////////////////////////////
typedef HANDLE hSerialCDC;


/////////////////////////////////////////////////////////////////////////////
// Functions
/////////////////////////////////////////////////////////////////////////////
int COMPort_Open(hSerialCDC * hPort, DWORD dwComNum);
int COMPort_SetConfig(hSerialCDC * hPort, DWORD dwBaudRate, UCHAR ucByteSize, UCHAR ucStopBits, UCHAR ucParity, BOOL bCTS_flow_ctrl, BOOL bDSR_flow_ctrl);
int COMPort_Read(hSerialCDC * hPort, UCHAR * v_ReadBuffer, DWORD * dwNumBytesRead);
int COMPort_Write(hSerialCDC * hPort, UCHAR * v_WriteBuffer, DWORD * dwNumBytesWritten);
int COMPort_Close(hSerialCDC * hPort);


