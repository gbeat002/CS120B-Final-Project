/*
 * Project_Alpha.c
 *
 * Created: 2/28/2020 11:59:58 AM
 * Author : grant
 */ 

#define F_CPU 8000000UL		/* Define CPU Frequency e.g. here its 8MHz */
#include "io.c"
#include <avr/io.h>		/* Include AVR std. library file */
#include <util/delay.h>		/* Include inbuilt defined Delay header file */
#include "usart_ATmega1284.h"
#include <avr/eeprom.h>

#define LCD_Data_Dir DDRC	/* Define LCD data port direction */
#define LCD_Command_Dir DDRD	/* Define LCD command port direction register */
#define LCD_Data_Port PORTC	/* Define LCD data port */
#define LCD_Command_Port PORTD	/* Define LCD data port */
//#define RS 6			/* Define Register Select signal pin */
#define RW 5			/* Define Read/Write signal pin */
#define EN 7			/* Define Enable signal pin */
unsigned int hard=3;
unsigned int easy=2;
unsigned char PB_0 = 0x00;
unsigned char PA_0 = 0x00;
unsigned char PA_1 = 0x00;
unsigned int i=0;
unsigned int j=0;
unsigned int k=0;
unsigned int x=0;
unsigned char Pos;
unsigned int b[11]={1,7,5,4,7,9,6,7,8,5,3};
unsigned int f=0;
unsigned char bit='\'';
unsigned int Char_Pos;
unsigned int flag=0;//used to count points
unsigned int dead=0;//used to figurre out if the character is dead
unsigned int win=0;
unsigned char Byte=0x00;
unsigned char starter=1;
unsigned char HS_1=0;
unsigned char HS_2=0;
unsigned char HS_3=0;
unsigned char HS_4=0;
unsigned char HS_5=0;
unsigned int score=0;
unsigned int End_Count=0;
unsigned int num_1=0;
unsigned int num_2=0;
unsigned int num_3=0;
uint8_t value;
int highscore;
int win_condition=0;
char Intro[]="Yo";
char Intro_0[]="Bit Dodge";
char Intro_1[]="Dodge the walls";
char Intro_2[]="to survive.";
char Intro_3[]="Collect bits to";
char Intro_4[]="increase score.";
char Intro_5[]="High Score";
                                                                                       //* Custom char set for alphanumeric LCD Module */ //source:https://www.electronicwings.com/avr-atmega/lcd-custom-character-display-using-atmega-16-32-
void LCD_Command(unsigned char cmnd)
{
	LCD_Data_Port= cmnd;
	LCD_Command_Port &= ~(1<<RS);	/* RS=0 command reg. */
	LCD_Command_Port &= ~(1<<RW);	/* RW=0 Write operation */
	LCD_Command_Port |= (1<<EN);	/* Enable pulse */
	_delay_us(1);
	LCD_Command_Port &= ~(1<<EN);
	_delay_ms(2);
}

void LCD_Char (unsigned char char_data)  /* LCD data write function */
{
	LCD_Data_Port= char_data;
	LCD_Command_Port |= (1<<RS);	/* RS=1 Data reg. */
	LCD_Command_Port &= ~(1<<RW);	/* RW=0 write operation */
	LCD_Command_Port |= (1<<EN);	/* Enable Pulse */
	_delay_us(1);
	LCD_Command_Port &= ~(1<<EN);
	_delay_ms(2);			/* Data write delay */
}

void LCD_Init (void)			/* LCD Initialize function */
{
	LCD_Command_Dir = 0xFF;		/* Make LCD command port direction as o/p */
	LCD_Data_Dir = 0xFF;		/* Make LCD data port direction as o/p */
	_delay_ms(20);			/* LCD Power ON delay always >15ms */
	
	LCD_Command (0x38);		/* Initialization of 16X2 LCD in 8bit mode */
	LCD_Command (0x0C);		/* Display ON Cursor OFF */
	LCD_Command (0x06);		/* Auto Increment cursor */
	LCD_Command (0x01);		/* clear display */
	_delay_ms(2);			/* Clear display command delay> 1.63 ms */
	LCD_Command (0x80);		/* Cursor at home position */
}


void LCD_String (char *str)		/* Send string to LCD function */
{
	int i;
	for(i=0;str[i]!=0;i++)		/* Send each char of string till the NULL */
	{
		LCD_Char (str[i]);
	}
}

void LCD_String_xy (char row, char pos, char *str)  /* Send string to LCD with xy position */
{
	if (row == 0 && pos<16)
	LCD_Command((pos & 0x0F)|0x80);	/* Command of first row and required position<16 */
	else if (row == 1 && pos<16)
	LCD_Command((pos & 0x0F)|0xC0);	/* Command of first row and required position<16 */
	LCD_String(str);		/* Call LCD string function */
}

void LCD_Clear()
{
	LCD_Command (0x01);		/* clear display */
	LCD_Command (0x80);		/* cursor at home position */
}


void LCD_Custom_Char (unsigned char loc, unsigned char *msg)
{
	unsigned char i;
	if(loc<8)
	{
		LCD_Command (0x40 + (loc*8));	/* Command 0x40 and onwards forces the device to point CGRAM address */
		for(i=0;i<8;i++)	/* Write 8 byte for generation of 1 character */
		LCD_Char(msg[i]);
	}
}


                                                                                                                       //* Custom char set for alphanumeric LCD Module */ //source:https://www.electronicwings.com/avr-atmega/lcd-custom-character-display-using-atmega-16-32-

//state machine for wall

enum Wall_States { Wall_Start, Character_1, Base, done} Wall_State;

void Light_Wall(){
	PA_1=(Byte)&0x01;
	PA_0=(Byte)&0x02;
	switch(Wall_State) {   // Transitions
		case Wall_Start:  // Initial transition
		score=0;
		Char_Pos=3;
		f=0;
		x=0;
		i=0;
		Pos=0x80;
		Wall_State=Base;
		break;
		case Base:
		
		LCD_Command(Pos);
		for(i=0;i<b[f];i++)		/* function will send data 1 to 8 to lcd */
		{
			//	LCD_Char(i);		/* char at 'f'th position will display on lcd */
			LCD_Char(x);		/* space between each custom char. */
		}
		LCD_Char(' ');
		LCD_Char(bit);
		LCD_Char(' ');
		for(i=0;i<(13-b[f]);i++)		/* function will send data 1 to 8 to lcd */
		{
			
			LCD_Char(x);		/* space between each custom char. */
		}
		
		
		
		if(x==0){
		LCD_Clear();}
		if(x==8){
			
		LCD_Clear();}
		
		j=j+1;
		if((j%hard)!=0){
		if(flag){score+=1;}
		x=x+1;}
		if(j==16){
		j=0;}
		
		
		Wall_State=Character_1;
		break;
		case Character_1:
		k=k+1;
		if(k>1){
		k=0;
		if(dead||win){
		Wall_State=done;
		}
		else if(PA_1&&(Char_Pos<16)){
			LCD_Cursor_2(Char_Pos);
			LCD_WriteData(' ');
			Char_Pos=Char_Pos+1;
			Wall_State=Base;
		}
		else if(PA_0&&(Char_Pos>1)){
			LCD_Cursor_2(Char_Pos);
			LCD_WriteData(' ');
			Char_Pos=Char_Pos-1;
			Wall_State=Base;
		}
		else{
			Wall_State=Base;}
		}
		else{
		Wall_State=Base;}
			
			
		break;
		case done:
		Wall_State=done;
		break;
		
		
		default:
		Wall_State = Wall_Start;
		break;
	} // Transitions

	switch(Wall_State) {   // State actions
		case Wall_Start:
		
		break;
		
		case Base:
		if((Char_Pos==b[f]+2)&&(x==15)){
		bit='+'; flag=1;}
		else if((x==4||x==12)&&(flag==0)){
		bit=',';}
		else if(x==8||x==16){
		bit='\'';flag=0;}
		
		if(win_condition==2){
		win_condition=0;
		win=1;	
		}
		if(f>9){
			f=0;
			win_condition+=1;
		}
		
		if(x>15){
			x=0;
			f=f+1;
		}
		
		if(x>7){
			Pos=0xC0;
		}
		else{
			Pos=0x80;
		}
		
		break;
		case Character_1:
		LCD_Cursor_2(Char_Pos);
		if(!dead){
		LCD_Char(7);}
		if(((Char_Pos<=b[f])||(Char_Pos>b[f]+3))&&(x>=15)){dead=1;}
		break;
		
		case done:
		starter=0;
		break;
	} // State actions
}

//state machine for wall


//state machine for start screen
enum Start_States {Start_1, Display_1, Display_2, Display_3} Start_State;

void Light_Start(){
PB_0=(~PINB)&0x01;
switch(Start_State){//transitions
case Start_1:
i=0;
Start_State=Display_1;
break;
case Display_1:

if(i==10){LCD_DisplayString(1,Intro_0);}
if(i==2500){LCD_DisplayString(1,Intro_1);}
if(i==5000){LCD_DisplayString(1,Intro_2);}
if(i==7500){LCD_DisplayString(1,Intro_3);}
if(i==10000){LCD_DisplayString(1,Intro_4);}
if(i==12500){LCD_DisplayString(1,Intro_5);}	
if(i==13500){i=0; x=0; Start_State=Display_2;}
else{Start_State=Display_1;}			
break;
case Display_2:
if(i==100){
i=0;	
LCD_Cursor_2(0);
LCD_WriteData(HS_1+'A');
LCD_WriteData(HS_2+'A');
LCD_WriteData(HS_3+'A');
LCD_WriteData(' ');
LCD_WriteData(HS_4+'0');
LCD_WriteData(HS_5+'0');
Start_State=Display_3;
}
else{Start_State=Display_2;}
break;
case Display_3:
if(PB_0){
LCD_Clear();	
starter=0;
win=0;
dead=0;
x=0;
i=0;
Wall_State = Wall_Start;	
Start_State=Start_1;}
else if(i==2500){
	i=0;
	LCD_Clear();
	LCD_DisplayString(2," Press  Start");
	Start_State=Display_3;
}
else{Start_State=Display_3;}
break;
default:
Start_State=Start_1;
break;	
	
}
switch(Start_State){//state actions
case Start_1:
break;
case Display_1:
case Display_2:
case Display_3:
if(PB_0){Start_State=Display_3;}
i=i+1;
break;

}

	
}

//state machine for start screen

//end state machine


enum End_States {End_1, Dis_1, Dis_2, Dis_3, Dis_4, Dis_5} End_State;

void Light_End(){
PA_1=(Byte)&0x01;
PA_0=(Byte)&0x02;
PB_0=(~PINB)&0x01;
switch(End_State){//transitions
case End_1:
i=0;
if(win){LCD_DisplayString(1,"   You Win!");}
else{LCD_DisplayString(1,"   Game Over!");}
End_State=Dis_1;
break;
case Dis_1:
if(i==10){
LCD_DisplayString(1,"Your Score:");
LCD_Char('0'+(score%100)/10);
LCD_Char('0'+(score%10));}
if(i==20){i=0; End_State=Dis_2;}
else{End_State=Dis_1;}	
break;
case Dis_2:
if(score>highscore){
LCD_DisplayString(1,"New High Score!");
x=0;	
num_1=0;
num_2=0;
num_3=0;
if(i>10){
i=0;		
End_State = Dis_3;}	
}
else if(score<=highscore){
	i=0;
	dead=0;
	win=0;
	starter=1;
	Wall_State = Wall_Start;
	Start_State = Start_1;
	End_State = End_1;
	}
else{End_State=Dis_2;}	
break;
case Dis_3:
if(i==0){num_1=x;}
if(i==1){num_2=x;}
if(i==2){num_3=x;}
if(i==3){End_State=Dis_5; i=0; x=0;}	
else if(PB_0){
	i=i+1;
	End_State=Dis_4;}
else{End_State=Dis_3;}	
break;
case Dis_4:
if(!PB_0){End_State=Dis_3;}
else{End_State=Dis_4;}	
break;
case Dis_5:
eeprom_write_byte (( uint8_t *) 64,score);
eeprom_write_byte (( uint8_t *) 63,num_1);
eeprom_write_byte (( uint8_t *) 62,num_2);
eeprom_write_byte (( uint8_t *) 61,num_3);
dead=0;
win=0;
starter=1;
Wall_State = Wall_Start;
Start_State = Start_1;
End_State = End_1;
break;
default:
End_State=End_1;
break;	
}
switch(End_State){//state actions
case End_1:
break;
case Dis_1:
i=i+1;
break;
case Dis_2:
i=i+1;
break;
case Dis_3:
if((PA_0)&&(x>0)){
	x=x-1;
}
else if((PA_1)&&(x<25)){
	x=x+1;
}
LCD_DisplayString(1,"IGN:");
LCD_Char('A'+num_1);
LCD_Char('A'+num_2);
LCD_Char('A'+num_3);

break;
case Dis_4:
if((PA_0)&&(x>0)){
	x=x-1;
}
else if((PA_1)&&(x<25)){
	x=x+1;
}
LCD_DisplayString(1,"IGN:");
LCD_Char('A'+num_1);
LCD_Char('A'+num_2);
LCD_Char('A'+num_3);
break;
case Dis_5:
break;
}
}
//end state machine





int main()
{	DDRA = 0xFF; PORTA = 0x00;
	DDRB = 0x00; PORTB = 0xFF;
	DDRC = 0xFF; PORTC = 0x00; // Configure port B's 8 pins as outputs, initialize to 0s
	DDRD = 0xFA; PORTD = 0x05; //C is output
	
	char i;
	
	unsigned char Character1[8] = { 0x1f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };  //* Custom char set for alphanumeric LCD Module */ //source:https://www.electronicwings.com/avr-atmega/lcd-custom-character-display-using-atmega-16-32-
	unsigned char Character2[8] = { 0x00, 0x1F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
	unsigned char Character3[8] = { 0x00, 0x00, 0x1f, 0x00, 0x00, 0x00, 0x00, 0x00 };
	unsigned char Character4[8] = { 0x00, 0x00, 0x00, 0x1F, 0x00, 0x00, 0x00, 0x00 };
	unsigned char Character5[8] = { 0x00, 0x00, 0x00, 0x00, 0x1f, 0x00, 0x00, 0x00 };
	unsigned char Character6[8] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x1f, 0x00, 0x00 };
	unsigned char Character7[8] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1f, 0x00 };
	unsigned char Character8[8] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1f };
	
	initUSART(0);
	LCD_Init();
	unsigned int TimerCount=1;
	TimerSet(TimerCount);
	TimerOn();
	Wall_State = Wall_Start;
	Start_State = Start_1;
	End_State = End_1;
	LCD_Custom_Char(0, Character1);  /* Build Character1 at position 0 */
	LCD_Custom_Char(1, Character2);  /* Build Character2 at position 1 */
	LCD_Custom_Char(2, Character3);  /* Build Character3 at position 2 */
	LCD_Custom_Char(3, Character4);  /* Build Character4 at position 3 */
	LCD_Custom_Char(4, Character5);  /* Build Character5 at position 4 */
	LCD_Custom_Char(5, Character6);  /* Build Character6 at position 5 */
	LCD_Custom_Char(6, Character7);  /* Build Character6 at position 6 */
	LCD_Custom_Char(7, Character8);  /* Build Character6 at position 7 */
	                                                                                 //* Custom char set for alphanumeric LCD Module */ //source:https://www.electronicwings.com/avr-atmega/lcd-custom-character-display-using-atmega-16-32-

  




	while(1){
		
		
		highscore=eeprom_read_byte ((const uint8_t*)64);//           all of
//	eeprom_write_byte (( uint8_t *) 64,0x02);//
	HS_1=eeprom_read_byte ((const uint8_t*)63);
	HS_2=eeprom_read_byte ((const uint8_t*)62);
	HS_3=eeprom_read_byte ((const uint8_t*)61);
	HS_4=(highscore%100)/10;//                                these need to be in loop
	HS_5=(highscore%10);//
		
		
		
			if(USART_HasReceived(0)){
			Byte = USART_Receive(0);}
					
		

			if((starter==0)){Light_Wall();}		
	   else if((starter==1)){Light_Start();}
				
			if(End_Count>250){	
			End_Count=0;	
			if((dead)||(win)){Light_End();}
			}
	
		

			while(!TimerFlag){};
			TimerFlag=0;
			End_Count+=TimerCount;
		
		
		
		
		
		
		
	};
}