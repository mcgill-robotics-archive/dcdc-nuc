PACKAGES=libusb
LIBS=`pkg-config --libs ${PACKAGES}` -lm
INCS=`pkg-config --cflags ${PACKAGES}`
CFLAGS=-Wall 

LIBSRC=dcdc-nuc-comm.c dcdc-nuc-proto.c dcdc-nuc-parse.c
MAINSRC=main.c
OBJS=main.o

all: libdcdc-nuc.so dcdc-nuc
install: all
	install -v -m755 dcdc-nuc /usr/local/bin/
	install -v -m755  libdcdc-nuc.so  /usr/local/lib
	ldconfig

$(OBJS): dcdc-nuc.h
.c.o:
	$(CC) $(CFLAGS) $(INCS) -c $(LIBSRC) $(MAINSRC)

libdcdc-nuc.so: $(LIBSRC)
	$(CC) $(CFLAGS) -shared -fPIC -L. $(LIBSRC) -Wl,-soname,$@ -o $@ 

dcdc-nuc: $(MAINSRC)
	$(CC) $(CFLAGS) $(MAINSRC) -L. -ldcdc-nuc -o $@ $(LIBS)

clean:
	rm -rf *.o *.so dcdc-nuc
