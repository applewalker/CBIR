#include "secondwindow.h"
#include "ui_secondwindow.h"

SecondWindow::SecondWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::SecondWindow)
{
    ui->setupUi(this);
    ui->listWidget->setResizeMode(QListView::Adjust);
    this->setProperty("ablemove", true);

    //设置窗体标题栏隐藏
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowSystemMenuHint | Qt::WindowMinimizeButtonHint);

    connect(ui->listWidget,SIGNAL(itemClicked(QListWidgetItem*)),this,SLOT(OpenPic(QListWidgetItem*)));
    ui->listWidget->setAutoScroll(true);
}

SecondWindow::~SecondWindow()
{
    delete ui;
}
void SecondWindow::title(QString title)
{
    ui->label->setText(title);
}
void SecondWindow::on_back_button_clicked()
{
    hide();
    emit back();
    //this->~SecondWindow();
}
void SecondWindow::ReceivePool(QString path){
    dir=path;
    //ui->textEdit->setText(dir);
}
void SecondWindow::ReceivePicPath(vector<PicItem> Top12Pic){

    ui->listWidget->clear();

    if(!Top12Pic.empty()){
    Top12=Top12Pic;
    for(int i=0;i<12;i++){
        QListWidgetItem * picitem = new QListWidgetItem(QIcon(PreviewPic(Top12Pic[i].path,120,120)),Top12Pic[i].name,ui->listWidget);
        ui->listWidget->addItem(picitem);
        //ui->listWidget->setIconSize(QSize(100,100));
        picitem->setData(Qt::UserRole,i);
        picitem->setSizeHint(QSize(120,120));
        picitem->setTextAlignment(Qt::AlignCenter);
    }
    ui->listWidget->setMovement(QListView::Static);
    }
    else
    {
        QMessageBox a;
        a.setWindowTitle(tr("提示"));
        a.setText(tr("无结果"));
        a.exec();
    }
}
void SecondWindow::OpenPic(QListWidgetItem * item){
    int i=item->data(Qt::UserRole).toInt();
    string name=item->text().toStdString();
    if(!name.empty() && i>=0 && i<=11 ){
    Mat pic=imread(Top12[i].path.toStdString());
    imshow(name,pic);
    waitKey();
    }
}
