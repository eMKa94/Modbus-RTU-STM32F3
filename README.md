**Free MODBUS Library port on STM32F3**

This example is based on custom PCB with STM32F373RCT6 mcu on board.
Low level libraries for controlling mcu peripherials was used.

Due to the necessity of controling data flow direction in half-duplex RS485 interface,
there are functions, which definitions are dependent on hardware:
* **boardRs485SetDirectionTransmit()**
* **boardRs485SetDirectionReceive()**
  
These functions are used in the modifications of **eMbPool** function in **mb.c** file:

When the frame from master is received, the data direction should be changed to transmission.

    '''C
    if( xMBPortEventGet( &eEvent ) == TRUE )
    {
        switch ( eEvent )
        {
        case EV_READY:
            break;

        case EV_FRAME_RECEIVED:
            eStatus = peMBFrameReceiveCur( &ucRcvAddress, &ucMBFrame, &usLength );
            if( eStatus == MB_ENOERR )
            {
                /* Check if the frame is for us. If not ignore the frame. */
                if( ( ucRcvAddress == ucMBAddress ) || ( ucRcvAddress == MB_ADDRESS_BROADCAST ) )
                {
                    boardRs485SetDirectionTransmit();
                    ( void )xMBPortEventPost( EV_EXECUTE );
                }
            }
            break;
    '''
When the response frame was transmitted, the data direction should be set back to reception.
Because in the STM32F3, the "Transmit register empty" interrupt (TXE) is triggered before the USART physically send the byte, software should wait few miliseconds before it changes the data direction to reception. This is done by setting a flag in **vMBPortSerialEnable** function in **mb_portserial.c** file:

    '''C
    if (xRxEnable)
    {
    	LL_USART_EnableIT_RXNE(USART3);
    	rx_enable_request = 1;
    	rxOnTimeMs = board_runtime_ms+10;
    }
    '''

And then at the end of **eMbPool** function in **mb.c** file:

    '''C
    if (rx_enable_request == 1)
    {
    	if (board_runtime_ms >= rxOnTimeMs)
        {
            boardRs485SetDirectionReceive();
            rx_enable_request = 0;
        }
    }
    '''

Thank You.
Maciej Kowalski
