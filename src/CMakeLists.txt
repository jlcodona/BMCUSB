####################################################
# Test Programs
####################################################

# playBMC: This is the experimental program for trying out commands, etc.
ADD_EXECUTABLE(playBMC playBMC.c)
target_link_libraries(playBMC usb)

ADD_EXECUTABLE(playMorph playMorph.c)
target_link_libraries(playMorph bmcusb usb)

ADD_EXECUTABLE(testbmcusb testbmcusb.c)
target_link_libraries(testbmcusb bmcusb usb)

# Shared libraries for controlling the BMC USB DM
#build a shared library
ADD_LIBRARY(bmcusb SHARED bmcusb.c bmcusb.h bmcusb_defs.h BMC_Mappings.h CIUsbLib.h CIUsbShared.h )

# C++ shared library
#ADD_LIBRARY(bmcusboo SHARED bmcusboo.cpp)

#add an install target here
#INSTALL_FILES(...)
#INSTALL_PROGRAMS(...)
#INSTALL_TARGET(...)

