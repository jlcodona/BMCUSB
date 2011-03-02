function setBMC(VALS)

% setBMC(VALS): set the actuators on a BMC DM using a USB multi-driver.
% VALS is 0 <= VALS <= 1
% 20110225: JLCodona
% 20110227: JLCodona Make sure you run BMCMappings first to get this right.

VALS = max(VALS,0);
VALS = min(VALS,1);

DACS = sqrt(VALS) * 32768; % 0x8000 is the max I am using here.

% mexBMC('setDM',0,DACS);
% mexBMC('setDM_',0,DACS);
mexBMC('setPostmappedDM',0,DACS);

