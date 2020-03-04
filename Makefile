PROJ = xremapkey
OBJS = device.o keybord.o main.o

CR = 
CC = $(CR)gcc
CFLAGS = -Wall -g

#.PHONY: clean

$(PROJ) : $(OBJS)
	$(CC) $(CFLAGS) -o $@ $?

all : $(PROJ)

clean :
	rm -f *.o core $(PROJ)
