obj-m := main.o
obj-m += between.o     
obj-m += find.o
obj-m += list.o 
obj-m += remove.o
obj-m += sort.o
obj-m += empty_list.o 
obj-m += init_list.o 
obj-m += size.o 
obj-m += swap.o


KDIR := /lib/modules/$(shell uname -r)/build
PWD := $(shell pwd)

default:
	$(MAKE) -C $(KDIR) M=$(PWD) modules

clean:
	$(MAKE) -C $(KDIR) M=$(PWD) clean

