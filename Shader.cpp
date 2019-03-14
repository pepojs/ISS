#include "Shader.h"

uint8_t SprawdzGLError1()
{
    GLenum err;

    if((err = glGetError()) != GL_NO_ERROR)
    {
        std::cerr<<"Wystapil blad : "<<err<<" "<<gluErrorString(err)<<std::endl;
        return 1;
    }

    return 0;
}
string Shader::WczytajPlik(const char* plik)
{
    stringstream buff;
    fstream temp(plik, ios::in);
    string konteksPlik;

    if(temp.good())
    {
        buff<<temp.rdbuf();
        konteksPlik = buff.str();

        return konteksPlik;

    }else
    {
        cerr<<"Nie udalo otworzyc sie pliku :"<<plik<<endl;
        return 0;
    }

}

int Shader::StworzShader(const char* nazwaPliku, GLenum shaderTyp)
{
    string s = WczytajPlik(nazwaPliku);

    if(s.length() != 0)
    {
        //Zamiana z const char* (c_str() daje const char*) na char*
        char* code = const_cast<char*>(s.c_str());
        int32_t rozmiar = s.length();

        //Tworzy pusty uchwyt do vertex shader
        int shaderID = glCreateShader(shaderTyp);

        //Wysyla kod zrodlowy vertex shader do OpenGL
        glShaderSource(shaderID, 1, &code, &rozmiar);

        //Zwraca uchwyt
        return shaderID;

    }else
    {
        cerr<<"Blad wczytania z pliku : "<<nazwaPliku<<endl;
        return -1;
    }
}

void Shader::WypiszBledyKompilacji(int32_t shaderID)
{
    // Znajduje dlugosc log info od shadera
    int maksDlugosc;
    glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &maksDlugosc);

    //Pobiera log info
    char* shaderInfoLog = new char[maksDlugosc];
    glGetShaderInfoLog(shaderID, maksDlugosc, &maksDlugosc, shaderInfoLog);

    string log = shaderInfoLog;

    if(log.length())
    {
        cout<<"______________________________________"<<endl;
        cout<<shaderInfoLog<<endl;
        cout<<"______________________________________"<<endl<<endl;
    }

    delete shaderInfoLog;
}

uint8_t Shader::KompilujShader(int shaderID)
{
    //Kompiluje vertex shader
    glCompileShader(shaderID);

    //Sprawdza czy shader byl skompilowany
    int skompilowany = 0;
    glGetShaderiv(shaderID, GL_COMPILE_STATUS, &skompilowany);

    WypiszBledyKompilacji(shaderID);

    //Zwraca 0 jezeli kompilacja sie nie udala
    return (skompilowany != 0);
}

uint8_t Shader::LinkowanieShader()
{
    glLinkProgram(shaderProgram);

    int zlinkowany;
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &zlinkowany);

    if(zlinkowany == 0)
        WypiszBledyLinkowania(shaderProgram);

    return (zlinkowany != 0);
}

void Shader::WypiszBledyLinkowania(int32_t shaderID)
{
    cout<<"______________________________________"<<endl;
    cout<<"Nieudane linkowanie Shader-a : "<<endl;

    // Znajduje dlugosc log info od shadera
    int maksDlugosc;
    glGetProgramiv(shaderID, GL_INFO_LOG_LENGTH, &maksDlugosc);

     //Pobiera log info
    char* shaderProgramInfoLog = new char[maksDlugosc];
    glGetProgramInfoLog(shaderProgram, maksDlugosc, &maksDlugosc, shaderProgramInfoLog);

    cout<<"Blad linkowania : "<<shaderProgramInfoLog<<endl<<endl;

    cout<<"Nacisni przycisk aby kontynuowac. "<<endl;
    getchar();

    delete shaderProgramInfoLog;

}

uint8_t Shader::Init()
{
    //Tworzy shader program
    shaderProgram = glCreateProgram();

    return (shaderProgram != 0);
}

uint8_t Shader::WczytajShader(const char* nazwaPliku, GLenum shaderTyp)
{
    cout<<"Wczytuje Shader: "<<nazwaPliku<<endl;

    int shaderID = StworzShader(nazwaPliku, shaderTyp);

    if(KompilujShader(shaderID) && shaderProgram != 0)
    {
        //Dolacza shader do shader program
        glAttachShader(shaderProgram, shaderID);
        shaderIDs.push_back(shaderID);

        return 1;
    }

    return 0;
}

void Shader::Sprzatanie()
{
    glUseProgram(0);

    for(unsigned int i = 0; i < shaderIDs.size(); i++)
        glDetachShader(shaderProgram, shaderIDs[i]);

    glDeleteProgram(shaderProgram);

    for(unsigned int i = 0; i < shaderIDs.size(); i++)
        glDeleteShader(shaderIDs[i]);
}

void Shader::Uzyjprogramu()
{
    //Laduje shader do sterty renderingu
    glUseProgram(shaderProgram);
}

//Pozwala z programu przekazac macierz 4x4 do Shedera jako uniform mat4 NazwaWShederze
void Shader::PrzekazMacierz4x4(const char* NazwaWShederze, mat4 macierz)
{
    if(shaderProgram != 0)
    {
        //Sprawdza polozenie naszej zmiennej w shederze i przekazuje dane w postaci macierzy
        GLuint PozycjaMacierzy = glGetUniformLocation(shaderProgram, NazwaWShederze);
        glUniformMatrix4fv(PozycjaMacierzy, 1, GL_FALSE, value_ptr(macierz));
    }

}
