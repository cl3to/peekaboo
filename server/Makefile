
# Flags
CC = gcc
CFLAGS = -Wall -Werror

server: main.o orchestrator.o
	$(CC) $(CFLAGS) -o server main.o orchestrator.o

main.o: main.c
	$(CC) $(CFLAGS) -c main.c -o main.o

orchestrator.o:
	$(CC) $(CFLAGS) -c orchestrator/tcp_orchestrator.c -o orchestrator.o

# remove object files and executable when user executes "make clean"
clean:
	rm *.o server