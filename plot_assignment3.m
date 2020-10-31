clear all
close all

%% Load H
[symbol, Z, A, AW, T, NNU, nubar_data, MTs, Qs, energy_cross_section_data] = input_reader_matlab('H1.dat.txt');


%% Test
E = 1;
data = energy_cross_section_data{1};
%% Plot H
figure
total_cross_sections = [];

E_range = linspace(1e-11,20,500);
for E = E_range
    len = length(energy_cross_section_data);
    total_cross_section = 0;
    for i = 1:len
        data = energy_cross_section_data{i};
        total_cross_section  = total_cross_section + cross_section_interpolation(E,data(:,1),data(:,2));
    end
    total_cross_sections(end+1) = total_cross_section;
end

loglog(E_range, total_cross_sections)

hold on
xlabel('Neutron energy (MeV)')
ylabel('Total microscopic cross section (barn)')
grid on
axis tight

%% Load O
clear all
[symbol, Z, A, AW, T, NNU, nubar_data, MTs, Qs, energy_cross_section_data] = input_reader_matlab('O16.dat.txt');

%% Plot O
total_cross_sections = [];
E_range = linspace(1e-11,20,500);
for E = E_range
    len = length(energy_cross_section_data);
    total_cross_section = 0;
    for i = 1:len
        data = energy_cross_section_data{i};
        total_cross_section  = total_cross_section + cross_section_interpolation(E,data(:,1),data(:,2));
    end
    total_cross_sections(end+1) = total_cross_section;
end

loglog(E_range, total_cross_sections)

legend('H', 'O')