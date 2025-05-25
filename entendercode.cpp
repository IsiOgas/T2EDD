#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstdlib>
#include <ctime>
using namespace std;

class Jugador{
    private:
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
    void setPrecision(float p) {precision = p; }
    void setRecuperacion(int r) { recuperacion = r; }
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

struct Evento {
    string nombre;
    float probabilidad;
    string descripcion;
    string opcion_A;
    string loquesucede_A;
    string resultado_A;
    string opcion_B;
    string loquesucede_B;
    string resultado_B;

    bool usado = false;
};


Habitacion** ListaHabitaciones = nullptr;
int TotalHabitaciones = 0;

Enemigo** ListaEnemigos = nullptr;
int TotalEnemigos = 0;

Evento** ListaEventos = nullptr;
int TotalEventos = 0; 

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
    cout << "\nPuedes moverte de habitación!"<<endl;
    if (h->hijo1) cout << "1. Ir a " << h->hijo1->nombre << endl;
    if (h->hijo2) cout << "2. Ir a " << h->hijo2->nombre << endl;
    if (h->hijo3) cout << "3. Ir a " << h->hijo3->nombre << endl;
    
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
}

/******
*   void CargarEnemigos
******
*   Lee los enemigos asignados en un archivo y luego los guarda en un arreglo.
******
* Input:
*   ifstream& archivo : archivo de entrada con los datos de los enemigos.
******
* Returns:
*   void, no retorna nada -> modifica la variable global ListaEnemigos.
*****/

void CargarEnemigos(ifstream& archivo){
    string linea;
    while(getline(archivo,linea) && linea != "ENEMIGOS");
    getline(archivo,linea);
    TotalEnemigos = stoi(linea);
    ListaEnemigos = new Enemigo*[TotalEnemigos];
        
    for(int i = 0; i < TotalEnemigos;i++){
        getline(archivo, linea);
        stringstream enemy(linea);
        Enemigo* e = new Enemigo();
        string cursor; 

        getline(enemy, e->nombre, '|');

        while (enemy >> cursor) {
            if (cursor == "Vida") {
                enemy >> e->vida;
            } else if (cursor == "Ataque") {
                enemy >> e->ataque;
            } else if (cursor == "Precision") {
                enemy >> e->precision;
            } else if (cursor == "Probabilidad") {
                enemy >> e->probabilidad;
            }
        }
        ListaEnemigos[i] = e;
    }
}

/******
*   void AparicionEnemigos
******
*   Utiliza la función rand(), inicialmente para randomizar si saldrán 1 o 2 enemigos en dicha habitación,
*   luego recorre el arreglo global ListaEnemigos para randomizar cual enemigo saldrá, no se repitan en
*   la misma habitación y en las siguientes si tiene vida igual a 0
******
* Input:
*   Habitacion* H : Puntero a la habitacion que se desea aparecer un enemigo.
******
* Returns:
*   void, no retorna nada -> modifica las variables enemigos[] y cantidadEnemigosAsignados de la habitación.
*****/

void AparicionEnemigos(Habitacion* H){
    if (H->tipo == "COMBATE" && !H->enemigosAsignados){
        int Cantidad;
        if (rand() % 2 == 0) { //entre 0 y 32767
            Cantidad = 1;
        } else {
            Cantidad = 2;
        }

        H->cantidadEnemigosAsignados = 0;
        int Intentos = 0;
        
        while (H->cantidadEnemigosAsignados < Cantidad && Intentos < 20){ //Usamos ese 20 como tope para q no se vaya a bug, porque puede seguir buscando enemigos que ya no existen xd o nunca existieron.
            int IndiceAleatorio = rand() % TotalEnemigos; //Recordatorio % te entrega el resto en una división
            Enemigo* EnemigoElegido = ListaEnemigos[IndiceAleatorio];

            bool YaExiste = false;
            for(int i = 0; i < H->cantidadEnemigosAsignados; i++){
                if(H->enemigos[i] == EnemigoElegido){
                    YaExiste = true; //Para que, si llegan a salir 2, no se repita el mismo enemigo (me paso en un ejemplo y quede O.O ?)
                    i= H->cantidadEnemigosAsignados;
                }
            }
        
            if(!YaExiste && EnemigoElegido->vida > 0){ //El enemigo puede volver a aparecer, si no murio, ya que en la habitación doble solo peleas con 1.
            H->enemigos[H->cantidadEnemigosAsignados] = EnemigoElegido;
            H->cantidadEnemigosAsignados++;
            }

            Intentos++;
        }

        H->enemigosAsignados = true;

    }
}


bool hayEnemigosVivos(Habitacion* h) {
    for (int i = 0; i < h->cantidadEnemigosAsignados; i++) {
        if (h->enemigos[i]->vida > 0) return true;
    }
    return false;
}

void AplicarResultado(string resultado, Jugador& jugador){
    stringstream result(resultado);
    string caracteristicas;
    float valor;

    while(result >> valor >> caracteristicas) {
        if (caracteristicas == "VIDA" || caracteristicas == "vida" || caracteristicas == "Vida") {
            jugador.setVida(jugador.getVida() + valor);
        } else if (caracteristicas == "ATAQUE" || caracteristicas == "ataque" || caracteristicas == "Ataque") {
            jugador.setAtaque(jugador.getAtaque() + valor);
        } else if (caracteristicas == "PRECISION" || caracteristicas == "precision" || caracteristicas == "Precision") {
            jugador.setPrecision(jugador.getPrecision() + valor);
        } else if (caracteristicas == "RECUPERACION" || caracteristicas == "recuperacion" || caracteristicas == "Recuperacion") {
            jugador.setRecuperacion(jugador.getRecuperacion() + valor);
        }
    }
}

void CargarEventos(ifstream& archivo){
    string linea;
    while(getline(archivo,linea) && linea !="EVENTOS");
    getline(archivo,linea);
    TotalEventos = stoi(linea);
    ListaEventos = new Evento*[TotalEventos];

    for(int i = 0; i<TotalEventos; i++){
        Evento* ev = new Evento();
        getline(archivo,linea); //Estamos acá "&"
        getline(archivo,linea); //Estamos en nombre.
        ev->nombre = linea;

        getline(archivo,linea);
        stringstream event(linea);
        string Palabra;
        event >> Palabra >> ev->probabilidad; // "Probabilidad 0.3" toma esa linea, luego ignora palabra y toma el num.

        getline(archivo, ev->descripcion);

        getline(archivo, ev->opcion_A);
        getline(archivo, ev->loquesucede_A);
        getline(archivo, ev->resultado_A);

        getline(archivo, ev->opcion_B);
        getline(archivo, ev->loquesucede_B);
        getline(archivo, ev->resultado_B);

        ListaEventos[i] = ev;
    }
}


void SucedeEvento(Habitacion* H){
    if(H->tipo == "EVENTO"){
        float random_num = (rand() % 1000)/1000.0; //no supe como hacer del 0 al 1 asi q ese es del 0 a al 0.999
        float suma = 0;
        Evento* cualevento = nullptr;

        for(int i = 0; i < TotalEventos; i++){
            if(!ListaEventos[i]->usado){
                suma += ListaEventos[i]->probabilidad;
                if(random_num <= suma && cualevento == nullptr){
                    cualevento = ListaEventos[i];
                }
            }
        }

        if(cualevento == nullptr){ //se acabaron los eventos o no hay eventos disponibles
            return;
        }

        cualevento->usado = true; //le ponemos q el evento fue asignado y ya no se puede asignar en otra habitación.

        cout << "\n--- Evento: " << cualevento->nombre << " ---\n";
        cout << cualevento->descripcion << endl;
        cout << cualevento->opcion_A << endl;
        cout << cualevento->opcion_B << endl;
        cout << "Elige A o B: ";

        char Eleccion;
        cin >> Eleccion;
        if(Eleccion == 'A' || Eleccion == 'a'){
            cout << cualevento->loquesucede_A << endl;
            cout << cualevento->resultado_A <<endl;
            AplicarResultado(cualevento->resultado_A, jugador);
            cout << "-------------------------------" << endl;
            cout << "Estado actual del jugador:" << endl;
            cout << "Vida: " << jugador.getVida() << endl;
            cout << "Ataque: " << jugador.getAtaque() << endl;
            cout << "Precisión: " << jugador.getPrecision() << endl;
            cout << "Recuperación: " << jugador.getRecuperacion() << endl;
            cout << "-------------------------------" << endl;
        } else if (Eleccion == 'B' || Eleccion == 'b'){
            cout << cualevento->loquesucede_B << endl;
            cout << cualevento->resultado_B << endl; //AplicarResultado(cualevento->resultado_B, jugador)
            AplicarResultado(cualevento->resultado_B, jugador);
            cout << "-------------------------------" << endl;
            cout << "Estado actual del jugador:" << endl;
            cout << "Vida: " << jugador.getVida() << endl;
            cout << "Ataque: " << jugador.getAtaque() << endl;
            cout << "Precisión: " << jugador.getPrecision() << endl;
            cout << "Recuperación: " << jugador.getRecuperacion() << endl;
            cout << "-------------------------------" << endl;
        } else{
            cout << "Por favor, selecciona A o B.";
        }
    }
}


int main() {
    ifstream archivo("ejemplo.map");
    if (!archivo.is_open()) {
        cerr << "Error: No se pudo abrir el archivo." << endl;
        return 1;
    }

    srand(time(nullptr)); //Para que se vaya cambiando el núm q entrega la función rand cada vez q se reinicia el programa.

    CargarHabitaciones(archivo);
    CargarArcos(archivo);
    CargarEnemigos(archivo);
    CargarEventos(archivo);

    Habitacion* habitacionActual = ListaHabitaciones[0];
    bool JuegoActivo = true;

    while (JuegoActivo) {
        mostrarHabitacion(habitacionActual);
        AparicionEnemigos(habitacionActual);
        SucedeEvento(habitacionActual);

        if (habitacionActual->cantidadEnemigosAsignados > 0) {
        cout << "\nEnemigos en esta habitación:" << endl;
            for (int i = 0; i < habitacionActual->cantidadEnemigosAsignados; i++) {
                Enemigo* e = habitacionActual->enemigos[i];
                cout << "- " << e->nombre 
                 << " (Vida: " << e->vida 
                 << ", Ataque: " << e->ataque
                 << ", Precisión: " << e->precision 
                 << ", Probabilidad: " << e->probabilidad << ")" << endl;
             }
        }

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

    for (int i = 0; i < TotalEnemigos; i++) {
        delete ListaEnemigos[i];
    }
    delete[] ListaEnemigos;

    for (int i = 0; i < TotalEventos; i++) {
        delete ListaEventos[i];
    }
    delete[] ListaEventos;

    return 0;
}
