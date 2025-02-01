#include "confirmpurchase.h"
#include "ui_confirmpurchase.h"

ConfirmPurchase::ConfirmPurchase(QWidget *parent, const QString &orderId)
    : QDialog(parent)
    , ui(new Ui::ConfirmPurchase)
{
    ui->setupUi(this);
    qDebug() << " ConfirmPurchase created with Order ID: " << orderId;
    this->orderId = orderId;
}

ConfirmPurchase::~ConfirmPurchase()
{
    delete ui;
}

void ConfirmPurchase::on_btn_confirm_clicked()
{
    PurchaseWindow *pay = new PurchaseWindow(nullptr, orderId);
    pay->show();
    newState = "待支付";
    connect(this, &ConfirmPurchase::closethis, pay, &PurchaseWindow::close);
    connect(pay, &PurchaseWindow::notPay, this, &ConfirmPurchase::showNotPay);
    connect(pay, &PurchaseWindow::purchaseWindowClosed, this, &ConfirmPurchase::updateOrderState);
}

void ConfirmPurchase::on_btn_abandon_clicked()
{
    QMessageBox::StandardButton resBtn = QMessageBox::question( this, "温馨提示",tr("是否确认退票？\n",""),
                                        QMessageBox::Cancel | QMessageBox::No | QMessageBox::Yes,QMessageBox::Yes);
    if (resBtn == QMessageBox::Yes) {
        newState = "已退票";
        updateOrderState();
    }
    else {
        ui->note->setText("退票已取消");
    }
}

void ConfirmPurchase::showNotPay() {
    ui->note->setText("订单支付已取消");
}

void ConfirmPurchase::updateOrderState() {
    // 创建SQL查询，更新订单状态
    QSqlQuery query(db);
    if(newState == "待支付") {
        newState = "已支付";
    }
    query.prepare("UPDATE Orders SET State = :state WHERE Order_ID = :orderId");
    query.bindValue(":state", newState);  // 动态设置状态
    query.bindValue(":orderId", orderId); // 绑定订单ID
    qDebug()<<newState;
    qDebug()<<orderId;
    // 执行查询
    if (query.exec())
    {
        if(newState == "已支付") {
            ui->note->setText("恭喜您，支付成功！");
            ui->lineEditState->setText(newState); // 更新状态显示
            ui->lineEditState->update(); // 仅刷新状态显示的QLineEdit
        }
        if(newState == "已退票") {
            ui->note->setText("退票成功");
            ui->lineEditState->setText(newState); // 更新状态显示
            ui->lineEditState->update(); // 仅刷新状态显示的QLineEdit
        }
        emit updateTicket();
        this->close();
    }
    else {
        QMessageBox::critical(this, "Error", "Failed to update order state: " + query.lastError().text());
    }
}

void ConfirmPurchase::setOrderDetails(const QStringList &details) {
    // 填充界面控件
    ui->lineEditOrderId->setText(details[0]);
    qDebug() << "UI Content: " << ui->lineEditOrderId;
    qDebug() << "lineEditOrderId pointer:" << ui->lineEditOrderId;
    if (!ui->lineEditOrderId) {
        qDebug() << "Error: lineEditOrderId is nullptr!";
        return;
    }
    ui->lineEditFlightId->setText(details[1]);
    // ui->lineEditOrigin->setText(details[2]);       // 出发地
    // ui->lineEditDestination->setText(details[3]);  // 目的地
    ui->lineEditBeginTime->setText(details[2]);
    ui->lineEditPrice->setText(details[3]);
    ui->lineEditSeat->setText(details[4]);
    ui->lineEditState->setText(details[5]);
    ui->lineEditName->setText(details[6]);
    ui->lineEditIdNumber->setText(details[7]);

    // 使用 Flight_ID 查询 Flights 表
    QString flightId = details[1]; // 从 details 获取 Flight_ID
    QSqlQuery query(db);

    // 准备 SQL 查询语句
    QString sql = "SELECT BeginLocation, EndLocation,EndTime FROM Flights WHERE Flight_ID = :flightId";
    query.prepare(sql);
    query.bindValue(":flightId", flightId);

    // 执行查询
    if (query.exec()) {
        if (query.next()) {
            // 获取查询结果
            QString origin = query.value(0).toString();       // 获取 Origin
            QString destination = query.value(1).toString();  // 获取 Destination
            QString endtime=query.value(2).toString();//获取endtime
            qDebug()<<endtime;
            // 填充到界面控件
            ui->lineEditOrigin->setText(origin);
            ui->lineEditDestination->setText(destination);
            ui->lineEditEndTime->setText(endtime);
            qDebug() << "Flight_ID:" << flightId;
            qDebug() << "Origin:" << origin;
            qDebug() << "Destination:" << destination;
        } else {
            qDebug() << "Error: No matching flight found for Flight_ID:" << flightId;
        }
    } else {
        qDebug() << "SQL query failed:" << query.lastError().text();
    }
}

void ConfirmPurchase::closeEvent(QCloseEvent *event) {
    emit closethis();
    QWidget::closeEvent(event);
}

