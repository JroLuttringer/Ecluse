#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "ecluse.h"
#include "alarm.h"
#include "equipment.h"
#include <QPushButton>
#include <QThread>
#include "iostream"
#include <QDebug>
#include <QtConcurrent/QtConcurrent>
#include "stylesheets.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    Alarm a; // Générateur de Panne
    explicit MainWindow(QWidget *parent = 0);
    Ecluse* e;
    Equipment::State etat_vanne_amont;
    Equipment::State etat_vanne_aval;
    Equipment::State etat_porte_amont;
    Equipment::State etat_porte_aval;
    float position_porte_amont;
    float position_porte_aval;
    float nv_eau;
    void connexion_frame(QObject*);
    void init_ui();
    void setup_aval();
    void setup_amont();
    void bouton_marche();
    void set_enable_frame(QObject*, bool);
     ~MainWindow();

private:
    Ui::MainWindow *ui;
public slots:
    void sl_interface_manuel();
    void sl_show_auth();
    void sl_auth();
    void sl_disconnect();
    void sl_arret_urgence();
    void sl_fin_urgence();
    void sl_vanne_aval(Equipment::State);
    void sl_vanne_amont(Equipment::State);
    void sl_porte_amont(Equipment::State);
    void sl_porte_aval(Equipment::State);
    void sl_pos_porte_amont(float);
    void sl_pos_porte_aval(float);
    void sl_niveau_eau(float);
    void sl_setup_aval();
    void sl_setup_amont();
    void sl_bouton_marche();
    void sl_panne_vanne_aval();
    void sl_panne_vanne_amont();
    void sl_fin_panne_vanne_aval();
    void sl_fin_panne_vanne_amont();
    void sl_panne_porte_aval();
    void sl_panne_porte_amont();
    void sl_fin_panne_porte_aval();
    void sl_fin_panne_porte_amont();
    void sl_check_vanne_aval(int);
    void sl_check_vanne_amont(int);
    void sl_check_porte_aval(int);
    void sl_check_porte_amont(int);
};

#endif // MAINWINDOW_H
