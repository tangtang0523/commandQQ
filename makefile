clientcfile = client.c get_time.c common.c
clienthfile = client.h get_time.h common.h
servercfile = server.c get_time.c common.c
serverhfile = server.h get_time.h common.h

TARGET_SERVER = server
TARGET_CLIENT = client

CFLAG = -Wall -g

$(TARGET_SERVER):$(servercfile) 
	$(CC) $(CFLAG) $(servercfile) -o $@
$(TARGET_CLIENT):$(clientcfile) 
	$(CC) $(CFLAG) $(clientcfile) -o $@

all: $(TARGET_SERVER) $(TARGET_CLIENT)
clean: 
	$(RM) *.o
	$(RM) $(TARGET_SERVER) $(TARGET_CLIENT)
.PHONY: clean

ser:
	./server 7838 1
.PHONY: ser
cli:
	./client 127.0.0.1 7838
.PHONY: cli
