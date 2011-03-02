DMMASK = ones(12);
DMMASK(1,1) = 0;
DMMASK(1,12) = 0;
DMMASK(12,1) = 0;
DMMASK(12,12) = 0;
% DMMASK = find(DMMASK);

MAP = mapMultiDM01;
[TMP,MAP_] = sort(MAP);

for nn=1:10
%     DAC = zeros(1,160);
    DAC = zeros(12);
    for n=1:160
        DAC = zeros(12);
        %DAC(n) = 35000;
        %DAC(MAP(n)) = 35000;
        %DAC(MAP_(n)) = 35000;
        DAC(MAP_(1:n)) = 35000;
        %DAC((1:n)) = 35000;
        %mexBMC('setDM_',0,DAC);
        %mexBMC('setDM',0,DAC);
        %say(sprintf('%d',n));
        pause(1/60);
    end
end
