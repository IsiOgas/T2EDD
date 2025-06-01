#include <iostream>
#include <fstream>
#include <sstream> //Para usar stringstream
#include <string>
#include <cstdlib>
#include <ctime>
using namespace std;


//Para TDA
struct Nodo{
    string nombreTurno; // Puede ser "jugador" o el nombre de un enemigo
    Nodo *sig;
};

struct Enemigo{
    string nombre;
    int vida;
    int ataque;
    float precision;
    float probabilidad; // Probabilidad de aparecer en una habitación de combate
};

struct Habitacion{
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

struct UpgradeCombate{
    int vida=0;
    float precision=0.0;
    int ataque=0;
    int recuperacion=0;

};

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
    void setPrecision(float p) { precision = p; }
    void setRecuperacion(int r) { recuperacion = r; }
};


// funciones para TDA Cola
class TDACola{
    private:
        Nodo *head;
        Nodo *tail;
       
    public:
        // métodos de la clase
        TDACola(){ // constructor
            head = nullptr;
            tail = nullptr;
        }
    bool vacia() { return head == nullptr; }

    void encolar(string nombre){
        Nodo *nuevo = new Nodo;
        nuevo->nombreTurno = nombre;
        nuevo->sig = nullptr;

        if (vacia()){
            head = tail = nuevo;
        }else{
            tail->sig = nuevo;
            tail = nuevo;
        }
    }

    string desencolar(){
        if (vacia()){
            return ""; // o lanzar un error
        }

        Nodo *temp = head;
        string nombre = temp->nombreTurno;

        head = head->sig;
        if (head == nullptr){
            tail = nullptr;
        }

        delete temp;

        return nombre;
    }

    string TurnoSgte(){
        if (vacia()){
            return 0;
        }
        // Guardamos el nombre del primer turno
        string nombre = desencolar(); // Lo sacamos del frente
        encolar(nombre); // Lo metemos de nuevo al final
        return nombre; // Y lo devolvemos como el turno actual
    }
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


Jugador jugador;
bool juegoActivo = true;

Enemigo** ListaEnemigos = nullptr;
int TotalEnemigos = 0;

Habitacion** ListaHabitaciones = nullptr;
int TotalHabitaciones = 0;

int totalArcos;

int TotalMejoras;
UpgradeCombate **ListaMejoras= nullptr;

Evento** ListaEventos = nullptr;
int TotalEventos = 0; 



bool hayEnemigosVivos(Habitacion *h){
    for (int i = 0; i < h->cantidadEnemigosAsignados; i++){
        if (h->enemigos[i]->vida > 0){
            return true;
        }
    }
    return false;
}



void mostrarEstado(Jugador &jugador, Habitacion *h){
    // Mostrar nombres
    cout << "Jugador | ";
    for (int i = 0; i < h->cantidadEnemigosAsignados; ++i){
        cout << h->enemigos[i]->nombre << "| ";
    }
    cout << endl;

    // Mostrar vida o X
    if (jugador.getVida() > 0){
        cout << jugador.getVida() << " | ";
    }else{
        cout << "X  |";
    }
    for (int i = 0; i < h->cantidadEnemigosAsignados; i++){
        if (h->enemigos[i]->vida > 0){
            cout << h->enemigos[i]->vida << " | ";
        }else{
            cout << "X | ";
        }
    }
    cout << endl;
}



void mostrarMejorasCombate(Jugador &jugador, UpgradeCombate* ListaMejoras[], int TotalMejoras){

    int MejCom1 = rand() % TotalMejoras; //Mejora 1: Es el resto de un numero muy grande(rand) dividido entre el total de mejoras
    int MejCom2; //Se declara Mejora 2
    do {    //Hacer(Buscar en este caso) Mejora 2 mientras Mejora 2 es igual a Mejora 1. Aqui buscando que M1 sea distinta de M2
        MejCom2 = rand() % TotalMejoras;
    } while (MejCom2 == MejCom1);

    //ELIMINAR LUEGO
    cout << "Mejora indices: " << MejCom1 << ", " << MejCom2 << endl;

    UpgradeCombate* mejora1 = ListaMejoras[MejCom1]; //Punteros a las mejoras seleccionadas 
    UpgradeCombate* mejora2 = ListaMejoras[MejCom2];

    cout << "Debes decidir: " << endl;
        
    cout << " 1.";
    if (mejora1->vida > 0){
        cout << " Recuperar " << mejora1->vida << " de vida." << endl;
    }else if (mejora1->ataque > 0){ //Si la mejora 1 seleccionada fue vida, se realiza el cout correspondiente 
        cout << " Aumentar ataque en " << mejora1->ataque << "." << endl;
    }else if (mejora1->precision > 0){
        cout << " Aumentar precisión en " << mejora1->precision << "." << endl;
    }else if (mejora1->recuperacion > 0){ 
        cout << " Aumentar recuperación en " << mejora1->recuperacion << "." << endl;
    }
    
    cout << " 2.";
    if (mejora2->vida > 0){
        cout << " Recuperar " << mejora2->vida << " de vida." << endl;
    }else if (mejora2->ataque > 0){
        cout << " Aumentar ataque en " << mejora2->ataque << "." << endl;
    }else if (mejora2->precision > 0){
        cout << " Aumentar precisión en " << mejora2->precision << "." << endl;
    }else if (mejora2->recuperacion > 0){ 
        cout << " Aumentar recuperación en " << mejora2->recuperacion << "." << endl;
    }
    

    int eleccion;
    cin >> eleccion;
    while (eleccion != 1 && eleccion != 2) {
        cout << "Opción inválida. Intente de nuevo: ";
        cin >> eleccion;
    }

    //  ? se refiere a si la condicion es verdadera, : se refiere a si la condicion es falsa.
    UpgradeCombate* elegida = (eleccion == 1) ? mejora1 : mejora2;
    //Si la eleccion es igual a 1, entonces la elegida será igual a mejora1, si no será igual a mejora2.

    /* EQUIVALENTE CON IF
    UpgradeCombate* elegida;
    if (eleccion == 1) {
        elegida = mejora1;
    } else {
        elegida = mejora2;
    }
    */

    if (elegida->vida > 0) {
        jugador.setVida(jugador.getVida() + elegida->vida);
        cout << "¡Recuperaste " << elegida->vida << " de vida!" << endl;
    }
    else if (elegida->ataque > 0) {
        jugador.setAtaque(jugador.getAtaque() + elegida->ataque);
        cout << "¡Aumentaste tu ataque en " << elegida->ataque << "!" << endl;
    }
    else if (elegida->precision > 0) {
        jugador.setPrecision(jugador.getPrecision() + elegida->precision);
        cout << "¡Aumentaste tu precisión en " << elegida->precision << "!" << endl;
    }
    else if (elegida->recuperacion > 0) {
        jugador.setRecuperacion(jugador.getRecuperacion() + elegida->recuperacion);
        cout << "¡Aumentaste tu recuperación en " << elegida->recuperacion << "!" << endl;
    }

    cout << endl;


}


void turnoCombate(Jugador &jugador, Habitacion *h){
    TDACola colaTurnos;
    // Encolar jugador
    colaTurnos.encolar("Jugador");
    // Encolar enemigos (por nombre)
    for (int i = 0; i < h->cantidadEnemigosAsignados; i++){
        colaTurnos.encolar(h->enemigos[i]->nombre);
    }

    while (jugador.getVida() > 0 && hayEnemigosVivos(h)){
        string turnoActual = colaTurnos.TurnoSgte();
        bool huboAccion = false;
        if (turnoActual == "Jugador"){
            // === Turno del jugador ===
            for (int i = 0; i < h->cantidadEnemigosAsignados; i++){
                if (h->enemigos[i]->vida > 0){
                    if ((rand() % 1000) / 1000.0f <= jugador.getPrecision()){
                        h->enemigos[i]->vida -= jugador.getAtaque();
                        cout << "Jugador golpea a " << h->enemigos[i]->nombre << " por " << jugador.getAtaque() << " de daño!" << endl;
                    }else{
                        cout << "Jugador falla!" << endl;
                    }
                    huboAccion = true;
                    break;
                }
            }
        }else{
            // === Turno de un enemigo ===
            for (int i = 0; i < h->cantidadEnemigosAsignados; i++){
                if (h->enemigos[i]->nombre == turnoActual && h->enemigos[i]->vida > 0){
                    if ((rand() % 1000) / 1000.0f <= h->enemigos[i]->precision){
                        int nuevaVida = jugador.getVida() - h->enemigos[i]->ataque;
                        jugador.setVida(max(nuevaVida, 0));
                        cout << h->enemigos[i]->nombre << " golpea a Jugador por " << h->enemigos[i]->ataque << " de daño!" << endl;
                    }else{
                        cout << h->enemigos[i]->nombre << " falla!" << endl;
                    }
                    huboAccion = true;
                    break;
                }
            }
        }

        if (huboAccion){
            mostrarEstado(jugador, h);
        }
        cout << endl;
    }

    if (jugador.getVida() > 0){
        cout << "¡Ganaste el combate!" << endl;
        mostrarMejorasCombate(jugador, ListaMejoras, TotalMejoras);
        
    }else{
        cout << "Has sido derrotado..." << endl;
        juegoActivo=false; 
            
    }
}


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


void cargaMejorasCombate(ifstream &archivo){
    string linea;
    while (getline(archivo, linea) && linea != "MEJORAS DE COMBATE");
    
    getline(archivo, linea);
    TotalMejoras = stoi(linea);
    ListaMejoras= new UpgradeCombate*[TotalMejoras];

    //ELIMINAR LUEGO
    cout<< "Cantidad mejoras a leer: "<<TotalMejoras<<endl;

    for(int i=0; i<TotalMejoras; i++){
        UpgradeCombate* mejora = new UpgradeCombate();
        getline(archivo,linea); //Estamos en 1er mejora.

        
        
        cout<< "Linea a leer: "<<linea<<endl;

        stringstream Mejora(linea); 
        string tipo;
        float valor;
        char signo;

        Mejora >> signo >> valor >> tipo; // +4

        if (tipo == "Vida" || tipo == "vida"){
            mejora->vida = (int)valor;
        }else if (tipo == "Precision" || tipo == "precision"){
            mejora->precision = valor;
        }else if (tipo == "Ataque" || tipo == "ataque"){
            mejora->ataque = (int)valor;
        }else if (tipo == "Recuperacion" || tipo == "recuperacion"){
            mejora->recuperacion = (int)valor;
        }
        
        ListaMejoras[i] = mejora;
        
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

void AparicionEnemigos(Habitacion* h, Enemigo* ListaEnemigos[], int TotalEnemigos){
    int Cantidad;
        if (rand() % 2 == 0){ // entre 0 y 32767
            Cantidad = 1;
        }else{
            Cantidad = 2;
        }

        h->cantidadEnemigosAsignados = 0;
        int Intentos = 0;

        while (h->cantidadEnemigosAsignados < Cantidad && Intentos < 20){  // Usamos ese 20 como tope para q no se vaya a bug, porque puede seguir buscando enemigos que ya no existen xd o nunca existieron.
            int IndiceAleatorio = rand() % TotalEnemigos; // Recordatorio % te entrega el resto en una división
            Enemigo *EnemigoElegido = ListaEnemigos[IndiceAleatorio];

            bool YaExiste = false;
            for (int i = 0; i < h->cantidadEnemigosAsignados; i++){
                if (h->enemigos[i] == EnemigoElegido){
                    YaExiste = true; // Para que, si llegan a salir 2, no se repita el mismo enemigo (me paso en un ejemplo y quede O.O ?)
                    i = h->cantidadEnemigosAsignados;
                }
            }

            if (!YaExiste && EnemigoElegido->vida > 0){ // El enemigo puede volver a aparecer, si no murio, ya que en la habitación doble solo peleas con 1.
                h->enemigos[h->cantidadEnemigosAsignados] = EnemigoElegido;
                h->cantidadEnemigosAsignados++;
            }

            Intentos++;
        }

        h->enemigosAsignados = true;
}



Habitacion *elegirHabitacion(Habitacion *h){
    cout << "\nPuedes moverte de habitación!"<<endl;
    if (h->hijo1) cout << "1. Ir a " << h->hijo1->nombre << endl;
    if (h->hijo2) cout << "2. Ir a " << h->hijo2->nombre << endl;
    if (h->hijo3) cout << "3. Ir a " << h->hijo3->nombre << endl;
    
    int opcion;
    cin >> opcion;

    if (opcion == 1 && h->hijo1){
        return h->hijo1;
    }else if (opcion == 2 && h->hijo2){
        return h->hijo2;
    }else if (opcion == 3 && h->hijo3){
        return h->hijo3;
    }else{
        cout << "Opción inválida. Intenta de nuevo." << endl;
        return h; // se queda en la misma habitación
    }
}


void mostrarHabitacion(Habitacion *h, Jugador &jugador){
    if (h->id == 0){
        cout << "-- Habitación Inicial --" << endl;
        cout << h->descripcion << endl;
    }else if (h->tipo == "COMBATE" && !h->enemigosAsignados){

        AparicionEnemigos(h, ListaEnemigos, TotalEnemigos);

        if (h->cantidadEnemigosAsignados == 1){
            cout << "¡Te enfrentas al monstruo llamado " << h->enemigos[0]->nombre << "!" << endl;
        }else{
            cout << "¡Te enfrentas a dos monstruos: " << h->enemigos[0]->nombre << " y " << h->enemigos[1]->nombre << "!" << endl;
        }

        mostrarEstado(jugador, h);
        cout << "-------------------------" << endl;
        cout << endl;
        turnoCombate(jugador, h);
        cout<<endl;
    }
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
            cout << "\n-------------------------------" << endl;
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
            cout << "\n-------------------------------" << endl;
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
        mostrarHabitacion(habitacionActual, jugador);
        SucedeEvento(habitacionActual);

        if (habitacionActual->tipo == "FIN") {
            JuegoActivo = false;
        } else {
            habitacionActual = elegirHabitacion(habitacionActual);
            cout << endl;
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

    for (int i = 0; i < TotalMejoras; ++i){
        delete ListaMejoras[i];
    }
    delete[] ListaMejoras;

    return 0;
}
