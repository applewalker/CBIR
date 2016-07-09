#ifndef SECONDWINDOW_H
#define SECONDWINDOW_H

#include <StdHeader.h>

namespace Ui {
class SecondWindow;
}

class SecondWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit SecondWindow(QWidget *parent = 0);
    ~SecondWindow();
    QString dir;
    void title(QString title);
private slots:
    void on_back_button_clicked();
    void ReceivePool(QString);
    void ReceivePicPath(vector<PicItem>);
    void OpenPic(QListWidgetItem* item);

private:
    Ui::SecondWindow *ui;
    vector<PicItem> Top12;
signals:
    void back();

};

#endif // SECONDWINDOW_H
