
    #include "STD_TYPES.h"
	#include "BIT_MATH.h"
	#include "UART_register.h"
	#include "UART_interface.h"
	//HTO5//

	static  void (*u8GlobalPointerReceive)(u8 Global_u8UdrAdress);
	void UART_VidSetCallBackReceive(void(*u8LocalPointerReceive)(u8 LOC_u8UdrAdress)){

		if(u8LocalPointerReceive!=D_NULL){
			u8GlobalPointerReceive=u8LocalPointerReceive;
		}

	}

	static void (*VidGlobalPointerSend)(u8 u8GlOBAL_UDRTAdress);
	void UART_VidSetCallBackSend(void (*VidLocalPointerSend)(u8 u8LOC_UDRTAdress)){

		if(VidLocalPointerSend!=D_NULL){
			VidGlobalPointerSend=VidLocalPointerSend;
		}

	}
	void UART_VidInit(void){

		u8 x=0;
	#if UART_MODE==POOLING
		UBRRL=51;//for boud rate//
		UBRRH=0;
		SET_BIT(x,1);
		SET_BIT(x,2);
		SET_BIT(x,7);
		UCSRC=x;
	SET_BIT(UCSRB,PIN3);//ENABLE TRANSMITTER//
	SET_BIT(UCSRB,PIN4);//ENABLE RECIVER//

	#elif UART_MODE==INTERRUPT
	UBRRL=51;//for boud rate//
	UBRRH=0;
	SET_BIT(x,1);
	SET_BIT(x,2);
	SET_BIT(x,7);
	UCSRC=x;
	SET_BIT(UCSRB,PIN3);
	SET_BIT(UCSRB,PIN4);
	SET_BIT(UCSRB,PIN6);//ENABLE TRANSMITTER   interrupt//
	SET_BIT(UCSRB,PIN7);//ENABLE RECIVER interrupt//
	#endif
	}
	void UART_VidSend(u8 *LOC_u8Data){
	u8 i=0;
	for(i=0;LOC_u8Data[i]!=D_NULL;i++){
		while(0==GET_BIT(UCSRA,PIN5));
		UDR_T=LOC_u8Data[i];
		}
	}
	u8 UART_VidReceive(u8 *PointerToUartData){
		u8 flag=0;

		if(		GET_BIT(UCSRA,PIN7)		==		1)
		{
			*PointerToUartData=UDR_R;
			flag=1;
		}
		return flag;
	}



	//RX COmplete//
	void __vector_13(void){

		if(u8GlobalPointerReceive!=D_NULL){

			u8GlobalPointerReceive(UDR_R);
		}
	}
	//TX COmplete//
	void __vector_15(void){

		if(VidGlobalPointerSend!=D_NULL){
			VidGlobalPointerSend(UDR_T);
		}

	}


	//DATA REGISTER EMPTY//
	void __vector_14(void){
		}

