CC = gcc

CFLAGS = -c -O3  -I/home/$(USER)/local/include/ -I/usr/include/ -I.

CFLAGSDEBUG = -g -Wall -c -I/home/$(USER)/local/include/ -I/usr/include/ 

LFLAGS = -lm -L/home/$(USER)/local/lib -Wl,-R /home/$(USER)/local/lib 


distancia:distancia.h
	echo Estoy compilando $@.c
	$(CC) $(CFLAGS) $@.c -o $@.o

simpson:simpson.h
	echo Estoy compilando $@.c
	$(CC) $(CFLAGS) $@.c -o $@.o

interpolador:interpolador.h
	echo Estoy compilando $@.c
	$(CC) $(CFLAGS) $@.c -o $@.o

decae_densidad:decae_densidad.h
	echo Estoy compilando $@.c
	$(CC) $(CFLAGS) $@.c -o $@.o

programa_main:
	echo Estoy compilando $@.c
	$(CC) $(CFLAGS) $@.c -o $@.o
	$(CC) distancia.o interpolador.o simpson.o decae_densidad.o $@.o $(LFLAGS) -lm -lgsl -lgslcblas -o  $@.x		

debug: 
	echo Estoy compilando para debugear $@.c
	$(CC) $(CFLAGSDEBUG) $@.c -o $@.o
	$(CC) $@.o $(LFLAGS)  -o $@.x


clean:
	rm -rf *.~
	rm -rf *.out
	rm -rf *#
	rm -rf *.o
	rm -rf *.a
	rm -rf *.so
	rm *.x
