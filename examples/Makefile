all: use_ch5md

CC=h5cc
INC=-I../src
VPATH=../src

%.o: %.c
	$(CC) $(INC) -c $<

use_ch5md: use_ch5md.o ch5md.o
	$(CC) $(INC) use_ch5md.o ch5md.o -o use_ch5md
