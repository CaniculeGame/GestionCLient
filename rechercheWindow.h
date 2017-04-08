#ifndef RECHERCHEWINDOW_H
#define RECHERCHEWINDOW_H

#include <QWidget>

namespace Ui {
class rechercheWindow;
}

class rechercheWindow : public QWidget
{
    Q_OBJECT

public:
    explicit rechercheWindow(QWidget *parent = 0);
    ~rechercheWindow();

private:
    Ui::rechercheWindow *ui;
};

#endif // RECHERCHEWINDOW_H
