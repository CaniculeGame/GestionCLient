#ifndef GRAPHFIDELITE_H
#define GRAPHFIDELITE_H

#include <QWidget>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QSize>
#include <QFont>
#include <QPen>
#include <QLayout>

class GraphFidelite
{
    public:
        GraphFidelite(QLayout *widget);
        ~GraphFidelite();

        bool Draw(int nb); //nb = nombre de croix à faire

        void text(const char *txt);
        void font(QFont f);
        void feutre(QPen f);
        int get_nb();
        void set_nb(int nb);
        void set_size(QSize size);
        void row(int r);
        void column(int c);
        int row();
        int column();


    private:
        QGraphicsView *view;
        QGraphicsScene *scene;
        QPen feutre_;
        QFont font_;
        const char *text_;
        int nb_case; //nombre de case à remplir
        int nb_visite; //nombre de fois venu
        int width;
        int height;
        int column_;
        int row_;

};

#endif // GRAPHFIDELITE_H
