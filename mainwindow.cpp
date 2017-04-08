#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "prixconfigw.h"
#include "gestion.h"
#include "argentio.h"
#include "windowpromo.h"
#include "uistock.h"
#include "planningui.h"

MainWindow::MainWindow(QWidget *parent) :QMainWindow(parent),ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    index =0;
    index_max=2;
    id_selec=-1;
    ui->stackedWidget->setCurrentIndex(index);

    QPalette* palette = new QPalette();
    QLinearGradient linearGradient(QPointF(0, 0), QPointF(30, 1024));
    linearGradient.setColorAt(0,QColor(245,255,245));
    linearGradient.setColorAt(1, QColor(151,213,98));
    palette->setBrush(QPalette::Window,*(new QBrush(linearGradient)));

    this->setPalette(*palette);

    MdpDialog dial;
    int ret=dial.exec();


    model = new QStandardItemModel(0,7);
    model->setHeaderData(0, Qt::Horizontal, tr("Id client"));
    model->setHeaderData(1, Qt::Horizontal, tr("Nom"));
    model->setHeaderData(2, Qt::Horizontal, tr("Prénom"));
    model->setHeaderData(3, Qt::Horizontal, tr("Age"));
    model->setHeaderData(4, Qt::Horizontal, tr("Tel Fixe"));
    model->setHeaderData(5, Qt::Horizontal, tr("Tel Mobile"));
    model->setHeaderData(6, Qt::Horizontal, tr("Mail"));

    ui->tableView_Principale->setAlternatingRowColors(true);
    ui->tableView_Principale->horizontalHeader()->setStretchLastSection(true);
    ui->tableView_Principale->setModel(model);
    connect(ui->tableView_Principale,SIGNAL(doubleClicked(QModelIndex)),this,SLOT(appelFicheClient(QModelIndex)));
    connect(ui->tableView_Principale,SIGNAL(clicked(QModelIndex)),this,SLOT(modif_id_select(QModelIndex)));

    ui->voirTTClientBoutton->setIcon(QIcon("image/search.png"));
    ui->pushButton_supprClient->setIcon(QIcon("image/del.png"));
    ui->creationBouton->setIcon(QIcon("image/add.png"));
    ui->rechercherBouton->setIcon(QIcon("image/Search-user-icon.png"));
    ui->razBouton->setIcon(QIcon("image/Undo.png"));
    ui->sauvegarderBouton->setIcon(QIcon("image/save.png"));
    ui->quitterFormButton->setIcon(QIcon("image/Cancel.png"));



    Recherche();
}

MainWindow::~MainWindow()
{
   // db.close();
    delete model;
    delete ui;
}

void MainWindow::on_creationBouton_clicked()
{
    index = 1;
    Reinit();
    ui->stackedWidget->setCurrentIndex(index);
}

int MainWindow::Save()
{
        QString query;
        QString req;
        QString p;
        QString v;
        QString a;
        QString test;


        //test Info Personnel

        QRegExp reg( "^[a-zA-Z]+$");
        QRegExp reg1( "^[0-9]{1,3}$");
        QRegExp reg2("^[\\w|\\.]+@[\\w]+\\.[\\w]{2,4}$");
        QRegExp reg3( "^0[0-9]{9}");

        //test nom
        test= ui->nomEdit->text();
        if(test != "")
        if(!test.contains(reg))
        {
            QMessageBox::warning(this,"Erreur nom","Le nom contient des caractères non valides");
            return 0;//retourne erreur
        }
        //test nom
        test= ui->prenomEdit->text();
        if(test != "")
        if(!test.contains(reg))
        {
            QMessageBox::warning(this,"Erreur prénom","Le prénom contient des caractères non valides");
            return 0;//retourne erreur
        }
        //test age
        test=ui->lineEditAge->text();
        if(test != "")
        if(!test.contains(reg1))
        {
            QMessageBox::warning(this,"Erreur age","L'age contient des caractères non valides");
            return 0;//retourne erreur
        }

        //test email ( renseignement non obligatoire )
        test= ui->lineEdit_Email->text();
        if(test!="")
        {
            if(!test.contains(reg2))
            {
                QMessageBox::warning(this,"Erreur Email","Le champ email est mal renseigné");
                return 0;//retourne erreur
            }
        }
        //test tel
        //test si au moins un des tel est renseigné
        if(ui->lineEdit_TEl->text()=="" && ui->lineEditMobile->text()=="")
        {
            QMessageBox::warning(this,"Erreur info tel","Au moins un des champ tel doit etre rempli");
            return 0;//retourne erreur
        }
        else
        {
            //test tel mobile
            test= ui->lineEditMobile->text();
            if(test!="")
            {
                if(!test.contains(reg3) && test != "NC")
                {
                    QMessageBox::warning(this,"Erreur mobile","Le champ Tel mobile est mal renseigné");
                    return 0;//retourne erreur
                }
            }
            else
            {
               ui->lineEditMobile->setText("NC");
            }

            //test fixe
            test= ui->lineEdit_TEl->text();
            if(test!="")
            {
                if(!test.contains(reg3) && test != "NC")
                {
                    QMessageBox::warning(this,"Erreur Email","Le champ tel fixe est mal renseigné");
                    return 0;//retourne erreur
                }
            }
            else
            {
                ui->lineEdit_TEl->setText("NC");
            }
          }

        //ajout info personnel dans la base de donnée
        query = "INSERT INTO clients ( nom, prenom, age, telephoneFixe,mobile,mail,adresse,newsletter,date) values(";
        p = ");";
        v = ",";
        a = "'";

        QString test_age = ui->lineEditAge->text();
        if(test_age == "")
            test_age = "NULL";

        req = query+a+ui->nomEdit->text()+a+v+a+ui->prenomEdit->text()+a+v+test_age+v+a+ui->lineEdit_TEl->text()+a+v+a+ui->lineEditMobile->text()
                +a+v+a+ui->lineEdit_Email->text()+a+v+a+ui->textEditAdresse->toPlainText()+a+v;

        if(ui->CheckNewsletter->isChecked())
            req+=a+QString::number(1)+a+v+a;
        else
            req+=a+QString::number(0)+a+v+a;

        //ajout date enregistrement AAAA-MM-JJ
        QDate date = QDate::currentDate();
        QString dateString = date.toString("yyyy-MM-dd");
        req+=dateString+a+p;


        QSqlQuery requete;
        if(!requete.exec(req))
            QMessageBox::information(this,"Erreur!","Echec de l'ajout d'un client");

        int idclient = 0;
        req = "SELECT MAX(idClient) FROM clients ;";

        requete.exec(req);
        requete.next();
        QString std = requete.value(0).toString();
        idclient = std.toInt();



        //santé
        for(int i=0;i<ui->verticalLayout_5->count();i++)
        {
           QCheckBox * check = findChild<QCheckBox*>(ui->verticalLayout_5->itemAt(i)->widget()->objectName());
            if(check)
            {
                if(check->isChecked())
                {
                   req = "INSERT INTO possedesante (idclient,idSante) values("+a+QString::number(idclient)+a+v+a+QString::number(i+1)+a+p;
                   requete.exec(req);
                }
              }
        }

        //type
        for(int i=0;i<ui->verticalLayout_7->count();i++)
        {
           QCheckBox * check = findChild<QCheckBox*>(ui->verticalLayout_7->itemAt(i)->widget()->objectName());
            if(check)
            {
                if(check->isChecked())
                {
                   req = "INSERT INTO possedetype (idclient,idtype) values("+a+QString::number(idclient)+a+v+a+QString::number(i+1)+a+p;
                   requete.exec(req);
                }
              }
        }

        //info suplementaire:
        QString text = ui->textEdit_info->toPlainText();
        req = "INSERT INTO information (idclient,intitule) values("+a+QString::number(idclient)+a+v+a+text+a+p;
        requete.exec(req);

        cout<< QString(requete.lastError().text()).toStdString()<<endl;
        cout<<req.toStdString()<<endl;
        cout<<idclient<<endl;


    Recherche();
    return 1; // retourn ok
}

void MainWindow::Reinit()
{
    ui->nomEdit->clear();
    ui->prenomEdit->clear();
    ui->lineEditAge->clear();
    ui->lineEditMobile->clear();
    ui->lineEdit_Email->clear();
    ui->lineEdit_TEl->clear();
    ui->textEditAdresse->clear();
    ui->textEdit_info->clear();
}

void MainWindow::on_quitterFormButton_clicked()
{
    if(TestChamp())
    {
        index=0;
        Reinit();
        ui->stackedWidget->setCurrentIndex(index);
    }
}

void MainWindow::on_sauvegarderBouton_clicked()
{
    if(Save())
    {
        Reinit();
        index=0;
        ui->stackedWidget->setCurrentIndex(index);
    }
}

void MainWindow::on_razBouton_clicked()
{
    Reinit();
}

void MainWindow::Recherche()
{
        QString str = ui->barreRecherche->text();
        QStringList liste = str.split(" ");
        QString query = "SELECT idClient,nom,prenom,age,telephoneFixe,mobile,mail FROM clients where (";
        QString ou = " OR ";
        QString s ="%";
        QString  p = ");";
        QString v = ",";
        QString a = "'";
        QString nom = "nom LIKE ";
        QString prenom = "prenom LIKE ";
        QString req;
        QSqlQuery requete;

        req +=query;

        for(int i=0; i < liste.count();i++)
        {
            req = req+nom+a+s+liste[i]+s+a;
            if(i<liste.count()-1)
                req+=ou;
        }
        req+=") OR (";

        for(int i=0; i < liste.count();i++)
        {
            req = req+prenom+a+s+liste[i]+s+a;
            if(i<liste.count()-1)
                req+=ou;
        }

        req+=")";
        req+=" ORDER BY nom ";
        req+=";";

        delete model;
        id_selec=-1;
        int y=0;

        if(requete.exec(req))
        {
           model = new QStandardItemModel(requete.numRowsAffected(),requete.record().count(),this);
           model->setHeaderData(0, Qt::Horizontal, tr("Id client"));
           model->setHeaderData(1, Qt::Horizontal, tr("Nom"));
           model->setHeaderData(2, Qt::Horizontal, tr("Prénom"));
           model->setHeaderData(3, Qt::Horizontal, tr("Age"));
           model->setHeaderData(4, Qt::Horizontal, tr("Tel Fixe"));
           model->setHeaderData(5, Qt::Horizontal, tr("Tel Mobile"));
           model->setHeaderData(6, Qt::Horizontal, tr("Mail"));

            while(requete.next())
            {
                for(int x=0; x < requete.record().count(); ++x) //colone
                {
                    model->setItem(y,x, new QStandardItem(requete.value(x).toString()));
                }
                y++; //ligne
            }

            ui->tableView_Principale->setSelectionBehavior(QAbstractItemView::SelectRows);
            ui->tableView_Principale->setEditTriggers(QAbstractItemView::NoEditTriggers);
            ui->tableView_Principale->setModel(model);

            QString txt ="Nombre de clients: ";
            txt+=QString::number(y);
            ui->label_nb_client->setText(txt);

        }
        else
        {
            //QMessageBox::warning(this,"Erreur","Erreur recherche dans base de donnée");
            exit(0);
        }
}

void MainWindow::on_rechercherBouton_clicked()
{
        if(ui->barreRecherche->text()!="")
            Recherche();
        else
            on_voirTTClientBoutton_clicked();

}


void MainWindow::appelFicheClient(QModelIndex id)
{  
      QString str = id.sibling(id.row(),0).data().toString();
      Dialog* myDialog(new Dialog(str.toInt()-1,this));
      myDialog->setModal(true);
      int ret = myDialog->exec();

       on_rechercherBouton_clicked();
}

void MainWindow::on_voirTTClientBoutton_clicked()
{
        QSqlQuery  requete;
        QString query;
        QString v;
        QString a;
        QString p;

        p = ");";
        v = ",";
        a = "'";

        query = "SELECT idClient,nom,prenom,age,telephoneFixe,mobile,mail FROM clients ORDER BY nom;";


        delete model;
        id_selec=-1;
        int y=0;

        if(requete.exec(query))
        {
            model = new QStandardItemModel(requete.numRowsAffected(),requete.record().count(),this);
            model->setHeaderData(0, Qt::Horizontal, tr("Id client"));
            model->setHeaderData(1, Qt::Horizontal, tr("Nom"));
            model->setHeaderData(2, Qt::Horizontal, tr("Prénom"));
            model->setHeaderData(3, Qt::Horizontal, tr("Age"));
            model->setHeaderData(4, Qt::Horizontal, tr("Tel Fixe"));
            model->setHeaderData(5, Qt::Horizontal, tr("Tel Mobile"));
            model->setHeaderData(6, Qt::Horizontal, tr("Mail"));

            while(requete.next())
            {
                for(int x=0; x < requete.record().count(); ++x)
                {
                    model->setItem(y,x, new QStandardItem(requete.value(x).toString()));
                }
                y++;
            }
            ui->tableView_Principale->setSelectionBehavior(QAbstractItemView::SelectRows);
            ui->tableView_Principale->setEditTriggers(QAbstractItemView::NoEditTriggers);
            ui->tableView_Principale->setModel(model);
        }
}

void MainWindow::on_pushButton_supprClient_clicked()
{
    if(id_selec!=-1)
    {
        int ret = QMessageBox::warning(this,"Suppression de client","Voulez vous supprimer ce client?", QMessageBox::Yes | QMessageBox::No);
        switch(ret)
        {
            case QMessageBox::Yes:
                Suppr();
                Recherche();
            break;
        }
    }

}

void MainWindow::Suppr()
{
    QSqlQuery  requete;
    QString query = "DELETE FROM clients WHERE idClient = "+QString::number(id_selec)+";";
    requete.exec(query);
}


void MainWindow::modif_id_select(QModelIndex id)
{
     QString str = id.sibling(id.row(),0).data().toString();
     id_selec = str.toInt();
}


bool MainWindow::TestChamp()
{
    if(ui->lineEditAge->text()!= "" || ui->lineEditMobile->text() != "" || ui->lineEdit_Email->text() != "" || ui->lineEdit_TEl->text() != ""  || ui->textEditAdresse->toPlainText() != "" || ui->nomEdit->text() != "" || ui->prenomEdit->text() != "")
    {
        int ret=QMessageBox::warning(this,"Quittez?","Des informations ont été saisies<br> <b> Voulez vous quittez ?<b>",QMessageBox::Yes | QMessageBox::No);
        if(ret==QMessageBox::Yes)
        {return true;}
        else
        {return false;}
    }

    return true;
}

void MainWindow::on_nomEdit_textChanged(const QString &arg1)
{
    QRegExp reg( "^[a-zA-z]+$");
    QString test= arg1;
    if(!test.contains(reg) || test == "")
    {
        ui->nomEdit->setStyleSheet( QString("QLineEdit{border: 1px solid #FF0000;}"));
        ui->label_nom->setStyleSheet( QString("QLabel{color:#FF0000;}"));
    }
    else
    {
        ui->nomEdit->setStyleSheet( QString("QLineEdit{border: 1px solid #00FF00;}"));
        ui->label_nom->setStyleSheet( QString("QLabel{color:#00FF00;}"));
    }
}

void MainWindow::on_prenomEdit_textEdited(const QString &arg1)
{
    QRegExp reg( "^[a-zA-z]+$");
    QString test= arg1;
    if(!test.contains(reg) || test == "")
    {
        ui->prenomEdit->setStyleSheet( QString("QLineEdit{border: 1px solid #FF0000;}"));
        ui->label_prenom->setStyleSheet( QString("QLabel{color:#FF0000;}"));
    }
    else
    {
        ui->prenomEdit->setStyleSheet( QString("QLineEdit{border: 1px solid #00FF00;}"));
        ui->label_prenom->setStyleSheet( QString("QLabel{color:#00FF00;}"));
    }
}

void MainWindow::on_lineEditAge_textEdited(const QString &arg1)
{
    QRegExp reg( "^[0-9]{1,3}$");
    QString test= arg1;
    if(!test.contains(reg) || test == "")
    {
        ui->lineEditAge->setStyleSheet( QString("QLineEdit{border: 1px solid #FF0000;}"));
        ui->ageLabel->setStyleSheet( QString("QLabel{color:#FF0000;}"));
    }
    else
    {
        ui->lineEditAge->setStyleSheet( QString("QLineEdit{border: 1px solid #00FF00;}"));
        ui->ageLabel->setStyleSheet( QString("QLabel{color:#00FF00;}"));
    }
}

void MainWindow::on_lineEditMobile_textEdited(const QString &arg1)
{
    QRegExp reg( "^0[0-9]{9}$");
    QString test= arg1;
    if(test=="NC")
    {
            ui->lineEditMobile->setStyleSheet( QString("QLineEdit{border: 1px solid #00FF00;}"));
            ui->mobileLabel->setStyleSheet( QString("QLabel{color:#00FF00;}"));
    }
    else
    {
        if(!test.contains(reg) || test == "")
        {
            ui->lineEditMobile->setStyleSheet( QString("QLineEdit{border: 1px solid #FF0000;}"));
            ui->mobileLabel->setStyleSheet( QString("QLabel{color:#FF0000;}"));
        }
        else
        {
            ui->lineEditMobile->setStyleSheet( QString("QLineEdit{border: 1px solid #00FF00;}"));
            ui->mobileLabel->setStyleSheet( QString("QLabel{color:#00FF00;}"));
        }
    }
}

void MainWindow::on_lineEdit_TEl_textEdited(const QString &arg1)
{
    QRegExp reg( "^0[0-9]{9}$");
    QString test= arg1;
    if(test=="NC")
    {
            ui->lineEdit_TEl->setStyleSheet( QString("QLineEdit{border: 1px solid #00FF00;}"));
            ui->telLabel->setStyleSheet( QString("QLabel{color:#00FF00;}"));
    }
    else
    {
        if(!test.contains(reg) || test == "")
        {
            ui->lineEdit_TEl->setStyleSheet( QString("QLineEdit{border: 1px solid #FF0000;}"));
            ui->telLabel->setStyleSheet( QString("QLabel{color:#FF0000;}"));
        }
        else
        {
            ui->lineEdit_TEl->setStyleSheet( QString("QLineEdit{border: 1px solid #00FF00;}"));
            ui->telLabel->setStyleSheet( QString("QLabel{color:#00FF00;}"));
        }
    }
}

void MainWindow::on_lineEdit_Email_textEdited(const QString &arg1)
{
    QRegExp reg("^[\\w|\\.]+@[\\w]+\\.[\\w]{2,4}$");
    QString test=arg1;
    if(test!="")
    {
        if(!test.contains(reg))
        {
            ui->lineEdit_Email->setStyleSheet( QString("QLineEdit{border: 1px solid #FF0000;}"));
            ui->mailLabel->setStyleSheet( QString("QLabel{color:#FF0000;}"));
        }
        else
        {
            ui->lineEdit_Email->setStyleSheet( QString("QLineEdit{border: 1px solid #00FF00;}"));
            ui->mailLabel->setStyleSheet( QString("QLabel{color:#00FF00;}"));
        }
    }
}

void MainWindow::on_pushButton_modifPrix_clicked()
{
    PrixConfigW *windows  = new PrixConfigW(this);
    this->hide();

    QPalette* palette = new QPalette();
    QLinearGradient linearGradient(qreal(0),qreal(0), qreal(0),qreal(1000));
    linearGradient.setSpread(QGradient::PadSpread);
    linearGradient.setColorAt(0, QColor(240,255,240,255));
    linearGradient.setColorAt(1, QColor(151,213,98,255));
    palette->setBrush(QPalette::Window,*(new QBrush(linearGradient)));
    windows->setPalette(*palette);

    windows->setFocus();
    windows->show();
    this->show();
}

void MainWindow::on_button_gestion_clicked()
{
    Gestion *windows  = new Gestion(this);
    this->hide();


    QPalette* palette = new QPalette();
    QLinearGradient linearGradient(qreal(0),qreal(0), qreal(0),qreal(1000));
    linearGradient.setSpread(QGradient::PadSpread);
    linearGradient.setColorAt(0, QColor(240,255,240,255));
    linearGradient.setColorAt(1, QColor(151,213,98,255));
    palette->setBrush(QPalette::Window,*(new QBrush(linearGradient)));
    windows->setPalette(*palette);


    windows->show();
    this->show();
}

void MainWindow::on_button_io_clicked()
{
    ArgentIO *windows  = new ArgentIO(this);
    this->hide();

    QPalette* palette = new QPalette();
    QLinearGradient linearGradient(qreal(0),qreal(0), qreal(0),qreal(1000));
    linearGradient.setSpread(QGradient::PadSpread);
    linearGradient.setColorAt(0, QColor(240,255,240,255));
    linearGradient.setColorAt(1, QColor(151,213,98,255));
    palette->setBrush(QPalette::Window,*(new QBrush(linearGradient)));
    windows->setPalette(*palette);

    windows->show();
    this->show();
}

void MainWindow::on_button_options_clicked()
{
    WindowPromo *windows  = new WindowPromo(this);
    this->hide();

    QPalette* palette = new QPalette();
    QLinearGradient linearGradient(qreal(0),qreal(0), qreal(0),qreal(1000));
    linearGradient.setSpread(QGradient::PadSpread);
    linearGradient.setColorAt(0, QColor(240,255,240,255));
    linearGradient.setColorAt(1, QColor(151,213,98,255));
    palette->setBrush(QPalette::Window,*(new QBrush(linearGradient)));
    windows->setPalette(*palette);

    windows->show();
    this->show();
}

void MainWindow::on_button_stock_clicked()
{
    UIStock *windows  = new UIStock(this);
    this->hide();

    QPalette* palette = new QPalette();
    QLinearGradient linearGradient(qreal(0),qreal(0), qreal(0),qreal(1000));
    linearGradient.setSpread(QGradient::PadSpread);
    linearGradient.setColorAt(0, QColor(240,255,240,255));
    linearGradient.setColorAt(1, QColor(151,213,98,255));
    palette->setBrush(QPalette::Window,*(new QBrush(linearGradient)));
    windows->setPalette(*palette);

    windows->show();
    this->show();
}

void MainWindow::on_quit_button_clicked()
{
    int ret = QMessageBox::information(this,"Quitter?","VOulez vous quitter? Si vous quittez, toute prograssion non sauvegardé sera perdu.", QMessageBox::Yes | QMessageBox::No);
    if(ret == QMessageBox::Yes)
        qApp->quit();
}

void MainWindow::on_calendar_button_clicked()
{
    PlanningUi *windows  = new PlanningUi(this);

    QPalette* palette = new QPalette();
    QLinearGradient linearGradient(qreal(0),qreal(0), qreal(0),qreal(1000));
    linearGradient.setSpread(QGradient::PadSpread);
    linearGradient.setColorAt(0, QColor(240,255,240,255));
    linearGradient.setColorAt(1, QColor(151,213,98,255));
    palette->setBrush(QPalette::Window,*(new QBrush(linearGradient)));
    windows->setPalette(*palette);

    windows->show();
    this->show();
}
