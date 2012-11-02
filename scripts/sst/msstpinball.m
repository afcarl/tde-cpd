clear all;
load ../../datasets/pinball/pinball1.dat
Y = pinball1; 

% Clip the data in the 0-1 range
for i = 1:size(Y, 2)
    Y(:, i) = (Y(:, i) - min(Y(:, i)))/(max(Y(:, i)) - min(Y(:, i)));
end


subplot(4, 1, 1);
plot(Y(:, 1));
subplot(4, 1, 2);
plot(Y(:, 2));
subplot(4, 1, 3);
stem(Y(:, 3));

%% Changepoint Detection
% Window size
w = 5;

% Number of windows in the future.
% This number is the same as n for the past.
m = 5;

% Length of the subsequence falling under the m
% overlapping windows of length w.
subseq_len = m+w-1;
    
scores = zeros(size(Y,1));
for n = subseq_len+1:size(Y,1)-subseq_len;  
    % Multivariate embedding for the past
    H = [make_hankel(Y(n-subseq_len:end, 1), w, m); ...
         make_hankel(Y(n-subseq_len:end, 2), w, m); ...
         make_hankel(Y(n-subseq_len:end, 3), w, m)];
    
    [Ul, S, ~] = svd(H);
    S = diag(S);
    cum_energy = cumsum(S)/sum(S);
    lp = find(cum_energy > 0.9, 1);     

    % Embedding of the future sequence
    G = [make_hankel(Y(n:end, 1), w, m); ...
         make_hankel(Y(n:end, 2), w, m); ... 
         make_hankel(Y(n:end, 3), w, m)];
    [B, ~, ~] = svd(G);
      
    s = 0;
    for i = 1:lp
        s = s + (B(:, 1)'*Ul(:, i))^2;
    end
    scores(n) = 1 - s;
end

subplot(4,1,4);
plot(scores);
