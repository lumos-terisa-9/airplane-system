#ifndef GLOBAL_H
#define GLOBAL_H

#include <QSqlDatabase>
#include <QDate>

extern QSqlDatabase db; // 声明为外部链接
extern QString globalUsername;
extern QDate selectTime;
#endif // GLOBAL_H
