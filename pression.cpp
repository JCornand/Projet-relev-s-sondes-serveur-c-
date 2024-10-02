#include "pression.h"
//
//constructeurs
Pression::Pression()
{

}

float Pression::getPression(float voltage)
{
	return((2.75*voltage)-0.375-0.435);//-0.435 erreur en moins environ
}
