function genRandDataFile(n, fname)

if mod(n,8) ~= 0
    error("Parameter error: n must be multiple of 8");
end

d = zeros(n,1);
d(1:4)        = [1 0 1 0];        % start marker
d(end-3:end)  = [0 1 0 1];        % end marker
d(5:end-4)    = randi([0 1], n-8, 1);

d8 = reshape(d, 8, []).';
ws = [128 64 32 16 8 4 2 1];   % let them be double
bytes = uint8(double(d8) * ws.');

fileID = fopen(fname, 'w');
if fileID < 0
    error("Unable to open file: %s", fname);
end

fprintf(fileID, '%d\n', bytes);   % write to csv
fclose(fileID);
end
