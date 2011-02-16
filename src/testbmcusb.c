#include <stdio.h>
#include "bmcusb.h"

#define TIMEOUT   1000
#define BYTESPERLINE 16

#define MAPPING mapMultiDM01

void clearBuffer(bmc_actuatorData_t buf[], int N) {
  int n;
  for(n=0;n<N;n++) {
    buf[n] = 0;
  }
}

bmc_actuatorData_t *mapActs(bmc_actuatorData_t *FROM, bmc_actuatorData_t *TO, const int actMap[]) {
  int n;
  for(n=0;n<NUM_ACTUATORS;n++)
    TO[n] = FROM[actMap[n]];
  return(TO);
}

int main(int argc, char **argv)
{
  int Nbmc;
  Nbmc = bmcusb_probe();
  
  printf("Found %d BMC Multi-Drivers.\n",Nbmc);
  
  return(0);
}
