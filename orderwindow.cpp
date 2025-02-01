#include "orderwindow.h"
#include "ui_orderwindow.h"

OrderWindow::OrderWindow(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::OrderWindow)
{
    ui->setupUi(this);
    setLCD();
    setMaxSelectable(1);  // 初始化 maxSelectable = 1
}

OrderWindow::~OrderWindow()
{
    if (timer) {
        timer->stop();
        delete timer;
    }
    delete ui;
}

void OrderWindow::selectedPassenger(const QList<QStringList>& passengerInfo) {
    this->passengerInfo.clear();
    this->ID_Number.clear();
    this->passengerInfo = passengerInfo;

    // 得到ID_Number
    for (const QStringList& passenger : passengerInfo) {
        QString id_number = passenger.at(1);
        ID_Number.append(id_number);
        qDebug() << "Passenger's ID_Number: " << id_number;
    }
    qDebug() << ID_Number.length();
    setMaxSelectable(this->ID_Number.length());
}

void OrderWindow::setMaxSelectable(int max) {
    maxSelectable = max;
    qDebug() << " maxSelectable = " << maxSelectable;
    enforceMaxSelectable(nullptr);
}

// 确保当前选中状态符合最大选择数量
void OrderWindow::enforceMaxSelectable(QCheckBox* thisSel) {
    if(thisSel != nullptr) {
        if(thisSel->isChecked()) {
            isSelected.append(thisSel);
        }
        else {
            isSelected.removeOne(thisSel);
        }
    }

    qDebug() << "isSelected.size() = " << isSelected.size();

    while(isSelected.size() > maxSelectable) {
        isSelected.first()->setChecked(false);
        qDebug() << isSelected.front()->objectName();
        isSelected.removeFirst();
    }
}

void OrderWindow::setData(QString f_id, QString beLo, QString endLo, QString beTim, QString endTim, QString username) {
    ui->flight_ID->setText(f_id);
    ui->beginLocation->setText(beLo);
    ui->endLocation->setText(endLo);
    ui->beginTime->setText(beTim);
    ui->endTime->setText(endTim);
    this->username = username;

    // setPrice
    try {
        QString beginTimeText = ui->beginTime->text();
        QString endTimeText = ui->endTime->text();

        if (beginTimeText.length() < 15 || endTimeText.length() < 15) {
            // 处理错误：时间格式不正确
            qDebug() << "时间格式不正确";
            return;
        }

        int beginHour = beginTimeText.mid(11, 2).toInt();
        int endHour = endTimeText.mid(11, 2).toInt();
        int beginMinute = beginTimeText.mid(14, 2).toInt();
        int endMinute = endTimeText.mid(14, 2).toInt();

        int hour = (endHour + 24 - beginHour) % 24;
        int minute = endMinute - beginMinute;
        int passTime = hour * 60 + minute;
        int passPrice = passTime * 10;

        qDebug() << " 经过时间：" << hour << "小时" << minute << "分钟" << passTime << "分钟";
        this->price = QString::number(passPrice);
        qDebug() << " 价格：" << this->price;
        ui->showPrice->setText(this->price);
    } catch (const std::exception& e) {
        // 捕获并处理异常
        qDebug() << "发生异常：" << e.what();
    }
}

void OrderWindow::on_selectPassenger_clicked()
{
    Passenger *passenger=new Passenger;
    connect(passenger, &Passenger::passengersConfirmed, this, &OrderWindow::selectedPassenger);
    passenger->exec();
}

void OrderWindow::setLCD() {
    ui->countDown->setDigitCount(5);
    ui->countDown->setSegmentStyle(QLCDNumber::Filled);  // 设置LCD风格
    // 初始化倒计时时间为10分钟
    QTime time(0, 10, 0);
    remainTime = time;
    // 设置倒计时格式
    ui->countDown->setMode(QLCDNumber::Dec);
    ui->countDown->display(time.toString("mm:ss"));
    // 创建一个定时器
    timer = new QTimer(this);
    // 连接定时器的 timeout() 信号到 upDateTimer() 槽函数
    connect(timer, &QTimer::timeout, this, &OrderWindow::upDateTimer);
    timer->start(1000);  // 设置定时器每秒触发一次
}

void OrderWindow::upDateTimer() {
    if(remainTime <= QTime(0, 0, 0)) {
        timer->stop(); // 停止定时器
        ui->countDown->display("00:00"); // 显示倒计时结束
        QMessageBox::information(this, "倒计时结束", "订单失效！");
        this->close();
    }
    else {
        remainTime = remainTime.addSecs(-1);                  // 每次减少1秒
        ui->countDown->display(remainTime.toString("mm:ss")); // 显示倒计时
        if (remainTime <= QTime(0, 0, 10)) {
            ui->countDown->setStyleSheet("QLCDNumber { color: yellow; }");
        }
    }
}

// 生成订单ID
QString OrderWindow::generateOrder_ID(QString id) {
    QString str_id = id.right(2);

    quint64 timestamp_ms = QDateTime::currentMSecsSinceEpoch(); // 获取当前时间的毫秒时间戳
    // 使用 QString 的 arg 方法来格式化字符串
    QString formattedString = QString("%1%2").arg(str_id).arg(timestamp_ms, 7, 9, QChar('0'));
    // 如果组合后的长度超过9位，则截取最后9位
    if (formattedString.length() > 9) {
        formattedString = formattedString.right(9);
    }
    qDebug() << " Order_ID = " << formattedString;

    return formattedString;
}


void OrderWindow::on_sure_clicked()
{
    ui->note->setText("");
    bool isWorkable=true;
    if(!ID_Number.length()) {
        ui->note->setText("请选择乘机人");
        return;
    }
    QList<QString> seat;
    if(ui->checkBox->isChecked()) seat.append("A座");
    if(ui->checkBox_2->isChecked()) seat.append("B座");
    if(ui->checkBox_3->isChecked()) seat.append("C座");
    if(ui->checkBox_4->isChecked()) seat.append("D座");
    if(ui->checkBox_5->isChecked()) seat.append("E座");
    if(!ID_Number.length()) {
        ui->note->setText("请选择乘机人");
        return;
    }
    if(seat.length() != ID_Number.length()) {
        qDebug() << seat.length() << " != " << ID_Number.length();
        ui->note->setText("请选择相应数量的座位");
        return;
    }
    if(!ui->rbtn_wechat->isChecked() && !ui->rbtn_zhifubao->isChecked() && !ui->rbtn_creditcard->isChecked()) {
        ui->note->setText("请选择支付方式");
        return;
    }

    // 将数据写入数据库
    QSqlQuery query(db);
    QString firstid = ID_Number.first();
    qDebug() << "first ID_Number = " << firstid;
    QString order_ID = generateOrder_ID(firstid);
    QString state = "待支付";


    // 创建插入语句...
    // 对于 idNumbers 列表中的每个 ID_Number，执行插入操作
    query.prepare("INSERT INTO Orders (Order_ID, Flight_ID, BeginTime, User_Name, ID_Number, Price, Seat, State) "
                  "VALUES (:order_ID, :flight_ID, :beginTime, :username, :id_Number, :price, :seat, :state)");
    for(int i = 0; i < ID_Number.length(); i++) {
        query.prepare("INSERT INTO Orders (Order_ID, Flight_ID, BeginTime, User_Name, ID_Number, Price, Seat, State) "
                      "VALUES (:order_ID, :flight_ID, :beginTime, :username, :id_Number, :price, :seat, :state)");

        query.bindValue(":order_ID", order_ID);
        query.bindValue(":flight_ID", ui->flight_ID->text());
        query.bindValue(":beginTime", ui->beginTime->text());
        query.bindValue(":username", username);
        query.bindValue(":id_Number", ID_Number[i]);
        query.bindValue(":price", price);
        query.bindValue(":seat", seat[i]);
        query.bindValue(":state", state);

        if (!query.exec()) {
            qDebug() << "Insert failed: " << query.lastError().text();
            ui->note->setText("生成订单失败: " + query.lastError().text());
            isWorkable = false;
            break;
        }
    }

    if (isWorkable) {
        PurchaseWindow* purchase = new PurchaseWindow(nullptr,order_ID);
        connect(this, &OrderWindow::closethis, purchase, &PurchaseWindow::close);
        connect(purchase, &PurchaseWindow::purchaseWindowClosed, this, &OrderWindow::close);
        purchase->show();
    }
    else {
        ui->note->setText("生成订单失败");
    }
}

void OrderWindow::closeEvent(QCloseEvent *event) {
    emit closethis();
    QWidget::closeEvent(event);
}

void OrderWindow::on_checkBox_clicked()
{
    enforceMaxSelectable(ui->checkBox);
}

void OrderWindow::on_checkBox_2_clicked()
{
    enforceMaxSelectable(ui->checkBox_2);
}

void OrderWindow::on_checkBox_3_clicked()
{
    enforceMaxSelectable(ui->checkBox_3);
}

void OrderWindow::on_checkBox_4_clicked()
{
    enforceMaxSelectable(ui->checkBox_4);
}

void OrderWindow::on_checkBox_5_clicked()
{
    enforceMaxSelectable(ui->checkBox_5);
}
