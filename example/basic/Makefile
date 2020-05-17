CC := gcc
W_DIR := ${shell pwd}
CFLAGS ?= -O3 -g0
MAINSRC := cmocka_test.c 
PROM = cmocka_test 
LDFLAGS = -lcmocka -lm

include cmocka_test.mk
OBJECT ?= .o

MAINOBJ = $(MAINSRC:.c=$(OBJECT))
COBJS = $(CSRCS:.c=$(OBJECT))

all:default

%.o:%.c
	$(CC) $(CFLAGS) -o $@ -c $^ -lcmocka
	@echo "gcc $<"

default: $(COBJS) $(MAINOBJ) 
	$(CC) $^ $(LDFLAGS) -o $(PROM)

.PHONY: clean
clean:
	@rm *.o $(PROM)

