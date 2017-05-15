function out = getSMap( quaternion,original_scalse )
%SALIENCYMAP Summary of this function goes here
%   Detailed explanation goes here
    out=q_fft2(quaternion);
    phase=q_compose(out.w./q_abs(out),out.x./q_abs(out),out.y./q_abs(out),out.z./q_abs(out));
    IX=q_ifft2(phase); 
    sM=(q_abs(IX)).^2;  
    %gau=creatgauss(5,4);                                %将sM进行高斯平滑
    gau=fspecial('gaussian',[5,5],8);
    %sM=gaufilter(gau,sM);
    sM=conv2(sM,gau,'same');

    sM=mat2gray(sM);
    out=imresize(sM,original_scalse);
end

