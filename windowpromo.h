#ifndef WINDOWPROMO_H
#define WINDOWPROMO_H

#include <QMainWindow>
#include <iostream>
#include <QtSql>
#include <QString>
#include <QStandardItemModel>
#include <QtSql>
#include <QMessageBox>
#include <QSqlQuery>
#include <vector>

using namespace std;

namespace Ui {class WindowPromo;}

class WindowPromo : public QMainWindow
{
    Q_OBJECT

    public:
        explicit WindowPromo(QWidget *parent = 0);
        ~WindowPromo();

    private slots:
        void on_cbbox_comportement_currentIndexChanged(int index);
        void on_add_button_clicked();
        void on_retour_buton_clicked();
        void appelOption(QModelIndex);

        void on_checkbox_show_disable_stateChanged(int arg1);

private:
        void maj_tab();

    private:
            Ui::WindowPromo *ui;
            QStringList list;
            QStringList list_soins;
            vector<int> id_soins; //tableau d'idordonnée associé a la liste de soin
            QStandardItemModel *model;
};

#endif // WINDOWPROMO_H
