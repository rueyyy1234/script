clear;

n = 1500;

% Define bit rates
bps.setup_1 = 1000;
bps.setup_2 = 3000;
bps.setup_3 = 5000;
bps.setup_4 = 7000;

setup_names = fieldnames(bps);
total_bps_setup = numel(setup_names);

for i = 1:total_bps_setup
    bps.(sprintf('bit_interval_us_%d', i)) = round(1e6 / bps.(sprintf('setup_%d', i)));
end

fileID = fopen("data.h", "w");

% Generate random data
data = randi([0 1], 1, n);

% Loop and print each setup block
for i = 1:total_bps_setup

    bit_interval_us = bps.(sprintf('bit_interval_us_%d', i));
    bitrate = bps.(sprintf('setup_%d', i));
    runtime = bit_interval_us * n;

    % Select unit
    if runtime >= 1e6
        runtime_adj = runtime / 1e6;
        unit = 's';
    elseif runtime >= 1e3
        runtime_adj = runtime / 1e3;
        unit = 'ms';
    else
        runtime_adj = runtime;
        unit = 'us';
    end

    % Print runtime comment
    fprintf(fileID, "// Setup %d (%d bps) estimated time: %.3f %s\n", ...
        i, bitrate, runtime_adj, unit);

    % Print macro
    fprintf(fileID, "#define BIT_INTERVAL_US_%d %d\n\n", ...
        i, bit_interval_us);
end

% Print data array
fprintf(fileID, "#define BIT_NO %d\n", n);
fprintf(fileID, "uint8_t rand_data[BIT_NO] = {");

fprintf(fileID, "%d, ", data(1:end-1));
fprintf(fileID, "%d};\n", data(end));

fclose(fileID);
