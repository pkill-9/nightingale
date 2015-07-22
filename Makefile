SUBDIRS = kernel
GRUB_INSTALL_FLAGS = --boot-directory=./vfs/boot --no-floppy \
		     --modules="normal part_msdos ext2 multiboot"


all:		$(SUBDIRS)

$(SUBDIRS):
	$(MAKE) -C $@ $(MAKECMDGOALS)

# create and format a disk image, and set it up so that we can treat it
# as if it is an ordinary block device.
format-disk:	disk.hdd loop-devices
	mke2fs /dev/loop1

mount-disk:	vfs
	mount /dev/loop1 ./vfs

install-grub:	
	grub-install $(GRUB_INSTALL_FLAGS) /dev/loop0

umount:
	umount ./vfs
	losetup -d /dev/loop0 /dev/loop1

disk.hdd:
	dd if=/dev/zero of=disk.hdd bs=1 count=0 seek=1GB
	fdisk disk.hdd

loop-devices:
	losetup /dev/loop0 ./disk.hdd
	losetup /dev/loop1 ./disk.hdd -o 1048576

vfs:
	mkdir ./vfs

tags:
	cscope -b

clean:		$(SUBDIRS) umount

scrub:		$(SUBDIRS) clean
	rm -f disk.hdd
	rm -rf vfs

.PHONY:		clean scrub all $(SUBDIRS) format-disk mount-disk umount\
    install-grub loop-devices

# vim: ts=8 sw=4 noet
