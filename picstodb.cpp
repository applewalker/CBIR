#include <StdHeader.h>
void CreatPiclist(QString picpooldir,FILE *fp)
{
    std::string path=picpooldir.toStdString();
    struct _finddata_t fa;
    long fileHandle;
    qDebug(path.data());
    if((fileHandle = _findfirst((path+"\\*").data(),&fa)) == -1L)
    {
        //没有文件
    }
    else{
        do{
            if(fa.attrib & _A_SUBDIR)//是目录
            {
                if( (strcmp(fa.name,".") != 0 ) &&(strcmp(fa.name,"..") != 0))
                {
                    QString newPath = QString::fromStdString(path + "\\" + fa.name);
                    CreatPiclist(newPath,fp);
                }
            }
            else{
                char ext[6];
                _splitpath(fa.name,NULL,NULL,NULL,ext);
                if( strcmp(ext,".bmp")==0 || strcmp(ext,".BMP")==0 ||
                        strcmp(ext,".jpg")==0 || strcmp(ext,".JPG")==0 ||
                        strcmp(ext,".png")==0)
                {
                    //printf( "找到%s\n", fa.name );
                    string temp=path+"\\"+fa.name;
                    qDebug()<<QString::fromStdString(fa.name);
                    qDebug()<<QString::fromStdString(temp);
                    Mat m=imread(temp,IMREAD_COLOR);
                    m.convertTo(m,CV_8UC3);
                    valueHSV  a=colorHSV(m);
                    valueGLCM b=grayGLCM(m);
                    valueHME  c=shapeHME(m);
                    //temp=replace_all_distinct(temp,"\\","\\\\");
                    QSqlQuery query;
                    QSqlQuery query2;
                    QSqlQuery query3;
                    query.prepare("INSERT INTO Color (name,path,h1,h2,h3,s1,s2,s3,v1,v2,v3)"
                                  //"entropy1,energy1,contrast1,homogenity1,entropy2,energy2,contrast2,homogenity2)"
                                  "VALUES(:name,:path,"
                                  ":h1,:h2,:h3,:s1,:s2,:s3,:v1,:v2,:v3)");
                    //":entropy1,:energy1,:contrast1,:homogenity1,:entropy2,:energy2,:contrast2,:homogenity2)");
                    //QString::fromStdString(fa.name)+"','"+QString::fromStdString(temp)+"',"+a.h1+","+a.h2+","+a.h3+","+a.s1+","+a.s2+","+a.s3+","+a.v1+","+a.v2+","+a.v3+")");
                    query2.prepare("INSERT INTO Texture (name,path,"
                                   "entropy1,energy1,contrast1,correlation1,entropy2,energy2,contrast2,correlation2)"
                                   "VALUES(:name,:path,"
                                   ":entropy1,:energy1,:contrast1,:correlation1,:entropy2,:energy2,:contrast2,:correlation2)");
                    query3.prepare("INSERT INTO Shape (name,path,hu1,hu2,hu3,hu4,hu5,hu6,hu7,e)"
                                   "VALUES(:name,:path,:hu1,:hu2,:hu3,:hu4,:hu5,:hu6,:hu7,:e)");

                    query.bindValue(":name",QString::fromStdString(fa.name));
                    query.bindValue(":path",QString::fromStdString(temp));
                    for(int i=0;i<3;i++)
                    {
                        query.bindValue(i+2,a.h[i]);
                        query.bindValue(i+5,a.s[i]);
                        query.bindValue(i+8,a.v[i]);
                    }
                    query2.bindValue(":name",QString::fromStdString(fa.name));
                    query2.bindValue(":path",QString::fromStdString(temp));

                    for(int i=0;i<4;i++)
                    {
                        query2.bindValue(i+2,b.meansrcGRAY[i]);
                        query2.bindValue(i+6,b.SDsrcGRAY[i]);
                    }

                    query3.bindValue(":name",QString::fromStdString(fa.name));
                    query3.bindValue(":path",QString::fromStdString(temp));
                    for(int i=0;i<7;i++)
                    {
                        query3.bindValue(i+2,c.M[i]);
                    }
                    query3.bindValue(":e",c.e);

                    //                    query.bindValue(":h1",a.h[0]);
                    //                    query.bindValue(":h2",a.h[1]);
                    //                    query.bindValue(":h3",a.h[2]);
                    //                    query.bindValue(":s1",a.s[0]);
                    //                    query.bindValue(":s2",a.s[1]);
                    //                    query.bindValue(":s3",a.s[2]);
                    //                    query.bindValue(":v1",a.v[0]);
                    //                    query.bindValue(":v2",a.v[1]);
                    //                    query.bindValue(":v3",a.v[2]);

                    bool ok=query.exec();
                    bool ok2=query2.exec();
                    bool ok3=0;
                    ok3=query3.exec();
                    if(ok)
                        qDebug()<<"Color insert success";
                    else
                        qDebug()<<"Color insert false";
                    if(ok2)
                        qDebug()<<"Texture insert success";
                    else
                        qDebug()<<"Texture insert false";
                    if(ok3)
                        qDebug()<<"Shape insert success";
                    else
                        qDebug()<<"Shape insert false";

                    //                    qDebug()<<a.h[0];
                    //                    qDebug()<<a.h[1];
                    //                    qDebug()<<a.h[2];
                    //                    qDebug()<<a.s[0];
                    //                    qDebug()<<a.s[1];
                    //                    qDebug()<<a.s[2];
                    //                    qDebug()<<a.v[0];
                    //                    qDebug()<<a.v[1];
                    //                    qDebug()<<a.v[2];
                    //                    for(int i=0;i<4;i++){
                    //                        qDebug()<<b.meansrcGRAY[i];
                    //                    }
                    //                    for(int i=0;i<4;i++){
                    //                        qDebug()<<b.SDsrcGRAY[i];
                    //                    }
//                    for(int i=0;i<7;i++)
//                        qDebug()<<c.M[i];
//                    qDebug()<<c.e;
                    fprintf(fp,(temp+"\n").data());
                }
                else
                {
                    //不是图片
                }
            }
        }while (_findnext(fileHandle, &fa) == 0);
    }
}
