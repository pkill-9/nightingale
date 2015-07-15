OBJS = descriptors.o io.o output.o main.o memutils.o protect.o start.o \
       utils.o vga.o keyboard.o ps2.o interrupts.o irqs.o
CC = gcc
AS = gcc
CFLAGS = -fno-hosted -fleading-underscore -nostdlib -Wall --std=c99

DEPEND_DIR = .depend


all:		$(DEPEND_DIR) nightingale

%.o:		%.s
	$(CC) $(CFLAGS) 	-c $< -o $@

%.o:		%.c
	$(CC) $(CFLAGS)		-c $< -o $@
	gcc -MM $(CFLAGS) $*.c > $(DEPEND_DIR)/$*.depend
	@cp -f $(DEPEND_DIR)/$*.depend $*.depend.tmp
	@sed -e 's/.*://' -e 's/\\$$//' < $*.depend.tmp | fmt -1 | \
	    sed -e 's/^ *//' -e 's/$$/:/' >> $(DEPEND_DIR)/$*.depend
	@rm -f $*.depend.tmp

$(DEPEND_DIR):
	mkdir $(DEPEND_DIR)

nightingale:	$(OBJS)
	ld --script=link.ld -o nightingale $(OBJS)

clean:
	rm -f $(OBJS)
	rm -f $(OBJS:%.o=$(DEPEND_DIR)/%.depend)

scrub:		clean
	rm -f nightingale cscope.out
	rmdir $(DEPEND_DIR)

tags:
	cscope -b

.PHONY:		clean scrub tags all

-include $(OBJS:%.o=$(DEPEND_DIR)/%.depend)

# vim: ts=8 sw=4 noet
