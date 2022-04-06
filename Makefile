#
#	Makefile 
#

DEBUG = -g
CC = qcc
LD = qcc


TARGET = -Vgcc_ntox86_64
#TARGET = -Vgcc_ntox86
#TARGET = -Vgcc_ntoarmv7le
#TARGET = -Vgcc_ntoaarch64le

CFLAGS += $(DEBUG) $(TARGET) -Wall
LDFLAGS+= $(DEBUG) $(TARGET)

ODIR=./build
DEPS = scheduler.h policy.h

_OBJ = scheduler.o priority.o round_robin.o modified_rr.o modulo_rr.o priority_rr.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

BINS = scheduler
all: $(BINS)

$(ODIR)/%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)
	
scheduler: $(OBJ)
	$(CC) -o $@ $^

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o $(BINS) *.core;


