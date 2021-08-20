/*
	Zenex sp. z o. o.
	Maciej Kowalski

	IPA Board source file
*/

/* Includes ------------------------------------------------------------------*/

#include "board.h"

// Modbus
uint8_t          modbusId 		= 1;
rs485Baudrate_t  rs485Baudrate 	= baudrate_19200;
rs485Parity_t    rs485Parity 	= parity_none;
rs485StopBits_t  rs485StopBits 	= stopBits_1;
