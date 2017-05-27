#ifndef PORTE_H
#define PORTE_H

#include <QWidget>
#include "equipment.h"
#include "QTimer"
class Porte : public Equipment
{
    Q_OBJECT
public:
    explicit Porte(QWidget *parent = 0);
    void fermeture();
    void ouverture();
    void arret();
private:
     float position ;// ouverture de la porte en %
     QTimer timer;
     void emit_pos();
     void lancer_timer();
signals:
    void sig_pos(float);

public slots:
     void sl_alarme();
     void sl_etat_pos();

};

#endif // PORTE_H
