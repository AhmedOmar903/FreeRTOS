

#ifndef _LCD_INTERFACE_H_
#define _LCD_INTERFACE_H_

#define CLR              0b00000001
#define FUNCTION_SET     0b00111000
#define DISPLAY_ON       0b00001100
#define RETURN_HOME      0b00000010
#define SHIFT_LEFT       0b00011000
#define SHIFT_RIGHT      0b00011100
#define CRUSER_ON        0b00001111
#define CRUSER_LEFT      0b00010000
#define CRUSER_RIGHT     0b00010100
#define FUNCTION_SET4    0b00101000
#define RS_PIN              PIN2
#define RW_PIN              PIN3
#define ES_PIN              PIN4
#define DATA_PORT           PORTC
#define CTRL_PORT           PORTD
#define FULL                  0
#define HALF                  1
#define LCD_MODE            FULL








// LCD FUNCTIONS//
void LCD_VidInit(void);
void LCD_VidEnable(void);
void LCD_VidWriteCommand(u8 loc_u8command);
void LCD_VidWriteData(u8 loc_u8data);
void LCD_VidWriteString(u8* loc_u8string);
void LCD_VidWriteNumber(s32 loc_s32number);
void LCD_VidSetDdram(u8 loc_u8row,u8 loc_u8col);
//              //
#endif

