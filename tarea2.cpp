#include <iostream>
#include <fstream>
#include <sstream> //Para usar stringstream
#include <string>
using namespace std;

//Structs necesarios
class Jugador {
    private :
        int vida ;
        int ataque ;
        float precision ;
        int recuperacion ;
};

struct Enemigo {
    string nombre;
    int vida;
    int ataque;
    float precision;
    float probabilidad;  // Probabilidad de aparecer en una habitación de combate
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
    
    Enemigo* enemigos[2]; //Hasta 2 enemigos por habitacion
    bool enemigosAsignados = false;
    int cantidadEnemigosAsignados;
};




Enemigo** enemigos = nullptr;
int totalEnemigos;

void cargarEnemigos(ifstream &ejemplo){
    string linea;

    // Buscar la línea "ENEMIGOS"
    while (getline(ejemplo, linea)) {
        if (linea =="ENEMIGOS") 
            break;
    }

    getline(ejemplo, linea);
    cout << "Linea que se intenta convertir a entero: '" << linea << "'" << endl;
    int cantidadEnemigos = stoi(linea);
    totalEnemigos=cantidadEnemigos;

    enemigos = new Enemigo*[cantidadEnemigos];
    //ELIMINAR LUEGO, SOLO PARA COMPROBAR
    cout << "Cantidad de enemigos a leer: " << cantidadEnemigos << endl;

    for (int i = 0; i < cantidadEnemigos; i++) {

        getline(ejemplo, linea);
        stringstream enemy(linea);
        string temp;
        
        getline(enemy, temp, '|');
        string nombre= temp;

        getline(enemy, temp, '|'); 
        stringstream streamVida(temp); 
        int vida;     
        streamVida >> temp >> vida;

        getline(enemy, temp, '|');
        stringstream streamAtaque(temp); 
        int ataque;     
        streamAtaque >> temp >> ataque;

        getline(enemy, temp, '|');
        stringstream streamPrecision(temp); 
        float precision;     
        streamPrecision >> temp >> precision;

        getline(enemy, temp);
        stringstream streamProbabilidad(temp); 
        float probabilidad;     
        streamProbabilidad >> temp >> probabilidad;
        

        Enemigo* e = new Enemigo();
        e->nombre = nombre;
        e->vida = vida;
        e->ataque = ataque;
        e->precision = precision;
        e->probabilidad= probabilidad;

        enemigos[i] = e;
        
    }
    
    
}

Habitacion** habitaciones = nullptr;       
int totalHabitaciones;  
 
void mostrarHabitacion(Habitacion* h){
    if (h->id == 0) {
        cout << "-- Habitación Inicial --" << endl;
        cout<< h->descripcion <<endl;
    }else if (h->tipo == "COMBATE" && !h->enemigosAsignados){
        
        int EnemigosQueAparecen = 1 + rand() % 2; //Si aparece 1 o 2 en la habitacion
        int EnemigoAsignado = 0;

        while (EnemigoAsignado < EnemigosQueAparecen) {
            float dado = rand() % 1000 / 1000.0f; //Genera un numero entre 0 y 1
            float acumulado = 0; // Va sumando las prob de los enemigos 
            for (int j = 0; j < totalEnemigos; ++j) {
                acumulado += enemigos[j]->probabilidad;
                if (dado <= acumulado) {
                // Verifica que no se repita
                    bool repetido = false;
                    for (int i = 0; i < EnemigoAsignado; ++i) {
                     if (h->enemigos[i] == enemigos[j]) {
                          repetido = true;
                         break;
                        }
                    }
                    if (!repetido) {
                    h->enemigos[EnemigoAsignado] = enemigos[j];
                    EnemigoAsignado++;
                    }
                    break;
                }
            }   
        }
        h->cantidadEnemigosAsignados = EnemigoAsignado;
        h->enemigosAsignados = true;

        if (h->cantidadEnemigosAsignados == 1) {
            cout << "¡Te enfrentas al monstruo llamado"<< h->enemigos[0]->nombre << "!" << endl;
        } else {
        cout << "¡Te enfrentas a dos monstruos: " << h->enemigos[0]->nombre << " y " << h->enemigos[1]->nombre << "!" << endl;
        }
        cout << "Jugador | ";
        for (int i = 0; i < h->cantidadEnemigosAsignados; ++i) {
            cout << h->enemigos[i]->nombre;
            if (i != h->cantidadEnemigosAsignados - 1) {
                cout << " | ";
            }
        }
        cout << endl;

        cout << "30      | ";
        for (int i = 0; i < h->cantidadEnemigosAsignados; ++i) {
            cout << h->enemigos[i]->vida;
            if (i != h->cantidadEnemigosAsignados - 1){
                cout << " | ";
            }
        }
        
        cout << endl;

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

void cargarArcos(ifstream &ejemplo) {
    string linea;

    // Buscar la línea "ARCOS"
    while (getline(ejemplo, linea)) {
        if (linea == "ARCOS") 
            break;
    }
    // Ahora leemos la cantidad de arcos que sigue a "ARCOS"
    getline(ejemplo, linea);
    int cantidadArcos = stoi(linea);

    //ELIMINAR LUEGO, SOLO PARA COMPROBAR
    cout << "Cantidad de arcos a leer: " << cantidadArcos << endl;

    for (int i = 0; i < cantidadArcos; i++) {

        getline(ejemplo, linea);
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
    string linea;

    // Leer hasta encontrar el número de habitaciones
    while (getline(ejemplo, linea)) {
        if (linea == "HABITACIONES") 
            break;
    }
    getline(ejemplo, linea);
    int cantidadHabitaciones = stoi(linea);
    totalHabitaciones=cantidadHabitaciones;

    //ELIMINAR LUEGO, SOLO PARA COMPROBAR
    cout << "Cantidad de habitaciones a leer: " << cantidadHabitaciones << endl;
 
    habitaciones = new Habitacion*[cantidadHabitaciones];
    
    for( int i=0; i < cantidadHabitaciones; i++) {
        
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

        habitaciones[i] = h;
    }
}

int main(){
    ifstream archivo;
    archivo.open("ejemplo.map");
    if (!archivo.is_open()) {
        cout << "No se pudo abrir el archivo" << endl;
        return 0;
    }

    cargarHabitaciones(archivo);
    cargarArcos(archivo);
    cargarEnemigos(archivo);

    
    Habitacion* habitacionActual = habitaciones[0]; //puntero a la habitacion inicial

    bool juegoActivo = true;

    while (juegoActivo) {
        mostrarHabitacion(habitacionActual);

        if (habitacionActual->tipo == "FIN") {
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

    for (int i = 0; i < totalEnemigos; i++) {
        delete enemigos[i];
    }   
    delete[] enemigos;
    
    return 0;
}
