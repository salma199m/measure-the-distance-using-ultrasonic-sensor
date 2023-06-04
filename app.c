/*
 * app.c
 *
 *  Created on: ٢٦‏/٠٤‏/٢٠٢٣
 *      Author: lap shop
 */

#include "lcd.h"
#include"avr/io.h"
#include"ultrasonic.h"
#include"util/delay.h"

int main() {
	uint16 dist = 0;
	SREG |= (1 << 7);
	LCD_init();
	Ultrasonic_init();
	LCD_displayString("Distance= ");
	while (1)
	{
		dist = Ultrasonic_readDistance();
		_delay_ms(10);
		if (dist >= 100)
		{
			LCD_moveCursor(0, 10);
			LCD_intgerToString(dist - 2);
			LCD_displayString("Cm ");
		}
		else if (dist >= 10 && dist < 100)
		{
			LCD_moveCursor(0, 10);
			LCD_intgerToString(dist - 3);
			LCD_displayCharacter(' ');
			LCD_displayString("Cm ");
		}
		else
		{
			LCD_moveCursor(0, 10);
			LCD_intgerToString(dist - 3);
			LCD_displayCharacter(' ');
			LCD_displayCharacter(' ');
			LCD_displayString("Cm ");
		}
	}
}
