#include "rechercheWindow.h"
#include "ui_recherchewindow.h"

rechercheWindow::rechercheWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::rechercheWindow)
{
    ui->setupUi(this);
}

rechercheWindow::~rechercheWindow()
{
    delete ui;
}
