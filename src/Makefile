.PHPONY:server_start_app
INCLUDE_PATH=../include/
LIBRARY_PATH=../library/
INSTALL_PATH=/usr/bin/
CC=gcc
CFLAGS=-I$(INCLUDE_PATH) -g -c -Wall
CPPFLAGS=
LDFLAGS=-lpthread
TARGET=server_start_app
OLD_FILE=$(wildcard *.c)
NEW_FILE=$(patsubst %.c,%.o,$(OLD_FILE))

$(TARGET):$(NEW_FILE)
	$(CC) $^ $(LDFLAGS) -o $@

%.o:%.c
	$(CC) $(CFLAGS) $<

clean:
	rm $(NEW_FILE) -rf
	rm $(TARGET) -rf

