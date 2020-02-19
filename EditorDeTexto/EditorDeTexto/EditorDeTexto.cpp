#include <iostream>
#include <cstdlib>
#include <windows.h>
#include <conio.h>
#include<stdlib.h>
#include <fstream>
#include <string>

#define CTRL(c) ((c) & 037)

using namespace std;

struct NodoChara {
    char Chara;
    int Linea;
    struct NodoChara* Siguiente;
    struct NodoChara* Anterior;
};

struct NodoPila {
    string Buscada;
    string Remplazo;
    string Estado;
    string Palabra;
    //Linea y Columna
    int Posicion;
    struct NodoPila* Siguiente;
};

struct NodoCircular {
    string Nombre;
    string Ruta;
    struct NodoCircular* Siguiente;
};

//Menu Principal
void Principal();
//Registro de Recientes
void Reciente(string);
void SeleccionReciente();
//Editor de texto
void Editor(string);
//Reportes
void Reportes(NodoChara*&, NodoPila*&, NodoPila*&);
//Metodos de la lista doblemente enlazada
void InsertarChar(NodoChara*&, char, int);
string MostrarChara(NodoChara*&);
string MostrarLDoble(NodoChara*&);
bool BorrarUltimo(NodoChara*&);
void Llenar(NodoChara*&, string);
int Remplazo(NodoChara*&, string, string, NodoPila*);
//Metodos de la pila
void Push(NodoPila*&, string, string, string, string, int);
NodoPila* Pop(NodoPila*& Cabeza);
string MostrarPila(NodoPila*&);
//Metodos de la lista circular
void InsertarCircular(NodoCircular*&, string, string);
string MostrarCircular(NodoCircular*&);
//Metodos de soporte
void gotoxy(int, int);

string Recientes[5];

int main(){
    for (int i = 0; i < 5; i++) {
        Recientes[i] = "";
    }
    Principal();
    return 0;
}

void Principal() {
    int Opcion = 1;
    do{
        bool Repetir = true;
        do{
            int Tecla;
            system("cls");
            //Titulo
            cout << "\t----Menu Principal----\n";
            //Opcion
            cout << "\n\t1. Crear Archivo\n";
            cout << "\t2. Abrir Archivo\n";
            cout << "\t3. Archivos Recientes\n";
            cout << "\t4. Salir\n";
            cout << "\n\t----------------------";
            gotoxy(1, 1 + Opcion); cout << "  ==>";
            do {
                Tecla = _getch();
                //72 es tecla abajo, 80 tecla arriba y 13 es enter
            } while (Tecla != 72 && Tecla != 80 && Tecla != 13);
            switch (Tecla){
                case 72:
                    Opcion--;
                    if (Opcion == 0) {
                        Opcion = 4;
                    }
                break;
                case 80:
                    Opcion++;
                    if (Opcion == 5) {
                        Opcion = 1;
                    }
                    break;
                case 13:
                    Repetir = false;
                break;
            }
        } while (Repetir);
        //Seleccion
        string URL;
        system("cls");
        switch (Opcion) {
            case 1:
                Editor("");
            break;
            case 2:
                cout << "\tInserte la dirrecion del archivo(use /): ";
                URL = "";
                getline(cin, URL);
                Editor(URL);
            break;
            case 3:
                SeleccionReciente();
            break;
        }
    } while (Opcion != 4);
    system("cls");
    cout << "\t----------------------\n";
    cout << "\tFinalizando Programa\n";
    cout << "\t----------------------";
}

void Reciente(string URL) {
    int Pos = 4;
    for (int i = 0; i < 5; i++) {
        if (URL == Recientes[i]) {
            Pos = i;
        }
    }
    for (int i = Pos; i > 0; i--) {
        Recientes[i] = Recientes[i - 1];
    }
    Recientes[0] = URL;
}

void SeleccionReciente(){
    system("cls");
    if (Recientes[0] != "") {
        int Opcion = 1;
        do {
            int Tecla;
            system("cls");
            //Titulo
            cout << "\t----Menu Recientes----\n";
            //Opcion
            int Max = 0;;
            for (int i = 0; i < 5; i++) {
                if (Recientes[i] != "") {
                    Max++;
                }
            }
            for (int i = 0; i < Max; i++) {
                cout << "\n\t" + to_string((i+1)) + ". " + Recientes[i] + "";
            }
            cout << "\n\n\t----------------------";
            cout << "\n\tRegresar [Esc]";
            gotoxy(1, 1 + Opcion); cout << "  ==>";
            do {
                Tecla = _getch();
                //72 es tecla abajo, 80 tecla arriba y 13 es enter
            } while (Tecla != 72 && Tecla != 80 && Tecla != 13 && Tecla != 27);
            switch (Tecla) {
            case 72:
                Opcion--;
                if (Opcion == 0) {
                    Opcion = Max;
                }
                break;
            case 80:
                Opcion++;
                if (Opcion == (Max + 1)) {
                    Opcion = 1;
                }
                break;
            case 13:
                Editor(Recientes[Opcion]);
                break;
            case 27:
                system("cls");
                cout << "\tregresando al menu principal\n";
                system("pause");
                Opcion = 10;
                break;
            }
        } while (Opcion != 10);
    } else {
        cout << "\tNo hay archivos Recientes, regresando al menu principal";
        system("pause");
    }
}

void Editor(string URL) {
    NodoChara* Caracteres = NULL;
    NodoPila* Realizados = NULL;
    NodoPila* Revertidos = NULL;
    int Lineas=1;
    int LineaActual=1;
    int Tecla = 0;
    bool Guardado;
    if (URL == "") {
        Guardado = false;
    } else {
        fstream Archivo(URL);
        string Text;
        if (!Archivo.is_open()) {
            Archivo.open(URL, ios::out);
        }
        string Texto = "";
        bool Primera = true;
        while (getline(Archivo, Text)){
            if (!Primera) {
                Texto += "\n";
                Lineas++;
                LineaActual++;
            } else {
                Primera = !Primera;
            }
            Texto += Text;
        }
        for (int i = 0; i < Texto.length(); i++) {
            InsertarChar(Caracteres, Texto[i], 0);
        }
        cout << "\tArchivo Cargado con exito\n\t";
        Reciente(URL);
        system("pause");
        Archivo.close();
        Guardado = true;
    }
    do{
        system("cls");
        cout << "\t---------------------Editor---------------------\n\n";
        gotoxy(0, 1 + Lineas); cout << "\t--------------------Acciones--------------------\n";
        gotoxy(0, 2 + Lineas); cout << "\t^w(Buscar Y Remplazar) ^c(Reportes) ^s(Guardar)\n";
        gotoxy(0, 1); cout << "\t" << MostrarChara(Caracteres);
        Tecla = _getch();
        char Letra = Tecla;
        string Buscar, RemplazoPor;
        bool Seguir;
        int Opcion, Cambios;
        switch (Letra){
            case CTRL('w'):
                gotoxy(0, 3 + Lineas); cout << "\tBuscar(palabra sin espacios): ";
                Buscar = "";
                getline(cin, Buscar);
                cout << "\tRemplazar por(palabra sin espacios): ";
                RemplazoPor = "";
                getline(cin, RemplazoPor);
                Cambios = Remplazo(Caracteres, Buscar, RemplazoPor, Realizados);
                cout << "\tCambios Realizados: " + to_string(Cambios);
                cout << "\n\tRemplazo Realizado con exito\n\t";
                system("pause");
            break;
            case CTRL('c'):
                Seguir = true;
                if (!Guardado) {
                    Opcion = 1;
                    bool Repetir = true;
                    do {
                        int TeclaB;
                        system("cls");
                        //Titulo
                        cout << "\thay cambios sin guardar, ¿desea continuar?\n";
                        //Opcion
                        cout << "\n\t1. Si\n";
                        cout << "\t2. No\n";
                        gotoxy(1, 1 + Opcion); cout << "  ==>";
                        do {
                            TeclaB = _getch();
                            //72 es tecla abajo, 80 tecla arriba y 13 es enter
                        } while (TeclaB != 72 && TeclaB != 80 && TeclaB != 13);
                        switch (TeclaB) {
                        case 72:
                            Opcion--;
                            if (Opcion == 0) {
                                Opcion = 4;
                            }
                            break;
                        case 80:
                            Opcion++;
                            if (Opcion == 5) {
                                Opcion = 1;
                            }
                            break;
                        case 13:
                            Repetir = false;
                            if(Opcion == 1){
                                Seguir = true;
                            } else {
                                Seguir = false;
                            }
                            break;
                        }
                        if (Seguir) {
                            system("cls");
                            Tecla = 27;
                            Reportes(Caracteres, Realizados, Revertidos);
                        } else {
                            Tecla = 1;
                        }
                    } while (Repetir);
                }else {
                    system("cls");
                    Tecla = 27;
                    Reportes(Caracteres, Realizados, Revertidos);
                }
                break;
            case CTRL('s'):
                if (true) {
                    if (URL == "") {
                        gotoxy(0, 3 + Lineas); cout << "\tInserte la dirrecion a guardar el archivo(use /): ";
                        URL = "";
                        getline(cin, URL);
                        string Nombre = "";
                        cout << "\tInserte el nombre del archivo: ";
                        getline(cin, Nombre);
                        URL += "/" + Nombre + ".txt";
                    }
                    fstream Archivo(URL);
                    string Texto = MostrarChara(Caracteres);
                    if (!Archivo.is_open()) {
                        Archivo.open(URL, ios::out);
                    }
                    Archivo << Texto;
                    system("cls");
                    cout << "\tArchivo Guardado con exito\n\t";
                    Reciente(URL);
                    system("pause");
                    Archivo.close();
                    Guardado = true;
                }
                break;
            default:
                if (Tecla == 8) {
                    if (BorrarUltimo(Caracteres)) {
                        Lineas--;
                        LineaActual--;
                    }
                } else {
                    InsertarChar(Caracteres, Tecla, LineaActual);
                    if (Tecla == 13) {
                        Lineas++;
                        LineaActual++;
                    }
                }
                Guardado = false;
            break;
        }
    } while (Tecla != 27);
}

void Reportes(NodoChara*& Caracteres, NodoPila*& Realizados, NodoPila*& Revertidos) {
    cout << "\tCaracteres\n\t";
    cout << MostrarLDoble(Caracteres);
    cout << "\tCambios Realizados\n\t";
    cout << MostrarPila(Realizados);
    system("pause");
}

//Lista Doblemente Enlazada

void InsertarChar(NodoChara *&Cabeza, char Chara, int Linea) {
    NodoChara* Nuevo = new NodoChara();
    Nuevo->Chara = Chara;
    Nuevo->Linea = Linea;
    if (Cabeza == NULL) {
        Cabeza = Nuevo;
        Cabeza->Siguiente = NULL;
        Cabeza->Anterior = NULL;
    }
    else {
        NodoChara* Aux = Cabeza;
        while (Aux->Siguiente != NULL) {
            Aux = Aux->Siguiente;
        }
        Aux->Siguiente = Nuevo;
        Nuevo->Anterior = Aux;
        Nuevo->Siguiente = NULL;
    }
}

bool BorrarUltimo(NodoChara*& Cabeza) {
    char Chara;
    if (Cabeza != NULL) {
        NodoChara* Aux = Cabeza;
        while (Aux->Siguiente != NULL) {
            Aux = Aux->Siguiente;
        }
        Chara = Aux->Chara;
        if (Aux == Cabeza) {
            Cabeza = NULL;
        } else {
            Aux = Aux->Anterior;
            Aux->Siguiente = NULL;
        }
    }
    return (Chara == 13 || Chara == '\n');
}

string MostrarChara(NodoChara *&Cabeza) {
    if (Cabeza != NULL) {
        NodoChara* Aux = Cabeza;
        string Texto;
        while (Aux != NULL) {
            if (Aux->Chara == 13) {
                Texto += "\n\t";
            }else {
                Texto += Aux->Chara;
            }
            Aux = Aux->Siguiente;
        }
        return Texto;
    }else {
        return "";
    }
}

string MostrarLDoble(NodoChara *&Cabeza) {
    if (Cabeza != NULL) {
        NodoChara* Aux = Cabeza;
        string Texto;
        int i = 1;
        while (Aux != NULL) {
            if (Aux->Chara == 13 || Aux->Chara == '\n') {
                Texto += to_string(i) + ". Caracter: Salto de linea, Linea: " + to_string(Aux->Linea) + "\n";
            }
            else {
                Texto += to_string(i) + ". Caracter: " + (1, Aux->Chara) + ", Linea: " + to_string(Aux->Linea) + "\n";
            }
            Aux = Aux->Siguiente;
            i++;
        }
        return Texto;
    }
    else {
        return "";
    }
}

int Remplazo(NodoChara *&Cabeza, string Buscada, string Remplazo, NodoPila* Realizados) {
    int Cambios = 0;
    int Pos = 0;
    if (Cabeza != NULL) {
        NodoChara* Aux = Cabeza;
        while (Aux != NULL) {
            if (Aux->Chara == Buscada[0]) {
                bool remplazar = true;
                NodoChara* Aux2 = Aux;
                for (int i = 1; i < Buscada.length(); i++) {
                    Aux2 = Aux2->Siguiente;
                    if (Aux2 == NULL && Aux->Chara != Buscada[i]) {
                        remplazar = false;
                        i = Buscada.length();
                    }
                }
                if (remplazar) {
                    Aux2 = Aux2->Siguiente;
                    int Linea = Aux->Linea;
                    Aux->Chara = Remplazo[0];
                    for (int i = 1; i < Remplazo.length(); i++) {
                        NodoChara* Nuevo = new NodoChara();
                        Nuevo->Chara = Remplazo[i];
                        Nuevo->Linea = Linea;
                        Aux->Siguiente = Nuevo;
                        Nuevo->Anterior = Aux;
                        Aux = Aux->Siguiente;
                    }
                    if (Aux2 != NULL) {
                        Aux2->Anterior = Aux;
                        Aux->Siguiente = Aux2;
                    }
                    Push(Realizados, Buscada, Remplazo, "No Revertido", "NULL", Pos);
                    Cambios++;
                }
            }
            Pos++;
            Aux = Aux->Siguiente;
        }
    }
    return Cambios;
}

//Pila de Cambios

void Push(NodoPila*& Cabeza, string Buscada, string Remplazo, string Estado, string Palabra, int Posicion) {
    NodoPila* Nuevo = new NodoPila();
    Nuevo->Buscada = Buscada;
    Nuevo->Remplazo = Remplazo;
    Nuevo->Estado = Estado;
    Nuevo->Palabra = Palabra;
    Nuevo->Posicion = Posicion;
    if (Cabeza == NULL) {
        Cabeza = Nuevo;
        Cabeza->Siguiente = NULL;
    } else {
        Nuevo->Siguiente = Cabeza;
        Cabeza = Nuevo;
    }
}

NodoPila* Pop(NodoPila*& Cabeza) {
    if (Cabeza != NULL) {
        NodoPila* Aux = Cabeza;
        Cabeza = Cabeza->Siguiente;
        return Cabeza;
    }else {
        return NULL;
    }
};

string MostrarPila(NodoPila*& Cabeza) {
    if (Cabeza != NULL) {
        NodoPila* Aux = Cabeza;
        string Texto = "";
        int i = 1;
        while (Aux != NULL) {
            Texto += to_string(i) + ". Palabra Buscada: " + Aux->Buscada + ", Remplazada por: " + Aux->Remplazo + ", Estado: " + Aux->Estado + ", Palabra: " + Aux->Palabra + ", Posicion: " + to_string(Aux->Posicion) + "\n";
            Aux = Aux->Siguiente;
            i++;
        }
        return Texto;
    } else {
        return "Pila Vacia";
    }
}

//Lista Circular

void InsertarCircular(NodoCircular*& Cabeza, string Nombre, string Ruta) {
    NodoCircular* Nuevo = new NodoCircular();
    Nuevo->Nombre = Nombre;
    Nuevo->Ruta = Ruta;
    if (Cabeza == NULL) {
        Cabeza = Nuevo;
        Cabeza->Siguiente = Cabeza;
    } else {
        NodoCircular* Aux = Cabeza;
        while (Aux->Siguiente != Cabeza) {
            Aux = Aux->Siguiente;
        }
        Aux->Siguiente = Nuevo;
        Nuevo->Siguiente = Cabeza;
    }
}

string MostrarCircular(NodoCircular*& Cabeza) {
    if (Cabeza != NULL) {
        string Texto = "";
        NodoCircular* Aux = Cabeza;
        int i = 1;
        do {
            Texto += to_string(i) + ". Nombre del Archivo: " + Aux->Nombre + ", Ruta del Archivo: " + Aux->Ruta +"\n";
            Aux = Aux->Siguiente;
            i++;
        } while (Aux != Cabeza);
        return Texto;
    } else {
        return "Lista Vacia";
    }
}

void gotoxy(int x, int y) {
    COORD coordenada;
    coordenada.X = x;
    coordenada.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coordenada);
}