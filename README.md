# Pi-OS
Build your own OS from scratch for the Rapsberry Pi 3B using Assembly and C code

You can build on Linux or MAC OS. This project is recommended for people who have programmed C before and understand a little bit of Assembly. 

The system boots up in about 1-2 seconds after power on. As soon as it boots up you will see text displayed on the monitor plugged into the HDMI port.

While programming an OS from scratch is not the easiest to develop software on the speed of boot, performance of your OS, and finite control make it an excellent option if you require a fast boot and/or critical performance. It is also an excellent learning opportunity to understand how an OS works. Note that the C stdlib depends on an OS to work so you will have to write C code without it. 

The main loop of this OS will show a small white rectangle border of that display size and fill yellow squares inside that area. Once the rectangle is completely full it will start removing the yellow squares until it is completely empty. Then it will start filling the rectangle up with yellow squares again and will keep repeating just like the first cycle. 




Prerequisites: 
aarch64-elf-gcc
aarch64-elf-objcopy
python3
mkfs.fat
mcopy
mmd
dd
truncate





To build enter the build directory. Run "cmake .. -DCMAKE_TOOLCHAIN_FILE=../aarch64-baremetal.cmake"

Then run "make" to build the image named pi_os.img. Burn this image to an sd card just as you would a regular pi os image.



Sources:
Font bitmap from https://github.com/dhepper/font8x8/tree/master

Firmware from https://github.com/raspberrypi/firmware