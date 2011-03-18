% This is a convenience function for using the mexBMC interface to libbmcusb.
%
% JLCodona 20110301.  Simplify!

%  This initializes the mex file, looks for any BMC USB interface, and builds data structures.
mexBMC('init');

%  This claims device 0.  I haven't tested it with more than one device, so let me know if you do!
mexBMC('claim',0);

%  Reset the USB interface on device 0.
mexBMC('reset',0);

%  Turn on the high-voltage power to the DM.
mexBMC('powerON');

%  This file contains some actuator mappings.  JLCMAP1 allows you to use 12x12 MATLAB arrays easily.
BMCMappings
mexBMC('setMap',0,JLCMAP1);  % Use JLCMAP1 with DM 0.

%  These are piston tests and serve no other purpose.
% setBMC(0,1);
% setBMC(0,0);

