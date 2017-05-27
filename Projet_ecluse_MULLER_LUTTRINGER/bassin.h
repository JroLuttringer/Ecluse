#ifndef BASSIN_H
#define BASSIN_H

#include <QWidget>
#include "equipment.h"
#include "QTimer"

class Bassin : public QWidget
{
    Q_OBJECT
public:
    explicit Bassin(QWidget *parent = 0);

private:
    float eau_aval;
    float eau_amont;
    float eau_ecluse;
    QTimer timer;
    Equipment::State vanne_aval;    // Permet de mémoriser l'état des vannes
    Equipment::State vanne_amont;

public:
    void emit_niveau_eau();

signals:
    void sig_niveau_eau(float nv_eau);

private slots:
    void sl_niveau_eau();  // Declenché par le timer, recalcule le niveau d'eau regulierement
    void sl_vanne_amont(Equipment::State);
    void sl_vanne_aval(Equipment::State);

};

#endif // BASSIN_H
