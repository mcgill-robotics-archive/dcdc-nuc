# DCDC NUC Power Supply Linux Driver
__License:__ LGPL 2.1

Example Linux application `dcdc_nuc` for showing the IO data for mini-box.com
[DCDC-NUC](http://www.mini-box.com/DCDC-NUC) product. This project is build on
the source code of Windows library provided by the courtesy of Mini-Box.com. 


## Getting Started

### Prerequites

You will need `libusb` and `libusb-dev` to compile the code. You can get them
by running: 

```bash
sudo apt-get install libusb libusb-dev
```

You will need also patch your HID kernel module to be able to communicate with
the device. An automatics script is provided for **Ubunut 16.04**. **Note:**
A reboot is required for the change to take effect.

```bash
./setup
```

### Compiling & install

Simply run the following command:

```bash
make & make install
```

### Usage

Simply run `dcdc_nuc`, with the PSU connected through USB. The program will 
continously output all IO date from the PSU.

## Authors
* **Nicu Pavel** - *Original Author for DCDC USB* 
* **Bei Chen Liu** - *Porting to DCDC NUC and update to c++* - [Baycken](https://github.com/Baycken) 

## License
This project is licensed under LGPL 2.1 -- see the header file 
[dcdc_nuc.h](dcdc_nuc.h) for more detail.
