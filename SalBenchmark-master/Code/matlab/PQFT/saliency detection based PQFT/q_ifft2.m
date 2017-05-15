function Q = q_ifft2(q)
%默认是左变换.
%a为scalar,标量.
%b,c,d是vector,向量.
%a,b,c,d均为实数.
A=q_compose(0,1,1,1);   %默认是i+j+k的轴
A=q_unit(A);
B = q_orthonormal_basis(A);
X = q_change_basis(q, B);
C1=ifft2(complex(X.w,X.x));
C2=ifft2(complex(X.y,X.z));
Q=q_compose(real(C1),imag(C1),real(C2),imag(C2));
Q= q_change_basis(Q, B.');
%[A,B,C,D]是变换后的四元数.
end


