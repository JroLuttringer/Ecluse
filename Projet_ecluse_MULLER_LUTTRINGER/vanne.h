#ifndef VANNE_H
#define VANNE_H

#include <QWidget>
#include "equipment.h"

class Vanne : public Equipment
{
    Q_OBJECT
public:
    explicit Vanne(QWidget *parent = 0);
    virtual void fermeture();
    virtual void ouverture();
private slots:
    void sl_alarme();
};

#endif // VANNE_H
