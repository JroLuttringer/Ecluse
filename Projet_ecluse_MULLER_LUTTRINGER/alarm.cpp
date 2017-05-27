#include "alarm.h"

Alarm::Alarm(QWidget *parent) :
    QWidget(parent)
{
    QObject::connect(&timer, SIGNAL(timeout()), this, SLOT(sl_send_panne()), Qt::UniqueConnection);
    timer.start(30000); // modifier par un nombre aléa pour panne aléatoire
}


/* Choisi un élément aléatoire et le met en panne */
void Alarm::sl_send_panne(){
    srand(time(NULL));
    int panne = rand() % 4 + 1;    
    switch(panne){
        case 1 : e->vanne_aval->panne();
        break;
        case 2 : e->vanne_amont->panne();
        break;
        case 3 : e->porte_aval->panne();
        break;
        case 4 : e->porte_amont->panne();
        break;
    }
}
