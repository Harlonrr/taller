#include <iostream>
#include <string>
#include <queue>
#include <limits>

using namespace std;

// Struct for passengers
struct Pasajero {
    string nombre;
};

// Struct for trips (nodo)
struct Viaje {
    string id;
    string matricula;
    string nombreEmbarcacion;
    string fecha; // Format: YYYYMMDD
    float precio;
    string destino;
    int capacidad;
    queue<Pasajero> pasajeros;
    Viaje* izq;
    Viaje* der;
    int altura;
};

// Function to create a new trip
Viaje* crearViaje(string id, string matricula, string nombreEmbarcacion, string fecha, float precio, string destino, int capacidad) {
    Viaje* nuevoViaje = new Viaje();
    nuevoViaje->id = id;
    nuevoViaje->matricula = matricula;
    nuevoViaje->nombreEmbarcacion = nombreEmbarcacion;
    nuevoViaje->fecha = fecha;
    nuevoViaje->precio = precio;
    nuevoViaje->destino = destino;
    nuevoViaje->capacidad = capacidad;
    nuevoViaje->izq = nullptr;
    nuevoViaje->der = nullptr;
    nuevoViaje->altura = 1;
    return nuevoViaje;
}

// Function to get the height of a node
int obtenerAltura(Viaje* v) {
    if (v == nullptr) {
        return 0;
    }
    return v->altura;
}

// Function to get the maximum of two integers
int mayor(int a, int b) {
    return (a > b) ? a : b;
}

// Function to get the balance factor of a node
int obtenerBalance(Viaje* v) {
    if (v == nullptr) {
        return 0;
    }
    return obtenerAltura(v->izq) - obtenerAltura(v->der);
}

// Function to right rotate a subtree rooted with y
Viaje* rotarDerecha(Viaje* y) {
    Viaje* x = y->izq;
    Viaje* T2 = x->der;

    x->der = y;
    y->izq = T2;

    y->altura = mayor(obtenerAltura(y->izq), obtenerAltura(y->der)) + 1;
    x->altura = mayor(obtenerAltura(x->izq), obtenerAltura(x->der)) + 1;

    return x;
}

// Function to left rotate a subtree rooted with x
Viaje* rotarIzquierda(Viaje* x) {
    Viaje* y = x->der;
    Viaje* T2 = y->izq;

    y->izq = x;
    x->der = T2;

    x->altura = mayor(obtenerAltura(x->izq), obtenerAltura(x->der)) + 1;
    y->altura = mayor(obtenerAltura(y->izq), obtenerAltura(y->der)) + 1;

    return y;
}

// Function to insert a new trip into the AVL tree
Viaje* insertar(Viaje* nodo, string id, string matricula, string nombreEmbarcacion, string fecha, float precio, string destino, int capacidad) {
    if (nodo == nullptr) {
        return crearViaje(id, matricula, nombreEmbarcacion, fecha, precio, destino, capacidad);
    }

    if (id < nodo->id) {
        nodo->izq = insertar(nodo->izq, id, matricula, nombreEmbarcacion, fecha, precio, destino, capacidad);
    } else if (id > nodo->id) {
        nodo->der = insertar(nodo->der, id, matricula, nombreEmbarcacion, fecha, precio, destino, capacidad);
    } else {
        return nodo;
    }

    nodo->altura = 1 + mayor(obtenerAltura(nodo->izq), obtenerAltura(nodo->der));

    int balance = obtenerBalance(nodo);

    if (balance > 1 && id < nodo->izq->id) {
        return rotarDerecha(nodo);
    }

    if (balance < -1 && id > nodo->der->id) {
        return rotarIzquierda(nodo);
    }

    if (balance > 1 && id > nodo->izq->id) {
        nodo->izq = rotarIzquierda(nodo->izq);
        return rotarDerecha(nodo);
    }

    if (balance < -1 && id < nodo->der->id) {
        nodo->der = rotarDerecha(nodo->der);
        return rotarIzquierda(nodo);
    }

    return nodo;
}

// Function to find a trip by its ID
Viaje* buscar(Viaje* raiz, string id) {
    if (raiz == nullptr || raiz->id == id) {
        return raiz;
    }

    if (raiz->id < id) {
        return buscar(raiz->der, id);
    }

    return buscar(raiz->izq, id);
}

// Function to perform an in-order traversal to list all trips
void inOrden(Viaje* raiz) {
    if (raiz != nullptr) {
        inOrden(raiz->izq);
        cout << "ID: " << raiz->id << ", Embarcacion: " << raiz->nombreEmbarcacion << ", Fecha: " << raiz->fecha << ", Destino: " << raiz->destino << ", Precio: " << raiz->precio << ", Capacidad: " << raiz->capacidad << endl;
        inOrden(raiz->der);
    }
}

// Function to delete the entire tree
void deleteTree(Viaje* raiz) {
    if (raiz != nullptr) {
        deleteTree(raiz->izq);
        deleteTree(raiz->der);
        delete raiz;
    }
}

// Function to generate a unique trip ID
string generarId(string matricula, string fecha) {
    return matricula.substr(0, 2) + fecha;
}

// Function to register a new passenger
bool registrarPasajero(Viaje* raiz, string id, string nombre) {
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

// Function to list all passengers for a trip
void listarPasajeros(Viaje* raiz, string id) {
    Viaje* viaje = buscar(raiz, id);
    if (viaje != nullptr) {
        queue<Pasajero> tempQueue = viaje->pasajeros;
        while (!tempQueue.empty()) {
            Pasajero pasajero = tempQueue.front();
            cout << pasajero.nombre << " ";
            tempQueue.pop();
        }
        cout << endl;
    } else {
        cout << "Viaje no encontrado." << endl;
    }
}

// Function to delete a node in AVL tree
Viaje* eliminarViaje(Viaje* raiz, string id) {
    if (raiz == nullptr) {
        return raiz;
    }

    if (id < raiz->id) {
        raiz->izq = eliminarViaje(raiz->izq, id);
    } else if (id > raiz->id) {
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

            delete temp;
        } else {
            Viaje* temp = raiz->der;
            while (temp->izq != nullptr) {
                temp = temp->izq;
            }

            raiz->id = temp->id;
            raiz->matricula = temp->matricula;
            raiz->nombreEmbarcacion = temp->nombreEmbarcacion;
            raiz->fecha = temp->fecha;
            raiz->precio = temp->precio;
            raiz->destino = temp->destino;
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
                string matricula, nombreEmbarcacion, fecha, destino;
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

                string id = generarId(matricula, fecha);
                raiz = insertar(raiz, id, matricula, nombreEmbarcacion, fecha, precio, destino, capacidad);
                break;
            }
            case 2: {
                string id;
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
                string id;
                cout << "ID del viaje: ";
                cin >> id;
                raiz = eliminarViaje(raiz, id);
                break;
            }
            case 5: {
                string id, nombre;
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
                string id;
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

    deleteTree(raiz);
    return 0;
}
