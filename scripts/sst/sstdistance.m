clear all;

Y1 = [arrayfun(@(x)(1.5*sin(x*0.2)), 1:400) ...
      arrayfun(@(x)(1.5*sin(x*0.3)), 401:700) ...
      arrayfun(@(x)(1.5*sin(x*0.7)), 701:1200)]';
Y1 = Y1 + randn(length(Y1), 1);

% Window size
w = 80;

% Number of overlapping windows
m = 15;

subseq_len = m+w-1;

scores = zeros(size(Y1));
for n = subseq_len+1:length(Y1)-subseq_len;
   % Create the base matrix
   H = make_hankel(Y1(n-subseq_len:end), w, m); 
   [Ul, S, ~] = svd(H);
   S = diag(S);
   cum_energy = cumsum(S)/sum(S);
   lp = find(cum_energy > 0.9, 1);

   % Create test matrix
   G = make_hankel(Y1(n:end), w, m);
   [B, ~, ~] = svd(G);
      
   s = 0;
   for i = 1:lp
       s = s + (B(:, 1)'*Ul(:, i))^2;
   end
   scores(n) = 1 - s;
end

subplot(2, 1, 1);
plot(Y1);
subplot(2, 1, 2);
plot(scores);