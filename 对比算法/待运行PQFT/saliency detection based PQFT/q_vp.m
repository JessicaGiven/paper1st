function c = q_vp( a,b )
%四元数的向量积,a和b都是纯四元数.
c = q_compose(0,a.y .* b.z - a.z .* b.y, ...
                  a.z .* b.x - a.x .* b.z, ...
                  a.x .* b.y - a.y .* b.x);
end

