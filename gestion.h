#ifndef GESTION_H
#define GESTION_H


#include <QMainWindow>
#include <QtSql>
#include <QSqlQuery>
#include <vector>
#include <QStandardItemModel>

using namespace std;
namespace Ui {class Gestion;}


struct info_produit
{
    int numero_produit;
    int nb_client;
    int produit_utilise;
    double prix; //cout de revien par cliente
};


class Gestion : public QMainWindow
{
    Q_OBJECT

    public:
        explicit Gestion(QWidget *parent = 0);
        ~Gestion();

    private:
        void button_stat_Changed(int numero);
        void chiffre_affaire();
        void frequentation();
        void chiffre_affaire_variation();
        void frequentation_variation();
        void init_annee();
        void init_color();
        void benefice();
        void depense();

    private slots:
        void on_button_ca_clicked();
        void on_button_frequentation_clicked();
        void on_button_retour_clicked();
        void on_button_benefice_clicked();
        void on_buton_depense_clicked();
        void on_checkbox_variation_stateChanged(int arg1);
        void on_checkbox_couleur_stateChanged(int arg1);
        void on_button_print_clicked();
        void on_checkbox_var_couleur_stateChanged(int arg1);

        void on_reel_button_clicked();

        void on_reel_button_stateChanged(int arg1);

private:
        Ui::Gestion *ui;
        int selection_stat=0;
        QSqlQuery  requete;
        QStandardItemModel *model_ca;       //affichage ca
        QStandardItemModel *model_freq;     //affichage freq
        QStandardItemModel *model_ca_var;   //affichage ca avec les variation en %
        QStandardItemModel *model_freq_var; //affichage freq avec les varaiations en %
        QStandardItemModel *model_depense;  //affichage toutes les dépense
        QStandardItemModel *model_benefice; //affiche les bénéfices


        int size=0; //nombre d'année répertorié
        vector<int> numero_annees_; //toute les dates
        vector<info_produit> produits;  //contient tous les porduits

};

#endif // GESTION_H
