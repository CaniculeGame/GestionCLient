#include "uistock.h"
#include "ui_uistock.h"

#include <QtSql>
#include <QString>
#include <QMessageBox>
#include <QListWidgetItem>
#include <QStringList>
#include <QLayout>
#include <QLayoutItem>




#include <iostream>
using namespace std;

UIStock::UIStock(QWidget *parent) :QMainWindow(parent),ui(new Ui::UIStock)
{
    ui->setupUi(this);

    main_layout = new QVBoxLayout();
    frame= new QFrame();

    initialisation_List_Soins();
    initialisation_List_Produit();

    ui->accepter_button->setIcon(QIcon("image/save.png"));
    ui->reinit_all_button->setIcon(QIcon("image/Undo.png"));
    ui->reinit_button->setIcon(QIcon("image/Undo.png"));
    ui->return_button->setIcon(QIcon("image/Cancel.png"));
    ui->add_produit->setIcon(QIcon("image/add.png"));

}


UIStock::~UIStock()
{
    spin_box_nb.clear();
    spinbox_prix.clear();
    moyenne_label.clear();
    spin_box_utilise.clear();
    widget_old_prix.clear();
    widget_old_restant.clear();
    nb_prod_utilise.clear();
    id.clear();
    prix_par_client.clear();

    delete frame;
    delete main_layout;
    delete ui;
}


void UIStock::on_click_button()
{
    QObject* obj = sender();
    QStringList str = obj->objectName().split("_");
    int num_row = str[2].toInt();

    double value = spin_box_nb[num_row]->value();

    if(value>0)
    {
        spin_box_nb[num_row]->setValue( value-1);


        QString strg = spin_box_utilise[num_row]->text();
        value = strg.toInt();
        spin_box_utilise[num_row]->setText(QString::number(value+1));


        if(value>0)
        {
            moyenne_label[num_row]->setText(QString::number(nb_prod_utilise[num_row]/(value+1)));
        }
        else
        {
            moyenne_label[num_row]->setText(QString::number(nb_prod_utilise[num_row]));
        }

        if(moyenne_label[num_row]>0)
        {
            double a = widget_old_prix[num_row];
            double b = moyenne_label[num_row]->text().toDouble();

             prix_par_client[num_row]->setText(QString::number(a/b));

        }
        else
        {
            prix_par_client[num_row]->setText(QString::number(widget_old_prix[num_row]));
        }



    }


}

//réinitialise les valeurs pour l'ajout d'un produit
void UIStock::init_Ajout()
{
    int size = ui->soins_list->count();
    for(int i=0;i<size;i++)
    {
        ui->soins_list->item(i)->setCheckState(Qt::Unchecked);
    }

    ui->intitule_lineedit->setText("");
    ui->nb_acheter_spinbox->setValue(0);
    ui->prix_dspinbox->setValue(0);
}

//initialisation de la liste des produits
void UIStock::initialisation_List_Produit()
{
    QString    query;
    QSqlQuery  requete;

    QString query2;
    QSqlQuery requete2;

    QString name;



    QHBoxLayout *layout_label = new QHBoxLayout();
    QLabel *label_intitule= new QLabel();
    label_intitule->setText("Intitule");
    QLabel *label_prix_unitaire= new QLabel();
    label_prix_unitaire->setText("Prix");
    QLabel *label_nb_restant= new QLabel();
    label_nb_restant->setText("Restant");
    QLabel *label_button= new QLabel();
    label_button->setText("-1 produit");
    QLabel *label_nb_utilise= new QLabel();
    label_nb_utilise->setText("Utilisé");
    QLabel *label_nb_soin_unite= new QLabel();
    label_nb_soin_unite->setText("nb cliente");
    QLabel *label_cout_moyen= new QLabel();
    label_cout_moyen->setText("clt/prod"); //prix moyen
    QLabel *label_cout_median= new QLabel();
    label_cout_median->setText("prix/clt"); //prix moyen



    label_intitule->setFixedSize(QSize(100,20));
    label_prix_unitaire->setFixedSize(QSize(50,20));
    label_nb_restant->setFixedSize(QSize(50,20));
    label_button->setFixedSize(QSize(50,20));
    label_nb_utilise->setFixedSize(QSize(50,20));
    label_nb_soin_unite->setFixedSize(QSize(50,20));
    label_cout_moyen->setFixedSize(QSize(50,20));
    label_cout_median->setFixedSize(QSize(50,20));

    label_intitule->setAlignment(Qt::AlignLeft);
    label_prix_unitaire->setAlignment(Qt::AlignHCenter);
    label_nb_restant->setAlignment(Qt::AlignHCenter);
    label_button->setAlignment(Qt::AlignRight);
    label_nb_utilise->setAlignment(Qt::AlignRight);
    label_cout_moyen->setAlignment(Qt::AlignRight);
    label_cout_median->setAlignment(Qt::AlignRight);
    label_nb_soin_unite->setAlignment(Qt::AlignRight);


    layout_label->addWidget(label_intitule);
    layout_label->addWidget(label_prix_unitaire);
    layout_label->addWidget(label_nb_restant);
    layout_label->addWidget(label_button);
    layout_label->addWidget(label_nb_utilise);
    layout_label->addWidget(label_nb_soin_unite);
    layout_label->addWidget(label_cout_moyen);
    layout_label->addWidget(label_cout_median);

    main_layout->addLayout(layout_label);

    query = "select  * from stock;";
    requete.exec(query);
    int i=0;
    double utilise =0;
    double res  =0;
    while(requete.next())
    {
        widget_old_prix.push_back(requete.value(2).toDouble());
        widget_old_restant.push_back(requete.value(3).toInt());


        QHBoxLayout *layoutCustom = new QHBoxLayout();
        name = "layoutCustom_"+QString::number(i);
        layoutCustom->setObjectName(name);

        int ident = requete.value(0).toInt();
        id.push_back(ident);

        QLabel *label_intitule = new QLabel;
        name = "label_intitule_"+QString::number(i);;
        label_intitule->setObjectName(name);
        label_intitule->setText( requete.value(1).toString());

        QDoubleSpinBox *spin_box_double = new QDoubleSpinBox();
        name = "prix_spin_box_"+QString::number(i);;
        spin_box_double->setObjectName(name);
        spin_box_double->setValue( requete.value(2).toDouble());
        spinbox_prix.push_back(spin_box_double);

        QSpinBox *spin_box_int = new QSpinBox();
        name = "nb_restant_spin_box_"+QString::number(i);
        spin_box_int->setObjectName(name);
        spin_box_int->setValue( requete.value(3).toInt());
        spin_box_nb.push_back(spin_box_int);

        QPushButton *button = new QPushButton();
        name = "button_down_"+QString::number(i);
        button->setObjectName(name);
       // button->setText("-1");
        button->setIcon(QIcon("image/del.png"));


        QLabel *label_nbut = new QLabel() ;
        name = "label_porduit_utilise_"+QString::number(i);;
        label_nbut->setObjectName(name);
        int nb_produit_utilise = requete.value(4).toInt();
        label_nbut->setText(QString::number(nb_produit_utilise));
        spin_box_utilise.push_back(label_nbut);

        QLabel *label_nb_cliente = new QLabel() ;
        name = "label_nombre_utilise_"+QString::number(i);;
        label_nb_cliente->setObjectName(name);
        query2 =  "select count(*) from concerne "
                  "where concerne.idsoins in "
                  "(select id_soins from soin_concerne_stock, soins "
                  "where soin_concerne_stock.id_produit = "+QString::number(ident)+" and soin_concerne_stock.id_soins = soins.idsoins group by soins.intitule);";

        if(!requete2.exec(query2))
                QMessageBox::critical(this,"ERREUR","ERREUR requete compte nb soin client effectué");
        else
        {
           requete2.next();
           utilise = requete2.value(0).toInt();
           label_nb_cliente->setText(QString::number(utilise));
           nb_prod_utilise.push_back(utilise);
        }


        QLabel *label_moyen = new QLabel() ;
        name = "label_revenue_moyen"+QString::number(i);;
        label_moyen->setObjectName(name);
        if(utilise<=0)
            label_moyen->setText("0");
        else
        {
            if(nb_produit_utilise<=0)
                nb_produit_utilise = 1;

            res = utilise / nb_produit_utilise; //cliente / pot
            label_moyen->setText(QString::number(res));
        }
        moyenne_label.push_back(label_moyen);


        QLabel *label_median = new QLabel() ;
        name = "label_revenu_median_"+QString::number(i);;
        label_median->setObjectName(name);
        if(utilise<=0)
            label_median->setText("0");
        else
        {
            if(res<=0)
                res = 1;

            label_median->setText(QString::number(widget_old_prix[widget_old_prix.size()-1]/res));
            prix_par_client.push_back(label_median);
        }



        label_intitule->setFixedSize(QSize(100,20));
        spin_box_double->setFixedSize(QSize(50,20));
        spin_box_int->setFixedSize(QSize(50,20));
        button->setFixedSize(QSize(50,20));
        label_nbut->setFixedSize(QSize(50,20));
        label_nb_cliente->setFixedSize(QSize(50,20));
        label_moyen->setFixedSize(QSize(50,20));
        label_median->setFixedSize(QSize(50,20));

        //label_intitule->setAlignment(Qt::AlignLeft);
        spin_box_double->setAlignment(Qt::AlignRight);
        spin_box_int->setAlignment(Qt::AlignRight);
        label_nbut->setAlignment(Qt::AlignRight);
        label_nb_cliente->setAlignment(Qt::AlignRight);
        label_moyen->setAlignment(Qt::AlignRight);
        label_median->setAlignment(Qt::AlignRight);


        layoutCustom->addWidget(label_intitule);
        layoutCustom->addWidget(spin_box_double);
        layoutCustom->addWidget(spin_box_int);
        layoutCustom->addWidget(button);
        layoutCustom->addWidget(label_nbut);
        layoutCustom->addWidget(label_nb_cliente);
        layoutCustom->addWidget(label_moyen);
        layoutCustom->addWidget(label_median);


        main_layout->addLayout(layoutCustom);


        i++;

        QObject::connect(button,SIGNAL(clicked()),this,SLOT(on_click_button()));
    }

    frame->setLayout(main_layout);
    ui->produit->setWidget(frame);
}




void UIStock::initialisation_List_Soins()
{
    QString    query;
    QSqlQuery  requete;

    query = "SELECT idsoins,intitule FROM soins;";
    if(requete.exec(query))
    {
        while(requete.next())
        {
            QString value =requete.value(0).toString()+":"+requete.value(1).toString();
            QListWidgetItem* item = new QListWidgetItem(value, ui->soins_list);
            item->setFlags(item->flags() | Qt::ItemIsUserCheckable);
            item->setCheckState(Qt::Unchecked);
        }
    }
    else
    {
        QMessageBox::warning(this,"ERREUR","Erreur dialogue base de donnée: init_soins stock.ui");
    }
}

void UIStock::on_add_produit_clicked()
{
    QString    query;
    QSqlQuery  requete;



    if(ui->intitule_lineedit->text().isEmpty())
    {
        QMessageBox::warning(this,"ERREUR","Erreur l'intitulé est vide");
        return;
    }


    query = "insert into stock (intitule,prix,nb_restant)value ('"+ui->intitule_lineedit->text()+"',"+QString::number(ui->prix_dspinbox->value())+","+QString::number(ui->nb_acheter_spinbox->value())+");";
    if(requete.exec(query))
    {

        query = "select max(id) from stock";
        requete.exec(query);
        requete.next();
        int id = requete.value(0).toInt();

        for( int i=0; i< ui->soins_list->count();i++)
        {
            if(ui->soins_list->item(i)->checkState() == Qt::Checked)
            {
               QStringList split = ui->soins_list->item(i)->text().split(":");
               query = "INSERT INTO soin_concerne_stock (id_produit,id_soins) VALUES ("+QString::number(id)+","+split.at(0)+");";
                if(!requete.exec(query))
                    QMessageBox::warning(this,"ERREUR","Erreur ajout produit requete 2"); 
            }
        }

        QMessageBox::information(this,"Reussi","Ajout effectué avec succès");
        init_Ajout();
    }
    else
    {
        QMessageBox::warning(this,"ERREUR","Erreur ajout produit requete 1");
    }
}

void UIStock::on_return_button_clicked()
{
    this->close();
}

void UIStock::on_reinit_button_clicked()
{
    init_Ajout();
}

void UIStock::on_accepter_button_clicked()
{
    int ret =QMessageBox::question(this,"Enregistrer?","Voulez vous enregistrer les changements?",QMessageBox::Yes | QMessageBox::No | QMessageBox::Close);
    if(ret == QMessageBox::Yes)
    {
        QString    query;
        QSqlQuery  requete;
        QObjectList lst = main_layout->children();
        int size = lst.size();
        for(int i=0;i<size-1;i++) //on enleve 1 car c'est le layout de base qui est compté en trop
        {
            QSpinBox * spb_nb =spin_box_nb[i];
            QDoubleSpinBox *spdb_prx = spinbox_prix[i];

            if(widget_old_prix[i]!=spdb_prx->value() || widget_old_restant[i] != spb_nb->value())
            {
                QStringList str = spb_nb->objectName().split("_");
                int num_row = str[4].toInt();
                QString strg = spin_box_utilise[num_row]->text();
                int value = strg.toInt();

                query = "UPDATE stock SET prix="+QString::number(spdb_prx->value())+", nb_restant="+QString::number(spb_nb->value())+
                        ", nb_utilise="+QString::number(value)+" WHERE id="+QString::number(id[i])+";";
                if(!requete.exec(query))
                {
                    QMessageBox::warning(this,"ERREUR","Erreur enregistrement ");
                    return;
                }
            }
        }

         QMessageBox::information(this,"Réussi","Sauvegarde réussi");
    }
    else if(ret == QMessageBox::Close)
    {
        on_return_button_clicked();
    }
}




void UIStock::on_reinit_all_button_clicked()
{
   qDeleteAll(ui->produit->findChildren<QLabel *>());
   qDeleteAll(ui->produit->findChildren<QSpinBox *>());
   qDeleteAll(ui->produit->findChildren<QDoubleSpinBox *>());
   qDeleteAll(ui->produit->findChildren<QPushButton *>());

   spin_box_nb.clear();
   spinbox_prix.clear();
   moyenne_label.clear();
   spin_box_utilise.clear();
   widget_old_prix.clear();
   widget_old_restant.clear();
   nb_prod_utilise.clear();
   id.clear();
   prix_par_client.clear();

   initialisation_List_Produit();
   on_reinit_button_clicked();
}
