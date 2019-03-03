#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QByteArray>
#include <QObject>
#include <QLabel>

#include "http.h"
#include "iss_dane.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void WypelniDanymi(QString* dane){*dane = DaneStrony;}

private slots:
    void Dane();


private:
    Ui::MainWindow *ui;
    QString DaneStrony;
    Http* Html;
    ISS_Dane DaneStacji;
};

#endif // MAINWINDOW_H
