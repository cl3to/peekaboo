
# Flags
CC = gcc
CFLAGS = -Wall -Werror -lsqlite3 -lcrypto

server: clean main.o orchestrator.o general_serializer.o user_serializer.o admin_serializer.o database.o profile.o profile_to_json.o cjson.o
	$(CC) -o ../bin/server main.o orchestrator.o general_serializer.o cjson.o user_serializer.o admin_serializer.o database.o profile.o profile_to_json.o $(CFLAGS)

main.o: main.c
	$(CC) -c main.c -o main.o

orchestrator.o:
	$(CC) -c orchestrator/tcp_orchestrator.c -o orchestrator.o 

general_serializer.o: cjson.o
	$(CC) -c serializers/general_serializer.c -o general_serializer.o 

user_serializer.o: 
	$(CC) -c serializers/user_serializer.c -o user_serializer.o

admin_serializer.o: 
	$(CC) -c serializers/admin_serializer.c -o admin_serializer.o

profile_to_json.o:
	$(CC) -c ../utils/profile_to_json.c -o profile_to_json.o 

database.o:
	$(CC) -c database/database.c -o database.o

profile.o:
	$(CC) -c ../model/profile.c -o profile.o

cjson.o:
	$(CC) -c ../lib/cJSON.c -o cjson.o

# remove object files and executable when user executes "make clean"
clean:
	rm -f *.o ../bin/server