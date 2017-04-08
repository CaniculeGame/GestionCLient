#ifndef ARGENTIO_H
#define ARGENTIO_H

#include <QMainWindow>
#include <QStandardItemModel>

namespace Ui {class ArgentIO;}

class ArgentIO : public QMainWindow
{
    Q_OBJECT

    public:
        explicit ArgentIO(QWidget *parent = 0);
        ~ArgentIO();

private slots:
    void on_button_ajout_clicked();

    void on_button_retour_clicked();

private:
        void maj_Tableaux();

    private:
        Ui::ArgentIO *ui;
        QStandardItemModel *model_entree;
        QStandardItemModel *model_sortie;

    };

#endif // ARGENTIO_H
