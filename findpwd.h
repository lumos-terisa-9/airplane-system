#ifndef FINDPWD_H
#define FINDPWD_H

#include <QWidget>
#include <QLineEdit>
#include <QLabel>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QTimer>
#include "global.h"

namespace Ui {
class findPwd;
}

class findPwd : public QWidget
{
    Q_OBJECT

public:
    explicit findPwd(QWidget *parent = nullptr);
    ~findPwd();

private slots:
    void on_pushButton_2_clicked();

    void on_find_clicked();

private:
    Ui::findPwd *ui;
};

#endif // FINDPWD_H
