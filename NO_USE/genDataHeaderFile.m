clear;

n = 5000;
bps = 1000;

fileID = fopen("data.h", 'w');

bit_interval_us = 1e6/bps;
runtime = bit_interval_us * n;
if runtime >= 1e6
    runtime = runtime / 1e6;
    unit = 's';
elseif runtime >= 1e3
    runtime = runtime / 1e3;
    unit = 'ms';
else
    unit = 'us';
end

data = randi([0 1],1,n);
fprintf(fileID, "// Estimated time: %d %s\n\n", runtime,unit);
fprintf(fileID, "#define BIT_NO %d\n", n);
fprintf(fileID, "#define BIT_INTERVAL_US %d\n\n", bit_interval_us);
fprintf(fileID, "uint8_t rand_data[BIT_NO] = {");
fprintf(fileID, "%d, ",data(1:end-1));
fprintf(fileID, "%d};\n",data(end));

fclose(fileID);