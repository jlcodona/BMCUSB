function DM = pokeBMC(n1,n2,val)

% DM = pokeBMC(n1,n2,[val]): set one actuator to a test value.
% val is optional and defaults to 0.5
%  This is a test function to poke an actuator.  I used it mostly to figure out the actuator mappings.
%
% NOTE: This function may be out of sync with any reasonable numbering for the actuators.
%  
% 20110226: JLCodona
%
% JLCMAP1 = ...
% [ 79, 88, 34, 35,103, 23, 89,129,124, 44, 11, 99, ...
%  134, 54, 55, 83, 38, 98,118,153, 93, 40, 39,  8, ...
%  114,128,139,123,  3, 18,158,104,113, 53,  4, 59, ...
%  119,108,148,  9, 29  69,109,149,133, 60, 45, 48, ...
%   68, 94, 14, 50, 43, 78, 58,159, 84, 73, 30, 19, ...
%  154, 74, 28,143, 63, 49,138,144, 64, 24, 42,122, ...
%  147,131, 20, 96, 16, 81,152, 97,106, 17, 82,102, ...
%   87, 27, 10,116,  1, 92,141,  5, 37,  6, 22,157, ...
%   91, 67,136, 76, 41,132,101, 77, 46, 33, 62,127, ...
%  111,  7, 25, 32, 52,121,137, 57, 66, 13, 51,142, ...
%  107, 31,156, 36, 72,112,117,126, 26, 15, 71, 47, ...
%  151, 12, 56, 21, 61,146, 86,  2, 65, 70, 75, 80, ...
%   85, 90, 95,100,105,110,115,120,125,130,135,140, ...
%  145,150,155,160,160,160,160,160,160,160,160,160]; 

SCALE = 2^15;

if(nargin<3)
    val = .5;
end
DM = zeros(12);
if(nargin==3)
    DM(n1,n2) = SCALE*min(max(val,0),1);
else
    DM(n1) = SCALE*min(max(n2,0),1); % linear addressing
end
imagesc(DM');daspect([1 1 1]);colorbar;
% axis xy;
drawnow;

% setBMC(DM);
% mexBMC('setDM_',0,DM*(2^15));
% mexBMC('setDM',0,DM*(2^15));
mexBMC('setPostmappedDM',0,DM);