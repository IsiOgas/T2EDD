#include <iostream>
#include <fstream>
#include <sstream> //Para usar stringstream
using namespace std;

//Structs necesarios
class Jugador {
    private :
        int vida ;
        int ataque ;
        float precision ;
        int recuperacion ;
};

struct Habitacion {
    int id;
    std::string nombre;
    std::string tipo; // "INICIO", "EVENTO", "COMBATE", "FIN"
    std::string descripcion;

    // Punteros a los hijos (árbol ternario)
    Habitacion* hijo1;
    Habitacion* hijo2;
    Habitacion* hijo3;
};

Habitacion** habitaciones = nullptr;
int capacidad = 0;        
int totalHabitaciones = 0;  

void cargarHabitaciones(ifstream &ejemplo) {
    if (!ejemplo.is_open()) {
        cout << "No se pudo abrir el archivo." << endl;
        return;
    };

    int cantidadHabitaciones= 0;
    string linea;

    // Leer hasta encontrar el número de habitaciones
    while (getline(ejemplo, linea)) {
        stringstream numHabitaciones(linea);
        if (numHabitaciones >> cantidadHabitaciones) { //Introducimos el numHab a la cantidad 
        // Ya encontramos el número, salimos del bucle
            break;
    }
}
    //ELIMINAR LUEGO, SOLO PARA COMPROBAR
    cout << "Cantidad de habitaciones a leer: " << cantidadHabitaciones << endl;
    cout << "Leyendo líneas de habitaciones..." << endl;

    capacidad = cantidadHabitaciones;  // actualizar capacidad global
    habitaciones = new Habitacion*[capacidad];
    totalHabitaciones = 0; // inicializamos
    while (totalHabitaciones < cantidadHabitaciones) {
        
        getline(ejemplo, linea); // Leer línea completa
        stringstream cuarto(linea); // Nos permite recorrer por partes la linea que obtuvimos 
        string temp; //Declaramos variable donde se ira guardando cada valor que se extrae

        // Leer los campos separados por ';'
        getline(cuarto, temp, ';');
        int id = stoi(temp); //Convierte el id de string a int

        getline(cuarto, temp, ';');
        string nombre = temp;

        getline(cuarto, temp, ';');
        // Remover los paréntesis del tipo
        string tipo = temp;

        getline(cuarto, temp); // descripción
        string descripcion = temp;

        // Crear y guardar la habitación
        Habitacion* h = new Habitacion();
        h->id = id;
        h->nombre = nombre;
        h->tipo = tipo;
        h->descripcion = descripcion;
        h->hijo1 = h->hijo2 = h->hijo3 = nullptr;

        habitaciones[totalHabitaciones++] = h;
    }
}

int main(){
    ifstream archivo("ejemplo.map");
    if (!archivo.is_open()) {
        cout << "No se pudo abrir el archivo ejemplo.map" << endl;
        return 0;
    }
    cargarHabitaciones(archivo);

    // Imprimir habitaciones para verificar
    for (int i = 0; i < totalHabitaciones; i++) {
        cout << "ID: " << habitaciones[i]->id << endl;
        cout << "Nombre: " << habitaciones[i]->nombre << endl;
        cout << "Tipo: " << habitaciones[i]->tipo << endl;
        cout << "Descripcion: " << habitaciones[i]->descripcion << endl;
        cout << "---------------------------" << endl;
    }

    // Liberar memoria
    for (int i = 0; i < totalHabitaciones; i++) {
        delete habitaciones[i];
    }
    delete[] habitaciones;

    return 0;
}