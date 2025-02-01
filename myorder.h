#ifndef MYORDER_H
#define MYORDER_H

#include <QDialog>
#include <QString>
#include <QVector> // 用于存储航班或乘客信息
#include <QSqlQuery>
#include <QTableWidget>
#include <QWidget>
#include "rescheduleticket.h"
#include "confirmpurchase.h"
#include <QMessageBox>
#include <QSqlError>
#include <QSqlRecord>

namespace Ui {
class MyOrder;
}

class MyOrder : public QDialog
{
    Q_OBJECT

public:
    explicit MyOrder(QWidget *parent = nullptr);
    ~MyOrder();
    void setFlightInfo(QTableWidget *tableWidget, QSqlQuery &query);

protected:
    void closeEvent(QCloseEvent *event);

signals:
    void closeMyOrder();

private:
    Ui::MyOrder *ui;

private slots:
    void onFlightInfoTableClicked(int row, int column);

private:
    QString flightInfo;                      // 存储航班信息
    QList<QStringList> passengerInfoList;    // 存储乘客信息
    void loadOrders();                            // 加载订单数据
    void populateTable(QTabWidget *table, QSqlQuery &query); // 填充表格 //需要修改
    void setupFlightTable();
    void adjustTableSize();
};

#endif // MYORDER_H

