function [raw_data] = getRawData(bps,distance,medium,samp_rate)

file_name = [int2str(bps) 'bps_1500bit_' int2str(distance) 'mm_' medium '.csv'];
bit_no = 1500;
bit_length = ceil(samp_rate * 1/bps * bit_no);

switch (file_name)
    case "1000bps_1500bit_50mm_air.csv"
        start_idx = 482838;
    case "1000bps_1500bit_50mm_salt4.csv"
        start_idx = 146461;
    case "3000bps_1500bit_50mm_air.csv"
        start_idx = 71550;
    case "3000bps_1500bit_50mm_salt4.csv"
        start_idx = 84885;
    case "5000bps_1500bit_50mm_air.csv"
        start_idx = 449220;
    case "5000bps_1500bit_50mm_salt4.csv"
        start_idx = 161574;
    case "7000bps_1500bit_50mm_air.csv"
        start_idx = 39531;
    case "7000bps_1500bit_50mm_salt4.csv"
        start_idx = 49450;
    case "1000bps_1500bit_100mm_air.csv"
        start_idx = 520853;
    case "1000bps_1500bit_100mm_salt4.csv"
        start_idx = 152172;
    case "3000bps_1500bit_100mm_air.csv"
        start_idx = 17912;
    case "3000bps_1500bit_100mm_salt4.csv"
        start_idx = 130560;
    case "5000bps_1500bit_100mm_air.csv"
        start_idx = 530736;
    case "5000bps_1500bit_100mm_salt4.csv"
        start_idx = 136615;
    case "7000bps_1500bit_100mm_air.csv"
        start_idx = 12237;
    case "7000bps_1500bit_100mm_salt4.csv"
        start_idx = 47745;
    otherwise
        error("File name error!")
end

end_idx = start_idx + bit_length - 1;

data_range = [start_idx 2 end_idx 3];
% data_range = [start_idx 2];

raw_data = readmatrix(file_name, "Range", data_range);
if isempty(raw_data)
    error("File read error!")
end
end
