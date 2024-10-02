#include "ads1115a.h"
#include "pression.h"
#include "temperature.h"
#include "led_rgb_ll509rvb.h"
#include <stdio.h>
#include <stdlib.h>
#include <iostream>


using namespace std;

int main ()
{
	

	/*
	//
	//test led
	Led* led_rgb = new Led(27, 23, 22);
	//bleu
	led_rgb->setCouleur(1,0,0);
	sleep(1);
	//rouge
	led_rgb->setCouleur(0,1,0);
	sleep(1);
	//vert
	led_rgb->setCouleur(0,0,1);
	sleep(1);
	led_rgb->setCouleur(0,0,0);
	
	while(1){
		led_rgb->blinkLed(23,2);
	}
	*/
	float bar;
	float temp;
	float stuff;
	//
	//obj capteur de température 3v
	Ads1115* ads = new Ads1115(0x49);
	Temperature* vtemp = new Temperature();
	//
	//obj capteur pression huile 0-12 bar 5v
	Ads1115* adsp = new Ads1115(0x49);
	Pression* vbar = new Pression();
	int pga = 0;
	int pgap = 0;

	while(1)
	{
		//
		//capteur de température
		cout<<"La tension lue est "<< adsp->getVoltage(3, pga) << endl;
		stuff = ads->getVoltage(3, pga);
		temp = vtemp->getTemperature(stuff);
		cout<<"La température est de "<< temp <<"en C°\n\n"<< endl;
		//
		//capteur pression huile
		cout<<"La valeur lue est "<< adsp->getValue(2, pga) << endl;
		stuff = adsp->getVoltage(2, pgap);
		bar = vbar->getPression(stuff);
		cout<<"La pression est de "<< bar <<" en bar \n\n"<< endl;
		sleep(1);
	}
	return 0;
}