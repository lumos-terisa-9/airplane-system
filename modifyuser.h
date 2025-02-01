#ifndef MODIFYUSER_H
#define MODIFYUSER_H

#include <QWidget>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QString>
#include <QTimer>
#include "global.h"

namespace Ui {
class modifyUser;
}

class modifyUser : public QWidget
{
    Q_OBJECT

public:
    explicit modifyUser(QWidget *parent = nullptr,QString name= QString());
    ~modifyUser();
private slots:
    void on_cancel_clicked();
    void on_save_clicked();

private:
    Ui::modifyUser *ui;
    QString username;

};

#endif // MODIFYUSER_H
