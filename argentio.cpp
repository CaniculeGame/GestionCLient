#include "argentio.h"
#include "ui_argentio.h"

#include <QString>
#include <QtSql>
#include <QSqlQuery>
#include <QMessageBox>
#include <QStringList>

#include <iostream>
using namespace std;


ArgentIO::ArgentIO(QWidget *parent) : QMainWindow(parent), ui(new Ui::ArgentIO)
{
    ui->setupUi(this);
    model_entree = NULL;
    model_sortie = NULL;

    maj_Tableaux();

    //qcombox init
    QStringList list;
    QStringList list2;
    list << "Année" <<"Mois"<<"Semaine";
    list2 << "Autre" << "Produits";
    ui->combobox_repetition->addItems(list);
    ui->cbx_type_ajout->addItems(list2);
    ui->dateedit_repetition->setDate(QDate::currentDate());
    ui->spinbox_reptition->setMinimum(1);

    ui->grid_entree->verticalHeader()->hide();
    ui->grid_sortie->verticalHeader()->hide();

    ui->button_ajout->setIcon(QIcon("image/add.png"));
    ui->button_retour->setIcon(QIcon("image/Cancel.png"));

}


ArgentIO::~ArgentIO()
{
    delete ui;
}


void ArgentIO::maj_Tableaux()
{
    QString query = "SELECT Intitule, Prix, TypeRepetition, Repetition, DateDebut FROM entrees_sorties WHERE Prix > 0;";
    QString query2 = "SELECT Intitule, Prix, TypeRepetition, Repetition, DateDebut FROM entrees_sorties WHERE Prix <= 0;";
    QSqlQuery  requete;
    double temp;

    if(requete.exec(query))
    {
        model_entree = new QStandardItemModel(requete.numRowsAffected(),requete.record().count(),this);
        model_entree->setHeaderData(0, Qt::Horizontal, tr("Intitulé"));
        model_entree->setHeaderData(1, Qt::Horizontal, tr("Prix"));
        model_entree->setHeaderData(2, Qt::Horizontal, tr("Type"));
        model_entree->setHeaderData(3, Qt::Horizontal, tr("Répétitions"));
        model_entree->setHeaderData(4, Qt::Horizontal, tr("Date Début"));

        int y=0;
        while(requete.next())
        {
            temp = requete.value(2).toDouble();
            for(int x=0; x < requete.record().count(); x++)
            {
                if(model_entree!=NULL)
                {
                    if(x == 2)
                    {
                        switch(requete.value(x).toInt())
                        {
                            case 0:
                                 model_entree->setItem(y,x, new QStandardItem("Année"));
                            break;
                           /* case 1:
                                 model_entree->setItem(y,x, new QStandardItem("Semestre"));
                            break;
                            case 2:
                                 model_entree->setItem(y,x, new QStandardItem("Trimestre"));
                            break;*/
                            case 1:
                                 model_entree->setItem(y,x, new QStandardItem("Mois"));
                            break;
                            case 2:
                                 model_entree->setItem(y,x, new QStandardItem("Semaine"));
                            break;
                           /* case 5:
                                 model_entree->setItem(y,x, new QStandardItem("Jour"));
                            break;*/
                        }
                    }
                    else if (x == 4)
                    {
                        QString d = requete.value(x).toString();
                        QDate date = QDate::fromString(d, "yyyy-MM-dd");
                        model_entree->setItem(y,x, new QStandardItem(date.toString("dd-MM-yyyy")));
                    }
                    else
                        model_entree->setItem(y,x, new QStandardItem(requete.value(x).toString()));
                }
            }
            y++;
        }
    }

    if(requete.exec(query2))
    {
        model_sortie = new QStandardItemModel(requete.numRowsAffected(),requete.record().count(),this);
        model_sortie->setHeaderData(0, Qt::Horizontal, tr("Intitulé"));
        model_sortie->setHeaderData(1, Qt::Horizontal, tr("Prix"));
        model_sortie->setHeaderData(2, Qt::Horizontal, tr("Type"));
        model_sortie->setHeaderData(3, Qt::Horizontal, tr("Répétitions"));
        model_sortie->setHeaderData(4, Qt::Horizontal, tr("Date Début"));
        int y=0;
        while(requete.next())
        {
            for(int x=0; x < requete.record().count(); x++)
            {
                if(model_sortie!=NULL)
                {
                    if(x == 2)
                    {
                        switch(requete.value(x).toInt())
                        {
                            case 0:
                                 model_sortie->setItem(y,x, new QStandardItem("Année"));
                            break;
                           /* case 1:
                                 model_sortie->setItem(y,x, new QStandardItem("Semestre"));
                            break;
                            case 2:
                                 model_sortie->setItem(y,x, new QStandardItem("Trimestre"));
                            break;*/
                            case 1:
                                 model_sortie->setItem(y,x, new QStandardItem("Mois"));
                            break;
                            case 2:
                                 model_sortie->setItem(y,x, new QStandardItem("Semaine"));
                            break;
                          /*  case 5:
                                 model_sortie->setItem(y,x, new QStandardItem("Jour"));
                            break;*/
                        }
                    }
                    else
                         model_sortie->setItem(y,x, new QStandardItem(requete.value(x).toString()));
                }
            }
            y++;
        }
    }

    ui->grid_entree->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->grid_entree->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->grid_entree->setAlternatingRowColors(true);
    ui->grid_entree->horizontalHeader()->setStretchLastSection(true);

    ui->grid_sortie->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->grid_sortie->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->grid_sortie->setAlternatingRowColors(true);
    ui->grid_sortie->horizontalHeader()->setStretchLastSection(true);


    ui->grid_entree->setModel(model_entree);
    ui->grid_sortie->setModel(model_sortie);
}

void ArgentIO::on_button_ajout_clicked()
{
    if(ui->lineedit_intitule->text()== "")
    {
        QMessageBox::warning(this,"Erreur","Intitulé non renseigné");
        return ;
    }

    QSqlQuery  requete;
    QDate date = ui->dateedit_repetition->date();
    QString query = "INSERT INTO entrees_sorties (Intitule, Prix, TypeRepetition, Repetition, DateDebut, type)"
                    "VALUES ('"+ui->lineedit_intitule->text()+"','"+QString::number(ui->spinbox_prix->value())+"','" +QString::number(ui->combobox_repetition->currentIndex())+"','"
            +ui->spinbox_reptition->text()+"','"+date.toString("yyyy-MM-dd")+"','"+ui->cbx_type_ajout->currentText()+"');";


    if(!requete.exec(query))
        QMessageBox::warning(this,"Erreur","Ajout impossible");
    else
    {
        maj_Tableaux();
    }

}

void ArgentIO::on_button_retour_clicked()
{
    this->close();
}

