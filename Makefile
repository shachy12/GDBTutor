CC = gcc
all: GDBTutor

questions.o: questions.c questions_internal.h color_utils.h
	$(CC) -c questions.c -o questions.o

main.o: main.c questions_exports.h color_utils.h
	$(CC) -c main.c -o main.o 

GDBTutor: main.o questions.o
	$(CC) main.o questions.o -o GDBTutor

clean:
	rm -Rf *.o GDBTutor
