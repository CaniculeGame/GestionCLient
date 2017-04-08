#include "gestion.h"
#include "ui_gestion.h"
#include "qcustomplot.h"
#include <iostream>
#include <QString>

using namespace std;

/*
 * 0 ca
 * 1 freq
 * 2 benef
 * 3 depense
 */



Gestion::Gestion(QWidget *parent) : QMainWindow(parent), ui(new Ui::Gestion)
{

    ui->setupUi(this);

    selection_stat  = 0;
    model_ca        = NULL;
    model_freq      = NULL;
    model_ca_var    = NULL;
    model_freq_var  = NULL;
    model_depense   = NULL;
    model_benefice  = NULL;

    button_stat_Changed(0);

    size=0;
    init_annee();
    chiffre_affaire();
    frequentation();
    chiffre_affaire_variation();
    frequentation_variation();
    depense();
    benefice();

    ui->vue_grille->setAlternatingRowColors(true);
    ui->vue_grille->horizontalHeader()->setStretchLastSection(true);

}

Gestion::~Gestion()
{
    delete ui;
}



void Gestion::init_annee()
{
    //recupere nombre d'année présente dans la bdd
    QString query = "SELECT COUNT(DISTINCT (YEAR( date))) FROM prestations;";
    requete.exec(query);
    requete.next();
    int taille = requete.value(0).toInt();
    size=taille;
    //int annee_min = date.year() - size;

    //recupere les années
    query = "select distinct YEAR(date)  FROM prestations;";
    requete.exec(query);
    if(requete.exec(query))
    {
        while(requete.next())
        {
            numero_annees_.push_back(requete.value(0).toInt());
        }
    }

    ui->button_retour->setIcon(QIcon("image/Cancel.png"));
    ui->button_print->setIcon(QIcon("image/print.png"));
}

void Gestion::frequentation()
{
    if(size==0)
        return;

    if(model_freq==NULL)
    {
        //recupere nombre d'année présente dans la bdd
        QString query = "SELECT COUNT(DISTINCT (YEAR( date))) FROM prestations;";
        requete.exec(query);
        requete.next();
        int size = requete.value(0).toInt();
        //int annee_min = date.year() - size;

        //recupere les années
        query = "select distinct YEAR(date)  FROM prestations;";
        requete.exec(query);
        if(requete.exec(query))
        {
            while(requete.next())
            {
                numero_annees_.push_back(requete.value(0).toInt());
            }
        }


        //creation du header avec les années
        model_freq = new QStandardItemModel(0,size);
        for(int i=0;i<size;i++)
        {
            model_freq->setHeaderData(i, Qt::Horizontal, numero_annees_[i]);
        }

        //enregistrement premiere colone
        int nb_occurence =52;
        QStringList labels;
        labels <<"Année "<<"Janvier "<<"Février "<<"Mars "<<"Avril "<<"Mai "<<"Juin "<<"Juillet "<<"Aout "<<
                 "Septembre "<<"Octobre "<<"Novembre "<<"Décembre ";
        for(int j=13; j<nb_occurence+13 ; j++)
        {
           labels << QString::number(j-12);
        }
        model_freq->setVerticalHeaderLabels(labels);

        //remplissage du tableau
        int taille_tableau = nb_occurence+1+12;
        int valeur[65];
        for(int i=0 ; i<size;i++) //pour chaque année faire
        {
             //tableau ordonnée et efemère, ayant le c.a. de chacune des 60 occurence ( année, mois, semaine)
            for(int k=0;k<taille_tableau;k++)
                valeur[k] = 0;

            query = "SELECT * FROM prestations WHERE YEAR(date) = "+QString::number(numero_annees_[i])+";";
            requete.exec(query);
            if(requete.exec(query))
            {
                while(requete.next())//pour chaque ligne faire:
                {
                    //année 0
                    valeur[0]++;

                    //mois 1-12
                    QDate date = requete.value(2).toDate();
                    int mois = date.month();
                    valeur[mois]++;

                    //semaine 13-64
                    int semaine = date.weekNumber() + 12;
                    valeur[semaine]++;
                 }
            }

            //remplissage du tableview avec des items
            for(int x =0; x <taille_tableau;x++)
                model_freq->setItem(x,i, new QStandardItem(QString::number(valeur[x])));
        }
    }
    else
    {
        if(model_freq!=NULL && ui->grille_statistique != NULL)
            ui->vue_grille->setModel(model_freq);
    }
}


void Gestion::chiffre_affaire()
{
    if(size==0)
        return;

    if(model_ca==NULL)
    {
        //creation du header avec les années
        model_ca = new QStandardItemModel(0,size);
        for(int i=0;i<size;i++)
        {
            model_ca->setHeaderData(i, Qt::Horizontal, numero_annees_[i]);
        }

        //enregistrement premiere colone
        int nb_occurence =52;
        QStringList labels ;
        labels <<"Année "<<"Janvier "<<"Février "<<"Mars "<<"Avril "<<"Mai "<<"Juin "<<"Juillet "<<"Aout "<<
                 "Septembre "<<"Octobre "<<"Novembre "<<"Décembre ";
        for(int j=13; j<nb_occurence+13 ; j++)
        {
           labels << QString::number(j-12);
        }

        model_ca->setVerticalHeaderLabels(labels);

        //remplissage du tableau
        int taille_tableau = nb_occurence+1+12;
        int valeur[65];
        for(int i=0 ; i<size;i++) //pour chaque année faire
        {
             //tableau ordonnée et efemère, ayant le c.a. de chacune des 60 occurence ( année, mois, semaine)
            for(int k=0;k<taille_tableau;k++)
                valeur[k] = 0;

            QString query = "SELECT * FROM prestations WHERE YEAR(date) = "+QString::number(numero_annees_[i])+";";
            requete.exec(query);
            if(requete.exec(query))
            {
                while(requete.next())//pour chaque ligne faire:
                {

                     double prix = requete.value(1).toDouble();
                     QDate date = requete.value(2).toDate();

                    //année 0
                    valeur[0]+=prix;

                    //mois 1-12
                    int mois = date.month();
                    valeur[mois]+=prix;

                    //semaine 13-64
                    int semaine = date.weekNumber() + 12;
                    valeur[semaine]+=prix;
                }
            }

            //recuperation des entrées ponctuelles et ajout aux tableaux
            query = "select * from  entrees_sorties where Prix > 0 and YEAR(DateDebut)<="+QString::number(numero_annees_[i])+";";
            requete.exec(query);
            if(requete.exec(query))
            {
                while(requete.next())//pour chaque ligne faire:
                {
                    int rep = requete.value(4).toInt();
                    int type = requete.value(3).toInt();
                    double prix = requete.value(2).toDouble();
                    QDate dte = requete.value(5).toDate();
                    int iter=0;
                    int mois = 0;
                    int semaine =13;

                    switch(type)
                    {
                        case -1: //ponctuel

                            valeur[0]+=prix;
                            mois = dte.month();
                            valeur[mois]+=prix;
                            semaine = dte.weekNumber() + 12;
                            valeur[semaine]+=prix;

                        break;

                        case 0: //année

                            //si même année alors on ajoute
                            if(dte.year() == numero_annees_[i])
                            {
                                valeur[0]+=prix;
                                mois = dte.month();
                                valeur[mois]+=prix;
                                semaine = dte.weekNumber() + 12;
                                valeur[semaine]+=prix;
                            }
                            else
                            {   //sinon on vérifie si la répétition est encore appliquable
                                dte.addYears(rep);
                                if(dte.year()<=numero_annees_[i])
                                {
                                    valeur[0]+=prix;
                                    mois = dte.month();
                                    valeur[mois]+=prix;
                                    semaine = dte.weekNumber() + 12;
                                    valeur[semaine]+=prix;
                                }
                            }

                        break;

                        case 1: //mois

                            QDate date_fin = dte.addMonths(rep);
                            iter =0;
                            if(numero_annees_[i] == dte.year()) //si date dans la même année faire
                            {
                               //nombre de mois entre le debut et la date actuelle
                               for(int m = dte.month(); m <= 12; m++)//pour chaque mois à partir du mois de départ faire
                               {
                                   if(m<dte.month()+rep)
                                   {
                                       valeur[1+m]+=prix;
                                       valeur[dte.addMonths(m-1).weekNumber()+13]+=prix;
                                       iter++;
                                   }
                               }


                               if(iter == 1)
                                   iter++;

                              valeur[0]+= ((iter-1) * prix);
                            }
                            else if(date_fin.year() >= numero_annees_[i])//si valide dans l'année courante faire
                            {
                                QDate date_courante = QDate(numero_annees_[i],1,1);
                                for(int m = 1; m <= 12; m++)//pour chaque mois faire
                                {
                                    if(date_courante<=date_fin)
                                    {
                                        valeur[1+m]+=prix;
                                        iter++;
                                        valeur[13+date_courante.weekNumber()]+=prix;
                                        date_courante.addMonths(1); //ajout de 1 mois à chaque fois

                                    }
                                }
                                 valeur[0]+= ((iter-1) * prix);
                            }


                        break;

                        /*case 2: //semaine
                        break;*/
                    }
                }
            }


            //remplissage du tableview avec des items
            for(int x =0; x <taille_tableau;x++)
                model_ca->setItem(x,i, new QStandardItem(QString::number(valeur[x])));
        }

        ui->vue_grille->setModel(model_ca);
    }
    else
    {
        if(model_ca!=NULL && ui->grille_statistique != NULL)
            ui->vue_grille->setModel(model_ca);
    }
}

void Gestion::chiffre_affaire_variation()
{
    if(size==0)
        return;

    if(model_ca_var==NULL)
    {
        //creation du header avec les années
        model_ca_var = new QStandardItemModel(0,size);
        for(int i=0;i<size;i++)
        {
            model_ca_var->setHeaderData(i, Qt::Horizontal, numero_annees_[i]);
        }

        //enregistrement premiere colone
        int nb_occurence =52;
        QStringList labels ;
        labels <<"Année "<<"%"<<"Janvier "<<"%"<<"Février "<<"%"<<"Mars "<<"%"<<"Avril "<<"%"<<"Mai "<<"%"<<"Juin "<<"%"<<"Juillet "<<"%"<<"Aout "<<"%"<<
                 "Septembre "<<"%"<<"Octobre "<<"%"<<"Novembre "<<"%"<<"Décembre "<<"%";
        for(int j=25; j<(nb_occurence+25) ; j++)
        {
           labels << QString::number(j-12*2);
           labels <<"%";
        }

        model_ca_var->setVerticalHeaderLabels(labels);

        //remplissage du tableau
        int taille_tableau = (nb_occurence+1+12)*2;
        double valeur[65*2];
        for(int i=0 ; i<size;i++) //pour chaque année/colone faire
        {
             //tableau ordonnée et efemère, ayant le c.a. de chacune des 60 occurence ( année, mois, semaine)
            for(int k=0;k<taille_tableau;k++)
                valeur[k] = 0;

            QString query = "SELECT * FROM prestations WHERE YEAR(date) = "+QString::number(numero_annees_[i])+";";
            requete.exec(query);
            if(requete.exec(query))
            {
                while(requete.next())//pour chaque ligne faire:
                {
                    //remplir le tableau de valeur

                    //année 0
                    valeur[0]+=requete.value(1).toInt();

                    //mois 2-24
                    QDate date = requete.value(2).toDate();
                    double mois = date.month();
                    int indice = mois*2;
                    valeur[indice]+=requete.value(1).toDouble();

                    //semaine 24-128
                    double semaine = date.weekNumber() + 12;
                    indice = semaine*2;
                    valeur[indice]+=requete.value(1).toDouble();
                 }
            }

            //remplissage du tableview avec des items
            for(int x =0; x <taille_tableau;x++)
                model_ca_var->setItem(x,i, new QStandardItem(QString::number(valeur[x])));
        }

        //remplir avec des pourcentages
        for(int i=0 ; i<size-1;i++) //pour chaque colone faire
        {
            for(int j=1 ;j<taille_tableau;j+=2) //pour chaque ligne faire
            {
                QVariant item   = model_ca_var->item(j-1,i)->data(Qt::DisplayRole);
                QVariant item2  = model_ca_var->item(j-1,i+1)->data(Qt::DisplayRole);
                double temp     = item.toDouble();
                double temp2    = item2.toDouble();

                if(temp2==0)
                    continue;

                valeur[j]= ((temp/temp2 )-1) * 100;
                model_ca_var->item(j,i)->setData(QVariant(valeur[j]),Qt::EditRole);
            }
        }
    }
    else
    {
        if(model_ca_var!=NULL && ui->grille_statistique != NULL)
            ui->vue_grille->setModel(model_ca_var);
    }
}

void Gestion::frequentation_variation()
{
    if(size==0)
        return;

    if(model_freq_var==NULL)
    {
        //creation du header avec les années
        model_freq_var = new QStandardItemModel(0,size);
        for(int i=0;i<size;i++)
        {
            model_freq_var->setHeaderData(i, Qt::Horizontal, numero_annees_[i]);
        }

        //enregistrement premiere colone
        int nb_occurence =52;
        QStringList labels ;
        labels <<"Année "<<"%"<<"Janvier "<<"%"<<"Février "<<"%"<<"Mars "<<"%"<<"Avril "<<"%"<<"Mai "<<"%"<<"Juin "<<"%"<<"Juillet "<<"%"<<"Aout "<<"%"<<
                 "Septembre "<<"%"<<"Octobre "<<"%"<<"Novembre "<<"%"<<"Décembre "<<"%";
        for(int j=25; j<(nb_occurence+25) ; j++)
        {
           labels << QString::number(j-12*2);
           labels <<"%";
        }

        model_freq_var->setVerticalHeaderLabels(labels);

        //remplissage du tableau
        int taille_tableau = (nb_occurence+1+12)*2;
        double valeur[65*2];
        for(int i=0 ; i<size;i++) //pour chaque année/colone faire
        {
             //tableau ordonnée et efemère, ayant le c.a. de chacune des 60 occurence ( année, mois, semaine)
            for(int k=0;k<taille_tableau;k++)
                valeur[k] = 0;

            QString query = "SELECT * FROM prestations WHERE YEAR(date) = "+QString::number(numero_annees_[i])+";";
            requete.exec(query);
            if(requete.exec(query))
            {
                while(requete.next())//pour chaque ligne faire:
                {
                    //remplir le tableau de valeur

                    //année 0
                    valeur[0]++;

                    //mois 2-24
                    QDate date = requete.value(2).toDate();
                    double mois = date.month();
                    int indice = mois*2;
                    valeur[indice]++;

                    //semaine 24-128
                    double semaine = date.weekNumber() + 12;
                    indice = semaine*2;
                    valeur[indice]++;
                 }
            }

            //remplissage du tableview avec des items
            for(int x =0; x <taille_tableau;x++)
                model_freq_var->setItem(x,i, new QStandardItem(QString::number(valeur[x])));
        }

        //remplir avec des pourcentages
        for(int i=0 ; i<size-1;i++) //pour chaque colone faire
        {
            for(int j=1 ;j<taille_tableau;j+=2) //pour chaque ligne faire
            {
                QVariant item   = model_freq_var->item(j-1,i)->data(Qt::DisplayRole);
                QVariant item2  = model_freq_var->item(j-1,i+1)->data(Qt::DisplayRole);
                double temp     = item.toDouble();
                double temp2    = item2.toDouble();

                if(temp2==0)
                    continue;

                valeur[j]= ((temp/temp2 )-1) * 100;
                model_freq_var->item(j,i)->setData(QVariant(valeur[j]),Qt::EditRole);
            }
        }
    }
    else
    {
        if(model_freq_var!=NULL && ui->grille_statistique != NULL)
            ui->vue_grille->setModel(model_freq_var);
    }
}



void Gestion::benefice()
{
    if(size==NULL)
        return;

    if(model_benefice==NULL)
    {

        if(model_ca == NULL || model_depense == NULL)
            return;

        //creation du header avec les années
        model_benefice = new QStandardItemModel(0,size);
        for(int i=0;i<size;i++)
        {
            model_benefice->setHeaderData(i, Qt::Horizontal, numero_annees_[i]);
        }

        //enregistrement premiere colone
        int nb_occurence =52;
        QStringList labels;
        labels <<"Année "<<"Janvier "<<"Février "<<"Mars "<<"Avril "<<"Mai "<<"Juin "<<"Juillet "<<"Aout "<<
                 "Septembre "<<"Octobre "<<"Novembre "<<"Décembre ";
        for(int j=13; j<nb_occurence+13 ; j++)
        {
           labels << QString::number(j-12);
        }
        model_benefice->setVerticalHeaderLabels(labels);

        //pour chaque année faire
        for(int x=0;x<size;x++)
        {
            //pour chaque ligne faire
            int i=0;
            for(int y=0;y<model_ca->rowCount();y++)
            {
                QStandardItem *item_ca  = model_ca->item(y,x);
                QStandardItem *item_dep = model_depense->item(y,x);

                QVariant it_ca;
                QVariant it_dep;
                if(item_ca==NULL)
                    continue;
                if(item_dep==NULL)
                    continue;

                it_ca =item_ca->data(Qt::DisplayRole);
                it_dep = item_dep->data(Qt::DisplayRole);


                double valeur =  it_ca.toDouble() + it_dep.toDouble();
                if(y==0) //année
                    model_benefice->setItem(y,x, new QStandardItem(QString::number(valeur)));
                else if(y>=1 && y<13)
                    model_benefice->setItem(y,x, new QStandardItem(QString::number(valeur)));
                else
                    model_benefice->setItem(y,x, new QStandardItem(QString::number(valeur)));
            }
        }
    }
    else
    {
        if(model_benefice!=NULL && ui->grille_statistique != NULL)
            ui->vue_grille->setModel(model_benefice);
    }

}


//manque les dépenses soins
void Gestion::depense()
{
    QSqlQuery  req;
    QString query;

    if(size==0)
        return;

    if(model_depense==NULL)
    {

        //creation du header avec les années
        model_depense = new QStandardItemModel(0,size);
        for(int i=0;i<size;i++)
        {
            model_depense->setHeaderData(i, Qt::Horizontal, numero_annees_[i]);
        }

        //enregistrement premiere colone
        int nb_occurence =52;
        QStringList labels;
        labels <<"Année "<<"Janvier "<<"Février "<<"Mars "<<"Avril "<<"Mai "<<"Juin "<<"Juillet "<<"Aout "<<
                 "Septembre "<<"Octobre "<<"Novembre "<<"Décembre ";
        for(int j=13; j<nb_occurence+13 ; j++)
        {
           labels << QString::number(j-12);
        }
        model_depense->setVerticalHeaderLabels(labels);




        vector<double> annees; // champ année = depense annuel -> nb année non connu

        //si boutton non coché alors on affiche les depense théoriques sinon on affiche les dépenses réel en regardant uniquement le tableau entrée/sortie
        if(!ui->reel_button->isChecked())
        {
            query = "select DISTINCT year(datedebut) FROM gestionclients.entrees_sorties where Prix < 0 and type='Autre' group by DateDebut order by DateDebut desc;";
        }
        else //réel
        {
            query = "select DISTINCT year(datedebut) FROM gestionclients.entrees_sorties where Prix < 0  group by DateDebut order by DateDebut desc;";
        }

        if(req.exec(query))
        {
            int taille =  req.size();
            for(int i=0;i<taille;i++)//pour chaque année/colonne faire
            {

                double mois[12]={0}; //tableau de 12 cases , 1/ mois
                double semaine[53]={0}; //tableau de 52 cases 1/ semaine
                double annee=0;

               //ajout de l'année courante

                req.next();

                QString query2;
                QSqlQuery  req2;

                int annee_courante = req.value(0).toInt();

                if(!ui->reel_button->isChecked())
                {
                     query2 = "SELECT * FROM gestionclients.entrees_sorties where Prix < 0 and year(DateDebut) <="+QString::number(annee_courante)+" and type='Autre';";
                }
                else //réel
                {
                    query2 = "SELECT * FROM gestionclients.entrees_sorties where Prix < 0 and year(DateDebut) <="+QString::number(annee_courante)+";";
                }

                if(req2.exec(query2))
                {
                    while(req2.next())
                    {
                        double cout = req2.value(2).toDouble();
                        int type_repetition = req2.value(3).toInt();
                        int nb_repetition = req2.value(4).toInt();
                        QDate  date =  req2.value(5).toDate();
                        QDate date_fin;
                        int iter =0;

                        switch(type_repetition)
                        {
                            case -1: //ponctuelle
                                if(date.year() == annee_courante)
                                {
                                    annee+=cout;
                                    mois[date.month()]+=cout;
                                    semaine[date.weekNumber()]+=cout;

                                }
                            break;

                            case 0: //année
                                if( nb_repetition+date.year() >= annee_courante ) //si la dépense est encore appliqualble faire
                                {
                                    //la dépense annuele se fait a un certain moi et une certaine semaine, on l'ajoute
                                   annee+=cout;
                                   mois[date.month()]+=cout;
                                   semaine[date.weekNumber()]+=cout;

                                }
                            break;

                            case 1://mois

                                date_fin = date.addMonths(nb_repetition);
                                iter =0;
                                if(annee_courante == date.year()) //si date dans la même année faire
                                {
                                   //nombre de mois entre le debut et la date actuelle
                                   //int nb_mois = abs(date.month() - QDate::currentDate().month());
                                   for(int m = date.month(); m <= 12; m++)//pour chaque mois à partir du mois de départ faire
                                   {
                                       if(m<date.month()+nb_repetition)
                                       {
                                           mois[m]+=cout;
                                           semaine[date.addMonths(m-1).weekNumber()]+=cout;
                                           iter++;
                                       }
                                   }


                                   if(iter == 1)
                                       iter++;
                                   annee+= ((iter-1) * cout);
                                }
                                else if(date_fin.year() >= annee_courante)//si valide dans l'année courante faire
                                {
                                    QDate date_courante = QDate(annee_courante,1,1);
                                    for(int m = 1; m <= 12; m++)//pour chaque mois faire
                                    {
                                        if(date_courante<=date_fin)
                                        {
                                            mois[m]+=cout;
                                            iter++;
                                            semaine[date_courante.weekNumber()]+=cout;
                                            date_courante.addMonths(1); //ajout de 1 mois à chaque fois

                                        }
                                    }
                                    annee+= ((iter-1) * cout);
                                }

                            break;

                            case 2://semaine
                                date_fin = date.addDays(7*nb_repetition);//1 semaine = 7 jours
                                iter =0;
                                if(annee_courante == date.year()) //si date dans la même année faire
                                {
                                   //nombre de mois entre le debut et la date actuelle
                                   for(int s = date.weekNumber(); s <= 52; s++)//pour chaque semaine à partir du mois de départ faire
                                   {
                                       if(s<=date.weekNumber()+nb_repetition)
                                       {
                                           semaine[s]+=cout;
                                           iter++;
                                           mois[s/4]+=cout;
                                       }
                                   }
                                  annee+= ((iter-1) * cout);
                                }
                                else if(date_fin.year() >= annee_courante)//si valide dans l'année courante faire
                                {
                                    QDate date_courante = QDate(annee_courante,1,1);
                                    for(int s = 0; s <= 52; s++)//pour chaque semaine faire
                                    {
                                        if(date_courante<=date_fin)
                                        {
                                            semaine[s]+=cout;
                                            iter++;
                                            date_courante.addDays(7); //ajout de 1 semaine à chaque fois
                                            mois[s/4]+=cout;
                                        }
                                    }
                                    annee+= ((iter-1) * cout);
                                }

                            break;

                        }
                    }
                }

                annees.push_back(annee);



                //recuperation des dépenses liés aux soins


                //si boutton non coché alors on affiche les depense théoriques sinon on affiche les dépenses réel en regardant uniquement le tableau entrée/sortie
                if(!ui->reel_button->isChecked())
                {

                    //initialisation de la liste des produits existants
                    query2 =" select * from stock;";
                    if(req2.exec(query2))
                    {
                        while(req2.next())
                        {
                            info_produit produit;
                            produit.numero_produit=req2.value(0).toInt();
                            produit.produit_utilise = req2.value(4).toInt();

                            QString     query3;
                            QSqlQuery   req3;

                            query3="select count(*) from soin_concerne_stock where id_produit = "+QString::number(produit.numero_produit)+";";
                            if(req3.exec(query3))
                            {
                                req3.next();
                                produit.nb_client = req3.value(0).toInt();

                                if(produit.produit_utilise==0)
                                    produit.produit_utilise = 1;

                                int res = produit.nb_client / produit.produit_utilise;

                                if(res==0)
                                    res=1;

                                produit.prix = req2.value(2).toDouble()/res;

                                produits.push_back(produit);
                            }
                            else
                                QMessageBox::critical(this,"ERREUR","Erreur requete req3 depense à echoué");

                        }
                    }
                    else
                        QMessageBox::critical(this,"ERREUR","Erreur requete req2 depense() à echoué");


                    //maintenent que les produits sont connus. ainsi que sont cou de revien, il faut l'appliquer
                    //pour chaque produit, regarder combient sont utilisé par année
                    for(int j=0;j<produits.size();j++)
                    {
                        QString     query3;
                        QSqlQuery   req3;


                        query3="select p.date, c.idprest, ss.id_soins, st.id from prestations p, soin_concerne_stock ss, stock st, concerne c "
                               "where Year(date) = "+QString::number(numero_annees_[i])+" "
                               "and st.id = ss.id_produit "
                               "and ss.id_soins = c.idsoins  "
                               "and c.idprest = p.idprestations "
                               "and st.id ="+QString::number(produits[j].numero_produit)+";";

                        if(req3.exec(query3))
                        {
                            while(req3.next())
                            {
                                QDate dte = req3.value(0).toDate();
                                annee-=produits[j].prix;
                                mois[dte.month()]-=produits[j].prix;
                                semaine[dte.weekNumber()]-=produits[j].prix;
                            }
                        }
                        else
                            QMessageBox::critical(this,"ERREUR","Erreur requete req3 depense à echoué");

                    }
                }


                //ajout des donnée à l'ui
                //remplissage du tableview avec des items
                for(int x =0; x <12 + 1 + 52 ;x++)
                {
                    if(x==0) //anneé
                        model_depense->setItem(x,i, new QStandardItem(QString::number(annee)));
                    else if(x>=1 && x<13)
                        model_depense->setItem(x,i, new QStandardItem(QString::number(mois[x])));
                    else
                        model_depense->setItem(x,i, new QStandardItem(QString::number(semaine[x-13])));

                }

            }
        }
    }
    else
    {
        if(model_depense!=NULL && ui->grille_statistique != NULL)
            ui->vue_grille->setModel(model_depense);
    }
}

void Gestion::init_color()
{
    int nb_lignes = 52+1+12;

    if(ui->checkbox_variation->isChecked())
        nb_lignes*=2;

    for(int i=0; i<nb_lignes;i++)
    {
        //pour chaque collone de la ligne faire:
        for(int j =0; j< size;j++)
        {
            switch(selection_stat)
            {
                case 0:
                    if(ui->checkbox_variation->isChecked())
                    {
                        model_ca_var->item(i, j)->setForeground(QBrush(QColor(Qt::black)));
                        model_ca_var->item(i, j)->setForeground(QBrush(QColor(Qt::black)));
                    }
                    else
                    {
                        model_ca->item(i, j)->setForeground(QBrush(QColor(Qt::black)));
                        model_ca->item(i, j)->setForeground(QBrush(QColor(Qt::black)));
                    }
                break;

                case 1:
                    if(ui->checkbox_variation->isChecked())
                    {
                        model_freq_var->item(i, j)->setForeground(QBrush(QColor(Qt::black)));
                        model_freq_var->item(i, j)->setForeground(QBrush(QColor(Qt::black)));
                    }
                    else
                    {
                        model_freq->item(i, j)->setForeground(QBrush(QColor(Qt::black)));
                        model_freq->item(i, j)->setForeground(QBrush(QColor(Qt::black)));
                    }
                break;
            }
        }
     }
}


void Gestion::button_stat_Changed(int numero)
{
    //changement de la couleur du bouton
    switch(numero)
    {
        case 0:
             ui->button_ca->setStyleSheet("QPushButton { background-image:url(image/bg-button_ns.png);background-repeat:repeat-xy;"
                                      "border-color:#739442;border-style: outset;"
                                      "border-width: 2px;border-radius: 2px;font: bold 12px; min-width: 8em;padding: 7px;}}");
        break;

        case 1:

            ui->button_frequentation->setStyleSheet("QPushButton { background-image:url(image/bg-button_ns.png);background-repeat:repeat-xy;"
                                                    "border-color:#739442;border-style: outset;"
                                                    "border-width: 2px;border-radius: 2px;font: bold 12px; min-width: 8em;padding: 7px;}}");

        break;

        case 2:

            ui->button_benefice->setStyleSheet("QPushButton { background-image:url(image/bg-button_ns.png);background-repeat:repeat-xy;"
                                            "border-color:#739442;border-style: outset;"
                                            "border-width: 2px;border-radius: 2px;font: bold 12px; min-width: 8em;padding: 7px;}}");

        break;

        case 3:

            ui->buton_depense->setStyleSheet("QPushButton { background-image:url(image/bg-button_ns.png);background-repeat:repeat-xy;"
                                            "border-color:#739442;border-style: outset;"
                                            "border-width: 2px;border-radius: 2px;font: bold 12px; min-width: 8em;padding: 7px;}}");

        break;
    }

    //changement de la vue
    switch(selection_stat)
    {
        case 0:
             ui->button_ca->setStyleSheet("QPushButton {background-image:url(image/bg-button_s.png); background-repeat:repeat-xy;"
                                        "border-color:#739442; border-style: inset; border-width: 2px; border-radius: 2px;"
                                        "font: bold 12px;min-width: 8em; padding: 7px;}");
            chiffre_affaire();
            ui->titre_label->setText("Chiffre D'affaire  (en € )");

            if(ui->checkbox_couleur->isChecked())
                on_checkbox_couleur_stateChanged(0);
            else if (ui->checkbox_var_couleur->isChecked())
                on_checkbox_var_couleur_stateChanged(0);
            else
                init_color();

            if(ui->checkbox_variation->isChecked())
                on_checkbox_variation_stateChanged(0);


        break;

        case 1:

            ui->button_frequentation->setStyleSheet("QPushButton {background-image:url(image/bg-button_s.png); background-repeat:repeat-xy;"
                                            "border-color:#739442; border-style: inset; border-width: 2px; border-radius: 2px;"
                                            "font: bold 12px;min-width: 8em; padding: 7px;}");

            frequentation();
            ui->titre_label->setText("Fréquentation");
            if(ui->checkbox_couleur->isChecked())
                on_checkbox_couleur_stateChanged(0);
            else if (ui->checkbox_var_couleur->isChecked())
                on_checkbox_var_couleur_stateChanged(0);
            else
                init_color();

            if(ui->checkbox_variation->isChecked())
                on_checkbox_variation_stateChanged(0);



        break;

        case 2:

            benefice();
            ui->button_benefice->setStyleSheet("QPushButton {background-image:url(image/bg-button_s.png); background-repeat:repeat-xy;"
                                            "border-color:#739442; border-style: inset; border-width: 2px; border-radius: 2px;"
                                            "font: bold 12px;min-width: 8em; padding: 7px;}");
            ui->titre_label->setText("Bénéfice");

        break;

        case 3:

            depense();
            ui->buton_depense->setStyleSheet("QPushButton {background-image:url(image/bg-button_s.png); background-repeat:repeat-xy;"
                                            "border-color:#739442; border-style: inset; border-width: 2px; border-radius: 2px;"
                                            "font: bold 12px;min-width: 8em; padding: 7px;}");
            ui->titre_label->setText("Dépense");

        break;

     }
}


void Gestion::on_button_ca_clicked()
{
    int nb = selection_stat;
    selection_stat = 0;
    button_stat_Changed(nb);
}


void Gestion::on_button_frequentation_clicked()
{
    int nb = selection_stat;
    selection_stat = 1;
    button_stat_Changed(nb);
}


void Gestion::on_button_benefice_clicked()
{
    int nb = selection_stat;
    selection_stat = 2;
    button_stat_Changed(nb);
}


void Gestion::on_buton_depense_clicked()
{
    int nb = selection_stat;
    selection_stat = 3;
    button_stat_Changed(nb);
}


void Gestion::on_button_retour_clicked()
{
    this->close();
}






void Gestion::on_checkbox_variation_stateChanged(int arg1)
{
    if(ui->checkbox_variation->isChecked())
    {
        switch(selection_stat)
        {
            case 0:
               ui->vue_grille->setModel(model_ca_var);
            break;

            case 1:
              ui->vue_grille->setModel(model_freq_var);
            break;
        }

    }
    else
    {
        switch(selection_stat)
        {
            case 0:
               ui->vue_grille->setModel(model_ca);
            break;

            case 1:
              ui->vue_grille->setModel(model_freq);
            break;
        }
    }
}


void Gestion::on_checkbox_couleur_stateChanged(int arg1)
{
    int nb_lignes = 52+1+12;
    double max =-1000000;     //chiffre a coloré en vert
    double min =1000000; //chiffre à coloré en rouge
    int pos_i_max =0;
    int pos_j_max =0;
    int pos_i_min =0;
    int pos_j_min =0;
    double var = 0;

    if(ui->checkbox_couleur->isChecked())
    {

        ui->checkbox_var_couleur->setChecked(false);
        on_checkbox_var_couleur_stateChanged(0);

        if(ui->checkbox_variation->isChecked())
            nb_lignes = nb_lignes*2;



        //pour chaque ligne faire
        for(int i=0; i<nb_lignes;i++)
        {

            max =-100000;   //chiffre a coloré en vert
            min =1000000;   //chiffre à coloré en rouge
            pos_i_max =0;
            pos_j_max =0;
            pos_i_min =0;
            pos_j_min =0;

            //pour chaque collone de la ligne faire:
            for(int j =0; j< size;j++)
            {

                QVariant temp;
                switch(selection_stat)
                {
                    case 0:
                        if(ui->checkbox_variation->isChecked())
                            temp = model_ca_var->item(i,j)->data(Qt::DisplayRole);
                        else
                            temp = model_ca->item(i,j)->data(Qt::DisplayRole);


                        var = temp.toDouble();
                        if(var > max)
                        {
                            max = var;
                            pos_i_max = i;
                            pos_j_max =j;
                        }

                        if(var < min)
                        {
                            min = var;
                            pos_i_min = i;
                            pos_j_min =j;
                        }
                    break;

                    case 1:

                        if(ui->checkbox_variation->isChecked())
                            temp = model_freq_var->item(i,j)->data(Qt::DisplayRole);
                        else
                            temp = model_freq->item(i,j)->data(Qt::DisplayRole);

                        var = temp.toDouble();
                        if(var > max)
                        {
                            max = var;
                            pos_i_max = i;
                            pos_j_max =j;
                        }

                        if(var < min)
                        {
                            min = var;
                            pos_i_min = i;
                            pos_j_min =j;
                        }
                    break;

                }
            }

            //application du style
             switch(selection_stat)
             {
                 case 0:
                     if(ui->checkbox_variation->isChecked())
                     {
                         model_ca_var->item(pos_i_max, pos_j_max)->setForeground(QBrush(QColor(Qt::green)));
                         model_ca_var->item(pos_i_min, pos_j_min)->setForeground(QBrush(QColor(Qt::red)));
                     }
                     else
                     {
                         model_ca->item(pos_i_max, pos_j_max)->setForeground(QBrush(QColor(Qt::green)));
                         model_ca->item(pos_i_min, pos_j_min)->setForeground(QBrush(QColor(Qt::red)));
                     }
                 break;

                 case 1:
                     if(ui->checkbox_variation->isChecked())
                     {
                         model_freq_var->item(pos_i_max, pos_j_max)->setForeground(QBrush(QColor(Qt::green)));
                         model_freq_var->item(pos_i_min, pos_j_min)->setForeground(QBrush(QColor(Qt::red)));
                     }
                     else
                     {
                         model_freq->item(pos_i_max, pos_j_max)->setForeground(QBrush(QColor(Qt::green)));
                         model_freq->item(pos_i_min, pos_j_min)->setForeground(QBrush(QColor(Qt::red)));
                     }
                 break;
             }
        }
    }
    else
        init_color();
}


void Gestion::on_checkbox_var_couleur_stateChanged(int arg1)
{
    int nb_lignes = 52+1+12;
    int max =-1000000;     //chiffre a coloré en vert
    int min =1000000; //chiffre à coloré en rouge

    int var=0;
    int variance =0;
    int res =0;
    int r =0;
    int g =0;
    int b = 0;

     QVariant temp;


    if(ui->checkbox_var_couleur->isChecked())
    {
        ui->checkbox_couleur->setChecked(false);
        on_checkbox_couleur_stateChanged(0);

        //recherche du min et du max
        for(int i=0;i<nb_lignes;i++)    //pour chaque ligne
        {
            max =-100000;   //chiffre a coloré en vert
            min =1000000;   //chiffre à coloré en rouge

            for(int j=0;j<size;j++)     //pour chaque colonne
            {
                switch(selection_stat)
                {
                    case 0:
                        temp = model_ca->item(i,j)->data(Qt::DisplayRole);
                        if(temp.toInt() > max)
                        {
                            max = temp.toInt();
                        }
                        else if(temp.toInt() < min)
                        {
                            min = temp.toInt();
                        }

                    break;

                    case 1:
                        temp = model_freq->item(i,j)->data(Qt::DisplayRole);
                        if(temp.toInt() > max)
                        {
                            max = temp.toInt();
                        }
                        else if(temp.toInt() < min)
                        {
                            min = temp.toInt();
                        }
                    break;
                }
            }

            variance = max - min;
            for(int j=0;j<size;j++)     //pour chaque colonne
            {
                //on colorie les colones
                switch(selection_stat)
                {
                    case 0:
                        temp = model_ca->item(i,j)->data(Qt::DisplayRole);
                        var = temp.toInt();//x
                        if(variance ==0)
                            variance = 1;

                        res  = (100 * (var - min)) / variance; // notre pourcentage de vert
                        r = 255 * (100-res) / 100 ;
                        g = 255 * res / 100;
                        model_ca->item(i, j)->setForeground(QBrush(QColor(r,g,b,255)));
                    break;

                    case 1:
                        temp = model_freq->item(i,j)->data(Qt::DisplayRole);
                        var = temp.toInt();//x
                        if(variance ==0)
                            variance = 1;

                        res  = (100 * (var - min)) / variance; // notre pourcentage de vert
                        r = 255 * (100-res) / 100 ;
                        g = 255 * res / 100;
                        model_freq->item(i, j)->setForeground(QBrush(QColor(r,g,b,255)));
                        var =temp.toInt();
                    break;

                }
            }

        }
 }
    else
        init_color();
}

void Gestion::on_button_print_clicked()
{

}


void Gestion::on_reel_button_clicked()
{

}

void Gestion::on_reel_button_stateChanged(int arg1)
{
    //selection_stat  = 0;

    model_ca->clear();
    model_freq->clear();
    model_ca_var->clear();
    model_freq_var->clear();
    model_depense->clear();
    model_benefice->clear();

    model_ca        = NULL;
    model_freq      = NULL;
    model_ca_var    = NULL;
    model_freq_var  = NULL;
    model_depense   = NULL;
    model_benefice  = NULL;

   // button_stat_Changed(0);

    size=0;
    init_annee();
    chiffre_affaire();
    frequentation();
    chiffre_affaire_variation();
    frequentation_variation();
    depense();
    benefice();

}
