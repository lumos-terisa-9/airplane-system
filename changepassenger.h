#ifndef CHANGEPASSENGER_H
#define CHANGEPASSENGER_H

#include <QWidget>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QTimer>
#include "global.h"
namespace Ui {
class ChangePassenger;
}

class ChangePassenger : public QWidget
{
    Q_OBJECT

public:
    explicit ChangePassenger(QList<QStringList> selectedPassengers,QWidget *parent = nullptr);
    ~ChangePassenger();

signals:
    void changePassengerWindowClosed(); // 定义一个信号，当支付窗口关闭时发射
private slots:
    void on_cancle_clicked();

    void on_save_clicked();

private:
    Ui::ChangePassenger *ui;
    QList<QStringList> selectedPassengers;
};

#endif // CHANGEPASSENGER_H
