function quaternion= create_channel(name,IM1,IM2,rescale)
%�����ĸ�����ͨ������Ԫ�����,
%quternion��һ���ṹ��(w,x,y,z)��ʾ��w+x*i+y*j+z*k
%rescale��ʾ��ͼ���²����Ĵ�С,32*32,64*64,128*128,256*256.
if nargin<=2
    error('wrong parameters.');
end
if nargin==3         %name,IM1,rescal,��ʾ��̬ͼ��
    if strcmp(name,'gray')==1
        error('It must be a color image.');
    end
    if strcmp(name,'color')==1                     %��ɫ��̬ͼ��
        I=imresize(IM1,IM2);
        [m,n,d]=size(I);     %mΪ�У�nΪ��,dΪ��ɫͨ��
        I=im2double(I);
        R=zeros(m,n);G=zeros(m,n);B=zeros(m,n);Y=zeros(m,n); 
        for i=1:m
            for j=1:n
            R(i,j)=I(i,j,1)-(I(i,j,2)+I(i,j,3))/2;
            G(i,j)=I(i,j,2)-(I(i,j,1)+I(i,j,3))/2;
            B(i,j)=I(i,j,3)-(I(i,j,1)+I(i,j,2))/2;
            Y(i,j)=(I(i,j,1)+I(i,j,2))/2-abs(I(i,j,1)-I(i,j,2))/2-I(i,j,3); 
            end
        end
        %�����ǹ����ĸ�ͨ��
        Intensity=zeros(m,n);            %����ͨ��
        Move=zeros(m,n);                   %�˶�ͨ��
        RG=R-G;BY=B-Y;                   %������ɫͨ���Ĺ���
        for i=1:m
            for j=1:n
                Intensity(i,j)=(I(i,j,1)+I(i,j,2)+I(i,j,3))/3;
            end
        end
        %�õ��ĸ�����ͨ����RG,BY,Intensity,Move.
        %����Move(t)+RG(t)*i+BY(t)*j+Intensity(t)*k
        quaternion=q_compose(Move,RG,BY,Intensity);                     %������Ԫ����Ҷ�任
    end
    
else
    if strcmp(name,'gray')==1                %�Ҷȶ�̬ͼ��
       I1=imresize(IM1,rescale);
       I2=imresize(IM2,rescale);
       [m,n]=size(I1);     %mΪ�У�nΪ��
       I1=im2double(I1);I2=im2double(I2);
       RG=zeros(m,n);BY=zeros(m,n);              %����ͼ���ǻҶ�ͼ��,����BG��BY��Ĭ����0.
       %�����ǹ�������ͨ����һ������ͨ����һ���˶�ͨ����
       Intensity=I2;            %����ͨ��
       Move=abs(I2-I1);      %�˶�ͨ��
       %�õ��ĸ�����ͨ����RG,BY,Intensity,Move.
       %����Move(t)+RG(t)*i+BY(t)*j+Intensity(t)*k    
       quaternion=q_compose(Move,RG,BY,Intensity);
    end
    if strcmp(name,'color')==1                     %��ɫ��̬ͼ��
        I1=imresize(IM1,rescale);
        I2=imresize(IM2,rescale);
        [m,n,d]=size(I1);     %mΪ�У�nΪ��,dΪ��ɫͨ��
        I1=im2double(I1);I2=im2double(I2);
        R=zeros(m,n);G=zeros(m,n);B=zeros(m,n);Y=zeros(m,n); 
        for i=1:m
            for j=1:n
                R(i,j)=I2(i,j,1)-(I2(i,j,2)+I2(i,j,3))/2;
                G(i,j)=I2(i,j,2)-(I2(i,j,1)+I2(i,j,3))/2;
                B(i,j)=I2(i,j,3)-(I2(i,j,1)+I2(i,j,2))/2;
                Y(i,j)=(I2(i,j,1)+I2(i,j,2))/2-abs(I2(i,j,1)-I2(i,j,2))/2-I2(i,j,3); 
            end
        end
        %�����ǹ����ĸ�ͨ��
        Intensity=zeros(m,n);            %����ͨ��
        Intensity2=zeros(m,n);
        RG=R-G;BY=B-Y;                   %������ɫͨ���Ĺ���
        for i=1:m
            for j=1:n
                Intensity(i,j)=(I1(i,j,1)+I1(i,j,2)+I1(i,j,3))/3;
                Intensity2(i,j)=(I2(i,j,1)+I2(i,j,2)+I2(i,j,3))/3;
            end
        end
        Move=abs(Intensity-Intensity2);      %�˶�ͨ��
        %�õ��ĸ�����ͨ����RG,BY,Intensity,Move.
        %����Move(t)+RG(t)*i+BY(t)*j+Intensity(t)*k
        quaternion=q_compose(Move,RG,BY,Intensity2);
        
    end
end
end

