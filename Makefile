CC = gcc -Wall -c
OBJS = queue.o heap.o process.o

main.out : $(OBJS)
	gcc main.c -Wall -o main.out $(OBJS)

queue.o : queue.c queue.h
	$(CC) queue.c
	
heap.o : heap.c heap.h
	$(CC) heap.c

process.o: process.c process.h
	$(CC) process.c

clean:
	rm *.o main.out