function q = q_compose( a,b,c,d )
%QUATERNION_COMPOSE Summary of this function goes here
%   Detailed explanation goes here
if nargin==3
q.x=a;
q.y=b;
q.z=c;
end
if nargin==4
q.w=a;
q.x=b;
q.y=c;
q.z=d;
end
end

