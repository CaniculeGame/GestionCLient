#include "prixconfigw.h"
#include "ui_prixconfigw.h"

#include <iostream>
using namespace std;

PrixConfigW::PrixConfigW(QWidget *parent) : QMainWindow(parent),ui(new Ui::PrixConfigW)
{
    ui->setupUi(this);

    main_layout = new QVBoxLayout();
    frame= new QFrame();

    ui->button_reset->setIcon(QIcon("image/Undo.png"));
    ui->button_ok->setIcon(QIcon("image/save.png"));
    ui->button_cancel->setIcon(QIcon("image/Cancel.png"));

    taille = ui->scrollAreaWidgetContents->width();

    initialisation(false);
    init_add();

    show();
}

PrixConfigW::~PrixConfigW()
{
    delete ui;
}


void PrixConfigW::initialisation(bool afficher)
{


    widget_label_soin.clear();
    widget_prix.clear();
    widget_old_prix.clear();
    widget_intitule.clear();
    widget_h_box.clear();


    query = "SELECT * FROM categoriesoins ORDER BY idCategorie;";
    if(!requete.exec(query))
    {
        QMessageBox::warning(this,"Erreur","Erreur requete catégorie");
        return;
    }

    while(requete.next())
        string_categorie.push_back(requete.value(1).toString());


    query = "SELECT idsoins FROM soins ORDER BY idsoins;";
    if(!requete.exec(query))
    {
        QMessageBox::warning(this,"Erreur","Erreur requete soins");
        return;
    }
    while(requete.next())
        id_soins.push_back(requete.value(0).toInt());


    if(afficher == false)
        query = "SELECT intitule,prix,soinsCategorie FROM soins WHERE enCours = 1 ;";
    else
        query = "SELECT intitule,prix,soinsCategorie,enCours FROM soins;";

    if(!requete.exec(query))
    {
        QMessageBox::warning(this,"Erreur","Erreur requete soins");
        return;
    }


    QHBoxLayout *layout_label = new QHBoxLayout();
    QLabel *label_prix= new QLabel();
    label_prix->setText("Nouveau prix");
    QLabel *label_intitule= new QLabel();
    label_intitule->setText("Intitulé du soin");
    QLabel *label_categorie= new QLabel();
    label_categorie->setText("Catégorie du soin");
    QLabel *old_price= new QLabel();
    old_price->setText("Ancien prix");
    QLabel *activate_label= new QLabel();
    activate_label->setText("Activer/Désactiver");

    int  h = 50;
    int  w = (taille-50)/5;


    label_prix->setFixedSize(QSize(w,h));
    label_intitule->setFixedSize(QSize(w,h));
    label_categorie->setFixedSize(QSize(w,h));
    old_price->setFixedSize(QSize(w,h));
    activate_label->setFixedSize(QSize(w,h));

    layout_label->addWidget(label_intitule);
    layout_label->addWidget(label_categorie);
    layout_label->addWidget(old_price);
    layout_label->addWidget(label_prix);
    layout_label->addWidget(activate_label);



    main_layout->addLayout(layout_label);
    QString name;
    int i=0;
    while(requete.next())
    {
        QHBoxLayout *layoutCustom = new QHBoxLayout();
        name = "layoutCustom_"+QString::number(i);
        layoutCustom->setObjectName(name);

        QLabel *label_intitule = new QLabel;
        name = "label_intitule_"+QString::number(i);
        label_intitule->setObjectName(name);
        label_intitule->setText( requete.value(0).toString());


        QLabel *label_soinsCategorie = new QLabel() ;
        name = "label_soinsCategorie_"+QString::number(i);
        label_soinsCategorie->setObjectName(name);
        name = string_categorie[requete.value(2).toInt()-1];
        label_soinsCategorie->setText(name);


        QLabel *label_old_price = new QLabel();
        label_old_price->setText( requete.value(1).toString());
        label_old_price->setAlignment(Qt::AlignCenter);


        QDoubleSpinBox *spin_box = new QDoubleSpinBox();
        name = "prix_spin_box_"+QString::number(i);
        spin_box->setObjectName(name);
        spin_box->setValue( requete.value(1).toDouble());
        spin_box->setAlignment(Qt::AlignRight);

        QPushButton *activate_button = new QPushButton();


        if(afficher == false)
        {
            activate_button->setText("Desactiver");
            name = "prix_button_"+QString::number(id_soins[i])+"_1";
        }
        else
        {
            if(requete.value(3).toBool() == true)
            {
                activate_button->setText("Desactiver");
                name = "prix_button_"+QString::number(id_soins[i])+"_1";
            }
            else
            {
                activate_button->setText("Activer");
                name = "prix_button_"+QString::number(id_soins[i])+"_0";
            }
        }

        activate_button->setObjectName(name);
        connect(activate_button,SIGNAL(clicked()),this,SLOT(activate_Desactivate_Button()));



        layoutCustom->addWidget(label_intitule);
        layoutCustom->addWidget(label_soinsCategorie);
        layoutCustom->addWidget(label_old_price);
        layoutCustom->addWidget(spin_box);
        layoutCustom->addWidget(activate_button);

        main_layout->addLayout(layoutCustom);
        i++;

        widget_label_soin.push_back(label_soinsCategorie);
        widget_prix.push_back(spin_box);
        widget_old_prix.push_back(label_old_price);
        widget_intitule.push_back(label_intitule);
        widget_h_box.push_back(layoutCustom);
    }

    frame->setLayout(main_layout);
    ui->scrollArea->setWidget(frame);

}

void PrixConfigW::activate_Desactivate_Button()
{
    QObject *senderObj = sender();
    QString senderObjName = senderObj->objectName();
    QStringList strlist = senderObjName.split("_");

    QString str_req;
    QSqlQuery req;

    if(strlist[3].toInt()==1)
    {
       str_req = "UPDATE soins SET enCours = 0 WHERE idsoins ="+QString::number(strlist[2].toInt())+";";
    }
    else
    {
       str_req = "UPDATE soins SET enCours = 1 WHERE idsoins ="+QString::number(strlist[2].toInt())+";";
    }

    req.exec(str_req);


    //mise à jour du boutton
    QPushButton *button =this->findChild<QPushButton*>(senderObjName);
    if(strlist[3].toInt()==1)
    {
        button->setText("Activer");
        button->setObjectName(strlist[0]+"_"+strlist[1]+"_"+strlist[2]+"_0");
    }
    else
    {
        button->setText("Désactiver");
        button->setObjectName(strlist[0]+"_"+strlist[1]+"_"+strlist[2]+"_1");
    }


}

void PrixConfigW::update()
{

}


void PrixConfigW::init_add()
{
    QStringList lst;

    query = "SELECT * FROM categoriesoins  ORDER BY  idCategorie ASC";
    requete.exec(query);

    while(requete.next())
        lst << requete.value(1).toString();

    ui->comboBox_categorie->addItems(lst);
}




//mise à jour des prix
void PrixConfigW::on_button_ok_clicked()
{

    int ret =QMessageBox::warning(this,"Sauvegarder?","Voulez vous sauvegarder les modifications?",QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
    if(ret == QMessageBox::No)
    {
        this->close();

    }
    else if(ret == QMessageBox::Cancel)
        return;
    else
    {
        int size = widget_prix.size();
        for(int i=0;i<size;i++)
        {
            QString prix = widget_old_prix[i]->text();
            double n_prix = widget_prix[i]->value();

            if(n_prix != prix.toDouble())
            {
                query = QString("UPDATE soins SET prix=%1 WHERE idsoins=%2;").arg(n_prix).arg(id_soins[i]);
                if(!requete.exec(query))
                {
                    QMessageBox::warning(this,"Erreur","Erreur mise à jour des données");
                    return;
                }
             }
         }

        this->close();
    }
}

void PrixConfigW::on_button_reset_clicked()
{
    int size = widget_old_prix.size();
    for(int i =0; i< size;i++)
    {
        QString str = widget_old_prix[i]->text();
        widget_prix[i]->setValue(str.toDouble());
    }
}

void PrixConfigW::on_button_cancel_clicked()
{
    int ret=QMessageBox::information(this,"Quitter?","Voulez vous fermer la fenetre et annuler les modifications?",QMessageBox::Yes | QMessageBox::No);
    switch (ret)
    {
        case QMessageBox::Yes:
                close();
        break;
    }
}

void PrixConfigW::on_checkBox_show_en_cours_stateChanged(int arg1)
{
    //effacement de la liste
    main_layout->deleteLater();
    main_layout = new QVBoxLayout();

    frame->deleteLater();
    frame= new QFrame();


    if(ui->checkBox_show_en_cours->isChecked())
    {
        initialisation(true);
    }
    else
    {
        initialisation(false);
    }
}





void PrixConfigW::on_Add_categorie_clicked()
{
    if(ui->lineEdit_categorie->text().isEmpty())
    {
        QMessageBox::warning(this,"Erreur","Champ intitulé de la nouvelle catégorie est vide",QMessageBox::Ok);
        return;
    }
    else
    {
        int ret =QMessageBox::warning(this,"Attention","Voulez vous ajouter une nouvelle catégorie?",QMessageBox::Yes | QMessageBox::No);
        if(ret == QMessageBox::No)
            return;

        query = "INSERT INTO categoriesoins(intitule) VALUES ('"+ui->lineEdit_categorie->text()+"');";
        if(requete.exec(query))
        {
            //maj de la comboBox
            ui->comboBox_categorie->addItem(ui->lineEdit_categorie->text());
            QMessageBox::information(this,"Réussi","Ajout terminer avec succès",QMessageBox::Ok);
        }
        else
            QMessageBox::information(this,"Echec","Echec de l'ajout",QMessageBox::Ok);


    }

}



void PrixConfigW::add_List_Soins(QString intitule,QString categorie, double old_price, double price)
{
    int i = id_soins.size();
    QString name;

    QHBoxLayout *layoutCustom = new QHBoxLayout();
    name = "layoutCustom_"+QString::number(i);
    layoutCustom->setObjectName(name);

    QLabel *label_intitule = new QLabel;
    name = "label_intitule_"+QString::number(i);
    label_intitule->setObjectName(name);
    label_intitule->setText( intitule);


    QLabel *label_soinsCategorie = new QLabel() ;
    name = "label_soinsCategorie_"+QString::number(i);
    label_soinsCategorie->setObjectName(name);
    name = categorie;
    label_soinsCategorie->setText(name);


    QLabel *label_old_price = new QLabel();
    label_old_price->setText( QString::number(old_price));
    label_old_price->setAlignment(Qt::AlignCenter);


    QDoubleSpinBox *spin_box = new QDoubleSpinBox();
    name = "prix_spin_box_"+QString::number(i);
    spin_box->setObjectName(name);
    spin_box->setValue( price);
    spin_box->setAlignment(Qt::AlignRight);

    QPushButton *activate_button = new QPushButton();
    activate_button->setText("Desactiver");
    name = "prix_button_"+QString::number(id_soins[i])+"_1";
    activate_button->setObjectName(name);
    connect(activate_button,SIGNAL(clicked()),this,SLOT(activate_Desactivate_Button()));


    layoutCustom->addWidget(label_intitule);
    layoutCustom->addWidget(label_soinsCategorie);
    layoutCustom->addWidget(label_old_price);
    layoutCustom->addWidget(spin_box);
    layoutCustom->addWidget(activate_button);

    main_layout->addLayout(layoutCustom);

    widget_label_soin.push_back(label_soinsCategorie);
    widget_prix.push_back(spin_box);
    widget_old_prix.push_back(label_old_price);
    widget_intitule.push_back(label_intitule);
    widget_h_box.push_back(layoutCustom);
}

void PrixConfigW::on_Add_soins_clicked()
{
    if(ui->lineEdit_add_soin->text().isEmpty())
    {
        QMessageBox::warning(this,"Erreur","Champ intitulé de la nouvelle catégorie est vide",QMessageBox::Ok);
        return;
    }
    else
    {
        int ret =QMessageBox::warning(this,"Attention","Voulez vous ajouter un nouveau soin?",QMessageBox::Yes | QMessageBox::No);
        if(ret == QMessageBox::No)
            return;

        query = "SELECT idCategorie FROM categoriesoins WHERE intitule='"+ui->comboBox_categorie->currentText()+"';";
        cout<<query.toStdString()<<endl;
        requete.exec(query);
        requete.next();

        QString tps = requete.value(0).toString();
        query = "INSERT INTO soins(intitule,prix,soinsCategorie) VALUES ('"+ui->lineEdit_add_soin->text()+"','"+QString::number(ui->spinBox_prix->value())+"','"+tps+"');";
        if(requete.exec(query))
        {
             //ajout de la nouvelle ligne
            add_List_Soins(ui->lineEdit_add_soin->text(),ui->comboBox_categorie->currentText(),ui->spinBox_prix->value(),ui->spinBox_prix->value());
            QMessageBox::information(this,"Réussi","Ajout terminer avec succès",QMessageBox::Ok);
        }
        else
            QMessageBox::information(this,"Echec","Echec de l'ajout",QMessageBox::Ok);
    }

}


