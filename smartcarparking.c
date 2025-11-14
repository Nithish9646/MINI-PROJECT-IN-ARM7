#include<lpc21xx.h>
#define motor (1<<17)
#define IR 14
#define TRIG (1<<12)
#define ECHO 15
#define BUZZER (1<<18)
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
int main()
{
float limit=10.0;
int count;
float distance;
IODIR0=motor|TRIG|BUZZER;
	while(1)
	{	 //FOR IR 1 
		if(((IOPIN0>>IR)&1)==0)
		  IOSET0=motor;
		else
		IOCLR0=motor;

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
