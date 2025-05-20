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
    string nombre;
    string tipo; // "INICIO", "EVENTO", "COMBATE", "FIN"
    string descripcion;

    // Punteros a los hijos (árbol ternario)
    Habitacion* hijo1;
    Habitacion* hijo2;
    Habitacion* hijo3;
};

Habitacion** habitaciones = nullptr;
int capacidad = 0;        
int totalHabitaciones;  

void mostrarHabitacion(Habitacion* h) {
    if (h->id == 0) {
        cout << "-- Habitación Inicial --" << endl;
        cout<< h->descripcion <<endl;
    }else{
        cout<< "-- " << h->nombre << " --" <<endl;
        cout<< h->descripcion <<endl;
    }
    
    
    if (h->hijo1 || h->hijo2 || h->hijo3) {
        cout << "A donde quieres ir?" << endl;
        if (h->hijo1) {
            cout<< "1. " << h->hijo1->nombre <<endl;
        }
        if (h->hijo2){
            cout<< "2. " << h->hijo2->nombre <<endl;
        }
        if (h->hijo3){
            cout<< "3. " << h->hijo3->nombre <<endl;
        }
        cout << "(Presiona la tecla correspondiente)" <<endl;
    }
}

Habitacion* elegirHabitacion(Habitacion* h) {
    int opcion;
    cin >> opcion;

    if (opcion == 1 && h->hijo1) {
        return h->hijo1;
    } else if (opcion == 2 && h->hijo2) {
        return h->hijo2;
    } else if (opcion == 3 && h->hijo3) {
        return h->hijo3;
    } else {
        cout<< "Opción inválida. Intenta de nuevo." <<endl;
        return h; // se queda en la misma habitación
    }
}

void cargarArcos(ifstream &archivo) {
    string linea;

    // Buscar la línea "ARCOS"
    while (getline(archivo, linea)) {
        if (linea == "ARCOS") 
            break;
    }
    // Ahora leemos la cantidad de arcos que sigue a "ARCOS"
    getline(archivo, linea);
    int cantidadArcos = stoi(linea);

    //ELIMINAR LUEGO, SOLO PARA COMPROBAR
    cout << "Cantidad de arcos a leer: " << cantidadArcos << endl;

    for (int i = 0; i < cantidadArcos; i++) {

        getline(archivo, linea);
        stringstream arco(linea);
        string temp;
        // Leer origen hasta espacio
        getline(arco, temp, ' ');
        int origen = stoi(temp);

        // Leer "->"
        getline(arco, temp, ' '); // temp = "->"

        // Leer destino
        getline(arco, temp);
        int destino = stoi(temp);

        // Ahora conectamos origen y destino
        Habitacion* origenHabitacion = habitaciones[origen]; //Puntero al id de habitacion
        Habitacion* destinoHabitacion = habitaciones[destino];

        // Asignar el destino al primer hijo vacío
        if (origenHabitacion->hijo1 == nullptr) {
            origenHabitacion->hijo1 = destinoHabitacion;
        } else if (origenHabitacion->hijo2 == nullptr) {
            origenHabitacion->hijo2 = destinoHabitacion;
        } else if (origenHabitacion->hijo3 == nullptr) {
            origenHabitacion->hijo3 = destinoHabitacion;
        }
    }

}

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
        if (numHabitaciones >> cantidadHabitaciones) { //Lee y prueba si la linea tiene un num entero, si es asi se guarda en cantHab 
        // Ya encontramos el número, salimos del bucle
            break;
    }
}
    //ELIMINAR LUEGO, SOLO PARA COMPROBAR
    cout << "Cantidad de habitaciones a leer: " << cantidadHabitaciones << endl;

    capacidad = cantidadHabitaciones;  
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
        cout << "No se pudo abrir el archivo" << endl;
        return 0;
    }

    cargarHabitaciones(archivo);
    cargarArcos(archivo);
    
    Habitacion* habitacionActual = habitaciones[0]; //puntero a la habitacion inicial

    bool juegoActivo = true;

    while (juegoActivo) {
        mostrarHabitacion(habitacionActual);

        if (habitacionActual->nombre == "Pantano") {
            juegoActivo = false;  // Detiene el ciclo
        }else {
            habitacionActual = elegirHabitacion(habitacionActual);
            cout << endl;
        }
    }

    
    // Liberar memoria
    for (int i = 0; i < totalHabitaciones; i++) {
        delete habitaciones[i];
    }
    delete[] habitaciones;

    return 0;
}
