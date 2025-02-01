#include "addpassenger.h"
#include "ui_addpassenger.h"

QString userID;

AddPassenger::AddPassenger(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddPassenger)
{
    ui->setupUi(this);
    setWindowTitle("乘客信息");

    // 设置占位提示文本
    ui->lineEditName->setPlaceholderText("请输入姓名");
    ui->lineEditID->setPlaceholderText("请输入身份证号");
    ui->lineEditPhone->setPlaceholderText("请输入手机号");
    ui->lineEditAge->setPlaceholderText("请输入年龄");
    ui->lineEditName->setFocus();
    // 创建一个快捷键，当按下回车键时，触发 pushButtonSave 的点击事件
    new QShortcut(QKeySequence(Qt::Key_Return), this, SLOT(on_pushButtonSave_clicked()));
    new QShortcut(QKeySequence(Qt::Key_Enter),  this, SLOT(on_pushButtonSave_clicked()));

    setTabOrder(ui->lineEditName, ui->lineEditID);
    setTabOrder(ui->lineEditID, ui->lineEditPhone);
    setTabOrder(ui->lineEditPhone, ui->lineEditAge);
    setTabOrder(ui->lineEditAge, ui->pushButtonSave);
    setTabOrder(ui->pushButtonSave, ui->pushButtonCancel);
}

AddPassenger::~AddPassenger()
{
    delete ui;
}


void AddPassenger::on_pushButtonSave_clicked()
{
    // 获取用户输入的数据
    ui->note->setText("");
    QString name = ui->lineEditName->text();
    QString id = ui->lineEditID->text();
    QString phone = ui->lineEditPhone->text();
    QString age = ui->lineEditAge->text();
    QString gender = ui->comboBoxGender->currentText();
    // 验证输入是否完整
    if (name.isEmpty()) {
        ui->lineEditName->setPlaceholderText("姓名不能为空");
        ui->lineEditName->setFocus();
    }
    else if(id.isEmpty())
    {
        ui->lineEditID->setPlaceholderText("身份证号不能为空");
        ui->lineEditID->setFocus();
    }
    else if(age.isEmpty())
    {
        ui->lineEditAge->setPlaceholderText("年龄不能为空");
        ui->lineEditAge->setFocus();
    }
    else if(gender.isEmpty())
    {
        ui->note->setText("请选择性别");
    }
    else if (id.length() != 18) {
        ui->note->setText("身份证号码必须为18位");
        ui->lineEditID->setFocus();
    }
    else if (!phone.isEmpty() && phone.length() != 11)
    {
        ui->note->setText("手机号必须为11位");
        ui->lineEditPhone->setFocus();
    }
    else
    {
        // 检查乘客是否已存在
        QSqlQuery checkQuery(db);
        checkQuery.prepare("SELECT COUNT(*) FROM Passengers WHERE ID_Number = :id_number AND User_Name = :user");
        checkQuery.bindValue(":id_number", id);
        checkQuery.bindValue(":user", globalUsername);
        if (!checkQuery.exec()) {
            QMessageBox::critical(this, "数据库错误", "查询乘客信息失败: " + checkQuery.lastError().text());
            return;
        }
        if (checkQuery.next() && checkQuery.value(0).toInt() > 0) {
            // 如果查询返回的计数大于0，则表示乘客已存在
            ui->note->setText("乘客信息已存在，请勿重复存储");
            return;
        }


            // 将数据写入数据库
            QSqlQuery query(db);
            query.prepare("INSERT INTO Passengers (Name, ID_Number, Phone_Number, Age, Gender, User_Name) "
                          "VALUES (:name, :id, :phone, :age, :gender, :user)");
            query.bindValue(":name", name);
            query.bindValue(":id", id);
            query.bindValue(":phone", phone);
            query.bindValue(":age", age);
            query.bindValue(":gender", gender);
            query.bindValue(":user", globalUsername);

            if (!query.exec()) {
                QMessageBox::critical(this, "数据库错误", "无法保存数据: " + query.lastError().text());
                return;
            }

            // 发出信号，将新乘客数据传递回 Passenger 窗口
            emit passengerAdded(name, id, phone, age, gender);
            this->close();


    }
}




// 取消按钮点击事件
void AddPassenger::on_pushButtonCancel_clicked()
{
    this->close();
}
