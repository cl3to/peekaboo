
# Flags
CC = gcc
CFLAGS = -Wall -Werror -lsqlite3
OBJDIR = ../obj
BINDIR = ../bin

server: clean $(OBJDIR)/main.o $(OBJDIR)/orchestrator.o $(OBJDIR)/general_serializer.o $(OBJDIR)/user_serializer.o $(OBJDIR)/admin_serializer.o $(OBJDIR)/database.o $(OBJDIR)/profile.o $(OBJDIR)/user_message.o $(OBJDIR)/cjson.o $(OBJDIR)/email_validator.o $(OBJDIR)/sha1.o
	$(CC) -o $(BINDIR)/server $(OBJDIR)/main.o $(OBJDIR)/orchestrator.o $(OBJDIR)/general_serializer.o $(OBJDIR)/cjson.o $(OBJDIR)/user_serializer.o $(OBJDIR)/admin_serializer.o $(OBJDIR)/database.o $(OBJDIR)/profile.o $(OBJDIR)/user_message.o $(OBJDIR)/email_validator.o $(OBJDIR)/sha1.o $(CFLAGS)

$(OBJDIR)/main.o: main.c
	$(CC) -c main.c -o $(OBJDIR)/main.o

$(OBJDIR)/orchestrator.o:
	$(CC) -c orchestrator/tcp_orchestrator.c -o $(OBJDIR)/orchestrator.o 

$(OBJDIR)/general_serializer.o: $(OBJDIR)/cjson.o
	$(CC) -c serializers/general_serializer.c -o $(OBJDIR)/general_serializer.o 

$(OBJDIR)/user_serializer.o: 
	$(CC) -c serializers/user_serializer.c -o $(OBJDIR)/user_serializer.o

$(OBJDIR)/admin_serializer.o: 
	$(CC) -c serializers/admin_serializer.c -o $(OBJDIR)/admin_serializer.o

$(OBJDIR)/user_message.o:
	$(CC) -c ../utils/user_message.c -o $(OBJDIR)/user_message.o 

$(OBJDIR)/database.o:
	$(CC) -c database/database.c -o $(OBJDIR)/database.o

$(OBJDIR)/profile.o:
	$(CC) -c ../model/profile.c -o $(OBJDIR)/profile.o

$(OBJDIR)/email_validator.o:
	$(CC) -c ../utils/email_validator.c -o $(OBJDIR)/email_validator.o 

$(OBJDIR)/cjson.o:
	$(CC) -c ../lib/cJSON.c -o $(OBJDIR)/cjson.o

$(OBJDIR)/sha1.o:
	$(CC) -c ../lib/sha1-c/sha1.c -o $(OBJDIR)/sha1.o

# remove object files and executable when user executes "make clean"
clean:
	rm -f $(OBJDIR)/*.o $(BINDIR)/server