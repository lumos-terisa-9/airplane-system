#include "loadwindow.h"
#include "ui_loadwindow.h"


LoadWindow::LoadWindow(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::LoadWindow)
{

    ui->setupUi(this);
    ui->userName->setPlaceholderText("请输入用户名");
    ui->userPwd->setEchoMode(QLineEdit::Password); // 设置为密码模式
    ui->userPwd->setPlaceholderText("请输入密码");
}

LoadWindow::~LoadWindow()
{
    delete ui;
}


void LoadWindow::on_pushButton_2_clicked()
{
    exit(0);
}

void LoadWindow::showLoginWindow()
{
    ui->userName->setText("");
    ui->userPwd->setText("");
    ui->userName->setFocus();
    ui->userName->setPlaceholderText("请输入用户名");
    ui->userPwd->setEchoMode(QLineEdit::Password); // 设置为密码模式
    ui->userPwd->setPlaceholderText("请输入密码");
    this->show(); // 显示LoadWindow
}

void LoadWindow::on_pushButton_clicked()
{

    if (ui->userName->text().isEmpty()) {
        // 用户名为空，显示警告
        ui->userName->setFocus();
        ui->userName->setPlaceholderText("用户名不能为空!");
    }
    else if (ui->userPwd->text().isEmpty())
    {
        // 密码为空，显示警告
        ui->userPwd->setFocus();
        ui->userPwd->setPlaceholderText("密码不能为空!");
    }
    else {
        // 用户名不为空
        ui->note->setText("");
            //检测用户名是否存在
            // 创建一个查询
            QSqlQuery query(db);
            //准备查询语句
            QString sqlQuery="SELECT User_Pwd FROM Users WHERE User_Name=:userName";
            // 准备查询
            if (!query.prepare(sqlQuery))
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
                    if(query.next())//用户名存在，获取密码
                    {
                        QString password = query.value(0).toString();
                        //对比密码
                        if(password==ui->userPwd->text())//密码正确，登录成功
                        {

                            MainWindow* mainWindow=new MainWindow();//因为会退出登录和重新登录，mainWindow随着登录操作一齐创建
                            // 连接信号与槽
                            connect(mainWindow, &MainWindow::logoutRequested, this, &LoadWindow::showLoginWindow);
                            // 创建MainWindow实例
                            mainWindow->setWindowTitle("欢迎使用415航空! "+ui->userName->text()+" :D");
                            // 加载城市到QComboBox
                            mainWindow->loadCitiesToComboBox();
                            // 在mainWindow里面设置username
                            mainWindow->setUsername(ui->userName->text());
                            // 显示MainWindow
                            mainWindow->show();
                            // 关闭LoadWindow
                            this->close();
                        }
                        else//密码错误，登录失败
                        {
                            LoadWindow::count++;
                            if(LoadWindow::count<3)
                            {
                                ui->note->setText("密码错误,请重新输入");
                                ui->userPwd->setFocus();
                            }
                            else
                            {
                                LoadWindow::count=0;
                                findpwd=new findPwd();
                                findpwd->show();
                            }

                        }
                    }
                    else//用户名不存在
                    {
                        ui->note->setText("用户名不存在,请重新输入");
                        ui->userName->setFocus();
                    }
                }
            }

        }

}


void LoadWindow::on_pushButton_3_clicked()
{
    //打开注册窗口
    registerWindow=new Register();
    registerWindow->setWindowTitle("注册");
    registerWindow->show();
}


void LoadWindow::on_pushButton_4_clicked()
{
    findpwd=new findPwd();
    findpwd->show();
}

