/*
 * ultrasonic.c
 *
 *  Created on: ٢٦‏/٠٤‏/٢٠٢٣
 *      Author: lap shop
 */
#include"ultrasonic.h"
#include"icu.h"
#include"util/delay.h"
/*******************************************************************************
 *                               Declarations                                   *
 *******************************************************************************/
static volatile uint8 g_edgeCount = 0;
static volatile uint16 time_pulse = 0;
/*
 * Description
 *  Initialize the ICU driver as required.
 *  Setup the ICU call back function.
 *  Setup the direction for the trigger pin as output pin through the
 *  GPIO driver.
 */
void Ultrasonic_init(void) {
	Icu_ConfigType Icu_Config = { F_CPU_8, RISING };
	GPIO_setupPinDirection(ULTRASONIC_TRIGGER_PORT_ID,
	ULTRASONIC_TRIGGER_PIN_ID, PIN_OUTPUT);
	GPIO_writePin(ULTRASONIC_TRIGGER_PORT_ID, ULTRASONIC_TRIGGER_PIN_ID,
			LOGIC_LOW);
	//Setup the ICU call back function.
	Icu_setCallBack(Ultrasonic_edgeProcessing);
	Icu_init(&Icu_Config);
}
/*
 * Description
 * Send the Trigger pulse to the ultrasonic.*/
void Ultrasonic_Trigger(void) {
	GPIO_writePin(ULTRASONIC_TRIGGER_PORT_ID, ULTRASONIC_TRIGGER_PIN_ID,
			LOGIC_HIGH);
	_delay_us(10);
	GPIO_writePin(ULTRASONIC_TRIGGER_PORT_ID, ULTRASONIC_TRIGGER_PIN_ID,
			LOGIC_LOW);
	// to start with zero in the registers
	Icu_clearTimerValue();
}
/*
 * Description
 * Send the trigger pulse by using Ultrasonic_Trigger function.
 * Start the measurements by the ICU from this moment.*/
uint16 Ultrasonic_readDistance(void) {
	// use the function Ultrasonic_Trigger to send a trigger
	Ultrasonic_Trigger();
	return time_pulse;
}
/*
 *  Description
 *  This is the call back function called by the ICU driver.
 *  This is used to calculate the high time (pulse time) generated by
 *  the ultrasonic sensor.*/
void Ultrasonic_edgeProcessing(void) {
	g_edgeCount++;
	if (g_edgeCount == 1) {
		Icu_clearTimerValue();
		/* Detect falling edge */
		Icu_setEdgeDetectionType(FALLING);
	} else if (g_edgeCount == 2) {
		time_pulse = (uint16)(Icu_getInputCaptureValue() / 58.8);
		g_edgeCount = 0;
		Icu_clearTimerValue();
	}
}

