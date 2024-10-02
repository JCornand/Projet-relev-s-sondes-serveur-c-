#include "ads1115.h"
#include "jaugeFuel.h"
#include "serveur.h"
#include "alternator.h"

int main()
{


	float nivEss1;
	float nivEss2;
	
	float Valt1;
	float Valt2;

	int req;
	pid_t pid;

	Serveur serveur;
	Alternator alt1;
	Alternator alt2;
	JaugeFuel ess1;
	JaugeFuel ess2;

	serveur.open();

	while(1)
	{
		
		serveur.accepte();
		pid = fork();

		switch(pid)
		{
			case -1:	//cas d'erreur du fiston
				
				cout << "Erreur de création du nouveau processus\n" << endl;
			break;

			case 0 : 

				do
				{
					req=serveur.testreq();
					if(req==0)
					{
						ess1.select_Jauge(2);
						cout << "========================\n" << endl;
						nivEss1 = ess1.get_niveau();
						cout << "\t" << "Niveau ess1 ="<< dec << nivEss1<<"%"<< endl;	
						ess2.select_Jauge(1);
						nivEss2 = ess2.get_niveau();
						cout << "\t" << "Niveau ess2 ="<< dec << nivEss2<<"%"<< endl; 
						
						alt1.select_Pin(1);
						Valt1 = alt1.get_tension();

						alt2.select_Pin(2);
						Valt2 = alt2.get_tension();

						serveur.set_data(nivEss1,nivEss2,Valt1,Valt2);
						serveur.send_data();
						sleep(2);
					}	
				}
				while(req==0);
			break;
		}
	}

	exit(1);
}