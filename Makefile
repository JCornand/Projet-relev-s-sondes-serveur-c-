AppEmbarque:ads1115a.o ads1115.o pression.o temperature.o jaugefuel.o alternator.o serveur.o Integrer.o
	g++ -Wall ads1115a.o ads1115.o pression.o temperature.o jaugefuel.o alternator.o serveur.o Integrer.o -o AppEmbarque

ads1115a.o: ads1115a.cpp ads1115a.h
	g++ -c ads1115a.cpp -Wall

pression.o: pression.cpp pression.h
	g++ -c pression.cpp -Wall

temperature.o: temperature.cpp temperature.h
	g++ -c temperature.cpp -Wall

ads1115.o: ads1115.h ads1115.cpp
	g++ -Wall -c ads1115.cpp -o ads1115.o

jaugefuel.o: jaugeFuel.cpp jaugeFuel.h ads1115.h
	g++ -Wall -c jaugeFuel.cpp -o jaugefuel.o

alternator.o: alternator.cpp alternator.h ads1115.h
	g++ -Wall -c alternator.cpp -o alternator.o

serveur.o: serveur.h serveur.cpp
	g++ -Wall -c serveur.cpp -o serveur.o

Integrer.o: Integrer.cpp
	g++ -c Integrer.cpp -Wall

cleane:
	rm -rf .o