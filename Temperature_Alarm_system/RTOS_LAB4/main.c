/*
 * main.c
 *
 *  Created on: Feb 7, 2021
 *     Author: Ahmed Ramadan
 *  Description: FreeRTOS_LAB4
 */

#include"STD_TYPES.h"
#include"BIT_MATH.h"
#include "DIO_interface.h"
#include "UART_interface.h"
#include "LCD_interface.h"
#include "ADC_interface.h"
#include "DISPLAY_FUNCTIONS.h"
#include "main.h"




#include "FreeRTOS/Src/FreeRTOS.h"
#include "FreeRTOS/Src/task.h"
#include "FreeRTOS/Src/queue.h"
#include "FreeRTOS/Src/semphr.h"
#include "FreeRTOS/Src/event_groups.h"




#define SYSTEM_STATE_MAIN         0
#define SYSTEM_STATE_ALARM        1
#define SYSTEM_STATE_CONFIG       2





void ALARMINSTATUS_VoidToggleing(void);
void TERMINAL_VoidSetMoreThanOneEvent(void);



void RTOS2_VoidInitializationsTheHardWareComponets(void);


//>>>>.................................................................................












void TASK1_VoidGetTheTemperatureReading(void *PointerToData);
void TASK2_VoidAlarmingEnable(void *PointerToData);
void TASK3_VoidCheckingAboutSystemTempertaure(void *PointerToData);
void TASK4_VoidResponsibleAboutTheTerminal(void *PointerToData);
void TASK5_VoidDisplayAndExcuteAllEvents(void *PointerToData);

#define E_EVENT1   (1<<0)
#define E_EVENT2   (1<<1)
#define E_EVENT3   (1<<2)
#define E_EVENT4   (1<<3)
#define E_EVENT5   (1<<4)
#define E_EVENT6   (1<<5)
#define E_EVENT7   (1<<6)



u8 ThersholdValue;


EventGroupHandle_t EventGroupHandleForTheSystem;

EventBits_t EventBit_AlarmSystem;

SemaphoreHandle_t SemaphoreCheck=NULL;



int main(void)
{

	RTOS2_VoidInitializationsTheHardWareComponets(); //Initialize The Component


	EventGroupHandleForTheSystem=xEventGroupCreate();// Create The Event Group

	SemaphoreCheck=xSemaphoreCreateBinary();// Create The Semaphore



	xTaskCreate(TASK2_VoidAlarmingEnable,"Alarming",100,NULL,5,NULL);//Alarming Task  PR :5

	xTaskCreate(TASK3_VoidCheckingAboutSystemTempertaure,"Checking",100,NULL,4,NULL);//Check_Task;  PR :4

	xTaskCreate(TASK4_VoidResponsibleAboutTheTerminal,"UartTerminal",150,NULL,3,NULL);//Terminal;  PR :3

	xTaskCreate(TASK1_VoidGetTheTemperatureReading,"TemperatureTask",150,NULL,2,NULL);//Temp_Task;  PR :2

	xTaskCreate(TASK5_VoidDisplayAndExcuteAllEvents,"DisplayTask",200,NULL,1,NULL);//Display_Task;  PR :1





	vTaskStartScheduler();

}





/*	>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>		Temp TASK		<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<	*/

void TASK1_VoidGetTheTemperatureReading(void *PointerToData)
{
	static u16 Local_U16PreviousTemp=0;
	u16 Local_U16DigitialValue=0;
	u16 Local_U16AnalogValue=0;



	while(1)
	{

		Local_U16DigitialValue=ADC_VidGetReadingPooling(ADC0);
		Local_U16AnalogValue=(Local_U16DigitialValue*5000UL)/(1024);		//  Voltage Divider
		Local_U16AnalogValue/=10;										   //   Temp Sensor DataSheet

		if(		Local_U16AnalogValue		!=		Local_U16PreviousTemp)	  // Check if The Temp Changed
		{
			Local_U16PreviousTemp=Local_U16AnalogValue;						// Update Local_U16PreviousTemp

			AlarmingSystem.CurrentTemp=Local_U16AnalogValue;		//Update The Current TEMO

			xEventGroupSetBits(EventGroupHandleForTheSystem,E_EVENT5);//Set The Fifith Event

			xSemaphoreGive(SemaphoreCheck); //Give The Semaphore inorder to Check The Conditions
		}
		vTaskDelay(150);
	}

}



/*	>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>		Alarming TASK		<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<	*/


void TASK2_VoidAlarmingEnable(void *PointerToData)
{

	while(1)
	{
		EventBit_AlarmSystem=xEventGroupWaitBits(EventGroupHandleForTheSystem,E_EVENT7,0,0,portMAX_DELAY);

		if(		(EventBit_AlarmSystem	&	E_EVENT7 ) 		)
		{
			 DIO_VidSetPinValue(0,PIN1,HIGH); //Buzzer ON
			 vTaskDelay(500);
			 DIO_VidSetPinValue(0,PIN1,LOW); //Buzzer OFF
			 vTaskDelay(500);
		}
	}
}

/*	>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>		Checking TASK		<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<	*/

void TASK3_VoidCheckingAboutSystemTempertaure(void *PointerToData)
{

	while(1)
	{
		if(		xSemaphoreTake(SemaphoreCheck,portMAX_DELAY)		)
		{

			if(	(AlarmingSystem.SystemState==SYSTEM_STATE_MAIN) && (AlarmingSystem.CurrentTemp >= AlarmingSystem.Threshold	) &&(AlarmingSystem.AlarmingStatus == 'E')	)
			{
				AlarmingSystem.SystemState=SYSTEM_STATE_ALARM;
				xEventGroupSetBits(EventGroupHandleForTheSystem,E_EVENT3);//Set The Third  Event
				xEventGroupSetBits(EventGroupHandleForTheSystem,E_EVENT7);//Set The Seventh Event
			}
			else if(	(AlarmingSystem.SystemState==SYSTEM_STATE_ALARM) &&	 (AlarmingSystem.CurrentTemp < AlarmingSystem.Threshold	)	)
			{
				AlarmingSystem.SystemState=SYSTEM_STATE_MAIN;
				xEventGroupSetBits(EventGroupHandleForTheSystem,E_EVENT1);//Set The First  Event
				xEventGroupClearBits(EventGroupHandleForTheSystem,E_EVENT7);//Clear The Seventh Event
			}
		}

	}
}


/*	>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>		Terminal TASK		<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<	*/



void TASK4_VoidResponsibleAboutTheTerminal(void *PointerToData)
{

	u8 Local_U8ValidateFlag=0;
	u8 Local_U8ReciveingData=0;

	while(1)
	{
		Local_U8ValidateFlag=UART_VidReceive(&Local_U8ReciveingData);

		if(		Local_U8ValidateFlag		==		1		)
		{
			if(		AlarmingSystem.SystemState	==	SYSTEM_STATE_MAIN			)
			{
				switch(Local_U8ReciveingData)
				{
					case 'C':
						AlarmingSystem.SystemState	=SYSTEM_STATE_CONFIG; // Change TO Configuration Mode
						xEventGroupSetBits(EventGroupHandleForTheSystem,E_EVENT2);//Set The Secand  Event
						break;
					case 'T':
						ALARMINSTATUS_VoidToggleing();//Toggle The Alarmming Status
						xEventGroupSetBits(EventGroupHandleForTheSystem,E_EVENT4);//Set The Fourth  Event
						xSemaphoreGive(SemaphoreCheck);// Give The Semephore
						break;
				}
			}
			else if(		AlarmingSystem.SystemState	==	SYSTEM_STATE_CONFIG			)
			{
				if(		Local_U8ReciveingData		==		'C'		)
				{

					TERMINAL_VoidSetMoreThanOneEvent();
				}
				else if(	Local_U8ReciveingData		==		'O'			)
				{
					AlarmingSystem.Threshold=ThersholdValue;
					ThersholdValue=0;
					TERMINAL_VoidSetMoreThanOneEvent();
					xSemaphoreGive(SemaphoreCheck);
				}
				else
				{
					ThersholdValue=(ThersholdValue*10)+(Local_U8ReciveingData-48);// New Value OF the Thershold

					LCD_VidSetDdram(2,1);
					LCD_VidWriteNumber(ThersholdValue);

					xEventGroupSetBits(EventGroupHandleForTheSystem,E_EVENT6);//Set The Six  Event
				}
			}
		}

		vTaskDelay(50);
	}

}


/*	>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>		Display_TASK		<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<	*/



void TASK5_VoidDisplayAndExcuteAllEvents(void *PointerToData)
{

	while(1)
	{
		 EventBit_AlarmSystem=xEventGroupWaitBits(EventGroupHandleForTheSystem,
		(E_EVENT1|E_EVENT2|E_EVENT3|E_EVENT4|E_EVENT5|E_EVENT6),
		1,0,portMAX_DELAY);


		 if(		(EventBit_AlarmSystem	& 		E_EVENT1) 	) // Display The Main Screen
		 {
				 LCD_VoidDisplayTheMainScreen();
		 }
		 else if(		EventBit_AlarmSystem	& 		E_EVENT2		) // Display The Config Screen
		 {
			 LCD_VoidDisplayTheConfigurationScreen();
		 }
		 else if(		EventBit_AlarmSystem	& 		E_EVENT3		) // Display The Alarm Screen
		 {
			 LCD_VoidDisplayTheAlarmmingScreen();
		 }
		 else if(		EventBit_AlarmSystem	& 		E_EVENT4 		) // Update The Alarm Status
		 {
			 LCD_VoidDisplayAlarmStatuscreen();
		 }
		 else if(		(EventBit_AlarmSystem	& 		E_EVENT5)	&& (	AlarmingSystem.SystemState	==	SYSTEM_STATE_MAIN	)	) // Update The Current Temp
		 {
			 LCD_VoidDisplayAndUpdateTheCurrentTemp();
		 }
		 else if(  (EventBit_AlarmSystem		& 		E_EVENT6) 	&& (	AlarmingSystem.SystemState	==	SYSTEM_STATE_MAIN	)	) // Update TheThersholdTemp
		 {
			 LCD_VoidDisplayAndUpdateThershold();
		 }



	}




}








//>>>>>>>>>>>>>>>>>>>>>>>>>>>>... 		Used Functions 			<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<



void ALARMINSTATUS_VoidToggleing(void)
{
	static u8 Index=0;
	u8 LocalCharacterArray[]={'E','D'};

	switch(Index)
	{
		case 0: Index=1; break;
		case 1: Index=0; break;
	}
	AlarmingSystem.AlarmingStatus=LocalCharacterArray[Index];
}


void TERMINAL_VoidSetMoreThanOneEvent(void)
{

	AlarmingSystem.SystemState =	SYSTEM_STATE_MAIN ;// Change TO Main Mode
	xEventGroupSetBits(EventGroupHandleForTheSystem,E_EVENT1);//Set The First  Event
	xEventGroupSetBits(EventGroupHandleForTheSystem,E_EVENT4);//Set The Fourth  Event
	xEventGroupSetBits(EventGroupHandleForTheSystem,E_EVENT5);//Set The FIFTH  Event
	xEventGroupSetBits(EventGroupHandleForTheSystem,E_EVENT6);//Set The Six  Event

}







void RTOS2_VoidInitializationsTheHardWareComponets(void)
{
	DIO_VidSetPinDirection(0,PIN0,INPUT);//PORTA     ADC Channel
	DIO_VidSetPinDirection(0,PIN1,OUTPUT);//PORTA     Buzzer BIN

	DIO_VidSetPinDirection(3,PIN0,INPUT);//RX		//PORTD
	DIO_VidSetPinDirection(3,PIN1,OUTPUT);//TX		//PORTD


	UART_VidInit();//UART INIT
	LCD_VidInit();//LCD Initializations
	ADC_VidInit();//ADC Initializations

	AlarmingSystem.SystemState=SYSTEM_STATE_MAIN;
	AlarmingSystem.Threshold=40; // Max Temp
	AlarmingSystem.AlarmingStatus='E'; // State is Enable
	AlarmingSystem.CurrentTemp=20;

	LCD_VidWriteCommand(CLR);
	LCD_VoidDisplayTheMainScreen();

}







