 
clc

clear

list = textread('list.txt','%s');

for folder_i = 1:size(list)
    % some video sequence
    img_dir = fullfile('F:/git/paper1st/SalBenchmark-master/Data/DataSet3/ImgsGBVS/', list(folder_i), '/');
    img_file = dir(fullfile(img_dir{1}, '*jpg'));
    
%     i = 1;
%     for imgi = 0 : 1
%         fname{i} = sprintf('F:/git/paper1st/SalBenchmark-master/Data/DataSet3/ImgsGBVS/%02d.jpg',imgi);
%         i = i + 1;
%     end
%     N = length(fname);

    % compute the saliency maps for this sequence

    param = makeGBVSParams; % get default GBVS params
    param.channels = 'IF';  % but compute only 'I' instensity and 'F' flicker channels
    param.levels = 3;       % reduce # of levels for speedup

    motinfo = [];           % previous frame information, initialized to empty
    for i = 1 : size(img_file)
        img = imread([img_dir{1}, img_file(i).name]);
        [out{i} motinfo] = gbvs( img, param , motinfo );
    end
%     ii = 1;
%     for oimg=0:1
%         ofname{ii} = sprintf('F:/git/paper1st/SalBenchmark-master/Data/DataSet3/Saliency/%08d_GBVS.png',oimg);
%         imwrite(out{ii}.master_map_resized, ofname{ii});
%         ii = ii + 1;
%     end
    ofname = sprintf('F:/git/paper1st/SalBenchmark-master/Data/DataSet3/Saliency/%08d_GBVS.png',folder_i-1);
    imwrite(out{2}.master_map_resized, ofname);
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
