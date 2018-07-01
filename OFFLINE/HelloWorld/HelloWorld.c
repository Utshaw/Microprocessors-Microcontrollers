#ifndef F_CPU
#define F_CPU 4000000UL // 1 MHz clock speed
#endif
#define D4 eS_PORTD4
#define D5 eS_PORTD5
#define D6 eS_PORTD6
#define D7 eS_PORTD7
#define RS eS_PORTC6
#define EN eS_PORTC7


#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
char buffer[64];


void InitADC()
{
	ADMUX |= (1<<REFS0);
	ADCSRA|= (1<<ADEN)|(1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0);
}

uint64_t ReadADC(uint64_t ch)
{
	
	ch=ch&0b00000111;
	ADMUX|=ch;
	ADCSRA|=(1<<ADSC);
	while(!(ADCSRA & (1<<ADIF)));
	ADCSRA|=(1<<ADIF);
	
	return(ADCW);
}

void foo(){
	buffer[63]='\n';
	float DV,PV;
	int D1,D2,D3;
	//DDRD=0xFF;
	DDRA|=0b00000001;
	//DDRC=0xFF;
	_delay_ms(1000);
	InitADC();
	DV=ReadADC(0);
	PV=DV*5/1023;
	
	buffer[0]='0' + (unsigned int) (PV);
	buffer[1] = '.';
	PV = PV - (unsigned int) (PV);
	PV = PV * 10.0;
	buffer[2] = '0' + (unsigned int) (PV);
	buffer[3] = '\0';  // End of string character

	
	//int ret = snprintf(buffer, sizeof buffer, "%f", PV);
	
	//sprintf(buffer, "%f", PV);
	//dtostrf(buffer, 3, 1, PV);

	
}


#include "lcd.h" //Can be download from the bottom of this article



int main(void)
{
	DDRD = 0xFF;
	DDRC = 0xFF;
	int i;
	Lcd4_Init();
	
	while(1)
	{
		foo();
		DDRD = 0xFF;
		DDRC = 0xFF;
		Lcd4_Init();
		Lcd4_Set_Cursor(1,1);
		Lcd4_Write_String(buffer);
		for(i=0;i<15;i++)
		{
			_delay_ms(500);
			Lcd4_Shift_Left();
		}
		for(i=0;i<15;i++)
		{
			_delay_ms(500);
			Lcd4_Shift_Right();
		}
		Lcd4_Clear();
		Lcd4_Set_Cursor(2,1);
		Lcd4_Write_Char('e');
		Lcd4_Write_Char('S');
		_delay_ms(2000);
	}
}