TARGETS = control_station soldier

obj-m += io_driver_dr_doom.o

SOURCES = $(TARGETS:=.c)

all: $(TARGETS)
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

$(TARGETS): $(SOURCES)
	$(CC) $(CFLAGS) -o $@ $@.c

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
	rm -f $(TARGETS)