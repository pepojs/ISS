#ifndef OPENGLWIDGET_H
#define OPENGLWIDGET_H

#include "KGrafika3D.h"
#include "modele.h"
#include "iss_dane.h"

#include <QWidget>
#include <QOpenGLWidget>
#include <QSurfaceFormat>
#include <QCoreApplication>
#include <QEvent>
#include <QMouseEvent>
#include <QWheelEvent>


class OpenGLWidget : public QOpenGLWidget
{
    Q_OBJECT
public:
    OpenGLWidget(QWidget* Rodzic = nullptr);
    ~OpenGLWidget();

    int8_t UstawVertexShader(QString NazwaShadera);
    int8_t UstawFragmentShader(QString NazwaShadera);
    int8_t LinkujShader();
    void TworzZiemie(GLfloat Promien);
    void TworzPunktPozycji();
    void TworzTrajektorie();
    void TworzSiatke();
    void SledzenieStacji(GLboolean Sledz){fSledzenieStacji = Sledz;}
    void RysowanieTrajektorii(GLboolean Rysuj){Scena->RysujObiekt(IDTrajektoria, Rysuj);}
    void RysowanieSiatki(GLboolean Rysuj){Scena->RysujObiekt(IDSiatki, Rysuj);}
    void RysowaniePunktuSledzacego(GLboolean Rysuj){Scena->RysujObiekt(IDWskaznikStacji, Rysuj);}
    void UstawKamereNaStrefie(int Strefa);
    void PodswietlenieStrefy(int Podswietl){Scena->PrzekazIntaDoShedera("WlaczStrefe", Podswietl);}

protected:
    void initializeGL();
    void resizeGL(int Szerokosc, int Wysokosc);
    void paintGL();
    bool event(QEvent* Zdarzenie);

private slots:
    void NoweDaneISS(ISS_Dane NoweDane);


private:
    Grafika3D* Scena;
    int SzerokoscOkna;
    int WysokoscOkna;
    GLfloat KameraOdObiektu;
    GLfloat KameraX, KameraY, KameraZ;
    GLfloat ObrotX, ObrotY;
    int PosMyszyX, PosMyszyY;
    GLfloat PromienKuli;
    GLuint IDWskaznikStacji;
    GLfloat KatSzer, KatDlug;
    GLuint fSledzenieStacji;
    GLboolean IDTrajektoria;
    GLuint OffsetTrajektori;
    GLuint IDSiatki;

};

#endif // OPENGLWIDGET_H
