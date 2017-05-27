#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{

    etat_vanne_amont = Equipment::Ferme;
    etat_vanne_aval = Equipment::Ferme;
    etat_porte_amont = Equipment::Ferme;
    etat_porte_aval = Equipment::Ferme;
    position_porte_amont = 0;
    position_porte_aval = 0;
    this->nv_eau = 500;
    this->e = new Ecluse;
    this->a.e = e;
    ui->setupUi(this);    
    ui->lineEdit_password->setEchoMode(QLineEdit::Password);
    //connexion de tous les boutons de l'UI manuel
    connexion_frame(ui->Frame_manuelle);

    //Connexion des boutons gérant l'authentification
    connect(ui->show_module_authentication,SIGNAL(pressed()), this, SLOT(sl_show_auth()));
    connect(ui->bouton_connexion,SIGNAL(pressed()), this, SLOT(sl_auth()));
    connect(ui->deconnexion, SIGNAL(pressed()), this, SLOT(sl_disconnect()));


    //connect emit_alarm des equipment à l'arret d'urgence
    connect(e->porte_amont, SIGNAL(sig_alarme()), this, SLOT(sl_arret_urgence()));
    connect(e->porte_aval, SIGNAL(sig_alarme()), this, SLOT(sl_arret_urgence()));
    connect(e->vanne_amont, SIGNAL(sig_alarme()), this, SLOT(sl_arret_urgence()));
    connect(e->vanne_aval, SIGNAL(sig_alarme()), this, SLOT(sl_arret_urgence()));
    connect(ui->arret_urgence, SIGNAL(pressed()), this, SLOT(sl_arret_urgence()));

    //fin urgence
    connect(ui->remove_alarm, SIGNAL(pressed()), this, SLOT(sl_fin_urgence()));

    // Connexion des singaux reçus par les valves
    connect(e->vanne_aval, SIGNAL(sig_etat(Equipment::State)), this, SLOT(sl_vanne_aval(Equipment::State)));
    connect(e->vanne_amont, SIGNAL(sig_etat(Equipment::State)), this, SLOT(sl_vanne_amont(Equipment::State)));
    connect(e->vanne_aval, SIGNAL(sig_etat(Equipment::State)), e->b, SLOT(sl_vanne_aval(Equipment::State)));
    connect(e->vanne_amont, SIGNAL(sig_etat(Equipment::State)), e->b, SLOT(sl_vanne_amont(Equipment::State)));


    // Connexion des signaux des portes
    connect(e->porte_aval, SIGNAL(sig_etat(Equipment::State)), this, SLOT(sl_porte_aval(Equipment::State)));
    connect(e->porte_amont, SIGNAL(sig_etat(Equipment::State)), this, SLOT(sl_porte_amont(Equipment::State)));
    connect(e->porte_aval, SIGNAL(sig_pos(float)), this, SLOT(sl_pos_porte_aval(float)));
    connect(e->porte_amont, SIGNAL(sig_pos(float)), this, SLOT(sl_pos_porte_amont(float)));

    // Connexion boutons automatiques
    connect(ui->direction_amont, SIGNAL(pressed()), this, SLOT(sl_setup_aval()));
    connect(ui->direction_aval, SIGNAL(pressed()), this, SLOT(sl_setup_amont()));
    connect(ui->bouton_marche, SIGNAL(pressed()), this, SLOT(sl_bouton_marche()));

    //Connexion des pannes check et uncheck, traitement des signaux
    connect(e->porte_amont, SIGNAL(sig_alarme()), this, SLOT(sl_panne_porte_amont()));
    connect(e->porte_aval, SIGNAL(sig_alarme()), this, SLOT(sl_panne_porte_aval()));
    connect(e->vanne_amont, SIGNAL(sig_alarme()), this, SLOT(sl_panne_vanne_amont()));
    connect(e->vanne_aval, SIGNAL(sig_alarme()), this, SLOT(sl_panne_vanne_aval()));
    connect(ui->porte_amont_panne, SIGNAL(stateChanged(int)), this, SLOT(sl_check_porte_amont(int)));
    connect(ui->porte_aval_panne, SIGNAL(stateChanged(int)), this, SLOT(sl_check_porte_aval(int)));
    connect(ui->vanne_amont_panne, SIGNAL(stateChanged(int)), this, SLOT(sl_check_vanne_amont(int)));
    connect(ui->vanne_aval_panne, SIGNAL(stateChanged(int)), this, SLOT(sl_check_vanne_aval(int)));   

    // Connextion bassin
    connect(e->b, SIGNAL(sig_niveau_eau(float)), this, SLOT(sl_niveau_eau(float)));
    init_ui();
}

/* Connecte les boutons d'une frame ( récursivement ) */
void MainWindow::connexion_frame(QObject* f){
    if(!f) return;
    if ( f->children().size() == 0 ){
        QPushButton* button = qobject_cast<QPushButton*>( f );
        if ( button )   connect( button, SIGNAL(pressed()), this, SLOT(sl_interface_manuel()));
    } else {
        QListIterator<QObject *> i(f->children());
        while(i.hasNext())
            connexion_frame(i.next());
    }
}

// Active ou désactive les boutons d'une frame
void MainWindow::set_enable_frame(QObject* f, bool enabled){
    if(!f) return;
    if ( f->children().size() == 0 ){
        QPushButton* button = qobject_cast<QPushButton*>( f );
        if ( button )   button->setEnabled(enabled);
    } else {
        QListIterator<QObject *> i(f->children());
        while(i.hasNext())
           set_enable_frame(i.next(),enabled);
    }
}

/* Ajuste le niveau d'eau lorsque le bassin change son niveau */
void MainWindow::sl_niveau_eau(float new_nv_eau){
    this->nv_eau = new_nv_eau;
    ui->water_level_bassin->display(new_nv_eau);
}


/* Change l'affichage de la vanne suivant son état */
void MainWindow::sl_vanne_aval(Equipment::State s){
    etat_vanne_aval = s;
    if (s == Equipment::Ferme)
    {
        ui->vanne_aval_ferme->show();
        ui->vanne_aval_ouvert->hide();
    }
    else
    {
        ui->vanne_aval_ferme->hide();
        ui->vanne_aval_ouvert->show();
    }
}

void MainWindow::sl_vanne_amont(Equipment::State s){
    etat_vanne_amont = s;
    if (s == Equipment::Ferme)
    {
        ui->vanne_amont_ferme->show();
        ui->vanne_amont_ouvert->hide();
    }
    else
    {
        ui->vanne_amont_ferme->hide();
        ui->vanne_amont_ouvert->show();
    }
}

/* Met à jour l'état des portes */
void MainWindow::sl_porte_amont(Equipment::State s){
    etat_porte_amont = s;
}
void MainWindow::sl_porte_aval(Equipment::State s){
    etat_porte_aval = s;
}

void MainWindow::sl_pos_porte_amont(float f){
    ui->porte_amont->setFixedHeight(f*1.8);

}

void MainWindow::sl_pos_porte_aval(float f){
    ui->porte_aval->setFixedHeight(f*1.8);
}

/* Gere l'action des boutons de l'interface manuels */
void MainWindow::sl_interface_manuel(){
    QPushButton* button = qobject_cast<QPushButton*>( sender() );
    if (!button) return;
    if ((button->objectName() == "feu_aval_vert")&&(etat_porte_aval == Equipment::Ouvert))
    {
        e->feu_aval.set_vert(!e->feu_aval.vert);
        ui->voyant_feu_aval_rouge->setStyleSheet(FEU_OFF);
        ui->voyant_feu_aval_vert->setStyleSheet(FEU_VERT_ON);
    }
    else if (button->objectName() == "feu_aval_rouge")
    {
        e->feu_aval.set_rouge(!e->feu_aval.rouge);
        ui->voyant_feu_aval_rouge->setStyleSheet(FEU_ROUGE_ON);
        ui->voyant_feu_aval_vert->setStyleSheet(FEU_OFF);
    }
    else if ((button->objectName() == "feu_amont_vert")&&(etat_porte_amont == Equipment::Ouvert))
    {
        //SI porte ouvert
        e->feu_amont.set_vert(!e->feu_amont.vert);
        ui->voyant_feu_amont_rouge->setStyleSheet(FEU_OFF);
        ui->voyant_feu_amont_vert->setStyleSheet(FEU_VERT_ON);
    }
    else if (button->objectName() == "feu_amont_rouge")// END FEU
    {
        e->feu_amont.set_rouge(!e->feu_amont.rouge);
        ui->voyant_feu_amont_rouge->setStyleSheet(FEU_ROUGE_ON);
        ui->voyant_feu_amont_vert->setStyleSheet(FEU_OFF);
    }
    else if ((button->objectName() == "vanne_aval_ouverture")&&(etat_porte_amont == Equipment::Ferme))
            QtConcurrent::run(e->vanne_aval, &Vanne::ouverture);
    else if (button->objectName() == "vanne_aval_fermeture")
            QtConcurrent::run(e->vanne_aval, &Vanne::fermeture);
    else if ((button->objectName() == "vanne_amont_ouverture")&&(etat_porte_aval == Equipment::Ferme))
            QtConcurrent::run(e->vanne_amont, &Vanne::ouverture);
    else if (button->objectName() == "vanne_amont_fermeture")
            QtConcurrent::run(e->vanne_amont, &Vanne::fermeture);
    else  if ((button->objectName() == "porte_amont_ouverture")&&(this->nv_eau==800))
            e->porte_amont->ouverture();
    else if (button->objectName() == "porte_amont_fermeture")
    {
            e->porte_amont->fermeture();
            ui->voyant_feu_amont_rouge->setStyleSheet(FEU_ROUGE_ON);
            ui->voyant_feu_amont_vert->setStyleSheet(FEU_OFF);
    }

    else if (button->objectName() == "porte_amont_stop")
    {
            e->porte_amont->arret();
    }
    else if ((button->objectName() ==  "porte_aval_ouverture")&&(this->nv_eau==200))
            e->porte_aval->ouverture();

    else if (button->objectName() == "porte_aval_fermeture")
    {
            e->porte_aval->fermeture();
            ui->voyant_feu_aval_rouge->setStyleSheet(FEU_ROUGE_ON);
            ui->voyant_feu_aval_vert->setStyleSheet(FEU_OFF);
    }

    else if (button->objectName() == "porte_aval_stop")
            e->porte_aval->arret();

}

void MainWindow::sl_show_auth(){
    ui->show_module_authentication->hide();
    ui->module_authentification->show();
}

void MainWindow::sl_auth() {
    std::cout<<"Auth"<<std::endl;
    if (ui->lineEdit_username->text()=="admin" && ui->lineEdit_password->text()=="admin"){
        std::cout<<"Auth OK"<<ui->stacked_interface->currentIndex()<<std::endl;
        ui->stacked_interface->setCurrentIndex(0);
        ui->module_authentification->hide();
        ui->deconnexion->show();
    }
}

void MainWindow::sl_disconnect(){
    ui->stacked_interface->setCurrentIndex(1);
    ui->show_module_authentication->show();
    ui->deconnexion->hide();
    ui->lineEdit_password->setText("password");
    ui->lineEdit_username->setText("Username");
}

void MainWindow::init_ui(){
    //les vannes sont fermées
    ui->bouton_marche->setEnabled(false);
    ui->vanne_amont_ouvert->hide();
    ui->vanne_aval_ouvert->hide();
    ui->module_authentification->hide();
    ui->deconnexion->hide();
    ui->stacked_interface->setCurrentIndex(1);
}

void MainWindow::sl_arret_urgence(){
    //Allumer alarme
    ui->voyant_alarme->setStyleSheet(ALARME_ON);
    //Disable l'interface automatique    
    ui->bouton_marche->setEnabled(false);
    ui->direction_amont->setEnabled(false);
    ui->direction_aval->setEnabled(false);
    set_enable_frame(ui->frame_amont, false);
    set_enable_frame(ui->frame_aval, false);

    //FERMER LES VANNES
    QtConcurrent::run(e->vanne_aval, &Vanne::fermeture);
    //if it sends closed
    ui->vanne_aval_ferme->show();
    ui->vanne_aval_ouvert->hide();
    QtConcurrent::run(e->vanne_amont, &Vanne::fermeture);
    //if it sends closed
    ui->vanne_amont_ferme->show();
    ui->vanne_amont_ouvert->hide();

    //STOPPER LES PORTES
    e->porte_amont->arret();
    e->porte_aval->arret();

    //METTRE FEUX A ROUGE
    e->feu_amont.set_rouge(!e->feu_amont.rouge);
    ui->voyant_feu_amont_rouge->setStyleSheet(FEU_ROUGE_ON);
    ui->voyant_feu_amont_vert->setStyleSheet(FEU_OFF);

    e->feu_aval.set_rouge(!e->feu_aval.rouge);
    ui->voyant_feu_aval_rouge->setStyleSheet(FEU_ROUGE_ON);
    ui->voyant_feu_aval_vert->setStyleSheet(FEU_OFF);

}

void MainWindow::sl_fin_urgence(){
    if (!(e->porte_amont->en_panne)&&!(e->porte_aval->en_panne)&&!(e->vanne_amont->en_panne)&&!(e->vanne_aval->en_panne))
    {           
    ui->voyant_alarme->setStyleSheet(ALARME_OFF);

    ui->bouton_marche->setEnabled(true);
    ui->direction_amont->setEnabled(true);
    ui->direction_aval->setEnabled(true);
    set_enable_frame(ui->frame_amont, true);
    set_enable_frame(ui->frame_aval, true);
    }
}

void MainWindow::sl_setup_aval(){
    QtConcurrent::run(this,&MainWindow::setup_aval);
}

void MainWindow::sl_setup_amont(){
    QtConcurrent::run(this, &MainWindow::setup_amont);
}

void MainWindow::sl_bouton_marche(){
    QtConcurrent::run(this, &MainWindow::bouton_marche);
}

void MainWindow::bouton_marche(){
    ui->direction_amont->setEnabled(false);
    ui->direction_aval->setEnabled(false);
    if (etat_porte_aval == Equipment::Ouvert)
    {
        //fermeture aval donc feu rouge
        ui->voyant_feu_aval_rouge->setStyleSheet(FEU_ROUGE_ON);
        ui->voyant_feu_aval_vert->setStyleSheet(FEU_OFF);
        e->porte_aval->fermeture();
        while(etat_porte_aval != Equipment::Ferme) ;
        ui->voyant_feu_amont_rouge->setStyleSheet(FEU_ROUGE_ON);
        ui->voyant_feu_amont_vert->setStyleSheet(FEU_OFF);
        ui->porte_aval->setStyleSheet("background-color: brown;");
        //fermeture vanne_aval

        QtConcurrent::run(e->vanne_aval, &Vanne::fermeture);
        //wait for porte = ferme
        QtConcurrent::run(e->vanne_amont, &Vanne::ouverture);
        //wait level
        while(this->nv_eau != 800) ;
        e->porte_amont->ouverture();
        //wait ouvert
        while(etat_porte_amont != Equipment::Ouvert) ;
        ui->voyant_feu_amont_rouge->setStyleSheet(FEU_OFF);
        ui->voyant_feu_amont_vert->setStyleSheet(FEU_VERT_ON);

    }
    else if (etat_porte_amont == Equipment::Ouvert)
    {
        //fermeture amont donc feu rouge
        ui->voyant_feu_amont_rouge->setStyleSheet(FEU_ROUGE_ON);
        ui->voyant_feu_amont_vert->setStyleSheet(FEU_OFF);
        e->porte_amont->fermeture();
        while(etat_porte_amont != Equipment::Ferme) ;
        ui->voyant_feu_aval_rouge->setStyleSheet(FEU_ROUGE_ON);
        ui->voyant_feu_aval_vert->setStyleSheet(FEU_OFF);
        //fermeture vanne_amont
        ui->porte_amont->setStyleSheet("background-color: brown;");
        QtConcurrent::run(e->vanne_amont, &Vanne::fermeture);
        //wait for porte = ferme
        QtConcurrent::run(e->vanne_aval, &Vanne::ouverture);
        //wait level
        while(this->nv_eau != 200) ;
        e->porte_aval->ouverture();
        //wait ouvert
        while(etat_porte_aval != Equipment::Ouvert) ;
        ui->voyant_feu_aval_rouge->setStyleSheet(FEU_OFF);
        ui->voyant_feu_aval_vert->setStyleSheet(FEU_VERT_ON);

    }


    ui->direction_amont->setEnabled(true);
    ui->direction_aval->setEnabled(true);
    ui->bouton_marche->setEnabled(false);
}


void MainWindow::setup_aval(){
    ui->direction_amont->setEnabled(false);
    ui->direction_aval->setEnabled(false);
    if (etat_porte_aval == Equipment::Ouvert)
    {
        e->feu_aval.set_vert(!e->feu_aval.vert);
        ui->voyant_feu_aval_rouge->setStyleSheet(FEU_OFF);
        ui->voyant_feu_aval_vert->setStyleSheet(FEU_VERT_ON);
//        ui->bouton_marche->setEnabled(true);
    }
    else
    {
        //fermeture amont donc feu rouge
        ui->voyant_feu_amont_rouge->setStyleSheet(FEU_ROUGE_ON);
        ui->voyant_feu_amont_vert->setStyleSheet(FEU_OFF);
        e->porte_amont->fermeture();
        while(etat_porte_amont != Equipment::Ferme) ;
        ui->voyant_feu_aval_rouge->setStyleSheet(FEU_ROUGE_ON);
        ui->voyant_feu_aval_vert->setStyleSheet(FEU_OFF);
        //fermeture vanne_amont
        ui->porte_amont->setStyleSheet("background-color: brown;");
        QtConcurrent::run(e->vanne_amont, &Vanne::fermeture);
        //wait for porte = ferme
        QtConcurrent::run(e->vanne_aval, &Vanne::ouverture);
        //wait level
        while(this->nv_eau != 200) ;
        e->porte_aval->ouverture();
        //wait ouvert
        while(etat_porte_aval != Equipment::Ouvert) ;
        ui->voyant_feu_aval_rouge->setStyleSheet(FEU_OFF);
        ui->voyant_feu_aval_vert->setStyleSheet(FEU_VERT_ON);

    }
    ui->bouton_marche->setEnabled(true);
}

void MainWindow::setup_amont(){
    ui->direction_aval->setEnabled(false);
    ui->direction_amont->setEnabled(false);

    if (etat_porte_amont == Equipment::Ouvert)
    {
        e->feu_amont.set_vert(!e->feu_amont.vert);
        ui->voyant_feu_amont_rouge->setStyleSheet(FEU_OFF);
        ui->voyant_feu_amont_vert->setStyleSheet(FEU_VERT_ON);
    }
    else
    {
        //fermeture aval donc feu rouge
        ui->voyant_feu_aval_rouge->setStyleSheet(FEU_ROUGE_ON);
        ui->voyant_feu_aval_vert->setStyleSheet(FEU_OFF);
        e->porte_aval->fermeture();
        while(etat_porte_aval != Equipment::Ferme) ;
        ui->voyant_feu_amont_rouge->setStyleSheet(FEU_ROUGE_ON);
        ui->voyant_feu_amont_vert->setStyleSheet(FEU_OFF);
        ui->porte_aval->setStyleSheet("background-color: brown;");
        //fermeture vanne_aval
        QtConcurrent::run(e->vanne_aval, &Vanne::fermeture);
        //wait for porte = ferme
        QtConcurrent::run(e->vanne_amont, &Vanne::ouverture);
        //wait level
        while(this->nv_eau != 800) ;
        e->porte_amont->ouverture();
        //wait ouvert
        while(etat_porte_amont != Equipment::Ouvert) ;
        ui->voyant_feu_amont_rouge->setStyleSheet(FEU_OFF);
        ui->voyant_feu_amont_vert->setStyleSheet(FEU_VERT_ON);

    }
    ui->bouton_marche->setEnabled(true);
}

void MainWindow::sl_check_porte_amont(int state){
    if (state)
        e->porte_amont->panne();
    else
        MainWindow::sl_fin_panne_porte_amont();
}
void MainWindow::sl_check_porte_aval(int state){
    if (state)
        e->porte_aval->panne();
    else
        MainWindow::sl_fin_panne_porte_aval();
}
void MainWindow::sl_check_vanne_amont(int state){
    if (state)
        e->vanne_amont->panne();
    else
        MainWindow::sl_fin_panne_vanne_amont();
}
void MainWindow::sl_check_vanne_aval(int state){
    if (state)
        e->vanne_aval->panne();
    else
        MainWindow::sl_fin_panne_vanne_aval();
}

void MainWindow::sl_panne_vanne_aval(){
    ui->vanne_aval_panne->setChecked(true);
}
void MainWindow::sl_panne_vanne_amont(){
    ui->vanne_amont_panne->setChecked(true);
}
void MainWindow::sl_fin_panne_vanne_aval(){
    e->vanne_aval->fin_panne();
    ui->vanne_aval_panne->setChecked(false);
}
void MainWindow::sl_fin_panne_vanne_amont(){
    e->vanne_amont->fin_panne();
    ui->vanne_amont_panne->setChecked(false);
}
void MainWindow::sl_panne_porte_aval(){
    ui->porte_aval_panne->setChecked(true);
}
void MainWindow::sl_panne_porte_amont(){
    ui->porte_amont_panne->setChecked(true);
}
void MainWindow::sl_fin_panne_porte_aval(){
    e->porte_aval->fin_panne();
    ui->porte_aval_panne->setChecked(false);
}
void MainWindow::sl_fin_panne_porte_amont(){
    e->porte_amont->fin_panne();
    ui->porte_amont_panne->setChecked(false);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete e;
}
