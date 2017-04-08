#include "planningui.h"
#include "ui_planningui.h"

#include <QUrl>
#include <QWebFrame>
#include <QSettings>
#include <iostream>
#include <string>
#include <QtWebKitWidgets>
#include <QNetworkCookieJar>
#include <QNetworkAccessManager>

using namespace std;

PlanningUi::PlanningUi(QWidget *parent) : QMainWindow(parent),ui(new Ui::PlanningUi)
{
    ui->setupUi(this);

    ui->webView->load(QUrl("https://www.google.com/calendar/render#main_7"));
    QObject::connect(ui->webView,SIGNAL(loadFinished(bool)),this,SLOT(finish(bool)));

}


PlanningUi::~PlanningUi()
{
   /* QWebFrame *frame =ui->webView->page()->mainFrame();
    QVariant var_usr = frame->evaluateJavaScript("document.getElementById(\"Email\").value;");
    QVariant var_mdp = frame->evaluateJavaScript("document.getElementById(\"Passwd\").value;");


    QSettings *settings = new QSettings("FRaffalli", "GestionClient_pgm");
    settings->setValue("User_calendar",var_usr.toString());


    string mot = var_mdp.toString().toStdString();
    QByteArray val(mot.data());
    QString m = val.toBase64();

    //cout<<<<endl;
    cout<<mot<<endl;
    cout<<m.toStdString()<<endl;

    settings->setValue("Pass_calendar",m);*/


    delete ui;
}

void PlanningUi::finish(bool estFini)
{
    if(estFini == true)
    {

        QSettings *settings = new QSettings("FRaffalli", "GestionClient_pgm");
        QString user=settings->value("User_calendar","email utilisateur").toString();
        QString pass=settings->value("Pass_calendar","Mot de passe").toString();

        //lire
        /*string pass = m.toStdString();
        QByteArray text = QByteArray::fromBase64(mot.data());*/

        //ecrire
        /*QByteArray val(mot.data());
        QString m = val.toBase64();*/


        QWebFrame *frame =ui->webView->page()->mainFrame();
        frame->evaluateJavaScript("document.getElementById(\"Passwd\").value=\""+pass+"\";");
        frame->evaluateJavaScript("document.getElementById(\"Email\").value=\""+user+"\";");
        frame->evaluateJavaScript("document.getElementById(\"signIn\").setAttribute(\"onclick\",\"sub()\");");


    }
}

