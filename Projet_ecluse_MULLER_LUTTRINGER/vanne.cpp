#include "vanne.h"

Vanne::Vanne(QWidget *parent)
{
    this->en_panne = false;
    this->alarme = false;
    this->etat = Ferme;
}

void Vanne::fermeture(){
    if (this->alarme) return;
    this->etat = Ferme;
    this->emit_etat();
}


void Vanne::ouverture(){
    if (this->alarme) return;
    this->etat = Ouvert;
    this->emit_etat();
}


void Vanne::sl_alarme() {
    this->alarme = true;
    this->fermeture();
}
