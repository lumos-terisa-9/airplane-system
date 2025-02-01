#ifndef MAINWINDOW_H
#define MAINWINDOW_H

//主窗口，大部分功能在此实现

#include <QMainWindow>
#include <QTextEdit>
#include <QFontComboBox>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>
#include <QDateEdit>
#include <QListWidget>
#include "modifyuser.h"
#include "PurchaseWindow.h"
#include "passenger.h"
#include "addpassenger.h"
#include "timetable.h"
#include "orderwindow.h"
#include "myorder.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void setUsername(const QString& username);
    void check_show();
signals:
    void logoutRequested(); // 定义退出登录信号
public slots:
    void loadCitiesToComboBox();

public slots:
    void updateBeginDate(const QDate &date); // 定义槽函数原型

protected:
    void showEvent(QShowEvent *event);

private slots:
    void on_exchangeLocation_clicked();

    void on_MyPassenger_clicked();

    void on_searchTicket_clicked();

    void on_flights_itemClicked(QListWidgetItem *item);

    void on_logOut_clicked();

    void on_showTime_clicked();

    void on_modify_clicked();

    void on_Check_beginLocation_currentTextChanged(const QString &arg1);

    void on_Check_endLocation_currentTextChanged(const QString &arg1);

    void on_pushButton_clicked();

    void on_showTime_2_clicked();

    void on_MyOrder_clicked();

private:
    Ui::MainWindow *ui;
    PurchaseWindow *purchasewindow;
    AddPassenger *addpassenger;
    Passenger *passenger;
    timeTable* timetable;
    modifyUser* moduser;
    QComboBox *beginLocation;
    QComboBox *endLocation;
    QComboBox *Check_beginLocation;
    QComboBox *Check_endLocation;
    QDateEdit *beginDate;
    QListWidget *flights;
    QString username;

};
#endif // MAINWINDOW_H
