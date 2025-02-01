#include "modifyuser.h"
#include "ui_modifyuser.h"

modifyUser::modifyUser(QWidget *parent, QString name) : QWidget(parent), ui(new Ui::modifyUser), username(name)
{
    ui->setupUi(this);
    ui->oldPwd->setFocus();
    ui->oldPwd->setPlaceholderText("请输入旧密码");
    ui->newPwd1->setEchoMode(QLineEdit::Password); // 设置为密码模式
    ui->newPwd1->setPlaceholderText("请输入新密码");
    ui->newPwd2->setEchoMode(QLineEdit::Password); // 设置为密码模式
    ui->newPwd2->setPlaceholderText("请再次确认新密码");
    connect(ui->oldPwd, &QLineEdit::returnPressed, this, &modifyUser::on_save_clicked);
    connect(ui->newPwd1, &QLineEdit::returnPressed, this, &modifyUser::on_save_clicked);
    connect(ui->newPwd2, &QLineEdit::returnPressed, this, &modifyUser::on_save_clicked);
    setTabOrder(ui->oldPwd, ui->newPwd1);
    setTabOrder(ui->newPwd1, ui->newPwd2);
    setTabOrder(ui->newPwd2, ui->save);
    setTabOrder(ui->save, ui->cancel);
}

modifyUser::~modifyUser()
{
    delete ui;
}

void modifyUser::on_cancel_clicked()
{
    this->close();
}


void modifyUser::on_save_clicked()
{
    if (ui->oldPwd->text().isEmpty()) {
        ui->oldPwd->setFocus();
        ui->oldPwd->setPlaceholderText("旧密码不能为空!");
    }
    else if (ui->newPwd1->text().isEmpty())
    {
        // 密码为空，显示警告
        ui->newPwd1->setFocus();
        ui->newPwd1->setPlaceholderText("新密码不能为空!");
    }
    else if (ui->newPwd2->text().isEmpty())
    {
        // 密码为空，显示警告
        ui->newPwd2->setFocus();
        ui->newPwd2->setPlaceholderText("请确认密码!");
    }
    else if(ui->newPwd1->text()!=ui->newPwd2->text())
    {
        ui->notice->setText("两次密码不一致，请重新输入");
        ui->newPwd2->setFocus();
    }
    else
    {
        // 创建一个查询
        QSqlQuery query(db);
        //准备查询语句
        QString sqlQuery="SELECT User_Pwd FROM Users WHERE User_Name=:userName";
        if (!query.prepare(sqlQuery))
        {
            qDebug() << "查询准备失败：" << query.lastError();
        }
        else
        {
            //绑定用户名参数
            query.bindValue(":userName",username);
            //执行查询
            if(!query.exec())
            {
                qDebug() << "查询失败：" << query.lastError();
            }
            else
            {
                if(query.next())//用户名存在，获取密码
                {
                    QString password = query.value(0).toString();
                    //对比密码
                    if(password==ui->oldPwd->text())//密码正确，开始修改密码
                    {
                        QSqlQuery updateQuery;
                        bool success = updateQuery.prepare("UPDATE Users SET User_Pwd = :newPassword WHERE User_Name = :username");
                        if(success) {
                            // 绑定参数以防止SQL注入
                            updateQuery.bindValue(":newPassword", ui->newPwd1->text());
                            updateQuery.bindValue(":username", username);
                            // 执行更新操作
                            success = updateQuery.exec();
                            if(success) {
                                // 更新成功
                                ui->notice->setText("密码修改成功,页面即将关闭");
                                QTimer::singleShot(3000, this, SLOT(close()));
                            } else {
                                // 更新失败
                                ui->notice->setText("密码修改失败: " + updateQuery.lastError().text());
                            }
                        }
                        else {
                            // SQL语句准备失败
                            ui->notice->setText("密码修改失败: " + updateQuery.lastError().text());
                        }
                    }
                    else
                    {
                        ui->notice->setText("旧密码错误，修改失败");
                        ui->oldPwd->setFocus();
                    }
                }
            }
        }
    }
}

