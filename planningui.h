#ifndef PLANNINGUI_H
#define PLANNINGUI_H

#include <QMainWindow>

namespace Ui {class PlanningUi;}

class PlanningUi : public QMainWindow
{
    Q_OBJECT

    public:
        explicit PlanningUi(QWidget *parent = 0);
        ~PlanningUi();

    private slots:
        void finish(bool);

    private:
        Ui::PlanningUi *ui;
};

#endif // PLANNINGUI_H
