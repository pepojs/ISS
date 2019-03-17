#ifndef OPENGLWIDGET_H
#define OPENGLWIDGET_H

#include "KGrafika3D.h"
#include "modele.h"

#include <QWidget>
#include <QOpenGLWidget>
#include <QSurfaceFormat>
#include <QCoreApplication>
#include <QEvent>
#include <QMouseEvent>
#include <QWheelEvent>


class OpenGLWidget : public QOpenGLWidget
{

public:
    OpenGLWidget(QWidget* Rodzic = nullptr);
    ~OpenGLWidget();

    int8_t UstawVertexShader(QString NazwaShadera);
    int8_t UstawFragmentShader(QString NazwaShadera);
    int8_t LinkujShader();
    void TworzZiemie(GLfloat Promien);

protected:
    void initializeGL();
    void resizeGL(int Szerokosc, int Wysokosc);
    void paintGL();
    bool event(QEvent* Zdarzenie);

private:
    Grafika3D* Scena;
    int SzerokoscOkna;
    int WysokoscOkna;
    GLfloat KameraOdObiektu;
    GLfloat KameraX, KameraY, KameraZ;
    GLfloat ObrotX, ObrotY;
    int PosMyszyX, PosMyszyY;

};

#endif // OPENGLWIDGET_H
