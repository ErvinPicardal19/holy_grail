CC:= gcc
TARGET:= main
all: main

%.o : %.c 
	$(CC) -c $^ -o $@

main : main.o
	$(CC) -o $@ $^

clean :
	rm -rf *.o $(TARGET)
