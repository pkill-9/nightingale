SRC = descriptors.c output.c protect.c utils.c vga.c main.c
OBJS = descriptors.o io.o output.o protect.o start.o utils.o vga.o main.c
CC = gcc
AS = as
CFLAGS = -fno-hosted -fleading-underscore -nostdlib -Wall --std=c99


all:		nightingale

%.o:		%.s
	$(AS) $< -o $@

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
