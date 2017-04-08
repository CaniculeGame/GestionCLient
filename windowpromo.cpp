#include "windowpromo.h"
#include "ui_windowpromo.h"

#include <QStringList>

WindowPromo::WindowPromo(QWidget *parent) :QMainWindow(parent),ui(new Ui::WindowPromo)
{
    ui->setupUi(this);

    model = NULL;

    list << "Total" << "Min" << "Max" << "Soin";

    QSqlQuery  requete;
    QString query;

    query = "Select intitule,idsoins from soins ;";
    if(requete.exec(query))
    {
        while(requete.next())
        {
            list_soins << requete.value(0).toString();
            id_soins.push_back(requete.value(1).toInt()); //id des soins ordre du plus petit au plus grand. Les chiffres ne se suivent aps forcement
        }
    }


    ui->cbbox_comportement->addItems(list);
    ui->produit_liste->addItems(list_soins);

    if(ui->cbbox_comportement->currentIndex() != 3)
        ui->produit_liste->setEnabled(false);

    maj_tab();

    connect(ui->tableView,SIGNAL(doubleClicked(QModelIndex)),this,SLOT(appelOption(QModelIndex)));

    ui->add_button->setIcon(QIcon("image/add.png"));
    ui->retour_buton->setIcon(QIcon("image/Cancel.png"));
}



WindowPromo::~WindowPromo()
{
    delete ui;
}



void WindowPromo::appelOption(QModelIndex index)
{

    QSqlQuery  requete;
    QString query;
    int ret =0;
    int row=index.row();
    int column = 5; //colone etat = colone numéro 6 -> 5  en partant de 0
    QVariant tps = model->item(row,column)->data(Qt::DisplayRole);


    cout<<row<<"  "<<column<<"  "<<tps.toString().toStdString()<<endl;
    if(tps.toString()=="En cours")//activer
    {
        ret = QMessageBox::information(this,"Desactiver?","Voulez vous desactiver la promotion?",QMessageBox::Yes | QMessageBox::No);
        if(ret == QMessageBox::Yes)
        {
            int id = model->item(index.row(),0)->data(Qt::DisplayRole).toInt();
            query = "UPDATE promotions SET Activer=0 WHERE id="+QString::number(id)+";";
            requete.exec(query);
        }
    }
    else
    {
        ret = QMessageBox::information(this,"Activer?","Voulez vous activer la promotion?",QMessageBox::Yes | QMessageBox::No);
        if(ret == QMessageBox::Yes)
        {
            int id = model->item(index.row(),0)->data(Qt::DisplayRole).toInt();
            query = "UPDATE promotions SET Activer=1 WHERE id="+QString::number(id)+";";
            requete.exec(query);
        }
    }

    maj_tab();

}

void WindowPromo::maj_tab()
{
    QString query;
    QSqlQuery  requete;
    double temp;

    if(!ui->checkbox_show_disable->isChecked())
        query = "SELECT * FROM promotions WHERE Activer > 0;";
    else
        query = "SELECT * FROM promotions;";

    if(requete.exec(query))
    {
        model = new QStandardItemModel(requete.numRowsAffected(),requete.record().count(),this);
        model->setHeaderData(0, Qt::Horizontal, tr("Id"));
        model->setHeaderData(1, Qt::Horizontal, tr("Intitulé"));
        model->setHeaderData(2, Qt::Horizontal, tr("Reduction (%)"));
        model->setHeaderData(3, Qt::Horizontal, tr("Comportement"));
        model->setHeaderData(4, Qt::Horizontal, tr("Soin lié"));
        model->setHeaderData(5, Qt::Horizontal, tr("Etat"));


        int y=0;
        while(requete.next())
        {
            model->setItem(y,0, new QStandardItem(requete.value(0).toString()));
            model->setItem(y,1, new QStandardItem(requete.value(1).toString()));
            model->setItem(y,2, new QStandardItem(requete.value(2).toString()));
            model->setItem(y,3, new QStandardItem(list.at(requete.value(3).toInt())));

            if(requete.value(4).toInt()<0)
                model->setItem(y,4, new QStandardItem("Aucun"));
            else
            {
                int id=-1;
                for(int i=0;i<list_soins.size();i++)
                {
                    if(id_soins[i]==requete.value(4).toInt())
                    {
                       id = i;
                       break;
                    }
                }

                if(id!=-1)
                    model->setItem(y,4, new QStandardItem(list_soins.at(id)));
            }

            if(requete.value(5).toInt()>0)
                model->setItem(y,5, new QStandardItem("En cours"));
            else
                model->setItem(y,5, new QStandardItem("Terminer"));

            y++;
        }

    }


    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableView->setAlternatingRowColors(true);
    ui->tableView->horizontalHeader()->setStretchLastSection(true);
    ui->tableView->verticalHeader()->hide();

    ui->tableView->setModel(model);

}



void WindowPromo::on_cbbox_comportement_currentIndexChanged(int index)
{
    if(ui->cbbox_comportement->currentIndex() != 3)
        ui->produit_liste->setEnabled(false);
    else
        ui->produit_liste->setEnabled(true);

}

void WindowPromo::on_add_button_clicked()
{
    QSqlQuery  requete;
    QString query;

    if(ui->cbbox_comportement->currentIndex() != 3)
    {
        query = "INSERT INTO promotions (Intitule, valeur, comportement,Activer) VALUES "
                "('"+ui->intitule->text()+"',"+QString::number(ui->valeur_reductuin_pourcent->value())+","+
                QString::number(ui->cbbox_comportement->currentIndex())+",1);";

        if(!requete.exec(query))
                QMessageBox::warning(this,"Erreur","Echec maj promotion");
    }
    else
    {
        query = "INSERT INTO promotions (Intitule, valeur, comportement,Activer,id_produit) VALUES "
                "('"+ui->intitule->text()+"',"+QString::number(ui->valeur_reductuin_pourcent->value())+","+
                QString::number(ui->cbbox_comportement->currentIndex())+",1,"+  QString::number(id_soins[ui->produit_liste->currentIndex()])+");";

        cout<<query.toStdString()<<endl;

        if(!requete.exec(query))
                QMessageBox::warning(this,"Erreur","Echec maj promotion");
    }

    maj_tab();
}

void WindowPromo::on_retour_buton_clicked()
{
    this->close();
}

void WindowPromo::on_checkbox_show_disable_stateChanged(int arg1)
{
    maj_tab();
}
