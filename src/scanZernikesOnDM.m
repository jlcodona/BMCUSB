% SCALE = 5000;
SCALE = 1000;
OFFSET = 0.2;

% mexBMC('init')
% mexBMC('claim',0)
% mexBMC('reset',0)
% mexBMC('powerON',0)

xx = demean(1:12)/12;
[XX,YY] = meshgrid(xx,xx);
r = 2*sqrt(XX.^2+YY.^2);
t = atan2(YY,XX);

for n=1:10
    for m=-n:2:n
        %say(sprintf('mode %d %d',n,m));
        fprintf('mode %d %d\n',n,m);
        ZERN = eval(ZernikeStringP(n,m));
        %for q=0:.002:8*pi
        for q=0:.02:8*pi
            VALS = sin(q)*ZERN;
            VALS=sqrt(VALS-min(VALS(:))+OFFSET)*SCALE;
            mexBMC('setDM',0,VALS);
        end
    end
end

mexBMC('zero');
