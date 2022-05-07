obj-m := driver.o
driver-y := between.o find.o list.o size.o empty_list.o init_list.o remove.o swap.o


KDIR := /lib/modules/$(shell uname -r)/build
PWD := $(shell pwd)

default:
	$(MAKE) -C $(KDIR) M=$(PWD) modules 

clean:
	$(MAKE) -C $(KDIR) M=$(PWD) clean

