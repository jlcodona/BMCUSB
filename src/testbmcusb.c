#include <stdio.h>
#include "bmcusb.h"

#define TIMEOUT   1000
#define BYTESPERLINE 16

#define MAPPING mapMultiDM01

int main(int argc, char **argv)
{
  int Nbmc;
  Nbmc = bmcusb_probe();
  
  printf("Found %d BMC Multi-Drivers.\n",Nbmc);
  
  return(0);
}
