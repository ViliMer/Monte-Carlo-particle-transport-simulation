function [symbol, Z, A, AW, T, NNU, nubar_data, MTs, Qs, energies] = input_reader_matlab(fn)

fid = fopen(fn);


% Parse first line
line = split(fgetl(fid));
symbol = line{1};
Z = str2double(line{2});
A = str2double(line{3});
AW = str2double(line{4});
T = str2double(line{5});

% Parse second line
line = split(fgetl(fid));
NNU = str2double(line{1});

% Parse nubar data
nubar_data = zeros(NNU,2);
for i = 1:NNU
    line = split(fgetl(fid));
    nubar_data(i,:) = [str2double(line{1}), str2double(line{2})];
end

% Parse reactions
MTs = [];
Qs = [];
energy_cross_section_data = [];
energies = {};
j=0;
line = fgetl(fid);
while ischar(line)
    line = split(line);
    j = j+1;
    MTs(end+1) = str2double(line{1});
    Qs(end+1) = str2double(line{2});
    NE = str2double(line{3});
    energy_cross_section_data = zeros(NE,2);
    
    for i = 1:NE
        line = split(fgetl(fid));
        energy_cross_section_data(i,:) = [str2double(line{1}), str2double(line{2})];
    end
    
    energies{j} = energy_cross_section_data;
    line = fgetl(fid);
end


end

