#ifndef __LCD_H
#define __LCD_H

//Declarations
#define dataport   PORTD			//LCD data port to PORTD
#define dataport_dir   TRISD	
#define cmdport_dir   TRISB			
#define rs         PORTBbits.RB5	//register select signal to RB5
#define rw         PORTBbits.RB4	//read/write signal to RB4
#define e          PORTBbits.RB3	//enable signal to RB3
#define busy       PORTDbits.RD7	//BUSY signal to RD7
#define enbusy     TRISDbits.TRISD7	

char check_4bit=0;
//function to initialise lcd at beginning
void lcd_init(unsigned char bits);
//object to check whether lcd ready to accept data
char lcd_ready();
//function to send command to lcd
void lcdcmd(unsigned char chr);
//Function to send data to LCD
void lcddata(unsigned char chr);
//Function to write string to LCD
void lcd_msg_display(const rom char *a);
//Function to set surser position on LCD
void curser_pos(unsigned char chr1,unsigned char chr2);
//Function to clear LCD
void lcd_clear();
//Function to write variable integer to LCD
void lcd_Decimal_display(unsigned int value);
void lcd_BCD_display(unsigned int value);
//Function to shift entire display to left
void lcd_shift_left(unsigned char time,unsigned int duration);
//Function to shift entire display to right
void lcd_shift_right(unsigned char time,unsigned int duration);

void send_data(unsigned char a);

void delay_ms(unsigned int msec);

void lcd_BCD_display(unsigned int value);

void lcd_var_display(static char *a,unsigned char count);

void lcd_var_display(static char *a,unsigned char count)
{
	int i;
	for(i=0;i<count;i++)
         lcddata(a[i]);
}

void delay_ms(unsigned int msec)
{
int o,p;

	for(p=0;p<msec;p++)
for(o=0;o<60;o++);
}

void lcd_Decimal_display(unsigned int value)
{
unsigned char b,c,d;
	b=value/100;
	lcddata(b+0x30);
	value=value%100;
	c=value/10;
	lcddata(c+0x30);
	value=value%10;
	d=value;
	lcddata(d+0x30);
        }

void lcd_BCD_display(unsigned int value)
{
	   unsigned char b,c;
        b=value&0xF0;
        b=b>>4;
        lcddata(b+0x30);
        c=value&0x0F;
        lcddata(c+0x30);
}
void lcd_init(unsigned char bits)
{
	dataport_dir= dataport_dir & 0x0F;
    	cmdport_dir = cmdport_dir & 0xC7;
    	dataport=0x00;
    	delay_ms(50);
    	lcdcmd(0x30);
    	delay_ms(5);
    	lcdcmd(0x30);
    	delay_ms(1);
    	lcdcmd(0x30);
    	delay_ms(1);
    	if(bits==4)
        {
            lcdcmd(0x20);
            delay_ms(1);
            check_4bit=1;
            lcdcmd(0x28);  // for using 4-bit 2 row mode of LCD
        }
    else if(bits==8)
        {
            lcdcmd(0x30);
            delay_ms(1);
            check_4bit=0;
            lcdcmd(0x38);  // for using 8-bit 2 row mode of LCD
        }
    lcdcmd(0x0C);  // turn display ON for cursor blinking
    lcdcmd(0x01);  //clear screen
    lcdcmd(0x06);  //display ON
    lcdcmd(0x80);  // bring cursor to position 0 of line 1
 }
 //Function to send command to LCD
void lcdcmd(unsigned char chr)
{
    rs=0;
    rw=0;
    if(check_4bit)
    {
        send_data(chr);
    }
    else
    {
        dataport = chr;
        e=1;
        delay_ms(2);
        e=0;
    }
 while(lcd_ready()!=1);
}

void send_data(unsigned char a)
{
      char temp,y;
           temp = a&0x0F;
            y = a&0xF0;
            dataport=y;             //Data transfer
            e = 1;
            delay_ms(2);
            e = 0;
            dataport=(temp<<4);
            e = 1;
            delay_ms(2);
            e = 0;
}
//function to send data to lcd
void lcddata(unsigned char chr)  //Function to send data to LCD
{
    rs=1;
    rw=0;
    if(check_4bit)
    {
        send_data(chr);
    }
    else
    {
        dataport = chr;
        e=1;
        delay_ms(1);
        e=0;

    }
  while(lcd_ready()!=1);
}
void lcd_msg_display(const rom char *a)
{
	int i;
	for(i=0;a[i]!='\0';i++)
         lcddata(a[i]);
}

//function to clear to lcd
void lcd_clear()
{
    lcdcmd(0x01);
}
//function to set curser position on lcd
void curser_pos(unsigned char row,unsigned char col)
{

    unsigned char pos;
	col=col-1;
    if(row==1)
       pos=0x80;
    else if(row==2)
         pos=0xC0;
    pos=pos+col;
    lcdcmd(pos);
}

void lcd_shift_left(unsigned char time,unsigned int duration)
{
    unsigned char z;
    for(z=0;z<time;z++)
    {
        lcdcmd(0x18);
        delay_ms(duration);
    }
}

void lcd_shift_right(unsigned char time,unsigned int duration)
{
    unsigned char z;
    for(z=0;z<time;z++)
    {
        lcdcmd(0x1C);
        delay_ms(duration);
    }

}

//Function to check whether display is ready to accept data
char lcd_ready()
{
    rs= 0;
    rw=1;
    enbusy=1;
    while(busy==1)
        {
            e=0;
            delay_ms(1);
            e=1;
	}
    enbusy=0;
    return 1;
}



#endif
