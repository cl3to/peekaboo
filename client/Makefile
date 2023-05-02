# Flags
CC = gcc
CFLAGS = -Wall -Werror

# Variables
OBJDIR = ../obj/client
BINDIR = ../bin
CLIENT_OBJTS = $(OBJDIR)/main.o $(OBJDIR)/stub.o $(OBJDIR)/tui.o \
               $(OBJDIR)/serializer.o $(OBJDIR)/cjson.o $(OBJDIR)/email_validator.o

# Build client code

# Build the client
client: $(CLIENT_OBJTS)
	$(CC) $(CFLAGS) -o ../bin/client $(CLIENT_OBJTS)

# Build the client object file
$(OBJDIR)/main.o: main.c
	$(CC) $(CFLAGS) -c main.c -o $@

# Build the serializer object file
$(OBJDIR)/serializer.o:
	$(CC) $(CFLAGS) -c serializer/serializer.c -o $@

# Build the stub object file
$(OBJDIR)/stub.o:
	$(CC) $(CFLAGS) -c stub/tcp_stub.c -o $@

# Build the tui object file
$(OBJDIR)/tui.o:
	$(CC) $(CFLAGS) -c ui/peekaboo_tui.c -o $@

$(OBJDIR)/cjson.o:
	$(CC) -c ../lib/cJSON/cJSON.c -o $@

$(OBJDIR)/email_validator.o:
	$(CC) -c ../utils/email_validator.c -o $@

# remove object files and executable when user executes "make clean"
clean:
	rm -f $(OBJDIR)/*.o $(BINDIR)/client