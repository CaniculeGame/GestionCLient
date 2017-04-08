#include "fidelitedessin.h"
#include <QPainter>

FideliteDessin::FideliteDessin(QWidget *parent) : QWidget(parent)
{

}

FideliteDessin::~FideliteDessin()
{

}


void FideliteDessin::paintEvent(QPaintEvent *event)
{
   QPainter painter;

   painter.begin(this);
   painter.setPen(Qt::blue);
   painter.setFont(QFont("Arial", 80));
   painter.drawText(80, 250, "Qt");
   painter.end();

}
