#include "mex.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <math.h>
#include "bmcusb.h"
#include "BMC_Mappings.h"
#include "CIUsbLib.h"

#define min(a,b) (((a) < (b)) ? (a) : (b))

static int MEX_VERBOSE = 0;

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

    /* Copy the string data from prhs[0] into a C string * input_buf. */
    int status = mxGetString(prhs[0], cmd, buflen);
    if (status != 0)
        mexWarnMsgTxt("Not enough space. String is truncated.");

    if(MEX_VERBOSE) printf("mexBMC: command is '%s'\n",cmd);

    if (!strcmp(cmd,"help")) {
        printf("Commands: ");
        printf("claim clear debug help init powerOFF powerON quiet release \nreset setMexDebug setDM setDM_ setMap startup0 shutdown0 verbose zero\n");
        return;
    }



// init
    else if (!strcmp(cmd,"init")) { // init
        int ndevices = bmcusb_probe();
        printf("Found %d BMC Multi-Driver boxes.\n",ndevices);
        return;
    }


// clear
    else if (!strcmp(cmd,"clear") || !strcmp(cmd,"zero") ) {
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
            if(MEX_VERBOSE) printf("clear: clearing SmartDriver DM #%d.\n",dm);
            bmcusb_zeroDM(dm);
            return;
        }
        return;
    }



// setMexDebug
    else if (!strcmp(cmd,"setMexDebug") || !strcmp(cmd,"zero") ) {
        if (nrhs < 2) { // no argument just toggles debug state.
            MEX_VERBOSE = !MEX_VERBOSE;
            return;
        } else if (nrhs >= 2) { // a DM was specified.
            int m = mxGetM(prhs[1]);
            int n = mxGetN(prhs[1]);

            if (m>1 || n>1) {
                printf("setDebug: WARNING! Argument not a scalar. (%d,%d)\n",m,n);
            }

            MEX_VERBOSE = (int) *mxGetPr(prhs[1]);
            if(MEX_VERBOSE) printf("setDebug: MEX_VERBOSE set to #%d.\n",MEX_VERBOSE);
            return;
        }
        return;
    }



// setDM
    else if (!strcmp(cmd,"setDM") || !strcmp(cmd,"setMappedDM")||!strcmp(cmd,"setPremappedDM")) {
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
            u_int16_t DAC[USB_NUM_ACTUATORS_MULTI]; // This is the size of the Multi-Driver USB buffer.  We only use the first 144;
            double *fDAC = mxGetPr(prhs[2]); // The MATLAB array will be doubles.
            int i;
            for(i=0;i<M*N;i++) {
                DAC[i] = (u_int16_t)(fDAC[i]);
                if(MEX_VERBOSE) printf("%3d: %6d (%7.1f) \n", i,DAC[i],fDAC[i]);
            }
            if(MEX_VERBOSE) printf("\n");

            bmcusb_setMappedDM(dm,DAC);

        } else {
            printf("ERROR: mexBMC setDM: DAC has dims %dx%d.\n",M,N);
            return;
        }
        return;
      }


// setPostmappedDM
    else if (!strcmp(cmd,"setPostmappedDM") || !strcmp(cmd,"setPostMappedDM") || !strcmp(cmd,"setPostDM")) {
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
            u_int16_t DAC[USB_NUM_ACTUATORS_MULTI]; // This is the size of the Multi-Driver USB buffer.  We only use the first 144;
            double *fDAC = mxGetPr(prhs[2]); // The MATLAB array will be doubles.
            int i;
            for(i=0;i<M*N;i++) {
                DAC[i] = (u_int16_t)(fDAC[i]);
                if(MEX_VERBOSE) printf("%3d: %6d (%7.1f) \n", i,DAC[i],fDAC[i]);
            }
            if(MEX_VERBOSE) printf("\n");

            bmcusb_setPostMappedDM (dm,DAC);

        } else {
            printf("ERROR: mexBMC setDM: DAC has dims %dx%d.\n",M,N);
            return;
        }
        return;
      }



// setDM_
    else if (!strcmp(cmd,"setDM_")||!strcmp(cmd,"setUnmappedDM")) {
        // Unmapped setDM_: This requires two more arguments: (cmd,dm,DAC)
        // DAC can be any scalar or a 12x12 matrix.
        // DAC can be any dimension, but only the first 160 elements get sent.

        if (nrhs < 3) { // no DM specified.
            mexWarnMsgTxt("mexBMC setUnmappedDM: require two args: dm,DAC.");
            return;
        }

        int dm = (int) *mxGetPr(prhs[1]);

        int M = mxGetM(prhs[2]);
        int N = mxGetN(prhs[2]);

        u_int16_t DAC[USB_NUM_ACTUATORS_MULTI] = {0}; // This is the size of the Multi-Driver USB buffer.  We only use the first 144;
        double *fDAC = mxGetPr(prhs[2]); // The MATLAB array will be doubles.
        int i; int Nlimit = min(M*N,USB_NUM_ACTUATORS_MULTI);

        for (i=0;i<Nlimit;i++) {
            DAC[i] = (u_int16_t)(fDAC[i]);
            if (MEX_VERBOSE) printf("%3d: %6d (%7.1f) \n", i,DAC[i],fDAC[i]);
        }
        if (MEX_VERBOSE) printf("\n");

        bmcusb_setDM(dm,DAC);
        return;
    }

// setMap
    else if (!strcmp(cmd,"setMap")) {
        // This requires two more arguments: (cmd,dm,MAP)
        // MAP is a linear map array from unwrapped matrix location to serialized command slot.
        // MAP is an integer map of some length, but only the first 160 will be used.

        if (nrhs < 3) { // no MAP specified.
            mexWarnMsgTxt("mexBMC setMap: requires two args: dm,MAP.");
            return;
        }

        int dm = (int) *mxGetPr(prhs[1]);

        int M = mxGetM(prhs[2]);
        int N = mxGetN(prhs[2]);

        int Nelements = min(N*M,USB_NUM_ACTUATORS_MULTI);

        double *fMAP = mxGetPr(prhs[2]); // The MATLAB array will be doubles.
        int i;
        for (i=0;i<Nelements;i++) {
            customMap[i] = (int)(fMAP[i]);
            if(MEX_VERBOSE) printf("%3d: %6d (%7.1f) \n", i,customMap[i],fMAP[i]);
        }
        if(MEX_VERBOSE) printf("\n");

        bmcusb_setMap(dm,customMap);
        return;
    }

// debug
    else if (!strcmp(cmd,"debug")) {
        bmcusb_setDebug(1);
        usb_set_debug(5);
    }

// verbose
    else if (!strcmp(cmd,"verbose")) {
        MEX_VERBOSE = 1;
        bmcusb_setDebug(1);
        usb_set_debug(5);
    }

// quiet
    else if (!strcmp(cmd,"quiet")) {  // TODO: Fix support for multiple DMs.
        MEX_VERBOSE = 0;
        bmcusb_setDebug(0);
        usb_set_debug(0);
    }

// startup0 (this is a shorthand command for claim, reset, and enable power on DM 0).
    else if (!strcmp(cmd,"startup0")) {  
        bmcusb_claim(0);
        bmcusb_reset(0);
        bmcusb_setHV(0,1);
        return;
    }
    
// shutdown0 (this is a shorthand command for power-off and release DM 0).
    else if (!strcmp(cmd,"shutdown0")) {
        bmcusb_setHV(0,0);
        bmcusb_release(0);
        return;
    }

// reset
    else if (!strcmp(cmd,"reset")) {  // TODO: Fix support for multiple DMs.
        bmcusb_reset(0);
        return;
    }

// claim
    else if (!strcmp(cmd,"claim")) {  // TODO: Fix support for multiple DMs.
        bmcusb_claim(0);
        return;
    }
    
// release
    else if (!strcmp(cmd,"release")) {  // TODO: Fix support for multiple DMs.
        bmcusb_release(0);
        return;
    }
    
// powerON
    else if (!strcmp(cmd,"powerON")) {  // TODO: Fix support for multiple DMs.
        bmcusb_setHV(0,1);
        return;
    }
    
// powerOFF
    else if (!strcmp(cmd,"powerOFF")) {  // TODO: Fix support for multiple DMs.
        bmcusb_setHV(0,0);
        return;
    }
    
// out of options...
    else {
        mexWarnMsgTxt("Unknown command.");
        return;
    }

    return;
}
// kate: indent-mode cstyle; space-indent on; indent-width 0; 
