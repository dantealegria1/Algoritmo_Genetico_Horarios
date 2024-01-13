#include <iostream>
#include <vector>
#include <string>
using namespace std;

/*
Generacion es el individuo
y individuo es el cromosoma
*/

const int TAM = 20;
const int CROMOSOMAS = 8;
string Generacion[TAM][CROMOSOMAS];
string GeneracionGrande[TAM][CROMOSOMAS][10];

//FUNCIONA 
string Materia(int semestre, const vector<string>& primerSemestre, const vector<string>& tercerSemestre, const vector<string>& quintoSemestre, const vector<string>& septimoSemestre, const vector<string>& novenoSemestre)
{
    
    string materia;
    int random;
    switch (semestre)
    {
    case 1:
        random = rand() % primerSemestre.size();
        materia = primerSemestre[random];
        break;
    case 3:
        random = rand() % tercerSemestre.size();
        materia = tercerSemestre[random];
        break;
    case 5:
        random = rand() % quintoSemestre.size();
        materia = quintoSemestre[random];
        break;
    case 7: 
        random = rand() % septimoSemestre.size();
        materia = septimoSemestre[random];
        break;
    case 9:
        random = rand() % novenoSemestre.size();
        materia = novenoSemestre[random];
        break;
    }
    return materia;
}

//FUNCIONA
void Individuo(int numeroIndividuo,const vector<string>& Modulo2, const vector<string>& Modulo3, const vector<string>& Modulo4, const vector<string>& Modulo5, const vector<string>& Modulo6, const vector<string>& Modulo7, const vector<string>& primerSemestre, const vector<string>& tercerSemestre, const vector<string>& quintoSemestre, const vector<string>& septimoSemestre, const vector<string>& novenoSemestre)
{

    //Agrego Primero el grupo
    int random = rand() % Modulo2.size();
    Generacion[numeroIndividuo][0] = Modulo2[random];
    //Agrego el dia
    random = rand() % Modulo3.size();
    Generacion[numeroIndividuo][1] = Modulo3[random];
    //Agrego la hora
    random = rand() % Modulo4.size();
    Generacion[numeroIndividuo][2] = Modulo4[random];
    //Agrego el dia 
    random = rand() % Modulo5.size();
    Generacion[numeroIndividuo][3] = Modulo5[random];
    //Agrego la hora
    random = rand() % Modulo6.size();
    Generacion[numeroIndividuo][4] = Modulo6[random];
    //Agrego el aula
    random = rand() % Modulo7.size();
    Generacion[numeroIndividuo][5] = Modulo7[random];
    //Agrego la materia
    string grupo = Generacion[numeroIndividuo][0];
    int semestre = stoi(grupo.substr(0, 1));
    Generacion[numeroIndividuo][6] = Materia(semestre, primerSemestre, tercerSemestre, quintoSemestre, septimoSemestre, novenoSemestre);
}

//FUNCIONA ANTES DE PREGUNTARLE AL PROFE
int CalculoDeChoques() 
{
    int choques = 0;
    int indice = 0;
    while(indice < TAM-1)
    {
        for (int i = 0; i < TAM; i++)
        {
            //Ver si el salon es el mismo
            if(Generacion[indice][5] == Generacion[i][5] && indice != i)
            {
                //Ver si la hora es la misma independientemente si es del dia de inicio o de final
                if((Generacion[indice][2] == Generacion[i][2] || Generacion[indice][2] == Generacion[i][4] || Generacion[indice][4] == Generacion[i][2] || 
                Generacion[indice][4] == Generacion[i][4]) && indice != i)
                {
                    if(Generacion[indice][6] == Generacion[i][6] && indice != i)
                    {
                        choques--;
                    }
                    choques++;
                }
            }
        }
        indice++;
    }

    choques = choques/2;
    return choques;
}
void CrearGeneracionGrande(vector<string> Modulo2, vector<string> Modulo3, vector<string> Modulo4, vector<string> Modulo5, vector<string> Modulo6, vector<string> Modulo7, vector<string> primerSemestre, vector<string> tercerSemestre, vector<string> quintoSemestre, vector<string> septimoSemestre, vector<string> novenoSemestre)
{
    for (int i = 0; i < TAM-1; i++)
    {
        Individuo(i,Modulo2, Modulo3, Modulo4, Modulo5, Modulo6, Modulo7, primerSemestre, tercerSemestre, quintoSemestre, septimoSemestre, novenoSemestre);
    }
    Generacion[TAM-1][0] = to_string(CalculoDeChoques());
    //Imprimo la generacion
    for (int i = 0; i < TAM; i++)
    {
        for (int j = 0; j < CROMOSOMAS; j++)
        {
            cout << Generacion[i][j] << " ";
        }
        cout << endl;
    }
}
int main() {

    srand(time(NULL));
    vector<vector<string>> GeneracionGrandota(TAM, vector<string>(CROMOSOMAS));
    vector<string> Modulo2 = {"1a", "3a", "5a", "7a", "9a"};
    vector<string> Modulo3 = {"Lunes", "Martes", "Miércoles"};
    vector<string> Modulo4 = {"7:00", "8:00", "9:00", "10:00", "11:00", "12:00", "13:00"};
    vector<string> Modulo5 = {"Jueves", "Viernes"};
    vector<string> Modulo6 = {"8:00", "9:00", "10:00", "11:00", "12:00", "13:00", "14:00"};
    vector<string> Modulo7 = {"54 A", "54 C", "54 F", "54 G", "54 H", "61 LAB", "203 LAB", "204 LAB", "1 AUD"};
    vector<string> primerSemestre = {"LC1-1", "FEC-2", "CD-3", "AS-4", "CB-5"};
    vector<string> tercerSemestre = {"LC3-6", "ECA-7", "IA-8", "AL-9", "EDP-10", "RB-11"};
    vector<string> quintoSemestre = {"OI-12", "AU-13", "AID-14", "LI-15", "ED-16", "BD-17"};
    vector<string> septimoSemestre = {"AU2-18", "DMD-19", "MH-20", "ESI-21", "PI-22", "LE-23", "RE1-24"};
    vector<string> novenoSemestre = {"TSI-25", "SI1-26", "SW-27", "PA-28", "SIS-29", "MD-30"};

    //Quiero guardar cada individuo en una generacion
    for(int i=0; i<10; i++)
    {
        cout<<"---------------- Individuo: "<<i+1<<" -----------------"<<endl;
        CrearGeneracionGrande(Modulo2, Modulo3, Modulo4, Modulo5, Modulo6, Modulo7, 
        primerSemestre, tercerSemestre, quintoSemestre, septimoSemestre, novenoSemestre);
    }
    return 0;
}
