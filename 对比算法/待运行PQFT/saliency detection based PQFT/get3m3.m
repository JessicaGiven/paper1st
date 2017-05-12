function out = get3m3( M , location )
%GET3M3 Summary of this function goes here
%   Detailed explanation goes here
output=zeros(3);
x=location(1);
y=location(2);
for i=-1:1
    for j=-1:1
        output(i+2,j+2)=M(x+i,y+j);
    end
end
out=output;
end

