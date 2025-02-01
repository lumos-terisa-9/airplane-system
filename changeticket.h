#ifndef CHANGETICKET_H
#define CHANGETICKET_H

#include <QWidget>
#include "orderwindow.h"
#include "timetable.h"
#include <QSqlQuery>
#include <QDebug>
namespace Ui {
class ChangeTicket;
}

class ChangeTicket : public QWidget
{
    Q_OBJECT

public:
    explicit ChangeTicket(QDate &orderdate,QWidget *parent = nullptr, const QString &orderId = "");
    ~ChangeTicket();
    void setTicketTableData(const QList<QList<QString>>& tickets);
    void initTableWidget();

signals:
    void closeChange();  // 自定义的关闭信号

private slots:
    void on_ticketRow_clicked(int row, int column);
    void on_dateChanged(const QDate &selectedDate);  // 日期改变时的槽函数

    void on_confirmButton_clicked();

    void on_cancelButton_clicked();

private:
    Ui::ChangeTicket *ui;
    void filterFlightsByDate(const QDate &date);  // 筛选航班数据
    QString currentOrderId;  // 用于存储传递的 orderId
    QDate orderdate;
};

#endif // CHANGETICKET_H
