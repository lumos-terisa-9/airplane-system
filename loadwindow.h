#ifndef LOADWINDOW_H
#define LOADWINDOW_H

//登录界面

#include <QDialog>
#include <QLineEdit>
#include <QLabel>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>
#include "global.h"
#include "MainWindow.h"
#include "register.h"
#include "findPwd.h"

namespace Ui {
class LoadWindow;
}

class LoadWindow : public QDialog
{
    Q_OBJECT

public:
    explicit LoadWindow(QWidget *parent = nullptr);
    ~LoadWindow();
public slots:
    void showLoginWindow(); // 槽函数，用于显示登录窗口
private slots:
    void on_pushButton_2_clicked();

    void on_pushButton_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

private:
    Ui::LoadWindow *ui;
    // 添加一个指向MainWindow的指针
    findPwd* findpwd;
    QLineEdit *userName;
    QLineEdit *userPwd;
    QLabel *label_2;
    Register *registerWindow;
    int count=0;
};

#endif // LOADWINDOW_H
