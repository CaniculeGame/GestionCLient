#ifndef DIALOG_H
#define DIALOG_H


#include <QDialog>
#include <QtSql>
#include <QMessageBox>
#include <QSqlQuery>
#include <iostream>
#include <QStandardItemModel>
#include <QDate>
#include <vector>

#include "graphfidelite.h"
#include "dialogprestation.h"

using namespace std;



namespace Ui {class Dialog;}

class Dialog : public QDialog
{
    Q_OBJECT

    public:
        explicit Dialog(QWidget *parent = 0);
        Dialog(int id, QWidget *parent);
        ~Dialog();

        bool Save();
        void Suppr();
        void Recharger();
        void set_Fidelite(int nb);
        int get_Fidelite();


    private slots:
        void on_pushButton_retour_clicked();
        void on_pushButton_OK_clicked();
        void on_pushButton_SAVE_clicked();
        void appelDetailPrestation(QModelIndex index);
        void on_ajoutPrestation_clicked();
        void on_pushButton_suppr_clicked();
        void on_lineEdit_nom_textChanged(const QString &arg1);
        void on_lineEdit_prenom_textChanged(const QString &arg1);
        void on_lineEdit_age_textChanged(const QString &arg1);
        void on_lineEdit_mobile_textChanged(const QString &arg1);
        void on_lineEdit_tel_textChanged(const QString &arg1);
        void on_lineEdit_mail_textChanged(const QString &arg1);

        void on_adresse_textEdit_textChanged();
        void on_checkBox_newsletter_stateChanged(int arg1);
        void on_checkBox_type_0_stateChanged(int arg1);
        void on_checkBox_type_1_stateChanged(int arg1);
        void on_checkBox_type_2_stateChanged(int arg1);
        void on_checkBox_type_3_stateChanged(int arg1);
        void on_checkBox_sante_0_stateChanged(int arg1);
        void on_checkBox_sante_1_stateChanged(int arg1);
        void on_checkBox_sante_2_stateChanged(int arg1);
        void on_checkBox_sante_3_stateChanged(int arg1);
        void on_checkBox_sante_4_stateChanged(int arg1);

    private:
        Ui::Dialog *ui;
        QSqlDatabase db;
        QStandardItemModel *model;
        int id_client=0;
        vector<int> liste;
        bool modif;

       int nb_fidelite;
       int nb_visite;

       GraphFidelite *fidelite;

};

#endif // DIALOG_H
