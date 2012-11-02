function [ H ] = make_hankel( X, w, m )
% make_hankel Create the Hankel matrix of time series X.
%   w specifies the window size, m their number. The embedding of X
% is returned in matrix H.

H = zeros(w, m);

for i = 1:m
   H(:, i) = X(i:i+w-1)'; 
end

end

