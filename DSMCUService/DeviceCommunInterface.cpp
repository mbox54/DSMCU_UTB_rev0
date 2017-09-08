// DeviceCommunInterface.cpp : implementation file

// implementation


/////////////////////////////////////////////////////////////////////////////
// Includes
/////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "DeviceCommunInterface.h"


CDeviceCommunInterface::CDeviceCommunInterface()
{

}


CDeviceCommunInterface::CDeviceCommunInterface(hSerialCDC * hSerialCDC)
	: m_hUTBDevice(*hSerialCDC)
{

}


CDeviceCommunInterface::~CDeviceCommunInterface()
{

}


// init 
void CDeviceCommunInterface::Init()
{
	// constants
	m_dwBufferSize = 64;

}

int CDeviceCommunInterface::Connect(DWORD dwComNum)
{
	int iResult = COMPort_Open(&this->m_hUTBDevice, dwComNum);

	if (iResult == DEVICE_OP_SUCCESS)
	{
		// [VALID PORT ADDR]

		m_dwComNum = dwComNum;
	}

	return iResult;
}


int CDeviceCommunInterface::Send(channelFrame frInput)
{
	// NOTE:
	// Universal Send procedure

	DWORD dwBytesWritten = 0;
	int iResult = COMPort_Write(&m_hUTBDevice, frInput, &dwBytesWritten);


	return DEVICE_OP_SUCCESS;
}


int CDeviceCommunInterface::SendRequest(UCHAR ucRequestType, UCHAR ucParameter)
{
	// NOTE:
	// Custom purposs Requests
	// PROC Next Requests:
	// OID = BYTE01
	// PAR = BYTE02


	return 0;
}


int CDeviceCommunInterface::Get()
{


	return DEVICE_OP_SUCCESS;
}

// Send Request to Show Table, Get response with referred Table 
int CDeviceCommunInterface::ShowTable(UCHAR ucTableNum, channelFrame * frTableOutput)
{
	// > Send Request: SHOW
	// define frame
	channelFrame frRequest;
	
	frRequest[0] = OID_TYPE_SHOW;
	frRequest[1] = ucTableNum;

	// send frame
	int iResult = Send(frRequest);

	// check error
	if (iResult != DEVICE_OP_SUCCESS)
	{
		return iResult;
	}

	// > Get Response


	return 0;
}


int CDeviceCommunInterface::Disconnect()
{


	return DEVICE_OP_SUCCESS;
}


int CDeviceCommunInterface::Reconnect()
{


	return DEVICE_OP_SUCCESS;
}
