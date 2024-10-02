#include "alternator.h"

using namespace std; 

Alternator::Alternator(void)
{
	can=new ads1115(0x48);
}

void Alternator::select_Pin(int i)
{
	if ((i=1))
	{
		can->set_input(2);
	}
	else if ((i=2))
	{
		can->set_input(3);
	}
}

int Alternator::get_tension()
{
	float gain; 

	gain = can->set_gain(001);
	can->set_convtype(1);

	tension = can->get_voltage((gain/2),16383);

	//return(tension);
	return(tension*4.5454);
	return(tension*5);
}

Alternator::~Alternator()
{
	delete can;
}