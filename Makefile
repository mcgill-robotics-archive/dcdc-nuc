PACKAGES=libusb
LIBS=`pkg-config --libs ${PACKAGES}` -lm
CFLAGS=-Wall 
CC=g++
MAINSRC=dcdc_nuc.cpp
OBJS=dcdc_nuc

all: dcdc_nuc

install: all 
	sudo install -v -m755 dcdc_nuc /usr/bin/

.cpp.o:
	$(CC) $(CFLAGS) $(MAINSRC)

$(OBJS): $(MAINSRC)
	$(CC) $(CFLAGS) $(MAINSRC) $(LIBS) -L. -o $(OBJS)

clean:
	rm -rf *.o *.so dcdc_nuc
