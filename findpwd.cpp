#include "findpwd.h"
#include "ui_findpwd.h"

findPwd::findPwd(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::findPwd)
{
    ui->setupUi(this);
    ui->userName->setPlaceholderText("请输入用户名");
    ui->userID->setPlaceholderText("请输入身份证号码");
    connect(ui->userName, &QLineEdit::returnPressed, this, &findPwd::on_find_clicked);
    connect(ui->userID, &QLineEdit::returnPressed, this, &findPwd::on_find_clicked);
    ui->userName->setFocus();

    setTabOrder(ui->userName, ui->userID);
    setTabOrder(ui->userID, ui->find);
    setTabOrder(ui->find, ui->pushButton_2);
}

findPwd::~findPwd()
{
    delete ui;
}

void findPwd::on_pushButton_2_clicked()
{
    this->close();
}


void findPwd::on_find_clicked()
{
    ui->notice->setText("");
    if (ui->userName->text().isEmpty()) {
        // 用户名为空，显示警告
        ui->userName->setFocus();
        ui->userName->setPlaceholderText("用户名不能为空!");
    }
    else if (ui->userID->text().isEmpty())
    {
        // 密码为空，显示警告
        ui->userID->setFocus();
        ui->userID->setPlaceholderText("身份证号码不能为空!");
    }
    else
    {
        // 创建一个查询
        QSqlQuery query(db);
        //准备查询语句,先查名字
        QString sqlQuerywithname="SELECT User_Pwd FROM Users WHERE User_Name=:userName";
        // 准备查询
        if (!query.prepare(sqlQuerywithname))
        {
            qDebug() << "查询准备失败：" << query.lastError();
        }
        else
        {
            //绑定用户名参数
            query.bindValue(":userName",ui->userName->text());
            //执行查询
            if(!query.exec())
            {
                qDebug() << "查询失败：" << query.lastError();
            }
            else
            {
                if (query.next())
                {
                    QString sqlQuery="SELECT User_Pwd FROM Users WHERE User_Name=:userName And ID_Number=:userID";
                    QSqlQuery queryWithID(db);
                    if (!queryWithID.prepare(sqlQuery))
                    {
                        qDebug() << "查询准备失败：" << query.lastError();
                    }
                    else
                    {
                        //绑定参数
                        queryWithID.bindValue(":userName",ui->userName->text());
                        queryWithID.bindValue(":userID",ui->userID->text());
                        if (!queryWithID.exec())
                        {
                            qDebug() << "带ID的查询失败：" << queryWithID.lastError();
                        }
                        else
                        {
                            if (queryWithID.next())
                            {
                                QString password = queryWithID.value("User_Pwd").toString();
                                ui->notice->setText(QString("密码找回成功：%1\n页面即将自动关闭").arg(password));
                                QTimer::singleShot(3000, this, SLOT(close()));

                            }
                            else
                            {
                                ui->userID->setFocus();
                                ui->notice->setText("身份证号码错误,请检查输入");
                            }

                        }
                    }
                }
                else
                {
                    ui->userName->setFocus();
                    ui->notice->setText("用户名不存在,请检查输入");
                }
            }
        }
    }
}

