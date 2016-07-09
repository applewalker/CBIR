#include <StdHeader.h>
void ValueToDb(valueHSV src){
    QSqlQuery query;
    valueHSV a;

    bool s=query.exec("SELECT id,h1,h2,h3,s1,s2,s3,v1,v2,v3 FROM Color");

    if(s)
        qDebug("Color select success");

    while(query.next()){
        int id = query.value(0).toInt();
        qDebug()<<id;
        for(int i=0;i<3;i++){
            a.h[i]=query.value(i+1).toDouble();
            a.s[i]=query.value(i+4).toDouble();
            a.v[i]=query.value(i+7).toDouble();
        }
        double temp=0;
        double delta[3]={2,1,1};
        for(int i=0;i<3;i++){
            temp += pow((a.h[i]-src.h[i]),2)*delta[i]
                    +pow((a.s[i]-src.s[i]),2)
                    +pow((a.v[i]-src.v[i]),2);
        }
        double tp=sqrt(temp);
        qDebug()<<"a:"<<a.h[0]<<a.h[1]<<a.h[2]<<a.s[0]<<a.s[1]<<a.s[2]<<a.v[0]<<a.v[1]<<a.v[2];
        qDebug()<<"src:"<<src.h[0]<<src.h[1]<<src.h[2]<<src.s[0]<<src.s[1]<<src.s[2]<<src.v[0]<<src.v[1]<<src.v[2];
        qDebug()<<"temp="<<temp;
        QSqlQuery temp_query;
        temp_query.prepare("UPDATE Color SET disHSV = :disHSV  WHERE id = :id");
        temp_query.bindValue(":disHSV",tp);
        //query.bindValue(":disGLCM",tp2);
        temp_query.bindValue(":id",id);
        bool ok=temp_query.exec();
        if(ok)
            qDebug("Color update success");
        else
            qDebug("Color update false");
    }

}
void ValueToDb(valueGLCM src2){
    QSqlQuery query2;
    valueGLCM b;
    bool s2=query2.exec("SELECT id,entropy1,energy1,contrast1,correlation1,entropy2,energy2,contrast2,correlation2 FROM Texture");
    if(s2)
        qDebug("Texture select success");
    while(query2.next()){
        int id = query2.value(0).toInt();
        qDebug()<<id;
        for(int i=0;i<4;i++){
            b.meansrcGRAY[i]=query2.value(i+1).toDouble();
            b.SDsrcGRAY[i]=query2.value(i+5).toDouble();
        }
        double temp=0;
        for(int i=0;i<4;i++){
            temp +=pow((b.meansrcGRAY[i]-src2.meansrcGRAY[i])/src2.meansrcGRAY[i],2)
                    +pow((b.SDsrcGRAY[i]-src2.SDsrcGRAY[i])/src2.meansrcGRAY[i],2);
        }
        double tp=sqrt(temp);
        QSqlQuery temp_query;
        temp_query.prepare("UPDATE Texture SET disGLCM = :disGLCM  WHERE id = :id");
        temp_query.bindValue(":disGLCM",tp);
        temp_query.bindValue(":id",id);
        bool ok=temp_query.exec();
        if(ok)
            qDebug("Texture update success");
        else
            qDebug("Texture update false");
    }
}
void ValueToDb(valueHME src3){
    QSqlQuery query3;
    valueHME c;
    bool s3=query3.exec("SELECT id,hu1,hu2,hu3,hu4,hu5,hu6,hu7,e FROM Shape");
    if(s3)
        qDebug("Shape select success");
    while(query3.next()){
        int id = query3.value(0).toInt();
        qDebug()<<id;
        for(int i=0;i<7;i++){
            c.M[i]=query3.value(i+1).toDouble();
        }
        c.e=query3.value(8).toDouble();
        double temp=0;
        double delta[8]={1,1,1,1,1,1,1,1};
        for(int i=0;i<7;i++){
            temp +=pow(fabs(c.M[i]-src3.M[i])/fabs(src3.M[i]),2)*delta[i];
        }
        temp +=pow(fabs(c.e-src3.e)/fabs(src3.e),2)*delta[7];
        double tp=sqrt(temp);
        QSqlQuery temp_query;
        temp_query.prepare("UPDATE Shape SET disHME= :disHME  WHERE id = :id");
        temp_query.bindValue(":disHME",tp);
        temp_query.bindValue(":id",id);
        bool ok=temp_query.exec();
        if(ok)
            qDebug("Shape update success");
        else
            qDebug("Shape update false");
    }
}
