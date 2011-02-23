#include <usb.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "bmcusb.h"

#define TIMEOUT   1000

int main(int argc, char **argv)
{
  int Nbmc;

  Nbmc = bmcusb_probe();
  //bmcusb_setDebug(TRUE);
  
  printf("Found %d BMC Multi-Drivers.\n",Nbmc);
  
  if(Nbmc==0) exit(-1);
  
  int DM = 0;

  if(bmcusb_claim(DM)) {
    printf("FAILED to claim interface.\n");
    exit(1);
  } else printf("CLAIMED the USB interface to the BMC DM driver.\n");
  
  printf("Firmware version: %s\n",bmcusb_getFirmwareVersion(DM));
  bmcusb_reset(DM);
  bmcusb_printStatus(DM);
  bmcusb_setHV(DM,TRUE);
  bmcusb_printStatus(DM);
  
  int nlevel;
  int nloop;
  long updates = 0;

  u_int16_t data[USB_NUM_ACTUATORS_MULTI];
  
  for(nloop=0;nloop<1;nloop++) {
    for(nlevel=0;nlevel<4096;nlevel+=5) {
	int q;
	for(q=0;q<USB_NUM_ACTUATORS_MULTI;q++) data[q] = nlevel;
	bmcusb_setDM(DM,data);
	//printf("%d \n",nlevel);
	//bmcusb_constantDM(DM,nlevel);
	//bmcusb_printStatus(DM);
      updates++;
      usleep(1000);
    }
  }
  
  printf("Made %d updates to the DM.\n", updates);
//   bmcusb_zeroDM(DM);
  bmcusb_setHV(DM,FALSE);

  bmcusb_release(DM);
  exit(0);
}
