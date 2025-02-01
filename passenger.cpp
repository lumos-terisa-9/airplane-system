#include "passenger.h"
#include "addpassenger.h"
#include "ui_passenger.h"
#include "global.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>
#include <QCheckBox>
#include <QHBoxLayout>

Passenger::Passenger(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Passenger)
{
    ui->setupUi(this);
    // 初始化乘客表格
    setupPassengerTable();
    // 加载数据
    loadPassengerData();
}

Passenger::~Passenger()
{
    delete ui;
}

void Passenger::setupPassengerTable()
{
    // 设置表头
    ui->passengerTable->setColumnCount(6);
    ui->passengerTable->setHorizontalHeaderLabels({"选择","姓名", "身份证号", "年龄", "性别", "手机号"});
    ui->passengerTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->passengerTable->setSelectionBehavior(QAbstractItemView::SelectRows);
}

void Passenger::loadPassengerData()
{
    QSqlQuery query(db);

    // 查询当前用户的乘客信息
    query.prepare("SELECT Passenger_ID, Name, ID_Number, Age, Gender, Phone_Number "
                  "FROM Passengers WHERE User_Name = :username");
    query.bindValue(":username", globalUsername);
    qDebug() << "当前用户：" << globalUsername;


    // 查询乘客信息
    //query.prepare("SELECT Name, ID_Number, Age, Gender, Phone_Number, User_Name FROM Passengers");
    if (!query.exec()) {
        QString errorStr = query.lastError().text();
        qDebug() << "查询错误: " << errorStr;  // 打印错误信息到调试输出
        QMessageBox::critical(this, "查询错误", "无法加载乘客信息: " + query.lastError().text());
        return;
    }

    // 清空表格内容
    ui->passengerTable->clearContents();
    ui->passengerTable->setRowCount(0);

    int row = 0; // 初始化行号
    while (query.next()) {
        ui->passengerTable->insertRow(row);

        // 创建复选框
        QCheckBox *checkBox = new QCheckBox(this);
        QWidget *checkBoxWidget = new QWidget();
        QHBoxLayout *layout = new QHBoxLayout(checkBoxWidget);
        layout->addWidget(checkBox);
        layout->setAlignment(Qt::AlignCenter);
        layout->setContentsMargins(0, 0, 0, 0);
        checkBoxWidget->setLayout(layout);
        ui->passengerTable->setCellWidget(row, 0, checkBoxWidget);

        // 填充表格内容
        ui->passengerTable->setItem(row, 1, new QTableWidgetItem(query.value("Name").toString()));
        ui->passengerTable->setItem(row, 2, new QTableWidgetItem(query.value("ID_Number").toString()));
        ui->passengerTable->setItem(row, 3, new QTableWidgetItem(query.value("Age").toString()));
        ui->passengerTable->setItem(row, 4, new QTableWidgetItem(query.value("Gender").toString()));
        ui->passengerTable->setItem(row, 5, new QTableWidgetItem(query.value("Phone_Number").toString()));

        row++;
    }
    // 禁用所有编辑操作
    ui->passengerTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
}

void Passenger::addPassengerToTable(QString name, QString id, QString phone, QString age, QString gender)
{
    // 获取表格的当前行数
    int row = ui->passengerTable->rowCount();
    ui->passengerTable->insertRow(row);

    // 创建复选框
    QCheckBox *checkBox = new QCheckBox(this);
    QWidget *checkBoxWidget = new QWidget();
    QHBoxLayout *layout = new QHBoxLayout(checkBoxWidget);
    layout->addWidget(checkBox);
    layout->setAlignment(Qt::AlignCenter);
    layout->setContentsMargins(0, 0, 0, 0);
    checkBoxWidget->setLayout(layout);
    ui->passengerTable->setCellWidget(row, 0, checkBoxWidget);


    ui->passengerTable->setItem(row, 1, new QTableWidgetItem(name));
    ui->passengerTable->setItem(row, 2, new QTableWidgetItem(id));  // 身份证号
    ui->passengerTable->setItem(row, 3, new QTableWidgetItem(age));
    ui->passengerTable->setItem(row, 4, new QTableWidgetItem(gender));
    ui->passengerTable->setItem(row, 5, new QTableWidgetItem(phone));
}


void Passenger::on_addPassenger_clicked()
{
    // 打开添加乘客窗口
    AddPassenger* addpassenger = new AddPassenger(this);

    // 连接信号和槽
    connect(addpassenger, &AddPassenger::passengerAdded, this, &Passenger::addPassengerToTable);

    addpassenger->exec(); // 使用 exec() 让窗口以模态方式运行
}

void Passenger::on_confirmButton_clicked()
{
    QList<QStringList> selectedPassengers = getSelectedPassengers();
    qDebug() << "Selected Passengers:" << selectedPassengers;
    emit passengersSelected(getSelectedPassengers());
    if (selectedPassengers.isEmpty()) {
        QMessageBox::warning(this, "提示", "请至少选择一个乘客！");
        return;
    }
    emit passengersConfirmed(selectedPassengers);
    qDebug() << "Signal passengersSelected emitted with data:" << selectedPassengers;
    this->accept(); // 关闭 Passenger 窗口
}

QList<QStringList> Passenger::getSelectedPassengers() const {
    QList<QStringList> selectedPassengers;
    for (int row = 0; row < ui->passengerTable->rowCount(); ++row) {
        QCheckBox* checkBox = qobject_cast<QCheckBox*>(ui->passengerTable->cellWidget(row, 0)->layout()->itemAt(0)->widget());
        if (checkBox && checkBox->isChecked()) {
            QStringList passengerInfo;
            for (int col = 1; col < ui->passengerTable->columnCount(); ++col) {
                passengerInfo.append(ui->passengerTable->item(row, col)->text());
            }
            selectedPassengers.append(passengerInfo);
        }
    }
    return selectedPassengers;
}


int Passenger::getSelectedPassengerRow()
{
    for (int row = 0; row < ui->passengerTable->rowCount(); ++row) {
        QCheckBox* checkBox = qobject_cast<QCheckBox*>(ui->passengerTable->cellWidget(row, 0)->layout()->itemAt(0)->widget());
        if (checkBox && checkBox->isChecked()) {
            return row;
        }
    }
    return -1; // 如果没有选中的乘客，返回 -1

}

void Passenger::on_modify_clicked()
{
    // 获取选中的乘客信息列表
    QList<QStringList> selectedPassengers = getSelectedPassengers();
    // 只处理第一个选中的乘客
    if (selectedPassengers.size() > 1) {
        QMessageBox::warning(this, "提示", "只能选择一个乘客进行修改！");
        return;
    }

    // 我们只处理第一个选中的乘客
    int row = getSelectedPassengerRow();
    if (row == -1) {
        QMessageBox::warning(this, "提示", "未找到选中的乘客！");
        return;
    }
    ChangePassenger* change=new ChangePassenger(selectedPassengers,nullptr);
    connect(this, &Passenger::closePassenger, change, &ChangePassenger::close);
    connect(change, &ChangePassenger::changePassengerWindowClosed, this, &Passenger::loadPassengerData);
    change->show();
}

void Passenger::closeEvent(QCloseEvent *event) {
    emit closePassenger();
    QWidget::closeEvent(event);
}




void Passenger::on_passengerTable_itemClicked(QTableWidgetItem *item)
{
    if (!item) return;

    // 获取点击的行号
    int row = item->row();

    // 获取该行的复选框
    QWidget *checkBoxWidget = ui->passengerTable->cellWidget(row, 0);
    if (checkBoxWidget) {
        QCheckBox *checkBox = qobject_cast<QCheckBox*>(checkBoxWidget->layout()->itemAt(0)->widget());
        if (checkBox) {
            // 切换复选框的状态
            checkBox->setChecked(!checkBox->isChecked());
        }
    }

}



