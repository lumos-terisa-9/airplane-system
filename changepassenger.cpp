#include "changepassenger.h"
#include "ui_changepassenger.h"

ChangePassenger::ChangePassenger(QList<QStringList> selectedPassengers,QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ChangePassenger)
    ,selectedPassengers(selectedPassengers)
{
    ui->setupUi(this);
    // 可以在这里添加代码来填充 UI 控件，例如将 selectedPassengers 中的数据填充到对应的输入框中
    if (!selectedPassengers.isEmpty()) {
        // 假设 selectedPassengers 的第一个元素包含了乘客的所有信息
        QStringList passengerInfo = selectedPassengers.first();

        ui->userName->setText(passengerInfo.at(0));
        ui->ID_Number->setText(passengerInfo.at(1));
        ui->Age->setText(passengerInfo.at(2));
        QString gender = passengerInfo.at(3);
        // 设置性别信息
        ui->Gender->setCurrentText(gender); // 如果性别是预定义选项之一

        ui->PhoneNumber->setText(passengerInfo.at(4));
        connect(ui->PhoneNumber, &QLineEdit::returnPressed, this, &ChangePassenger::on_save_clicked);
        connect(ui->Age, &QLineEdit::returnPressed, this, &ChangePassenger::on_save_clicked);
    }
}

ChangePassenger::~ChangePassenger()
{
    delete ui;
}

void ChangePassenger::on_cancle_clicked()
{
    this->close();
}


void ChangePassenger::on_save_clicked()
{
    ui->note->setText("");
    QSqlQuery query(db);
    if(ui->PhoneNumber->text()!=""&&ui->PhoneNumber->text().length()!=11)
    {
        ui->note->setText("电话号码需要为11位");
        ui->PhoneNumber->setFocus();
        return;
    }
    else if(ui->Age->text()=="")
    {
        ui->note->setText("年龄不能为空");
        ui->Age->setFocus();
        return;
    }
    QString preper="UPDATE Passengers SET Age=:age, Gender = :gender,Phone_Number=:phonenumber WHERE User_Name=:username And ID_Number=:id_number";
    if(!query.prepare(preper))
    {
        qDebug() << "查询准备失败：" << query.lastError();
        return;
    }
    else
    {
        //绑定参数
        query.bindValue(":age",ui->Age->text());
        query.bindValue(":gender",ui->Gender->currentText());
        query.bindValue(":phonenumber",ui->PhoneNumber->text());
        query.bindValue(":username",globalUsername);
        query.bindValue(":id_number",ui->ID_Number->text());
        //执行操作
        if(!query.exec())
        {
            qDebug() << "插入失败2：" << query.lastError();
        }
        else
        {
            emit changePassengerWindowClosed();
            this->close();
        }
    }
}

