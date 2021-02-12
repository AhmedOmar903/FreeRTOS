


	#include "STD_TYPES.h"
	#include "BIT_MATH.h"
	#include "ADC_interface.h"
	#include "ADC_register.h"

	static void(*Gptr)(u16 ADC_Adress)=D_NULL;
	void ADC_SetCallBack(void (*LocPtr)(u16 ADC_Adress)){

		if(LocPtr!=D_NULL){
			Gptr=LocPtr;
			}

	}
	void ADC_VidInit(void){
	//CHOOSE THE Vref//
	  SET_BIT(ADMUX,PIN6);
	  CLR_BIT(ADMUX,PIN7);

	#if DATA_READING==RIGHT  
	   CLR_BIT(ADMUX,PIN5);//choose the right adjest//
	#elif DATA_READING==LEFT  
	   SET_BIT(ADMUX,PIN5);//choose the LEFT adjest//  
	#endif   

	#if AUTO_TRIG==OFF
	   CLR_BIT(ADCSRA,PIN5);//auto trigger OFF//
	#elif   AUTO_TRIG==ON
	  SET_BIT(ADCSRA,PIN5);//auto trigger on//
	  CLR_BIT(SFIOR,PIN4);// this bit must be zero when SFIOR is written //
			  #if AUTO_TRIG_SOURCE==RUNNING
				  CLR_BIT(SFIOR,PIN5);
				  CLR_BIT(SFIOR,PIN6);
				  CLR_BIT(SFIOR,PIN7);
			  #elif AUTO_TRIG_SOURCE==ANALOG_COMPARATOR
				  SET_BIT(SFIOR,PIN5);
				  CLR_BIT(SFIOR,PIN6);
				  CLR_BIT(SFIOR,PIN7);

			  #elif AUTO_TRIG_SOURCE==EXTERNAL_INTERRUPT
				  CLR_BIT(SFIOR,PIN5);
				  SET_BIT(SFIOR,PIN6);
				  CLR_BIT(SFIOR,PIN7);

			  #elif AUTO_TRIG_SOURCE==COUNTER0_COMP_MATCH

				  SET_BIT(SFIOR,PIN5);
				  SET_BIT(SFIOR,PIN6);
				  CLR_BIT(SFIOR,PIN7);

			  #elif AUTO_TRIG_SOURCE==COUNTER0_OVERFLOW

				  CLR_BIT(SFIOR,PIN5);
				  CLR_BIT(SFIOR,PIN6);
				  SET_BIT(SFIOR,PIN7);

			  #elif AUTO_TRIG_SOURCE==COUNTER0_OVERFLOW

				CLR_BIT(SFIOR,PIN5);
				CLR_BIT(SFIOR,PIN6);
				SET_BIT(SFIOR,PIN7);
			  #elif AUTO_TRIG_SOURCE==COUNTER1_Comp_MatchB

				SET_BIT(SFIOR,PIN5);
				CLR_BIT(SFIOR,PIN6);
				SET_BIT(SFIOR,PIN7);
			  #elif AUTO_TRIG_SOURCE==COUNTER1_OVERFLOW

				CLR_BIT(SFIOR,PIN5);
				SET_BIT(SFIOR,PIN6);
				SET_BIT(SFIOR,PIN7);
			  #elif AUTO_TRIG_SOURCE==COUNTER1_CAP_EVENT

				SET_BIT(SFIOR,PIN5);
				SET_BIT(SFIOR,PIN6);
				SET_BIT(SFIOR,PIN7);


			  #endif

	#endif   
	   
		CLR_BIT(ADCSRA,PIN0);  //pre scaller 64//
		SET_BIT(ADCSRA,PIN1);
		SET_BIT(ADCSRA,PIN2);
		
	  
		SET_BIT(ADCSRA ,PIN7);//ADC ENABLBE//

	}

	u16 ADC_VidGetReadingPooling(u8 LOC_u8AdcNum){
		
		ADMUX&=0b11100000;
		ADMUX|=LOC_u8AdcNum;
		
		SET_BIT(ADCSRA,PIN6);//start the conversion//
		
		while(0==GET_BIT(ADCSRA,PIN4)){
			
		}
		SET_BIT(ADCSRA,PIN4);
		return ADC;
	}

	void ADC_VidGetReadingInterrupt(u8 LOC_u8AdcNum){
		ADMUX&=0b11100000;
		ADMUX|=LOC_u8AdcNum;

	#if	INTERRUPT_ADC==ON
	 SET_BIT(ADCSRA,PIN3);//enable the ADC interrupt on//
	#elif  INTERRUPT_ADC==OFF
	 CLR_BIT(ADCSRA,PIN3);//Disable the ADC interrupt on//
	#endif
	SET_BIT(ADCSRA,PIN6);//start the conversion//

	}

	void __vector_16( void ){

	 if(Gptr!=D_NULL){
		Gptr(ADC);
		}

	}
