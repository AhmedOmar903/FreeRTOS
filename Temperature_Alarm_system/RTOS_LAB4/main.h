/*
 * main.h
 *
 *  Created on: Feb 8, 2021
 *      Author: Ramadona
 */

#ifndef MAIN_H_
#define MAIN_H_


typedef struct AlarmSystem
{
	u8 CurrentTemp;
	u8 Threshold;
	u8 AlarmingStatus;
	u8 SystemState;
}Alarm;



Alarm AlarmingSystem;


#endif /* MAIN_H_ */
