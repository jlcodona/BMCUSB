#include <stdio.h>
#include <usb.h>
#include <stdlib.h>
#include <unistd.h>
#include "bmcusb.h"

int main(int argc, char **argv)
{
    int DM = 0;
    int NumBMC;

    if (!(NumBMC=bmcusb_probe())) {
        printf("No BMC drivers found.  Quitting.\n");
        exit(1);
    }

    if (bmcusb_claim(DM)) {
        printf("ERROR: Could not CLAIM the DM Driver interface.\n");
        exit(1);
    }
    printf("Firmware version: <%s>\n",bmcusb_getFirmwareVersion(DM));

    bmcusb_reset(DM);
    bmcusb_setHV(DM,1);

    u_int16_t sActData[NUM_ACTUATORS] = {0x0000};    // unmapped actuator data for sending to the DM
    int counter, val, n, nloop, nloop2;
    
    int MAXVAL = 0x8000;
    for (nloop=0; nloop<4; nloop++) {
        counter = 0;
        for (nloop2=0; nloop2<4; nloop2++) {
            for (val=0; val<MAXVAL; val+=1) {
                bmcusb_constantDM(DM,val);
            }
        }

        clearBuffer(sActData,NUM_ACTUATORS);
        for (n=0;n<NUM_ACTUATORS;n++) {
            for (val=0; val<MAXVAL; val+=10 ) {
                sActData[n] = val;
                bmcusb_setMappedDM(DM,sActData);
            }
            sActData[n] = 0;
        }
        bmcusb_zeroDM(DM);
    }

    bmcusb_release(DM);

    exit(0);
}
// kate: indent-mode cstyle; space-indent on; indent-width 0; 
