function out = detection( imbw ,I )
%ssM��ʾ����ͼ��IM2��ʾԭʼͼ��
    [m,n]=size(imbw);
    for i=2:m-1
        for j=2:n-1
             Matrix=get3m3(imbw,[i j]);
             avg=sum(Matrix(:));
             if avg>2 && avg<9
                 I(i,j,:)=255;
             end
        end
    end
    out=I;

end

