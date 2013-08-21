MACConversion
=============
Command line utility written in C++ that converts hex values to their corresponding time and date values. Little endian is assumed for this utility.
<br> Usages specifications for the utility are given below.
<br>mac_conversion -T|-D [–f filename | -h hex value ]
<br>-T Use time conversion module. Either –f or –h must be given.
<br>-D Use date conversion module. Either –f or –h must be given.
<br>-f filename
<br>This specifies the path to a filename that includes a hex value 
of time or date. Note that the hex value should follow this 
notation: 0x1234. For the multiple hex values in either a file 
or a command line input, we consider only one hex value so the 
recursive mode for MAC conversion is optional.
<br>-h hex value
<br>This specifies the hex value for converting to either date or 
time value. Note that the hex value should follow this notation: 
Ox1234. For the multiple hex values in either a file or a
