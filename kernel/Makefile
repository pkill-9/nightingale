SRC = descriptors.c output.c protect.c utils.c vga.c main.c keyboard.c \
      ps2.c
OBJS = descriptors.o io.o output.o main.o memutils.o protect.o start.o \
       utils.o vga.o keyboard.o ps2.o irqs.o
CC = gcc
AS = gcc
CFLAGS = -fno-hosted -fleading-underscore -nostdlib -Wall --std=c99


all:		nightingale

%.o:		%.s
	$(CC) $(CFLAGS) -c $< -o $@

nightingale:	depend $(OBJS)
	ld --script=link.ld -o nightingale $(OBJS)

depend:		$(SRC)
	gcc -MM $(SRC) > depend

clean:
	rm -f $(OBJS)

scrub:		clean
	rm -f nightingale cscope.out depend

tags:
	cscope -b

.PHONY:		clean scrub tags all

include depend

# vim: ts=8 sw=4 noet
