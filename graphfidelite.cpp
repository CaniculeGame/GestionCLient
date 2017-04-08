#include "graphfidelite.h"

#include <QLayout>
#include <QGraphicsTextItem>

GraphFidelite::GraphFidelite(QLayout *widget)
{
    scene = new QGraphicsScene();
    view = new QGraphicsView();
    view->setScene(scene);
    view->setInteractive(false);
    view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff );
    view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff );
    view->setAlignment(Qt::AlignCenter);
    widget->addWidget(view);
}

GraphFidelite::~GraphFidelite()
{
    delete scene;
    delete view;
}


bool GraphFidelite::Draw(int nb)
{
    //taille des carrés
    int carre_height = height/row_;
    int carre_width  = width / column_;
    int nb_croix = 0;

    scene->clear();

    nb_visite =nb;
    for(int i=0;i<row_;i++)
    {
        for(int j=0;j<column_;j++)
        {
            scene->addRect(QRectF(j*carre_width,i*carre_height,carre_width,carre_height),feutre_);

            if(nb_croix<nb_visite%nb_case)
            {
                QGraphicsTextItem * io = new QGraphicsTextItem;
                io->setPos(j*carre_width,i*carre_height);
                io->setFont(font_);
                io->setPlainText(text_);
                scene->addItem(io);
                nb_croix++;
            }
        }
    }

    if(nb_visite%nb_case == 0 && nb_visite > 0)
        return true;
    else
        return false;

}

void GraphFidelite::text(const char *txt){text_=txt;}
void GraphFidelite::font(QFont f){font_ = f;}
void GraphFidelite::feutre(QPen f){feutre_=f;}
void GraphFidelite::set_size(QSize size)
{
    width  = size.width();
    height = size.height();
}

void GraphFidelite::set_nb(int nb)
{
    nb_case=nb;
}
int GraphFidelite::get_nb(){return nb_case;}
void GraphFidelite::row(int r){row_ = r;}
void GraphFidelite::column(int c){column_ = c;}
int GraphFidelite::row(){return row_;}
int GraphFidelite::column(){return column_;}
