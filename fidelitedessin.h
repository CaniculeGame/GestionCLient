#ifndef FIDELITEDESSIN_H
#define FIDELITEDESSIN_H

#include <QWidget>

class FideliteDessin : public QWidget
{
    Q_OBJECT
    public:
        explicit FideliteDessin(QWidget *parent = 0);
        ~FideliteDessin();
        void paintEvent(QPaintEvent *event);

    signals:

    public slots:

};

#endif // FIDELITEDESSIN_H
