#ifndef _DIO_INTERFACE_H_
#define _DIO_INTERFACE_H_

#define OUTPUT   1
#define INPUT    0
#define PORTA    0
#define PORTB    1
#define PORTC    2
#define PORTD    3
#define HIGH     1
#define LOW      0

//  loc_u8port=port name   loc_u8pin=pin number      loc_u8state=pin state (input or output)  //
//set the pin direction//
void DIO_VidSetPinDirection(u8 loc_u8port,u8 loc_u8pin, u8 loc_u8state);
// set PIN value//
void DIO_VidSetPinValue(u8 loc_u8port,u8 loc_u8pin, u8 loc_u8value);
//set the port direction//
void DIO_VidSetPortDirection(u8 loc_u8port,u8 loc_u8value);
// set port value//
void DIO_VidSetPortValue(u8 loc_u8port,u8 loc_u8value);
//get the value of the pin//
u8 DIO_u8GetPinValue(u8 loc_u8port,u8 loc_u8pin);

void DIO_VidTogBit(u8 loc_u8port,u8 LOC_u8BitNum);

#endif