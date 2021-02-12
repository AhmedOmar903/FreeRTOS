

	#ifndef _UART_INTERFACE_H_
	#define _UART_INTERFACE_H_
	   
	#define POOLING         0
	#define INTERRUPT       1
	#define UART_MODE       POOLING
	
	
	
	void UART_VidInit(void);
	void UART_VidSend(u8 *LOC_u8Data);
	u8 UART_VidReceive(u8 *PointerToUartData);
	void UART_VidSetCallBackReceive(void(*u8LocalPointerReceive)(u8 LOC_u8UdrAdress));

	void UART_VidSetCallBackSend(void (*VidLocalPointerSend)(u8 u8LOC_UDRTAdress));

	#endif
