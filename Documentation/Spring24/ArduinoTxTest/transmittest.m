clear
close all
clc

%%

fileID = fopen("outputstring6.txt");
Data = fread(fileID, inf, "uchar");
fclose(fileID);

%%

HelloWorld = "0100100001100101011011000110110001101111001000000101011101101111011100100110110001100100";

fprintf(HelloWorld)
split1 = Data(40720 : 40860);
finding = sprintf("%d", transpose(split1));
fprintf("\n" + finding)

%% 

figure
plot(Data)

figure
plot(split1)

%%

%outputstring2.txt successful in first burst
%outputstring3.txt semi successful, has weird spacings
%outputstring4.txt successful, needs proper ratios (1s spaced more than 0s) 
%outputstring5.txt very successful, looks like lower power was better
%outputstring6.txt successful, but decim/interp too tight, needs
%decision making instead