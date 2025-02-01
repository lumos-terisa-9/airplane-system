#ifndef CONFIRMPURCHASE_H
#define CONFIRMPURCHASE_H

#include <QDialog>
#include <QMessageBox>
#include "purchasewindow.h"
#include "global.h"

namespace Ui {
class ConfirmPurchase;
}

class ConfirmPurchase : public QDialog
{
    Q_OBJECT

public:
    explicit ConfirmPurchase(QWidget *parent = nullptr, const QString &orderId = "");
    ~ConfirmPurchase();
    void setOrderDetails(const QStringList &details);
    void showNotPay();

protected:
    void closeEvent(QCloseEvent *event);

signals:
    void updateTicket();            // 刷新票信息
    void closethis();

private slots:
    void on_btn_confirm_clicked();

    void on_btn_abandon_clicked();

private:
    Ui::ConfirmPurchase *ui;
    QString newState;
    QString orderId;
    void updateOrderState();  // 更新订单状态为已退票
};

#endif // CONFIRMPURCHASE_H
