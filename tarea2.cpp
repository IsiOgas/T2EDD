#include <iostream>
#include <fstream>
#include <sstream> //Para usar stringstream
#include <string>
#include <cstdlib>
#include <ctime>
using namespace std;

// Structs necesarios
class Jugador
{
private:
    int vida;
    int ataque;
    float precision;
    int recuperacion;

public:
    // Constructor para inicializar
    Jugador()
    {

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
struct Nodo
{
    string nombreTurno; // Puede ser "jugador" o el nombre de un enemigo
    Nodo *sig;
};
// funciones para TDA Cola
class TDACola
{
private:
    Nodo *head;
    Nodo *tail;
    Nodo *curr;
    unsigned int listSize;
    unsigned int pos; // posicion actual en la lista
public:
    // métodos de la clase
    TDACola()
    { // constructor
        head = nullptr;
        tail = nullptr;
        listSize = 0;
    }
    bool vacia() { return head == nullptr; }

    void encolar(string nombre)
    {
        Nodo *nuevo = new Nodo;
        nuevo->nombreTurno = nombre;
        nuevo->sig = nullptr;

        if (vacia())
        {
            head = tail = nuevo;
        }
        else
        {
            tail->sig = nuevo;
            tail = nuevo;
        }

        listSize++;
    }
    string desencolar()
    {
        if (vacia())
        {
            return ""; // o lanzar un error
        }

        Nodo *temp = head;
        string nombre = temp->nombreTurno;

        head = head->sig;
        if (head == nullptr)
        {
            tail = nullptr;
        }

        delete temp;
        listSize--;

        return nombre;
    }

    string TurnoSgte()
    {
        if (vacia())
            return "";

        // Guardamos el nombre del primer turno
        string nombre = desencolar(); // Lo sacamos del frente

        encolar(nombre); // Lo metemos de nuevo al final

        return nombre; // Y lo devolvemos como el turno actual
    }
};

struct Enemigo
{
    string nombre;
    int vida;
    int ataque;
    float precision;
    float probabilidad; // Probabilidad de aparecer en una habitación de combate
};

struct Habitacion
{
    int id;
    string nombre;
    string tipo; // "INICIO", "EVENTO", "COMBATE", "FIN"
    string descripcion;

    // Punteros a los hijos (árbol ternario)
    Habitacion *hijo1;
    Habitacion *hijo2;
    Habitacion *hijo3;

    Enemigo *enemigos[2]; // Hasta 2 enemigos por habitacion
    bool enemigosAsignados = false;
    int cantidadEnemigosAsignados;
};

Jugador jugador;
bool juegoActivo = true;
Enemigo **ListaEnemigos = nullptr;
int TotalEnemigos;

bool hayEnemigosVivos(Habitacion *h)
{
    for (int i = 0; i < h->cantidadEnemigosAsignados; i++)
    {
        if (h->enemigos[i]->vida > 0)
            return true;
    }
    return false;
}

void mostrarEstado(Jugador &jugador, Habitacion *h)
{
    // Mostrar nombres
    cout << "Jugador | ";
    for (int i = 0; i < h->cantidadEnemigosAsignados; ++i)
    {
        cout << h->enemigos[i]->nombre << "| ";
    }
    cout << endl;

    // Mostrar vida o X
    if (jugador.getVida() > 0)
        cout << jugador.getVida() << " | ";
    else
        cout << "X  |";

    for (int i = 0; i < h->cantidadEnemigosAsignados; i++)
    {
        if (h->enemigos[i]->vida > 0)
            cout << h->enemigos[i]->vida << " | ";
        else
            cout << "X | ";
    }
    cout << endl;
}

bool intentoAtaque(float precision)
{
    float dado = (rand() % 1000) / 1000.0f; // número entre 0.0 y 0.999
    return dado <= precision;
}

void turnoCombate(Jugador &jugador, Habitacion *h)
{
    TDACola colaTurnos;

    // Encolar jugador
    colaTurnos.encolar("Jugador");

    // Encolar enemigos (por nombre)
    for (int i = 0; i < h->cantidadEnemigosAsignados; i++)
    {
        colaTurnos.encolar(h->enemigos[i]->nombre);
    }

    while (jugador.getVida() > 0 && hayEnemigosVivos(h))
    {

        string turnoActual = colaTurnos.TurnoSgte();

        bool huboAccion = false;

        if (turnoActual == "Jugador")
        {
            // === Turno del jugador ===
            for (int i = 0; i < h->cantidadEnemigosAsignados; i++)
            {
                if (h->enemigos[i]->vida > 0)
                {
                    if ((rand() % 1000) / 1000.0f <= jugador.getPrecision())
                    {
                        h->enemigos[i]->vida -= jugador.getAtaque();
                        cout << "Jugador golpea a " << h->enemigos[i]->nombre << " por " << jugador.getAtaque() << " de daño!" << endl;
                    }
                    else
                    {
                        cout << "Jugador falla!" << endl;
                    }
                    huboAccion = true;
                    break;
                }
            }
        }
        else
        {
            // === Turno de un enemigo ===
            for (int i = 0; i < h->cantidadEnemigosAsignados; i++)
            {
                if (h->enemigos[i]->nombre == turnoActual && h->enemigos[i]->vida > 0)
                {
                    if ((rand() % 1000) / 1000.0f <= h->enemigos[i]->precision)
                    {
                        int nuevaVida = jugador.getVida() - h->enemigos[i]->ataque;
                        jugador.setVida(max(nuevaVida, 0));
                        cout << h->enemigos[i]->nombre << " golpea a Jugador por " << h->enemigos[i]->ataque << " de daño!" << endl;
                    }
                    else
                    {
                        cout << h->enemigos[i]->nombre << " falla!" << endl;
                    }
                    huboAccion = true;
                    break;
                }
            }
        }

        if (huboAccion)
        {
            mostrarEstado(jugador, h);
        }
        cout << endl;
    }

    if (jugador.getVida() > 0){
        cout << "¡Ganaste el combate!" << endl;
        cout << "Debes decidir: " << endl;
        cout << "1. Recuperar 5 de vida." << endl;
        cout << "2. Aumentar ataque en 1" << endl;

        int eleccion;
        cin >> eleccion;

        while (eleccion != 1 && eleccion != 2) {
            if (eleccion == 1){
                jugador.setVida(jugador.getVida() + 5);
                cout << "¡Recuperaste 5 de vida!" << endl;
            }else if (eleccion == 2){
                jugador.setAtaque(jugador.getAtaque() + 1);
                cout << "¡Aumentaste tu ataque en 1!" << endl;
            }else{
                cout << "Opción inválida. Intente de nuevo." << endl;
            }
            cout<<endl;
        }
    }else{
        cout << "Has sido derrotado..." << endl;
        juegoActivo=false; 
            
        
    }
}

void CargarEnemigos(ifstream &archivo)
{
    string linea;
    while (getline(archivo, linea) && linea != "ENEMIGOS")
        ;
    getline(archivo, linea);
    TotalEnemigos = stoi(linea);
    ListaEnemigos = new Enemigo *[TotalEnemigos];

    for (int i = 0; i < TotalEnemigos; i++)
    {
        getline(archivo, linea);
        stringstream enemy(linea);
        Enemigo *e = new Enemigo();
        string cursor;

        getline(enemy, e->nombre, '|');

        while (enemy >> cursor)
        {
            if (cursor == "Vida")
            {
                enemy >> e->vida;
            }
            else if (cursor == "Ataque")
            {
                enemy >> e->ataque;
            }
            else if (cursor == "Precision")
            {
                enemy >> e->precision;
            }
            else if (cursor == "Probabilidad")
            {
                enemy >> e->probabilidad;
            }
        }
        ListaEnemigos[i] = e;
    }
}

void mostrarHabitacion(Habitacion *h, Jugador &jugador)
{
    if (h->id == 0)
    {
        cout << "-- Habitación Inicial --" << endl;
        cout << h->descripcion << endl;
    }
    else if (h->tipo == "COMBATE" && !h->enemigosAsignados)
    {

        int Cantidad;
        if (rand() % 2 == 0)
        { // entre 0 y 32767
            Cantidad = 1;
        }
        else
        {
            Cantidad = 2;
        }

        h->cantidadEnemigosAsignados = 0;
        int Intentos = 0;

        while (h->cantidadEnemigosAsignados < Cantidad && Intentos < 20)
        {                                                 // Usamos ese 20 como tope para q no se vaya a bug, porque puede seguir buscando enemigos que ya no existen xd o nunca existieron.
            int IndiceAleatorio = rand() % TotalEnemigos; // Recordatorio % te entrega el resto en una división
            Enemigo *EnemigoElegido = ListaEnemigos[IndiceAleatorio];

            bool YaExiste = false;
            for (int i = 0; i < h->cantidadEnemigosAsignados; i++)
            {
                if (h->enemigos[i] == EnemigoElegido)
                {
                    YaExiste = true; // Para que, si llegan a salir 2, no se repita el mismo enemigo (me paso en un ejemplo y quede O.O ?)
                    i = h->cantidadEnemigosAsignados;
                }
            }

            if (!YaExiste && EnemigoElegido->vida > 0)
            { // El enemigo puede volver a aparecer, si no murio, ya que en la habitación doble solo peleas con 1.
                h->enemigos[h->cantidadEnemigosAsignados] = EnemigoElegido;
                h->cantidadEnemigosAsignados++;
            }

            Intentos++;
        }

        h->enemigosAsignados = true;

        if (h->cantidadEnemigosAsignados == 1)
        {
            cout << "¡Te enfrentas al monstruo llamado " << h->enemigos[0]->nombre << "!" << endl;
        }
        else
        {
            cout << "¡Te enfrentas a dos monstruos: " << h->enemigos[0]->nombre << " y " << h->enemigos[1]->nombre << "!" << endl;
        }

        mostrarEstado(jugador, h);
        cout << "-------------------------" << endl;
        cout << endl;
        turnoCombate(jugador, h);
        cout<<endl;
    }
    else
    {
        cout << "-- " << h->nombre << " --" << endl;
        cout << h->descripcion << endl;
    }

    if (h->hijo1 || h->hijo2 || h->hijo3)
    {
        cout << "A donde quieres ir?" << endl;
        if (h->hijo1)
        {
            cout << "1. " << h->hijo1->nombre << endl;
        }
        if (h->hijo2)
        {
            cout << "2. " << h->hijo2->nombre << endl;
        }
        if (h->hijo3)
        {
            cout << "3. " << h->hijo3->nombre << endl;
        }
        cout << "(Presiona la tecla correspondiente)" << endl;
    }
}

Habitacion *elegirHabitacion(Habitacion *h)
{
    int opcion;
    cin >> opcion;

    if (opcion == 1 && h->hijo1)
    {
        return h->hijo1;
    }
    else if (opcion == 2 && h->hijo2)
    {
        return h->hijo2;
    }
    else if (opcion == 3 && h->hijo3)
    {
        return h->hijo3;
    }
    else
    {
        cout << "Opción inválida. Intenta de nuevo." << endl;
        return h; // se queda en la misma habitación
    }
}

Habitacion **habitaciones = nullptr;
int totalHabitaciones;

void cargarHabitaciones(ifstream &ejemplo)
{
    string linea;

    // Leer hasta encontrar el número de habitaciones
    while (getline(ejemplo, linea) && linea != "HABITACIONES")
        ;

    getline(ejemplo, linea);

    totalHabitaciones = stoi(linea);

    // ELIMINAR LUEGO, SOLO PARA COMPROBAR
    cout << "Cantidad de habitaciones a leer: " << totalHabitaciones << endl;

    habitaciones = new Habitacion *[totalHabitaciones];

    for (int i = 0; i < totalHabitaciones; i++)
    {

        getline(ejemplo, linea);    // Leer línea completa
        stringstream cuarto(linea); // Nos permite recorrer por partes la linea que obtuvimos
        string dato;                // Declaramos variable donde se ira guardando cada valor que se extrae
        // Crear y guardar la habitación
        habitaciones[i] = new Habitacion();
        // Leer los campos separados por ';'
        getline(cuarto, dato, ';');
        habitaciones[i]->id = stoi(dato); // Convierte el id de string a int

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
void cargarArcos(ifstream &ejemplo)
{

    string linea;

    // Buscar la línea "ARCOS"
    while (getline(ejemplo, linea) && linea != "ARCOS")
        ;
    // Ahora leemos la cantidad de arcos que sigue a "ARCOS"
    getline(ejemplo, linea);
    totalArcos = stoi(linea);

    // ELIMINAR LUEGO, SOLO PARA COMPROBAR
    cout << "Cantidad de arcos a leer: " << totalArcos << endl;

    for (int i = 0; i < totalArcos; i++)
    {
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
        Habitacion *origenHabitacion = habitaciones[origen]; // Puntero al id de habitacion
        Habitacion *destinoHabitacion = habitaciones[destino];

        // Asignar el destino al primer hijo vacío
        if (origenHabitacion->hijo1 == nullptr)
        {
            origenHabitacion->hijo1 = destinoHabitacion;
        }
        else if (origenHabitacion->hijo2 == nullptr)
        {
            origenHabitacion->hijo2 = destinoHabitacion;
        }
        else if (origenHabitacion->hijo3 == nullptr)
        {
            origenHabitacion->hijo3 = destinoHabitacion;
        }
    }
}


int main()
{
    ifstream archivo;
    archivo.open("ejemplo.map");
    if (!archivo.is_open())
    {
        cout << "No se pudo abrir el archivo" << endl;
        return 0;
    }
    srand(time(nullptr));

    cargarHabitaciones(archivo);
    cargarArcos(archivo);
    CargarEnemigos(archivo);

    Habitacion *habitacionActual = habitaciones[0]; // puntero a la habitacion inicial

    

    while (juegoActivo)
    {
        mostrarHabitacion(habitacionActual, jugador);

        if (habitacionActual->tipo == "FIN")
        {
            juegoActivo = false; // Detiene el ciclo
        }
        else
        {
            habitacionActual = elegirHabitacion(habitacionActual);
            cout << endl;
        }
    }

    // Liberar memoria
    for (int i = 0; i < totalHabitaciones; i++)
    {
        delete habitaciones[i];
    }
    delete[] habitaciones;

    for (int i = 0; i < TotalEnemigos; ++i)
    {
        delete ListaEnemigos[i];
    }
    delete[] ListaEnemigos;

    return 0;
}
