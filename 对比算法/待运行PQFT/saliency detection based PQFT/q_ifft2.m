function Q = q_ifft2(q)
%Ĭ������任.
%aΪscalar,����.
%b,c,d��vector,����.
%a,b,c,d��Ϊʵ��.
A=q_compose(0,1,1,1);   %Ĭ����i+j+k����
A=q_unit(A);
B = q_orthonormal_basis(A);
X = q_change_basis(q, B);
C1=ifft2(complex(X.w,X.x));
C2=ifft2(complex(X.y,X.z));
Q=q_compose(real(C1),imag(C1),real(C2),imag(C2));
Q= q_change_basis(Q, B.');
%[A,B,C,D]�Ǳ任�����Ԫ��.
end


