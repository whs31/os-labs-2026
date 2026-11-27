obj-m := whs31_chardev.o
KDIR ?= /lib/modules/$(shell uname -r)/build

all: module test

module:
	$(MAKE) -C $(KDIR) M=$(PWD) modules

test: test.c whs31_chardev.h
	$(CC) -static -O2 -Wall -o test test.c

clean:
	$(MAKE) -C $(KDIR) M=$(PWD) clean
	rm -f test
