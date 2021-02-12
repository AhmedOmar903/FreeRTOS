
	#include "STD_TYPES.h"
	#include "BIT_MATH.h"
	#include "DIO_interface.h"
	#include "LCD_interface.h"
	#include "avr/delay.h"

	void LCD_VidEnable(void){
		DIO_VidSetPinValue(CTRL_PORT,ES_PIN,HIGH); // EN raising edge//
		_delay_ms(1);
		DIO_VidSetPinValue(CTRL_PORT,ES_PIN,LOW);// EN falling edge//
		_delay_ms(1);
		
	}


	/*u8 loc_u8DataPort=the port that connected from D0 to D7
	u8   loc_u8ControlPort=the port that connected to RS&RW&EN
	*/
	void LCD_VidInit(void){
		_delay_ms(50);
	u8 i=0;	
	u8 array[]={RS_PIN,RW_PIN,ES_PIN};	
	while(i<3){
		DIO_VidSetPinDirection(CTRL_PORT,array[i],HIGH);
	i++;	
	}
	#if LCD_MODE==FULL	
		 DIO_VidSetPortDirection(DATA_PORT,255);
		 LCD_VidWriteCommand(FUNCTION_SET);    //function set  //
		 _delay_ms(1);
		 LCD_VidWriteCommand(DISPLAY_ON);    //display on    //
		 _delay_ms(1);
		 LCD_VidWriteCommand(CLR );        //clear display //
		 _delay_ms(2);
	#elif LCD_MODE==HALF
		 DIO_VidSetPortDirection(DATA_PORT,0b11110000);//connect to D4~D7//
		 LCD_VidWriteCommand(FUNCTION_SET4);    //function set  //
		 _delay_ms(1);
		 LCD_VidWriteCommand(DISPLAY_ON);    //display on    //
		 _delay_ms(1);
		 LCD_VidWriteCommand(CLR );        //clear display //
		 _delay_ms(2);

		 
		 
		 
	#endif	 
		 
	}
	/*

	loc_u8DataPort=the port that connected from D0 to D7
	loc_u8ControlPort=the port that connected to RS&RW&EN
	*/
	void LCD_VidWriteCommand(u8 loc_u8command){

	#if LCD_MODE==FULL	
	DIO_VidSetPinValue(CTRL_PORT,RS_PIN,LOW); //make the RS in command mode//
	DIO_VidSetPinValue(CTRL_PORT,RW_PIN,LOW);//make the RW in write mode//
	DIO_VidSetPortValue(DATA_PORT,loc_u8command);//take the data from D0 to D7//
	LCD_VidEnable();
	#elif LCD_MODE==HALF



	DIO_VidSetPinValue(CTRL_PORT,RS_PIN,LOW); //make the RS in command mode//
	DIO_VidSetPinValue(CTRL_PORT,RW_PIN,LOW);//make the RW in write mode//
	DIO_VidSetPortValue(DATA_PORT,loc_u8command);//take the data from D4 to D7//
	LCD_VidEnable();

	DIO_VidSetPortValue(DATA_PORT,loc_u8command<<4);//take the data from D4 to D7//
	LCD_VidEnable();
	#endif		
			
	}
	void LCD_VidWriteData(u8 loc_u8data){
		
	#if LCD_MODE==FULL		
		DIO_VidSetPinValue(CTRL_PORT,RS_PIN,HIGH); //make the RS in data mode//
		DIO_VidSetPinValue(CTRL_PORT,RW_PIN,LOW);//make the RW in write mode//
		DIO_VidSetPortValue(DATA_PORT,loc_u8data);//take the data from D0 to D7//
		LCD_VidEnable();
	#elif LCD_MODE==HALF

		DIO_VidSetPinValue(CTRL_PORT,RS_PIN,HIGH); //make the RS in data mode//
		DIO_VidSetPinValue(CTRL_PORT,RW_PIN,LOW);//make the RW in write mode//
		DIO_VidSetPortValue(DATA_PORT,loc_u8data);//take the data from D4 to D7//
		LCD_VidEnable();	
		DIO_VidSetPortValue(DATA_PORT,loc_u8data<<4);//take the data from D4 to D7//
		LCD_VidEnable();		
	#endif	
		
	}
	void LCD_VidWriteString(u8* loc_u8string){
		u8 i=0;
		for(i=0; loc_u8string[i]!=D_NULL;i++){
		LCD_VidWriteData(loc_u8string[i]);
		}
		
	}
	void LCD_VidWriteNumber(s32 loc_s32number){
// NEGATIVE AND POSTIVE VALUES//

	 s32 x=loc_s32number;
	 s32 y=0;
	 s32 reversed=1;
	 u8 negtive_flag=0;
		 if(loc_s32number<0){
					 x=(loc_s32number*-1);
					 negtive_flag=1;
			 }

		 else if(x==0){
					 LCD_VidWriteData(48);
				}
		 while(x>0){
					 reversed=(reversed*10)+(x%10);
					   x=x/10;
				   }
		 if(loc_s32number<0){
					 x=(loc_s32number*-1);
		 }
		 else{
					x=loc_s32number;
			 }
		 while(x>0){
					y=reversed%10;
					if(negtive_flag==1){
					 LCD_VidWriteData('-');
					  negtive_flag=0;
									}
					LCD_VidWriteData(y+48);
					reversed=reversed/10;
					x=x/10;
		 }
	}


	void LCD_VidSetDdram(u8 loc_u8row,u8 loc_u8col){
		if(1==loc_u8row){
			LCD_VidWriteCommand((127+loc_u8col));
		}
		else if(2==loc_u8row){

			LCD_VidWriteCommand((127+64+loc_u8col));
		}
	}




