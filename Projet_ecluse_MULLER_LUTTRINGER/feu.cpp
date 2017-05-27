#include "feu.h"

Feu::Feu(QWidget *parent) :
    QWidget(parent)
{
    this->rouge = false;
    this->vert = false;
}


void Feu::set_rouge(bool b) {
    this->rouge = b;
}

void Feu::set_vert(bool b) {
    this->vert =b;
}
