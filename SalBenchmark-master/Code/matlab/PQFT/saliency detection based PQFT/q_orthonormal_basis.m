function B = q_orthonormal_basis( V )
qunit=q_compose(0,1,1,1);
L = parallel(V, qunit);
    if any(L)
        if L(1)
            W = q_compose(0,1,1,-1);
        elseif L(2)
            W = q_compose(0,-1,1,1);
        else
            W = q_compose(0,1,-1,1);
        end
    else
        W = q_compose(0,1,1,1); 
        if parallel(V, W)
            W =q_compose(0,-1,-1,0);
        end
    end
C = q_vp(V, W);
W = q_unit(C);

X = q_unit(q_vp(V, W));
V = q_unit(V);
B = [V.x, V.y, V.z; W.x, W.y, W.z; X.x, X.y, X.z];

end


function tf = parallel(x, y)
tf = abs(q_abs(q_vp(x, y))) < eps;
end