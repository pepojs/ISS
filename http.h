#ifndef HTTP_H
#define HTTP_H

/*!
 * \file
 * \brief Definicja klasy Http
*/

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

//Zla kolejnosc pobierania danych, wysyla w odpowiedniej kolejnosci, ale odbiera
//w rozne, brak uzupelniania danych kiedy gdzies w srodku pojawia sie zera

/*!
 * \brief Komunikacja ze stroną internetową
 *
 * Klasa ma za zadanie realizowac komunikacje z serwerem przechowującym dane na temat stacji kosmicznej.
 * Pozwala ona na pobranie danych ze strony oraz parserowanie tych danych do postać pozwalającej korzystać z nich
 * w pozostałej części programu.
 *
*/

class Http : public QObject
{
    Q_OBJECT

public:

    Http(QObject* Rodzic = nullptr);

    /*!
     * \brief Pobiera zawartość strony internetowej.
     * \param[in] Url - adres strony internetowej do pobrania.
     */
    void PobierzStrone(const QUrl &Url);

    /*!
     * \brief Zwraca zawartość strony w postaci QString.
     * \return Zawartość pobranej strony.
     */
    QString ZwrocZawartoscStrony();

    /*!
     * \brief Pobiera aktualne dane o Międzynarodowej Stacji Kosmicznej.
     * \return Aktualne dane na temat stacji kosmicznej.
     */
    ISS_Dane PobierzDaneOISS();

    /*!
     * \brief Pobiera dane o Międzynarodowej Stacji Kosmicznej.
     * Metoda pozwala pobrać do 10 rekordów na temat pozycji stacji kosmicznej od podanego czasu poczatkowego
     *  co podaną częstotliwość. Dane zostaną zapisane do tablicy.
     * \param TablicaDanych - wskaźnik na tablice, do której mają być zapisane pobrane dane.
     * \param IloscDanych - ilość danych do pobranie. (Maksymalnie 10)
     * \param PoczatekCzasu - wartość czas umieszczonego w danych stacji, od któreg rozpocznie się pobieranie danych, podany w formacie unixowym w sekundach.
     * \param Czestotliwoasc - odstęp czasu pomiedzy kolejnymi pobieranymi danami, wyrażony w sekundach.
     */
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
