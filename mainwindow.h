#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <StdHeader.h>
#include <secondwindow.h>


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();   

    QString file_full;//原图路径
    QString dir;//图库路径
    Picdrop picpoollist[MAXNUM];//存储图库列表
    int picnum;//记录图库图片数量
    int TopCmpHist[MAXNUM];//直方图对比值排序表，从高到底
    vector<PicItem> Top12Pic;
    Picdrop srcpic;//存储原图



private slots:

    void b();
    void ShowPic(int type,SecondWindow *s);
    void on_ChoosePicButton_clicked();
    void on_ChoosePoolButton_clicked();
    void CheckForJump();
    void on_jump2button_clicked();
    void closeEvent(QCloseEvent *);


    void on_jump3button_clicked();

    void on_action_triggered();

    void on_action_2_triggered();

    void on_jump4button_clicked();

    void on_actionCoffee_triggered();

    void on_actionMetro_triggered();

    void on_exitbutton_clicked();

private:
    Ui::MainWindow *ui;
    SecondWindow s;
    SecondWindow s2;
    SecondWindow s3;
    valueHSV src_HSV;
    valueGLCM src_GLCM;
    valueHME src_HME;
    QRect locatRect;
signals:
    void jump(QString);
    void PostTopPic(vector<PicItem>);
    void PostTopPic2(vector<PicItem>);
    void PostTopPic3(vector<PicItem>);
};

#endif // MAINWINDOW_H
