#include "mex.h"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <math.h>
#include "bmcusb.h"
#include "BMC_Mappings.h"
#include "CIUsbLib.h"

extern void _main();

static int Nbmc = 0;
static int customMap[USB_NUM_ACTUATORS_MULTI] = {0}; // This is our own actuator map storage.

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
    /* Check for proper number of arguments */

    if (nrhs < 1) {
        mexErrMsgTxt("mexBMC requires at least one argument.");
    } else if (nlhs >= 1) {
        mexErrMsgTxt("MEXBMC requires no output argument.");
    }

    /* Input must be a string. */
    if (mxIsChar(prhs[0]) != 1)
        mexErrMsgTxt("The first argument must be a command string.");

    /* Input must be a row vector. */
    if (mxGetM(prhs[0]) != 1)
        mexErrMsgTxt("Input must be a row vector.");

    /* Get the length of the input string. */
    int buflen = (mxGetM(prhs[0]) * mxGetN(prhs[0])) + 1;

    /* Allocate memory for input and output strings. */
    char *cmd = (char *)mxCalloc(buflen, sizeof(char));
    //output_buf = mxCalloc(buflen, sizeof(char));

    /* Copy the string data from prhs[0] into a C string
     * input_buf. */
    int status = mxGetString(prhs[0], cmd, buflen);
    if (status != 0)
        mexWarnMsgTxt("Not enough space. String is truncated.");

    //printf("mexBMC: command is '%s'\n",cmd);

    // Commands:
    // "help"
    // "init"
    // "claim"
    // "release"
    // "reset"
    // "power"
    // "clear" or "zero"
    // "setDM"

    if (!strcmp(cmd,"help")) {
        printf("Commands... (for simplicity, this mex command assumes you have only one DM.)\n");
        printf("help: this output\n");
        printf("init: scan for devices and return the count.\n");
        printf("reset\n");
        printf("powerON or powerOFF\n");
        printf("clear or zero\n");
        printf("setDM (can be a scalar or a 12x12 array)\n");
        printf("setActs (uses actuator list as a buffer.  NOTE: 160 elements!!!)\n");
        printf("setMap (Define and use an actuator map. NOTE: 160 elements!!!)\n");

        return;
    } else if (!strcmp(cmd,"init")) { // init
        int ndevices = bmcusb_probe();
        printf("Found %d BMC Multi-Driver boxes.\n",ndevices);
        return;

        ///////////////////////////////////////
    } else if (!strcmp(cmd,"clear") || !strcmp(cmd,"zero") ) {
        if (nrhs < 2) { // no DM specified.
            bmcusb_zeroDM(0);
            return;
        } else if (nrhs >= 2) { // a DM was specified.
            int m = mxGetM(prhs[1]);
            int n = mxGetN(prhs[1]);

            if (m>1 || n>1) {
                printf("clear: WARNING! DM specifier wasn't a scalar. (%d,%d)\n",m,n);
            }

            int dm = (int) *mxGetPr(prhs[1]);
            printf("clear: clearing SmartDriver DM #%d.\n",dm);
            bmcusb_zeroDM(dm);
            return;
        }
        return;
    } else if (!strcmp(cmd,"setDM")) {
        // This requires two more arguments: (cmd,dm,DAC)
        // DAC can be a scalar or a 12x12 matrix.
        // DAC is expected to contain integer DAC settings.

        if (nrhs < 3) { // no DM specified.
            mexWarnMsgTxt("mexBMC setDM: require two args: dm,DAC.");
            return;
        }

        int dm = (int) *mxGetPr(prhs[1]);

        int M = mxGetM(prhs[2]);
        int N = mxGetN(prhs[2]);

        if (M==1 && N==1) { // DAC is a scalar
            int dac = (int) *mxGetPr(prhs[2]);

            bmcusb_constantDM(dm,dac);
            return;
        } else if (M==12 && N==12) { // DAC is a 12x12 matrix.
            u_int16_t DAC[160]; // This is the size of the Multi-Driver USB buffer.  We only use the first 144;
            double *fDAC = mxGetPr(prhs[2]); // The MATLAB array will be doubles.
            int i;
            for(i=0;i<M*N;i++) {
                DAC[i] = (u_int16_t)(fDAC[i]);
                // printf("%6d (%7.1f) \n", DAC[i], fDAC[i]);
            }
            // printf("\n");

            bmcusb_setMappedDM(dm,DAC);

        } else {
            printf("ERROR: mexBMC setDM: DAC has dims %dx%d.\n",M,N);
            return;
        }
        return;

        ///////////////////////////////////////
 
      
        
        
        
      } else if (M==12 && N==12) { // DAC is a 12x12 matrix.
            u_int16_t DAC[160]; // This is the size of the Multi-Driver USB buffer.  We only use the first 144;
            double *fDAC = mxGetPr(prhs[2]); // The MATLAB array will be doubles.
            int i;
            for(i=0;i<M*N;i++) {
                DAC[i] = (u_int16_t)(fDAC[i]);
                // printf("%6d (%7.1f) \n", DAC[i], fDAC[i]);
            }
            // printf("\n");

            bmcusb_setMappedDM(dm,DAC);

        } else {
            printf("ERROR: mexBMC setDM: DAC has dims %dx%d.\n",M,N);
            return;
        }
        return;

        ///////////////////////////////////////
        
        
        
        
        
        
        
        
        
        
        
        
        
        
    } else if (!strcmp(cmd,"setDM_")) {
        // This requires two more arguments: (cmd,dm,DAC)
        // DAC can be a scalar or a 12x12 matrix.
        // DAC is expected to contain integer DAC settings.

        if (nrhs < 3) { // no DM specified.
            mexWarnMsgTxt("mexBMC setDM_: require two args: dm,DAC.");
            return;
        }

        int dm = (int) *mxGetPr(prhs[1]);

        int M = mxGetM(prhs[2]);
        int N = mxGetN(prhs[2]);

        if (M==1 && N==1) { // DAC is a scalar
            int dac = (int) *mxGetPr(prhs[2]);

            bmcusb_constantDM(dm,dac);
            return;
        } else if (M==12 && N==12) { // DAC is a 12x12 matrix.
            u_int16_t DAC[160];
            int *iDAC = (int *) mxGetPr(prhs[2]);
            int i;
            for(i=0;i<M*N;i++) {
                DAC[i] = (WORD)(iDAC[i]);
                // printf("%6d (%7.1f) \n", DAC[i], fDAC[i]);
            }
            // printf("\n");

            bmcusb_setMappedDM(dm,DAC);
        } else {
            printf("mexBMC setDM_: DAC has dims %dx%d. This is an error.\n",M,N);
            return;
        }
        return;
    } else if (!strcmp(cmd,"debug")) {
        bmcusb_setDebug(1);
        usb_set_debug(5);
    } else if (!strcmp(cmd,"verbose")) {
        bmcusb_setDebug(1);
        usb_set_debug(5);
    } else if (!strcmp(cmd,"quiet")) {
        bmcusb_setDebug(0);
        usb_set_debug(0);
    } else if (!strcmp(cmd,"reset")) {
        bmcusb_reset(0);
        return;
    } else if (!strcmp(cmd,"claim")) {
        bmcusb_claim(0);
        return;
    } else if (!strcmp(cmd,"release")) {
        bmcusb_release(0);
        return;
    } else if (!strcmp(cmd,"powerON")) {
        bmcusb_setHV(0,1);
        return;
    } else if (!strcmp(cmd,"powerOFF")) {
        bmcusb_setHV(0,0);
        return;
    } else {
        mexWarnMsgTxt("Unknown command.");
        return;
    }

    return;
}
// kate: indent-mode cstyle; space-indent on; indent-width 0; 
