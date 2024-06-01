#include <iostream>
#include <queue>

using namespace std;

struct Pasajero {
    string nombre;
};

struct Viaje {
    char id[50];
    char matricula[50];
    char nombreEmbarcacion[50];
    char fecha[9]; // Formato: YYYYMMDD
    float precio;
    char destino[50];
    int capacidad;
    queue<Pasajero> pasajeros;
    Viaje* izq;
    Viaje* der;
    int altura;
};

Viaje* crearViaje(const char* id, const char* matricula, const char* nombreEmbarcacion, const char* fecha, float precio, const char* destino, int capacidad) {
    Viaje* nuevoViaje = (Viaje*) malloc(sizeof(Viaje));
    for (int i = 0; i < 50; ++i) {
        nuevoViaje->id[i] = id[i];
        nuevoViaje->matricula[i] = matricula[i];
        nuevoViaje->nombreEmbarcacion[i] = nombreEmbarcacion[i];
        nuevoViaje->destino[i] = destino[i];
    }
    for (int i = 0; i < 9; ++i) {
        nuevoViaje->fecha[i] = fecha[i];
    }
    nuevoViaje->precio = precio;
    nuevoViaje->capacidad = capacidad;
    nuevoViaje->izq = nullptr;
    nuevoViaje->der = nullptr;
    nuevoViaje->altura = 1;
    return nuevoViaje;
}

int obtenerAltura(Viaje* v) {
    if (v == nullptr) {
        return 0;
    }
    return v->altura;
}

int mayor(int a, int b) {
    return (a > b) ? a : b;
}

int obtenerBalance(Viaje* v) {
    if (v == nullptr) {
        return 0;
    }
    return obtenerAltura(v->izq) - obtenerAltura(v->der);
}

Viaje* rotarDerecha(Viaje* y) {
    Viaje* x = y->izq;
    Viaje* T2 = x->der;

    x->der = y;
    y->izq = T2;

    y->altura = mayor(obtenerAltura(y->izq), obtenerAltura(y->der)) + 1;
    x->altura = mayor(obtenerAltura(x->izq), obtenerAltura(x->der)) + 1;

    return x;
}

Viaje* rotarIzquierda(Viaje* x) {
    Viaje* y = x->der;
    Viaje* T2 = y->izq;

    y->izq = x;
    x->der = T2;

    x->altura = mayor(obtenerAltura(x->izq), obtenerAltura(x->der)) + 1;
    y->altura = mayor(obtenerAltura(y->izq), obtenerAltura(y->der)) + 1;

    return y;
}

Viaje* insertar(Viaje* nodo, const char* id, const char* matricula, const char* nombreEmbarcacion, const char* fecha, float precio, const char* destino, int capacidad) {
    if (nodo == nullptr) {
        return crearViaje(id, matricula, nombreEmbarcacion, fecha, precio, destino, capacidad);
    }

    if (string(id) < string(nodo->id)) {
        nodo->izq = insertar(nodo->izq, id, matricula, nombreEmbarcacion, fecha, precio, destino, capacidad);
    } else if (string(id) > string(nodo->id)) {
        nodo->der = insertar(nodo->der, id, matricula, nombreEmbarcacion, fecha, precio, destino, capacidad);
    } else {
        return nodo;
    }

    nodo->altura = 1 + mayor(obtenerAltura(nodo->izq), obtenerAltura(nodo->der));

    int balance = obtenerBalance(nodo);

    if (balance > 1 && string(id) < string(nodo->izq->id)) {
        return rotarDerecha(nodo);
    }

    if (balance < -1 && string(id) > string(nodo->der->id)) {
        return rotarIzquierda(nodo);
    }

    if (balance > 1 && string(id) > string(nodo->izq->id)) {
        nodo->izq = rotarIzquierda(nodo->izq);
        return rotarDerecha(nodo);
    }

    if (balance < -1 && string(id) < string(nodo->der->id)) {
        nodo->der = rotarDerecha(nodo->der);
        return rotarIzquierda(nodo);
    }

    return nodo;
}

Viaje* buscar(Viaje* raiz, const char* id) {
    if (raiz == nullptr || string(raiz->id) == string(id)) {
        return raiz;
    }

    if (string(raiz->id) < string(id)) {
        return buscar(raiz->der, id);
    }

    return buscar(raiz->izq, id);
}

void inOrden(Viaje* raiz) {
    if (raiz != nullptr) {
        inOrden(raiz->izq);
        cout << "ID: " << raiz->id << ", Embarcacion: " << raiz->nombreEmbarcacion << ", Fecha: " << raiz->fecha << ", Destino: " << raiz->destino << ", Precio: " << raiz->precio << ", Capacidad: " << raiz->capacidad << endl;
        inOrden(raiz->der);
    }
}

Viaje* eliminarViaje(Viaje* raiz, const char* id) {
    if (raiz == nullptr) {
        return raiz;
    }

    if (string(id) < string(raiz->id)) {
        raiz->izq = eliminarViaje(raiz->izq, id);
    } else if (string(id) > string(raiz->id)) {
        raiz->der = eliminarViaje(raiz->der, id);
    } else {
        if ((raiz->izq == nullptr) || (raiz->der == nullptr)) {
            Viaje* temp = raiz->izq ? raiz->izq : raiz->der;

            if (temp == nullptr) {
                temp = raiz;
                raiz = nullptr;
            } else {
                *raiz = *temp;
            }

            free(temp);
        } else {
            Viaje* temp = raiz->der;
            while (temp->izq != nullptr) {
                temp = temp->izq;
            }

            for (int i = 0; i < 50; ++i) {
                raiz->id[i] = temp->id[i];
                raiz->matricula[i] = temp->matricula[i];
                raiz->nombreEmbarcacion[i] = temp->nombreEmbarcacion[i];
                raiz->destino[i] = temp->destino[i];
            }
            for (int i = 0; i < 9; ++i) {
                raiz->fecha[i] = temp->fecha[i];
            }
            raiz->precio = temp->precio;
            raiz->capacidad = temp->capacidad;
            raiz->pasajeros = temp->pasajeros;

            raiz->der = eliminarViaje(raiz->der, temp->id);
        }
    }

    if (raiz == nullptr) {
        return raiz;
    }

    raiz->altura = 1 + mayor(obtenerAltura(raiz->izq), obtenerAltura(raiz->der));

    int balance = obtenerBalance(raiz);

    if (balance > 1 && obtenerBalance(raiz->izq) >= 0) {
        return rotarDerecha(raiz);
    }

    if (balance > 1 && obtenerBalance(raiz->izq) < 0) {
        raiz->izq = rotarIzquierda(raiz->izq);
        return rotarDerecha(raiz);
    }

    if (balance < -1 && obtenerBalance(raiz->der) <= 0) {
        return rotarIzquierda(raiz);
    }

    if (balance < -1 && obtenerBalance(raiz->der) > 0) {
        raiz->der = rotarDerecha(raiz->der);
        return rotarIzquierda(raiz);
    }

    return raiz;
}

bool registrarPasajero(Viaje* raiz, const char* id, const char* nombre) {
    Viaje* viaje = buscar(raiz, id);
    if (viaje != nullptr) {
        if (viaje->pasajeros.size() < viaje->capacidad) {
            Pasajero pasajero = {nombre};
            viaje->pasajeros.push(pasajero);
            return true;
        } else {
            cout << "Capacidad maxima alcanzada para este viaje." << endl;
        }
    } else {
        cout << "Viaje no encontrado." << endl;
    }
    return false;
}

void listarPasajeros(Viaje* raiz, const char* id) {
    Viaje* viaje = buscar(raiz, id);
    if (viaje != nullptr) {
        queue<Pasajero> tempQueue = viaje->pasajeros;
        while (!tempQueue.empty()) {
            Pasajero p = tempQueue.front();
            tempQueue.pop();
            cout << p.nombre << endl;
        }
    } else {
        cout << "Viaje no encontrado." << endl;
    }
}

void generarId(char* id, const char* matricula, const char* fecha) {
    int i = 0;
    for (; i < 2; ++i) {
        id[i] = matricula[i];
    }
    for (int j = 0; j < 8; ++j, ++i) {
        id[i] = fecha[j];
    }
    id[i] = '\0';
}

int main() {
    Viaje* raiz = nullptr;
    int opc = 0;

    do {
        cout << "Menu de opciones" << endl;
        cout << "1. Registrar un nuevo viaje" << endl;
        cout << "2. Buscar un viaje por identificador" << endl;
        cout << "3. Listar todos los viajes" << endl;
        cout << "4. Eliminar un viaje por identificador" << endl;
        cout << "5. Registrar un pasajero en un viaje" << endl;
        cout << "6. Listar todos los pasajeros de un viaje" << endl;
        cout << "7. Salir" << endl;
        cin >> opc;

        switch (opc) {
            case 1: {
                char matricula[50], nombreEmbarcacion[50], fecha[9], destino[50];
                float precio;
                int capacidad;
                cout << "Matricula de la embarcacion: ";
                cin >> matricula;
                cout << "Nombre de la embarcacion: ";
                cin >> nombreEmbarcacion;
                cout << "Fecha del viaje (YYYYMMDD): ";
                cin >> fecha;
                cout << "Precio del viaje: ";
                cin >> precio;
                cout << "Destino del viaje: ";
                cin >> destino;
                cout << "Capacidad de la embarcacion: ";
                cin >> capacidad;

                char id[50];
                generarId(id, matricula, fecha);
                raiz = insertar(raiz, id, matricula, nombreEmbarcacion, fecha, precio, destino, capacidad);
                break;
            }
            case 2: {
                char id[50];
                cout << "ID del viaje: ";
                cin >> id;
                Viaje* viaje = buscar(raiz, id);
                if (viaje != nullptr) {
                    cout << "ID: " << viaje->id << ", Embarcacion: " << viaje->nombreEmbarcacion << ", Fecha: " << viaje->fecha << ", Destino: " << viaje->destino << ", Precio: " << viaje->precio << ", Capacidad: " << viaje->capacidad << endl;
                } else {
                    cout << "Viaje no encontrado." << endl;
                }
                break;
            }
            case 3:
                cout << "Lista de viajes (inOrden):" << endl;
                inOrden(raiz);
                break;
            case 4: {
                char id[50];
                cout << "ID del viaje: ";
                cin >> id;
                raiz = eliminarViaje(raiz, id);
                break;
            }
            case 5: {
                char id[50], nombre[50];
                cout << "ID del viaje: ";
                cin >> id;
                cout << "Nombre del pasajero: ";
                cin >> nombre;
                if (!registrarPasajero(raiz, id, nombre)) {
                    cout << "No se pudo registrar el pasajero." << endl;
                }
                break;
            }
            case 6: {
                char id[50];
                cout << "ID del viaje: ";
                cin >> id;
                cout << "Lista de pasajeros:" << endl;
                listarPasajeros(raiz, id);
                break;
            }
            case 7:
                cout << "Saliendo..." << endl;
                break;
            default:
                cout << "Opcion invalida. Intente de nuevo." << endl;
                break;
        }
    } while (opc != 7);

    // Liberar memoria del árbol AVL
    while (raiz != nullptr) {
        raiz = eliminarViaje(raiz, raiz->id);
    }
    
    return 0;
}

