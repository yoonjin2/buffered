include Kbuild
FLAGS := --warn-undefined-variables --debug
KDIR := /lib/modules/$(shell uname -r)/build
PWD := $(shell pwd)

default:
	$(MAKE) -C $(KDIR) $(FLAGS) M=$(PWD) LDDINC=$(PWD)/include modules
	/usr/src/kernels/$(shell uname -r)/scripts/sign-file sha256 buffer.priv buffer_public.der driver.ko

clean:
	$(MAKE) -C $(KDIR)  $(FLAGS) M=$(PWD) clean

