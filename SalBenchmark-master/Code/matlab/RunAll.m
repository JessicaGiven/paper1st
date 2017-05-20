clear; close all; clc;
RootDir = '../../../Data/';
SubNames = {'DataSet3/'};%{'DataSet1/' , 'DataSet2/'};

% MethodNames = {'CA', 'COV', 'DSR', 'FES', 'GR', 'ICVS', 'MC', 'PCA', 'RBD', 'SEG', 'SeR', 'SIM', 'SR', 'SUN', 'SWD', 'GBVS', 'SSEG', 'FOGR'};
% You can chose these methods to produce SaliencyMap as follow.

MethodNames = {'GBVS', 'FOGR', 'PQFT'};%{'PCA','SWD'};

RunDatasets(SubNames, MethodNames, RootDir);
