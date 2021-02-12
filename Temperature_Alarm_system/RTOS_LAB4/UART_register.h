
	#ifndef _UART_REGISTER_H_
	#define _UART_REGISTER_H_

	 #define UDR_T        *((volatile u8*)0x2C)
	 #define UDR_R        *((volatile u8*)0x2C)
	 #define UCSRA        *((volatile u8*)0x2B)
	 #define UCSRB        *((volatile u8*)0x2A)
	 #define UCSRC        *((volatile u8*)0x40)
	 #define UBRRL        *((volatile u8*)0x29)
	 #define UBRRH        *((volatile u8*)0x40)

	void __vector_13(void) __attribute__((signal));//RX COmplete//
	void __vector_14(void) __attribute__((signal));//DATA REGISTER EMPTY//
	void __vector_15(void) __attribute__((signal));//TX COmplete//



	#endif
