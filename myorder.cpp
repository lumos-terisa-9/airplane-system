#include "myorder.h"
#include "ui_myorder.h"

MyOrder::MyOrder(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MyOrder)
{
    ui->setupUi(this);
    setupFlightTable();
    loadOrders();
    adjustTableSize();

    connect(ui->flightInfoTable, &QTableWidget::cellClicked, this, &MyOrder::onFlightInfoTableClicked);

}

MyOrder::~MyOrder()
{
    delete ui;
}

void MyOrder::setupFlightTable()
{
    // 设置表头
    ui->flightInfoTable->setColumnCount(8); // 设置列数
    ui->flightInfoTable->setHorizontalHeaderLabels({"订单号", "航班号", "起飞时间", "价格", "座位", "支付状态", "乘客姓名", "身份证号"});

    // 设置列宽和行高的自动调整策略
    ui->flightInfoTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch); // 列宽自适应
    ui->flightInfoTable->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents); // 行高自适应

    // 启用选择整行模式
    ui->flightInfoTable->setSelectionBehavior(QAbstractItemView::SelectRows);

    // 禁用编辑功能（可选）
    ui->flightInfoTable->setEditTriggers(QAbstractItemView::NoEditTriggers);

    qDebug() << "Table has been created.";
}

void MyOrder::adjustTableSize()
{
    // 动态调整 QTableWidget 的高度以适应内容
    int rowCount = ui->flightInfoTable->rowCount();
    int tableHeight = ui->flightInfoTable->horizontalHeader()->height(); // 表头高度

    for (int i = 0; i < rowCount; ++i) {
        tableHeight += ui->flightInfoTable->rowHeight(i); // 累计每行高度
    }

    // 适当增加额外的高度空间
    tableHeight += 20;

    // 设置表格和窗口的高度
    ui->flightInfoTable->setMinimumHeight(tableHeight);
    this->setMinimumHeight(tableHeight + 100); // 窗口额外高度
}

void MyOrder::loadOrders()
{
    QSqlQuery query(db);

    query.prepare(R"(
    SELECT Orders.Order_ID, Orders.Flight_ID, Orders.BeginTime, Orders.Price, Orders.Seat, Orders.State,
        Passengers.Name, Passengers.ID_Number
        FROM Orders
        INNER JOIN Passengers ON Orders.ID_Number = Passengers.ID_Number
        WHERE Orders.User_Name = :userName OR Passengers.Name = :userName  -- 只查询当前用户的订单
    )");
    // 绑定参数，使用全局变量 globalUserName
    query.bindValue(":userName", globalUsername);

    //清空表格
    ui->flightInfoTable->clearContents();
    ui->flightInfoTable->setRowCount(0);

    // 执行查询
    if (!query.exec()) {
        qDebug() << "Query failed:" << query.lastError().text();
        return;
    }

    int row = 0;
    while (query.next()) {
        ui->flightInfoTable->insertRow(row);

        qDebug() << "Inserting row:" << row
                 << "Order_ID:" << query.value("Order_ID").toString()
                 << "Flight_ID:" << query.value("Flight_ID").toString();

        ui->flightInfoTable->setItem(row, 0, new QTableWidgetItem(query.value("Order_ID").toString()));
        ui->flightInfoTable->setItem(row, 1, new QTableWidgetItem(query.value("Flight_ID").toString()));
        ui->flightInfoTable->setItem(row, 2, new QTableWidgetItem(query.value("BeginTime").toString()));
        ui->flightInfoTable->setItem(row, 3, new QTableWidgetItem(query.value("Price").toString()));
        ui->flightInfoTable->setItem(row, 4, new QTableWidgetItem(query.value("Seat").toString()));
        ui->flightInfoTable->setItem(row, 5, new QTableWidgetItem(query.value("State").toString()));
        ui->flightInfoTable->setItem(row, 6, new QTableWidgetItem(query.value("Name").toString()));
        ui->flightInfoTable->setItem(row, 7, new QTableWidgetItem(query.value("ID_Number").toString()));

        QString state = query.value("State").toString();
        if(state == "待支付") {
            QTableWidgetItem *item = ui->flightInfoTable->item(row, 5);
            if (item) {
                // 设置单元格文本的颜色
                item->setData(Qt::ForegroundRole, QBrush(QColorConstants::Svg::orange));
            }
        }
        if(state == "已支付") {
            QTableWidgetItem *item = ui->flightInfoTable->item(row, 5);
            if (item) {
                // 设置单元格文本的颜色
                item->setData(Qt::ForegroundRole, QBrush(QColorConstants::Svg::green));
            }
        }
        if(state == "已改签") {
            QTableWidgetItem *item = ui->flightInfoTable->item(row, 5);
            if (item) {
                // 设置单元格文本的颜色
                item->setData(Qt::ForegroundRole, QBrush(QColorConstants::Svg::blue));
            }
        }
        if(state == "已退票") {
            QTableWidgetItem *item = ui->flightInfoTable->item(row, 5);
            if (item) {
                // 设置单元格文本的颜色
                item->setData(Qt::ForegroundRole, QBrush(QColorConstants::Svg::red));
            }
        }

        row++;
    }
}

void MyOrder::onFlightInfoTableClicked(int row, int column)
{
    // 检查是否是有效的行和列
    if (row < 0 || column < 0 || row >= ui->flightInfoTable->rowCount()) {
        qDebug() << "Invalid row or column!";
        return;
    }

    // 获取选中行的所有数据
    QStringList orderDetails;
    for (int col = 0; col < ui->flightInfoTable->columnCount(); ++col) {
        QTableWidgetItem *item = ui->flightInfoTable->item(row, col);
        if (item) {
            orderDetails << item->text();
        } else {
            orderDetails << "";  // 如果单元格为空，插入空字符串
        }
    }

    // 获取选中行的订单号
    QTableWidgetItem *orderItem = ui->flightInfoTable->item(row, 0);  // 订单号在第0列
    QString orderId = orderItem ? orderItem->text() : ""; // 如果订单号为空，避免崩溃

    if (orderId.isEmpty()) {
        qDebug() << "Invalid orderId!";
        return;  // 如果订单号为空，则返回
    }
    QTableWidgetItem *statusItem = ui->flightInfoTable->item(row,5);
    QString orderStatus = statusItem ? statusItem->text() : ""; // 如果状态单元格为空，则返回空字符串
    if(orderStatus=="待支付")
    {
        ConfirmPurchase * conPay = new ConfirmPurchase(nullptr, orderId);
        connect(this, &MyOrder::closeMyOrder, conPay, &ConfirmPurchase::close);
        connect(conPay, &ConfirmPurchase::updateTicket, this, &MyOrder::loadOrders);
        conPay->setOrderDetails(orderDetails);  // 传递订单数据
        conPay->show();
    }
    else
    {
        // 跳转到 rescheduleticket 界面
        RescheduleTicket *rescheduleWindow = new RescheduleTicket(nullptr, orderId);
        connect(this, &MyOrder::closeMyOrder, rescheduleWindow, &RescheduleTicket::close);
        rescheduleWindow->setOrderDetails(orderDetails);  // 传递订单数据
        connect(rescheduleWindow, &RescheduleTicket::updateticket, this, &MyOrder::loadOrders);
        // 显示窗口
        rescheduleWindow->show();
    }
}

void MyOrder::closeEvent(QCloseEvent *event) {
    emit closeMyOrder();
    QWidget::closeEvent(event);
}

