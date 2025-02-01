#ifndef ADDPASSENGER_H
#define ADDPASSENGER_H

//乘客窗口，显示乘客的信息：姓名、身份证号、手机号（选填）、年龄、性别

#include <QDialog>
#include <QShortcut>
#include "global.h"
#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlError>
#include <QKeyEvent>
#include <QT>
#include <QShortcut>
#include <QKeySequence>

namespace Ui {
class AddPassenger;
}

class AddPassenger : public QDialog
{
    Q_OBJECT

public:
    explicit AddPassenger(QWidget *parent = nullptr);
    ~AddPassenger();


signals:
    void passengerAdded(const QString &name, const QString &id, const QString &phone, const QString &age, const QString &gender);

public slots: // 添加槽函数声明需要放在 slots 部分
    void on_pushButtonSave_clicked(); // 声明槽函数

    void on_pushButtonCancel_clicked(); // 声明槽函数

private:
    Ui::AddPassenger *ui;
};

#endif // ADDPASSENGER_H
