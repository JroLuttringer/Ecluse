#include "porte.h"

Porte::Porte(QWidget *parent){
    this->en_panne = false;
    this->alarme = false;
    this->etat = Ferme;
    this->position =100;
}

void Porte::fermeture(){
    this->etat = Fermeture;
    this->lancer_timer();
}

void Porte::ouverture(){
    this->etat = Ouverture;
    this->lancer_timer();
}

void Porte::arret(){
    if((this->etat == Ouverture || this->etat == Fermeture ))
        this->timer.stop();
    this->etat = Arret;
}

void Porte::lancer_timer(){
    QObject::connect(&timer, SIGNAL(timeout()), this, SLOT(sl_etat_pos()), Qt::UniqueConnection);
    timer.start(1000);
}

void Porte::sl_alarme() {
    this->alarme = true;
    this->arret();
}

void Porte::emit_pos(){
    emit(sig_pos(this->position));
}

/* Signal émis regulierement lors d'une transition précisant l'état / la position de la porte */
void Porte::sl_etat_pos(){
    if(this->position > 10 && this->etat == Ouverture)
        this->position -= 10;
     else if(this->position < 90 && this->etat == Fermeture)
        this->position +=10;
    else {
        timer.stop();
        if (this->etat == Ouverture)
            this->etat = Ouvert;
        else{
            this->position = 100;
            this->etat = Ferme;
        }
    }
    this->emit_etat();
    this->emit_pos();
}


