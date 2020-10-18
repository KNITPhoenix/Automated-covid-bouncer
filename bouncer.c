// we assume that 7 segment hooked to the device is a common cathode one
//signal wire of servo motor is attached to PB5
// IR1 is attached to PA0 and IR2 to PA1
//Buzzer is attached to PD7
#define F_CPU 8000000UL
#include <stdio.h>
#include <avr/io.h>
#include <util/delay.h>
#define SegOne   0x01
#define SegTwo   0x02
#define SegThree 0x04
#define SegFour  0x08
#define max_cap 5  // maximum capacity of restaraunt
int initial_people = 0; //number of people initially and currently
int queue_last=0; //ticket number of person in last of queue
int next_entry=0; //ticket number of person to enter next

int main(void)
{
	DDRA= 0x00;  //input for IR sensors
	DDRC=0xFF; //Data lines for display1
	DDRD=0xFF; //control lines for display1
	DDRE=0xFF; //data lines for display2
	DDRF= 0xFF; //control lines for display2
	display1();
	display2();
	
	if((PINA&0x01)==0x01) //IR1 is ON
	{
		if(initial_people<=5)  //checking for number of people currently
		{
			servo_anti(); //gate opens
			while((PINA&0x02)!=0x02);  //until IR2 is OFF
			while((PINA&0x02)==0x02);  //until IR2 is ON
			servo_clock();  //gate closes
			initial_people=initial_people+1;  //increment the number of people by 1
			display2_incr(); //display incremented value on display2
		}
		else
		{
			PORTD=0x80;  //buzzer is on
			_delay_ms(1000);
			PORTD=0x00;
			queue_last+=1;
			queue_incr();
		}
	}
	
	else if((PINA&0x02)==0x02)  //IR2 detects
	{
		servo_anti(); //gate opens
		while((PINA&0x01)!=0x01);  //until the person passes from IR1
		while((PINA&0x01)==0x01);  //until the person passes from IR1 range
		servo_clock();  //gate closes
		initial_people=initial_people-1; //decrease the current people in rest
		display2_incr();  //display the same
		//now since place is available
		PORTD=0x80; //buzzer on for 3 sec
		_delay_ms(3000);
		PORTD=0x00;  //buzzer off
		next_entry=next_entry+1;
		next_person();
		if((PINA&0x01)==0x01)  //IR1 is ON
		{
			servo_anti();  //gate opens
			while((PINA&0x02)!=0x02);  //until IR2 is OFF
			while((PINA&0x02)==0x02);  //until IR2 is ON
			servo_clock();  //gate closed
			initial_people=initial_people+1;  //increment the current count
			display2_incr();  //display the same
		}
	}
	
}

void servo_anti()  // method to make servo position at 90 degree anticlockwise and gate opens
{
	DDRB |= (1<<PB5);	/* Make OC1A pin as output */
	TCNT1 = 0;		/* Set timer1 count zero */
	ICR1 = 2499;		/* Set TOP count for timer1 in ICR1 register */

	/* Set Fast PWM, TOP in ICR1, Clear OC1A on compare match, clk/64 */
	TCCR1A = (1<<WGM11)|(1<<COM1A1);
	TCCR1B = (1<<WGM12)|(1<<WGM13)|(1<<CS10)|(1<<CS11);
	while(1)
	{
		OCR1A = 65;	/* Set servo shaft at -90° position */
	}
}

void servo_clock()  // method to return servo at initial position and open gate closes
{
	DDRB |= (1<<PB5);	/* Make OC1A pin as output */
	TCNT1 = 0;		/* Set timer1 count zero */
	ICR1 = 2499;		/* Set TOP count for timer1 in ICR1 register */

	/* Set Fast PWM, TOP in ICR1, Clear OC1A on compare match, clk/64 */
	TCCR1A = (1<<WGM11)|(1<<COM1A1);
	TCCR1B = (1<<WGM12)|(1<<WGM13)|(1<<CS10)|(1<<CS11);
	while(1)
	{
		OCR1A = 175;	/* Set servo shaft at 0° position */
	}
}

void display1()  //initialise display1
{
	char seg_code[]={0x3F,0x06,0x5B,0x4F,0x66,0xED,0x7D,0x07,0x7F,0x6F};
	DDRC=0xFF;
	DDRD=0xFF;
	PORTD=SegOne;
	PORTC=seg_code[0];
	PORTD=SegTwo;
	PORTC=seg_code[0];
    PORTD=SegThree;
	PORTC=seg_code[0];
	PORTD=SegFour;
	PORTC=seg_code[0];
}

void display2()  //initialise display2
{
	char seg_code[]={0x3F,0x06,0x5B,0x4F,0x66,0xED,0x7D,0x07,0x7F,0x6F};
	DDRE=0xFF;
	DDRF=0xFF;
	PORTF=SegOne;
	PORTE=seg_code[0];
	PORTF=SegTwo;
	PORTE=seg_code[0];
	PORTF=SegThree;
	PORTE=seg_code[5];
	PORTF=SegFour;
	PORTE=seg_code[0];
}

void display2_incr()  //display2 increment
{
	char seg_code[]={0x3F,0x06,0x5B,0x4F,0x66,0xED,0x7D,0x07,0x7F,0x6F};
	DDRE=0xFF;
	DDRF=0xFF;
	PORTF=SegOne;
	PORTE=seg_code[initial_people];
}

void queue_incr()  //incrementing first 2 digits of display1
{
	char seg_code[]={0x3F,0x06,0x5B,0x4F,0x66,0xED,0x7D,0x07,0x7F,0x6F};
	PORTD=0xFF;
	PORTC=0xFF;
	if(queue_last<10)
	{
	PORTD=SegThree;
	PORTC=seg_code[queue_last];
	}
	else
	{
		int a=queue_last%10;  //last digit
		int b=queue_last/10;  //ten's digit
		PORTD=SegThree;
		PORTC=seg_code[a];
		PORTD=SegFour;
		PORTC=seg_code[b];
	}
}

void next_person()  //incrementing last 2 digits of display1
{
	char seg_code[]={0x3F,0x06,0x5B,0x4F,0x66,0xED,0x7D,0x07,0x7F,0x6F};
	PORTD=0xFF;
	PORTC=0xFF;
	if(next_entry<10)
	{
		PORTD=SegOne;
		PORTC=seg_code[next_entry];
	}
	else
	{
		int a=next_entry%10;  //last digit
		int b=next_entry/10;  //ten's digit
		PORTD=SegOne;
		PORTC=seg_code[a];
		PORTD=SegTwo;
		PORTC=seg_code[b];
	}
}