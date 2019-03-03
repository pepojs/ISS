#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    Html = new Http();
    //QUrl str("http://wsn.spaceflight.esa.int/iss/index_portal.php");
    //QUrl str("https://isstracker.pl/?lang=pl");
    //QUrl str("http://www.satview.org/?sat_id=37820U");
    //Html->PobierzStrone(str);
    DaneStacji = Html->PobierzDaneOISS();
    std::cout<<"Szerokosc Geograficzna: "<<DaneStacji.SzerokoscGeo<<std::endl;
    std::cout<<"Dlugosc Geograficzna: "<<DaneStacji.DlugoscGeo<<std::endl;
    std::cout<<"Wysokosc: "<<DaneStacji.Wysokosc<<std::endl;
    std::cout<<"Predkosc: "<<DaneStacji.Predkosc<<std::endl;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::Dane()
{
    DaneStrony = Html->ZwrocZawartoscStrony();
    std::cout<<DaneStrony.toStdString()<<std::endl;
}

