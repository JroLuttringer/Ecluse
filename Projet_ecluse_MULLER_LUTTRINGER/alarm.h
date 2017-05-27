#ifndef ALARM_H
#define ALARM_H

#include <QWidget>
#include <QTimer>
#include <ecluse.h>
#include <ctime>

class Alarm : public QWidget
{
    Q_OBJECT
public:
    explicit Alarm(QWidget *parent = 0);
    Ecluse* e;


private:
    QTimer timer;

private slots:
    void sl_send_panne();

};

#endif // ALARM_H
