#include "mainwindow.h"
#include <QApplication>
#include "stdio.h"
#include <QTextCodec>
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    // 以下部分解决中文乱码
    QTextCodec *codec = QTextCodec::codecForName("UTF-8"); //GBK gbk
    QTextCodec::setCodecForTr(codec);
    QTextCodec::setCodecForLocale(QTextCodec::codecForLocale());
    QTextCodec::setCodecForCStrings(QTextCodec::codecForLocale());
    // 以上部分解决中文乱码
    QString cachedir=QString::fromStdString(CACHEPIC);
    QDir *temp = new QDir;
    bool exist = temp->exists(cachedir);
    if(exist)
        qDebug("exist");
    else
    {
        bool ok = temp->mkdir(cachedir);
        if( ok )
            qDebug("cachedir ok");
    }
    // 加载qss文件
    QFile file(":/qss/metro.qss");
    file.open(QFile::ReadOnly);
    QString styleSheet = QLatin1String(file.readAll());
    qApp->setStyleSheet(styleSheet);
    InitWindows *init = new InitWindows;
    createConnection();
    MainWindow w;
    w.show();


    return a.exec();
}
