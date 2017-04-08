#ifndef BDDMANAGER_H
#define BDDMANAGER_H

#include <QtSql>
#include <QString>
#include <QInputDialog>
#include <QLineEdit>
#include <QMessageBox>


class BddManager
{

   public:
       static BddManager& Instance();
       void Connection();


   private:
     static BddManager m_instance;

     QSqlDatabase bdd;

     BddManager();
     ~BddManager();

};

#endif // BDDMANAGER_H
