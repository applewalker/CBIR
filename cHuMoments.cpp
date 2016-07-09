#include <StdHeader.h>

int calHME(Mat src,double M[],double &e)
{
      IplImage *img;
      img=cvCreateImage(CvSize(src.cols,src.rows),8,1);
      img->imageData=(char*)src.data;

      int bmpWidth = img->width;
      int bmpHeight = img->height;
      int bmpStep = img->widthStep;
      int bmpChannels = img->nChannels;
      uchar* pBmpBuf = (uchar*)img->imageData;

     double m00=0,m11=0,m20=0,m02=0,m30=0,m03=0,m12=0,m21=0;  //中心矩
     double x0=0,y0=0;    //计算中心距时所使用的临时变量（x-x'）
     double u20=0,u02=0,u11=0,u30=0,u03=0,u12=0,u21=0;//规范化后的中心矩
     //double M[7];    //HU不变矩
     double t1=0,t2=0,t3=0,t4=0,t5=0;//临时变量，
     //double Center_x=0,Center_y=0;//重心
     int Center_x=0,Center_y=0;//重心
     int i,j;            //循环变量

     //  获得图像的区域重心(普通矩)
     double s10=0,s01=0,s00=0;  //0阶矩和1阶矩
     for(j=0;j<bmpHeight;j++)//y
         {
     for(i=0;i<bmpWidth;i++)//x
             {
                 s10+=i*pBmpBuf[j*bmpStep+i];
                 s01+=j*pBmpBuf[j*bmpStep+i];
                 s00+=pBmpBuf[j*bmpStep+i];
             }
         }
         Center_x=(int)(s10/s00+0.5);
         Center_y=(int)(s01/s00+0.5);

     //  计算二阶、三阶矩(中心矩)
         m00=s00;
     for(j=0;j<bmpHeight;j++)
         {
     for(i=0;i<bmpWidth;i++)//x
             {
                 x0=(i-Center_x);
                 y0=(j-Center_y);
                 m11+=x0*y0*pBmpBuf[j*bmpStep+i];
                 m20+=x0*x0*pBmpBuf[j*bmpStep+i];
                 m02+=y0*y0*pBmpBuf[j*bmpStep+i];
                 m03+=y0*y0*y0*pBmpBuf[j*bmpStep+i];
                 m30+=x0*x0*x0*pBmpBuf[j*bmpStep+i];
                 m12+=x0*y0*y0*pBmpBuf[j*bmpStep+i];
                 m21+=x0*x0*y0*pBmpBuf[j*bmpStep+i];
             }
         }

     //  计算规范化后的中心矩: mij/pow(m00,((i+j+2)/2)
         u20=m20/pow(m00,2);
         u02=m02/pow(m00,2);
         u11=m11/pow(m00,2);
         u30=m30/pow(m00,2.5);
         u03=m03/pow(m00,2.5);
         u12=m12/pow(m00,2.5);
         u21=m21/pow(m00,2.5);

     //  计算中间变量
         t1=(u20-u02);
         t2=(u30-3*u12);
         t3=(3*u21-u03);
         t4=(u30+u12);
         t5=(u21+u03);

     //  计算不变矩
         M[0]=u20+u02;
         M[1]=t1*t1+4*u11*u11;
         M[2]=t2*t2+t3*t3;
         M[3]=t4*t4+t5*t5;
         M[4]=t2*t4*(t4*t4-3*t5*t5)+t3*t5*(3*t4*t4-t5*t5);
         M[5]=t1*(t4*t4-t5*t5)+4*u11*t4*t5;
         M[6]=t3*t4*(t4*t4-3*t5*t5)-t2*t5*(3*t4*t4-t5*t5);
         e=(u20-(pow(u02,2))+4*u11*u11)/(pow((u20+u02),2));
         cvReleaseImage(&img);
    qDebug()<<"calHME completed";
    return 0;
}
