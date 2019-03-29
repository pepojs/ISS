#include "KGrafika3D.h"

Grafika3D::~Grafika3D()
{
    //Odlacza atrybuty dla vertex shader
    glDisableVertexAttribArray(0);

    //Usuwa vbo i vao
    glDeleteBuffers(vbo.size(), &vbo[0]);
    glDeleteVertexArrays(vao.size(), &vao[0]);

}

void Grafika3D::CzyscPoGrafice3D()
{
     //Odlacza atrybuty dla vertex shader
     glDisableVertexAttribArray(0);

     //Usuwa vbo i vao
     glDeleteBuffers(vbo.size(), &vbo[0]);
     glDeleteVertexArrays(vao.size(), &vao[0]);

}

//Metoda tworzy nowy obiekt rysowany statycznie, pierwszy argument to zbior puktow tego opiektu,
//drugi argument to ilosc wspolrzednych opisujacych punkt, zwraca vao obiektu
GLuint Grafika3D::TworzObiekt(GLvoid* TablicaPunktow, size_t RozmiarTablicyPunktow, uint8_t IloscWspolrzednychPunktu)
{
    vao.push_back(0); //Nowa tablica obiektu (mozna w niej umiescic bufor z punktami, kolore itp.)
    vbo.push_back(0); //Nowy bufor obiektu dla punktow obiektu

    //Tworzy nowy voa i ustawia na aktywna
    glGenVertexArrays(1, &vao[vao.size()-1]);
    glBindVertexArray(vao[vao.size()-1]);

    //Tworzy nowy bufor, ustawia na aktywny i umieszcza dane z punktami
    glGenBuffers(1, &vbo[vbo.size()-1]);
    glBindBuffer(GL_ARRAY_BUFFER, vbo[vbo.size()-1]);
    glBufferData(GL_ARRAY_BUFFER, RozmiarTablicyPunktow, TablicaPunktow, GL_STATIC_DRAW);

    //Ustawia atrybut dla vertex shader
    if(IloscWspolrzednychPunktu >= 4 || IloscWspolrzednychPunktu == 0)
        glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, nullptr);
    else
        glVertexAttribPointer(0, IloscWspolrzednychPunktu, GL_FLOAT, GL_FALSE, 0, nullptr);

    glEnableVertexAttribArray(0);

    //Dezaktywuje voa
    glBindVertexArray(0);

    return vao[vao.size()-1];
}

//Metoda tworzy nowy obiekt rysowany statycznie oraz okresla jego kolor,
//pierwszy argument to zbior puktow tego obiektu, drugi argument to ilosc wspolrzednych opisujacych punkt,
//trzeci argument to tablica z kolorami odpowiednich punktow, czwarty ilosc elementow w wektorze
//opisujacym kolor (np. RGB i Alpha), zwraca vao obiektu
GLuint Grafika3D::TworzObiekt(GLvoid* TablicaPunktow, size_t RozmiarTablicyPunktow, uint8_t IloscWspolrzednychPunktu,
                       GLvoid* TablicaKolorow, size_t RozmiarTablicyKolorow, uint8_t IloscWspolrzednychKoloru)
{
    vao.push_back(0); //Nowa tablica obiektu (mozna w niej umiescic bufor z punktami, kolore itp.)
    vbo.push_back(0); //Nowy bufor obiektu dla punktow obiektu
    vbo.push_back(0); //Nowy bufor obiektu dla kolorow odpowiednich punktow

    //Tworzy nowy voa i ustawia na aktywna
    glGenVertexArrays(1, &vao[vao.size()-1]);
    glBindVertexArray(vao[vao.size()-1]);

    //Punkty obiektu
    //Tworzy nowy bufor, ustawia na aktywny i umieszcza dane z punktami
    glGenBuffers(1, &vbo[vbo.size()-2]);
    glBindBuffer(GL_ARRAY_BUFFER, vbo[vbo.size()-2]);
    glBufferData(GL_ARRAY_BUFFER, RozmiarTablicyPunktow, TablicaPunktow, GL_STATIC_DRAW);

    //Ustawia atrybut dla vertex shader
    if(IloscWspolrzednychPunktu >= 4 || IloscWspolrzednychPunktu == 0)
        glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);
    else
        glVertexAttribPointer(0, IloscWspolrzednychPunktu, GL_FLOAT, GL_FALSE, 0, 0);

    glEnableVertexAttribArray(0);

    //Kolory obiektu
    //Tworzy nowy bufor, ustawia na aktywny i umieszcza dane z kolorem obiektu
    glGenBuffers(1, &vbo[vbo.size()-1]);
    glBindBuffer(GL_ARRAY_BUFFER, vbo[vbo.size()-1]);
    glBufferData(GL_ARRAY_BUFFER, RozmiarTablicyKolorow, TablicaKolorow, GL_STATIC_DRAW);

    //Ustawia atrybut dla vertex shader
        //Ustawia atrybut dla vertex shader
    if(IloscWspolrzednychKoloru >= 4 || IloscWspolrzednychKoloru == 0)
        glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, nullptr);
    else
        glVertexAttribPointer(1, IloscWspolrzednychKoloru, GL_FLOAT, GL_FALSE, 0, nullptr);

    glEnableVertexAttribArray(1);

    //Dezaktywuje voa
    glBindVertexArray(0);

    return vao[vao.size()-1];
}

//Metoda tworzy nowy obiekt rysowany statycznie oraz okresla jego kolor,
//pierwszy argument to tablica zepunktami, kolorami, i rozmieszczeniem tekstur, drugi argument to ilosc wspolrzednych opisujacych punkt,
//trzeci to ilosc wspolrzednych opisujacych kolor (np. RGB i Alpha), czwarty to ilosc wspolrzednych tekstur, zwraca vao obiektu
GLuint Grafika3D::TworzObiekt(GLvoid* WspolnaTablica, size_t RozmiarTablicy, uint8_t IloscWspolrzednychPunktu, uint8_t IloscWspolrzednychKoloru, uint8_t IloscWspolrzednychTekstury)
{
    //Ilosc wszystkich danych potrzebnych do opisanie jednego punktu
    int SumaWspolrzednych = IloscWspolrzednychPunktu + IloscWspolrzednychKoloru + IloscWspolrzednychTekstury;

    vao.push_back(0); //Nowa tablica obiektu (mozna w niej umiescic bufor z punktami, kolore itp.)
    vbo.push_back(0); //Nowy bufor obiektu dla punktow obiektu

    //Tworzy nowy voa i ustawia na aktywna
    glGenVertexArrays(1, &vao[vao.size()-1]);
    glBindVertexArray(vao[vao.size()-1]);

    //Tworzy nowy bufor, ustawia na aktywny i umieszcza dane z punktami
    glGenBuffers(1, &vbo[vbo.size()-1]);
    glBindBuffer(GL_ARRAY_BUFFER, vbo[vbo.size()-1]);
    glBufferData(GL_ARRAY_BUFFER, RozmiarTablicy, WspolnaTablica, GL_STATIC_DRAW);


    //Ustawia atrybuty dla vertex shader
    //Punkty
    if(IloscWspolrzednychPunktu != 0)
    {
        if(IloscWspolrzednychPunktu >= 4)// || IloscWspolrzednychPunktu == 0)
        {
            IloscWspolrzednychPunktu = 4;
            glVertexAttribPointer(0, IloscWspolrzednychPunktu, GL_FLOAT, GL_FALSE, SumaWspolrzednych*sizeof(GLfloat), 0);

        }else
            glVertexAttribPointer(0, IloscWspolrzednychPunktu, GL_FLOAT, GL_FALSE, SumaWspolrzednych*sizeof(GLfloat), 0);

        glEnableVertexAttribArray(0);
    }


    //Kolor
    if(IloscWspolrzednychKoloru != 0)
    {
        if(IloscWspolrzednychKoloru >= 4)
        {
            IloscWspolrzednychKoloru = 4;
            glVertexAttribPointer(1, IloscWspolrzednychKoloru, GL_FLOAT, GL_FALSE, SumaWspolrzednych*sizeof(GLfloat), (GLvoid*)(IloscWspolrzednychPunktu * sizeof(GLfloat)));

        }else
            glVertexAttribPointer(1, IloscWspolrzednychKoloru, GL_FLOAT, GL_FALSE, SumaWspolrzednych*sizeof(GLfloat), (GLvoid*)(IloscWspolrzednychPunktu * sizeof(GLfloat)));

        glEnableVertexAttribArray(1);
    }


    //Tekstura
    if(IloscWspolrzednychTekstury != 0)
    {
        if(IloscWspolrzednychTekstury >= 3)// || IloscWspolrzednychTekstury == 0)
        {
            IloscWspolrzednychTekstury = 3;
            glVertexAttribPointer(2, IloscWspolrzednychTekstury, GL_FLOAT, GL_FALSE, SumaWspolrzednych*sizeof(GLfloat), (GLvoid*)((IloscWspolrzednychPunktu + IloscWspolrzednychKoloru) * sizeof(GLfloat)));

        }else
            glVertexAttribPointer(2, IloscWspolrzednychTekstury, GL_FLOAT, GL_FALSE, SumaWspolrzednych*sizeof(GLfloat), (GLvoid*)((IloscWspolrzednychPunktu + IloscWspolrzednychKoloru) * sizeof(GLfloat)));

        glEnableVertexAttribArray(2);
    }


    //Dezaktywuje voa
    glBindVertexArray(0);

    return vao[vao.size()-1];
}

//Metoda tworzy nowy obiekt rysowany statycznie oraz okresla jego kolor,
//pierwszy argument to tablica zepunktami, kolorami, i rozmieszczeniem tekstur, drugi argument to ilosc wspolrzednych opisujacych punkt,
//trzeci to ilosc wspolrzednych opisujacych kolor (np. RGB i Alpha), czwarty to ilosc wspolrzednych tekstur, zwraca vao obiektu
GLuint Grafika3D::TworzObiekt(const GLvoid* WspolnaTablica, size_t RozmiarTablicy, uint8_t IloscWspolrzednychPunktu, uint8_t IloscWspolrzednychKoloru, uint8_t IloscWspolrzednychTekstury)
{
    //Ilosc wszystkich danych potrzebnych do opisanie jednego punktu
    int SumaWspolrzednych = IloscWspolrzednychPunktu + IloscWspolrzednychKoloru + IloscWspolrzednychTekstury;

    vao.push_back(0); //Nowa tablica obiektu (mozna w niej umiescic bufor z punktami, kolore itp.)
    vbo.push_back(0); //Nowy bufor obiektu dla punktow obiektu

    //Tworzy nowy voa i ustawia na aktywna
    glGenVertexArrays(1, &vao[vao.size()-1]);
    glBindVertexArray(vao[vao.size()-1]);

    //Tworzy nowy bufor, ustawia na aktywny i umieszcza dane z punktami
    glGenBuffers(1, &vbo[vbo.size()-1]);
    glBindBuffer(GL_ARRAY_BUFFER, vbo[vbo.size()-1]);
    glBufferData(GL_ARRAY_BUFFER, RozmiarTablicy, WspolnaTablica, GL_STATIC_DRAW);


    //Ustawia atrybuty dla vertex shader
    //Punkty
    if(IloscWspolrzednychPunktu != 0)
    {
        if(IloscWspolrzednychPunktu >= 4)// || IloscWspolrzednychPunktu == 0)
        {
            IloscWspolrzednychPunktu = 4;
            glVertexAttribPointer(0, IloscWspolrzednychPunktu, GL_FLOAT, GL_FALSE, SumaWspolrzednych*sizeof(GLfloat), 0);

        }else
            glVertexAttribPointer(0, IloscWspolrzednychPunktu, GL_FLOAT, GL_FALSE, SumaWspolrzednych*sizeof(GLfloat), 0);

        glEnableVertexAttribArray(0);
    }


    //Kolor
    if(IloscWspolrzednychKoloru != 0)
    {
        if(IloscWspolrzednychKoloru >= 4)
        {
            IloscWspolrzednychKoloru = 4;
            glVertexAttribPointer(1, IloscWspolrzednychKoloru, GL_FLOAT, GL_FALSE, SumaWspolrzednych*sizeof(GLfloat), (GLvoid*)(IloscWspolrzednychPunktu * sizeof(GLfloat)));

        }else
            glVertexAttribPointer(1, IloscWspolrzednychKoloru, GL_FLOAT, GL_FALSE, SumaWspolrzednych*sizeof(GLfloat), (GLvoid*)(IloscWspolrzednychPunktu * sizeof(GLfloat)));

        glEnableVertexAttribArray(1);
    }


    //Tekstura
    if(IloscWspolrzednychTekstury != 0)
    {
        if(IloscWspolrzednychTekstury >= 3)// || IloscWspolrzednychTekstury == 0)
        {
            IloscWspolrzednychTekstury = 3;
            glVertexAttribPointer(2, IloscWspolrzednychTekstury, GL_FLOAT, GL_FALSE, SumaWspolrzednych*sizeof(GLfloat), (GLvoid*)((IloscWspolrzednychPunktu + IloscWspolrzednychKoloru) * sizeof(GLfloat)));

        }else
            glVertexAttribPointer(2, IloscWspolrzednychTekstury, GL_FLOAT, GL_FALSE, SumaWspolrzednych*sizeof(GLfloat), (GLvoid*)((IloscWspolrzednychPunktu + IloscWspolrzednychKoloru) * sizeof(GLfloat)));

        glEnableVertexAttribArray(2);
    }


    //Dezaktywuje voa
    glBindVertexArray(0);

    return vao[vao.size()-1];
}

//Metoda tworzy nowy obiekt rysowany statycznie oraz okresla jego kolor,
//pierwszy argument to tablica zepunktami, kolorami, i rozmieszczeniem tekstur, drugi argument to ilosc wspolrzednych opisujacych punkt,
//trzeci to ilosc wspolrzednych opisujacych kolor (np. RGB i Alpha), czwarty to ilosc wspolrzednych tekstur, zwraca vao obiektu
GLuint Grafika3D::TworzObiektD(GLvoid* WspolnaTablica, size_t RozmiarTablicy, uint8_t IloscWspolrzednychPunktu, uint8_t IloscWspolrzednychKoloru, uint8_t IloscWspolrzednychTekstury, GLuint* NoweVBO)
{
    //Ilosc wszystkich danych potrzebnych do opisanie jednego punktu
    int SumaWspolrzednych = IloscWspolrzednychPunktu + IloscWspolrzednychKoloru + IloscWspolrzednychTekstury;

    vao.push_back(0); //Nowa tablica obiektu (mozna w niej umiescic bufor z punktami, kolore itp.)
    vbo.push_back(0); //Nowy bufor obiektu dla punktow obiektu

    //Tworzy nowy voa i ustawia na aktywna
    glGenVertexArrays(1, &vao[vao.size()-1]);
    glBindVertexArray(vao[vao.size()-1]);
    //Tworzy nowy bufor, ustawia na aktywny i umieszcza dane z punktami
    glGenBuffers(1, &vbo[vbo.size()-1]);
    glBindBuffer(GL_ARRAY_BUFFER, vbo[vbo.size()-1]);
    glBufferData(GL_ARRAY_BUFFER, RozmiarTablicy, WspolnaTablica, GL_DYNAMIC_DRAW);

    //Ustawia atrybuty dla vertex shader
    //Punkty
    if(IloscWspolrzednychPunktu != 0)
    {
        if(IloscWspolrzednychPunktu >= 4)// || IloscWspolrzednychPunktu == 0)
        {
            IloscWspolrzednychPunktu = 4;
            glVertexAttribPointer(0, IloscWspolrzednychPunktu, GL_FLOAT, GL_FALSE, SumaWspolrzednych*sizeof(GLfloat), 0);

        }else
            glVertexAttribPointer(0, IloscWspolrzednychPunktu, GL_FLOAT, GL_FALSE, SumaWspolrzednych*sizeof(GLfloat), 0);

        glEnableVertexAttribArray(0);
    }


    //Kolor
    if(IloscWspolrzednychKoloru != 0)
    {
        if(IloscWspolrzednychKoloru >= 4)
        {
            IloscWspolrzednychKoloru = 4;
            glVertexAttribPointer(1, IloscWspolrzednychKoloru, GL_FLOAT, GL_FALSE, SumaWspolrzednych*sizeof(GLfloat), (GLvoid*)(IloscWspolrzednychPunktu * sizeof(GLfloat)));

        }else
            glVertexAttribPointer(1, IloscWspolrzednychKoloru, GL_FLOAT, GL_FALSE, SumaWspolrzednych*sizeof(GLfloat), (GLvoid*)(IloscWspolrzednychPunktu * sizeof(GLfloat)));

        glEnableVertexAttribArray(1);
    }


    //Tekstura
    if(IloscWspolrzednychTekstury != 0)
    {
        if(IloscWspolrzednychTekstury >= 3)// || IloscWspolrzednychTekstury == 0)
        {
            IloscWspolrzednychTekstury = 3;
            glVertexAttribPointer(2, IloscWspolrzednychTekstury, GL_FLOAT, GL_FALSE, SumaWspolrzednych*sizeof(GLfloat), (GLvoid*)((IloscWspolrzednychPunktu + IloscWspolrzednychKoloru) * sizeof(GLfloat)));

        }else
            glVertexAttribPointer(2, IloscWspolrzednychTekstury, GL_FLOAT, GL_FALSE, SumaWspolrzednych*sizeof(GLfloat), (GLvoid*)((IloscWspolrzednychPunktu + IloscWspolrzednychKoloru) * sizeof(GLfloat)));

        glEnableVertexAttribArray(2);
    }


    //Dezaktywuje voa
    glBindVertexArray(0);
    *NoweVBO = vbo[vbo.size()-1];
    return vao[vao.size()-1];
}


//Metoda dodaje nowy obiekt do sceny, bedzie on teraz wyswietlany, jako argumet podajemy
//vao obiektu ktory chcemy dodac oraz liczbe punktow z ktorych sklada sie nasz obiekt
//(potrzebne w procesie rysowania obiektu), metoda zwraca identyfikator obiektu na scenie
GLuint Grafika3D::DodajObiektDoSceny(GLuint vaoObiektu, GLuint IloscPunktowWObiekcie)
{
    ObiektNaScenie NowyObiekt;
    glm::mat4 Transformacja = glm::mat4(1.0f);
    GLuint IDNowegoObiektu;

    //Jezeli istnieje jakis obiekt na scenie to nastepny obiekt dostanie numer ID o 1 wiekszy
    if(ObiektyNaScenie.empty())
        IDNowegoObiektu = 0;
    else
        IDNowegoObiektu = ObiektyNaScenie[ObiektyNaScenie.size()-1].IDObiektu + 1;

    NowyObiekt.IDObiektu = IDNowegoObiektu;
    NowyObiekt.IDvaoObiektu = vaoObiektu;
    NowyObiekt.IDvboObiektu = 0;
    NowyObiekt.MacierzTransformacji = Transformacja;
    NowyObiekt.IloscPunktow = IloscPunktowWObiekcie;
    NowyObiekt.Tekstury.clear();
    NowyObiekt.RodzajObiektu = Graf3D_Plaszczyzna;
    NowyObiekt.fRysowanieObiektu = true;

    ObiektyNaScenie.push_back(NowyObiekt); //Dodaje nowy obiekt do sceny

    return IDNowegoObiektu; //Zwraca ID nowego obiektu
}

//Metoda dodaje nowy obiekt do sceny, bedzie on teraz wyswietlany, jako argumet podajemy
//vao obiektu ktory chcemy dodac, liczbe punktow z ktorych sklada sie nasz obiekt (potrzebne w procesie rysowania obiektu)
//oraz sposob w jaki bedzie rysowany obiekt (jako punkty, krawedzie albo plaszczyzny), metoda zwraca identyfikator obiektu na scenie
GLuint Grafika3D::DodajObiektDoSceny(GLuint vaoObiektu, GLuint IloscPunktowWObiekcie, Graf3D_RodzajObiektu RodzajObiektu)
{
    ObiektNaScenie NowyObiekt;
    glm::mat4 Transformacja = glm::mat4(1.0f);
    GLuint IDNowegoObiektu;

    //Jezeli istnieje jakis obiekt na scenie to nastepny obiekt dostanie numer ID o 1 wiekszy
    if(ObiektyNaScenie.empty())
        IDNowegoObiektu = 0;
    else
        IDNowegoObiektu = ObiektyNaScenie[ObiektyNaScenie.size()-1].IDObiektu + 1;

    NowyObiekt.IDObiektu = IDNowegoObiektu;
    NowyObiekt.IDvaoObiektu = vaoObiektu;
    NowyObiekt.IDvboObiektu = 0;
    NowyObiekt.MacierzTransformacji = Transformacja;
    NowyObiekt.IloscPunktow = IloscPunktowWObiekcie;
    NowyObiekt.Tekstury.clear();
    NowyObiekt.RodzajObiektu = RodzajObiektu;
    NowyObiekt.fRysowanieObiektu = true;

    ObiektyNaScenie.push_back(NowyObiekt); //Dodaje nowy obiekt do sceny

    return IDNowegoObiektu; //Zwraca ID nowego obiektu
}

//Metoda dodaje nowy obiekt do sceny, bedzie on teraz wyswietlany, jako argumet podajemy
//vao obiektu ktory chcemy dodac oraz liczbe punktow z ktorych sklada sie nasz obiekt
//(potrzebne w procesie rysowania obiektu), metoda zwraca identyfikator obiektu na scenie
GLuint Grafika3D::DodajObiektDoSceny(GLuint vaoObiektu, GLuint vboObiektu, GLuint IloscPunktowWObiekcie, Graf3D_RodzajObiektu RodzajObiektu)
{
    ObiektNaScenie NowyObiekt;
    glm::mat4 Transformacja = glm::mat4(1.0f);
    GLuint IDNowegoObiektu;

    //Jezeli istnieje jakis obiekt na scenie to nastepny obiekt dostanie numer ID o 1 wiekszy
    if(ObiektyNaScenie.empty())
        IDNowegoObiektu = 0;
    else
        IDNowegoObiektu = ObiektyNaScenie[ObiektyNaScenie.size()-1].IDObiektu + 1;

    NowyObiekt.IDObiektu = IDNowegoObiektu;
    NowyObiekt.IDvaoObiektu = vaoObiektu;
    NowyObiekt.IDvboObiektu = vboObiektu;
    NowyObiekt.MacierzTransformacji = Transformacja;
    NowyObiekt.IloscPunktow = IloscPunktowWObiekcie;
    NowyObiekt.Tekstury.clear();
    NowyObiekt.RodzajObiektu = RodzajObiektu;
    NowyObiekt.fRysowanieObiektu = true;

    ObiektyNaScenie.push_back(NowyObiekt); //Dodaje nowy obiekt do sceny

    return IDNowegoObiektu; //Zwraca ID nowego obiektu
}


//Metoda usuwa obiekt ze sceny, argument jaki przyjmuje to identyfikator obiektu do usuniecia
void Grafika3D::UsunObiektZeSceny(GLuint IDObiektu)
{
    //Petla przeszukuje vektor aby znalezc wskaznik na obiekt do usuniecia, jezeli sie nie uda wyswietla komunikat
    for(vector<ObiektNaScenie>::iterator i = ObiektyNaScenie.begin(); i != ObiektyNaScenie.end(); i++)
    {
        if(i->IDObiektu == IDObiektu)
        {
            ObiektyNaScenie.erase(i);
            return;
        }
    }

    cerr<<"Na scenie nie istnieje obiekt o ID: "<<IDObiektu<<" . Nie mozna wiec go usunac"<<endl;
}

//Metoda kopiujaca obiekt na scenie, argumentem jest identyfikator obiektu, zwraca
//identyfikator nowego obiektu
GLuint Grafika3D::KopiujObiekt(GLuint IDObiektu)
{
    ObiektNaScenie NowyObiekt;

    for(size_t i = 0; i < ObiektyNaScenie.size(); i++)
    {
        if(IDObiektu == ObiektyNaScenie[i].IDObiektu)
        {
            NowyObiekt.IDObiektu = ObiektyNaScenie[ObiektyNaScenie.size()-1].IDObiektu + 1;
            NowyObiekt.IDvaoObiektu = ObiektyNaScenie[i].IDvaoObiektu;
            NowyObiekt.MacierzTransformacji = ObiektyNaScenie[i].MacierzTransformacji;
            NowyObiekt.IloscPunktow = ObiektyNaScenie[i].IloscPunktow;
            NowyObiekt.Tekstury.assign(ObiektyNaScenie[i].Tekstury.begin(), ObiektyNaScenie[i].Tekstury.end());
            NowyObiekt.RodzajObiektu = ObiektyNaScenie[i].RodzajObiektu;
            NowyObiekt.IDvboObiektu = ObiektyNaScenie[i].IDvboObiektu;
            NowyObiekt.fRysowanieObiektu = ObiektyNaScenie[i].fRysowanieObiektu;
            ObiektyNaScenie.push_back(NowyObiekt);
            return NowyObiekt.IDObiektu;
        }
    }

    cerr<<"Nie istnieje obiekt o ID: "<<IDObiektu<<endl;
    return 0;
}

//Metoda pozwala dodac obiekt na scene bez koniecznosci wywolywania kolejnych metod,
//pierwszy argument to zbior puktow tego opiektu, drugi to rozmiar tablicy zawierajacej punkty,
//trzeci argument to ilosc wspolrzednych opisujacych punkt, czwarty liczbe punktow z ktorych sklada sie nasz obiekt
//(potrzebne w procesie rysowania obiektu), zwraca identyfikator obiektu.
//Aby dodawac kolejne takie same obiekty nalezy kopiowac pierwszy utworzony
GLuint Grafika3D::DodajObiekt(GLvoid* TablicaPunktow, size_t RozmiarTablicyPunktow, uint8_t IloscWspolrzednychPunktu, GLuint IloscPunktowWObiekcie)
{
    GLuint vao = TworzObiekt(TablicaPunktow, RozmiarTablicyPunktow, IloscWspolrzednychPunktu);
    return DodajObiektDoSceny(vao, IloscPunktowWObiekcie);
}

//Metoda pozwala dodac obiekt na scene bez koniecznosci wywolywania kolejnych metod,
//pierwszy argument to zbior puktow tego opiektu, drugi to rozmiar tablicy zawierajacej punkty,
//trzeci argument to ilosc wspolrzednych opisujacych punkt, czwarty liczbe punktow z ktorych sklada sie nasz obiekt
//(potrzebne w procesie rysowania obiektu), piaty sposob w jaki bedzie rysowany obiekt (jako punkty, krawedzie albo plaszczyzny),
//zwraca identyfikator obiektu. Aby dodawac kolejne takie same obiekty nalezy kopiowac pierwszy utworzony
GLuint Grafika3D::DodajObiekt(GLvoid* TablicaPunktow, size_t RozmiarTablicyPunktow, uint8_t IloscWspolrzednychPunktu, GLuint IloscPunktowWObiekcie,
                              Graf3D_RodzajObiektu RodzajObiektu)
{
    GLuint vao = TworzObiekt(TablicaPunktow, RozmiarTablicyPunktow, IloscWspolrzednychPunktu);
    return DodajObiektDoSceny(vao, IloscPunktowWObiekcie, RodzajObiektu);
}

//Metoda pozwala dodac obiekt na scene bez koniecznosci wywolywania kolejnych metod,
//pierwszy argument to zbior puktow tego opiektu, drugi to rozmiar tablicy zawierajacej punkty, trzeci argument to ilosc wspolrzednych opisujacych punkt,
//czwarty argument to tablica zawierajaca kolory odpowiednich punktu, piaty to rozmiar tablicy zawierajacej kolory,
//szosty ilosc elementow w wektorze opisujacym kolor (np. RGB i Alpha),siodmy liczbe punktow z ktorych sklada sie nasz obiekt
//(potrzebne w procesie rysowania obiektu), zwraca identyfikator obiektu. Aby dodawac kolejne takie same obiekty nalezy kopiowac pierwszy utworzony
GLuint Grafika3D::DodajObiekt(GLvoid* TablicaPunktow, size_t RozmiarTablicyPunktow, uint8_t IloscWspolrzednychPunktu, GLvoid* TablicaKolorow,
                                size_t RozmiarTablicyKolorow, uint8_t IloscWspolrzednychKoloru, GLuint IloscPunktowWObiekcie)
{
    GLuint vao = TworzObiekt(TablicaPunktow, RozmiarTablicyPunktow, IloscWspolrzednychPunktu, TablicaKolorow, RozmiarTablicyKolorow, IloscWspolrzednychKoloru);
    return DodajObiektDoSceny(vao, IloscPunktowWObiekcie);
}

//Metoda pozwala dodac obiekt na scene bez koniecznosci wywolywania kolejnych metod,
//pierwszy argument to zbior puktow tego opiektu, drugi to rozmiar tablicy zawierajacej punkty, trzeci argument to ilosc wspolrzednych opisujacych punkt,
//czwarty argument to tablica zawierajaca kolory odpowiednich punktu, piaty to rozmiar tablicy zawierajacej kolory,
//szosty ilosc elementow w wektorze opisujacym kolor (np. RGB i Alpha),siodmy liczbe punktow z ktorych sklada sie nasz obiekt
//(potrzebne w procesie rysowania obiektu), osmy sposob w jaki bedzie rysowany obiekt (jako punkty, krawedzie albo plaszczyzny),
//zwraca identyfikator obiektu. Aby dodawac kolejne takie same obiekty nalezy kopiowac pierwszy utworzony
GLuint Grafika3D::DodajObiekt(GLvoid* TablicaPunktow, size_t RozmiarTablicyPunktow, uint8_t IloscWspolrzednychPunktu, GLvoid* TablicaKolorow,
                                size_t RozmiarTablicyKolorow, uint8_t IloscWspolrzednychKoloru, GLuint IloscPunktowWObiekcie, Graf3D_RodzajObiektu RodzajObiektu)
{
    GLuint vao = TworzObiekt(TablicaPunktow, RozmiarTablicyPunktow, IloscWspolrzednychPunktu, TablicaKolorow, RozmiarTablicyKolorow, IloscWspolrzednychKoloru);
    return DodajObiektDoSceny(vao, IloscPunktowWObiekcie, RodzajObiektu);
}

//Metoda pozwala dodac obiekt na scene bez koniecznosci wywolywania kolejnych metod,
//Pierwszy argument to tablica zawierajaca wspolne dane punktow, koloru i tekstur, drugi to rozmiar tablicy zawierajacej dane,
//trzeci to ilosc wspolrzednych opisujacych punkt,czwart ilosc elementow w wektorze opisujacym kolor (np. RGB i Alpha),
//piaty to ilosc wspolrzednych dla tektury, szosty liczbe punktow z ktorych sklada sie nasz obiekt (potrzebne w procesie rysowania obiektu),
//zwraca identyfikator obiektu. Aby dodawac kolejne takie same obiekty nalezy kopiowac pierwszy utworzony
 GLuint Grafika3D::DodajObiekt(GLvoid* WspolnaTablica, size_t RozmiarTablicy, uint8_t IloscWspolrzednychPunktu, uint8_t IloscWspolrzednychKoloru,
                       uint8_t IloscWspolrzednychTekstury, GLuint IloscPunktowWObiekcie)
{
    GLuint vao = TworzObiekt(WspolnaTablica, RozmiarTablicy, IloscWspolrzednychPunktu, IloscWspolrzednychKoloru, IloscWspolrzednychTekstury);
    return DodajObiektDoSceny(vao, IloscPunktowWObiekcie);
}

//Metoda pozwala dodac obiekt na scene bez koniecznosci wywolywania kolejnych metod,
//Pierwszy argument to tablica zawierajaca wspolne dane punktow, koloru i tekstur, drugi to rozmiar tablicy zawierajacej dane,
//trzeci to ilosc wspolrzednych opisujacych punkt,czwart ilosc elementow w wektorze opisujacym kolor (np. RGB i Alpha),
//piaty to ilosc wspolrzednych dla tektury, szosty liczbe punktow z ktorych sklada sie nasz obiekt (potrzebne w procesie rysowania obiektu),
//siodmy sposob w jaki bedzie rysowany obiekt (jako punkty, krawedzie albo plaszczyzny), zwraca identyfikator obiektu.
//Aby dodawac kolejne takie same obiekty nalezy kopiowac pierwszy utworzony
 GLuint Grafika3D::DodajObiekt(GLvoid* WspolnaTablica, size_t RozmiarTablicy, uint8_t IloscWspolrzednychPunktu, uint8_t IloscWspolrzednychKoloru,
                       uint8_t IloscWspolrzednychTekstury, GLuint IloscPunktowWObiekcie, Graf3D_RodzajObiektu RodzajObiektu)
{
    GLuint vao = TworzObiekt(WspolnaTablica, RozmiarTablicy, IloscWspolrzednychPunktu, IloscWspolrzednychKoloru, IloscWspolrzednychTekstury);
    return DodajObiektDoSceny(vao, IloscPunktowWObiekcie, RodzajObiektu);
}

//Metoda pozwala dodac obiekt na scene bez koniecznosci wywolywania kolejnych metod,
//Pierwszy argument to tablica zawierajaca wspolne dane punktow, koloru i tekstur, drugi to rozmiar tablicy zawierajacej dane,
//trzeci to ilosc wspolrzednych opisujacych punkt,czwart ilosc elementow w wektorze opisujacym kolor (np. RGB i Alpha),
//piaty to ilosc wspolrzednych dla tektury, szosty liczbe punktow z ktorych sklada sie nasz obiekt (potrzebne w procesie rysowania obiektu),
//siodmy sposob w jaki bedzie rysowany obiekt (jako punkty, krawedzie albo plaszczyzny), zwraca identyfikator obiektu.
//Aby dodawac kolejne takie same obiekty nalezy kopiowac pierwszy utworzony
GLuint Grafika3D::DodajObiektD(GLvoid* WspolnaTablica, size_t RozmiarTablicy, uint8_t IloscWspolrzednychPunktu, uint8_t IloscWspolrzednychKoloru,
                    uint8_t IloscWspolrzednychTekstury, GLuint IloscPunktowWObiekcie, Graf3D_RodzajObiektu RodzajObiektu)
{
    GLuint vbo;
    GLuint vao = TworzObiektD(WspolnaTablica, RozmiarTablicy, IloscWspolrzednychPunktu, IloscWspolrzednychKoloru, IloscWspolrzednychTekstury, &vbo);
    return DodajObiektDoSceny(vao, vbo, IloscPunktowWObiekcie, RodzajObiektu);
}


//Metoda pozwala dodac obiekt na scene bez koniecznosci wywolywania kolejnych metod,
//Pierwszy argument to tablica zawierajaca wspolne dane punktow, koloru i tekstur, drugi to rozmiar tablicy zawierajacej dane,
//trzeci to ilosc wspolrzednych opisujacych punkt,czwart ilosc elementow w wektorze opisujacym kolor (np. RGB i Alpha),
//piaty to ilosc wspolrzednych dla tektury, szosty liczbe punktow z ktorych sklada sie nasz obiekt (potrzebne w procesie rysowania obiektu),
//siodmy sposob w jaki bedzie rysowany obiekt (jako punkty, krawedzie albo plaszczyzny), zwraca identyfikator obiektu.
//Aby dodawac kolejne takie same obiekty nalezy kopiowac pierwszy utworzony
GLuint Grafika3D::DodajObiekt(const GLvoid* WspolnaTablica, size_t RozmiarTablicy, uint8_t IloscWspolrzednychPunktu, uint8_t IloscWspolrzednychKoloru,
                    uint8_t IloscWspolrzednychTekstury, GLuint IloscPunktowWObiekcie, Graf3D_RodzajObiektu RodzajObiektu)
{
 GLuint vao = TworzObiekt(WspolnaTablica, RozmiarTablicy, IloscWspolrzednychPunktu, IloscWspolrzednychKoloru, IloscWspolrzednychTekstury);
 return DodajObiektDoSceny(vao, IloscPunktowWObiekcie, RodzajObiektu);
}

void Grafika3D::UaktualniDaneObiektu(GLuint IDObiektu, GLintptr Offset, GLsizeiptr Rozmiar, const GLvoid *ZmienioneDane)
{
    //Szukanie numeru obiektu o podanym ID
    size_t i;
    for(i = 0; i < ObiektyNaScenie.size(); i++)
    {
        if(IDObiektu == ObiektyNaScenie[i].IDObiektu)
            break;
    }

    //Jezeli nie znaleziono obiektu to wyswietlany jest odpowiedni komunikat
    if(i == ObiektyNaScenie.size())
    {
        cerr<<"Na scenie nie ma obiektu o ID: "<<IDObiektu<<endl;
        return;
    }


    glBindVertexArray(ObiektyNaScenie[i].IDvaoObiektu);

    glBindBuffer(GL_ARRAY_BUFFER, ObiektyNaScenie[i].IDvboObiektu);

    glBufferSubData(GL_ARRAY_BUFFER, Offset, Rozmiar, ZmienioneDane);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}


//Metoda pozwalajaca przesunac obiekt o wektor, pierwszy argument to identyfikator obiektu na scenie,
//drugi argument to wektor przesuniecia
void Grafika3D::PrzesunObiekt(GLuint IDObiektu, glm::vec3 Przesuniecie)
{
    //Szukanie numeru obiektu o podanym ID
    size_t i;
    for(i = 0; i < ObiektyNaScenie.size(); i++)
    {
        if(IDObiektu == ObiektyNaScenie[i].IDObiektu)
            break;
    }

    //Jezeli nie znaleziono obiektu to wyswietlany jest odpowiedni komunikat
    if(i == ObiektyNaScenie.size())
    {
        cerr<<"Na scenie nie ma obiektu o ID: "<<IDObiektu<<endl;
        return;
    }


    //Do macierzy transformacji obiektu dodawany jest wektor przesuniecia
    ObiektyNaScenie[i].MacierzTransformacji = glm::translate(ObiektyNaScenie[i].MacierzTransformacji, Przesuniecie);

}

//Metoda pozwalajaca obrucic obiekt, pierwszy argument to identyfikator obiektu, drugi to kat obrotu,
//trzeci argument to os obrotu obiektu
void Grafika3D::ObrocObiekt(GLuint IDObiektu, GLfloat KatObrotu, glm::vec3 OsObrotu)
{
    //Szukanie numeru obiektu o podanym ID
    size_t i;
    for(i = 0; i < ObiektyNaScenie.size(); i++)
    {
        if(IDObiektu == ObiektyNaScenie[i].IDObiektu)
            break;
    }

    //Jezeli nie znaleziono obiektu to wyswietlany jest odpowiedni komunikat
    if(i == ObiektyNaScenie.size())
    {
        cerr<<"Na scenie nie ma obiektu o ID: "<<IDObiektu<<endl;
        return;
    }

    //Dodawanie obrotu do macierzy transformacji
    ObiektyNaScenie[i].MacierzTransformacji = glm::rotate(ObiektyNaScenie[i].MacierzTransformacji, KatObrotu, OsObrotu);
}

//Metoda pozwalajaca przeskalowac obiekt, pierwszy argument to identyfikator obiektu, drugi to wektor
//z kolejnymi wartoscimai skali wzdluz kolejnych osi
void Grafika3D::PrzeskalujObiekt(GLuint IDObiektu, glm::vec3 WektorSkali)
{
    //Szukanie numeru obiektu o podanym ID
    size_t i;
    for(i = 0; i < ObiektyNaScenie.size(); i++)
    {
        if(IDObiektu == ObiektyNaScenie[i].IDObiektu)
            break;
    }

    //Jezeli nie znaleziono obiektu to wyswietlany jest odpowiedni komunikat
    if(i == ObiektyNaScenie.size())
    {
        cerr<<"Na scenie nie ma obiektu o ID: "<<IDObiektu<<endl;
        return;
    }

    //Skaluje obiekt modyfikujac macierz transformacji
    ObiektyNaScenie[i].MacierzTransformacji = glm::scale(ObiektyNaScenie[i].MacierzTransformacji, WektorSkali);
}

void Grafika3D::UstawMacierzTransformacji(GLuint IDObiektu, glm::mat4 NowaMacierzTransformacji)
{
    size_t i;
    for(i = 0; i < ObiektyNaScenie.size(); i++)
    {
        if(IDObiektu == ObiektyNaScenie[i].IDObiektu)
            break;
    }

    //Jezeli nie znaleziono obiektu to wyswietlany jest odpowiedni komunikat
    if(i == ObiektyNaScenie.size())
    {
        cerr<<"Na scenie nie ma obiektu o ID: "<<IDObiektu<<endl;
        return;
    }

    ObiektyNaScenie[i].MacierzTransformacji = NowaMacierzTransformacji;
}

//Ustawia perspektywe ortogonalna (prostokatna), parametry to lewa i prawa, dolna i gorna
//krawedz przedniej sciany szescianu oraz odleglosc przedniej i tylniej scian
void Grafika3D::UstawRzutowanieOrtogonalne(GLfloat Lewa, GLfloat Prawa, GLfloat Dol, GLfloat Gora, GLfloat Przod, GLfloat Tyl)
{
    glm::mat4 rzut = glm::mat4(1.0f);
    rzut = glm::ortho(Lewa, Prawa, Dol, Gora, Przod, Tyl);

    shader.PrzekazMacierz4x4("rzutowanie", rzut);
}

//Ustawienie rzutowania perspektywicznego, parametry to kat miedzy liniami od kamery,
//stosunek miedzy szerokoscia, a wysokoscia ekranu oraz odleglosc przedniej i tylniej scian
void Grafika3D::UstawRzutowaniePerspektywiczne(GLfloat Kat, GLfloat ProporcjeEkranu, GLfloat Przod, GLfloat Tyl)
{
    glm::mat4 rzut = glm::mat4(1.0f);
    rzut = glm::perspective(Kat, ProporcjeEkranu, Przod, Tyl);

    shader.PrzekazMacierz4x4("rzutowanie", rzut);
}

//Ustawia kamere na scenie, pierwszy parametr to wspolrzedne globalne kamery na scenie,
//drugi to punkt, na ktory "patrzy" kamera, trzeci to kierunek osi Z kamery
void Grafika3D::UstawKamere(glm::vec3 Pozycja, glm::vec3 Cel, glm::vec3 OsZ)
{
    glm::mat4 widok;

    PozycjaKamery = Pozycja;
    CelKamery = Cel;
    OsZKamery = OsZ;

    widok = glm::lookAt(Pozycja, Cel, OsZ); //Tworzy macierz pozwalajaca przeksztalcic scene z przeszczeni globalnej do przestrzeni widoku

    shader.PrzekazMacierz4x4("widok", widok);
}

//Metoda pozwala wczytac obraz i stworzyc z niego teksture 2D, pierwszy argument to scierzka do obrazu,
//drugi to sposob "zawijania" tekstur na brzegach, trzeci to sposob filtracji tekstury by okreslic kolor piksela,
//zwraca identyfikator tekstury
GLuint Grafika3D::GenerujTeksture2D(const char* NazwaObrazka, GLint ZawijanieTekstury, GLint FiltracjaTekstury)
{
    GLuint NowaTekstura;
    int szerokosc, wysokosc;

    //generuje teksture
    glGenTextures(1, &NowaTekstura);
    glBindTexture(GL_TEXTURE_2D, NowaTekstura);

    //Ustawia parametry zawijania tekstury dla osi X(S) i Y(T)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, ZawijanieTekstury);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, ZawijanieTekstury);

    //Ustawienie parametrow filtru tekstury
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, FiltracjaTekstury);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, FiltracjaTekstury);

    //Wczytanie obrazka, stworzenie z niego tekstury 2D, stworzenie mipmaps i zwolnienie pamieci po obrazku
    unsigned char* Obrazek = SOIL_load_image(NazwaObrazka, &szerokosc, &wysokosc, nullptr, SOIL_LOAD_RGB);
    cout<<SOIL_last_result()<<endl;
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, szerokosc, wysokosc, 0, GL_RGB, GL_UNSIGNED_BYTE, Obrazek);
    glGenerateMipmap(GL_TEXTURE_2D);
    SOIL_free_image_data(Obrazek);

    glBindTexture(GL_TEXTURE_2D, 0);

    return NowaTekstura;

}

//Powduje nalozenie tekstury na obiekt
//Pierwszy argument to identyfikator tekstury, drugi identyfikator obiektu, na ktorego nakladamy teksture
void Grafika3D::DodajTekstureDoObiektu(GLuint IDTekstury, GLuint IDObiektu)
{
    //Szukanie numeru obiektu o podanym ID
    size_t i;
    for(i = 0; i < ObiektyNaScenie.size(); i++)
    {
        if(IDObiektu == ObiektyNaScenie[i].IDObiektu)
            break;
    }

    //Jezeli nie znaleziono obiektu to wyswietlany jest odpowiedni komunikat
    if(i == ObiektyNaScenie.size())
    {
        cerr<<"Na scenie nie ma obiektu o ID: "<<IDObiektu<<endl;
        return;
    }

    ObiektyNaScenie[i].Tekstury.push_back(IDTekstury);
}

//Metoda sluzaca do usuwania tekstury z obiektu,
//Pierwszy argument to identyfikator tekstury,drugi to identyfikator obiektu, z ktorego usuwamy teksture
void Grafika3D::UsunTekstureZObiektu(GLuint IDTekstury, GLuint IDObiektu)
{
    //Szukanie numeru obiektu o podanym ID
    size_t i;
    for(i = 0; i < ObiektyNaScenie.size(); i++)
    {
        if(IDObiektu == ObiektyNaScenie[i].IDObiektu)
            break;
    }

    //Jezeli nie znaleziono obiektu to wyswietlany jest odpowiedni komunikat
    if(i == ObiektyNaScenie.size())
    {
        cerr<<"Na scenie nie ma obiektu o ID: "<<IDObiektu<<endl;
        return;
    }

    //Petla przeszukuje vektor aby znalezc wskaznik na teksture do usuniecia, jezeli sie nie uda wyswietla komunikat
    for(vector<GLuint>::iterator j = ObiektyNaScenie[i].Tekstury.begin(); j != ObiektyNaScenie[i].Tekstury.end(); j++)
    {
        if(*j == IDTekstury)
        {
            ObiektyNaScenie[i].Tekstury.erase(j);
            return;
        }
    }

    cerr<<"Na scenie nie istnieje obiekt o ID: "<<IDObiektu<<" . Nie mozna wiec go usunac"<<endl;

}

//Metoda wczytuje czciake z pliku i zapisuje jej znaki jako teksture
//Pierwszy argument to scierzka do czcionki, drugi to rozmiar czcionki,
//trzeci to liczba znakow jaka chcemy wczytac ze czcionki
void Grafika3D::WczytajCzciake(const char* ScierzkaCzcioki, GLuint RozmiarCzcionki, GLuint IloscZnakowDoWczytania, GLuint NumerAtrybutuShadera)
{
    FT_Library ft;
    ZnakNaScenie Znak;
    GLuint Tekstura;

    //Inicjuje biblioteke FreeType
    if(FT_Init_FreeType(&ft))
        cerr<<"Blad::FREETYPE: Nie mozna zainicjowac biblioteki FreeType"<<endl;

    //Wczytuje nowa czcionke
    FT_Face Rodzaj;
    if(FT_New_Face(ft, ScierzkaCzcioki, 0, &Rodzaj))
        cerr<<"Blad::FREETYPE: Nie udalo sie zaladowac czcionki z pliku: "<<ScierzkaCzcioki<<endl;

    //Ustawia rozmiar czcionki
    FT_Set_Pixel_Sizes(Rodzaj, 0, RozmiarCzcionki);

    //Wylacza w openGL wymuszenie 4-byte wyrownania dla tekstury (jej rozmiar jest wielokrotnoscia 4-bytow)
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    for(GLuint z = 0; z < IloscZnakowDoWczytania; z++)
    {
        //Wczytujemy znak z czcionki, tworzymy dla niego teksture i zapisujemy do kontenera z odpowiednimi danymi
        if(FT_Load_Char(Rodzaj, z, FT_LOAD_RENDER))
        {
            cerr<<"Blad::FREETYPE: Nie udalo sie zaladowac Glifu numer: "<<z<<endl;
            continue;
        }

        //Tworzenie tekstury
        glGenTextures(1, &Tekstura);
        glBindTexture(GL_TEXTURE_2D, Tekstura);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, Rodzaj->glyph->bitmap.width, Rodzaj->glyph->bitmap.rows, 0, GL_RED, GL_UNSIGNED_BYTE, Rodzaj->glyph->bitmap.buffer);

        //ustawianie parametrow
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        //Zapisuje znaki w kontenerze
        Znak.IDTekstury = Tekstura;
        Znak.Rozmiar = glm::ivec2(Rodzaj->glyph->bitmap.width, Rodzaj->glyph->bitmap.rows);
        Znak.Przesuniecie = glm::ivec2(Rodzaj->glyph->bitmap_left, Rodzaj->glyph->bitmap_top);
        Znak.Odstep = Rodzaj->glyph->advance.x;

        ZnakiNaScenie.insert(pair<GLchar, ZnakNaScenie>(z, Znak));
    }

    glBindTexture(GL_TEXTURE_2D, 0);

    //Czyszczenie po FreeType
    FT_Done_Face(Rodzaj);
    FT_Done_FreeType(ft);

    TworzObiektCzcionka(NumerAtrybutuShadera);

    shaderTekstu.Init();
    if(!shaderTekstu.WczytajShader("VertTekst.glsl", GL_VERTEX_SHADER))
        return;

    if(!shaderTekstu.WczytajShader("FragTekst.glsl", GL_FRAGMENT_SHADER))
        return;

    if(!shaderTekstu.LinkowanieShader())
        return;


}

//Metoda tworzy buffor pozwalajacy dynamicznie rysowac znaki czcionki
//Argumentem jest liczba pod jaka w shaderze beda dostepne przekazywane dane,
void Grafika3D::TworzObiektCzcionka(GLuint NumerAtrybutuShadera)
{
    //Generuje VAO i VBo
    glGenVertexArrays(1, &VAOCzcionki);
    glGenBuffers(1, &VBOCzcionki);

    glBindVertexArray(VAOCzcionki);
    glBindBuffer(GL_ARRAY_BUFFER, VBOCzcionki);

    //Tworzy bufor o rozmiarze 6 * 4 * rozmiar GLfloat, wynika to z faktu ze prostokat opisany jest za pomoca 6 wierzcholkow
    //(2 trojkaty) oraz kazdy z wierzcholkow posiada 4 wspolrzedne, 2 polorzenia (x i y) i 2 rozmieszczenia tekstury
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, NULL, GL_DYNAMIC_DRAW);

    glEnableVertexAttribArray(NumerAtrybutuShadera);
    glVertexAttribPointer(NumerAtrybutuShadera, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);


    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

}

//Metoda pozwala wyswietlic napis, pierwszy argument to napis do wyswietlenia,
//drugi i trzeci to wspolrzedne napisu, czwarty to jego skala, piaty to wektor koloru
void Grafika3D::RenderujTekst(GLint WysokoscOkna, GLint SzerokoscOkna, string Napis, GLfloat x, GLfloat y, GLfloat Skala, glm::vec3 Kolor)
{
    glEnable(GL_BLEND);

    ZnakNaScenie Znak;
    GLfloat xpos, ypos;
    GLfloat szerokosc, wysokosc;
    GLfloat TablicaDanych[6][4];
    glm::mat4 rzut = glm::mat4(1.0f);

    //Pobiera szerokosc oraz wysokosc okna
    //SDL_GetWindowSize(GlowneOkno, &szerokosc_okna, &wysokosc_okna);

    shaderTekstu.Uzyjprogramu();

    //Ustawia macierz rzutowania na rzutowanie ortogonalne tak aby srodek okna odpowiadal punktowi (0,0)
    rzut = glm::ortho(-(SzerokoscOkna/2.0f), (SzerokoscOkna/2.0f), -(WysokoscOkna/2.0f), (WysokoscOkna/2.0f));
    shaderTekstu.PrzekazMacierz4x4("rzutowanie", rzut);

    //Wysyla kolory do shadera do zmiennej KolorTekstu
    glUniform3f(glGetUniformLocation(shaderTekstu.ZwrocProgramID(), "KolorTekstu"), Kolor.x, Kolor.y, Kolor.z);
    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(VAOCzcionki);

    //Odczytuje z napisu kolejne znaki, wypelnia dla nich tablice z danymi o polorzeniu kolejnych wierzcholkow kwadratu litery
    //oraz ustawienie tekstury na tym kwadracie
    string::const_iterator z;
    for(z = Napis.begin(); z != Napis.end(); z++)
    {
        Znak = ZnakiNaScenie[*z];

        xpos = x + Znak.Przesuniecie.x * Skala;
        ypos = y - (Znak.Rozmiar.y - Znak.Przesuniecie.y) * Skala;
        szerokosc = Znak.Rozmiar.x * Skala;
        wysokosc = Znak.Rozmiar.y * Skala;

        TablicaDanych[0][0] = xpos;             TablicaDanych[0][1] = ypos + wysokosc; TablicaDanych[0][2] = 0.0f; TablicaDanych[0][3] = 0.0f;
        TablicaDanych[1][0] = xpos;             TablicaDanych[1][1] = ypos;            TablicaDanych[1][2] = 0.0f; TablicaDanych[1][3] = 1.0f;
        TablicaDanych[2][0] = xpos + szerokosc; TablicaDanych[2][1] = ypos;            TablicaDanych[2][2] = 1.0f; TablicaDanych[2][3] = 1.0f;

        TablicaDanych[3][0] = xpos;             TablicaDanych[3][1] = ypos + wysokosc; TablicaDanych[3][2] = 0.0f; TablicaDanych[3][3] = 0.0f;
        TablicaDanych[4][0] = xpos + szerokosc; TablicaDanych[4][1] = ypos;            TablicaDanych[4][2] = 1.0f; TablicaDanych[4][3] = 1.0f;
        TablicaDanych[5][0] = xpos + szerokosc; TablicaDanych[5][1] = ypos + wysokosc; TablicaDanych[5][2] = 1.0f; TablicaDanych[5][3] = 0.0f;

        glBindTexture(GL_TEXTURE_2D, Znak.IDTekstury);
        glBindBuffer(GL_ARRAY_BUFFER, VBOCzcionki);

        //Przekazuje dane
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(TablicaDanych), TablicaDanych);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        //Rysuje litere
        glDrawArrays(GL_TRIANGLES, 0, 6);

        //Przesuwa kursor wzdluz osi x tak aby mozna zapisac nastepna litere
        x += (Znak.Odstep >> 6) * Skala;
    }

    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);

    //glFlush();
    //SDL_GL_SwapWindow(GlowneOkno);

    shader.Uzyjprogramu();
}

//Metoda pozwala dodac napis do sceny,
//Pierwszy argument to tresc napisu, drugi  itrzeci to jego polorzenie, czwarty to skala,
//piaty to wektor koloru (RGB), zwraca identyfikator napisu
GLuint Grafika3D::DodajNapisDoSceny(string Napis, GLfloat x, GLfloat y, GLfloat Skala, glm::vec3 Kolor)
{
    //Tworzy nowy napis
    NapisNaScenie NowyNapis;

    //Jezeli nie ma zadnych napisow na scenie to nowy napis dostaje ID = 0
    if(NapisyNaScenie.empty())
        NowyNapis.IDNapisu = 0;
    else
        NowyNapis.IDNapisu = NapisyNaScenie[NapisyNaScenie.size()-1].IDNapisu + 1;

    NowyNapis.Napis = Napis;
    NowyNapis.X = x;
    NowyNapis.Y = y;
    NowyNapis.Skala = Skala;
    NowyNapis.KolorNapisu = Kolor;

    //Dodaje napis do kontenera napisow na scenie
    NapisyNaScenie.push_back(NowyNapis);

    return NowyNapis.IDNapisu;
}

//Metoda usuwa napis ze sceny, pobiera identyfikator napisu
void Grafika3D::UsunNapisZeSceny(GLuint IDNapisu)
{
    //Wyszukuje napisu o odpowiednim ID z kontenera i go usuwa
    for(vector<NapisNaScenie>::iterator i = NapisyNaScenie.begin(); i != NapisyNaScenie.end(); i++)
    {
        if(i->IDNapisu == IDNapisu)
        {
            NapisyNaScenie.erase(i);
            return;
        }
    }

    cerr<<"Na scenie nie istnieje napis o ID: "<<IDNapisu<<" . Nie mozna wiec go usunac"<<endl;
}

//Metoda usuwa napis ze sceny, pobiera tresc napisu do usuniecia
void Grafika3D::UsunNapisZeSceny(string Napis)
{
    //Wyszukuje napisu o odpowiednim ID z kontenera i go usuwa
    for(vector<NapisNaScenie>::iterator i = NapisyNaScenie.begin(); i != NapisyNaScenie.end(); i++)
    {
        if(i->Napis == Napis)
        {
            NapisyNaScenie.erase(i);
            return;
        }
    }

    cerr<<"Na scenie nie istnieje napis o tresci: "<<Napis<<" . Nie mozna wiec go usunac"<<endl;
}

void Grafika3D::RysujObiekt(GLuint IDObiektu, GLboolean Rysuj)
{
    //Szukanie numeru obiektu o podanym ID
    size_t i;
    for(i = 0; i < ObiektyNaScenie.size(); i++)
    {
        if(IDObiektu == ObiektyNaScenie[i].IDObiektu)
            break;
    }

    //Jezeli nie znaleziono obiektu to wyswietlany jest odpowiedni komunikat
    if(i == ObiektyNaScenie.size())
    {
        cerr<<"Na scenie nie ma obiektu o ID: "<<IDObiektu<<endl;
        return;
    }

    ObiektyNaScenie[i].fRysowanieObiektu = Rysuj;
}

void Grafika3D::PrzekazIntaDoShedera(const char *NazwaWShederze, int Wartosc)
{
    shader.PrzekazInt(NazwaWShederze, Wartosc);
}

void Grafika3D::Rysuj(GLint WysokoscOkna, GLint SzerokoscOkna)
{
    string TeksturyWShader = "Tekstura";
    string Tymczasowy = "";

    glClearColor(0.0, 0.0, 0.0, 1.0); //Ustawia kolor tla
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //Czysci tlo
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);

    for(GLuint i = 0; i < ObiektyNaScenie.size(); i++)
    {
        if(ObiektyNaScenie[i].fRysowanieObiektu == false) continue;

        glBindVertexArray(ObiektyNaScenie[i].IDvaoObiektu);
        shader.PrzekazMacierz4x4("transformacja", ObiektyNaScenie[i].MacierzTransformacji);

        //Aktywowanie tekstur
        if(ObiektyNaScenie[i].Tekstury.size() > 0)
        {
           for(GLuint j = 0; j < ObiektyNaScenie[i].Tekstury.size(); j++)
            {
                if(j < GL_MAX_TEXTURE_UNITS - GL_TEXTURE0)
                {
                    glActiveTexture(GL_TEXTURE0 + j); // Aktywuje teksture o odpowiednim numerze

                    //Tworze nazwe zmiennej dla tekstury w shaderze
                    sprintf((char*)Tymczasowy.c_str(), "%d", j);
                    Tymczasowy = TeksturyWShader + Tymczasowy.c_str();;
                    //Przesyla teksture do shadera
                    glBindTexture(GL_TEXTURE_2D, ObiektyNaScenie[i].Tekstury[j]);

                    glUniform1i(glGetUniformLocation(shader.ZwrocProgramID(), Tymczasowy.c_str()), j);

                    Tymczasowy = ""; //Czysci napis tymczasowy


                }else
                {
                    cerr<<"Obiekt o ID: "<<ObiektyNaScenie[i].IDObiektu<<" posiada za duzo tekstur !"<<endl;
                    break;
                }
            }
        }

        //glDrawElements(GL_LINE_STRIP, 5, GL_UNSIGNED_BYTE, 0);
        switch(ObiektyNaScenie[i].RodzajObiektu)
        {
            case Graf3D_Punkt:
                glPointSize(10);
                glDrawArrays(GL_POINTS, 0 ,ObiektyNaScenie[i].IloscPunktow);
            break;

            case Graf3D_Krawedz:
                glDrawArrays(GL_LINES, 0 ,ObiektyNaScenie[i].IloscPunktow);
            break;

            case Graf3D_PolaczonaKrawedz:
                glDrawArrays(GL_LINE_STRIP, 0 ,ObiektyNaScenie[i].IloscPunktow);
            break;

            case Graf3D_Plaszczyzna:
                glDrawArrays(GL_TRIANGLES, 0 ,ObiektyNaScenie[i].IloscPunktow);
            break;

            case Graf3D_TasmaCzworokatow:
                glDrawArrays(GL_QUAD_STRIP, 0 ,ObiektyNaScenie[i].IloscPunktow);
            break;

            default:
            break;
        }

        glBindTexture(GL_TEXTURE_2D, 0);
        glBindVertexArray(0);
    }

    for(size_t j = 0; j < NapisyNaScenie.size(); j++)
    {
        RenderujTekst(WysokoscOkna, SzerokoscOkna, NapisyNaScenie[j].Napis, NapisyNaScenie[j].X, NapisyNaScenie[j].Y, NapisyNaScenie[j].Skala, NapisyNaScenie[j].KolorNapisu);
    }


    glFlush();

}
