#include "serveur.h"
#include <errno.h>  

Serveur::Serveur()
{
}

void Serveur::open()
{
	//Ouverture socket, bind, listen, accept 
											//
	struct sockaddr_in serverAddress;		//Structure qui contient la configuration du Serveur et du client									//Taille de la structure struct sockaddr_in
	
	debut:	
		listenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		if(listenSocket == -1)		   	  								//si =-1 il y a une erreur
		{
			cout << "Error : Problème d'ouverture de la socket" << endl;
			//printf("Error : Problème d'ouverture de la socket\n\n");
			cout << "Fermeture du Serveur terminé" << endl;
			//printf("Fermeture du Serveur terminé\n\n");
			close(listenSocket);
			goto debut;	//car erreur		
		}
			
		else
		{
			cout << "Succes : Socket ouverte" << endl;
			//printf("Succes : Socket ouverte\n\n");		
		}			

		//
		// Configuration de l'adresse physique du Serveur et de son port
		serverAddress.sin_addr.s_addr = htonl(INADDR_ANY);			//IP du Serveur
		serverAddress.sin_port = htons(1500);						//Le port de connexion sur le Serveur
		serverAddress.sin_family = AF_INET;							//Protocole IPv4

		//
		//Lien de la socket à l’adresse du Serveur définie dans la structure "serverAddress" grâce au socket "socketDescriptor"
		if(bind(listenSocket, (struct sockaddr *) &serverAddress,sizeof(serverAddress)) == -1)		 //si =-1 il y a une erreur
		{
		 	cout<<"Error : Problème lors du lien de la socket à l'adresse du Serveur"<<endl;
		 	//printf("Error : Problème lors du lien de la socket à l'adresse du Serveur\n\n");
			close(listenSocket);
			cout<<"Fermeture de la socket Serveur\n\nFermeture du Serveur terminé\n\n"<<endl;
			//printf("Fermeture de la socket Serveur\n\nFermeture du Serveur terminé\n\n");
			goto debut;	//car erreur
		}
		
		else
		{
			cout<<"Succes : Lien de la socket et de l'adresse du Serveur établie"<<endl;
			//printf("Succes : Lien de la socket et de l'adresse du Serveur établie\n\n");
		}	
		//
		//Définie la socket qui sera utilisée pour accepter les demandes de connexions entrantes
		if((listen(listenSocket, 5)) == -1)
		{
			cout<<"Error : Problème lors de la phase de la mise en mode passif de la socket"<<endl;
			//printf("Error : Problème lors de la phase de la mise en mode passif de la socket\n\n");
			close(listenSocket);
			cout<<"Fermeture de la socket Serveur\n\nFermeture du Serveur terminé"<<endl;
			//printf("Fermeture de la socket Serveur\n\nFermeture du Serveur terminé\n\n");
			goto debut;	//car erreur
		}
		else
		{
			cout<<"Succes : Serveur en écoute sur le port 1500 avec le protocole TCP"<<endl;
			//printf("Succes : Serveur en écoute sur le port 1500 avec le protocole TCP\n\n");
		}
}
	




void Serveur::accepte()
{
	struct sockaddr_in serverAddress, clientAddress;				//Structure qui contient la configuration du Serveur et du client
	socklen_t clientAddressLength;									//Taille de la structure struct sockaddr_in

		debut:
			//
			// Accepte une connexion de la file d’attente
			clientAddressLength = sizeof(serverAddress);		//Affectation de la taille de la structure struct sockaddr_in serverAddress à la variable contenant la taille de la structure struct sockaddr_in clientAddress
			connectSocket = accept(listenSocket, (struct sockaddr *) &clientAddress, &clientAddressLength);
			if(connectSocket == -1)		//si =-1 il y a une erreur
			{
				printf("Error : Problème lors de la connexion du client au Serveur\n\n");
				close(listenSocket);
				printf("Fermeture de la socket Serveur\n\nFermeture du Serveur terminé\n\n");
				goto debut;	//car erreur
			}
			
			else
			{
				cout << "connexion acceptée\n" << endl;
			}
}



int Serveur::testreq()
{
	int  msgSend, msgLength, testreq;
	char msgErreur[] = "Requête invalide !";

	//Reception du message envoyé par le client
	reception:
		 msgLength = recv(connectSocket, msg, TAILLE_MAX_MESSAGE, 0);
		 if(msgLength == -1)
		 {
		 	cout<<"Error message non reçu"<< endl; 
			//goto reception;
		 }

		 if(msgLength == 0)
		 {
		 	cout<<"req non reçu\n"<<endl;
		 	//goto reception;
		 }

	     if (msgLength != 0) 
	     {
			cout<<"Client reçu:"<< msg <<endl; //printf("Client recu: %s\n\n", msg);
	     }


		 //
		 // Comparer la requête envoyée par le client et celle qu'est censé recevoir le client
	     testreq = strcmp(msg, "req");
	     cout<< "testreq="<< dec << testreq << endl ; //printf("testreq =%d\n\n", testreq);

	     if(testreq != 0)
		 {
			//
			//Envoie d'un message par le Serveur
			msgSend = send(connectSocket, msgErreur, strlen(msgErreur), 0);
			if(msgSend == -1)
			{
				cout<<"Error : Message Erreur non envoyé\n\n"<<endl ; 
			}
			//goto reception;
		 }		
	return(testreq);
}

void Serveur::set_data(float essence, float essence2, float Valt1, float Valt2, float pression1, float temp1, float pression2, float temp2) /*, float Valt, float batterie*/
{
	int i,Ldata;

	float Regm1,Regm2;
	Regm1=2000;
	Regm2=3000;


	float fNivEssence1,fNivEssence2,fTensionAlt1, fTensionAlt2;
	fNivEssence1=essence;
	fNivEssence2=essence2;
	fTensionAlt1 = Valt1;
	fTensionAlt2 = Valt2;

	Ldata=strlen(data);
	for(i = 0; i < Ldata; i++)
 	{
		data[i] = 0;
	} 

	sprintf(valRegm1,"%f",Regm1);
	sprintf(valRegm2,"%f",Regm2);

	sprintf(valTemperature1, "%f",temp1);
	sprintf(valPressionHuile1,"%f",pression1);

	sprintf(valTemperature2, "%f",temp2);
	sprintf(valPressionHuile2,"%f",pression2);

	sprintf(valTensionAlt1,"%f",fTensionAlt1);
	sprintf(valTensionAlt2,"%f",fTensionAlt2);
	sprintf(valNivEssence1, "%f", fNivEssence1);
	sprintf(valNivEssence2, "%f\n", fNivEssence2);

	
	//changer les balises au dessus en fonction de théo
	strcat(data,"<Reg1>");
	strcat(data,valRegm1);
	strcat(data,"<Reg2>");
	strcat(data,valRegm2);
	strcat(data,"<TempEau1>"); 
	strcat(data, valTemperature1);
	strcat(data,"<TempEau2>"); 
	strcat(data, valTemperature2);
	strcat(data,"<PressionHuile1>");  
	strcat(data, valPressionHuile1);
	strcat(data,"<PressionHuile2>"); 
	strcat(data, valPressionHuile2);

	strcat(data,"<Alt1>");
	strcat(data, valTensionAlt1);
	strcat(data,"<Alt2>");
	strcat(data, valTensionAlt2);
	strcat(data, "<NivEssence1>");
	strcat(data, valNivEssence1);
	strcat(data, "<NivEssence2>");
	strcat(data, valNivEssence2);
	printf("data = %s\n",data);
	
}

void Serveur::send_data()
{
	int msgSend;
	//
	//Envoie d'un message par le Serveur
	EnvoiMsg:
		msgSend = send(connectSocket, data, strlen(data), 0);
		if(msgSend == -1)
		{
			perror("");
			printf("Error : Message non envoyé\n\n");
			goto EnvoiMsg;
		}
		else 
		{
			printf("message envoyé\n\n");
		}
}