#include "equipment.h"

Equipment::Equipment(QObject *parent):QObject(parent)
{
    qRegisterMetaType<Equipment::State>("Equipment::State");
    this->en_panne = false;
    this->alarme = false;
    this->etat = Ferme;
}

//EMISSION DE SIGNAUX
void Equipment::emit_alarme() {
    emit sig_alarme();
}

void Equipment::emit_etat() {
    emit sig_etat(this->etat);
}


// SLOTS
void Equipment::sl_fin_alarme(){
    this->alarme = false;
}




// METHODES
void Equipment::panne() {
    this->en_panne = true;
    emit_alarme();
    this->alarme = true;
}

void Equipment::fin_panne() {
    this->en_panne = false;
    this->alarme = false;
}

