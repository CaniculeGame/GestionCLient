#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include <iostream>
#include <QtSql>
#include <QString>
#include <QStandardItemModel>
#include <QStringList>
#include <QModelIndex>
#include "dialog.h"
#include "mdpdialog.h"
#include <QCheckBox>
#include <QDate>
#include <QRegExp>


#define q2c(string) string.toStdString()
#define MAX_SANTE 6

namespace Ui {class MainWindow;}
using namespace std;

class MainWindow : public QMainWindow
{
    Q_OBJECT

    public:
        explicit MainWindow(QWidget *parent = 0);
        int Save();
        void Reinit();
        void Recherche();
        void initModel();
        void Suppr();
        bool TestChamp();
        ~MainWindow();

    private slots:
        void on_creationBouton_clicked();
        void on_quitterFormButton_clicked();
        void on_sauvegarderBouton_clicked();
        void on_razBouton_clicked();
        void on_rechercherBouton_clicked();
        void on_voirTTClientBoutton_clicked();
        void appelFicheClient(QModelIndex);
        void modif_id_select(QModelIndex id);
        void on_pushButton_supprClient_clicked();
        void on_nomEdit_textChanged(const QString &arg1);
        void on_prenomEdit_textEdited(const QString &arg1);
        void on_lineEditAge_textEdited(const QString &arg1);
        void on_lineEditMobile_textEdited(const QString &arg1);
        void on_lineEdit_TEl_textEdited(const QString &arg1);
        void on_lineEdit_Email_textEdited(const QString &arg1);
        void on_pushButton_modifPrix_clicked();
        void on_button_gestion_clicked();

        void on_button_io_clicked();

        void on_button_options_clicked();

        void on_button_stock_clicked();

        void on_quit_button_clicked();

        void on_calendar_button_clicked();

private:
        Ui::MainWindow *ui;
        int index;
        int index_max;
        int id_selec;
        QSqlDatabase db;
        QStandardItemModel *model;


};

#endif // MAINWINDOW_H
