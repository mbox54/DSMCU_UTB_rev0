// CGridSFF_CP2112 : implementation file
//

#include "stdafx.h"
#include "GridSFF_CP2112.h"


// ###################################################################
// CGridSFF_CP2112 GridSFF_CP2112 class
// ###################################################################
CGridSFF_CP2112::CGridSFF_CP2112()
{
}

CGridSFF_CP2112::CGridSFF_CP2112(HID_SMBUS_DEVICE* pHidSmbus, CProgressCtrl * p_cPB_OP, CEdit * p_EDIT_STATUS, st_serviceData * p_service)
	: m_pHidSmbus(pHidSmbus)
	, p_cPB_OP(p_cPB_OP)
	, p_EDIT_STATUS(p_EDIT_STATUS)
	, m_service(p_service)
{
}


CGridSFF_CP2112::~CGridSFF_CP2112()
{
}

BEGIN_MESSAGE_MAP(CGridSFF_CP2112, CGridSFF)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// -------------------------------------------------------------------
// Table procedures
// -------------------------------------------------------------------

// Read 
// FROM Device: [startAddr : startAddr + count]
// TO Memory: [0 : count]
unsigned char CGridSFF_CP2112::DeviceSlave_Read(BYTE * v_ByteData, BYTE slaveAddr, BYTE startAddr, WORD count)
{
	BOOL				opened;
	HID_SMBUS_STATUS	status;
	HID_SMBUS_S0		status0;

	BYTE				buffer_cycle[HID_SMBUS_MAX_READ_RESPONSE_SIZE];
	BYTE				buffer_store[256];

	BYTE				numBytesRead = 0;

	// > SafeCheck
	// Make sure that the device is opened
	BOOL tryDevice = HidSmbus_IsOpened(*m_pHidSmbus, &opened);										// check, return to "opened"
																									// > Check connection																								
	if (opened == 0)
	{
		return MAXQBL_NO_DEVICE;
	}
	// Make sure that the device is operating
	if (tryDevice != HID_SMBUS_SUCCESS)																// check error
	{
		return MAXQBL_DEV_INACCESSIBLE;
	}
	// Make sure that parameters is in Valid Range
	if ((count == 0) || (startAddr + count > 256))
	{
		return MAXQBL_OP_WRONG;
	}

	// > Get Data from device
	BYTE	targetAddress[HID_SMBUS_MAX_TARGET_ADDRESS_SIZE];
	targetAddress[0] = startAddr;

	// > Read from Device
	// addressed Read
	status = HidSmbus_AddressReadRequest(*m_pHidSmbus, slaveAddr, count, 1, targetAddress);
	// wait for i2c proceed
	Sleep(100);

	// > Init read parameters
	// define one Read OP cycle size
	// NOTE: CP2112 provide /61 bytes back maximum/ at 1 Read OP
	// so cycle_size Value must be < 61
	const unsigned char cycle_size = 32;

	unsigned int uRounds = count / cycle_size;
	unsigned int uRemain = count - uRounds * cycle_size;
	unsigned char iCount;
	unsigned int uPos = 0;

	do
	{
		// > Define Read count
		if (uRounds > 0)
		{
			uRounds--;
			iCount = cycle_size;
		}
		else
		{
			iCount = uRemain;
		}

		// Notify device that it should send a read response back
		status = HidSmbus_ForceReadResponse(*m_pHidSmbus, iCount);
		// Load data to buffer
		status = HidSmbus_GetReadResponse(*m_pHidSmbus, &status0, buffer_cycle, HID_SMBUS_MAX_READ_RESPONSE_SIZE, &numBytesRead);

		// Check OP proceed
		if (status != HID_SMBUS_SUCCESS)
		{
			return MAXQBL_OP_FAILED;
		}
		// Check OP result
		if (numBytesRead != iCount)
		{
			return MAXQBL_OP_FAILED;
		}

		// > Copy this segment to the Store
		for (unsigned int k = 0; k < iCount; k++)
		{
			buffer_store[uPos + k] = buffer_cycle[k];
		}

		uPos += iCount;

		Sleep(10);

	} while (uPos < count);


	// Form Result
	for (unsigned int k = 0; k < count; k++)
	{
		v_ByteData[k] = buffer_store[k];
	}

	return HID_SMBUS_SUCCESS;
}

// Read 
// FROM Device: [startAddr : startAddr + count]
// TO Memory: [startAddr : startAddr + count]
unsigned char CGridSFF_CP2112::DeviceSlave_ReadUpdate(BYTE * v_ByteData, BYTE slaveAddr, BYTE startAddr, WORD count)
{
	BOOL				opened;
	HID_SMBUS_STATUS	status;
	HID_SMBUS_S0		status0;

	BYTE				buffer_cycle[HID_SMBUS_MAX_READ_RESPONSE_SIZE];
	BYTE				buffer_store[256];

	BYTE				numBytesRead = 0;

	// > SafeCheck
	// Make sure that the device is opened
	BOOL tryDevice = HidSmbus_IsOpened(*m_pHidSmbus, &opened);										// check, return to "opened"
																									// > Check connection																								
	if (opened == 0)
	{
		return MAXQBL_NO_DEVICE;
	}
	// Make sure that the device is operating
	if (tryDevice != HID_SMBUS_SUCCESS)																// check error
	{
		return MAXQBL_DEV_INACCESSIBLE;
	}
	// Make sure that parameters is in Valid Range
	if ((count == 0) || (startAddr + count > 256))
	{
		return MAXQBL_OP_WRONG;
	}

	// > Get Data from device
	BYTE	targetAddress[HID_SMBUS_MAX_TARGET_ADDRESS_SIZE];
	targetAddress[0] = startAddr;

	// > Read from Device
	// addressed Read
	status = HidSmbus_AddressReadRequest(*m_pHidSmbus, slaveAddr, count, 1, targetAddress);
	// wait for i2c proceed
	Sleep(30);

	// > Init read parameters
	// define one Read OP cycle size
	// NOTE: CP2112 provide /61 bytes back maximum/ at 1 Read OP
	// so cycle_size Value must be < 61
	const unsigned char cycle_size = 32;

	unsigned int uRounds = count / cycle_size;
	unsigned int uRemain = count - uRounds * cycle_size;
	unsigned char iCount;
	unsigned int uPos = 0;

	do
	{
		// > Define Read count
		if (uRounds > 0)
		{
			uRounds--;
			iCount = cycle_size;
		}
		else
		{
			iCount = uRemain;
		}

		// Notify device that it should send a read response back
		status = HidSmbus_ForceReadResponse(*m_pHidSmbus, iCount);
		// Load data to buffer
		status = HidSmbus_GetReadResponse(*m_pHidSmbus, &status0, buffer_cycle, HID_SMBUS_MAX_READ_RESPONSE_SIZE, &numBytesRead);

		// Check OP proceed
		if (status != HID_SMBUS_SUCCESS)
		{
			return MAXQBL_OP_FAILED;
		}
		// Check OP result
		if (numBytesRead != iCount)
		{
			return MAXQBL_OP_FAILED;
		}

		// > Copy this segment to the Store
		for (unsigned int k = 0; k < iCount; k++)
		{
			buffer_store[uPos + k] = buffer_cycle[k];
		}

		uPos += iCount;

	} while (uPos < count);


	// Form Result
	for (unsigned int k = 0; k < count; k++)
	{
		v_ByteData[startAddr + k] = buffer_store[k];
	}

	return HID_SMBUS_SUCCESS;
}

unsigned char CGridSFF_CP2112::DeviceSlave_Write(BYTE * v_ByteData, BYTE slaveAddr, BYTE startAddr, WORD count)
{
	BOOL				opened;
	HID_SMBUS_STATUS	status;

	BYTE				buffer_cycle[HID_SMBUS_MAX_WRITE_REQUEST_SIZE];

	// > SafeCheck
	// Make sure that the device is opened
	BOOL tryDevice = HidSmbus_IsOpened(*m_pHidSmbus, &opened);										// check, return to "opened"
																									// > Check connection																								
	if (opened == 0)
	{
		return MAXQBL_NO_DEVICE;
	}
	// Make sure that the device is operating
	if (tryDevice != HID_SMBUS_SUCCESS)																// check error
	{
		return MAXQBL_DEV_INACCESSIBLE;
	}
	// Make sure that parameters is in Valid Range
	if ((count == 0) || (startAddr + count > 256))
	{
		return MAXQBL_OP_WRONG;
	}

	// > Set Data to device
	// define one Write OP cycle size
	// NOTE: CP2112 provide /61 bytes forward maximum/ at 1 Write OP
	// so cycle_size Value must be < 61
	const unsigned char cycle_size = 32;

	unsigned int uRounds = count / cycle_size;
	unsigned int uRemain = count - uRounds * cycle_size;
	unsigned char iCount;
	unsigned int uPos = 0;

	buffer_cycle[0] = startAddr;			// Set Addr, write OP starts from 

	do
	{
		// > Define Read count
		if (uRounds > 0)
		{
			uRounds--;
			iCount = cycle_size;
		}
		else
		{
			iCount = uRemain;
		}

		// > Load segment to transfer
		for (unsigned int k = 0; k < iCount; k++)
		{
			buffer_cycle[1 + k] = v_ByteData[uPos + k];
		}

		status = HidSmbus_WriteRequest(*m_pHidSmbus, slaveAddr, buffer_cycle, iCount + 1);

		// wait for i2c proceed
		Sleep(50);

		// Check OP proceed
		if (status != HID_SMBUS_SUCCESS)
		{
			return MAXQBL_OP_FAILED;
		}

		uPos += iCount;
		buffer_cycle[0] = uPos;

	} while (uPos < count);

	return HID_SMBUS_SUCCESS;
}

unsigned char CGridSFF_CP2112::DeviceSlave_WriteUpdate(BYTE * v_ByteData, BYTE slaveAddr, BYTE startAddr, WORD count)
{
	BOOL				opened;
	HID_SMBUS_STATUS	status;

	BYTE				buffer_cycle[HID_SMBUS_MAX_WRITE_REQUEST_SIZE];

	// > SafeCheck
	// Make sure that the device is opened
	BOOL tryDevice = HidSmbus_IsOpened(*m_pHidSmbus, &opened);										// check, return to "opened"
																									// > Check connection																								
	if (opened == 0)
	{
		return MAXQBL_NO_DEVICE;
	}
	// Make sure that the device is operating
	if (tryDevice != HID_SMBUS_SUCCESS)																// check error
	{
		return MAXQBL_DEV_INACCESSIBLE;
	}
	// Make sure that parameters is in Valid Range
	if ((count == 0) || (startAddr + count > 256))
	{
		return MAXQBL_OP_WRONG;
	}

	// > Set Data to device
	// define one Write OP cycle size
	// NOTE: CP2112 provide /61 bytes forward maximum/ at 1 Write OP
	// so cycle_size Value must be < 61
	const unsigned char cycle_size = 32;

	unsigned int uRounds = count / cycle_size;
	unsigned int uRemain = count - uRounds * cycle_size;
	unsigned char iCount;
	unsigned int uPos = 0;

	buffer_cycle[0] = startAddr;			// Set Addr, write OP starts from 

	do
	{
		// > Define Read count
		if (uRounds > 0)
		{
			uRounds--;
			iCount = cycle_size;
		}
		else
		{
			iCount = uRemain;
		}

		// > Load segment to transfer
		for (unsigned int k = 0; k < iCount; k++)
		{
			buffer_cycle[1 + k] = v_ByteData[uPos + k];
		}

		status = HidSmbus_WriteRequest(*m_pHidSmbus, slaveAddr, buffer_cycle, iCount + 1);

		// wait for i2c proceed
		Sleep(50);

		// Check OP proceed
		if (status != HID_SMBUS_SUCCESS)
		{
			return MAXQBL_OP_FAILED;
		}

		uPos += iCount;
		buffer_cycle[0] = uPos;

	} while (uPos < count);

	Sleep(100);

	return HID_SMBUS_SUCCESS;
}

// -------------------------------------------------------------------
// Table procedures with CUSTOM selection
// -------------------------------------------------------------------
unsigned char CGridSFF_CP2112::DeviceSlave_ReadCustom(BYTE * v_ByteData, BYTE mode, 
	BYTE slaveAddr,  BYTE startAddr, WORD count, WORD tablSelectAddr, BYTE tablAddr)
{
	BOOL				opened;
	HID_SMBUS_STATUS	status;
	HID_SMBUS_S0		status0;

	BYTE				buffer_cycle[HID_SMBUS_MAX_READ_RESPONSE_SIZE];
	BYTE				buffer_store[256];

	BYTE				numBytesRead = 0;

	// > SafeCheck
	// Make sure that the device is opened
	BOOL tryDevice = HidSmbus_IsOpened(*m_pHidSmbus, &opened);										// check, return to "opened"
																									// > Check connection																								
	if (opened == 0)
	{
		return MAXQBL_NO_DEVICE;
	}
	// Make sure that the device is operating
	if (tryDevice != HID_SMBUS_SUCCESS)																// check error
	{
		return MAXQBL_DEV_INACCESSIBLE;
	}
	// Make sure that parameters is in Valid Range
	if ((count == 0) || (startAddr + count > 256))
	{
		return MAXQBL_OP_WRONG;
	}

	// > TABLE Selection MODE check:
	if (mode == 1)	// TABLE SELECTION
	{
		// NOTE: Activated by previously Sending 0xNN at 0xAA  (NN - is number of Table, AA - table byte addr)
		// NOTE: Table exist only at A2h Slave Addr by SFF
		buffer_cycle[0] = tablSelectAddr;
		buffer_cycle[1] = tablAddr;
		status = HidSmbus_WriteRequest(*m_pHidSmbus, slaveAddr, buffer_cycle, 2);
		Sleep(50);

		// Check OP proceed
		if (status != HID_SMBUS_SUCCESS)
		{
			return MAXQBL_OP_FAILED;
		}
	}

	// > Get Data from device
	BYTE	targetAddress[HID_SMBUS_MAX_TARGET_ADDRESS_SIZE];
	targetAddress[0] = startAddr;

	// > Read from Device
	// addressed Read
	status = HidSmbus_AddressReadRequest(*m_pHidSmbus, slaveAddr, count, 1, targetAddress);

	// wait for i2c proceed
	Sleep(50);

	// > Init read parameters
	// define one Read OP cycle size
	// NOTE: CP2112 provide /61 bytes back maximum/ at 1 Read OP
	// so cycle_size Value must be < 61
	const unsigned char cycle_size = 32;

	unsigned int uRounds = count / cycle_size;
	unsigned int uRemain = count - uRounds * cycle_size;
	unsigned char iCount;
	unsigned int uPos = 0;

	do
	{
		// > Define Read count
		if (uRounds > 0)
		{
			uRounds--;
			iCount = cycle_size;
		}
		else
		{
			iCount = uRemain;
		}

		// Notify device that it should send a read response back
		status = HidSmbus_ForceReadResponse(*m_pHidSmbus, iCount);
		// Load data to buffer
		status = HidSmbus_GetReadResponse(*m_pHidSmbus, &status0, buffer_cycle, HID_SMBUS_MAX_READ_RESPONSE_SIZE, &numBytesRead);

		// Check OP proceed
		if (status != HID_SMBUS_SUCCESS)
		{
			return MAXQBL_OP_FAILED;
		}
		// Check OP result
		if (numBytesRead != iCount)
		{
			return MAXQBL_OP_FAILED;
		}

		// > Copy this segment to the Store
		for (unsigned int k = 0; k < iCount; k++)
		{
			buffer_store[uPos + k] = buffer_cycle[k];
		}

		uPos += iCount;

	} while (uPos < count);


	// Form Result
	for (unsigned int k = 0; k < count; k++)
	{
		v_ByteData[startAddr + k] = buffer_store[k];
	}

	return HID_SMBUS_SUCCESS;
}

unsigned char CGridSFF_CP2112::DeviceSlave_WriteCustom(BYTE * v_ByteData, BYTE mode,
	BYTE slaveAddr, BYTE startAddr, WORD count, WORD tablSelectAddr, BYTE tablAddr)
{
	BOOL				opened;
	HID_SMBUS_STATUS	status;

	BYTE				buffer_cycle[HID_SMBUS_MAX_WRITE_REQUEST_SIZE];

	// > SafeCheck
	// Make sure that the device is opened
	BOOL tryDevice = HidSmbus_IsOpened(*m_pHidSmbus, &opened);										// check, return to "opened"
																									// > Check connection																								
	if (opened == 0)
	{
		return MAXQBL_NO_DEVICE;
	}
	// Make sure that the device is operating
	if (tryDevice != HID_SMBUS_SUCCESS)																// check error
	{
		return MAXQBL_DEV_INACCESSIBLE;
	}
	// Make sure that parameters is in Valid Range
	if ((count == 0) || (startAddr + count > 256))
	{
		return MAXQBL_OP_WRONG;
	}

	// > TABLE Selection MODE check:
	if (mode == 1)	// TABLE SELECTION
	{
		// NOTE: Activated by previously Sending 0xNN at 0xAA  (NN - is number of Table, AA - table byte addr)
		// NOTE: Table exist only at A2h Slave Addr by SFF
		buffer_cycle[0] = tablSelectAddr;
		buffer_cycle[1] = tablAddr;
		status = HidSmbus_WriteRequest(*m_pHidSmbus, slaveAddr, buffer_cycle, 2);
		Sleep(50);

		// Check OP proceed
		if (status != HID_SMBUS_SUCCESS)
		{
			return MAXQBL_OP_FAILED;
		}
	}

	// > Set Data to device
	// define one Write OP cycle size
	// NOTE: CP2112 provide /61 bytes forward maximum/ at 1 Write OP
	// so cycle_size Value must be < 61
	const unsigned char cycle_size = 32;

	unsigned int uRounds = count / cycle_size;
	unsigned int uRemain = count - uRounds * cycle_size;
	unsigned char iCount;
	unsigned int uPos = 0;

	buffer_cycle[0] = startAddr;			// Set Addr, write OP starts from 

	do
	{
		// > Define Read count
		if (uRounds > 0)
		{
			uRounds--;
			iCount = cycle_size;
		}
		else
		{
			iCount = uRemain;
		}

		// > Load segment to transfer
		for (unsigned int k = 0; k < iCount; k++)
		{
			buffer_cycle[1 + k] = v_ByteData[uPos + k];
		}

		status = HidSmbus_WriteRequest(*m_pHidSmbus, slaveAddr, buffer_cycle, iCount + 1);

		// wait for i2c proceed
		Sleep(30);

		// Check OP proceed
		if (status != HID_SMBUS_SUCCESS)
		{
			return MAXQBL_OP_FAILED;
		}

		uPos += iCount;
		buffer_cycle[0] = uPos;

	} while (uPos < count);


	return HID_SMBUS_SUCCESS;
}


// -------------------------------------------------------------------
// Timer Procedures
// -------------------------------------------------------------------
// Timer Write events
void CGridSFF_CP2112::StartTimer_Write()
{
	m_nTimer_Wr = SetTimer(TIMER_ID_WRITE, 100, NULL);
}

void CGridSFF_CP2112::StopTimer_Write()
{
	KillTimer(m_nTimer_Wr);

}

// Timer Read events
void CGridSFF_CP2112::StartTimer_Read()
{
	m_nTimer_Rd = SetTimer(TIMER_ID_READ, 100, NULL);

	// init Timer tick
	m_TimerTick_Read.tick = 0;
	
}

void CGridSFF_CP2112::StopTimer_Read()
{
	KillTimer(m_nTimer_Rd);
}


void CGridSFF_CP2112::DeviceSlave_WriteTimerInit(BYTE * v_ByteData, BYTE mode,
	BYTE slaveAddr, BYTE startAddr, WORD count, WORD tablSelectAddr, BYTE tablAddr)
{
	// init Timer pars
	this->m_TimerTick_Write.time_case = 0;

	this->m_TimerTick_Write.tick = 0;
	this->m_TimerTick_Write.count = 20000;

	// init pars
	this->GridSFF_Read(this->m_WriteDevice.v_ByteData_loc, startAddr, count);
//	this->m_WriteDevice.v_ByteData = v_ByteData;
	this->m_WriteDevice.slaveAddr = slaveAddr;
	this->m_WriteDevice.startAddr = startAddr;
	this->m_WriteDevice.count = count;

	// addition options
	this->m_WriteDevice.mode = mode;
	this->m_WriteDevice.tablSelectAddr = tablSelectAddr;
	this->m_WriteDevice.tablAddr = tablAddr;

	// init vars
	this->m_WriteDevice.return_msg = 0;

	// progress component
	p_cPB_OP->SetPos(0);

	// start timer
	this->StartTimer_Write();

}

void CGridSFF_CP2112::DeviceSlave_WriteTimerProc()
{
	// > Check last OP proceed
	if (m_WriteDevice.return_msg != 0)
	{
		// exit on fault
		return;
	}

	// > init vars
	BOOL				opened;
	HID_SMBUS_STATUS	status;
	BOOL				tryDevice;


	// define one Write OP cycle size
	// NOTE: CP2112 provide /61 bytes forward maximum/ at 1 Write OP
	// so cycle_size Value must be < 61
	const unsigned char cycle_size = 32;

	// > Do procedure
	switch (m_TimerTick_Write.time_case)
	{
	case 0:	// # Init

		// status output
		Trace(_T("ïîïûòêà: ÇÀÏÈÑÜ\n"));
		Trace(_T("ïàðàìåòðû: A0, 256 áàéò\n"));

		// > SafeCheck
		// Make sure that the device is opened
		tryDevice = HidSmbus_IsOpened(*m_pHidSmbus, &opened);										// check, return to "opened"
																									// > Check connection																								
		if (opened == 0)
		{
			m_WriteDevice.return_msg = MAXQBL_NO_DEVICE;
		}

		// Make sure that the device is operating
		if (tryDevice != HID_SMBUS_SUCCESS)																// check error
		{
			m_WriteDevice.return_msg = MAXQBL_DEV_INACCESSIBLE;
		}

		// Make sure that parameters is in Valid Range
		if ((m_WriteDevice.count == 0) || (m_WriteDevice.startAddr + m_WriteDevice.count > 256))
		{
			m_WriteDevice.return_msg = MAXQBL_OP_WRONG;
		}

		// > Service op: disabling components
		m_service->activeState = SERVICE_STATE_DISABLING;

		// > Set Data to device
		m_WriteDevice.uRounds = m_WriteDevice.count / cycle_size;
		m_WriteDevice.uRemain = m_WriteDevice.count - m_WriteDevice.uRounds * cycle_size;
		m_WriteDevice.uPos = 0;

		// > TABLE Selection MODE check:
		if (m_WriteDevice.mode == 1)	// TABLE SELECTION
		{
			// NOTE: Activated by previously Sending 0xNN at 0xAA  (NN - is number of Table, AA - table byte addr)
			// NOTE: Table exist only at A2h Slave Addr by SFF
			m_WriteDevice.buffer_cycle[0] = m_WriteDevice.tablSelectAddr;
			m_WriteDevice.buffer_cycle[1] = m_WriteDevice.tablAddr;
			status = HidSmbus_WriteRequest(*m_pHidSmbus, m_WriteDevice.slaveAddr, m_WriteDevice.buffer_cycle, 2);

			// Check OP proceed
			if (status != HID_SMBUS_SUCCESS)
			{
				m_WriteDevice.return_msg = MAXQBL_OP_FAILED;
			}
		}

		m_WriteDevice.buffer_cycle[0] = m_WriteDevice.startAddr;			// Set Addr, write OP starts from 

		// progress
		if (m_WriteDevice.uRounds > 0)
		{
			m_WriteDevice.fstepTrigger = m_WriteDevice.uRounds;
			m_WriteDevice.fstepTrigger = 100 / m_WriteDevice.fstepTrigger;
			p_cPB_OP->SetStep(m_WriteDevice.fstepTrigger);
		}
		else
		{
			m_WriteDevice.fstepTrigger = 100;
		}

		// change Timer OP case
		m_TimerTick_Write.time_case = 1;

		break;

	case 1:
		// # Proceed
		// > Define Read count
		unsigned char iCount;

		if (m_WriteDevice.uRounds > 0)
		{
			m_WriteDevice.uRounds--;
			iCount = cycle_size;
		}
		else
		{
			iCount = m_WriteDevice.uRemain;
		}

		// > Load segment to transfer
		for (unsigned int k = 0; k < iCount; k++)
		{
			m_WriteDevice.buffer_cycle[1 + k] = m_WriteDevice.v_ByteData_loc[m_WriteDevice.uPos + k];
		}

		status = HidSmbus_WriteRequest(*m_pHidSmbus, m_WriteDevice.slaveAddr, m_WriteDevice.buffer_cycle, iCount + 1);

		// wait for i2c proceed
		// :providing by Timer

		// Check OP proceed
		if (status != HID_SMBUS_SUCCESS)
		{
			m_WriteDevice.return_msg = MAXQBL_OP_FAILED;
		}

		m_WriteDevice.uPos += iCount;
		m_WriteDevice.buffer_cycle[0] = m_WriteDevice.uPos;

		// Check end of Proc
		if (m_WriteDevice.uPos >= m_WriteDevice.count)
		{
			// change Timer OP case
			m_TimerTick_Write.time_case = 2;
		}

		// progress bar		
		m_WriteDevice.stepValue += m_WriteDevice.fstepTrigger;

		p_cPB_OP->StepIt();

		break;

	case 2:	// # Final

		// set result
		m_WriteDevice.return_msg = HID_SMBUS_SUCCESS;

		// progress bar
		p_cPB_OP->SetPos(100);

		// timer
		this->StopTimer_Write();

		// > Service op: enabling components
		m_service->activeState = SERVICE_STATE_ENABLING;

		// status output
		Trace(_T("ÓÑÏÅØÍÎ.\n"));
		Trace(_T("-----------------------\n"));


		break;

	default:
		break;
	} // switch

	  // count ticks
	m_TimerTick_Write.tick++;


}

// Complete procedure WriteTimer
void CGridSFF_CP2112::DeviceSlave_WriteTimer(BYTE * v_ByteData, BYTE mode,
	BYTE slaveAddr, BYTE startAddr, WORD count, WORD tablSelectAddr, BYTE tablAddr)
{
	// Init
	DeviceSlave_WriteTimerInit(v_ByteData, mode, slaveAddr, startAddr, count, tablSelectAddr, tablAddr);

	// Proc
	// proc onTimer()

}

void CGridSFF_CP2112::DeviceSlave_ReadTimerInit(BYTE * v_ByteData, BYTE mode,
	BYTE slaveAddr, BYTE startAddr, WORD count, WORD tablSelectAddr, BYTE tablAddr)
{
	// init Timer pars
	this->m_TimerTick_Read.time_case = 0;

	this->m_TimerTick_Read.tick = 0;
	this->m_TimerTick_Read.count = 20000;

	// init pars
	// this->m_ReadDevice.v_ByteData = v_ByteData;
	this->m_ReadDevice.slaveAddr = slaveAddr;
	this->m_ReadDevice.startAddr = startAddr;
	this->m_ReadDevice.count = count;

	// addition options
	this->m_ReadDevice.mode = mode;
	this->m_ReadDevice.tablSelectAddr = tablSelectAddr;
	this->m_ReadDevice.tablAddr = tablAddr;

	// init vars
	this->m_WriteDevice.return_msg = 0;

	// start timer
	this->StartTimer_Read();
}

void CGridSFF_CP2112::DeviceSlave_ReadTimerProc()
{
	if (m_TimerTick_Read.tick == 0)			// :INIT
	{
		// status output
		Trace(_T("ïîïûòêà: ×ÒÅÍÈÅ\n"));
		Trace(_T("ïàðàìåòðû: A0, 256 áàéò\n"));

		// do service op: disabling
		m_service->activeState = SERVICE_STATE_DISABLING;

		// progress component
		p_cPB_OP->SetPos(0);
		p_cPB_OP->SetStep(20);

		// Read Device
		this->DeviceSlave_ReadCustom(m_ReadDevice.v_ByteData_loc, m_ReadDevice.mode,
			m_ReadDevice.slaveAddr, m_ReadDevice.startAddr, m_ReadDevice.count, m_ReadDevice.tablSelectAddr, m_ReadDevice.tablAddr);
		// this->DeviceSlave_Read(m_ReadDevice.v_ByteData_loc, m_ReadDevice.slaveAddr, m_ReadDevice.startAddr, m_ReadDevice.count);

		// faster op. Set 300 ms for Read delay 		
		m_TimerTick_Read.tick += 60;

		// do progress bar
		p_cPB_OP->SetPos(60);

	}
	else
	{
		if (m_TimerTick_Read.tick < 100)	// :PROC
		{
			// > do progress bar

			m_TimerTick_Read.tick += 20;

			p_cPB_OP->StepIt();
		}
		else								// :FIN
		{
			// interface output
			GridSFF_Write(m_ReadDevice.v_ByteData_loc, 0, 256);

			// finalize op
			StopTimer_Read();

			// do service op: enabling
			m_service->activeState = SERVICE_STATE_ENABLING;

			// status output
			Trace(_T("ÓÑÏÅØÍÎ.\n"));
			Trace(_T("-----------------------\n"));
		}
	}

}

// Complete procedure ReadTimer
void CGridSFF_CP2112::DeviceSlave_ReadTimer(BYTE * v_ByteData, BYTE mode,
	BYTE slaveAddr, BYTE startAddr, WORD count, WORD tablSelectAddr, BYTE tablAddr)
{
	// call prototype
	DeviceSlave_ReadTimerInit(v_ByteData, mode, slaveAddr, startAddr, count, tablSelectAddr, tablAddr);
}

void CGridSFF_CP2112::Trace(LPCTSTR szFmt, ...)
{
	CString str;

	// Format the message text
	va_list argptr;
	va_start(argptr, szFmt);
	str.FormatV(szFmt, argptr);
	va_end(argptr);

	str.Replace(_T("\n"), _T("\r\n"));

	CString strWndText;
	p_EDIT_STATUS->GetWindowText(strWndText);
	strWndText += str;
	p_EDIT_STATUS->SetWindowText(strWndText);

	//	m_TraceWnd.SetSel(str.GetLength()-1, str.GetLength()-2, FALSE);
	p_EDIT_STATUS->LineScroll(-p_EDIT_STATUS->GetLineCount());
	p_EDIT_STATUS->LineScroll(p_EDIT_STATUS->GetLineCount() - 4);
}

void CGridSFF_CP2112::OnTimer(UINT_PTR nIDEvent)
{
	switch (nIDEvent)
	{
	case TIMER_ID_WRITE:	// when Device Write procedure		
		DeviceSlave_WriteTimerProc();

		break;

	case TIMER_ID_READ:	// when Device Read procedure	
		DeviceSlave_ReadTimerProc();

		break;

	default:
		break;
	}

	// call default event
	CWnd::OnTimer(nIDEvent);

}
