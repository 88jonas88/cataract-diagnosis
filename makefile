PROG = catarata
CC = gcc
CFLAGS = -Wall -ansi -pedantic -std=c11 -g
OBJS = catarata.o PPM.o

$(PROG): $(OBJS)
	$(CC) -o $(PROG) $(OBJS) -lm

catarata.o: catarata.c funcoes.h structs.h
	$(CC) $(CFLAGS) -c catarata.c

PPM.o: PPM.c
	$(CC) $(CFLAGS) -c PPM.c

clean:
	rm -f core $(PROG) $(OBJS) saida.ppm diagnostico.txt

TrunPPM:
	./catarata -i cata.ppm -f ppm -o diagnostico.txt