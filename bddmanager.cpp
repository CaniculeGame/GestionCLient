#include "bddmanager.h"

BddManager::BddManager()
{

}

BddManager::~BddManager()
{

}

BddManager BddManager::m_instance=BddManager();

BddManager& BddManager::Instance()
{
    return m_instance;
}

void BddManager::Connection(QWidget *parent = 0)
{
    QString pseudo;
    QString mdp;
    bool login_Ok =false;
    bool ok = false;

    while(!login_Ok)
    {
        pseudo =QInputDialog::getText(parent, "Pseudo", "Quel est votre pseudo ?", QLineEdit::Normal, QString(), &ok);
        mdp = QInputDialog::getText(parent, "Pseudo", "Quel est votre mot de passe ?", QLineEdit::Normal, QString(), &ok);

        bdd = QSqlDatabase::addDatabase("QMYSQL");
        bdd.setHostName("localhost");
        bdd.setUserName(pseudo);
        bdd.setPassword(mdp);
        bdd.setDatabaseName("gestionclients");
        if(bdd.open())
        {
            QMessageBox::information(parent,"Connection1","Connection etabli");
            login_Ok = true;
        }
        else
        {
            QMessageBox::information(parent,"Connection1","Connection echoué");
            login_Ok = false;
        }
    }
}
