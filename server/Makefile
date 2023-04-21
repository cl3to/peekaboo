
# Flags
CC = gcc
CFLAGS = -lsqlite3 #-Wall -Werror

server: clean main.o orchestrator.o serializer.o profile.o profile_to_json.o
	$(CC) -o ../bin/server main.o orchestrator.o  serializer.o profile.o profile_to_json.o $(CFLAGS)

main.o: main.c
	$(CC) -c main.c -o main.o

orchestrator.o:
	$(CC) -c orchestrator/tcp_orchestrator.c -o orchestrator.o 

serializer.o: ../lib/jsmn.h
	$(CC) -c serializers/user_serializer.c -o serializer.o

profile_to_json.o:
	$(CC) -c ../utils/profile_to_json.c -o profile_to_json.o 

profile.o:
	$(CC) -c model/profile.c -o profile.o



# remove object files and executable when user executes "make clean"
clean:
	rm -f *.o ../bin/server