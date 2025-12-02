clear; clc
delete(serialportfind);

devName = "/dev/cu.usbmodem11201";
dev = serialport(devName,115200);

rand_data = readmatrix("test.csv");
configureCallback(dev, "terminator", @printRead);

bit_interval_us = uint16(5000);
write(dev, bit_interval_us, "uint16");

% wait for Arduino to confirm
pause(0.5);
runtime_us = double(length(rand_data))* 8 * double(bit_interval_us);
printRuntime(runtime_us);

for k = 1:length(rand_data)
    write(dev, uint8(rand_data(k)), "uint8");
end

function printRead(src, ~)
    data = read(src, src.NumBytesAvailable, "uint8");
    disp(char(data)')
end

function printRuntime(us)
if us >= 1e6
    value = us / 1e6;
    unit = 's';
elseif us >= 1e3
    value = us / 1e3;
    unit = 'ms';
else
    value = us;
    unit = 'us';
end

fprintf('Estimated runtime: %.3f %s\n', value, unit);

end