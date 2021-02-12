/*
 * Display_Functions.c
 *
 *  Created on: Feb 8, 2021
 *      Author: Ahmed Ramadan
 */


#include"STD_TYPES.h"
#include"BIT_MATH.h"
#include "LCD_interface.h"
#include "main.h"






void LCD_VoidDisplayTheMainScreen(void)
{


	LCD_VidWriteCommand(CLR);
	LCD_VidWriteString("C:");//DATA=3
	LCD_VidSetDdram(1,7);
	LCD_VidWriteString("T:");//Data=9
	LCD_VidWriteNumber(AlarmingSystem.Threshold);
	LCD_VidSetDdram(1,12);
	LCD_VidWriteString("AS:");//DATA=15
	LCD_VidWriteData(AlarmingSystem.AlarmingStatus);

	LCD_VidSetDdram(2,1);
	LCD_VidWriteString("TC:C");//DATA=4

	LCD_VidSetDdram(2,8);
	LCD_VidWriteString("AST:T");//DATA=12







}


void LCD_VoidDisplayTheConfigurationScreen(void)
{
	LCD_VidWriteCommand(CLR);
	LCD_VidWriteString("OK:O");//DATA=4
	LCD_VidSetDdram(1,11);
	LCD_VidWriteString("CN:C");//DATA=4

}


void  LCD_VoidDisplayTheAlarmmingScreen(void)
{
	LCD_VidWriteCommand(CLR);
	LCD_VidSetDdram(1,5);
	LCD_VidWriteString("Alarming");//DATA=4
	LCD_VidSetDdram(2,6);
	LCD_VidWriteString("SA:S");//DATA=4
}


void  LCD_VoidDisplayAlarmStatuscreen(void)
{
	LCD_VidSetDdram(1,15);

	LCD_VidWriteData(AlarmingSystem.AlarmingStatus);
}


void LCD_VoidDisplayAndUpdateTheCurrentTemp(void)
{
	LCD_VidSetDdram(1,3);

	LCD_VidWriteNumber(AlarmingSystem.CurrentTemp);

}


void LCD_VoidDisplayAndUpdateThershold(void)
{
	LCD_VidSetDdram(1,9);

	LCD_VidWriteNumber(AlarmingSystem.Threshold);


}

