function out = getImage( image )
%GETIMAGE Summary of this function goes here
%   Detailed explanation goes here
[men_m,men_n,men_d]=size(image);
quaternion=create_channel('color',image,[64,64]);
out=getSMap(quaternion,[men_m,men_n]);
end

