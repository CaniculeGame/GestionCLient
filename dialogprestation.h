#ifndef DIALOGPRESTATION_H
#define DIALOGPRESTATION_H

#include <QDialog>
#include <QtSql>
#include <QString>
#include <QMessageBox>
#include <QStandardItem>
#include <QStandardItemModel>
#include <QStringList>
#include <list>
#include "listesoins.h"
#include <iostream>
#include <QDate>
#include <QPrinter>
#include <QStringList>



namespace Ui {class DialogPrestation;}
using namespace std;


struct InfoPromotion
{
    int id; //id dans le tableau ou id promotion (depend du context)
    int comportement_id; //total, min,max, soin
    int id_produit; //porduit pour comportement soin
    double valeur; //pourcentage reduc
    QString intitule;  //nom
    QString comportement_intitule; //nom du comportemetn
};

class DialogPrestation : public QDialog
{
    Q_OBJECT

    public:
        explicit DialogPrestation(QWidget *parent = 0);
         DialogPrestation(int index,QWidget *parent = 0);
        ~DialogPrestation();


    private:
        Ui::DialogPrestation *ui;

        int id; //id du client
        QModelIndex index_;
        int prest_; //index de la prestation

        QString nom_;
        QString prenom_;

        double prix_ss_reduc;
        double prixT;
        int nb;
        QSqlQuery  requete;
        QSqlQuery  requete2;
        QStandardItemModel *modele;
        QStringList listeSoins;
        QStringListModel *model;
        vector<ListeSoins> listeS; //liste des soins appliqués
        vector<int> promotions_tab; //tableau des promotions appliquées
        vector<int> id_soins; //tableau d'id ordonnée associé a la liste de soin

        vector<InfoPromotion> info_promo_existantes;
        QStringList comportement_intitule; //tableau ordonné

    private:
        void add_Combo_Box();


    private slots:
        void ajoutElement(QModelIndex);
        void supprElement(QModelIndex);
        void on_pushButton_init_clicked();
        void on_pushButton_Annuler_clicked();
        void on_pushButton_OK_clicked();
        void print(QPrinter *p);
        void on_button_ajout_promo_clicked();
        void on_cbox_currentIndexChanged(int index);
        void on_clicked_del();


};

#endif // DIALOGPRESTATION_H
