#include "mainwindow.h"
#include "LoadWindow.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QApplication>

QSqlDatabase db; // 声明为全局变量

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    // 添加一个新的数据库连接，并指定一个唯一的连接名称
    db = QSqlDatabase::addDatabase("QSQLITE");

    // 设置数据库文件名
    db.setDatabaseName("D:/code2024/test/data/TICKET.db");
    if (!db.open()) {
        qDebug() << "Database Error: " << db.lastError().text();
    }
    MainWindow w;
    LoadWindow sw;
    sw.show();
    return a.exec();
}
