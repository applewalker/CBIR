#include "StdHeader.h"
bool createConnection()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
    db.setDatabaseName("picpool");
    db.setUserName("root");
    db.setPassword("123");
    bool ok = db.open();
    if(!ok){
        qDebug("datebase error");
        return false;
    }else{
        qDebug("datebase success ");
        return true;
    }
}
string&   replace_all_distinct(string&   str,const   string&   old_value,const   string&   new_value)
{
    for(string::size_type   pos(0);   pos!=string::npos;   pos+=new_value.length())   {
        if(   (pos=str.find(old_value,pos))!=string::npos   )
            str.replace(pos,old_value.length(),new_value);
        else   break;
    }
    return   str;
}
QString PreviewPic(QString file_full,int width,int height)
{
    //生成一个预览图
    QUuid id = QUuid::createUuid();//随机码
    String strId = id.toString().toStdString();
    QFileInfo fi;
    fi = QFileInfo(file_full);
    String file_name = (fi.fileName()).toStdString();
    String full_path=file_full.toStdString();
    Mat image =imread(full_path,IMREAD_COLOR);
    Size dsize = Size(width,height);
    Mat image2 = Mat(dsize,1);
    resize(image, image2,dsize,INTER_AREA);//使用象素关系重采样。当图像缩小时候，该方法可以避免波纹出现。
    String path=CACHEPIC+"//cache_"+strId+file_name;
    imwrite(path,image2);
    return  QString::fromStdString(path);
}


int ReadPiclist(const char* filename,FILE* fp)//读取piclist.txt中所有图片路径，并根据图片路径读取图片
{

    int i=0;
    ifstream file(filename,ios::in);//创建一个输入流对象
    if (!file)//如果输入流创建失败，则报错，程序终止
    {

        return -1;
    }
    string path;//string变量，用以存储读出的路径
    char buf[_MAX_PATH];
    while (fgets(buf,512,fp))//从文件中读出一行的内容，保存在变量path中
    {
      // getline(file,path);
        path=buf;
        if (!path.empty())
        {
//            picpoollist[i].Picmat=imread(path,1);
//            picpoollist[i].Picpath=QString::fromStdString(path);
            qDebug(path.data());
            char fname[_MAX_FNAME];
            _splitpath( path.data(),NULL,NULL,fname,NULL);
            QSqlQuery query;
            bool ok=query.exec("INSERT INTO picture (name,path) VALUES('"+
                       QString::fromStdString(fname)+"','"+QString::fromStdString(path)+"')");
            qDebug()<<ok;
            if(ok)
                i++;
        }
    }
    file.close();
    file.clear();
    return i;
}
void SortTop(int Top[],Picdrop picpoollist[],int picnum)
{
    for(int i=0;i<picnum;i++){
        Top[i]=i;//初始化
    }
    int temp;
    for(int i=0;i<picnum-1;i++){
        for(int j=i+1;j<picnum;j++){
            if(picpoollist[Top[i]].Histvalue<picpoollist[Top[j]].Histvalue)
            {
                temp=Top[i];
                Top[i]=Top[j];
                Top[j]=temp;
            }
        }
    }
}
int isNumber(double d)
{
    return (d==d);
}

double IterateThreshold(Mat src, int nMaxIter)
{
    //图像信息
    IplImage *img;
    img=cvCreateImage(CvSize(src.cols,src.rows),8,1);
    img->imageData=(char*)src.data;

    int height = img->height;
    int width = img->width;
    int step = img->widthStep;
    uchar *data = (uchar*)img->imageData;

    //iDiffRec =0;
    int F[256]={ 0 }; //直方图数组
    double iTotalGray=0;//灰度值和
    double iTotalPixel =0;//像素数和
    byte bt;//某点的像素值

    double iThrehold,iNewThrehold;//阀值、新阀值
    double iMaxGrayValue=0,iMinGrayValue=255;//原图像中的最大灰度值和最小灰度值
    double iMeanGrayValue1,iMeanGrayValue2;

    //获取(i,j)的值，存于直方图数组F
    for(int i=0;i<height;i++)
    {
        for(int j=0;j<width;j++)
        {
            bt = (data[i*step+j]);
            if(bt<iMinGrayValue)
                iMinGrayValue = bt;
            if(bt>iMaxGrayValue)
                iMaxGrayValue = bt;
            F[bt]++;
        }
    }

    iThrehold =0;//
    iNewThrehold = (iMinGrayValue+iMaxGrayValue)/2;//初始阀值
    //iDiffRec = iMaxGrayValue - iMinGrayValue;
    int a;
    for( a=0;(abs(iThrehold-iNewThrehold)>0.1) && a<nMaxIter;a++)//迭代中止条件
    {
        iThrehold = iNewThrehold;
        //小于当前阀值部分的平均灰度值
        for(int i=iMinGrayValue;i<iThrehold;i++)
        {
            iTotalGray += F[i]*i;//F[]存储图像信息
            iTotalPixel += F[i];
        }
        iMeanGrayValue1 = (iTotalGray/iTotalPixel);

        //大于当前阀值部分的平均灰度值
        iTotalPixel =0;
        iTotalGray =0;
        for(int j=iThrehold+1;j<iMaxGrayValue;j++)
        {
            iTotalGray += F[j]*j;//F[]存储图像信息
            iTotalPixel += F[j];
        }
        iMeanGrayValue2 = (iTotalGray/iTotalPixel);
        iNewThrehold = (iMeanGrayValue2+iMeanGrayValue1)/2;        //新阀值

        //iDiffRec = abs(iMeanGrayValue2 - iMeanGrayValue1);

    }
    cvReleaseImage(&img);
    qDebug()<<"threshold completed";
    return iThrehold;
}
double CmpHist(Mat src,Mat tst)
{
    Mat hsv_src,hsv_tst;
    double value;
    cvtColor( src, hsv_src, CV_BGR2HSV );
    cvtColor( tst, hsv_tst, CV_BGR2HSV );
     /// 对hue通道使用30个bin,对saturatoin通道使用32个bin
    int h_bins = 50;
    int s_bins = 60;
    int histSize[] = { h_bins, s_bins };
    // hue的取值范围从0到256, saturation取值范围从0到180
    float h_ranges[] = { 0, 256 };
    float s_ranges[] = { 0, 180 };
    const float* ranges[] = { h_ranges, s_ranges };
    // 使用第0和第1通道
    int channels[] = { 0, 1 };
    /// 直方图
    MatND hist_src;
    MatND hist_tst;

    /// 计算HSV图像的直方图
    calcHist( &hsv_src, 1, channels, Mat(), hist_src, 2, histSize, ranges, true, false );
    normalize( hist_src, hist_src, 0, 1, NORM_MINMAX, -1, Mat() );

    calcHist( &hsv_tst, 1, channels, Mat(), hist_tst, 2, histSize, ranges, true, false );
    normalize( hist_tst, hist_tst, 0, 1, NORM_MINMAX, -1, Mat() );
    value = compareHist( hist_src, hist_tst, 0 );
    return value;
}
valueHSV colorHSV(Mat src)
{
    Mat temp;
    int tempWidth=128;
    int tempHeight=128;

    resize(src,temp,Size(tempWidth,tempHeight),0,0,INTER_AREA);
    cvtColor(temp,temp,COLOR_BGR2HSV);

    valueHSV a;
    calHSV(temp,a.h,a.s,a.v);
/*
//    Vec3b intensity;
//    for(int k=0;k<3;k++)
//    {
//        double hue  =0 ;
//        double saturation =0;
//        double value   =0 ;
//        double delta = 0.1;
//        if(k==0)
//        {
//            for(i=0;i<tempWidth;i++)
//            {
//                for(j=0;j<tempHeight;j++)
//                {
//                intensity=temp.at<Vec3b>(i,j);
//                hue += intensity.val[0]*delta;
//                saturation += intensity.val[1]*delta;
//                value  += intensity.val[2]*delta;
//                }
//            }
//            a.h[0]=hue/(N);
//            a.s[0]=saturation/(N);
//            a.v[0]=value/(N);
//        }
//        else if(k==1)
//        {
//            for(i=0;i<tempWidth;i++)
//            {
//                for(j=0;j<tempHeight;j++)
//                {
//                    intensity=temp.at<Vec3b>(i,j);
//                    hue += pow((intensity.val[0]*delta-a.h[0]),2);
//                    saturation += pow((intensity.val[1]*delta-a.s[0]),2);
//                    value  += pow((intensity.val[2]*delta-a.v[0]),2);
//                }
//            }
//            a.h[1]=sqrt(hue/(N));
//            a.s[1]=sqrt(saturation/(N));
//            a.v[1]=sqrt(value/(N));
//         }
//        else if(k==2)
//        {
//            for(i=0;i<tempWidth;i++)
//            {
//                for(j=0;j<tempHeight;j++)
//                {
//                    intensity=temp.at<Vec3b>(i,j);
//                    hue += pow((intensity.val[0]*delta-a.h[0]),3);
//                    saturation += pow((intensity.val[1]*delta-a.s[0]),3);
//                    value  += pow((intensity.val[2]*delta-a.v[0]),3);
//                }
//            }
//            a.h[2]=pow(abs(hue/(N)),1.0/3);
//            a.s[2]=pow(abs(saturation/(N)),1.0/3);
//            a.v[2]=pow(abs(value/(N)),1.0/3);
//        }
//    }
//    for(i=0;i<3;i++)
//    {
//        if(isnan(a.h[i]))
//            a.h[i]=0;
//        if(isnan(a.s[i]))
//            a.s[i]=0;
//        if(isnan(a.v[i]))
//            a.v[i]=0;
//    }
*/
    qDebug()<<"colorHSV completed";
    return a;
}


valueGLCM grayGLCM(Mat src)
{
    int tempWidth=128;
    int tempHeight=128;
    //int N= tempWidth*tempHeight;
    resize(src,src,Size(tempWidth,tempHeight),0,0,INTER_AREA);
    GLCMofPic src_GRAY;
    valueGLCM a;
    calGLCM(src,0,src_GRAY.zero);
    calGLCM(src,1,src_GRAY.pi_di2);
    calGLCM(src,2,src_GRAY.pi_di4);
    calGLCM(src,3,src_GRAY.pi_di4m3);
    for(int i=0;i<4;i++){
        a.meansrcGRAY[i]=(src_GRAY.zero[i]+src_GRAY.pi_di2[i]+src_GRAY.pi_di4[i]+src_GRAY.pi_di4m3[i])/4;
        a.SDsrcGRAY[i]=sqrt((pow((src_GRAY.zero[i]-a.meansrcGRAY[i]),2)+
                          pow((src_GRAY.pi_di2[i]-a.meansrcGRAY[i]),2)+
                          pow((src_GRAY.pi_di4[i]-a.meansrcGRAY[i]),2)+
                          pow((src_GRAY.pi_di4m3[i]-a.meansrcGRAY[i]),2))/4);

        if(!isNumber(a.meansrcGRAY[i])){
            a.meansrcGRAY[i]=0;
        }
        if(!isNumber(a.SDsrcGRAY[i])){
            a.SDsrcGRAY[i]=0;
        }
    }
    qDebug()<<"garyGLCM completed";
    return a;
}
valueHME shapeHME(Mat src)
{
    valueHME a;
    //图像灰度化
    cvtColor(src,src,COLOR_BGR2GRAY);
    //中值滤波
    medianBlur(src,src,3);
    //锐化
    Mat src_x,src_y;
    Mat abs_src_x,abs_src_y;
    int ddepth=CV_16S;

    Scharr(src,src_x,ddepth,1,0,1,0,BORDER_DEFAULT);
    convertScaleAbs( src_x, abs_src_x );

    Scharr(src,src_y,ddepth,0,1,1,0,BORDER_DEFAULT);
    convertScaleAbs( src_y, abs_src_y );

    Mat temp;
    addWeighted(abs_src_x,0.5,abs_src_y,0.5,0,temp);

    //二值化
    double thresh=IterateThreshold(temp,100);
    threshold(temp,temp,thresh,255,THRESH_BINARY);

    //调整大小
    int tempWidth=128;
    int tempHeight=128;
    resize(temp,temp,Size(tempWidth,tempHeight),0,0,INTER_AREA);

    //计算Hu不变矩阵和离心率
    calHME(temp,a.M,a.e);
    qDebug()<<"shapeHME completed";
    return a;
}
