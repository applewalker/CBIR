#include "StdHeader.h"

#define GLCM_DIS 2  //灰度共生矩阵的统计距离
#define GLCM_CLASS 16 //计算灰度共生矩阵的图像灰度值等级化
#define GLCM_ANGLE_HORIZATION 0  //水平
#define GLCM_ANGLE_VERTICAL   1	 //垂直
#define GLCM_ANGLE_DIGONAL    2  //对角
#define GLCM_ANGLE_AGAINST_D  3  //反对角
int calGLCM(Mat src,int angleDirection,double* featureVector)
{
    cvtColor(src,src,COLOR_BGR2GRAY);
    IplImage* bWavelet;
    bWavelet=cvCreateImage(CvSize(src.cols,src.rows),8,1);
    bWavelet->imageData=(char*)src.data;

    int i,j;
    int width,height;

    if(NULL == bWavelet)
        return 1;

    width = bWavelet->width;
    height = bWavelet->height;

    double * glcm = new double[GLCM_CLASS * GLCM_CLASS];
    int * histImage = new int[width * height];
    int maxnumber=0;
    if(NULL == glcm || NULL == histImage)
        return 2;

    //灰度等级化---分GLCM_CLASS个等级
    uchar *data =(uchar*) bWavelet->imageData;
    for(i = 0;i < height;i++){
        for(j = 0;j < width;j++){
            histImage[i * width + j] = (int)(data[bWavelet->widthStep * i + j] * GLCM_CLASS / 256);
        }
    }

    //初始化共生矩阵
    for (i = 0;i < GLCM_CLASS;i++)
        for (j = 0;j < GLCM_CLASS;j++)
            glcm[i * GLCM_CLASS + j] = 0;

    //计算灰度共生矩阵
    int w,k,l;
    //水平方向
    if(angleDirection == GLCM_ANGLE_HORIZATION)
    {
        for (i = 0;i < height;i++)
        {
            for (j = 0;j < width;j++)
            {
                l = histImage[i * width + j];
                if(j + GLCM_DIS >= 0 && j + GLCM_DIS < width)
                {
                    k = histImage[i * width + j + GLCM_DIS];
                    glcm[l * GLCM_CLASS + k]++;
                    maxnumber++;
                }
                if(j - GLCM_DIS >= 0 && j - GLCM_DIS < width)
                {
                    k = histImage[i * width + j - GLCM_DIS];
                    glcm[l * GLCM_CLASS + k]++;
                    maxnumber++;
                }
            }
        }
    }
    //垂直方向
    else if(angleDirection == GLCM_ANGLE_VERTICAL)
    {
        for (i = 0;i < height;i++)
        {
            for (j = 0;j < width;j++)
            {
                l = histImage[i * width + j];
                if(i + GLCM_DIS >= 0 && i + GLCM_DIS < height)
                {
                    k = histImage[(i + GLCM_DIS) * width + j];
                    glcm[l * GLCM_CLASS + k]++;
                    maxnumber++;
                }
                if(i - GLCM_DIS >= 0 && i - GLCM_DIS < height)
                {
                    k = histImage[(i - GLCM_DIS) * width + j];
                    glcm[l * GLCM_CLASS + k]++;
                    maxnumber++;
                }
            }
        }
    }
    //对角方向
    else if(angleDirection == GLCM_ANGLE_DIGONAL)
    {
        for (i = 0;i < height;i++)
        {
            for (j = 0;j < width;j++)
            {
                l = histImage[i * width + j];

                if(j + GLCM_DIS >= 0 && j + GLCM_DIS < width && i + GLCM_DIS >= 0 && i + GLCM_DIS < height)
                {
                    k = histImage[(i + GLCM_DIS) * width + j + GLCM_DIS];
                    glcm[l * GLCM_CLASS + k]++;
                    maxnumber++;
                }
                if(j - GLCM_DIS >= 0 && j - GLCM_DIS < width && i - GLCM_DIS >= 0 && i - GLCM_DIS < height)
                {
                    k = histImage[(i - GLCM_DIS) * width + j - GLCM_DIS];
                    glcm[l * GLCM_CLASS + k]++;
                    maxnumber++;
                }
            }
        }
    }
    //反对角方向
    else if(angleDirection == GLCM_ANGLE_AGAINST_D)
    {
        for (i = height-1;i >=0 ;i--)
        {
            for (j = 0;j < width;j++)
            {
                l = histImage[i * width + j];

                if(j + GLCM_DIS >= 0 && j + GLCM_DIS < width && i - GLCM_DIS >= 0 && i - GLCM_DIS < height)
                {
                    k = histImage[(i - GLCM_DIS) * width + j + GLCM_DIS];
                    glcm[l * GLCM_CLASS + k]++;
                    maxnumber++;
                }
                if(j - GLCM_DIS >= 0 && j - GLCM_DIS < width && i + GLCM_DIS >= 0 && i + GLCM_DIS < height)
                {
                    k = histImage[(i + GLCM_DIS) * width + j - GLCM_DIS];
                    glcm[l * GLCM_CLASS + k]++;
                    maxnumber++;
                }
            }
        }
    }
    // 归一化
    for (i = 0;i < GLCM_CLASS;i++)
    {
        for (j = 0;j < GLCM_CLASS;j++)
        {
            if(maxnumber>0)
            glcm[i * GLCM_CLASS + j] = glcm[i * GLCM_CLASS + j]/maxnumber;
        }
    }

    //计算自相关用到的参数Ux,Uy
    double Ux=0,Uy=0;
    double Sx=0,Sy=0;
    for (i=0;i<GLCM_CLASS;i++)
    {
        for(j=0;j<GLCM_CLASS;j++)
        {
            Ux +=i*glcm[i * GLCM_CLASS + j];
            Uy +=j*glcm[i * GLCM_CLASS + j];
        }
    }
    for (i=0;i<GLCM_CLASS;i++)
    {
        for(j=0;j<GLCM_CLASS;j++)
        {
            Sx +=glcm[i * GLCM_CLASS + j]*(i-Ux)*(i-Ux);
            Sy +=glcm[i * GLCM_CLASS + j]*(j-Uy)*(j-Uy);
        }
    }
    Sx=sqrt(Sx);
    Sy=sqrt(Sy);
    //计算特征值
    double entropy = 0,energy = 0,contrast = 0,correlation = 0;//homogenity = 0;
    for (i = 0;i < GLCM_CLASS;i++)
    {
        for (j = 0;j < GLCM_CLASS;j++)
        {
            //熵
            if(glcm[i * GLCM_CLASS + j] > 0)
                entropy -= glcm[i * GLCM_CLASS + j] * log10(double(glcm[i * GLCM_CLASS + j]));
            //能量
            energy += glcm[i * GLCM_CLASS + j] * glcm[i * GLCM_CLASS + j];
            //对比度
            contrast += (i - j) * (i - j) * glcm[i * GLCM_CLASS + j];
            //一致性
            //homogenity += 1.0 / (1 + (i - j) * (i - j)) * glcm[i * GLCM_CLASS + j];
            //自相关性
            correlation +=i*j*glcm[i * GLCM_CLASS + j];
        }
    }
    correlation = (correlation - Ux*Uy)/(Sx*Sy);
    //返回特征值
    i = 0;
    featureVector[i++] = entropy;
    featureVector[i++] = energy;
    featureVector[i++] = contrast;
    featureVector[i++] = correlation;

    delete[] glcm;
    delete[] histImage;
    cvReleaseImage(&bWavelet);//释放内存
    return 0;
}
