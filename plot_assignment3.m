clear all
close all

%% Load H
[symbol, Z, A, AW, T, NNU, nubar_data, MTs, Qs, energy_cross_section_data] = input_reader_matlab('H1.dat.txt');
%% Plot H
figure('Position', [10 10 1200 800])
total_cross_sections = [];

E_range = linspace(log(1e-11),log(20),500);
E_range = exp(E_range);
for E = E_range
    len = length(energy_cross_section_data);
    total_cross_section = 0;
    for i = 1:len
        data = energy_cross_section_data{i};
        total_cross_section  = total_cross_section + cross_section_interpolation(E,data(:,1),data(:,2));
    end
    total_cross_sections(end+1) = total_cross_section;
end

loglog(E_range, total_cross_sections,'LineWidth',2)
set(gca,'FontSize',18)
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

E_range = linspace(log(1e-11),log(20),500);
E_range = exp(E_range);
for E = E_range
    len = length(energy_cross_section_data);
    total_cross_section = 0;
    for i = 1:len
        data = energy_cross_section_data{i};
        total_cross_section  = total_cross_section + cross_section_interpolation(E,data(:,1),data(:,2));
    end
    total_cross_sections(end+1) = total_cross_section;
end

loglog(E_range, total_cross_sections,'LineWidth',2)
set(gca,'FontSize',18)
legend('H', 'O')

%% Load U235
clear all
[symbol, Z, A, AW, T, NNU, nubar_data, MTs, Qs, energy_cross_section_data] = input_reader_matlab('U235.dat.txt');
%% Plot U235
figure('Position', [10 10 1200 800])

fission_cross_sections = [];
radiative_capture_cross_sections = [];

fission_index = find(MTs==18);
radiative_capture_index = find(MTs==102);

E_range = linspace(log(1e-11),log(20),500);
E_range = exp(E_range);
for E = E_range
    
    data = energy_cross_section_data{fission_index};
    fission_cross_sections(end+1) = cross_section_interpolation(E,data(:,1),data(:,2));
    
    data = energy_cross_section_data{radiative_capture_index};
    radiative_capture_cross_sections(end+1) = cross_section_interpolation(E,data(:,1),data(:,2));

end

loglog(E_range, fission_cross_sections,'LineWidth',2)
hold on
loglog(E_range, radiative_capture_cross_sections,'LineWidth',2)
set(gca,'FontSize',18)
xlabel('Neutron energy (MeV)')
ylabel('Cross section (barn)')
grid on
axis tight

%% Load U238
clear all
[symbol, Z, A, AW, T, NNU, nubar_data, MTs, Qs, energy_cross_section_data] = input_reader_matlab('U238.dat.txt');
%% Plot U238
fission_cross_sections = [];
radiative_capture_cross_sections = [];
inelastic_scattering_cross_sections = [];

fission_index = find(MTs==18);
radiative_capture_index = find(MTs==102);
inelastic_scattering_indices = [];
for i = 51:90
   inelastic_scattering_indices = [inelastic_scattering_indices find(MTs==i)];
end

E_range = linspace(log(1e-11),log(20),500);
E_range = exp(E_range);
for E = E_range
    
    data = energy_cross_section_data{fission_index};
    fission_cross_sections(end+1) = cross_section_interpolation(E,data(:,1),data(:,2));
    
    data = energy_cross_section_data{radiative_capture_index};
    radiative_capture_cross_sections(end+1) = cross_section_interpolation(E,data(:,1),data(:,2));
    
    inelastic_scattering_cross_section = 0;
    
    for i = inelastic_scattering_indices
        data = energy_cross_section_data{i};
        inelastic_scattering_cross_section  = inelastic_scattering_cross_section + cross_section_interpolation(E,data(:,1),data(:,2));
    end
    inelastic_scattering_cross_sections(end+1) = inelastic_scattering_cross_section;

end

loglog(E_range, fission_cross_sections,'LineWidth',2)
loglog(E_range, radiative_capture_cross_sections,'LineWidth',2)
set(gca,'FontSize',18)
legend('^{235}U (n,f)', '^{235}U (n,\gamma)','^{238}U (n,f)', '^{238}U (n,\gamma)')

figure('Position', [10 10 1200 800])
loglog(E_range, inelastic_scattering_cross_sections,'LineWidth',2)
xlabel('Neutron energy (MeV)')
ylabel('Microscopic inelastic scattering cross section (barn)')
grid on
axis tight
legend('^{238}U')
set(gca,'FontSize',18)

%% 4) - Macroscopic cross sections
% I'm not quite sure about the nuclide density. I'm going to estimate it
% based on the molar mass and density found on wikipedia

clear all
figure('Position', [10 10 1200 800])

water_molar_mass = 18.01528;
water_density = 1000e3;
uranium_molar_mass = 0.0072*235.04393  + 0.9928*238.02891;
uranium_density = 19050e3;

mol = 6.02214076e23;

% The following will be in units of 1/m^3
water_molecule_density = (water_density/water_molar_mass)*mol;
uranium_atom_density = (uranium_density/uranium_molar_mass)*mol;

H_dens = 2 * water_molecule_density;
O_dens = water_molecule_density;
U235_dens = 0.0072 * uranium_atom_density;
U238_dens = 0.9928 * uranium_atom_density;

% Remember to convert cross_sections to units of 1/m^2 later

[O_symbol, O_Z, O_A, O_AW, O_T, O_NNU, O_nubar_data, O_MTs, O_Qs, O_energy_cross_section_data] = input_reader_matlab('O16.dat.txt');
[H_symbol, H_Z, H_A, H_AW, H_T, H_NNU, H_nubar_data, H_MTs, H_Qs, H_energy_cross_section_data] = input_reader_matlab('H1.dat.txt');
[U235_symbol, U235_Z, U235_A, U235_AW, U235_T, U235_NNU, U235_nubar_data, U235_MTs, U235_Qs, U235_energy_cross_section_data] = input_reader_matlab('U235.dat.txt');
[U238_symbol, U238_Z, U238_A, U238_AW, U238_T, U238_NNU, U238_nubar_data, U238_MTs, U238_Qs, U238_energy_cross_section_data] = input_reader_matlab('U238.dat.txt');

H_total_macroscopic_cross_sections = [];
O_total_macroscopic_cross_sections = [];

E_range = linspace(log(1e-11),log(20),500);
E_range = exp(E_range);
for E = E_range
    len = length(H_energy_cross_section_data);
    H_total_cross_section = 0;
    for i = 1:len
        H_data = H_energy_cross_section_data{i};
        H_total_cross_section  = H_total_cross_section + cross_section_interpolation(E,H_data(:,1),H_data(:,2));
    end
    H_total_macroscopic_cross_sections(end+1) = H_total_cross_section*(1e-28)*H_dens;
    
    len = length(O_energy_cross_section_data);
    O_total_cross_section = 0;
    for i = 1:len
        O_data = O_energy_cross_section_data{i};
        O_total_cross_section  = O_total_cross_section + cross_section_interpolation(E,O_data(:,1),O_data(:,2));
    end
    O_total_macroscopic_cross_sections(end+1) = O_total_cross_section*(1e-28)*O_dens;
end

loglog(E_range,H_total_macroscopic_cross_sections + O_total_macroscopic_cross_sections,'LineWidth',2)
hold on

xlabel('Neutron energy (MeV)')
ylabel('Macroscopic total cross section (1/m)')
grid on
axis tight
legend('Water')


U_total_macroscopic_cross_sections = [];
for E = E_range
    len = length(U235_energy_cross_section_data);
    U235_total_cross_section = 0;
    for i = 1:len
        U_data = U235_energy_cross_section_data{i};
        U235_total_cross_section  = U235_total_cross_section + cross_section_interpolation(E,U_data(:,1),U_data(:,2));
    end
    
    len = length(U238_energy_cross_section_data);
    U238_total_cross_section = 0;
    for i = 1:len
        U_data = U238_energy_cross_section_data{i};
        U238_total_cross_section  = U238_total_cross_section + cross_section_interpolation(E,U_data(:,1),U_data(:,2));
    end
    U_total_macroscopic_cross_sections(end+1) = U235_total_cross_section*(1e-28)*U235_dens + U238_total_cross_section*(1e-28)*U238_dens;
end

loglog(E_range,U_total_macroscopic_cross_sections,'LineWidth',2)
legend('Water','Uranium')
set(gca,'FontSize',18)