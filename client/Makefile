
# Flags
CC = gcc
CFLAGS = -Wall -Werror


# Build client code

# Build the client
client: main.o stub.o
	$(CC) $(CFLAGS) -o client main.o stub.o

# Build the client object file
main.o: main.c
	$(CC) $(CFLAGS) -c main.c -o main.o

# Build the stub object file
stub.o:
	$(CC) $(CFLAGS) -c stub/tcp_stub.c -o stub.o

# remove object files and executable when user executes "make clean"
clean:
	rm *.o client