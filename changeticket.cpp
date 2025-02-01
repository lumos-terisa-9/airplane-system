#include "changeticket.h"
#include "ui_changeticket.h"
#include "global.h"

ChangeTicket::ChangeTicket(QDate &orderdate,QWidget *parent, const QString &orderId)
    : QWidget(parent), ui(new Ui::ChangeTicket), currentOrderId(orderId),orderdate(orderdate)
{
    ui->setupUi(this);

    qDebug() << "changeticket has been created";

    qDebug() << "orderId in changeTicket:" << currentOrderId;

    initTableWidget();

    // 连接表格的单元格点击事件
    connect(ui->ticketTable, &QTableWidget::cellClicked, this, &ChangeTicket::on_ticketRow_clicked);
    qDebug()<<"orderdate"<<orderdate;
    ui->beginDate->setDate(orderdate);

    // 设置 beginTime (QDateEdit) 的属性，允许用户手动输入日期
    ui->beginDate->setCalendarPopup(true);  // 显示日历弹窗
    ui->beginDate->setDisplayFormat("yyyy-MM-dd");  // 设置日期显示格式
    connect(ui->beginDate, &QDateEdit::dateChanged, this, &ChangeTicket::on_dateChanged);

}


ChangeTicket::~ChangeTicket()
{
    delete ui;
}

void ChangeTicket::setTicketTableData(const QList<QList<QString>> &ticketData) {
    int rowCount = ticketData.size();
    if (rowCount == 0) {
        qDebug() << "No data to display in the table.";
        return;
    }

    int columnCount = ticketData.first().size() + 1; // 多一列用于复选框
    ui->ticketTable->setRowCount(rowCount);
    ui->ticketTable->setColumnCount(columnCount);

    // 设置表头列标题
    QStringList headers;
    headers << "选择" << "航班号" << "出发地" << "目的地" << "出发时间" << "到达时间";
    ui->ticketTable->setHorizontalHeaderLabels(headers);

    // 填充表格数据
    for (int row = 0; row < rowCount; ++row) {
        // 为每一行的第0列添加复选框
        QCheckBox *checkBox = new QCheckBox();
        QWidget *checkBoxWidget = new QWidget();
        QHBoxLayout *layout = new QHBoxLayout(checkBoxWidget);
        layout->addWidget(checkBox);
        layout->setAlignment(Qt::AlignCenter); // 居中对齐
        layout->setContentsMargins(0, 0, 0, 0); // 无边距
        checkBoxWidget->setLayout(layout);
        ui->ticketTable->setCellWidget(row, 0, checkBox);

        // 连接复选框的状态变化信号到槽函数
        connect(checkBox, &QCheckBox::stateChanged, this, [this, row](int state) {
            if (state == Qt::Checked) {
                qDebug() << "Row" << row << "selected.";
                // 高亮显示选中行
                ui->ticketTable->selectRow(row);
                // 取消其他行的勾选
                for (int i = 0; i < ui->ticketTable->rowCount(); ++i) {
                    if (i != row) {
                        QCheckBox *otherCheckBox = qobject_cast<QCheckBox *>(ui->ticketTable->cellWidget(i, 0));
                        if (otherCheckBox && otherCheckBox->isChecked()) {
                            otherCheckBox->setChecked(false); // 取消其他行的勾选
                        }
                    }
                }
            } else {
                qDebug() << "Row" << row << "deselected.";
                // 取消高亮显示
                ui->ticketTable->setRangeSelected(QTableWidgetSelectionRange(row, 0, row, ui->ticketTable->columnCount() - 1), false);
            }
        });

        // 填充其他列数据
        for (int col = 0; col < columnCount - 1; ++col) {
            QString value = ticketData[row][col];
            ui->ticketTable->setItem(row, col + 1, new QTableWidgetItem(value));
        }
    }
    filterFlightsByDate(ui->beginDate->date());

    // 自动调整列宽，使其填满整个表格
    ui->ticketTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    // 设置表格内容居中
    ui->ticketTable->setEditTriggers(QAbstractItemView::NoEditTriggers); // 禁止编辑
    ui->ticketTable->setSelectionBehavior(QAbstractItemView::SelectRows); // 整行选择
    ui->ticketTable->setSelectionMode(QAbstractItemView::SingleSelection); // 单行选择
    ui->ticketTable->setStyleSheet("QTableWidget::item { text-align: center; }");

    // 强制刷新表格
    ui->ticketTable->viewport()->update();
}

void ChangeTicket::on_ticketRow_clicked(int row, int column) {
    if (column < 1) {
        return; // 点击的是复选框列，不做处理
    }

    QCheckBox *checkBox = qobject_cast<QCheckBox *>(ui->ticketTable->cellWidget(row, 0));
    if (!checkBox) {
        return;
    }

    // 如果点击的行的复选框没被选中，则选中它
    if (!checkBox->isChecked()) {
        checkBox->setChecked(true);
    }

    // 取消其他行的选中状态
    for (int i = 0; i < ui->ticketTable->rowCount(); ++i) {
        if (i != row) {
            QCheckBox *otherCheckBox = qobject_cast<QCheckBox *>(ui->ticketTable->cellWidget(i, 0));
            if (otherCheckBox && otherCheckBox->isChecked()) {
                otherCheckBox->setChecked(false); // 取消其他行的勾选
            }
        }
    }

    // 获取当前被点击行的航班信息
    QString flightId = ui->ticketTable->item(row, 1)->text(); // 航班号在第1列
    QString departureCity = ui->ticketTable->item(row, 2)->text(); // 出发地在第2列
    QString arrivalCity = ui->ticketTable->item(row, 3)->text(); // 目的地在第3列
    QString departureTime = ui->ticketTable->item(row, 4)->text(); // 出发时间在第4列
    QString arrivalTime = ui->ticketTable->item(row, 5)->text(); // 落地时间在第5列

}

void ChangeTicket::initTableWidget() {
    // 初始化表格时设置列数
    ui->ticketTable->setColumnCount(6); // 6列：复选框 + 5列航班信息
    QStringList headers;
    headers << "选择" << "航班号" << "出发地" << "目的地" << "出发时间" << "到达时间";
    ui->ticketTable->setHorizontalHeaderLabels(headers);

    // 设置复选框列的宽度
    ui->ticketTable->setColumnWidth(0, 50); // 设置复选框列宽度
    ui->ticketTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
}

void ChangeTicket::on_dateChanged(const QDate &selectedDate)
{
    qDebug() << "Date selected/entered:" << selectedDate.toString("yyyy-MM-dd");

    // 筛选表格中符合条件的航班数据
    filterFlightsByDate(selectedDate);
}


void ChangeTicket::filterFlightsByDate(const QDate &date)
{
    int rowCount = ui->ticketTable->rowCount();

    for (int row = 0; row < rowCount; ++row) {
        // 假设出发时间在第3列，格式为 "yyyy-MM-dd hh:mm"
        QString departureDateTimeStr = ui->ticketTable->item(row, 4)->text();

        // 提取出发时间的日期部分
        QDate departureDate = QDate::fromString(departureDateTimeStr.left(10), "yyyy-MM-dd");

        // 检查日期是否匹配
        bool isMatch = (departureDate == date);
        ui->ticketTable->setRowHidden(row, !isMatch); // 隐藏不匹配的行
    }
}

void ChangeTicket::on_confirmButton_clicked()
{
    QSqlQuery query(db);
    if (!db.open()) {
        qDebug() << "Failed to open the database: " << db.lastError().text();
    }

    for (int row = 0; row < ui->ticketTable->rowCount(); ++row) {
        QCheckBox *checkBox = qobject_cast<QCheckBox *>(ui->ticketTable->cellWidget(row, 0));
        if (!checkBox) {
            qDebug() << "No checkbox found in row " << row;
            continue;
        }
    }

    bool isAnyFlightSelected = false;

    // 遍历所有行，检查复选框是否选中
    for (int row = 0; row < ui->ticketTable->rowCount(); ++row) {
        QCheckBox *checkBox = qobject_cast<QCheckBox *>(ui->ticketTable->cellWidget(row, 0));  // 获取复选框
        if (checkBox && checkBox->isChecked()) {  // 如果复选框被选中
            isAnyFlightSelected = true;
            // 获取该行航班信息
            QString flightId = ui->ticketTable->item(row, 1)->text();
            QString departureCity = ui->ticketTable->item(row, 2)->text();
            QString arrivalCity = ui->ticketTable->item(row, 3)->text();
            QString departureTime = ui->ticketTable->item(row, 4)->text();
            QString arrivalTime = ui->ticketTable->item(row, 5)->text();

            // 输出要更新的航班信息
            qDebug() << "Updating Order with flight: " << flightId << departureCity << arrivalCity << departureTime << arrivalTime;

            // 更新数据库中 Orders 表的相应订单
            QSqlQuery query(db);
            query.prepare("UPDATE Orders SET Flight_ID = :flightId, BeginTime = :departureTime, State = :state "
                          "WHERE Order_ID = :orderId");  // 根据实际情况更新 WHERE 子句

            query.bindValue(":flightId", flightId);
            query.bindValue(":departureCity", departureCity);
            query.bindValue(":arrivalCity", arrivalCity);
            query.bindValue(":departureTime", departureTime);
            //query.bindValue(":arrivalTime", arrivalTime);
            query.bindValue(":orderId", currentOrderId);  // 假设 globalOrderId 是当前订单的 ID
            query.bindValue(":state", "已改签");

            qDebug() << "update orderId: " << currentOrderId;

            qDebug() << "Executed query:" << query.lastQuery();
            qDebug() << "Bound values:" << query.boundValues();


            if (query.exec()) {
                qDebug() << "Order updated successfully!";
            } else {
                qDebug() << "Failed to update order: " << query.lastError().text();
            }
        }
    }

    if (!isAnyFlightSelected) {
        // 如果没有选择任何航班，在QLabel中显示提示信息
        ui->note->setText("请选择一个航班");
        return;
    }
    else {
        ui->note->setText("改签成功");
        // 创建一个 QTimer 对象
        QTimer::singleShot(2000, this, &ChangeTicket::close);  // 3000 毫秒后关闭窗口，即 3 秒
    }

    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, [this]() {
        emit closeChange();
        this->close();
    });
    timer->start(1500);  // 设置定时器超时时间为1.5秒
}


void ChangeTicket::on_cancelButton_clicked()
{
    // 在 QLabel 中显示 "改签已取消"
    ui->note->setText("改签已取消");

    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, [this]() {
        emit closeChange();
        this->close();
    });
    timer->start(1500);  // 设置定时器超时时间为1.5秒
}



