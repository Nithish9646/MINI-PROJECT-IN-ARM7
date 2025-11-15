#include<lpc21xx.h>
#include "miniheader.h"
#define motor (1<<17)
#define IR_ENTRY 16
#define IR_EXIT	 14
#define TRIG (1<<12)
#define ECHO 13
#define BUZZER (1<<18)
int entrycount=1;
char exitcount;
void delay_ms(int ms)
{
T0PR=15000-1;
T0TCR=0X01;
while(T0TC<ms);
T0TCR=0X03;
T0TCR=0X00;
}
void delay_us(int ms)
{
T0PR=15-1;
T0TCR=0X01;
while(T0TC<ms);
T0TCR=0X03;
T0TCR=0X00;
}
void entry(void) __irq
{
EXTINT=0X01;
LCD_CMD(0x01);
LCD_CMD(0X80);
STRING("VEHICLE:");
LCD_CMD(0X87);
INTEGER(entrycount);
entrycount++;
IOSET0=motor;
delay_ms(1000);
IOCLR0=motor;
VICVectAddr=0;
}
void exit(void) __irq
{
EXTINT=0X02;
LCD_CMD(0x01);
LCD_CMD(0X80);
STRING("VEHICLE:");
LCD_CMD(0X87);
exitcount=entrycount;
exitcount--;
if(exitcount<=0)
exitcount=0;
INTEGER(exitcount);
IOSET0=motor;
delay_ms(1000);
IOCLR0=motor;
VICVectAddr=0;
}
int main()
{
float limit=10.0;
int count;
float distance;
IODIR0=motor|TRIG|BUZZER;
PINSEL1=0X1;
PINSEL0=1<<29;
VICIntSelect=0;
VICVectCntl0=(0x20)|14;
VICVectAddr0=(unsigned long)entry;
VICVectCntl1=(0x20)|15;
VICVectAddr1=(unsigned long)exit;
EXTMODE=0X03;
EXTPOLAR=0X00;
VICIntEnable=1<<14|1<<15;
while(1)
	{	
		//FOR ULTRASONIC

		T1PR=15;
		IOSET0=TRIG;
		delay_us(10);
		IOCLR0=TRIG;

		while(((IOPIN0 >> ECHO)&1)==0);
		T1TCR=0X01;
		while(((IOPIN0 >>ECHO)&1)==1);
		count=T1TC;
		T1TCR=0X03;
		T1TCR=0X00;
		distance=count/58.0;
		
		// FOR BUZZER
		if(distance<limit)
		IOCLR0=BUZZER;
		else
		IOSET0=BUZZER;

	}
}
