all: main clean

main: main.o cli.o functions.o bmpWork.o
	gcc main.o cli.o functions.o bmpWork.o

main.o: main.c
	gcc -c main.c

functions.o: functions.c
	gcc -c functions.c

cli.o: cli.c
	gcc -c cli.c

bmpWork.o: bmpWork.c
	gcc -c bmpWork.c

clean:
	rm -rf *.o