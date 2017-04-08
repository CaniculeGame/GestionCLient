#ifndef MDPDIALOG_H
#define MDPDIALOG_H

#include <QDialog>
#include <QString>
#include <QtSql>
#include <QMessageBox>
#include <QSettings>
#include <QByteArray>
#include <string>
#include <iostream>

namespace Ui {class MdpDialog;}
using namespace std;

class MdpDialog : public QDialog
{
    Q_OBJECT

    public:
        explicit MdpDialog(QWidget *parent = 0);
        ~MdpDialog();

    private slots:
        void on_pushButton_connection_clicked();

        void on_pushButton_reinit_clicked();

private:
            Ui::MdpDialog *ui;
            QSqlDatabase db;
            QSettings *set;
};

#endif // MDPDIALOG_H
