elenco: elenco.o 
	g++ elenco.o -o elenco

elenco.o: elenco.cpp
	g++ -c elenco.cpp

clean: 
	rm *.o elenco