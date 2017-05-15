function q = q_unit( a )
%求单位四元数.
c=q_abs(a);
q=q_compose(a.w./c,a.x./c,a.y./c,a.z./c);

end

