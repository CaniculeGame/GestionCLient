#include "mdpdialog.h"
#include "ui_mdpdialog.h"


MdpDialog::MdpDialog(QWidget *parent) :QDialog(parent),ui(new Ui::MdpDialog)
{
   ui->setupUi(this);

   set = new QSettings("FRaffalli", "GestionClient_pgm");

   ui->lineEdit_mdp->setEchoMode(QLineEdit::Password);

   ui->lineEdit_bddp->setText(set->value("baseMere","QMYSQL").toString());
   ui->lineEdit_hote->setText(set->value("Hote","localhost").toString());
   ui->lineEdit_nombdd->setText(set->value("base","gestionclients").toString());
   ui->lineEdit_user->setText(set->value("user","root").toString());

   QString m = set->value("mdp","").toString();
   string mot = m.toStdString();
   QByteArray text = QByteArray::fromBase64(mot.data());
   ui->lineEdit_mdp->setText(text);

}

MdpDialog::~MdpDialog()
{
    delete ui;
}

void MdpDialog::on_pushButton_connection_clicked()
{

    if(ui->checkBox_save->isChecked())
    {
        set->setValue("user",ui->lineEdit_user->text());
        set->setValue("baseMere",ui->lineEdit_bddp->text());
        set->setValue("Hote",ui->lineEdit_hote->text());
        set->setValue("base",ui->lineEdit_nombdd->text());

        string mot = ui->lineEdit_mdp->text().toStdString();
        QByteArray val(mot.data());
        QString m = val.toBase64();
        set->setValue("mdp",m);
    }

    QString user = ui->lineEdit_user->text();
    QString mdp = ui->lineEdit_mdp->text();
    QString bd = ui->lineEdit_bddp->text(); //bd parent
    QString hote = ui->lineEdit_hote->text();
    QString bdd = ui->lineEdit_nombdd->text(); //bdd fille

    db = QSqlDatabase::addDatabase(bd);
    db.setHostName(hote);
    db.setUserName(user);
    db.setPassword(mdp);
    db.setDatabaseName(bdd);
    if(db.open())
    {

        QMessageBox::information(this,"Connection","Connection etabli");
        close();
    }
    else
    {
        QSqlError error =db.lastError();
        QString chaine_error = "Connection echoué\n"+error.text();
        QMessageBox::information(this,"Connection",chaine_error);
    }
}

void MdpDialog::on_pushButton_reinit_clicked()
{
   ui->lineEdit_user->setText("root");
   ui->lineEdit_mdp->setText("");
   ui->lineEdit_bddp->setText("QMYSQL");
   ui->lineEdit_hote->setText("localhost");
   ui->lineEdit_nombdd->setText("gestionclients");

   set->setValue("user",ui->lineEdit_user->text());
   set->setValue("baseMere",ui->lineEdit_bddp->text());
   set->setValue("Hote",ui->lineEdit_hote->text());
   set->setValue("base",ui->lineEdit_nombdd->text());
}
