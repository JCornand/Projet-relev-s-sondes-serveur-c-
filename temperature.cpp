#include "temperature.h"
#include "ads1115a.h"
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <math.h>
//
//constructeurs
Temperature::Temperature()
{

}

float Temperature::getTemperature(float voltage)
{
	float R25=47;//kohm
	float T25=298.15;//K
	float B=4052;//K
	float RL=33;
	float R=22;

	float Req=(5-voltage)*R/(voltage);
	float Rctn=(Req*RL)/(RL-Req);//1/Rctn

	float X=log(Rctn/R25)/B;//X==B(1/T)1/T25)
	float x25=(1/T25);
	float x=x25+X;
	float T=1/x;
	float temp=T-273.15;
	printf("Le petit x vaut:%f\n",x);
	return(temp);
}