PACKAGES=libusb
LIBS=`pkg-config --libs ${PACKAGES}` -lm
INCS=`pkg-config --cflags ${PACKAGES}`
CFLAGS=-Wall 
CC=g++
LIBSRC=
MAINSRC=dcdc_nuc.cpp
OBJS=dcdc_nuc

all: dcdc_nuc
install: all
	install -v -m755 dcdc-nuc /usr/local/bin/

$(OBJS): dcdc_nuc.h
.cpp.o:
	$(CC) $(CFLAGS) $(INCS) -c $(LIBSRC) $(MAINSRC)

dcdc_nuc: $(MAINSRC)
	$(CC) $(CFLAGS) $(MAINSRC) $(LIBS) -L. -o dcdc_nuc

clean:
	rm -rf *.o *.so dcdc-nuc
