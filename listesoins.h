#ifndef LISTESOINS_H
#define LISTESOINS_H


class ListeSoins
{
    public:
        ListeSoins(int ids, int idc, int idx, double val =0);
        ListeSoins();
        ~ListeSoins();

        void setIdX(int idx);
        void setIdS(int id);
        void setIdC(int id);

        int getIdX(){return id;}
        int getIdS(){return idSoin;}
        int getIdC(){return idCategorie;}
        double valeur(){return valeur_;} //prix


    private:
        int idSoin;
        int idCategorie;
        int id;
        double valeur_;
};

#endif // LISTESOINS_H
