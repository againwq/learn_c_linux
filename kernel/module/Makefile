target = hello
obj-m := ${ target }.o
KERNELDIR = /lib/modules/5.15.90.1-microsoft-standard-WSL2/build
default:
	$(MAKE) -C $(KERNELDIR) M= $(CURDIR) modules
install:
	insmod $(target).ko
uninstall:
	rmmod $(target).ko
clean:
	rm -rf *.o *.mod.c *.ko
	rm -rf Modules.symvers .*cmd .tmp_versions