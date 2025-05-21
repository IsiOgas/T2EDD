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

void CargarHabitaciones(ifstream& archivo) {
    string linea;
    while (getline(archivo, linea) && linea != "HABITACIONES");
    getline(archivo, linea);
    TotalHabitaciones = stoi(linea);
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

void mostrarHabitacion(Habitacion* h) {
    cout << "\nEstás en: " << h->nombre << endl;
    cout << h->descripcion << endl;

    if (h->hijo1) cout << "1. Ir a " << h->hijo1->nombre << endl;
    if (h->hijo2) cout << "2. Ir a " << h->hijo2->nombre << endl;
    if (h->hijo3) cout << "3. Ir a " << h->hijo3->nombre << endl;
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
