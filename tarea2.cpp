#include <iostream>
#include <fstream>
#include <sstream> //Para usar stringstream
#include <string>
using namespace std;

//Structs necesarios
class Jugador {
    private :
        int vida;
        int ataque;
        float precision;
        int recuperacion;
    public:
     // Constructor para inicializar
        Jugador(){
        
            vida = 100;
            ataque = 7;
            precision = 0.8;
            recuperacion = 5;
        }
    
    

    // Getters
    int getVida() { return vida; }
    int getAtaque() { return ataque; }
    float getPrecision() { return precision; }
    int getRecuperacion() { return recuperacion; }
    bool estaVivo() { return vida > 0; }
    // Setters
    void setVida(int v) { vida = v; }
    void setAtaque(int a) { ataque = a; }
    void setRecuperacion(int r) { recuperacion = r; }

};
struct Nodo {
    string nombreTurno;  // Puede ser "jugador" o el nombre de un enemigo
    Nodo* sig;
};
//funciones para TDA Cola
class TDACola {
    private:
        Nodo* head;
        Nodo* tail;
        Nodo* curr;
        unsigned int listSize;
        unsigned int pos; // posicion actual en la lista
    public:
    // métodos de la clase
        TDACola(){ // constructor
            head=nullptr;
            tail=nullptr;
            listSize=0; 
        }          
        bool vacia(){ return head == nullptr; }
        
        void encolar(string nombre){
            Nodo* nuevo = new Nodo;
            nuevo->nombreTurno = nombre;
            nuevo->sig = nullptr;

            if (vacia()) {
                head = tail = nuevo;
            } else {
                tail->sig = nuevo;
                tail = nuevo;
            }

            listSize++;
        }
        string desencolar(){
            if (vacia()) {
                return ""; // o lanzar un error
            }

            Nodo* temp = head;
            string nombre = temp->nombreTurno;

            head = head->sig;
            if (head == nullptr) {
                tail = nullptr;
            }

            delete temp;
            listSize--;

            return nombre;
        }
        
        string TurnoSgte(){
            if (!vacia()) {
                return head->nombreTurno;
            }
            return "";
        }
        
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
    while (getline(ejemplo, linea) && linea != "ENEMIGOS");

    getline(ejemplo, linea);
    totalEnemigos = stoi(linea);

    enemigos = new Enemigo*[totalEnemigos];
    //ELIMINAR LUEGO, SOLO PARA COMPROBAR
    cout << "Cantidad de enemigos a leer: " << totalEnemigos << endl;

    for (int i = 0; i < totalEnemigos; i++) {

        getline(ejemplo, linea);
        stringstream enemy(linea);
        string temp;
        
        getline(enemy, temp, '|');
        string nombre= temp;

        getline(enemy, temp, '|'); //-> lee esto " Vida 10 " -> ajustar esto ! solo leer valor ej "10". pasa eso en todos para abajo
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


bool hayEnemigosVivos(Habitacion* h) {
    for (int i = 0; i < h->cantidadEnemigosAsignados; i++) {
        if (h->enemigos[i]->vida > 0) return true;
    }
    return false;
}

void mostrarEstado(Jugador& jugador, Habitacion* h) {
    // Mostrar nombres
    cout << "Jugador";
    for (int i = 0; i < h->cantidadEnemigosAsignados; ++i) {
        cout << " | " << h->enemigos[i]->nombre;
    }
    cout << endl;

    // Mostrar vida
    cout << jugador.getVida();
    for (int i = 0; i < h->cantidadEnemigosAsignados; ++i) {
        cout << " | " << h->enemigos[i]->vida;
    }
    cout << endl;
}


bool intentoAtaque(float precision) {
    float dado = (rand() % 1000) / 1000.0f;  // número entre 0.0 y 0.999
    return dado <= precision;
}

void turnoCombate(Jugador& jugador, Habitacion* h) {
    TDACola colaTurnos;

    // Encolar jugador
    colaTurnos.encolar("Jugador");

    // Encolar enemigos (por nombre)
    for (int i = 0; i < h->cantidadEnemigosAsignados; i++) {
        colaTurnos.encolar(h->enemigos[i]->nombre);
    }

   while (jugador.getVida() > 0 && hayEnemigosVivos(h)) {
        string turnoActual = colaTurnos.TurnoSgte();

        if (turnoActual == "Jugador") {
            for (int i = 0; i < h->cantidadEnemigosAsignados; i++) {
                if (h->enemigos[i]->vida > 0) {
                    int prob = rand() % 100;
                    if ((rand() % 1000) / 1000.0f <= jugador.getPrecision()) {
                        h->enemigos[i]->vida -= jugador.getAtaque();
                        if ((rand() % 1000) / 1000.0f <= h->enemigos[i]->precision) {

                            cout << "Jugador golpea a " << h->enemigos[i]->nombre << " por " << jugador.getAtaque() << " de da ~n o !\n";
                        } else {
                        cout << "Jugador falla !\n";
                    }
                    break;
                }
            }
        } else {
            for (int i = 0; i < h->cantidadEnemigosAsignados; i++) {
                if (h->enemigos[i]->nombre == turnoActual && h->enemigos[i]->vida > 0) {
                    int prob = rand() % 100;
                    if (prob < h->enemigos[i]->precision) {
                        int nuevaVida = jugador.getVida() - h->enemigos[i]->ataque;
                        if (nuevaVida < 0) nuevaVida = 0;
                        jugador.setVida(nuevaVida);
                        cout << h->enemigos[i]->nombre << " golpea a Jugador por " << h->enemigos[i]->ataque << " de da ~n o !\n";
                    } else {
                        cout << h->enemigos[i]->nombre << " falla !\n";
                    }
                }
            }
        }

        mostrarEstado(jugador, h); // Corrige el llamado
        colaTurnos.encolar(turnoActual);
    }

    if (jugador.getVida() > 0)
        cout << "¡Ganaste el combate!\n";
    else
        cout << "Has sido derrotado...\n";

}




void mostrarHabitacion(Habitacion* h, Jugador& jugador){
    if (h->id == 0) {
        cout << "-- Habitación Inicial --" << endl;
        cout<< h->descripcion <<endl;
    }else if (h->tipo == "COMBATE" && !h->enemigosAsignados){
        mostrarEstado(jugador, h);

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
        turnoCombate(jugador, h);

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


Habitacion** habitaciones = nullptr;       
int totalHabitaciones;

void cargarHabitaciones(ifstream &ejemplo) {
    string linea;
    
    // Leer hasta encontrar el número de habitaciones
    while (getline(ejemplo, linea) && linea != "HABITACIONES");
    
    getline(ejemplo, linea);
    
    

    totalHabitaciones = stoi(linea);

    //ELIMINAR LUEGO, SOLO PARA COMPROBAR
    cout << "Cantidad de habitaciones a leer: " << totalHabitaciones << endl;
 
    habitaciones = new Habitacion*[totalHabitaciones];
    
    for( int i=0; i < totalHabitaciones; i++) {
        
        getline(ejemplo, linea); // Leer línea completa
        stringstream cuarto(linea); // Nos permite recorrer por partes la linea que obtuvimos 
        string dato; //Declaramos variable donde se ira guardando cada valor que se extrae
        // Crear y guardar la habitación
        habitaciones[i] = new Habitacion();
        // Leer los campos separados por ';'
        getline(cuarto, dato, ';');
        habitaciones[i]->id = stoi(dato); //Convierte el id de string a int

        getline(cuarto, dato, ';');
        habitaciones[i]->nombre = dato;

        getline(cuarto, dato, ';');
        habitaciones[i]->tipo = dato;

        getline(cuarto, dato);
        habitaciones[i]->descripcion = dato;


        habitaciones[i]->hijo1 = habitaciones[i]->hijo2 = habitaciones[i]->hijo3 = nullptr;  
    }
}



int totalArcos;
void cargarArcos(ifstream &ejemplo) {

    string linea;

    // Buscar la línea "ARCOS"
    while (getline(ejemplo, linea) && linea != "ARCOS");
    // Ahora leemos la cantidad de arcos que sigue a "ARCOS"
    getline(ejemplo, linea);
    totalArcos = stoi(linea);

    //ELIMINAR LUEGO, SOLO PARA COMPROBAR
    cout << "Cantidad de arcos a leer: " << totalArcos << endl;

    for (int i = 0; i < totalArcos; i++) {
        getline(ejemplo, linea);
        stringstream arco(linea);
        string dato;
        // Leer origen hasta espacio
        getline(arco, dato, ' ');
        int origen = stoi(dato);

        // Leer "->"
        getline(arco, dato, ' '); // temp = "->"

        // Leer destino
        getline(arco, dato);
        int destino = stoi(dato);

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
Jugador jugador;
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
        mostrarHabitacion(habitacionActual, jugador);

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
