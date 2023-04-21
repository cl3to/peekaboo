
# Flags
CC = gcc
CFLAGS = -Wall -Werror

server: clean main.o orchestrator.o serializer.o profile.o
	$(CC) -o ../bin/server main.o orchestrator.o  serializer.o profile.o $(CFLAGS)

main.o: main.c
	$(CC) $(CFLAGS) -c main.c -o main.o

orchestrator.o:
	$(CC) -c orchestrator/tcp_orchestrator.c -o orchestrator.o 

serializer.o:
	$(CC) -c serializers/user_serializer.c -o serializer.o

profile.o:
	$(CC) -c model/profile.c -o profile.o



# remove object files and executable when user executes "make clean"
clean:
	rm -f *.o ../bin/server