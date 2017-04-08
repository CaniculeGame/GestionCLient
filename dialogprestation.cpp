#include "dialogprestation.h"
#include "ui_dialogprestation.h"

#include <QPrinter>
#include <QPainter>
#include <QPrintPreviewDialog>
#include <QTextEdit>
#include <QComboBox>
#include <QObjectList>
#include <QHBoxLayout>
#include <QList>


struct InfoSoins
{
  int id_soins; //identifiant du soins
  int categorie; //catégorie du soins
  double prix_soins; //prix du soin
  QString intitule_soins; //nom du soins
};

DialogPrestation::DialogPrestation(QWidget *parent) : QDialog(parent),ui(new Ui::DialogPrestation)
{
    ui->setupUi(this);
}

DialogPrestation::DialogPrestation(int index ,QWidget *parent) : QDialog(parent), ui(new Ui::DialogPrestation)
{
    id=index; //id client
    ui->setupUi(this);

    Qt::WindowFlags flags = windowFlags();
    Qt::WindowFlags helpFlag = Qt::WindowContextHelpButtonHint;
    Qt::WindowFlags maxFlag = Qt::Window;
    flags = flags & (maxFlag);
    setWindowFlags(flags);
    flags = flags & (~helpFlag);
    setWindowFlags(windowFlags());

    ui->pushButton_Annuler->setIcon(QIcon("image/Cancel.png"));
    ui->pushButton_init->setIcon(QIcon("image/Undo.png"));
    ui->pushButton_OK->setIcon(QIcon("image/save.png"));

    nb =0;
    prixT =0;
    prix_ss_reduc = 0;
    ui->listView_choix->setEditTriggers(QAbstractItemView::NoEditTriggers);

    QString query = "SELECT * FROM categoriesoins ORDER BY idCategorie;";
    QString query1 = "SELECT idsoins,intitule FROM soins WHERE soinsCategorie = ";
    QString req;
    QString sort = "ORDER BY idsoins;";

    modele = new QStandardItemModel();


    //recupere le nom + prenom
    req = "SELECT nom, prenom FROM clients WHERE idClient="+QString::number(id)+";";
    if(requete.exec(req))
    {
        requete.next();
        prenom_ = requete.value(1).toString();
        nom_    = requete.value(0).toString();
    }

    if(requete.exec(query))
    {
        while(requete.next()) //pour chaque catégorie faire:
        {
            QStandardItem *item = new QStandardItem(requete.value(1).toString());
            item->setEditable(false);
            modele->appendRow(item);
            req = query1;
            req+=QString::number(requete.value(0).toInt())+" and enCours = 1 "+sort;
           // cout<<req.toStdString()<<endl;
            if(requete2.exec(req))
            {
                while(requete2.next())//pour chaque soins de la catégorie faire:
                {
                    QStandardItem *item2 = new QStandardItem(requete2.value(1).toString());
                    item2->setEditable(false);
                    item->appendRow(item2);
                    id_soins.push_back(requete2.value(0).toInt());
                }
            }
        }

/*
        for(int i=0;i<id_soins.size();i++)
            cout<<id_soins[i]<<endl;*/

        ui->treeView_liste->setModel(modele);
        connect(ui->treeView_liste,SIGNAL(doubleClicked(QModelIndex)),this,SLOT(ajoutElement(QModelIndex)));
        connect(ui->listView_choix,SIGNAL(doubleClicked(QModelIndex)),this,SLOT(supprElement(QModelIndex)));
        connect(ui->pushButton_OK, SIGNAL(clicked()), this, SLOT(accept()));
        connect(ui->pushButton_Annuler, SIGNAL(clicked()), this, SLOT(reject()));

       ui->dateEdit->setDate( QDate::currentDate());
       ui->label_ss_reduc->setText("Prix Total sans reduction : "+QString::number(prix_ss_reduc)+"€");
       ui->label_prix->setText("Prix Total: "+QString::number(prix_ss_reduc)+"€");

    }
    else
        QMessageBox::warning(this,"Erreur","Erreur requete de donnée");



    //remplisage et initialisation du tableau des promotions
    comportement_intitule  << "Total" << "Min" << "Max" << "Soin";

    query = "SELECT * FROM promotions WHERE Activer > 0;";
    requete.exec(query);

    while(requete.next())
    {
        InfoPromotion info;
        info.id = requete.value(0).toInt(); //id unique dans la table sql
        info.intitule = requete.value(1).toString();
        info.valeur = requete.value(2).toDouble(); //pourcentage
        info.comportement_id  = requete.value(3).toInt();
        info.id_produit = requete.value(4).toInt(); //id du produit ciblé
        info.comportement_intitule = comportement_intitule[info.comportement_id];
        info_promo_existantes.push_back(info);
    }
}

DialogPrestation::~DialogPrestation()
{
    delete ui;
}


//montre dialog avec les prestation effectuées
void DialogPrestation::ajoutElement(QModelIndex index)
{
    //ids , idc
    if((index.parent().row()+1) !=0 )
    {
        index_=index;
        //maj prix
        QString query = "SELECT prix, idsoins FROM soins WHERE intitule = ";
        QString h=index.sibling(index.row(),0).data().toString();
        query += "'"+index.sibling(index.row(),0).data().toString()+"'"+";";
        requete.exec(query);
        requete.next();
        //cout<<query.toStdString()<<endl;
        prixT+=requete.value(0).toFloat();
        prix_ss_reduc+=requete.value(0).toFloat();
        ui->label_prix->setText("Prix total: "+QString::number(prix_ss_reduc)+" €");

        //maj liste
        listeS.push_back( ListeSoins(index.row()+1,index.parent().row()+1,requete.value(1).toInt(),requete.value(0).toFloat()));
        listeSoins.append(index.sibling(index.row(),0).data().toString());

        model = new QStringListModel(listeSoins);
        ui->listView_choix->setModel(model);

        //maj nb
        nb++;
        ui->label_nbChoix->setText("Nb choix: "+QString::number(nb)+" Soin(s)");
        ui->label_ss_reduc->setText("Prix Total sans reduction : "+QString::number(prix_ss_reduc)+"€");

    }
}

void DialogPrestation::supprElement(QModelIndex index)
{
    int ret=QMessageBox::information(this,"Supprimer?","Supprimer?",QMessageBox::Yes | QMessageBox::No);
    if(ret == QMessageBox::Yes )
    {
        //maj prix
        ListeSoins s =listeS[index.row()];
        prix_ss_reduc-=s.valeur();
        ui->label_ss_reduc->setText("Prix Total sans reduction : "+QString::number(prix_ss_reduc)+"€");

        //Maj prix avec reduction
        on_cbox_currentIndexChanged(0);


        ui->label_prix->setText("Prix total: "+QString::number(prixT)+" €");


        listeS.erase(listeS.begin()+index.row());
        listeSoins.erase(listeSoins.begin()+index.row());

        model = new QStringListModel(listeSoins);
        ui->listView_choix->setModel(model);

        nb--;
        ui->label_nbChoix->setText("Nb choix: "+QString::number(nb)+" Soin(s)");
    }

    //suppression de toute les promotions si liste remise à zero
    if(nb<=0)
    {
        ui->label_prix->setText("Prix total: 0 €");
        prixT = 0;
        prix_ss_reduc =0;
        promotions_tab.clear();

        QList<QComboBox *> obj_lst = this->findChildren<QComboBox *>();
        for(auto i:obj_lst)
        {
            delete i;
        }

        QList<QPushButton *> obj_lst1 = this->findChildren<QPushButton *>("btn");
        for(int i=0;i<obj_lst1.size();i++)
        {
             delete obj_lst1.at(i);
        }
    }
}

void DialogPrestation::on_pushButton_init_clicked()
{
    if(!listeS.empty())
    {
        if(!listeS.empty())
        {
            listeS.clear();
        }
    }

    if(!listeSoins.empty())
    {
        listeSoins.clear();
    }

    if(model!=NULL)
    {
        model = new QStringListModel();
        ui->listView_choix->setModel(model);
    }

    nb=0;
    prixT=0;
    prix_ss_reduc = 0;
    ui->label_nbChoix->setText("Nb choix: "+QString::number(nb)+" Soin(s)");
    ui->label_prix->setText("Prix total: "+QString::number(prix_ss_reduc)+" €");
    ui->label_ss_reduc->setText("Prix Total sans reduction : "+QString::number(prix_ss_reduc)+"€");

    QList<QComboBox *> obj_lst = this->findChildren<QComboBox *>();
    for(int i=0;i<obj_lst.size();i++)
    {
         //QComboBox lst =obj_lst.at(i);
         delete obj_lst.at(i);
    }

    QList<QPushButton *> obj_lst1 = this->findChildren<QPushButton *>("btn");
    for(int i=0;i<obj_lst1.size();i++)
    {
         //QComboBox lst =obj_lst.at(i);
         delete obj_lst1.at(i);
    }

}

void DialogPrestation::on_pushButton_Annuler_clicked()
{
    this->setResult(QDialog::Rejected);
    close();
}

void DialogPrestation::on_pushButton_OK_clicked()
{

    //mise à jour de la bdd -> ajout de la fiche de prestation
    if(!listeS.empty())
    {
        QString req = "INSERT INTO prestations (prixTotal,date,idClient) VALUE (";
        QString req1 = "SELECT idprestations FROM prestations ORDER BY idprestations DESC LIMIT 1;";
        QString req2 ="INSERT INTO concerne (idprest,idsoins) VALUE (";
        QString query;


        QDate date = ui->dateEdit->date();
        QString cDate= date.toString("yyyy-MM-dd");



        //ajout promotion et prestation
        QList<QComboBox *> obj_lst = this->findChildren<QComboBox *>();
        if(obj_lst.empty())
            query=req+QString::number(prix_ss_reduc)+",'"+cDate+"',"+QString::number(id)+");";
        else
            query=req+QString::number(prixT)+",'"+cDate+"',"+QString::number(id)+");";

        if(!requete.exec(query))
            QMessageBox::information(this,"ERROR","echec requete 1");

        requete.exec(req1);
        requete.next();
        prest_ = requete.value(0).toInt();

        //ListeSoins elem;
        for(int i=0; i<listeS.size(); i++)
        {
           ListeSoins  elem = listeS[i];

            //ajout soins
           // cout<<QString::number(prest).toStdString()<<","<<QString::number(elem.getIdX()).toStdString()<<endl;
            query=req2+QString::number(prest_)+","+QString::number(elem.getIdX())+");";
            if(!requete.exec(query))
              QMessageBox::information(this,"ERROR","echec requete 1");
        }


        //ajout des promotions
        ///recuperation des combos box et de tous les index

        if(!obj_lst.empty())
        for(int i=0;i<obj_lst.size();i++)
        {
           if(obj_lst[i] != NULL)
           {
              int index  = info_promo_existantes[obj_lst[i]->currentIndex()].id ;

              //verifier que promo est possible



              //mise à jour de la base de donnée
              query = "INSERT INTO concerne_promotions(id_promotion,id_prestation) VALUE ("+QString::number(index)+","+QString::number(prest_)+");";
              //cout<<query.toStdString()<<endl;
              if(!requete.exec(query))
                QMessageBox::information(this,"ERROR","echec requete dialogprestation on_push_OK 2");

              //cout<<query.toStdString()<<endl;
           }
        }



        //impression de la facture
        int ret =QMessageBox::information(this,"Impression?","Voulez vous imprimez une facture?",QMessageBox::Yes | QMessageBox::No );
        if(ret == QMessageBox::Yes)
        {
            //imprimer facture
            QPrinter printer;
            printer.setPaperSize(QPrinter::A5);
            printer.setOrientation(QPrinter::Portrait);

            QPrintPreviewDialog *pd = new QPrintPreviewDialog(&printer);
            connect(pd,SIGNAL(paintRequested(QPrinter*)),this,SLOT(print(QPrinter*)));
            pd->exec();
        }
    }

    setResult(QDialog::Accepted);
    close();
}


//Ecrire facture -> mettre dans options les donnée en dur
void DialogPrestation::print(QPrinter *p)
{
    // A déplacer
    QString address = " Raffalli FLorian \n 494 avenue des mésanges \n 83160 la valette du var";
    QString identite = prenom_+" "+nom_;


    //recupère toutes les prestation liées à la prestation ajouté
    QSqlQuery  requete;
    int a = index_.row();
    int b = prest_;
    QString id_prest2 = QString::number(b);//idprest
    QString id_prest = QString::number(b); //idprestations
    QString query = "SELECT s.intitule, s.prix, s.idsoins, s.soinsCategorie FROM prestations p NATURAL JOIN  concerne c NATURAL JOIN   soins s WHERE idClient = "
            +QString::number(id)+" and idprest = "+id_prest+" and idprestations = "+id_prest2+";";




    vector<InfoSoins> soins_vector;
    requete.exec(query);
    QString mess = "Soins:\n";
    double total =0;
    while(requete.next())
    {
        mess+=requete.value(0).toString()+" = "+requete.value(1).toString()+"€";
        mess+="\n";
        total+=requete.value(1).toDouble();

        InfoSoins s;
        s.categorie = requete.value(3).toInt();
        s.id_soins = requete.value(2).toInt();
        s.intitule_soins = requete.value(0).toString();
        s.prix_soins = requete.value(1).toDouble();

        soins_vector.push_back(s);

    }

    //Application des réductions

    vector<InfoPromotion> promo_vector;
    mess+="\n\nPromotions:\n";
    query = "SELECT * FROM concerne_promotions c, promotions p WHERE c.id_promotion=p.id and c.id_prestation ="+id_prest2+";";
    requete.exec(query);
    while(requete.next()) //pour chaque promotion faire:
    {
       InfoPromotion p;
       p.id = requete.value(1).toInt();
       p.comportement_id = requete.value(6).toInt();
       p.id_produit = requete.value(7).toInt();
       p.valeur = requete.value(5).toDouble();
       p.intitule = requete.value(4).toString();
       promo_vector.push_back(p);
    }

    //application des promotions

    double min=0;
    double max=0;
    double reduc=0;
    bool present = false;
    for(int i=0; i<promo_vector.size();i++)
    {
        int cpt = promo_vector[i].comportement_id;
        switch(cpt)
        {
             case 0: //total
                 total-= (total * promo_vector[i].valeur )/ 100;
                 mess+=promo_vector[i].intitule+" : -"+QString::number(promo_vector[i].valeur)+"%\n";
             break;

             case 1: //min

                min =soins_vector[0].prix_soins;
                for(int j =0 ; j <soins_vector.size();j++ )
                {
                    if(min >= soins_vector[j].prix_soins )
                        min = soins_vector[j].prix_soins;
                }

                 total-= (min * promo_vector[i].valeur )/ 100;
                 mess+=promo_vector[i].intitule+" : -"+QString::number(promo_vector[i].valeur)+"%\n";

             break;

             case 2: //max

                max =soins_vector[0].prix_soins;
                for(int j =0 ; j <soins_vector.size();j++ )
                {
                    if(max <= soins_vector[j].prix_soins )
                        max = soins_vector[j].prix_soins;
                }

                 total-= (max * promo_vector[i].valeur )/ 100;
                 mess+=promo_vector[i].intitule+" : -"+QString::number(promo_vector[i].valeur)+"%\n";

             break;

             case 3: //soins

                reduc=0;
                present = false;
                for(int j =0 ; j <soins_vector.size();j++ ) //recherche du porduit correspondant
                {
                    if(promo_vector[i].id_produit == soins_vector[j].id_soins )
                    {
                        reduc = soins_vector[j].prix_soins;
                        present = true;
                        break;
                    }
                }
                if(present)
                {
                    total-= (reduc * promo_vector[i].valeur )/ 100;
                    mess+=promo_vector[i].intitule+" : -"+QString::number(promo_vector[i].valeur)+"%\n";
                }
             break;
        }
    }


    QPainter painter;
    painter.begin(p);

        //mettre des valeurs absolues
        painter.drawText(QRectF(10,10,200,200),address);
        painter.drawText(QRectF(300,50,200,200),identite);
        painter.drawText(QRectF(300,100,200,200),QDate::currentDate().toString("ddd dd MMMM yyyy"));

        painter.drawText(QRectF(50,400,1000,1000),mess);
        painter.drawText(QRectF(450,700,75,75),"Total TTC: \n\t"+QString::number(total)+"€");

    painter.end();


}


//ajouter les promos à faire au client
void DialogPrestation::add_Combo_Box()
{
    if(info_promo_existantes.size()== 0)
        return;

    QComboBox *new_cb= new QComboBox;
    QHBoxLayout *new_layout = new QHBoxLayout();
    QPushButton *new_button = new QPushButton();

    new_button->setText("Supprimer");
    new_button->setObjectName("btn");
    new_cb->setObjectName("cbox");
    QObject::connect(new_button, SIGNAL(clicked()), this, SLOT(on_clicked_del()));
    QObject::connect(new_button, SIGNAL(clicked()), new_cb, SLOT(deleteLater()));
    QObject::connect(new_button, SIGNAL(clicked()), new_button, SLOT(deleteLater()));
    QObject::connect(new_button, SIGNAL(clicked()), new_layout, SLOT(deleteLater()));
    QObject::connect(new_cb, SIGNAL(currentIndexChanged(int)), this, SLOT(on_cbox_currentIndexChanged(int)));


    //ui->label_prix->setText("Prix Total : "+QString::number(prixT)+"€");

    for(int i=0; i<info_promo_existantes.size();i++)
           new_cb->insertItem(i,info_promo_existantes[i].intitule);

    new_layout->addWidget(new_cb);
    new_layout->addWidget(new_button);
    ui->layout_add_promo->addLayout(new_layout);

    on_cbox_currentIndexChanged(0);
}


void DialogPrestation::on_clicked_del()
{
    on_cbox_currentIndexChanged(0);
}

void DialogPrestation::on_button_ajout_promo_clicked()
{
    add_Combo_Box();
}

//on doit recalculer le prix total. en boucle
void DialogPrestation::on_cbox_currentIndexChanged(int index)
{

    prixT = prix_ss_reduc;
    QList<QComboBox *> obj_lst = this->findChildren<QComboBox *>();

    if(obj_lst.empty())
    {
        ui->label_prix->setText("Prix Total : "+QString::number(prixT)+"€");
        return;
    }

    //pour chaque combobox faire
    for(int i=0;i<obj_lst.size();i++) //pour toute les promos appliquées faire :
    {
       if(obj_lst[i] != NULL)
       {
          //id = identifiant de la promotion dans la combo box
          int id  = info_promo_existantes[obj_lst[i]->currentIndex()].id ;
          id = obj_lst[i]->currentIndex();

          //recupère pourcentage
          double valeur_pourcentage = info_promo_existantes[id].valeur;
          //recupere id comportement
          int id_comportement = info_promo_existantes[id].comportement_id;
          //recupere l'id produit pour vérification
          int id_produit = info_promo_existantes[id].id_produit;


          //cout<<id_produit<<endl;

          double tps = 0;
          double min=0;
          double max=0;
          bool present = false;

          //list<ListeSoins>::iterator i;
          //ListeSoins element;

          switch(id_comportement)
          {
              case 0: //total
                  prixT -= (prixT * valeur_pourcentage )/ 100;
                  ui->label_prix->setText("Total : "+QString::number(prixT)+"€");
              break;

              case 1: //min

                  min = 999999999;
                  for(int i=0; i<listeS.size(); i++)
                  {
                      ListeSoins lst =listeS[i];
                      if(lst.valeur()<min)
                          min = lst.valeur();
                  }

                  tps = (min * valeur_pourcentage) / 100;
                  prixT -= tps;
                  ui->label_prix->setText("Prix Total : "+QString::number(prixT)+"€");

              break;

              case 2: //max

                  max = 0;
                  for(int i=0; i<listeS.size(); i++)
                  {
                      ListeSoins lst =listeS[i];
                      if(lst.valeur()>=max)
                          max = lst.valeur();
                  }

                  tps = (max * valeur_pourcentage) / 100;
                  prixT -= tps;
                  ui->label_prix->setText("Prix Total : "+QString::number(prixT)+"€");

              break;

              case 3: //soin

                  //vérification si soin est présent ou pas
                  tps=0;
                  present = false;
                  for(int j =0 ; j <listeS.size();j++ ) //recherche du produit correspondant (pour chaque soin choisis faire:)
                  {

                      // /!\ ici idx c'est l'id du soin et non de la catégorie /!\
                     // cout<<id_produit<<"  id_prod choisi="<<listeS[j].getIdX()<<"   valeur ="<<listeS[j].valeur()<<endl;
                      if(listeS[j].getIdX() == id_produit )
                      {

                          tps = listeS[j].valeur(); //valeur en € du produit
                          present = true;
                          break;
                      }
                  }

                  if(present)
                  {
                      prixT-= (tps * valeur_pourcentage )/ 100;
                      ui->label_prix->setText("Prix Total : "+QString::number(prixT)+"€");
                  }

              break;
          }
       }
    }

    ui->label_ss_reduc->setText("Prix Total sans reduction : "+QString::number(prix_ss_reduc)+"€");
}














