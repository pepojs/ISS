#-------------------------------------------------
#
# Project created by QtCreator 2019-03-02T14:37:33
#
#-------------------------------------------------

QT       += core gui
QT       += network charts opengl
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ISS
TEMPLATE = app
DEPENDPATH += C:/OpenSSL-Win64
# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

SOURCES += \
        main.cpp \
        mainwindow.cpp \
    http.cpp \
    lokalizator.cpp \
    zegar.cpp \
    wykres.cpp \
    openglwidget.cpp \
    KGrafika3D.cpp \
    Shader.cpp \
    magazyn_danych.cpp \
    iss_dane.cpp

HEADERS += \
        mainwindow.h \
    http.h \
    iss_dane.h \
    lokalizator.h \
    zegar.h \
    wykres.h \
    openglwidget.h \
    KGrafika3D.h \
    Shader.h \
    SObiektNaScenie.h \
    modele.h \
    magazyn_danych.h

FORMS += \
        mainwindow.ui


LIBS +=  -lglu32 -lSOIL -lglew32 -lopengl32 -lfreetype

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target


RESOURCES +=
