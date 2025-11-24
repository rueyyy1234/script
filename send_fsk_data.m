clear;
delete(serialportfind);

devName = '/dev/tty.usbmodem1301';
dev = serialport(devName,460800);
flush(dev);

data = repelem(170,20);

while (true)
    write(dev, uint8(data), "uint8");
end