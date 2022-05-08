obj-m += driver.o
driver-y := between.o find.o list.o remove.o empty_list.o init_list.o size.o swap.o


KDIR := /lib/modules/$(shell uname -r)/build
PWD := $(shell pwd)

default:
	$(MAKE) -C $(KDIR) M=$(PWD) modules

clean:
	$(MAKE) -C $(KDIR) M=$(PWD) clean

