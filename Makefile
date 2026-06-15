CC = gcc
CFLAGS = -Wall -g -pthread

all: chat

chat: chat.c
	$(CC) $(CFLAGS) chat.c -o chat

clean:
	rm -f chat fifo1 fifo2
