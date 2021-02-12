

	#ifndef _ADC_INTERFACE_H_
	#define _ADC_INTERFACE_H_
		#define RUNNING                  0
		#define ANALOG_COMPARATOR        1
		#define EXTERNAL_INTERRUPT       2
		#define COUNTER0_COMP_MATCH      3
		#define COUNTER0_OVERFLOW        4
		#define COUNTER1_Comp_MatchB     5
		#define COUNTER1_OVERFLOW        6
		#define COUNTER1_CAP_EVENT       7

		#define LEFT                    	1
		#define RIGHT                   	0
		#define ON                 			1
		#define OFF                			0
		
	    #define AUTO_TRIG                 ON
		#define AUTO_TRIG_SOURCE         RUNNING
		#define INTERRUPT_ADC             OFF
		#define DATA_READING               RIGHT
	
	
	
	#define ADC0               			0
	#define ADC1               			1 
	#define ADC2               			2 
	#define ADC3               			3 
	#define ADC4               			4 
	#define ADC5               			5 
	#define ADC6               			6 
	#define ADC7               			7 
	void ADC_SetCallBack(void (*LocPtr)(u16 ADC_Adress));
	void ADC_VidInit(void);
	u16 ADC_VidGetReadingPooling(u8 LOC_u8AdcNum);
	//u8 ADC_VidGetReadingPooling(u8 LOC_u8AdcNum,u16 *PointerToData);

	void ADC_VidGetReadingInterrupt(u8 LOC_u8AdcNum);

	#endif


