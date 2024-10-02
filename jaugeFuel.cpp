#include "jaugeFuel.h"



using namespace std;

JaugeFuel::JaugeFuel(void)
{
	can=new ads1115(0x48);
}

void JaugeFuel::select_Jauge(int nb)
{
	switch (nb) 
	{
		case 1:					//ESS2
			can->set_input(0);
			break;
		case 2: 				//ESS1
			can->set_input(1); 
			break;
	}
}	


float JaugeFuel::get_niveau()
{
	
	float gain;
	float tension;
	gain=can->set_gain(001);
	
	can->set_convtype(1);
	
	tension = can->get_voltage( (gain/2),16383 );
	// faire 0% 25% 50% 75% 100%
	/*
	if( tension > 3.20 )
	{
		niveau = 0;
	}
	
	if (( 2.80 < tension) && (tension < 3.20 ))
	{
		niveau = 25;
	}
	
	if ((2.20 < tension) && (tension < 2.40 ))
	{
		niveau = 50;
	}
	
	if ((2.00 > tension) && (tension > 1.20 ))
	{
		niveau = 75;
	}
	
	if ( tension < 1.15 )
	{
		niveau = 100;
	}
	*/

	niveau = 100*((-0.438 * tension) + 1.4348); // y=-0.438x+1.4348 equation linéaire niveau en fonction de la tension 
	printf("tension=%f\n",tension);
	cout<<"tension="<< tension << endl;  
	return(niveau);
}

JaugeFuel::~JaugeFuel()
{
	delete can;
}
