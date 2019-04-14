#ifndef ISS_DANE_H
#define ISS_DANE_H

/*!
 * \file
 * \brief Definicja klasy ISS_Dane
*/

#include <QWidget>
#include <iostream>

//Przerobic to na klase wraz z całym interfejsem opisac 4 klasy, na pewno http oraz dane_stacji, 2 dodatkowe wybrac (ciekawsze)
//Dane stacji dodac jakie jednoski na przyklad pobierz_predkosc_kmH, dokumentacja html-u doxygen, metody ktore nie zmieniaja
//obiekt dodac const. W dokumentacji dodać przykładowe użycie, przykładowe uzyskane dane, poprawić w dziale komunikacji napisy w cudzyslowie
//wyroznic inna czcionka, dodac przykladowy rekord jak wygladaja dane, pozmieniac w dokumentacji te nazwy na poczatku, poprawic trajektorie
//poprawic napis "czas przelotu" na aktualna godzinne

/*!
 * \brief Przechowywanie dane na temat stacji
 *
 * Klasa ma za zadanie przechowywać dane związane z pojedynczą pozycją stacji w czasie.
 * Jej atrybutami są pola pozwalające określić położenie oraz prędkość stacji w danym czasie.
 * Odpowiednie wartości są wyrażone w jednostkach SI
 *
*/

class ISS_Dane
{
    /*!
     * \brief Czas, dla którego przechowywane są dane.
     *
     * Pole zawiera czas, dla którego stacja znajdowała się w wyznaczonej pozycji.
     * Dane przechowywane są w formacie uniksowym, w sekundach.
     */
    uint32_t CzasPrzelotu;

    /*!
     * \brief Predkość z jaką porusza się stacja.
     *
     * Pole zawiera prędkość z jaką porusza się stacja kosmiczna.
     * Dane przechowywane są w kilometrach na godzinę.
     */
    double Predkosc;

    /*!
     * \brief Wysokość stacji nad powierzchnią Ziemi.
     *
     * Pole zawiera wysokość nad powierzchnią Zimei, na której znajduje się stacja kosmiczna.
     * Dane przechowywane są w kilometrach.
     */
    double Wysokosc;

    /*!
     * \brief Szerokość geograficzna stacji
     *
     * Pole zawiera szerokość geograficzną punktu, nad którym znajduje się stacja kosmiczna.
     * Dane przechowywane są w stopniach.
     */
    double SzerokoscGeo;

    /*!
     * \brief Długość geograficzna stacji
     *
     * Pole zawiera długość geograficzną punktu, nad którym znajduje się stacja kosmiczna.
     * Dane przechowywane są w stopniach.
     */
    double DlugoscGeo;

    friend std::istream & operator >> (std::istream& We, ISS_Dane& D);
    friend std::ostream & operator << (std::ostream& Wy, ISS_Dane& D);

public:

    /*!
     * \brief Inicjalizuje wszystkie pola z wartoscia równą 0
     */
    ISS_Dane():SzerokoscGeo(0), DlugoscGeo(0),Predkosc(0), Wysokosc(0), CzasPrzelotu(0)
    {;}


    /*!
     * \brief Wartość szerokości geograficznej w stopniach
     * \return Zwraca wartość szerokości geograficznej w stopniach.
     */
    double ZwrocSzerokoscGeo_Stopnie() const {return SzerokoscGeo;}

    /*!
     * \brief Ustawia nową wartość szerokości geograficzne.
     * \param[in] NowaSzerokosc - wartość jaka ma zostać ustawiona dla pola SzerokoscGeo, wyrażona w stopniach.
     */
    void ZmienSzerokoscGeo_Stopnie(double NowaSzerokosc) {SzerokoscGeo = NowaSzerokosc;}

    /*!
     * \brief Wartość dlugości geograficznej w stopniach
     * \return Zwraca wartość dlugości geograficznej w stopniach.
     */
    double ZwrocDlugoscGeo_Stopnie() const {return DlugoscGeo;}

    /*!
     * \brief Ustawia nową wartość długości geograficzne.
     * \param[in] NowaDlugosc - wartość jaka ma zostać ustawiona dla pola DlugoscGeo, wyrażona w stopniach.
     */
    void ZmienDlugoscGeo_Stopnie(double NowaDlugosc) {DlugoscGeo = NowaDlugosc;}

    /*!
     * \brief Wartość czasu, dla których są zapisane dane.
     * \return Zwraca wartość czasu, dla których są zapisane dane, wyrażone jako czas unixowy w sekundach.
     */
    uint32_t ZwrocCzasPrzelotu_UTS() const {return CzasPrzelotu;}

    /*!
     * \brief Ustawia nową wartość czasu.
     * \param[in] NowyCzas - wartość jaka ma zostać ustawiona dla pola CzasPrzelotu, wyrażone jako czas unixowy w sekundach.
     */
    void ZmienCzasPrzelotu_UTS(uint32_t NowyCzas) {CzasPrzelotu = NowyCzas;}

    /*!
     * \brief Wartość prędkości.
     * \return Zwraca wartość prędkości stacji w kilometrach na godzinę.
     */
    double ZwrocPredkosc_kmH() const {return Predkosc;}

    /*!
     * \brief Ustawia nową wartość prędkości stacji kosmicznej.
     * \param[in] NowaPredkosc - wartość jaka ma zostać ustawiona dla pola Predkosc, wyrażona w kilometrach na godzinę.
     */
    void ZmienPredkosc_kmH(double NowaPredkosc) {Predkosc = NowaPredkosc;}

    /*!
     * \brief Wartość wysokości na jakiej znajduje się stacja.
     * \return Zwraca wartość wysokości na jakiej znajduje się stacja kosmiczna w kilometrach.
     */
    double ZwrocWysokosc_km() const {return Wysokosc;}

    /*!
     * \brief Ustawia nową wartość wysokości na jakiej znajduje się stacja.
     * \param[in] NowaWysokosc - wartość jaka ma zostać ustawiona dla pola Wysokosc, wyrażona w kilometrach.
     */
    void ZmienWysokosc_km(double NowaWysokosc) {Wysokosc = NowaWysokosc;}

};

#endif // ISS_DANE_H
