#ifndef PICTUREFUNC_H
#define PICTUREFUNC_H

#include <StdHeader.h>

using namespace cv;
using namespace std;
static String CACHEPIC="D:\\Qt\\cachepic";//在头文件中定义一个静态全局变量。那么所有引用该头文件的C文件，
                                          //均拥有一个作用域在本文件范围内的同名静态全局变量。
                                          //不同文件中的该变量虽然同名，但却是不同的变量。
typedef struct Picdrop{
    Mat Picmat;
    QString Picpath;
    double Histvalue;
}Picdrop;
typedef struct PicItem{
    QString path;
    QString name;
}PicItem;
typedef struct valueHSV{
    double h[3];
    double s[3];
    double v[3];
}valueHSV;
typedef struct valueGLCM{
    double meansrcGRAY[4];//熵,能量,对比度,一致性的均值
    double SDsrcGRAY[4];//熵,能量,对比度,一致性的标准差
}valueGLCM;
typedef struct GLCMofPic{
    double zero[4];//0度
    double pi_di4[4];//45度
    double pi_di2[4];//90度
    double pi_di4m3[4];//135度
}GLCMofPic;
typedef struct valueHME{
    double M[7];//Hu不变矩
    double e;//图像的离心率
}valueHME;
#define MAXNUM 500
bool createConnection();//连接数据库

string& replace_all_distinct(string& str,const string& old_value,const string& new_value);//替换字符

QString PreviewPic(QString file_full,int width,int height);//生成预览图片

void CreatPiclist(QString picpooldir,FILE *fp);//创建一个txt,记录本文件所有图片路径,并计算各个特征值存储到数据库

int ReadPiclist(const char* filename,FILE* fp);//读取piclist.txt中所有图片路径，并根据图片路径读取图片

void SortTop(int Top[],Picdrop picpoollist[],int picnum);//排序

int isNumber(double d);//来判断d是否为nan，若d是nan则返回0，否则返回非零值。

double CmpHist(Mat src,Mat tst);//对比两张图片的直方图

void ValueToDb(valueHSV src);//计算待检索图像颜色特征和图像特征库间的距离

void ValueToDb(valueGLCM src2);//计算待检索图像纹理特征和图像特征库间的距离

void ValueToDb(valueHME src3);//计算待检索图像形状特征和图像特征库间的距离

int calHSV(Mat temp,double h[],double s[],double v[]);//计算H,S,V的前三阶中心距的算法

valueHSV colorHSV(Mat src);//对图像预处理,调用calHSV算法，返回valueHSV

int calGLCM(Mat src,int angleDirection,double* featureVector);//计算0/45/90/135度的灰度共生矩阵，并以此计算出纹理特征的算法

valueGLCM grayGLCM(Mat src);//对图像预处理,调用calGLCM算法,返回valueGLCM

double IterateThreshold(Mat src, int nMaxIter);//迭代法阀值分割

int calHME(Mat src,double M[],double &e);//计算Hu不变矩和离心率的算法

valueHME shapeHME(Mat src);//对图像预处理,调用calHME算法,返回valueHu
#endif // PICTUREFUNC_H
