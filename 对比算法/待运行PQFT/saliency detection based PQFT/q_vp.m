function c = q_vp( a,b )
%��Ԫ����������,a��b���Ǵ���Ԫ��.
c = q_compose(0,a.y .* b.z - a.z .* b.y, ...
                  a.z .* b.x - a.x .* b.z, ...
                  a.x .* b.y - a.y .* b.x);
end

