#ifndef OPENGLWIDGET_H
#define OPENGLWIDGET_H

#include "KGrafika3D.h"
#include "modele.h"

#include <QWidget>
#include <QOpenGLWidget>
#include <QSurfaceFormat>
#include <QCoreApplication>


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

private:
    Grafika3D* Scena;

};

#endif // OPENGLWIDGET_H
