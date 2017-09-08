// class CGridSFF header file

// Inherited from CGridCtrl class
// Class purpose is: 
// - in case of CP2112 CU - define specific Table for SFF output

#pragma once

#include "SLABCP2112.h"
#include "GridSFF.h"
#include "DSMCUService.h"

/////////////////////////////////////////////////////////////////////////////
// Constants /aliases
/////////////////////////////////////////////////////////////////////////////

// Maxim DS4830a OP Return Codes
#define MAXQBL_SUCCESS						0x00
#define MAXQBL_NO_DEVICE					0x01
#define MAXQBL_DEV_INACCESSIBLE				0x02
#define MAXQBL_OP_FAILED					0x03
#define MAXQBL_OP_WRONG						0x04

#define SLAVEADDR_A0						0xA0
#define SLAVEADDR_A2						0xA2
#define SLAVEADDR_TABLE10					0x10

// Timers
#define TIMER_ID_WRITE						0x01
#define TIMER_ID_READ						0x02


// ###################################################################
// CGridSFF_CP2112 GridSFF_CP2112 class
// ###################################################################
class CGridSFF_CP2112 : public CGridSFF
{
public:
	// standard constructor
	CGridSFF_CP2112();
	CGridSFF_CP2112(HID_SMBUS_DEVICE* pHidSmbus, CProgressCtrl * p_cPB_OP, CEdit * p_EDIT_STATUS, st_serviceData * p_service);

	~CGridSFF_CP2112();

	// methods

private:
	// > Timer	
	// timer controler
	UINT_PTR m_nTimer_Wr;
	UINT_PTR m_nTimer_Rd;

	// timer global /for tick procedure
	struct st_TimerTick
	{
		unsigned int tick;
		unsigned int count;

		unsigned char time_case;
	} m_TimerTick_Write, m_TimerTick_Read;

	// timer support
	void StartTimer_Write();			// TODO: Set various delay
	void StopTimer_Write();
	void StartTimer_Read();				// TODO: Set various delay
	void StopTimer_Read();
	

public:
	// > Table procedures
	// table | simple
	unsigned char DeviceSlave_Read(BYTE * v_ByteData, BYTE slaveAddr, BYTE startAddr, WORD count);
	unsigned char DeviceSlave_ReadUpdate(BYTE * v_ByteData, BYTE slaveAddr, BYTE startAddr, WORD count);

	unsigned char DeviceSlave_Write(BYTE * v_ByteData, BYTE slaveAddr, BYTE startAddr, WORD count);
	unsigned char DeviceSlave_WriteUpdate(BYTE * v_ByteData, BYTE slaveAddr, BYTE startAddr, WORD count);

	// table | custom
	unsigned char DeviceSlave_ReadCustom(BYTE * v_ByteData, BYTE mode,
		BYTE slaveAddr, BYTE startAddr, WORD count, WORD tablSelectAddr, BYTE tablAddr);
	unsigned char DeviceSlave_WriteCustom(BYTE * v_ByteData, BYTE mode,
		BYTE slaveAddr, BYTE startAddr, WORD count, WORD tablSelectAddr, BYTE tablAddr);

private:
	// table  | extended (timer) 
	// global vars /for timer WriteDevice OP
	struct st_WriteDevice
	{
		// vars INIT
		int					return_msg;

		// standard
		unsigned char		startAddr;
		unsigned char		slaveAddr;
		unsigned int		count;

		// addition: options
		unsigned char		mode;
		unsigned int		tablSelectAddr;
		unsigned char		tablAddr;


		// vars PROC
		BYTE *				v_ByteData;
		BYTE				v_ByteData_loc[256];
		BYTE				buffer_cycle[HID_SMBUS_MAX_WRITE_REQUEST_SIZE];

		unsigned int		uRounds;
		unsigned int		uRemain;
		unsigned int		uPos;

		// progress
		double				fstepTrigger;
		double				stepValue;

	} m_WriteDevice;

	struct st_ReadDevice
	{
		// vars INIT
		int					return_msg;

		// standard
		unsigned char		startAddr;
		unsigned char		slaveAddr;
		unsigned int		count;

		// addition: options
		unsigned char		mode;
		unsigned int		tablSelectAddr;
		unsigned char		tablAddr;

		// vars PROC
		BYTE *				v_ByteData;
		BYTE				v_ByteData_loc[256];

		// progress
		double				stepValue;

	} m_ReadDevice;



public:
	// procs
	void DeviceSlave_WriteTimerInit(BYTE * v_ByteData, BYTE mode,
		BYTE slaveAddr, BYTE startAddr, WORD count, WORD tablSelectAddr, BYTE tablAddr);		// main call proc
	void DeviceSlave_WriteTimerProc();
	void DeviceSlave_WriteTimer(BYTE * v_ByteData, BYTE mode,
		BYTE slaveAddr, BYTE startAddr, WORD count, WORD tablSelectAddr, BYTE tablAddr);

	void DeviceSlave_ReadTimerInit(BYTE * v_ByteData, BYTE mode,
		BYTE slaveAddr, BYTE startAddr, WORD count, WORD tablSelectAddr, BYTE tablAddr);
	void DeviceSlave_ReadTimerProc();
	void DeviceSlave_ReadTimer(BYTE * v_ByteData, BYTE mode,
		BYTE slaveAddr, BYTE startAddr, WORD count, WORD tablSelectAddr, BYTE tablAddr);			// main call proc

	// Output sample
	void Trace(LPCTSTR szFmt, ...);

protected:
	HID_SMBUS_DEVICE* m_pHidSmbus;

	// display op progress
	CProgressCtrl* p_cPB_OP;

	// main window status input
	CEdit * p_EDIT_STATUS;

	// service data values
	st_serviceData* m_service;


public:
	DECLARE_MESSAGE_MAP()
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};

