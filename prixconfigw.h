#ifndef PRIXCONFIGW_H
#define PRIXCONFIGW_H

#include <QMainWindow>
#include <QString>
#include <QtSql>
#include <QMessageBox>
#include <vector>
#include <QLabel>
#include <QSpinBox>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QTextEdit>
#include <QObjectList>
#include <QFrame>
#include <QSize>

using namespace std;




namespace Ui {
class PrixConfigW;
}

class PrixConfigW : public QMainWindow
{
    Q_OBJECT

    public:
        explicit PrixConfigW(QWidget *parent = 0);
        ~PrixConfigW();

private slots:
    void on_button_ok_clicked();

    void on_button_reset_clicked();

    void on_button_cancel_clicked();
    void activate_Desactivate_Button();

    void on_checkBox_show_en_cours_stateChanged(int arg1);

    void on_Add_categorie_clicked();

    void on_Add_soins_clicked();

private:
        Ui::PrixConfigW *ui;
        QSqlQuery  requete;
        QString query;
        QFrame *frame;

        QVBoxLayout *main_layout;

        vector<QLabel*> widget_label_soin;
        vector<QDoubleSpinBox*> widget_prix;
        vector<QLabel*> widget_old_prix;
        vector<QLabel*> widget_intitule;
        vector<QHBoxLayout*> widget_h_box;

        vector<QString> string_categorie;
        vector<int> id_soins;

        int taille;

    private:
        void initialisation(bool afficher);
        void update();
        void init_add();
        void add_List_Soins(QString intitule,QString categorie, double old_price, double price);
};

#endif // PRIXCONFIGW_H
