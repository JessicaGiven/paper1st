function q = q_unit( a )
%��λ��Ԫ��.
c=q_abs(a);
q=q_compose(a.w./c,a.x./c,a.y./c,a.z./c);

end

