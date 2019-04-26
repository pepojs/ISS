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

//pakiet units latex
//Powtorzyc krotkie opisy tak aby byly pod spodem, poprawic w dokumentacji ogolnej na samym dole powtorzenia, przejzec ja
//i poprawic, dodac na wykresach kreske tak aby bylo widac ze w tej godzinie jest ta wartosc
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
    QString ZwrocZawartoscStrony() const;

    /*!
     * \brief Pobiera aktualne dane o Międzynarodowej Stacji Kosmicznej.
     * \return Aktualne dane na temat stacji kosmicznej.
     */
    ISS_Dane PobierzDaneOISS();

    /*!
     * \brief Pobiera dane o Międzynarodowej Stacji Kosmicznej.
     * Metoda pozwala pobrać do 10 rekordów na temat pozycji stacji kosmicznej od podanego czasu poczatkowego
     *  co podaną częstotliwość. Dane zostaną zapisane do tablicy.
     * \param[out] TablicaDanych - wskaźnik na tablice, do której mają być zapisane pobrane dane.
     * \param[in] IloscDanych - ilość danych do pobranie. (Maksymalnie 10)
     * \param[in] PoczatekCzasu - wartość czas umieszczonego w danych stacji, od któreg rozpocznie się pobieranie danych, podany w formacie unixowym w sekundach.
     * \param[in] Czestotliwoasc - odstęp czasu pomiedzy kolejnymi pobieranymi danami, wyrażony w sekundach.
     */
    void PobierzDaneOISS(ISS_Dane* TablicaDanych, int IloscDanych, uint PoczatekCzasu, uint Czestotliwoasc);

signals:
    /*!
     * \brief Sygnał wysyłany po pobraniu odpowiedzi ze strony internetowej.
     */
    void pobrano();

private slots:
    /*!
     * \brief Slot wywoływany po pobraniu strony aby zapisać ją do pola htmlStrony.
     * \param[in] Odpowiedz - ze strony internetowej.
     */
    void PobranoStrone(QNetworkReply* Odpowiedz);

private:

    /*!
     * \brief Obiekt pozwalający na komunikacje ze stroną internetową.
     */
    QNetworkAccessManager http;

    /*!
     * \brief String przechowujący treść pobranej strony.
     */
    QString htmlStrony;

    /*!
     * \brief Metoda prywatna służąca do parserowania danych ze strony internetowej.
     * \return Zwraca dane na temat stacji uzyskane ze strony internetowej.
     */
    ISS_Dane ParserDanychISS();

    /*!
     * \brief Metoda prywatna służąca do parserowania większej ilości danych za jednym wywołaniem (do 10)
     * \param[out] TablicaDanych - tablica, w której zostaną zapisane uzyskane dane o stacji.
     * \param[in] IloscDanych - ilość danych do praserowania.
     */
    void PraserWieluDanychISS(ISS_Dane* TablicaDanych, int IloscDanych);
};

#endif // HTTP_H
