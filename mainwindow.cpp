#include "mainwindow.h"
#include "qcalendarwidget.h"
#include "ui_mainwindow.h"
void MainWindow::loadCitiesToComboBox() {
    // 假设 db 是一个有效的 QSqlDatabase 对象
    QSqlQuery query(db);
    query.prepare("SELECT City_Name FROM Cities"); // 确认 Cities 和 CityName 是正确的
    if (query.exec()) {
        while (query.next()) {
            QString cityName = query.value(0).toString();
            ui->beginLocation->addItem(cityName); // 假设 beginLocation 是 MainWindow 中的一个 QComboBox
            ui->endLocation->addItem(cityName);   // 假设 endLocation 是 MainWindow 中的另一个 QComboBox
            ui->Check_beginLocation->addItem(cityName);
            ui->Check_endLocation->addItem(cityName);
        }
    } else {
        QMessageBox::critical(this, "查询错误", "查询失败: " + query.lastError().text());
    }

}


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->tabWidget->setCurrentIndex(0); // 默认显示第一个标签页“首页”
    QDate currentDate = QDate::currentDate();
    ui->beginDate->setDate(currentDate);
    // 连接按钮点击信号与槽函数
    connect(ui->logOut, &QPushButton::clicked, this, &MainWindow::on_logOut_clicked);
    timetable = new timeTable(); // 确保 timetable 是 MainWindow 的成员变量
    connect(this, &MainWindow::logoutRequested, timetable, &MainWindow::close);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::showEvent(QShowEvent *event)
{
    Q_UNUSED(event) // 如果你不需要使用 event 参数，可以使用 Q_UNUSED 宏来避免编译器警告
    // 设置当前日期到 QDateEdit 控件
    ui->beginDate->setDate(QDate::currentDate());
    QMainWindow::showEvent(event);
}

void MainWindow::on_exchangeLocation_clicked()
{
    QString text1 = ui->beginLocation->currentText();
    QString text2 = ui->endLocation->currentText();
    ui->beginLocation->setEditText(text2);
    ui->endLocation->setEditText(text1);
}

void MainWindow::setUsername(const QString& username) {
    this->username = username;
    globalUsername = username;
    ui->label_4->setText(username);
}


void MainWindow::on_MyPassenger_clicked()
{
    Passenger *passenger=new Passenger;
    connect(this, &MainWindow::logoutRequested, passenger, &Passenger::close);
    passenger->show();
}


void MainWindow::on_searchTicket_clicked()
{
    QString beginText = ui->beginLocation->currentText();
    QString endText = ui->endLocation->currentText();

    int beginIndex = ui->Check_beginLocation->findText(beginText);
    int endIndex = ui->Check_endLocation->findText(endText);

    if(beginIndex != -1) {
        // 让首页设置的查询地点成为当前项
        ui->Check_beginLocation->setCurrentIndex(beginIndex);
    }
    if(endIndex != -1) {
        ui->Check_endLocation->setCurrentIndex(endIndex);
    }

    ui->tabWidget->setCurrentIndex(1); // 显示第二个标签页“查询”
    check_show();
}


void MainWindow::check_show() {
    QSqlQuery query(db);
    //准备查询语句
    QString beginLocation = ui->Check_beginLocation->currentText();
    QString endLocation = ui->Check_endLocation->currentText();
    QString sqlQuery="SELECT Flight_ID, BeginLocation, BeginTime, EndTime FROM Flights WHERE BeginLocation=:beginLocation AND EndLocation=:endLocation";

    // 准备查询
    if (!query.prepare(sqlQuery)){
        qDebug() << "查询准备失败：" << query.lastError();
    }
    else {
        //绑定用户名参数
        query.bindValue(":beginLocation", beginLocation);
        query.bindValue(":endLocation", endLocation);
        //执行查询
        if(!query.exec()){
            qDebug() << "查询失败：" << query.lastError();
        }
        else{
            // 清空
            ui->flights->clear();
            // 遍历
            while (query.next()) {
                // 获取起飞时间
                QString beginTime = query.value("BeginTime").toString();
                // 获取起飞日期
                QString dateFormat = "yyyy/MM/dd";                              // 自定义日期格式
                QString date = ui->beginDate->dateTime().toString(dateFormat);  // 转换为自定义格式的字符串
                QChar newChar = '/';
                QString beginDate = beginTime.left(4) + newChar + beginTime.mid(5, 2) + newChar + beginTime.mid(8, 2);

                beginDate = beginDate.left(date.size());
                if(date != beginDate) continue;                                 // 如果数据不是要查询的日期的，就跳过

                // 获取航班ID
                QString flight_ID = query.value("Flight_ID").toString();
                // 获取降落时间
                QString endTime = query.value("EndTime").toString();

                // 创建一个项
                QString str_item = flight_ID + " ^_^ " + beginLocation + " --> " + endLocation + " ^_^ " + beginTime + " --> " + endTime + " ";
                QListWidgetItem *item = new QListWidgetItem(str_item);
                // 将项添加到列表中
                ui->flights->addItem(item);
            }
        }
    }
}

void MainWindow::on_flights_itemClicked(QListWidgetItem *item)
{
    QString text = item->text();
    int start = 0, count = 0;
    QString flight_ID, beginLocation, endLocation, beginTime, endTime;
    beginLocation = ui->Check_beginLocation->currentText();
    endLocation = ui->Check_endLocation->currentText();

    for(int i=0; i<text.size(); i++) {
        if(text.at(i) == ' ') {
            if(start) {
                switch(count) {       // 1:"^_^"  3:"-->"  5:"^_^"  8:"-->"
                case 6 : beginTime += text.mid(start + 1, i - start - 1); break;
                case 7 : beginTime += text.mid(start, i - start); break;
                case 9 : endTime += text.mid(start + 1, i - start - 1); break;
                case 10 : endTime += text.mid(start, i - start); break;
                }
            }
            else {
                flight_ID = text.left(i);
            }
            start = i;
            count++;
        }
    }
    OrderWindow* order = new OrderWindow;
    connect(this, &MainWindow::logoutRequested, order, &OrderWindow::close);
    order->setData(flight_ID, beginLocation, endLocation, beginTime, endTime, username);
    order->show();
}


void MainWindow::on_logOut_clicked()
{
    emit logoutRequested(); // 发射退出登录信号
    this->close();
}

// 添加槽函数
void MainWindow::updateBeginDate(const QDate &date)
{
    ui->beginDate->setDate(date);
    this->update();
    // 当日期被更新后，调用 check_show() 来刷新航班信息
    check_show();
}

void MainWindow::on_showTime_clicked()
{
    this->setUpdatesEnabled(true);
    connect(timetable, &timeTable::dateSelected, this, &MainWindow::updateBeginDate);
    timetable->show();
}


void MainWindow::on_modify_clicked()
{
    moduser=new modifyUser(nullptr,username);
    connect(this, &MainWindow::logoutRequested, moduser, &modifyUser::close);
    moduser->show();
}


void MainWindow::on_Check_beginLocation_currentTextChanged(const QString &arg1)
{
    QSqlQuery query(db);
    //准备查询语句
    QString beginLocation = ui->Check_beginLocation->currentText();
    QString endLocation = ui->Check_endLocation->currentText();
    QString sqlQuery="SELECT Flight_ID, BeginLocation, BeginTime, EndTime FROM Flights WHERE BeginLocation=:beginLocation AND EndLocation=:endLocation";

    // 准备查询
    if (!query.prepare(sqlQuery)){
        qDebug() << "查询准备失败：" << query.lastError();
    }
    else {
        //绑定用户名参数
        query.bindValue(":beginLocation", beginLocation);
        query.bindValue(":endLocation", endLocation);
        //执行查询
        if(!query.exec()){
            qDebug() << "查询失败：" << query.lastError();
        }
        else{
            // 清空
            ui->flights->clear();

            // 遍历
            while (query.next()) {
                // 获取起飞时间
                QString beginTime = query.value("BeginTime").toString();
                // 获取起飞日期
                QString dateFormat = "yyyy/MM/dd";                              // 自定义日期格式
                QString date = ui->beginDate->dateTime().toString(dateFormat);  // 转换为自定义格式的字符串
                QChar newChar = '/';
                QString beginDate = beginTime.left(4) + newChar + beginTime.mid(5, 2) + newChar + beginTime.mid(8, 2);

                beginDate = beginDate.left(date.size());
                if(date != beginDate) continue;                                 // 如果数据不是要查询的日期的，就跳过

                // 获取航班ID
                QString flight_ID = query.value("Flight_ID").toString();
                // 获取降落时间
                QString endTime = query.value("EndTime").toString();

                // 创建一个项
                QString str_item = flight_ID + " ^_^ " + beginLocation + " --> " + endLocation + " ^_^ " + beginTime + " --> " + endTime + " ";
                QListWidgetItem *item = new QListWidgetItem(str_item);
                // 将项添加到列表中
                ui->flights->addItem(item);
            }
        }
}
}

void MainWindow::on_Check_endLocation_currentTextChanged(const QString &arg1)
{
    QSqlQuery query(db);
    //准备查询语句
    QString beginLocation = ui->Check_beginLocation->currentText();
    QString endLocation = ui->Check_endLocation->currentText();
    QString sqlQuery="SELECT Flight_ID, BeginLocation, BeginTime, EndTime FROM Flights WHERE BeginLocation=:beginLocation AND EndLocation=:endLocation";

    // 准备查询
    if (!query.prepare(sqlQuery)){
        qDebug() << "查询准备失败：" << query.lastError();
    }
    else {
        //绑定用户名参数
        query.bindValue(":beginLocation", beginLocation);
        query.bindValue(":endLocation", endLocation);
        //执行查询
        if(!query.exec()){
            qDebug() << "查询失败：" << query.lastError();
        }
        else{
            // 清空
            ui->flights->clear();
            // 遍历
            while (query.next()) {
                // 获取起飞时间
                QString beginTime = query.value("BeginTime").toString();
                // 获取起飞日期
                QString dateFormat = "yyyy/MM/dd";                              // 自定义日期格式
                QString date = ui->beginDate->dateTime().toString(dateFormat);  // 转换为自定义格式的字符串
                QChar newChar = '/';
                QString beginDate = beginTime.left(4) + newChar + beginTime.mid(5, 2) + newChar + beginTime.mid(8, 2);
                beginDate = beginDate.left(date.size());
                if(date != beginDate) continue;                                 // 如果数据不是要查询的日期的，就跳过

                // 获取航班ID
                QString flight_ID = query.value("Flight_ID").toString();
                // 获取降落时间
                QString endTime = query.value("EndTime").toString();

                // 创建一个项
                QString str_item = flight_ID + " ^_^ " + beginLocation + " --> " + endLocation + " ^_^ " + beginTime + " --> " + endTime + " ";
                QListWidgetItem *item = new QListWidgetItem(str_item);
                // 将项添加到列表中
                ui->flights->addItem(item);

            }
        }
    }

}


void MainWindow::on_pushButton_clicked()
{
    QString text1 = ui->Check_beginLocation->currentText();
    QString text2 = ui->Check_endLocation->currentText();
    ui->Check_beginLocation->setEditText(text2);
    ui->Check_endLocation->setEditText(text1);
}


void MainWindow::on_showTime_2_clicked()
{
    // 启用更新，这在大多数情况下是默认的，可能不需要显式调用
    this->setUpdatesEnabled(true);

    // 断开之前的信号连接，以避免重复连接
    disconnect(timetable, &timeTable::dateSelected, this, &MainWindow::updateBeginDate);

    // 连接信号到槽函数，并传递一个标识符，指明是查询页面的日期更新
    connect(timetable, &timeTable::dateSelected, this, &MainWindow::updateBeginDate);

    // 显示日历对话框
    timetable->show();
    // 注意：不要在这里调用 check_show()

}


void MainWindow::on_MyOrder_clicked() {

    MyOrder* myOrderWindow = new MyOrder();

    connect(this, &MainWindow::logoutRequested, myOrderWindow, &MyOrder::close);

    // 获取当前屏幕的几何信息
    QRect screenGeometry = QGuiApplication::primaryScreen()->geometry();
    int x = (screenGeometry.width() - this->width()) / 2;
    int y = (screenGeometry.height() - this->height()) / 2;

    // 将窗口放置到屏幕中央
    this->move(x, y);
    myOrderWindow->show();

}
