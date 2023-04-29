
# Flags
CC = gcc
CFLAGS = -Wall -Werror -lsqlite3

server: clean main.o orchestrator.o general_serializer.o user_serializer.o admin_serializer.o database.o profile.o user_message.o cjson.o email_validator.o sha1.o
	$(CC) -o ../bin/server main.o orchestrator.o general_serializer.o cjson.o user_serializer.o admin_serializer.o database.o profile.o user_message.o email_validator.o sha1.o $(CFLAGS)

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

user_message.o:
	$(CC) -c ../utils/user_message.c -o user_message.o 

database.o:
	$(CC) -c database/database.c -o database.o

profile.o:
	$(CC) -c ../model/profile.c -o profile.o

email_validator.o:
	$(CC) -c ../utils/email_validator.c -o email_validator.o 

cjson.o:
	$(CC) -c ../lib/cJSON.c -o cjson.o

sha1.o:
	$(CC) -c ../lib/sha1-c/sha1.c -o sha1.o

# remove object files and executable when user executes "make clean"
clean:
	rm -f *.o ../bin/server