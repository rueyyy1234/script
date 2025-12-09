function data = samplesToData(sampled_data, samples_per_bit, sent_bit_length)

change_points = [true, sampled_data(1:end-1) ~= sampled_data(2:end)];

% find consecutive bits length
consec_bit_idx = find(change_points);

consec_bit_arr = diff([consec_bit_idx, length(sampled_data) + 1]);
consec_bit_arr = round(consec_bit_arr/samples_per_bit);
% consec_bit_arr(1:10)

val = 1;   % always start with 1
data = [];
for k = 1:length(consec_bit_arr)
    data = [data, repmat(val, 1, consec_bit_arr(k))];
    val = 1 - val;  % flip between 1 and 0
end

difference = length(data) - sent_bit_length;
if(difference > 0)
    % trim
    data = data(1:sent_bit_length);
elseif(difference < 0)
    % pad with zeros
    data = [data zeros(1,-difference)];
end
end