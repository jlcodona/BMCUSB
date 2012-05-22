// This is a simple test program to exercise the libbmcusb API.
// JLCodona 20110301

#include <usb.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "bmcusb.h"

int main(int argc, char **argv)
{
    int Nbmc = bmcusb_probe();
    //bmcusb_setDebug(TRUE);

    printf("Found %d BMC Multi-Drivers.\n",Nbmc);

    if (Nbmc==0) exit(-1);

    int DM = 0;

    if (bmcusb_claim(DM)) {
        printf("FAILED to claim interface.\n");
        exit(1);
    } else printf("CLAIMED the USB interface to the BMC DM driver.\n");

    printf("Firmware version: %s\n",bmcusb_getFirmwareVersion(DM));
    bmcusb_reset(DM);
    //bmcusb_printStatus(DM);
    bmcusb_setHV(DM,1);
    //bmcusb_printStatus(DM);

    int nlevel;
    int nloop;
    long updates = 0;

    //u_int16_t data[USB_NUM_ACTUATORS_MULTI];

    for (nloop=0;nloop<1;nloop++) {
        for (nlevel=0;nlevel<0x8000;nlevel+=5) {
            bmcusb_constantDM(DM,nlevel);
            updates++;
            //usleep(1000);
        }
    }

    printf("Made %ld updates to the DM.\n", updates);
    bmcusb_zeroDM(DM);
    bmcusb_setHV(DM,0);
    bmcusb_release(DM);
    exit(0);
}
// kate: indent-mode cstyle; space-indent on; indent-width 0; 
