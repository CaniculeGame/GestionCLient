#include "listesoins.h"

ListeSoins::ListeSoins()
{
    idSoin=1;
    idCategorie=1;
}

ListeSoins::ListeSoins(int ids, int idc, int idx, double val)
{
    if(ids > 0 && idc > 0)
    {
        idSoin=ids;
        idCategorie=idc;
        id=idx;
        valeur_ = val;
    }
    else
    {
        idSoin=1;
        idCategorie=1;
        id=0;
        valeur_ = val;
    }
}

ListeSoins::~ListeSoins()
{

}

void ListeSoins::setIdX(int idx){id=idx;}
void ListeSoins::setIdS(int id){idSoin=id;}
void ListeSoins::setIdC(int id){idCategorie=id;}

