#ifndef ORDERWINDOW_H
#define ORDERWINDOW_H

#include <QDialog>
#include <QLabel>
#include <QCheckBox>
#include <QVector>
#include <QLCDNumber>
#include <QTimer>
#include <QTime>
#include <QCloseEvent>
#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlError>
#include "passenger.h"
#include "global.h"
#include "purchasewindow.h"

namespace Ui {
class OrderWindow;
}

class OrderWindow : public QDialog
{
    Q_OBJECT

public:
    explicit OrderWindow(QWidget *parent = nullptr);
    ~OrderWindow();
    void setData(QString f_id, QString beLo, QString endLo, QString beTim, QString endTim, QString username);
    void setLCD();
    QString generateOrder_ID(QString ID_Number);
    void setMaxSelectable(int max);
    void enforceMaxSelectable(QCheckBox* thisSel);

public slots:
    // 槽函数用于接收选中项的文本信息 ID_Number
    void selectedPassenger(const QList<QStringList>& passengerInfo);

signals:
    void closethis();

protected:
    void closeEvent(QCloseEvent *event);

private slots:
 //   void cleanup();
    void upDateTimer();

    void on_selectPassenger_clicked();

    void on_sure_clicked();

    void on_checkBox_clicked();

    void on_checkBox_2_clicked();

    void on_checkBox_3_clicked();

    void on_checkBox_4_clicked();

    void on_checkBox_5_clicked();

private:
    Ui::OrderWindow *ui;
    QPushButton *sure;
    QLabel *flight_ID;
    QLabel *beginLocation;
    QLabel *endLocation;
    QLabel *beginTime;
    QLabel *endTime;
    QLabel *showPrice;

    QVector<QCheckBox*> isSelected;
    QCheckBox *checkBox;
    QCheckBox *checkBox_2;
    QCheckBox *checkBox_3;
    QCheckBox *checkBox_4;
    QCheckBox *checkBox_5;

    QLCDNumber *countDown;
    QTime remainTime;
    QTimer *timer;

    QList<QStringList> passengerInfo;
    int maxSelectable;
    QString price;
    QString username;
    QList<QString> ID_Number;
};

#endif // ORDERWINDOW_H
