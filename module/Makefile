KDIR  := /lib/modules/$(shell uname -r)/build

obj-m += decrypter.o

all:
	make -C $(KDIR) M=$(shell pwd) modules
clean:
	make -C $(KDIR) M=$(shell pwd) clean
	rm -f modules.order Module.symvers
	rm -f .??*cmd
	rm -rf .tmp_versions