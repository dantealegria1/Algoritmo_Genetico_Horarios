/*
Nombre de la institucion: Universidad autonoma de aguascalientes
Nombre del centro al que pertenece la carrera: centro de ciencias de básicas
Nombre del departamento al que pertence la carrera: ciencias de la computación 
Nombre de la materia: Optimizaciónn inteligente
Nombre(s) de quien(es) realiza(n) la practica: 
*Dante Alejandro Alegria Romero
*Andrea margarita Balandran Felix
nombre profesor: Luis Fernando Gutierrez Marfileño

Breve descripcion de lo que realiza el programa: 
este es el proyecto que genero el equipo a partir de la información que se tiene sobre la carrera de ICI
donde a partir de la información de los grupos como lo son el grupo, el dia, la hora de entrada, la hora de salida,
el aula y la materia, etc. nos encargamos de generar un horario que no genere ningun tipo de choque entre los grupos
y que se pueda visualizar de una manera mas sencilla para el usuario. todo esto a partir de lo que aprendimos 
a lo largo del semestre en la materia de optimización inteligente
*/

//Librerias utilizadas para el programa
#include <iostream>
#include <vector>
#include <string>
#include <time.h>
#include <chrono>
#include <thread>
//Incluyo la libreria stdlib.h para poder usar la funcion rand()
#include <stdlib.h>
using namespace std;

/*
Variables globales para el funcionamiento adecuado del programa
@NumCromosomas: Numero de cromosomas que tendra cada individuo
@TamCromosoma: Tamaño de cada cromosoma
@NumeroIndividuos: Numero de individuos que tendra la poblacion
@Generacion: Matriz tridimensional que contiene a todos los individuos
@Generacion_Hijos: Matriz tridimensional que contiene a todos los hijos
*/

const int NumCromosomas = 36;
const int TamCromosoma = 8;
const int NumeroIndividuos = 100;
const int MaxGeneracionesSinMejora = 500;
string Generacion[NumeroIndividuos][NumCromosomas][TamCromosoma];
string Generacion_Hijos[NumeroIndividuos][NumCromosomas][TamCromosoma];

/*
@Materia: Funcion que retorna una materia aleatoria dependiendo del semestre, ejemplo: 
si el semestre es 1, la materia sera de primer semestre, esto para evitar que una grupo 
de primer semestre tenga una materia de noveno semestre y viceversan
Lo hacemos usando un switch case, donde cada case es un semestre
*/

string Materia(int semestre, const vector<string>& primerSemestre, const vector<string>& tercerSemestre, const vector<string>& 
quintoSemestre, const vector<string>& septimoSemestre, const vector<string>& novenoSemestre)
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

/*
@CalcularChoques: Funcion que calcula los choques de un individuo, recibe como parametro el numero de individuo y la generacion
Si la hora de entrada de un cromosoma es igual a la hora de entrada de otro cromosoma, y el salon utilizado es el mismo, entonces
hay un choque de horario. En el caso de que la amteria sea la misma, entonces hay un choque de materia
Se divide entre 2 porque si hay un choque entre el cromosoma 1 y el 2, entonces hay un choque entre el 2 y el 1, por lo tanto
lo cuenta dos veces, por eso se divide entre 2
*/

int CalcularChoques(int numeroPolola,vector<vector<vector<string>>>& Generacion) {
    int choques = 0;
    int TAM = NumCromosomas;
    int indice = 0;
    while(indice < TAM-1)
    {
        for (int i = 0; i < TAM; i++)
        {
            if(Generacion[numeroPolola][indice][5] == Generacion[numeroPolola][i][5] && indice != i)
            {
                if((Generacion[numeroPolola][indice][2] == Generacion[numeroPolola][i][2]) && indice != i)
                {
                    if(Generacion[numeroPolola][indice][6] == Generacion[numeroPolola][i][6] && indice != i)
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

void ComprobarNumeroHoras(int NumeroIndividuo, vector<vector<vector<string>>>& Generacion, vector<string>& Modulo2,
                        vector<string>& primerSemestre, vector<string>& tercerSemestre,
                        vector<string>& quintoSemestre, vector<string>& septimoSemestre,
                        vector<string>& novenoSemestre) {

    for (int i = 0; i < NumCromosomas; i++) {
        string grupo = Generacion[NumeroIndividuo][i][0];
        int contador = 0; // Reiniciar contador para cada grupo

        for (int j = 0; j < NumCromosomas; j++) {
            if (grupo == Generacion[NumeroIndividuo][j][0]) {
                contador++;
                if (contador > 7) {
                    int random = rand() % Modulo2.size();
                    Generacion[NumeroIndividuo][j][0] = Modulo2[random];

                    // Restablecer el contador y obtener la nueva materia para el nuevo grupo
                    contador = 0;
                    grupo = Generacion[NumeroIndividuo][j][0];
                    int semestre = stoi(grupo.substr(0, 1));
                    Generacion[NumeroIndividuo][j][6] = Materia(semestre, primerSemestre, tercerSemestre,
                                                                quintoSemestre, septimoSemestre, novenoSemestre);
                }
            }
        }
    }
}


/*
@CrearGeneracion: Funcion que crea un individuo, recibe como parametro el numero de individuo, la generacion, el numero de cromosoma
y los modulos de cada cromosoma, dependiendo del numero de cromosoma, se le asigna un modulo diferente, por ejemplo, si el numero de
cromosoma es 0, entonces se le asigna un grupo, si es 1, entonces se le asigna un dia, y asi sucesivamente, 
Dependiendo del tamaño del modulo, se le asigna un numero aleatorio, por ejemplo, si el modulo es de 5, entonces se le asigna un numero
aleatorio entre 0 y 4, y asi sucesivamente, el unico modulo que no se le asigna un numero aleatorio es el modulo de la materia, 
ya que primero se debe de saber el semestre del grupo, para asi poder asignarle una materia de ese semestre, tambien a 
la hora de salida se le asigna una hora de salida, dependiendo de la hora de entrada, por ejemplo, si la hora de entrada es 7:00,
entonces la hora de salida sera 8:00 
*/

void CrearGeneracion(int NumeroIndividuo, vector<vector<vector<string>>>& Generacion, int numeroCromosoma, const vector<string>&
Modulo2, const vector<string>& Modulo3, const vector<string>& Modulo4, const vector<string>& Modulo5, const vector<string>& Modulo6,
const vector<string>& Modulo7, const vector<string>& primerSemestre, const vector<string>& tercerSemestre, const vector<string>&
quintoSemestre, const vector<string>& septimoSemestre, const vector<string>& novenoSemestre)
{
    //Agrego Primero el grupo
    int random = rand() % Modulo2.size();
    Generacion[NumeroIndividuo][numeroCromosoma][0] = Modulo2[random];
    //Agrego el dia
    random = rand() % Modulo3.size();
    Generacion[NumeroIndividuo][numeroCromosoma][1] = Modulo3[random];
    //Agrego la hora
    random = rand() % Modulo4.size();
    Generacion[NumeroIndividuo][numeroCromosoma][2] = Modulo4[random];
    //Agrego el dia 
    random = rand() % Modulo5.size();
    Generacion[NumeroIndividuo][numeroCromosoma][3] = Modulo5[random];
    //Agrego la hora de salida
    int horaEntrada = stoi(Generacion[NumeroIndividuo][numeroCromosoma][2].substr(0, 2));
    int horaSalida = horaEntrada + 1;
    Generacion[NumeroIndividuo][numeroCromosoma][4] = to_string(horaSalida) + ":00";
    //Agrego el aula
    random = rand() % Modulo7.size();
    Generacion[NumeroIndividuo][numeroCromosoma][5] = Modulo7[random];
    //Agrego la materiab 
    string grupo = Generacion[NumeroIndividuo][numeroCromosoma][0];
    int semestre = stoi(grupo.substr(0, 1));
    Generacion[NumeroIndividuo][numeroCromosoma][6] = Materia(semestre, primerSemestre, 
    tercerSemestre, quintoSemestre, septimoSemestre, novenoSemestre);
}

/*
@organizarGen: Funcion que organiza la generacion de menor a mayor, dependiendo de los choques, recibe como parametro la generacion,
el numero de individuo, el numero de cromosoma y el tamaño del cromosoma, para organizar la generacion, se utiliza el metodo de la
burbuja, donde se comparan los choques de un individuo con los choques del siguiente individuo, si el numero de choques del individuo
actual es mayor al numero de choques del siguiente individuo, entonces se intercambian los individuos, y asi sucesivamente, hasta
que la generacion este organizada de menor a mayor
*/

void organizarGen(vector<vector<vector<string>>>& Generacion, int NumeroIndividuo, int NumCromosomas, int TamCromosoma)
{
    // organizaremos la generacion de menor a mayor de acuerdo a los choques
    int aux=0;
    int choqueAux=0;
    //ordenar de menor a mayor
    for (int i = 0; i < NumeroIndividuo; i++)
    {
        for (int j = 0; j < NumeroIndividuo - 1; j++)
        {
            // Convertir cadenas a enteros para comparar
            int choqueActual = stoi(Generacion[j][NumCromosomas-1][0]);
            int choqueSiguiente = stoi(Generacion[j + 1][NumCromosomas-1][0]);

            if (choqueActual > choqueSiguiente)
            {
                for(int k=0; k<NumCromosomas; k++)
                {
                    for(int l=0; l<TamCromosoma; l++)
                    {
                        swap(Generacion[j][k][l], Generacion[j + 1][k][l]);
                    }
                }
            }
        }
    }
}

/*
@Cruzar: Funcion que cruza a dos individuos, recibe como parametro el numero de individuo, la generacion y la generacion de hijos
Primero se copia en la generacion de hijos al primer padre  despues en la mitad del numero de cromosomas se copia al segundo padre
la @param Beto es el numero de individuos menos 1, ya que el ultimo individuo es el que tiene mas  choques, por lo tanto, se le
asigna al padre 2
*/

void Cruzar(int NumeroIndividuo, vector<vector<vector<string>>>& Generacion, vector<vector<vector<string>>>& Generacion_Hijos) 
{
    int i = NumeroIndividuo;
        for(int j=0; j<NumCromosomas-1; j++)
        {
            for(int k=0; k<TamCromosoma; k++)
            {
                Generacion_Hijos[i][j][k] = Generacion[i][j][k];
            }
        }
        //El ultimo cromosoma se lo asigno al padre 2
        
        int Beto = (NumeroIndividuos-1);
        int padre2 = (Beto-i)/2 ;
        int random = rand() % 100;
        if(random < 10)
        {
            padre2 = Beto-i;
        }
        for(int j=4; j<NumCromosomas-1; j++)
        {
            for(int k=0; k<TamCromosoma; k++)
            {
                Generacion_Hijos[i][j][k] = Generacion[padre2][j][k];
            }
        }
}

void ResolverChoques(vector<vector<vector<string>>>& Generacion, int NumeroIndividuo, vector<string>& Modulo7, vector<string>& Modulo4) {
    int choques = stoi(Generacion[NumeroIndividuo][NumCromosomas-1][0]);
    int indice = 0;
    while(indice < NumCromosomas-1)
    {
        for (int i = 0; i < NumCromosomas-1; i++)
        {
            if(Generacion[NumeroIndividuo][indice][5] == Generacion[NumeroIndividuo][i][5] && indice != i)
            {
                if((Generacion[NumeroIndividuo][indice][2] == Generacion[NumeroIndividuo][i][2]) && indice != i)
                {
                        Generacion[NumeroIndividuo][i][2] = Modulo4[rand () % Modulo4.size()];
                        int horaEntrada = stoi(Generacion[NumeroIndividuo][i][2].substr(0, 2));
                        int horaSalida = horaEntrada + 1;
                        Generacion[NumeroIndividuo][i][4] = to_string(horaSalida) + ":00";
        }
        indice++;
    }
    if(choques < 0)
    {
        choques = 0;
    }
    Generacion[NumeroIndividuo][NumCromosomas-1][0] = to_string(choques);
}
    }
}


/*
@Mutar: Funcion que muta a un individuo, recibe como parametro el numero de individuo, la generacion de hijos, el modulo 4 y 
el modulo 7, tomamos el modulo 4 y el modulo 7, ya que son los modulos que tienen mas posibilidades de choques que son la hora
de entrada y el aula, Establecemos el procentaje de mutacion como la mitad de todos mis hijis. primero se genera un numero 
aleatorio entre 0 y 6, ya que el modulo 4 tiene 7 posibilidades, asigno al modulo 4 el numero aleatorio, despues a la hora de entrada
le asigno la hora de entrada del modulo 4, despues a la hora de salida le asigno la hora de entrada mas 1, y por ultimo al aula
le asigno el modulo 7. Esto para intentar generar menos choques 
*/

void Mutar(int NumeroIndividuo, vector<vector<vector<string>>>& Generacion_Hijos, vector<string>& Modulo4,
 vector<string>& Modulo7, vector<string>& Modulo2, vector<string>& primerSemestre, vector<string>& 
 tercerSemestre,vector<string>& quintoSemestre, vector<string>& septimoSemestre, vector<string>& novenoSemestre)
{
    int i = NumeroIndividuo;

    int PorcentajeDeMutar = NumeroIndividuos * 0.5;

    for(int pollito = 0; pollito < PorcentajeDeMutar; pollito++)
    {
        int random2 = rand() % 7;
        Generacion_Hijos[i][random2][2] = Modulo4[random2];
        int horaEntrada = stoi(Generacion_Hijos[i][random2][2].substr(0, 2));
        int horaSalida = horaEntrada + 1;
        Generacion_Hijos[i][random2][4] = to_string(horaSalida) + ":00";
        Generacion_Hijos[i][random2][5] = Modulo7[random2];
        
        //MUTAR LA MATERIA
        Generacion_Hijos[i][random2][0] = Modulo2[rand () % Modulo2.size()];
        int semestre = stoi(Generacion_Hijos[i][random2][0].substr(0, 1));
        Generacion_Hijos[i][random2][6] = Materia(semestre, primerSemestre, tercerSemestre, quintoSemestre, septimoSemestre, novenoSemestre);
    }
}

/*
@CompararGeneraciones: Funcion que compara a la generacion actual con la generacion de hijos, recibe como parametro la generacion
y la generacion de hijos, si el numero de choques de la generacion de hijos es menor al numero de choques de la generacion actual
entonces se copia la generacion de hijos en la generacion actual
*/

void CompararGeneraciones(vector<vector<vector<string>>>& Generacion, vector<vector<vector<string>>>& Generacion_Hijos)
{
    for(int i=0; i<NumeroIndividuos; i++)
    {
        if(stoi(Generacion[i][NumCromosomas-1][0]) > stoi(Generacion_Hijos[i][NumCromosomas-1][0]))
        {
            for(int j=0; j<NumCromosomas; j++)
            {
                for(int k=0; k<TamCromosoma; k++)
                {
                    Generacion[i][j][k] = Generacion_Hijos[i][j][k];
                }
            }
        }
    }
}

/*
@AlgoritmoBelico: Funcion que ejecuta el algoritmo genetico, recibe como parametro la generacion, la generacion de hijos, el modulo 4
y el modulo 7, primero se cruzan los individuos, despues se calculan los choques de los hijos, despues se mutan los hijos, despues
se calculan los choques de los hijos, despues se organizan los hijos de menor a mayor, despues se comparan los hijos con la generacion
actual
*/

void AlgoritmoBelico(vector<vector<vector<string>>>& Generacion, vector<vector<vector<string>>>& Generacion_Hijos, vector<string>& 
Modulo4, vector<string>& Modulo7, vector<string>& Modulo2, vector<string>& primerSemestre, vector<string>& 
tercerSemestre,vector<string>& quintoSemestre, vector<string>& septimoSemestre, vector<string>& novenoSemestre)
{
    //SI DOS CROMOSOMAS TIENEN LA MISMA MATERIA, SE CAMBIA LA MATERIA DE UNO DE ELLOS
    for(int i=0; i<NumeroIndividuos; i++)
    {
        for(int j=0; j<NumCromosomas-1; j++)
        {
            for(int k=0; k<NumCromosomas-1; k++)
            {
                if(Generacion[i][j][6] == Generacion[i][k][6] && j != k)
                {
                    int semestre = stoi(Generacion[i][j][0].substr(0, 1));
                Generacion[i][j][6] = Materia(semestre, primerSemestre, tercerSemestre, quintoSemestre, septimoSemestre, novenoSemestre);
                }
            }
        }
    }
    //CRUZAR--------------------------------------
    for(int i=0; i<NumeroIndividuos; i++)
    {
        Cruzar(i, Generacion, Generacion_Hijos);
    }
    for(int i=0; i<NumeroIndividuos; i++)
    {
        Mutar(i, Generacion, Modulo4, Modulo7, Modulo2, primerSemestre, tercerSemestre, quintoSemestre, septimoSemestre, novenoSemestre);
    }
    //HIJOS ---------------------------------------
    for(int i=0; i<NumeroIndividuos; i++)
    {
            Generacion_Hijos[i][NumCromosomas-1][0] = to_string(CalcularChoques(i, Generacion_Hijos));
        
    }
    //VERIFICO LAS HORAS
    for(int i=0; i<NumeroIndividuos; i++)
    {
        ComprobarNumeroHoras(i, Generacion_Hijos, Modulo2, primerSemestre, tercerSemestre, quintoSemestre, septimoSemestre, novenoSemestre);
    }
    //MUTO
    for(int i=0; i<NumeroIndividuos; i++)
    {
            Generacion_Hijos[i][NumCromosomas-1][0] = to_string(CalcularChoques(i, Generacion_Hijos));
        
    }

    //CHOQUESSSSS--------------------------------------
    organizarGen(Generacion_Hijos, NumeroIndividuos, NumCromosomas, TamCromosoma);

    //COMPARAR CON LA GENERACION ANTERIOR--------------------------------------
    CompararGeneraciones(Generacion, Generacion_Hijos);
    for(int i=0; i<NumeroIndividuos; i++)
            {
                ComprobarNumeroHoras(i, Generacion, Modulo2, primerSemestre, tercerSemestre, quintoSemestre, septimoSemestre, novenoSemestre);
            }


    if(Generacion[0][NumCromosomas-1][0] == "1")
    {
        ResolverChoques(Generacion, 0, Modulo7, Modulo4);
    }
    //Si veo que se repiten dos materias en un mismo grupo, entonces cambio la materia 
    //Creo una lista de materias que ya se usaron en el grupo y si se repite una materia, entonces cambio la materia
    //Por la materia que no se ha usado
    for(int i=0; i<NumeroIndividuos; i++)
    {
        for(int j=0; j<NumCromosomas-1; j++)
        {
            for(int k=0; k<NumCromosomas-1; k++)
            {
                if(Generacion[i][j][6] == Generacion[i][k][6] && j != k)
                {
                    int semestre = stoi(Generacion[i][j][0].substr(0, 1));
                Generacion[i][j][6] = Materia(semestre, primerSemestre, tercerSemestre, quintoSemestre, septimoSemestre, novenoSemestre);
                }
            }
        }
    }
}

/*
@main: Funcion principal del programa, donde se ejecuta todo el programa, se crean los modulos de cada cromosoma, se crea la generacion
se calculan los choques de la generacion, se organiza la generacion, se ejecuta el algoritmo genetico, y se imprimen los individuos
con 0 choques
*/

int main()
{
    srand(time(NULL));
    int opcion;
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
    //Creo la generacion
    vector<vector<vector<string>>> Generacion_Hijos(NumeroIndividuos, vector<vector<string>>(NumCromosomas, 
    vector<string>(TamCromosoma)));
    vector<vector<vector<string>>> Generacion(NumeroIndividuos, vector<vector<string>>(NumCromosomas, 
    vector<string>(TamCromosoma)));
    
    do
    {
        // Menú de opciones
        cout<<"HOLA BIENVENIDO AL ORGANIZADOR DE HORARIOS DE LA CARRERA DE ICI"<<endl;
        cout<<"Este codigo proporciona un horario sin choques para TODA, la carrera de ingenieria en comptacion inteligente"<<endl;
        cout<<"su funcionamiento se basa en la creacion de una poblacion de horarios"<<endl;
        cout<<"que en si cada individuo es un horario potencial y cada cromosoma de este es una materia"<<endl;
        cout<<"con su respectivo crupo, dia, hora de entrada, hora de salida, aula y materia"<<endl;
        cout<<"--------------------------------------------------------------------------"<<endl;
        cout<<"Despues de muchas ejecuciones se recomienda limpiar su terminal"<<endl;
        cout<<"para que no se vea saturada de informacion y liberar un poco de memoria!!"<<endl;
        cout<<"--------------------------------------------------------------------------"<<endl;
        cout << "Menú:" << endl;
        cout << "1) Generar horario" << endl;
        cout << "2) Salir" << endl;
        cout << "Seleccione una opción: ";
        int contador=0;
                    int hola = 0;
            int progreso = 0;
            
            int promedioDeGeneraciones = 0;
            int generacionesSinMejora = 0;
    int holaaaaaa = 0;
        cin >> opcion;

        switch (opcion)
        {
        case 1: 
            //CREAR GENERACION--------------------------------------
            for (int i = 0; i < NumeroIndividuos; i++)
            {

                for (int j = 0; j < NumCromosomas-1; j++)
                {
                    CrearGeneracion(i, Generacion, j, Modulo2, Modulo3, Modulo4, Modulo5, 
                    Modulo6, Modulo7, primerSemestre, tercerSemestre, quintoSemestre, septimoSemestre, 
                    novenoSemestre);
                }
            }
            //COMPRUEBO HORAS
            for(int i=0; i<NumeroIndividuos; i++)
            {
                ComprobarNumeroHoras(i, Generacion, Modulo2, primerSemestre, tercerSemestre, quintoSemestre, 
                septimoSemestre, novenoSemestre);
            }
            //CHOQUESSSSS--------------------------------------
            for(int i=0; i<NumeroIndividuos; i++)
            {
                    Generacion[i][NumCromosomas-1][0] = to_string(CalcularChoques(i, Generacion));
                
            }
            organizarGen(Generacion, NumeroIndividuos, NumCromosomas, TamCromosoma);
            for(int i=0; i<NumeroIndividuos; i++)
            {
                ComprobarNumeroHoras(i, Generacion, Modulo2, primerSemestre, tercerSemestre, quintoSemestre, 
                septimoSemestre, novenoSemestre);
            }
            //Imprimimos la primera generacion
            cout<<"Quiere Imprimir la primer generacion? (1=si, 2=no)"<<endl;
            int imprimir2;
            cin>>imprimir2;
            if(imprimir2 == 1)
            {
                cout<<"Imprimiendo la primer generacion"<<endl;
            //Hacer que espere 3 segundos
            std::cout << "Cargando";
            std::cout.flush();
            for (int i = 0; i < 10; ++i) {
                std::this_thread::sleep_for(std::chrono::milliseconds(300)); 
                std::cout << ".";
                std::cout.flush(); 
            }

                for (int i = 0; i < NumeroIndividuos; i++)
                {
                    for (int j = 0; j < NumCromosomas; j++)
                    {
                        for (int k = 0; k < TamCromosoma; k++)
                        {
                            cout << Generacion[i][j][k] << " ";
                        }
                        cout << endl;
                    }
                    cout << endl;
                }
            }
        //--------------------------------------------------------------------------------------------------------------------------------
            //ALGORITMO BELICO--------------------------------------
            cout<<"Cuantas generaciones quieres que se hagan:"<<endl;
            int numGeneraciones;
            cin>>numGeneraciones;
            for(int i=0; i<numGeneraciones; i++)
            {
                AlgoritmoBelico(Generacion, Generacion_Hijos, Modulo4, Modulo7, Modulo2, 
                primerSemestre, tercerSemestre, quintoSemestre, septimoSemestre, novenoSemestre); 
                  // Simulación de generación // Simula una generación cada 0.5 segundos
                hola++;
                float porcentaje = (static_cast<float>(hola) / numGeneraciones) * 100;

                if (static_cast<int>(porcentaje) >= progreso) {
                    std::cout << "Porcentaje de avance: " << progreso << "%" << std::endl;
                    progreso += 5; // Aumenta el progreso en intervalos de 5
                    //Imprimo el numero mas pequeño de choques
                    cout<<"Numero mas bajo de choques: "<<Generacion[0][NumCromosomas-1][0]<<endl;
                }
                int individuosConCeroChoques = 0;
                for(int j = 0; j < NumeroIndividuos; j++) {
                    if(Generacion[j][NumCromosomas-1][0] == "0") {
                        individuosConCeroChoques++;
                        break;
                    }
                }
                if (individuosConCeroChoques > 0) {
                    cout << "¡Se ha encontrado una solución sin choques en la generación "
                    << i + 1 << "! Deteniendo el algoritmo." << endl;
                    promedioDeGeneraciones = i + 1;
                    break;
                }
                // Si no hay mejora en el número de individuos con 0 choques, aumenta la variable de generaciones sin mejora
                if(individuosConCeroChoques == 0) {
                    generacionesSinMejora++;
                } else {
                    generacionesSinMejora = 0;
                }

                // Verificar si se ha alcanzado el número máximo de generaciones sin mejora
                if(generacionesSinMejora >= MaxGeneracionesSinMejora) {
                    cout << "Se han alcanzado " << MaxGeneracionesSinMejora << 
                    " generaciones sin mejora. Deteniendo el algoritmo." << endl;
                    break;
                }
                            // Limpiar la terminal (solo en sistemas Windows)

            };
            std::cout << "Cargando";
            std::cout.flush(); 
            for (int i = 0; i < 10; ++i) {
                std::this_thread::sleep_for(std::chrono::milliseconds(300)); 
                std::cout << ".";
                std::cout.flush(); 
            }
            cout<<endl;
            //Imprimimoos todos los individuos con 0 choques
            for (int i = 0; i < NumeroIndividuos; i++)
            {
                if(Generacion[i][NumCromosomas-1][0] == "0")
                {
                    for (int j = 0; j < NumCromosomas; j++)
                    {
                        for (int k = 0; k < TamCromosoma; k++)
                        {
                            cout << Generacion[i][j][k] << " ";
                        }
                        cout << endl;
                    }
                    cout << endl;
                    contador++;
                }
            }
            cout<<"Numero de horarios con 0 choques: "<<contador<<endl;
            if(contador == 0)
            {
                cout<<"Deseas saber el mejor idnividuo (1=si, 2=no)"<<endl;
                int respuesta;
                cin>>respuesta;
                if(respuesta == 1)
                {
                    for (int j = 0; j < NumCromosomas; j++)
                    {
                        for (int k = 0; k < TamCromosoma; k++)
                        {
                            cout << Generacion[0][j][k] << " ";
                        }
                        cout << endl;
                    }
                    cout << endl;
                }
            }
            cout<<"Quiere Imprimir la ultima generacion? (1=si, 2=no)"<<endl;
            int imprimir;
            cin>>imprimir;
            if(imprimir == 1)
            {
                for (int i = 0; i < NumeroIndividuos; i++)
                {
                    for (int j = 0; j < NumCromosomas; j++)
                    {
                        for (int k = 0; k < TamCromosoma; k++)
                        {
                            cout << Generacion[i][j][k] << " ";
                        }
                        cout << endl;
                    }
                    cout << endl;
                }
            }
            cout<<endl;
            
            break;
        case 2:
            cout << "Saliendo del programa. ¡Hasta luego!" << endl;
            break;
        default:
            cout << "Opción no válida. Por favor, seleccione una opción válida." << endl;
            break;
        }

    } while (opcion != 2);

    return 0;
}
