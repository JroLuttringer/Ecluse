#ifndef FEU_H
#define FEU_H

#include <QWidget>
#include <QDebug>

class Feu : public QWidget
{
    Q_OBJECT
public:
    explicit Feu(QWidget *parent = 0);
    bool rouge;
    bool vert;
    void set_rouge(bool);
    void set_vert(bool);
};

#endif // FEU_H
