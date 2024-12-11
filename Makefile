#Makefile for Assignment 8

PROG = Log Server Client
CC = gcc

build: $(PROG)

Log: log_s.c
	$(CC) -o Log log_s.c 

Server: echo_s.c
	$(CC) -o Server echo_s.c

Client: echo_c.c
	$(CC) -o Client echo_c.c


run_echo_s: $(PROG)
	./Server

run_echo_c: $(PROG)
	./Client

run_log_s: $(PROG)
	./Log






clean:
	rm -f $(PROG) $(OBJS) 
