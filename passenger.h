#ifndef PASSENGER_H
#define PASSENGER_H

// 乘客窗口，显示乘客的信息：姓名、身份证号、手机号（选填）、年龄、性别
#include "addpassenger.h"
#include "global.h"
#include <QDialog>
#include <QTableWidget>
#include "changepassenger.h"

namespace Ui {
class Passenger;
}

class Passenger : public QDialog
{
    Q_OBJECT

public:
    explicit Passenger(QWidget *parent = nullptr);
    ~Passenger();
    QList<QStringList> getSelectedPassengers() const;

protected:
    void closeEvent(QCloseEvent *event);

private slots:
    void on_addPassenger_clicked(); // 添加乘机人按钮的槽函数
    void on_confirmButton_clicked();

    void on_modify_clicked();


    void on_passengerTable_itemClicked(QTableWidgetItem *item);


signals:
    void passengersConfirmed(const QList<QStringList>& selectedPassengers);
    void passengersSelected(const QList<QStringList>& passengers);
    void closePassenger();

public slots:
    void addPassengerToTable(QString name, QString id, QString phone, QString age, QString gender);

private:
    void setupPassengerTable(); // 初始化乘客表格
    void loadPassengerData();   // 加载乘客数据
    int getCurrentUserId();
    int getSelectedPassengerRow();

    Ui::Passenger *ui;
    AddPassenger* addpassenger;
    QList<QStringList> selectedPassengers;
};

#endif // PASSENGER_H




