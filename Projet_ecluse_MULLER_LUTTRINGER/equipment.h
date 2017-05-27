#ifndef EQUIPMENT_H
#define EQUIPMENT_H

#include <QWidget>
#include <QDebug>
#include <iostream>
#include <QThread>



class Equipment : public QObject
{
    Q_OBJECT
public:
   Equipment(QObject *parent = 0);
   bool en_panne;
   void panne();
   void fin_panne();
   enum State { Ouvert, Ferme, Ouverture, Fermeture, Arret };

protected:
    bool alarme;
    State etat;
    void emit_alarme();
    void emit_etat();
    virtual void fermeture()=0;
    virtual void ouverture()=0;
signals:
    void sig_alarme();
    void sig_etat(Equipment::State);

private slots:
    void sl_fin_alarme();
    virtual void sl_alarme()=0;
};

 Q_DECLARE_METATYPE(Equipment::State);

#endif // EQUIPMENT_H
