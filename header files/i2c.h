#ifndef __I2C_H
#define __I2C_H


typedef unsigned char bool;
#define true 1
#define false 0
#define I2C_DELAY 0x0F /* For delay i2c bus */
#define SDA_DIR TRISCbits.TRISC4
#define SCK_DIR TRISCbits.TRISC3
#define I2C_SPEED	100				// kbps
#define SDA			PORTCbits.RC4				// Data pin for i2c
#define SCK			PORTCbits.RC3				// Clock pin for i2c
#define DS1307_ID 0xD0
void I2C_stop();
void InitI2C();
void I2C_start(void);
void I2C_Restart(void);
void I2C_Send_ACK(void);
void I2C_Send_NACK(void);
unsigned char I2C_read(void);
bool I2C_write(unsigned char Byte);

void InitI2C()
{
	SDA_DIR = 1;		// Make SDA and
	SCK_DIR = 1;		// SCK pins input

	SSPADD  = 0x28;
	SSPSTAT = 0x80;		// Slew Rate control is disabled
	SSPCON1  = 0x28;		// Select and enable I2C in master mode
}


// Function Purpose: I2C_Start sends start bit sequence
void I2C_start(void)
{
	SSPCON2bits.SEN = 1;			// Send start bit
	while(!PIR1bits.SSPIF);		// Wait for it to complete
	PIR1bits.SSPIF = 0;			// Clear the flag bit
}


//// Function Purpose: I2C_ReStart sends start bit sequence
void I2C_Restart(void)
{
	SSPCON2bits.RSEN = 1;			// Send Restart bit
	while(!PIR1bits.SSPIF);		// Wait for it to complete
	PIR1bits.SSPIF = 0;			// Clear the flag bit
}


//Function : I2C_Stop sends stop bit sequence
void I2C_stop()
{
	SSPCON2bits.PEN = 1;			// Send stop bit
	while(!PIR1bits.SSPIF);		// Wait for it to complete
	PIR1bits.SSPIF = 0;			// Clear the flag bit
}



//Function : I2C_Send_ACK sends ACK bit sequence
void I2C_Send_ACK(void)
{
	SSPCON2bits.ACKDT = 0;			// 0 means ACK
	SSPCON2bits.ACKEN = 1;			// Send ACKDT value
	while(!PIR1bits.SSPIF);		// Wait for it to complete
	PIR1bits.SSPIF = 0;			// Clear the flag bit
}


//Function : I2C_Send_NACK sends NACK bit sequence
void I2C_Send_NACK(void)
{
	SSPCON2bits.ACKDT = 1;			// 1 means NACK
	SSPCON2bits.ACKEN = 1;			// Send ACKDT value
	while(!PIR1bits.SSPIF);		// Wait for it to complete
	PIR1bits.SSPIF = 0;			// Clear the flag bit
}


// Function Purpose: I2C_Write_Byte transfers one byte
bool I2C_write(unsigned char Byte)
{
	SSPBUF = Byte;		// Send Byte value
	while(!PIR1bits.SSPIF);		// Wait for it to complete
        PIR1bits.SSPIF = 0;			// Clear the flag bit

	return SSPCON2bits.ACKSTAT;		// Return ACK/NACK from slave
}


// Function Purpose: I2C_Read_Byte reads one byte
unsigned char I2C_read(void)
{
	SSPCON2bits.RCEN = 1;			// Enable reception of 8 bits
	while(!PIR1bits.SSPIF);		// Wait for it to complete
	PIR1bits.SSPIF = 0;			// Clear the flag bit

    return SSPBUF;		// Return received byte
}

#endif