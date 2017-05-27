#include "ecluse.h"

Ecluse::Ecluse(QWidget *parent):QWidget(parent)
{
    this->vanne_amont = new Vanne;
    this->vanne_aval = new Vanne;
    this->porte_amont = new Porte;
    this->porte_aval = new Porte;
    this->b = new Bassin;

}

Ecluse::~Ecluse(){
    delete vanne_amont;
    delete vanne_aval;
    delete porte_aval;
    delete porte_amont;
    delete b;
}
