#include "ads1115a.h"
#include <iostream>
using namespace std;

//
//constructeurs
Ads1115::Ads1115(){

}

Ads1115::Ads1115(unsigned char address){ 
  // Ouverture de l'appareil sur /dev/i2c-1 
  // Par défaut sur  Raspberry Pi B
  if ((fd = open("/dev/i2c-1", O_RDWR)) < 0) {
    perror("Error: Couldn't open device!\n");
    exit(-1);
  }

  // Connecté sur l'ads1115 comme i2c slave
  if (ioctl(fd, I2C_SLAVE, address) < 0) {
    perror("Error: Couldn't find device on address!\n");
    exit(-1);
  }
}

//
//méthodes

void Ads1115::setMUX(int mux)
{
	//Permet la sélection d'une broche en particulier entre A0, A1, A2 et A3
	switch(mux)
	{
		case 0:
			writeBuf[1] = (writeBuf[1] | 0x40) & 0xcf;//A0
			break;
		case 1:
			writeBuf[1] = (writeBuf[1] | 0x50) & 0xdf;//A1
			break;
		case 2:
			writeBuf[1] = (writeBuf[1] | 0x60) & 0xef;//A2
			break;
		case 3:
			writeBuf[1] = (writeBuf[1] | 0x70) & 0xff;//A3
			break;
		default:
			writeBuf[1] = (writeBuf[1] | 0x40) & 0xcf;//A0
			break;
	}
}

void Ads1115::setPGA(int pga)
{
	// Permet de sélectionner un gain pour l'ads1115
	switch(pga)
	{
		case 0:				// gain 6.144V
 			writeBuf[1] = writeBuf[1] & 0xf1; // bit 15-8 0xF1
 			gain_float=6.144;
 			break;
 		case 1:				// gain 4.096V
 			writeBuf[1] = (writeBuf[1] | 0x02) & 0xf3; // bit 15-8 0xF3
 			gain_float=4.096;
 			break;
 		case 2:				// gain 2.048V
 			writeBuf[1] = (writeBuf[1] | 0x04) & 0xf5; // bit 15-8 0xF5
 			gain_float=2.048;
 			break;
 		case 3 :				// gain 1.024V
 			writeBuf[1] =  (writeBuf[1] | 0x06) & 0xf7; // bit 15-8 0xF7
 			gain_float=1.024;
 			break;
 		case 4:				// gain 0.512V
 			writeBuf[1] =   (writeBuf[1] | 0x08) & 0xf9; // bit 15-8 0xF9
 			gain_float=0.512;
 			break;
 		case 5:				// gain 0.256V
 			writeBuf[1] = (writeBuf[1] | 0x0a) & 0xfb; // bit 15-8 0xFB
 			gain_float=0.256;
 			break;
 		case 6:				// gain 0.256V
 			writeBuf[1] = (writeBuf[1] | 0x0c) & 0xfd; // bit 15-8 0xFD
 			gain_float=0.256;
 			break;
 		case 7:				// gain 0.256V
 			writeBuf[1] = (writeBuf[1] | 0x0e) & 0xff; // bit 15-8 0xFF
 			gain_float=0.256;
 			break;
		default: 				// gain 2.048 V 
			writeBuf[1] = (writeBuf[1] | 0x04) & 0xf5; // bit 15-8 0xF5
 			gain_float=2.048;
 			break;
	}
}

int Ads1115::getValue(int input,int pga) {
	
	writeBuf[0] = 1;
  	writeBuf[1] = 0xc5;
  	writeBuf[2] = 0x85;
	setMUX(input);
	setPGA(pga);
	//const float VPS = gain / quantum; //volts per step (échellon)
  	//
  	 // begin conversion
    if (write(fd, writeBuf, 3) != 3) {
      perror("Write to register 1");
      return(val = (-1));
    }
    // Attente de la conversion complète
    // check bit 15
    do {
      if (read(fd, writeBuf, 2) != 2) {//readbuf avant ici
        perror("Read conversion");
        return(val = (-2));
      }
    }
    while ((writeBuf[0] & 0x80) == 0);
    // read conversion register
    // write register pointer first
    readBuf[0] = 0;   // conversion register is 0
    if (write(fd, readBuf, 1) != 1) { //il y avait write ici
      perror("Write register select");
      return(val = (-3));
    }
    // read 2 bytes
    if (read(fd, readBuf, 2) != 2) {
      perror("Read conversion");
      return(val = (-4));
    }
    // convert display results
    val = readBuf[0] << 8 | readBuf[1];

    if (val < 0)   val = 0;
	return(val);
}

float Ads1115::getVoltage(int input, int pga) {
	int conversion;
	float voltage;
	do{
		conversion = getValue(input, pga);
		voltage = conversion * ((gain_float/2)/16383);// Conversion en Volts   gain_float
	} while (conversion < 0);
	return voltage;
}