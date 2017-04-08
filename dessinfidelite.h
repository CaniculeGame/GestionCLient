#ifndef DESSINFIDELITE_H
#define DESSINFIDELITE_H

#include <QWidget>

class DessinFidelite :
{
    Q_OBJECT

    public:
        DessinFidelite();
        ~DessinFidelite();
         void paintEvent(QPaintEvent *event);
};

#endif // DESSINFIDELITE_H
