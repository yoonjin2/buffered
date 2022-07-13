include Kbuild
FLAGS := --warn-undefined-variables --debug
KDIR := /lib/modules/$(shell uname -r)/build
PWD := $(shell pwd)

default:
	$(MAKE) -C $(KDIR) $(FLAGS) M=$(PWD) 

clean:
	$(MAKE) -C $(KDIR)  $(FLAGS) M=$(PWD) clean

