#
# iniparser tests Makefile
#

CC      ?= gcc
CFLAGS  += -g -I.
LFLAGS  += -L.. -liniparser
AR	    ?= ar
ARFLAGS += rcv
RM      ?= rm -f


default: all

all: goose 

goose: goose.c
	$(CC) $(CFLAGS) -o goose goose.c  -L. -liniparser

clean veryclean:
	$(RM) goose



