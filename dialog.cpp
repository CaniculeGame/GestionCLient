#include "dialog.h"
#include "ui_dialog.h"



Dialog::Dialog(QWidget *parent) : QDialog(parent), ui(new Ui::Dialog)
{
    ui->setupUi(this);
}

Dialog::Dialog(int id, QWidget *parent) : QDialog(parent), ui(new Ui::Dialog)
{


    QPalette* palette = new QPalette();
    QLinearGradient linearGradient(QPointF(0, 0), QPointF(30, 1024));
    linearGradient.setColorAt(0,QColor(245,255,245));
    linearGradient.setColorAt(1, QColor(151,213,98));
    palette->setBrush(QPalette::Window,*(new QBrush(linearGradient)));

    this->setPalette(*palette);


    nb_fidelite = 10;
    id_client = id+1;
    ui->setupUi(this);


    Qt::WindowFlags flags = windowFlags();
    Qt::WindowFlags helpFlag = Qt::WindowContextHelpButtonHint;
    Qt::WindowFlags maxFlag = Qt::Window;
    flags = flags & (maxFlag);
    setWindowFlags(flags);
    flags = flags & (~helpFlag);
    setWindowFlags(windowFlags());


    ui->ajoutPrestation->setIcon(QIcon("image/add.png"));
    ui->pushButton_OK->setIcon(QIcon("image/Ok.png"));
    ui->pushButton_retour->setIcon(QIcon("image/Cancel.png"));
    ui->pushButton_SAVE->setIcon(QIcon("image/save.png"));
    ui->pushButton_suppr->setIcon(QIcon("image/del.png"));

    model = new QStandardItemModel(0,2);
    model->setHeaderData(0, Qt::Horizontal, tr("Tarif (€)"));
    model->setHeaderData(1, Qt::Horizontal, tr("Date"));
    ui->tableView_prestations->setModel(model);
    ui->tableView_prestations->horizontalHeader()->setStretchLastSection(true);
    connect(ui->tableView_prestations,SIGNAL(doubleClicked(QModelIndex)),this,SLOT(appelDetailPrestation(QModelIndex)));
    connect(ui->pushButton_OK, SIGNAL(clicked()), this, SLOT(accept()));
    connect(ui->pushButton_retour, SIGNAL(clicked()), this, SLOT(reject()));

    QSqlQuery  requete;
    QString query;
    QString v;
    QString a;
    QString p;

    p = ");";
    v = ",";
    a = "'";

    query = "SELECT * FROM clients WHERE idClient = ";
    query+=QString::number(id+1)+";";

    //recuperation des infos personnel
    if(requete.exec(query))
    {
        requete.next();
        ui->lineEdit_nom->setText(requete.value(1).toString());
        ui->lineEdit_prenom->setText(requete.value(2).toString());
        ui->lineEdit_age->setText(requete.value(3).toString());
        ui->lineEdit_tel->setText(requete.value(4).toString());
        ui->lineEdit_mobile->setText(requete.value(5).toString());
        ui->lineEdit_mail->setText(requete.value(6).toString());
        ui->adresse_textEdit->setText(requete.value(7).toString());

        ui->lineEdit_nom->setStyleSheet( QString("QLabel{color:#000000;}"));
        ui->lineEdit_prenom->setStyleSheet( QString("QLabel{color:#000000;}"));
        ui->lineEdit_age->setStyleSheet( QString("QLabel{color:#000000;}"));
        ui->lineEdit_tel->setStyleSheet( QString("QLabel{color:#000000;}"));
        ui->lineEdit_mobile->setStyleSheet( QString("QLabel{color:#000000;}"));
        ui->lineEdit_mail->setStyleSheet( QString("QLabel{color:#000000;}"));
        ui->adresse_textEdit->setStyleSheet( QString("QLabel{color:#000000;}"));

        ui->label_Adresse->setStyleSheet( QString("QLabel{color:#000000;}"));
        ui->label_age->setStyleSheet( QString("QLabel{color:#000000;}"));
        ui->label_email->setStyleSheet( QString("QLabel{color:#000000;}"));
        ui->label_mobile->setStyleSheet( QString("QLabel{color:#000000;}"));
        ui->label_nom->setStyleSheet( QString("QLabel{color:#000000;}"));
        ui->label_prenom->setStyleSheet( QString("QLabel{color:#000000;}"));
        ui->label_tel->setStyleSheet( QString("QLabel{color:#000000;}"));
        modif=false;

        ui->ajoutPrestation->setFocus();

        if(requete.value(9).toString()=="1")
           ui->checkBox_newsletter->setChecked(true);
        else
           ui->checkBox_newsletter->setChecked(false);

        QString date_str = requete.value(8).toString();
        QDate date = QDate::fromString(date_str,"yyyy-MM-dd");
        ui->dateEdit_premVisite->setDate(date);
        ui->dateEdit_premVisite->setEnabled(true);


    }

    //recuperation des checkbox: santé
     query = "SELECT idSante FROM clients  natural join possedesante  natural join sante  ""WHERE idClient = ";
     query+=QString::number(id+1)+";";
     if(requete.exec(query))
     {
         while(requete.next())
         {
            int valeur = requete.value(0).toString().toInt();
            switch(valeur)
            {
                case 1:
                    ui->checkBox_sante_0->setChecked(true);
                break;

                case 2:
                    ui->checkBox_sante_1->setChecked(true);
                break;

                case 3:
                    ui->checkBox_sante_2->setChecked(true);
                break;

                case 4:
                    ui->checkBox_sante_3->setChecked(true);
                break;

                case 5 :
                    ui->checkBox_sante_4->setChecked(true);
                break;
            }
         }
     }

     //QString("Bad Query, It's inactive: %1").arg(query.lastError().text());


    //recuperation des prestations
     query = "SELECT DISTINCT prixTotal, date, idprestations FROM prestations  WHERE idClient = ";
     query+=QString::number(id+1)+" GROUP BY idprestations;";

   //  cout<<query.toStdString()<<endl;
     int y=0;
     int total = 0;
     nb_visite =0;
     if(requete.exec(query))
     {
         int nb=  requete.numRowsAffected();
        ui->nbVisite_valeur->setText(QString::number(nb) );

         while(requete.next())
         {
             for(int x=0; x < requete.record().count()-1; ++x)
             {
                 total+=requete.value(x).toInt();

                //changement format date
                 if(x%2==1)
                 {
                    QString d = requete.value(x).toString();
                    QDate date = QDate::fromString(d, "yyyy-MM-dd");
                    model->setItem(y,x, new QStandardItem(date.toString("dd-MM-yyyy")));
                 }
                else
                     model->setItem(y,x, new QStandardItem(requete.value(x).toString()));
             }

             liste.push_back(requete.value(2).toInt());
            // cout<<requete.value(2).toInt()<<endl;
             y++;
             nb_visite++;
         }



         ui->tableView_prestations->setSelectionBehavior(QAbstractItemView::SelectRows);
         ui->tableView_prestations->setEditTriggers(QAbstractItemView::NoEditTriggers);
         ui->tableView_prestations->setModel(model);

         ui->label_cout_total->setText(QString::number(total));
     }


 /*   //recuperation prescription beauté
     query = "SELECT idprescriptionBeaute FROM clients  natural join avoir  natural join prescriptionbeaute  ""WHERE idClient = ";
     query+=QString::number(id+1)+";";
     if(requete.exec(query))
     {
         while(requete.next())
         {

         }
     }*/

     //recuperation type de peau
     query = "SELECT idType FROM clients  natural join possedetype  natural join typepeau  ""WHERE idClient = ";
     query+=QString::number(id+1)+";";
     if(requete.exec(query))
     {
         while(requete.next())
         {
             int valeur = requete.value(0).toString().toInt();
             switch(valeur)
             {
                 case 1:
                     ui->checkBox_type_0->setChecked(true);
                 break;

                 case 2:
                     ui->checkBox_type_1->setChecked(true);
                 break;

                 case 3:
                     ui->checkBox_type_2->setChecked(true);
                 break;

                 case 4:
                     ui->checkBox_type_3->setChecked(true);
                 break;

                 case 5 :
                     ui->checkBox_type_4->setChecked(true);
                 break;
             }
         }
     }

     //recuperation information complémentaires
     query = "SELECT intitule FROM information WHERE idclient ="+QString::number(id+1)+";";

     /*cout<< QString(requete.lastError().text()).toStdString()<<endl;
     cout<<query.toStdString()<<endl;*/

     if(requete.exec(query))
     {
         requete.next();
         ui->textEdit_info->setText(requete.value(0).toString());
     }



     fidelite = new GraphFidelite(ui->verticalLayout_5);
     fidelite->set_nb(10);
     fidelite->set_size(QSize(300,200));
     fidelite->column(5);
     fidelite->row(2);
     fidelite->feutre(QPen(Qt::green));
     fidelite->font(QFont("Arial",30));
     fidelite->text("X");
     fidelite->Draw(nb_visite);
}

Dialog::~Dialog()
{
    delete fidelite;
    delete ui;
}



void Dialog::set_Fidelite(int nb){nb_fidelite=nb; repaint(); }
int Dialog::get_Fidelite(){return nb_fidelite;}




void Dialog::on_pushButton_retour_clicked()
{
    if(modif==true)
    {
        int ret=QMessageBox::warning(this,"Attention","Attention vous avez fait des modification \n Voulez vous enregistrer?",QMessageBox::Yes | QMessageBox::No);
        switch (ret)
        {
            case QMessageBox::Yes:
                if(Save())
                    close();
            break;

            case QMessageBox::No:
                 close();
            break;

        }
    }
    else
    {
        close();
    }
}

void Dialog::on_pushButton_OK_clicked()
{
    if(modif==true)
    {
        int ret=QMessageBox::warning(this,"Attention","Attention vous avez fait des modifications \n Voulez vous enregistrer?",QMessageBox::Yes | QMessageBox::No);
        switch (ret)
        {
            case QMessageBox::Yes:
                if(Save())
                    close();
            break;

            case QMessageBox::No:
                 close();
            break;

        }
    }
    else
    {
        close();
    }
}

void Dialog::on_pushButton_SAVE_clicked()
{
    if(Save())
        QMessageBox::information(this,"Enregistré!","Modification(s) enregistrée(s) avec succès :)");
    else
        QMessageBox::information(this,"Erreur!","Echec enregistrement des données :(");

    modif=false;

}


bool Dialog::Save()
{
        QString query;
        QString req;
        QString p;
        QString v;
        QString a;
        QString test;


        //test Info Personnel

        QRegExp reg( "^[a-zA-z]+$");
        QRegExp reg1( "^[0-9]{1,3}$");
        QRegExp reg2( "^[\\w|\\.]+@[\\w]+\\.[\\w]{2,4}$");
        QRegExp reg3( "^0[0-9]{9}");

        //test nom
        test= ui->lineEdit_nom->text();
        if(test != "")
        if(!test.contains(reg) )
        {
            QMessageBox::warning(this,"Erreur nom","Le nom contient des caractères non valides ou le champ est vide");
            return false;//retourne erreur
        }
        //test prénom
        test= ui->lineEdit_prenom->text();
        if(test != "")
        if(!test.contains(reg))
        {
            QMessageBox::warning(this,"Erreur prénom","Le prénom contient des caractères non valides ou le champ est vide");
            return false;//retourne erreur
        }

        if(ui->lineEdit_prenom->text() == "" &&  ui->lineEdit_nom->text() == "" )
        {
            QMessageBox::warning(this,"Erreur prénom, nom","Un nom ou prénom doit etre renseigné");
            return false;//retourne erreur
        }


        //test age
        test=ui->lineEdit_age->text();
        if(test != "")
        if(!test.contains(reg1))
        {
            QMessageBox::warning(this,"Erreur age","L'age contient des caractères non valides");
            return false;//retourne erreur
        }

        //test email
        test= ui->lineEdit_mail->text();
        if(test != "")
        if(!test.contains(reg2))
        {
            QMessageBox::warning(this,"Erreur Email","Le champ email est mal renseigné ou il est vide");
            return false;//retourne erreur
        }

        //test tel
        //test si au moins un des tel est renseigné
        if((ui->lineEdit_mobile->text()=="" && ui->lineEdit_tel->text()=="" )||( ui->lineEdit_mobile->text()=="NC" && ui->lineEdit_tel->text()=="NC"))
        {
            QMessageBox::warning(this,"Erreur info tel","Au moins un des champ tel doit etre rempli");
            ui->lineEdit_tel->setStyleSheet( QString("QLineEdit{border: 1px solid #ffaa00;}"));
            ui->label_tel->setStyleSheet( QString("QLabel{color:#ffaa00;}"));
            ui->lineEdit_mobile->setStyleSheet( QString("QLineEdit{border: 1px solid #ffaa00;}"));
            ui->label_mobile->setStyleSheet( QString("QLabel{color:#ffaa00;}"));
            return false;//retourne erreur
        }
        else
        {
            //test tel mobile
            test= ui->lineEdit_mobile->text();
            if(test!="")
            {
                if(!test.contains(reg3) && test != "NC")
                {
                    QMessageBox::warning(this,"Erreur mobile","Le champ Tel mobile est mal renseigné");
                    return false;//retourne erreur
                }
            }
            else
            {
               ui->lineEdit_mobile->setText("NC");
               ui->lineEdit_mobile->setStyleSheet( QString("QLineEdit{border: 1px solid #00ff00;}"));
               ui->label_mobile->setStyleSheet( QString("QLabel{color:#00ff00;}"));
            }

            //test fixe
            test= ui->lineEdit_tel->text();
            if(test!="")
            {
                if(!test.contains(reg3) && test != "NC")
                {
                    QMessageBox::warning(this,"Erreur Tel","Le champ tel fixe est mal renseigné");
                    return false;//retourne erreur
                }

            }
            else
            {
                ui->lineEdit_tel->setText("NC");
                ui->lineEdit_tel->setStyleSheet( QString("QLineEdit{border: 1px solid #ffaa00;}"));
                ui->label_tel->setStyleSheet( QString("QLabel{color:#00ff00;}"));
            }
          }

         //( nom, prenom, age, telephoneFixe,mobile,mail,adresse,newsletter,date)

        //ajout info personnel dans la base de donnée
        query = "UPDATE clients SET ";
        p = ");";
        v = ",";
        a = "'";

        QString test_age = ui->lineEdit_age->text();
        if(test_age == "")
            test_age = "NULL";

        req = query+" nom = "+a+ui->lineEdit_nom->text()+a+v+" prenom = "+a+ui->lineEdit_prenom->text()+a+v+" age = "+test_age+v+" telephoneFixe = " +a+ui->lineEdit_tel->text()+a+v+" mobile = "+a+ui->lineEdit_mobile->text()
                +a+v+" mail = "+a+ui->lineEdit_mail->text()+a+v+" adresse = "+a+ui->adresse_textEdit->toPlainText()+a+v;

        if(ui->checkBox_newsletter->isChecked())
            req+=" newsletter = "+a+QString::number(1)+a;
        else
            req+=" newsletter = "+a+QString::number(0)+a;

        QDate date_text = ui->dateEdit_premVisite->date();
        req+=v+"date ="+a+date_text.toString("yyyy-MM-dd")+a;

        req+=" WHERE idClient = "+QString::number(id_client)+";";

        QSqlQuery requete;
        if(!requete.exec(req))
            QMessageBox::information(this,"Erreur!","Echec de la maj d'un client");


        //delete les types et les soins
        req = " DELETE FROM possedesante WHERE idclient ="+QString::number(id_client)+";";
        requete.exec(req);
        req = " DELETE FROM possedetype WHERE idclient ="+QString::number(id_client)+";";
        requete.exec(req);

        //santé
        for(int i=0;i<ui->verticalLayout_3->count();i++)
        {
           QCheckBox * check = findChild<QCheckBox*>(ui->verticalLayout_3->itemAt(i)->widget()->objectName());
            if(check)
            {
                if(check->isChecked())
                {
                   req = "INSERT INTO possedesante (idclient,idSante) values("+a+QString::number(id_client)+a+v+a+QString::number(i)+a+p;
                   requete.exec(req);
                }
              }
        }

        //type
        for(int i=0;i<ui->verticalLayout_4->count();i++)
        {
           QCheckBox * check = findChild<QCheckBox*>(ui->verticalLayout_4->itemAt(i)->widget()->objectName());
            if(check)
            {
                if(check->isChecked())
                {
                   req = "INSERT INTO possedetype (idclient,idtype) values("+a+QString::number(id_client)+a+v+a+QString::number(i)+a+p;
                   requete.exec(req);
                }
              }
        }


        //ajout information
        QString txt=ui->textEdit_info->toPlainText();


         req="UPDATE information SET intitule = "+a+txt+a+" WHERE idclient="+QString::number(id_client)+";";
         if(requete.exec(req))
         {
             //test si une ligne est affecté ( si <=0 alors aucune affecté)
             if(requete.numRowsAffected()<=0)
             {
                req = "SELECT * FROM information WHERE idclient="+QString::number(id_client)+";";
                requete.exec(req);

                //test si la ligne existe pas alors, on insert, sinon c'est que la maj ne change rien
                if(requete.size()<=0)
                {
                    req = "INSERT INTO  information(idclient,intitule) VALUE ("+QString::number(id_client)+","+a+txt+a+");";
                    requete.exec(req);
                }
             }
         }
         else
            QMessageBox::warning(this,"erreur","update error");


    return true; // retourne ok
}




void Dialog::on_ajoutPrestation_clicked()
{
    DialogPrestation* myDialog(new DialogPrestation(id_client,this));

    //cout<<nb_visite<<endl;
    if(fidelite->Draw(nb_visite+1))
        QMessageBox::information(this,"Info","Carte de fidélité pleine, remise à 0 immediate");

    int ret = myDialog->exec();
    if(ret==QDialog::Accepted)
    {
       Recharger();
    }

    fidelite->Draw(nb_visite);
}

void Dialog::appelDetailPrestation(QModelIndex index)
{
    QSqlQuery  requete;
    int a = index.row();
   // cout<<a<<endl;
    int b = liste[a];// + index.row();
    //int b = a+1;
   // cout<<b<<endl;
    QString id_prest2 = QString::number(b);//idprest
    QString id_prest = QString::number(b); //idprestations
    QString query = "SELECT s.intitule, s.prix, c.idprest FROM prestations p NATURAL JOIN  concerne c NATURAL JOIN   soins s WHERE idClient = "+QString::number(id_client)+" and idprest = "+id_prest+" and idprestations = "+id_prest2+";";
    requete.exec(query);
    double total = 0;

  //  cout<<query.toStdString()<<endl;


    QString mess = "Soin(s):\n";
    while(requete.next())
    {
        mess+=requete.value(0).toString()+" = "+requete.value(1).toString()+"€";
        mess+="\n";
      //  total+=requete.value(1).toInt();
    }

    query = "SELECT DISTINCT prixTotal FROM prestations  WHERE idprestations ="+id_prest+";";
   // cout<<query.toStdString()<<endl;
    requete.exec(query);
    requete.next();
    total = requete.value(0).toDouble();


    query = "SELECT * FROM concerne_promotions c,promotions p where id_prestation ="+id_prest+" and c.id_promotion = p.id;";

    //cout<<query.toStdString()<<endl;
    requete.exec(query);
    mess+="\nPromotion(s) appliquée(s):\n";
    while(requete.next())
    {
        mess+=requete.value(4).toString()+" : -"+requete.value(5).toString()+"%";
        mess+="\n";
    }


    mess+="\nTotal : "+QString::number(total)+"€";
    QMessageBox::StandardButton reply = QMessageBox::information(this,"Information Prestations",mess,QMessageBox::Ok|QMessageBox::Discard);
    if(reply == QMessageBox::Discard)
    {
        int err=QMessageBox::warning(this,"Attention","Voulez vous supprimer le soin?",QMessageBox::Ok | QMessageBox::No);
        if(err == QMessageBox::Ok)
        {
            query = "DELETE FROM concerne WHERE idprest="+id_prest+";";
            requete.exec(query);
            query = "DELETE FROM prestations WHERE idprestations="+id_prest2+";";
            requete.exec(query);
            query = "DELETE FROM concerne_promotions WHERE id_prestation="+id_prest2+";";
            requete.exec(query);

            //mettre à jour vue
            fidelite->Draw(nb_visite-1);
            Recharger();
        }
    }
}

void Dialog::on_pushButton_suppr_clicked()
{
    int ret = QMessageBox::warning(this,"Suppression de client","Voulez vous supprimer ce client?", QMessageBox::Yes | QMessageBox::No);
    switch(ret)
    {
        case QMessageBox::Yes:
            Suppr();
            close();
        break;
    }
}

void Dialog::Suppr()
{
    QSqlQuery  requete;
    QString query = "DELETE FROM clients WHERE idClient = "+QString::number(id_client)+";";
    requete.exec(query);

//    fidelite->Draw(nb_visite);
}


void Dialog::Recharger()
{
    //recuperation des prestations
     QSqlQuery  requete;
     QString query = "SELECT DISTINCT prixTotal, date, idprestations FROM prestations  ""WHERE idClient = ";
     query+=QString::number(id_client)+";";
     int y=0;
     if(requete.exec(query))
     {
        int nb= requete.numRowsAffected();
        int total=0;
        ui->nbVisite_valeur->setText(QString::number(nb) );
        nb_visite = nb;
        liste.clear();
        model->clear();
        model = new QStandardItemModel(0,2);
        model->setHeaderData(0, Qt::Horizontal, tr("Tarif (€)"));
        model->setHeaderData(1, Qt::Horizontal, tr("Date"));

         while(requete.next())
         {
             for(int x=0; x < requete.record().count()-1; ++x)
             {
                 total+=requete.value(x).toInt();
                 model->setItem(y,x, new QStandardItem(requete.value(x).toString()));
                // liste.push_back(requete.value(2).toInt());
             }
             liste.push_back(requete.value(2).toInt());
             y++;
         }

         ui->label_cout_total->setText(QString::number(total));

         ui->tableView_prestations->setSelectionBehavior(QAbstractItemView::SelectRows);
         ui->tableView_prestations->setEditTriggers(QAbstractItemView::NoEditTriggers);
         ui->tableView_prestations->setModel(model);

         /*if(fidelite->Draw(nb_visite))
             QMessageBox::information(this,"Info","Reduction2");*/

     }
}

void Dialog::on_lineEdit_nom_textChanged(const QString &arg1)
{
     modif=true;
     QRegExp reg( "^[a-zA-z]+$");
     QString test= arg1;
     if(!test.contains(reg))
     {
         ui->lineEdit_nom->setStyleSheet( QString("QLineEdit{border: 1px solid #FF0000;}"));
         ui->label_nom->setStyleSheet( QString("QLabel{color:#FF0000;}"));
     }
     else
     {
         ui->lineEdit_nom->setStyleSheet( QString("QLineEdit{border: 1px solid #00FF00;}"));
         ui->label_nom->setStyleSheet( QString("QLabel{color:#00FF00;}"));
     }
}

void Dialog::on_lineEdit_prenom_textChanged(const QString &arg1)
{
    modif=true;
    QRegExp reg( "^[a-zA-z]+$");
    QString test= arg1;
    if(!test.contains(reg))
    {
        ui->lineEdit_prenom->setStyleSheet( QString("QLineEdit{border: 1px solid #FF0000;}"));
        ui->label_prenom->setStyleSheet( QString("QLabel{color:#FF0000;}"));
    }
    else
    {
        ui->lineEdit_prenom->setStyleSheet( QString("QLineEdit{border: 1px solid #00FF00;}"));
        ui->label_prenom->setStyleSheet( QString("QLabel{color:#00FF00;}"));
    }
}

void Dialog::on_lineEdit_age_textChanged(const QString &arg1)
{
    modif=true;
    QRegExp reg("^[0-9]{1,3}$");
    QString test= arg1;
    if(!test.contains(reg))
    {
        ui->lineEdit_age->setStyleSheet( QString("QLineEdit{border: 1px solid #FF0000;}"));
        ui->label_age->setStyleSheet( QString("QLabel{color:#FF0000;}"));
    }
    else
    {
        ui->lineEdit_age->setStyleSheet( QString("QLineEdit{border: 1px solid #00FF00;}"));
        ui->label_age->setStyleSheet( QString("QLabel{color:#00FF00;}"));
    }
}

void Dialog::on_lineEdit_mobile_textChanged(const QString &arg1)
{
    modif=true;
    QRegExp reg( "^0[0-9]{9}$");
    QString test= arg1;
    if(test=="NC")
    {
            ui->lineEdit_mobile->setStyleSheet( QString("QLineEdit{border: 1px solid #00FF00;}"));
            ui->label_mobile->setStyleSheet( QString("QLabel{color:#00FF00;}"));
    }
    else
    {
        if(!test.contains(reg) || test == "")
        {
            ui->lineEdit_mobile->setStyleSheet( QString("QLineEdit{border: 1px solid #FF0000;}"));
            ui->label_mobile->setStyleSheet( QString("QLabel{color:#FF0000;}"));
        }
        else
        {
            ui->lineEdit_mobile->setStyleSheet( QString("QLineEdit{border: 1px solid #00FF00;}"));
            ui->label_mobile->setStyleSheet( QString("QLabel{color:#00FF00;}"));
        }
    }
}

void Dialog::on_lineEdit_tel_textChanged(const QString &arg1)
{
    modif=true;
    QRegExp reg( "^0[0-9]{9}$");
    QString test= arg1;
    if(test=="NC")
    {
            ui->lineEdit_tel->setStyleSheet( QString("QLineEdit{border: 1px solid #00FF00;}"));
            ui->label_tel->setStyleSheet( QString("QLabel{color:#00FF00;}"));
    }
    else
    {
        if(!test.contains(reg) || test == "")
        {
            ui->lineEdit_tel->setStyleSheet( QString("QLineEdit{border: 1px solid #FF0000;}"));
            ui->label_tel->setStyleSheet( QString("QLabel{color:#FF0000;}"));
        }
        else
        {
            ui->lineEdit_tel->setStyleSheet( QString("QLineEdit{border: 1px solid #00FF00;}"));
            ui->label_tel->setStyleSheet( QString("QLabel{color:#00FF00;}"));
        }
    }
}

void Dialog::on_lineEdit_mail_textChanged(const QString &arg1)
{
    modif=true;
    QRegExp reg("^[\\w|\\.]+@[\\w]+\\.[\\w]{2,4}$");
    QString test=arg1;
    if(!test.contains(reg))
    {
        ui->lineEdit_mail->setStyleSheet( QString("QLineEdit{border: 1px solid #FF0000;}"));
        ui->label_email->setStyleSheet( QString("QLabel{color:#FF0000;}"));
    }
    else
    {
        ui->lineEdit_mail->setStyleSheet( QString("QLineEdit{border: 1px solid #00FF00;}"));
        ui->label_email->setStyleSheet( QString("QLabel{color:#00FF00;}"));
    }
}

void Dialog::on_adresse_textEdit_textChanged()
{
    modif=true;
}

void Dialog::on_checkBox_newsletter_stateChanged(int arg1)
{
    modif=true;
}

void Dialog::on_checkBox_type_0_stateChanged(int arg1)
{
    modif=true;
}

void Dialog::on_checkBox_type_1_stateChanged(int arg1)
{
    modif=true;
}

void Dialog::on_checkBox_type_2_stateChanged(int arg1)
{
    modif=true;
}

void Dialog::on_checkBox_type_3_stateChanged(int arg1)
{
    modif=true;
}

void Dialog::on_checkBox_sante_0_stateChanged(int arg1)
{
    modif=true;
}

void Dialog::on_checkBox_sante_1_stateChanged(int arg1)
{
    modif=true;
}

void Dialog::on_checkBox_sante_2_stateChanged(int arg1)
{
    modif=true;
}

void Dialog::on_checkBox_sante_3_stateChanged(int arg1)
{
    modif=true;
}

void Dialog::on_checkBox_sante_4_stateChanged(int arg1)
{
    modif=true;
}
