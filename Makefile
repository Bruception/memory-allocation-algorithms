CC = gcc -Wall -c
OBJS = queue.o heap.o process.o algorithms.o

main.out : $(OBJS) main.c
	gcc main.c -Wall -o main.out $(OBJS)

algorithms.o : algorithms.c algorithms.h
	$(CC) algorithms.c

queue.o : queue.c queue.h
	$(CC) queue.c
	
heap.o : heap.c heap.h
	$(CC) heap.c

process.o: process.c process.h
	$(CC) process.c

clean:
	rm *.o main.out
