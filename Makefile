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

goose: goose.c iniparser.c dictionary.c
	$(CC) $(CFLAGS) -o goose goose.c  iniparser.c dictionary.c

clean veryclean:
	$(RM) goose



