obj-m := asynccom.o
KDIR := /lib/modules/$(shell uname -r)/build
PWD	:= $(shell pwd)
IGNORE :=
asynccom-objs := src/asynccom.o

ifeq ($(DEBUG),1)
	EXTRA_CFLAGS += -DDEBUG
endif

ifeq ($(RELEASE_PREVIEW),1)
	EXTRA_CFLAGS += -DRELEASE_PREVIEW
endif

default:
	$(MAKE) -C $(KDIR) SUBDIRS=$(PWD) modules

install:
	cp asynccom.rules /etc/udev/rules.d/
	cp asynccom.ko /lib/modules/`uname -r`/kernel/drivers/char/
	echo 'asynccom' >> /etc/modules
	cp lib/raw/asynccom.h /usr/local/include/
	depmod

uninstall:
	rm /etc/udev/rules.d/asynccom.rules
	rm /lib/modules/`uname -r`/kernel/drivers/char/asynccom.ko
	sed --in-place '/asynccom/d' /etc/modules
	rm /usr/local/include/asynccom.h
	depmod

clean:
	@find . $(IGNORE) \
	\( -name '*.[oas]' -o -name '*.ko' -o -name '.*.cmd' \
		-o -name '.*.d' -o -name '.*.tmp' -o -name '*.mod.c' \
		-o -name '*.markers' -o -name '*.symvers' -o -name '*.order' \
		-o -name '*.tmp_versions' \) \
		-type f -print | xargs rm -f
	rm -rf .tmp_versions

help:
	@echo
	@echo 'Build targets:'
	@echo '  make - Build driver module'
	@echo '  make clean - Remove most generated files'
	@echo '  make install - Installs ASYNCCOM driver and C header file'
	@echo '  make uninstall - Clean up installation'
	@echo
