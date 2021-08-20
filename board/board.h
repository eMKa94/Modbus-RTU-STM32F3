/*
#	#	#	BOARD   H	#	#	#
#								#
#		eMKa elektronika		#
#		Maciej Kowalski			#
#								#
#	STM32F373RCT6 prototype		#
#		board header file		#	
#								#	
#	#	#	#	#	#	#	#	#
*/

#ifndef BOARD_H
#define BOARD_H

#include <stdint.h>
#include <stdbool.h>

#include "board_core.h"
#include "board_rs485.h"

extern uint8_t          modbusId;
extern rs485Baudrate_t  rs485Baudrate;
extern rs485Parity_t    rs485Parity;
extern rs485StopBits_t  rs485StopBits;

#endif