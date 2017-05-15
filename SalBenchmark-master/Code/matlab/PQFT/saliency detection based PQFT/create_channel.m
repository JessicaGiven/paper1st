function quaternion= create_channel(name,IM1,IM2,rescale)
%创建四个特征通道的四元数表达,
%quternion是一个结构体(w,x,y,z)表示成w+x*i+y*j+z*k
%rescale表示对图像下采样的大小,32*32,64*64,128*128,256*256.
if nargin<=2
    error('wrong parameters.');
end
if nargin==3         %name,IM1,rescal,表示静态图像
    if strcmp(name,'gray')==1
        error('It must be a color image.');
    end
    if strcmp(name,'color')==1                     %彩色静态图像
        I=imresize(IM1,IM2);
        [m,n,d]=size(I);     %m为行，n为列,d为三色通道
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
        %下面是构建四个通道
        Intensity=zeros(m,n);            %亮度通道
        Move=zeros(m,n);                   %运动通道
        RG=R-G;BY=B-Y;                   %两个颜色通道的构建
        for i=1:m
            for j=1:n
                Intensity(i,j)=(I(i,j,1)+I(i,j,2)+I(i,j,3))/3;
            end
        end
        %得到四个特征通道，RG,BY,Intensity,Move.
        %存在Move(t)+RG(t)*i+BY(t)*j+Intensity(t)*k
        quaternion=q_compose(Move,RG,BY,Intensity);                     %进行四元傅里叶变换
    end
    
else
    if strcmp(name,'gray')==1                %灰度动态图像
       I1=imresize(IM1,rescale);
       I2=imresize(IM2,rescale);
       [m,n]=size(I1);     %m为行，n为列
       I1=im2double(I1);I2=im2double(I2);
       RG=zeros(m,n);BY=zeros(m,n);              %由于图像是灰度图像,其中BG和BY都默认是0.
       %下面是构建两个通道（一个亮度通道和一个运动通道）
       Intensity=I2;            %亮度通道
       Move=abs(I2-I1);      %运动通道
       %得到四个特征通道，RG,BY,Intensity,Move.
       %存在Move(t)+RG(t)*i+BY(t)*j+Intensity(t)*k    
       quaternion=q_compose(Move,RG,BY,Intensity);
    end
    if strcmp(name,'color')==1                     %彩色动态图像
        I1=imresize(IM1,rescale);
        I2=imresize(IM2,rescale);
        [m,n,d]=size(I1);     %m为行，n为列,d为三色通道
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
        %下面是构建四个通道
        Intensity=zeros(m,n);            %亮度通道
        Intensity2=zeros(m,n);
        RG=R-G;BY=B-Y;                   %两个颜色通道的构建
        for i=1:m
            for j=1:n
                Intensity(i,j)=(I1(i,j,1)+I1(i,j,2)+I1(i,j,3))/3;
                Intensity2(i,j)=(I2(i,j,1)+I2(i,j,2)+I2(i,j,3))/3;
            end
        end
        Move=abs(Intensity-Intensity2);      %运动通道
        %得到四个特征通道，RG,BY,Intensity,Move.
        %存在Move(t)+RG(t)*i+BY(t)*j+Intensity(t)*k
        quaternion=q_compose(Move,RG,BY,Intensity2);
        
    end
end
end

