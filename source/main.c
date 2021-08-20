/*
  eMKa elektronika
  Maciej Kowalski

  IPA - Integrated Power Analyser
  Firmware main application

  No any licence yet =]

*/

#include "board.h"
#include "modbus.h"
#include <stdbool.h>
#include <string.h>

/*
  Error handling  (In progress)
*/
#define ERROR_SOURCE_CORE     0x00
#define ERROR_SOURCE_MODBUS   0x01

void errorHandler(uint8_t errorSource, uint8_t errorCode);


int main(void)
{
  
 	board_core_init();
  

  board_wait_ms(100);

  // Example Input Registers initial values
  usRegInputBuf[0] = 0;
  usRegInputBuf[1] = 1;
  usRegInputBuf[2] = 2;
 
  
  MB_init();
  while (1)
  {
    eMBPoll(  );
  }

}

void board_1ms_routine(void)
{

}

void board_100ms_routine(void)
{

}

void board_1s_routine(void)
{

  // Every 1s update input register 0 witch board runtime in miliseconds
  usRegInputBuf[0] = (uint16_t)board_runtime_ms;

}

void errorHandler(uint8_t errorSource, uint8_t errorCode)
{

}
