#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
using namespace std;

class Jugador {
private:
    int vida;
    int ataque;
    float precision;
    int recuperacion;
};

struct Enemigo {
    string nombre;
    int vida;
    int ataque;
    float precision;
    float probabilidad;
};

struct Habitacion {
    int id;
    string nombre;
    string tipo;
    string descripcion;

    Habitacion* hijo1;
    Habitacion* hijo2;
    Habitacion* hijo3;

    Enemigo* enemigos[2];
    bool enemigosAsignados = false;
    int cantidadEnemigosAsignados;
};

Habitacion** ListaHabitaciones = nullptr;
int TotalHabitaciones = 0;

Enemigo** ListaEnemigos = nullptr;
int TotalEnemigos = 0;

Jugador jugador;

/******
*   void CargarHabitaciones
******
*   Lee las habitaciones desde un archivo y las almacena en un arreglo global.
******
* Input:
*   ifstream& archivo : archivo de entrada con los datos de las habitaciones
******
* Returns:
*   void, no retorna nada -> modifica la variable global ListaHabitaciones.
*****/

void CargarHabitaciones(ifstream& archivo) {
    string linea;
    while (getline(archivo, linea) && linea != "HABITACIONES");
    getline(archivo, linea);
    TotalHabitaciones = stoi(linea  );
    string dato;
    ListaHabitaciones = new Habitacion*[TotalHabitaciones];

    for (int i = 0; i < TotalHabitaciones; i++) {
        getline(archivo, linea);
        stringstream ss(linea);
        ListaHabitaciones[i] = new Habitacion();

        getline(ss, dato, ';');
        ListaHabitaciones[i]->id = stoi(dato);

        getline(ss, dato, ';');
        ListaHabitaciones[i]->nombre = dato;

        getline(ss, dato, ';');
        ListaHabitaciones[i]->tipo = dato;

        getline(ss, dato);
        ListaHabitaciones[i]->descripcion = dato;

        ListaHabitaciones[i]->hijo1 = nullptr;
        ListaHabitaciones[i]->hijo2 = nullptr;
        ListaHabitaciones[i]->hijo3 = nullptr;
    }
}

/******
*   void CargarArcos
******
*   Lee los arcos entre habitaciones desde un archivo y establece los punteros hijos.
******
* Input:
*   ifstream& archivo : archivo de entrada con los datos de los arcos
******
* Returns:
*   void, no retorna nada -> modifica los punteros hijo1, hijo2 y hijo3 de las habitaciones
*****/

void CargarArcos(ifstream& archivo) {
    string linea;
    while (getline(archivo, linea) && linea != "ARCOS");
    getline(archivo, linea);
    int TotalArcos = stoi(linea);

    for (int i = 0; i < TotalArcos; i++) {
        getline(archivo, linea);
        stringstream ss(linea);
        string origenStr, flecha, destinoStr;

        ss >> origenStr >> flecha >> destinoStr;
        int origen = stoi(origenStr);
        int destino = stoi(destinoStr);

        if (ListaHabitaciones[origen]->hijo1 == nullptr) {
            ListaHabitaciones[origen]->hijo1 = ListaHabitaciones[destino];
        } else if (ListaHabitaciones[origen]->hijo2 == nullptr) {
            ListaHabitaciones[origen]->hijo2 = ListaHabitaciones[destino];
        } else if (ListaHabitaciones[origen]->hijo3 == nullptr) {
            ListaHabitaciones[origen]->hijo3 = ListaHabitaciones[destino];
        }
    }
}

/******
*   Habitacion* ElegirHabitacion
******
*   Permite al usuario elegir una de las habitaciones hijas disponibles para moverse.
******
* Input:
*   Habitacion* h : puntero a la habitación actual
******
* Returns:
*   Habitacion* : puntero a la habitación elegida por el usuario, o la misma habitación si la opción es inválida.
*****/

Habitacion* ElegirHabitacion(Habitacion* h) {
    int opcion;
    cout << "Elige una opción: ";
    cin >> opcion;

    if (opcion == 1 && h->hijo1) return h->hijo1;
    else if (opcion == 2 && h->hijo2) return h->hijo2;
    else if (opcion == 3 && h->hijo3) return h->hijo3;
    else {
        cout << "Opción inválida. Intenta de nuevo." << endl;
        return h;
    }
}

/******
*   void mostrarHabitacion
******
*   Muestra la información de la habitación actual por consola.
******
* Input:
*   Habitacion* h : puntero a la habitación que se desea mostrar
******
* Returns:
*   void, solo muestra información en pantalla
*****/

void mostrarHabitacion(Habitacion* h) {
    cout << "\nEstás en: " << h->nombre << endl;
    cout << h->descripcion << endl;

    if (h->hijo1) cout << "1. Ir a " << h->hijo1->nombre << endl;
    if (h->hijo2) cout << "2. Ir a " << h->hijo2->nombre << endl;
    if (h->hijo3) cout << "3. Ir a " << h->hijo3->nombre << endl;
}

void MostarEnemigos(ifstream& archivo){
    string linea;
    while(getline(archivo,linea) && linea != "ENEMIGOS");
    getline(archivo,linea);
    TotalEnemigos = stoi(linea);
    ListaEnemigos = new Enemigo*[TotalEnemigos];
        
    for(int i = 0; i < TotalEnemigos;i++){
        getline(archivo, linea);
        stringstream enemy(linea);
        string dato;
        
        getline(enemy, dato, '|'); //->lee esto "Pim "
        string nombre = dato;

        getline(enemy, dato, '|');  //-> lee esto " Vida 10 " -> ajustar esto ! solo leer valor ej "10". 
        stringstream streamVida(dato); 
        int vida;     
        streamVida >> dato >> vida;

        getline(enemy, dato, '|');
        stringstream streamAtaque(dato); 
        int ataque;     
        streamAtaque >> dato >> ataque;

        getline(enemy, dato, '|');
        stringstream streamPrecision(dato); 
        float precision;     
        streamPrecision >> dato >> precision;

        getline(enemy, dato);
        stringstream streamProbabilidad(dato); 
        float probabilidad;     
        streamProbabilidad >> dato >> probabilidad;
        

        Enemigo* e = new Enemigo();
        e->nombre = nombre;
        e->vida = vida;
        e->ataque = ataque;
        e->precision = precision;
        e->probabilidad= probabilidad;

        enemigos[i] = e;    
    }
}


void AsignarEnemigos(Habitacion* habitacion){
    if(Habitacion->tipo == "COMBATE"){
        for(int i = 0; i < TotalEnemigos; i++){

        }
    }
}



int main() {
    ifstream archivo("ejemplo.map");
    if (!archivo.is_open()) {
        cerr << "Error: No se pudo abrir el archivo ejemplo.map" << endl;
        return 1;
    }

    CargarHabitaciones(archivo);
    CargarArcos(archivo);

    Habitacion* habitacionActual = ListaHabitaciones[0];
    bool JuegoActivo = true;

    while (JuegoActivo) {
        mostrarHabitacion(habitacionActual);

        if (habitacionActual->tipo == "FIN") {
            cout << "\n╔══════════════════════════════╗\n";
            cout <<   "║     G A M E   O V E R . . .  ║\n";
            cout <<   "╚══════════════════════════════╝\n\n";
            JuegoActivo = false;
        } else {
            habitacionActual = ElegirHabitacion(habitacionActual);
        }
    }

    for (int i = 0; i < TotalHabitaciones; i++) {
        delete ListaHabitaciones[i];
    }
    delete[] ListaHabitaciones;

    return 0;
}
