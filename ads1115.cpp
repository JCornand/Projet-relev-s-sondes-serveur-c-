#include "ads1115.h"


#include <sys/types.h> // open
#include <sys/stat.h>  // open
#include <fcntl.h>     // open
#include <linux/i2c-dev.h> // I2C bus definitions
#include <unistd.h>    // read/write usleep
#include <sys/ioctl.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>

using namespace std;
#include <iostream>

/* 
	Pour remplacer les "goto" faire des "do while"
	en traitant les cas d'erreur de la fonction testée.
	
		if ((fd = open("/dev/i2c-1", O_RDWR)) < 0) 							do 
			{																{
				perror("Error: Couldn't open /dev/i2c-1");         ===> 		fd = open("/dev/i2c-1", O_RDWR)	
				goto debut;													}
			}																while(#cas d'erreur de "open");
*/


ads1115::ads1115(void) 
{
	ads1115(0x48);
}

ads1115::ads1115(unsigned char address)
{
	cout<<"Adresse i2C : 0x"<<hex<<(int)address<<endl;
	debut:
		if ((fd = open("/dev/i2c-1", O_RDWR)) < 0) 
		{
			perror("Error: Couldn't open /dev/i2c-1");
			goto debut;
		}
		else
		{
			// connect to Ads1115 as i2c slave
			if (ioctl(fd, I2C_SLAVE, address) < 0) 
			{
				perror("Error: Couldn't find device on address!");
				goto debut;
			}
			else
			{
				cout<<"Ouverture peripherique 0x"<<hex<<(int)address<<" OK"<<endl;
			}
		}

	writeBuf[1] = 0b10000001;
	writeBuf[2] = 0b10000101;
}

float ads1115::get_voltage(float gain,int quantum) 
{
	const float VPS = gain / quantum; //volts per step
	float voltage;
	int16_t val;
	uint8_t readBuf[2];

	debut:
		//
		// begin conversion
		if (write(fd, writeBuf, 3) != 3) {
		    perror("Write to register 1");
		    goto debut;
		}
		
		
		// wait for conversion complete
		// checking bit 15
		do {
		  if (read(fd, writeBuf, 2) != 2) {
		      perror("Read conversion");
		      goto debut;
		    }
		}
		while ((writeBuf[0] & 0x80) == 0);
		
		
			
		// read conversion register
		// write register pointer first
		readBuf[0] = 0;   // conversion register is 0
		if (write(fd, readBuf, 1) != 1) {
		    perror("Write register select");
		    goto debut; 
		}
		  
		// read 2 bytes
		if (read(fd, readBuf, 2) != 2) {
		    perror("Read conversion");
		    goto debut;
		}

		// convert display results
		val = readBuf[0] << 8 | readBuf[1];

		if (val < 0)   val = 0;

		voltage = val * VPS; // convert to voltage

			         
		
	return(voltage);
}


void ads1115::set_convtype(bool type)
{
	if (type == 1)
	{
		writeBuf[1] = writeBuf[1] | 0b00000001;
	}
	else
	{
		writeBuf[1] = writeBuf[1] & 0b11111110;
	}
}



float ads1115::set_gain(int bin) 
{
	switch(bin)
	{
		case 000:				// gain 6.144V
			writeBuf[0] = 1;    // config register is 1
 			writeBuf[1] = writeBuf[1] & 0b11110001; // bit 15-8 0xD3
 			//writeBuf[2] = 0b10000101; // bits 7-0  0x85
 			gain=6.144;
 			break;
 		case 001:				// gain 4.096V
 			writeBuf[0] = 1;    // config register is 1
 			writeBuf[1] = writeBuf[1] | 0b00000010; // bit 15-8 0xD3
 			writeBuf[1] = writeBuf[1] & 0b11110011;
 			//writeBuf[2] = 0b10000101; // bits 7-0  0x85
 			gain=4.096;
 			break;
 		case 011:				// gain 1.024V
 			writeBuf[0] = 1;    // config register is 1
 			writeBuf[1] = writeBuf[1] | 0b00000110; // bit 15-8 0xD3
 			writeBuf[1] = writeBuf[1] & 0b11110111;
 			//writeBuf[2] = 0b10000101; // bits 7-0  0x85
 			gain=1.024;
 			break;
 		case 100 :				// gain 0.512V
 			writeBuf[0] = 1;    // config register is 1
 			writeBuf[1] = writeBuf[1] | 0b00001000; // bit 15-8 0xD3
 			writeBuf[1] = writeBuf[1] & 0b11111001;
 			//writeBuf[2] = 0b10000101; // bits 7-0  0x85
 			gain=0.512;
 			break;
 		case 101:				// gain 0.256V
 		 	writeBuf[0] = 1;    // config register is 1
 			writeBuf[1] = writeBuf[1] | 0b00001010; // bit 15-8 0xD3
 			writeBuf[1] = writeBuf[1] & 0b11111011;
 			//writeBuf[2] = 0b10000101; // bits 7-0  0x85
 			gain=0.256;
 			break;
 		case 110:				// gain 0.256V
 			writeBuf[0] = 1;    // config register is 1
 			writeBuf[1] = writeBuf[1] | 0b00001100; // bit 15-8 0xD3
 			writeBuf[1] = writeBuf[1] & 0b11111101;
 			//writeBuf[2] = 0b10000101; // bits 7-0  0x85
 			gain=0.256;
 			break;
 		case 111:				// gain 0.256V
 		 	writeBuf[0] = 1;    // config register is 1
 			writeBuf[1] = writeBuf[1] | 0b00001110; // bit 15-8 0xD3
 			//writeBuf[2] = 0b10000101; // bits 7-0  0x85
 			gain=0.256;
 			break;
		case 010 : 				//010  gain 2.048 V 
			writeBuf[0] = 1;    // config register is 1
 			writeBuf[1] = writeBuf[1] | 0b00000100; // bit 15-8 0xD3
 			writeBuf[1] = writeBuf[1] & 0b11110101;
 			//writeBuf[2] = 0b10000101; // bits 7-0  0x85
 			gain=2.048;
 			break;
		
		default:					  //010  gain 2.048 V
			writeBuf[0] = 1;   		  // config register is 1
 			writeBuf[1] = writeBuf[1] | 0b00000100; // bit 15-8 0xD3
 			writeBuf[1] = writeBuf[1] & 0b11110101;
 			//writeBuf[2] = 0b10000101; // bits 7-0  0x85
 			gain=2.048;
 			break;
	}
	return(gain);
}



void ads1115::set_input(int number)
{
	switch (number)
	{
		case 0:	//entrée A0 donc 100
			writeBuf[1] = writeBuf[1] | 0b01000000;
			writeBuf[1] = writeBuf[1] & 0b11001111;
			break;
		
		case 1: //entrée A1 donc 101
			writeBuf[1] = writeBuf[1] | 0b01010000;
			writeBuf[1] = writeBuf[1] & 0b11011111;
			break;
			
		case 2: //entrée A2 donc 110
			writeBuf[1] = writeBuf[1] | 0b01100000;
			writeBuf[1] = writeBuf[1] & 0b11101111;
			break;
		
		case 3: //entrée A3 donc 111
			writeBuf[1] = writeBuf[1] | 0b01110000;
			break;

		default:
			break;
	}
}