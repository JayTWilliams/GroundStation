clear
close all
clc

%% data input

T = readtable("S11-032124-RI.CSV", "VariableNamingRule", "preserve");
T = T(3:end, :);
T = renamevars(T, ["# Channel 1", "Var2", "Var3"], ["f", "R", "I"]);
D = table2array(T);

freq = D(:,1);
S11 = D(:,2) + 1j* D(:,3);

%% processing

Z11 = reshape(s2z(S11, 50), [201 1]);
mag = abs(Z11);
phase = angle(Z11);

%% figures

NOAA = 137.62;
CubeSat = 437.5;

figure

subplot(3,1,1);
plot(freq/1e6, mag);
xline(CubeSat, '-', {"CubeSat","Band"});
xline(NOAA, '-', {"NOAA","Band"});
yline(50, '-', {"Matched Impedance"})
xticks([NOAA, CubeSat])
yticks([0 50 100])
ylim([0 100])
ylabel("Z11 (Ohms)")
xlabel("frequency (MHz)")

subplot(3,1,2);
plot(freq/1e6, 10*log10(abs(S11)));
xline(CubeSat, '-', {"CubeSat","Band"});
xline(NOAA, '-', {"NOAA","Band"});
xticks([NOAA, CubeSat])
ylabel("S11 (dB)")
xlabel("frequency (MHz)")

subplot(3,1,3);
plot(freq/1e6, unwrap(180*phase/pi));
xline(CubeSat, '-', {"CubeSat","Band"});
xline(NOAA, '-', {"NOAA","Band"});
xticks([NOAA, CubeSat])
ylabel("Phase (Degrees)")
xlabel("frequency (MHz)")

figure
plot(freq/1e6, mag);
xlim([400,475])
xline(420, '-', {"Lower Bound"});
xline(450, '-', {"Upper Bound"});
xline(432, '-', {"Ideal"});
yline(50, '-', {"Matched Impedance"})
xticks([400 420 432 450 475])
ylabel("Z11 (Ohms)")
xlabel("frequency (MHz)")

