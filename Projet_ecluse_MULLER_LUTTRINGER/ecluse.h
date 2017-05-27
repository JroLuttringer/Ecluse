#ifndef ECLUSE_H
#define ECLUSE_H

#include "feu.h"
#include "bassin.h"
#include "porte.h"
#include "vanne.h"

#include <QWidget>

class Ecluse : public QWidget
{
     Q_OBJECT
public:
    explicit Ecluse(QWidget *parent = 0);
    Bassin* b;
    /* AVAL */
    Feu feu_aval;
    Porte* porte_aval;
    Vanne* vanne_aval;

    /*AMONT*/
    Feu feu_amont ;
    Porte* porte_amont;
    Vanne* vanne_amont;
    ~Ecluse();

};

#endif // ECLUSE_H
