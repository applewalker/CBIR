#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{

    ui->setupUi(this);
    this->setWindowTitle(tr("CBIR"));
    //ui->jump2button->setEnabled(false);
    this->locatRect = this->geometry();
    QLabel *title=new QLabel();
    title->setObjectName("titlelabel");
    title->setText(tr("基于内容的图像检索系统"));
    ui->statusBar->addWidget(title);
    ui->statusBar->setStyleSheet(QString("QStatusBar::item{border: 0px}"));
    this->setProperty("ablemove", true);
    //this->setWindowTitle(ui->lab_Title->text());
    ui->menu->installEventFilter(this);
    //设置窗体标题栏隐藏
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowSystemMenuHint | Qt::WindowMinimizeButtonHint);
    ui->menuBar->setAccessibleDescription("abc");
    connect(this,SIGNAL(PostTopPic(vector<PicItem>)),&s,SLOT(ReceivePicPath(vector<PicItem>)));
    connect(this,SIGNAL(PostTopPic2(vector<PicItem>)),&s2,SLOT(ReceivePicPath(vector<PicItem>)));
    connect(this,SIGNAL(PostTopPic3(vector<PicItem>)),&s3,SLOT(ReceivePicPath(vector<PicItem>)));
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::b(){
    //this->setWindowFlags(Qt::WindowStaysOnTopHint);
    //this->setWindowFlags(NULL);
    this->show();
}
void MainWindow::ShowPic(int type,SecondWindow *s)
{
    if(!ui->SrcPicPath->text().isEmpty()){
    connect(s,SIGNAL(back()), this, SLOT(b()));
    //connect(this,SIGNAL(jump(QString)),s,SLOT(ReceivePool(QString)));

    s->move((QApplication::desktop()->width()-s->width())/2,(QApplication::desktop()->height()-s->height())/2);
    Top12Pic.clear();
    QSqlQuery query;
    string model="SELECT name,path FROM #table# ORDER BY #type# LIMIT 12";
    string a;
    s->setWindowTitle(tr("检索结果"));
    if (type == 1)
    {
        ValueToDb(src_HSV);
        s->title(tr("基于颜色特征检索"));
        a=replace_all_distinct(model,"#type#","disHSV");
        a=replace_all_distinct(model,"#table#","Color");

    }
    else if(type == 2)
    {
        ValueToDb(src_GLCM);
        s->title(tr("基于纹理特征检索"));
        a=replace_all_distinct(model,"#type#","disGLCM");
        a=replace_all_distinct(model,"#table#","Texture");

    }
    else if(type == 3)
    {
        ValueToDb(src_HME);
        s->title(tr("基于形状特征检索"));
        a=replace_all_distinct(model,"#type#","disHME");
        a=replace_all_distinct(model,"#table#","Shape");
    }
    query.exec(QString::fromStdString(a));
    while(query.next()){
        PicItem temp;
        temp.name=query.value(0).toString();
        temp.path=query.value(1).toString();
        Top12Pic.push_back(temp);
    }
    s->setWindowFlags(Qt::WindowStaysOnTopHint);
    s->setWindowFlags(NULL);
    s->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowSystemMenuHint | Qt::WindowMinimizeButtonHint);
    s->show();
    if (type == 1)
    {
        emit PostTopPic(Top12Pic);       
    }
    else if(type == 2)
    {
        emit PostTopPic2(Top12Pic);
    }
    else if(type == 3)
    {
        emit PostTopPic3(Top12Pic);
    }

   }
    else{
        QMessageBox::warning(s,tr("警告"),tr("请选择图片！"));
    }
}
void MainWindow::on_ChoosePicButton_clicked()
{
    //QString file_name, file_path;
    file_full = QFileDialog::getOpenFileName(this, tr("选择图片"),".\\",
                                              tr("Images (*.png *.bmp *.jpg)"));
    //QFileInfo fi;
    //fi = QFileInfo(file_full);
    //file_name = fi.fileName();
    //file_path = fi.absolutePath();
    if(!file_full.isEmpty())
    {
        ui->SrcPicPath->setText(QString(file_full));
        Mat src=imread(file_full.toStdString(),IMREAD_COLOR);

        qDebug()<<(file_full);

        src_HSV  =colorHSV(src);//颜色特征
        // qDebug()<<src_HSV.h[1];
        src_GLCM =grayGLCM(src);//纹理特征

        src_HME  =shapeHME(src);//形状特征


        for(int i=0;i<7;i++)
            qDebug()<<src_HME.M[i];
        qDebug()<<src_HME.e;

        QImage* img=new QImage;
        QString path=PreviewPic(file_full,ui->ShowLabel->width(),ui->ShowLabel->height());
        if(!( img->load(path) ) ) //加载图像
        {   ui->ShowLabel->setText("图片无法显示");
            delete img;
        }
        else
        {         
            //*img=img->scaled(QSize(ui->ShowLabel->width(),ui->ShowLabel->height()));
            ui->ShowLabel->setPixmap(QPixmap::fromImage(*img));
            //srcpic.Picmat=imread(file_full.toStdString());
            //QPixmap::fromImage(img->scaled(QSize(ui->ShowLabel->width(),ui->ShowLabel->height())))
        }
    }
     CheckForJump();
}



void MainWindow::on_ChoosePoolButton_clicked()
{
   dir = QFileDialog::getExistingDirectory(this, tr("选择图库"),".\\",QFileDialog::ShowDirsOnly
                                           | QFileDialog::DontResolveSymlinks);
   //qDebug(dir.toAscii().data());
    if(!dir.isEmpty()){
        ui->PicPoolPath->setText(dir);
        string filepath=dir.toStdString()+"\\piclist.txt";
        FILE *fp=fopen(filepath.data(),"w+");
        CreatPiclist(dir,fp);
        //picnum=ReadPiclist(filepath.data(),fp);
        //qDebug()<<picnum;
        //imshow("",picpoollist[1].Picmat);
        //waitKey();
        fclose(fp);
/*
//        QSqlQuery query;
//        query.exec("SELECT path from Color WHERE id=20");
//        while(query.next()){
//            int fieldNo = query.record().indexOf( "path");
//             QString path = query.value(fieldNo).toString();
//             Mat temp=imread(path.toStdString());
//             imshow("a",temp);
//            qDebug()<<path;
//        }
*/
        //emit jump(dir);
    }

    //CheckForJump();
}
void MainWindow::CheckForJump(){
    if(!file_full.isEmpty()){
        ui->jump2button->setEnabled(true);
    }
}
void MainWindow::on_jump2button_clicked()
{
    ShowPic(1,&s);
/*
//    for(int i=0;i<picnum;i++)
//    {
//        picpoollist[i].Histvalue=CmpHist(srcpic.Picmat,picpoollist[i].Picmat);
//    }
//    SortTop(TopCmpHist,picpoollist,picnum);
//    Top5Hist.clear();
//    for(int i=0;i<5;i++){
//        Top5Hist.push_back(picpoollist[TopCmpHist[i]].Picpath);
//        qDebug()<<picpoollist[TopCmpHist[i]].Picpath;
//    }
//    if(!ui->SrcPicPath->text().isEmpty()){
//    Top12HSV.clear();
//    QSqlQuery query;
//    query.exec("SELECT name,path FROM picture ORDER BY disHSV LIMIT 12");//取前12个
//    while(query.next()){
//        PicItem temp;
//        temp.name=query.value(0).toString();
//        temp.path=query.value(1).toString();
//        Top12HSV.push_back(temp);
//    }
//    emit PostTopPic(Top12HSV);
//        qDebug()<<TopCmpHist[i]<<" "<<picpoollist[TopCmpHist[i]].Histvalue;
//    imshow("0",picpoollist[TopCmpHist[0]].Picmat);
//    waitKey();
//    imshow("1",picpoollist[TopCmpHist[1]].Picmat);
//    waitKey();

//    s.show();
//    //this->hide();
//    }
//    else{
//        QMessageBox::warning(this,tr("警告"),tr("请选择图片！"));
//    }
*/
}
void MainWindow::on_jump3button_clicked()
{
    ShowPic(2,&s2);
}

void MainWindow::on_jump4button_clicked()
{
    ShowPic(3,&s3);
}
void MainWindow::on_action_triggered()
{
    QMessageBox a(QMessageBox::NoIcon,tr("关于"),tr("<center><font size='24'>CBIR</font></center>"));
    a.exec();

}

void MainWindow::on_action_2_triggered()
{
    QDir d(QString::fromStdString(CACHEPIC));
    d.setFilter(QDir::Files);
    int count=d.count()-1;
    for(int i=0;i<=count;i++)
        d.remove(d[i]);
}
void MainWindow::closeEvent(QCloseEvent *){
    exit(0);
}

void MainWindow::on_actionCoffee_triggered()
{
    QFile file(":/qss/coffee.qss");
    file.open(QFile::ReadOnly);
    QString styleSheet = QLatin1String(file.readAll());
    qApp->setStyleSheet(styleSheet);
}

void MainWindow::on_actionMetro_triggered()
{
    QFile file(":/qss/metro.qss");
    file.open(QFile::ReadOnly);
    QString styleSheet = QLatin1String(file.readAll());
    qApp->setStyleSheet(styleSheet);
}

void MainWindow::on_exitbutton_clicked()
{
    exit(0);
}
