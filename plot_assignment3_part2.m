clear all
close all

fid = fopen("MonteCarlo/ex3_results.txt",'r');

data = [];
i = 1;
line = fgetl(fid);
while ischar(line)
   words = split(line);
   data(i,1) = str2double(words{1});
   data(i,2) = str2double(words{2});
   line = fgetl(fid);
   i = i+1;
end

%%
numOfGens = max(data(:,1));
ar = zeros(numOfGens,2);
for i = 1:length(data)
    generation = data(i,1);
    energy = data(i,2);
    if (isnan(energy))
        i
    else
        ar(generation,1) = ar(generation,1) + energy;
        ar(generation,2) = ar(generation,2) + 1;
    end
end

results = zeros(numOfGens,1);
for i = 1:length(ar)
    results(i) = ar(i,1) / ar(i,2);
end

semilogy(1:numOfGens, results)