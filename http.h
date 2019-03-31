#ifndef HTTP_H
#define HTTP_H

#include <QObject>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>
#include <QUrl>
#include <QString>
#include <QByteArray>
#include <iostream>
#include <QLabel>
#include <QMessageBox>
#include <QTimer>
#include <QEventLoop>
#include <iostream>

#include "iss_dane.h"

class Http : public QObject
{
    Q_OBJECT

public:
    Http(QObject* Rodzic = nullptr);

    void PobierzStrone(const QUrl &Url);
    QString ZwrocZawartoscStrony();
    ISS_Dane PobierzDaneOISS();
    void PobierzDaneOISS(ISS_Dane* TablicaDanych, int IloscDanych, uint PoczatekCzasu, uint Czestotliwoasc);

signals:
    void pobrano();

private slots:
    void PobranoStrone(QNetworkReply* Odpowiedz);

private:
    QNetworkAccessManager http;
    QString htmlStrony;
    ISS_Dane ParserDanychISS();
    void PraserWieluDanychISS(ISS_Dane* TablicaDanych, int IloscDanych);
};

#endif // HTTP_H
