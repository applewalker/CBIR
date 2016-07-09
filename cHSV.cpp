#include <StdHeader.h>
int calHSV(Mat temp,double h[],double s[],double v[])
{
    int i,j;
    int tempWidth=temp.cols;
    int tempHeight=temp.rows;
    int N= tempWidth*tempHeight;
    for(int num=0;num<3;num++)
    {
        h[num]=0;
        s[num]=0;
        v[num]=0;
    }
    Vec3b intensity;
    double delta = 1;
    for(int k=0;k<3;k++)
    {
        for(i=0;i<tempWidth;i++)
        {
            for(j=0;j<tempHeight;j++)
            {
                if(k==0)
                {
                    intensity=temp.at<Vec3b>(i,j);
                    h[0] += intensity.val[0]*delta/N;
                    s[0] += intensity.val[1]*delta/N;
                    v[0] += intensity.val[2]*delta/N;
                }
                else if(k==1)
                {
                    intensity=temp.at<Vec3b>(i,j);
                    h[1] += pow((intensity.val[0]*delta-h[0]),2)/N;
                    s[1] += pow((intensity.val[1]*delta-s[0]),2)/N;
                    v[1] += pow((intensity.val[2]*delta-v[0]),2)/N;
                }
                else if(k==2)
                {
                    intensity=temp.at<Vec3b>(i,j);
                    h[2] += pow((intensity.val[0]*delta-h[0]),3)/N;
                    s[2] += pow((intensity.val[1]*delta-s[0]),3)/N;
                    v[2] += pow((intensity.val[2]*delta-v[0]),3)/N;
                }
            }
        }
    }
    h[1]=sqrt(h[1]);
    s[1]=sqrt(s[1]);
    v[1]=sqrt(v[1]);
    h[2]=pow(abs(h[2]),1.0/3);
    s[2]=pow(abs(s[2]),1.0/3);
    v[2]=pow(abs(v[2]),1.0/3);
    for(i=0;i<3;i++)
    {
        if(isnan(h[i]))
            h[i]=0;
        if(isnan(s[i]))
            s[i]=0;
        if(isnan(v[i]))
            v[i]=0;
    }

    return 0;
}
