obj-m := windows-v.o
PWD := $(CURDIR)

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean

load:
	-rmmod windows_v
	insmod windows-v.ko

unload:
	-rmmod windows_v