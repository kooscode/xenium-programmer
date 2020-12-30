# Xilinx CPLD JTAG Programming via XC3SPROG

![boot-partition](../images/xenium-os.jpg)

With the XC3SPROG open source tool you can program most Xilinx CPLD's with any Firmware/Bitstream  (JED file) of your choice.

XC3SPROG is what allows you to program the Xenium Flash firmware into OpenXenium CPLD chip. The Xenium Flash firmware is in turn used to load the XeniumOS into the OpenXenium's Flash memorty chip.

And of course, you will also use this tool to load the official OpenXenium Firmware onto the OpenXenium CPLD chip to finally allow you to run the XeniumOS on your XBOX.

## Installing XC3SPROG

- The main `install` script will automatically download, build and install XC3SPROG for you. (Please be patient, this might take a bit of time)

- Programming the OpenXenium CPLD is done by using the `xenium-programmer` script.
