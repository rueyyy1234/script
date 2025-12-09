function params = getParams(idx)

tbl = {
    1000,  50,  'air',   1;
    1000,  50, 'salt4',  2;
    3000,  50, 'air',    3;
    3000,  50, 'salt4',  4;
    5000,  50, 'air',    5;
    5000,  50, 'salt4',  6;
    7000,  50, 'air',    7;
    7000,  50, 'salt4',  8;
    1000, 100, 'air',    9;
    1000, 100, 'salt4', 10;
    3000, 100, 'air',   11;
    3000, 100, 'salt4', 12;
    5000, 100, 'air',   13;
    5000, 100, 'salt4', 14;
    7000, 100, 'air',   15;
    7000, 100, 'salt4', 16;
};


params.bps = tbl{idx, 1};
params.distance = tbl{idx, 2};
params.medium = tbl{idx, 3};
end