
# Flags
CC = gcc
CFLAGS = -Wall -Werror

# Build client code

# Build the client
client: clean main.o stub.o tui.o serializer.o
	$(CC) $(CFLAGS) -o ../bin/client main.o stub.o tui.o serializer.o

# Build the client object file
main.o: main.c
	$(CC) $(CFLAGS) -c main.c -o main.o

# Build the serializer object file
serializer.o:
	$(CC) $(CFLAGS) -c serializer/serializer.c -o serializer.o

# Build the stub object file
stub.o:
	$(CC) $(CFLAGS) -c stub/tcp_stub.c -o stub.o

# Build the tui object file
tui.o:
	$(CC) $(CFLAGS) -c ui/peekaboo_tui.c -o tui.o

# remove object files and executable when user executes "make clean"
clean:
	rm -f *.o ../bin/client