EXEC = server

default: main.o
	gcc -o $(EXEC) main.o

main.o: main.c 
	gcc -c main.c

clean:
	rm -rf main *.o
