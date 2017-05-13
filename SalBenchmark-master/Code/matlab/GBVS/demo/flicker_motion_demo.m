
% some video sequence
i = 1;
for imgi = 0 : 5
    fname{i} = sprintf('F:/git/paper1st/SalBenchmark-master/Data/DataSet3/ImgsGBVS/%08d.jpg',imgi);
    i = i + 1;
end
N = length(fname);

% compute the saliency maps for this sequence

param = makeGBVSParams; % get default GBVS params
param.channels = 'IF';  % but compute only 'I' instensity and 'F' flicker channels
param.levels = 3;       % reduce # of levels for speedup

motinfo = [];           % previous frame information, initialized to empty
for i = 1 : N
    [out{i} motinfo] = gbvs( fname{i}, param , motinfo );
end
ii = 1;
for oimg=0:5
    ofname{ii} = sprintf('F:/git/paper1st/SalBenchmark-master/Data/DataSet3/Saliency/%08d_GBVS.png',oimg);
    imwrite(out{ii}.master_map_resized, ofname{ii});
    ii = ii + 1;
end

% display results
% figure;
% for i = 1 : N
%    subplot(2,N,i);    
%    imshow( imread(fname{i}) );
%    title( fname{i} );
%    subplot(2,N,N+i);
%    imshow( out{i}.master_map_resized );
% end
