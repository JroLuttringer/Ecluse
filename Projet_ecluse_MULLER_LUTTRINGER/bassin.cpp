#include "bassin.h"
#include "QDebug"

Bassin::Bassin(QWidget *parent) :
    QWidget(parent)
{
    this->vanne_amont = Equipment::Ferme;
    this->vanne_aval = Equipment::Ferme;
    this->eau_amont  = 800;
    this->eau_aval   = 200;
    this->eau_ecluse = 500;
    QObject::connect(&timer, SIGNAL(timeout()), this, SLOT(sl_niveau_eau()), Qt::UniqueConnection);
    timer.start(1000);
}

/* Emet un signal contenant le niveau d'eau actuel du bassin */
void Bassin::emit_niveau_eau(){
    emit(sig_niveau_eau(this->eau_ecluse));

}


/* SLOTS */

/* Activé par le timer. Met à jour le niveau d'eau suivant l'état des deux vannes */
void Bassin::sl_niveau_eau(){
    if (this->vanne_amont == Equipment::Ouvert && this->vanne_aval == Equipment::Ouvert){
        if (this->eau_ecluse < 500)
            this->eau_ecluse+=30;
        else if (this->eau_ecluse > 500)
            this->eau_ecluse-=30;
    }
    else if (this->vanne_amont == Equipment::Ouvert && this->eau_ecluse < this->eau_amont){
        this->eau_ecluse+60>800?this->eau_ecluse=800:this->eau_ecluse+=60;
    }
    else if (this->vanne_aval == Equipment::Ouvert && this->eau_ecluse > this->eau_aval)
        this->eau_ecluse-60<200?this->eau_ecluse=200:this->eau_ecluse-=60;
    emit_niveau_eau();
}


/* Met à jour l'état d'une vanne lors d'un changement d'état de cette dernière*/
void Bassin::sl_vanne_amont(Equipment::State s){
    this->vanne_amont = s;
}

void Bassin::sl_vanne_aval(Equipment::State s){
    this->vanne_aval = s;
}
