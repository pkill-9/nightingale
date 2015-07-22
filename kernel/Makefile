OBJS = start.o descriptors.o io.o output.o main.o memutils.o protect.o \
       utils.o vga.o keyboard.o ps2.o interrupts.o irqs.o
CC = gcc
AS = gcc
CFLAGS = -fno-hosted -fleading-underscore -nostdlib -Wall --std=c99

DEPEND_DIR = .depend
DEPEND_FILES = $(OBJS:%.o=$(DEPEND_DIR)/%.depend)


all:		$(DEPEND_DIR) nightingale

%.o:		%.s
	$(CC) $(CFLAGS) 	-c $< -o $@

# for each C file, build a .o file, and also a .depend file, listing the
# dependencies. There is a bit of post processing as well, to ensure that
# if a header file is changed to a different name, it will not break the
# build. To do this, each header file is listed in the depend file as
# having no dependencies.
#
# Steps:
# 1. dependency list created in a .depend file, and copy created in a temp
#    file.
# 2. Strip out anything between the start of the line and the : char with
#    the first sed command.
# 3. fmt -1 puts a line break between each word.
# 4. Second sed command puts a : at the end of each line. Output appended
#    to the depend file, and the temp file is removed.
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
	rm -f $(DEPEND_FILES)

scrub:		clean
	rm -f nightingale cscope.out
	rmdir $(DEPEND_DIR)

tags:
	cscope -b

.PHONY:		clean scrub tags all

-include $(DEPEND_FILES)

# vim: ts=8 sw=4 noet
