# Student's Makefile for the CS:APP Performance Lab
TEAM = 
VERSION = 
HANDINDIR = /home/catlab/csapp/perflab_handin

CC = gcc
CFLAGS = -Wall -O2 -m32
LIBS = -lm

OBJS = driver.o kernels.o fcyc.o clock.o

all: driver

driver: $(OBJS) fcyc.h clock.h defs.h config.h
	$(CC) $(CFLAGS) $(OBJS) $(LIBS) -o driver

handin:
	cp kernels.c $(HANDINDIR)/$(TEAM)-kernels.c
	chmod 600 $(HANDINDIR)/$(TEAM)-kernels.c

clean: 
	-rm -f $(OBJS) driver core *~ *.o


