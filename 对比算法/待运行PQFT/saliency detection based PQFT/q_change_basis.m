function P = q_change_basis(Q, B)
%Q_CHANGE_BASIS Summary of this function goes here
%   Detailed explanation goes here
V1 = q_compose(0,B(1,1), B(1,2), B(1,3));
V2 = q_compose(0,B(2,1), B(2,2), B(2,3));
V3 = q_compose(0,B(3,1), B(3,2), B(3,3));

 P = q_compose(Q.w, q_sp(Q, V1),...
                     q_sp(Q, V2),...
                      q_sp(Q, V3));
end



