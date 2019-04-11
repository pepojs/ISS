#include "iss_dane.h"


std::istream & operator >> (std::istream& We, ISS_Dane& D)
{
    We>>D.CzasPrzelotu>>D.Predkosc>>D.Wysokosc>>D.DlugoscGeo>>D.SzerokoscGeo;
    return We;
}

std::ostream & operator <<(std::ostream& Wy, ISS_Dane& D)
{
    Wy<<D.CzasPrzelotu<<" "<<D.Predkosc<<" "<<D.Wysokosc<<" "<<D.DlugoscGeo<<" "<<D.SzerokoscGeo<<std::endl;
    return Wy;
}
