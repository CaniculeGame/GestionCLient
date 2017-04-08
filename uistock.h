#ifndef UISTOCK_H
#define UISTOCK_H

#include <QMainWindow>
#include <QFrame>
#include <QVBoxLayout>
#include <vector>
#include <QLabel>
#include <QSpinBox>
#include <QDoubleSpinBox>

using namespace std;


namespace Ui {class UIStock;}

class UIStock : public QMainWindow
{
    Q_OBJECT

    public:
        explicit UIStock(QWidget *parent = 0);
        ~UIStock();

    private slots:
        void on_add_produit_clicked();
        void on_return_button_clicked();
        void on_reinit_button_clicked();
        void on_accepter_button_clicked();
        void on_reinit_all_button_clicked();
        void on_click_button();


    private:
        void initialisation_List_Soins();
        void initialisation_List_Produit();
        void init_Ajout();

    private:

        Ui::UIStock *ui;
        QFrame *frame;
        QVBoxLayout *main_layout;

        vector<double> widget_old_prix;
        vector<int> widget_old_restant;
        vector<int> nb_prod_utilise;
        vector<int> id;


        vector<QSpinBox*> spin_box_nb; //tous les prosuits qui sont en stock
        vector<QDoubleSpinBox*> spinbox_prix; //tous les prix
        vector<QLabel*> moyenne_label; //toutes les moyennes = cliente / pot
        vector<QLabel*> spin_box_utilise; //affiche les produits utilisés
        vector<QLabel*> prix_par_client; //prix/cliente

};

#endif // UISTOCK_H
