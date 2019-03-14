#include "mainwindow.h"
#include "http.h"
#include "lokalizator.h"
#include <QApplication>
#include <QLabel>
#include <iostream>
#include <QSurfaceFormat>

#include "wykres.h"

int main(int argc, char *argv[])
{
    QApplication Aplikacja(argc, argv);

    MainWindow GlowneOkno;
    GlowneOkno.setGeometry(100,100,1024,512);
    Http Strona(&GlowneOkno);
    GlowneOkno.show();

    return Aplikacja.exec();
}
