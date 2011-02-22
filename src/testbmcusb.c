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
//   bmcusb_setDebug(TRUE);
  
  printf("Found %d BMC Multi-Drivers.\n",Nbmc);
  
  if(Nbmc==0) exit(-1);
  
  int DM = 0;

  if(bmcusb_claim(DM)) {
    printf("FAILED to claim interface.\n");
    exit(1);
  } else printf("CLAIMED the USB interface to the BMC DM driver.\n");
  
  printf("Firmware version: %s\n",bmcusb_getFirmwareVersion(DM));
  bmcusb_reset(DM);
  bmcusb_setHV(DM,TRUE);
  
  int nlevel;
  int nloop;
  long updates = 0;
  
  
  for(nloop=0;nloop<1;nloop++) {
    for(nlevel=0;nlevel<4096;nlevel+=5) {
      //printf("%d \n",nlevel);
      bmcusb_constantDM(DM,nlevel);
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
