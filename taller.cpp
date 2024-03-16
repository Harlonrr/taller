#include <iostream>
#include <cstring>

using namespace std;


struct RegistroTemperatura {
    char fecha[20];
    float temperatura_manana;
    float temperatura_tarde;
    float temperatura_noche;
    RegistroTemperatura* siguiente;
};


RegistroTemperatura* registrarTemperatura(RegistroTemperatura* cabeza) {
    RegistroTemperatura* nuevoRegistro = (RegistroTemperatura*)malloc(sizeof(RegistroTemperatura));

    cout << "Ingrese la fecha (DD/MM/AAAA): ";
    cin >> nuevoRegistro->fecha;
    cout << "Ingrese la temperatura de la mañana: ";
    cin >> nuevoRegistro->temperatura_manana;
    cout << "Ingrese la temperatura de la tarde: ";
    cin >> nuevoRegistro->temperatura_tarde;
    cout << "Ingrese la temperatura de la noche: ";
    cin >> nuevoRegistro->temperatura_noche;

    nuevoRegistro->siguiente = cabeza;
    return nuevoRegistro;
}


void mostrarRegistros(RegistroTemperatura* cabeza) {
    RegistroTemperatura* actual = cabeza;
    while (actual != NULL) {
        cout << "Fecha: " << actual->fecha << endl;
        cout << "Mañana: " << actual->temperatura_manana << " grados Celsius" << endl;
        cout << "Tarde: " << actual->temperatura_tarde << " grados Celsius" << endl;
        cout << "Noche: " << actual->temperatura_noche << " grados Celsius" << endl;
        actual = actual->siguiente;
    }
}

float promedioTemperaturaDia(RegistroTemperatura* cabeza) {
    RegistroTemperatura* actual = cabeza;
    float suma = 0;
    int contador = 0;
    while (actual != NULL) {
        suma += actual->temperatura_manana + actual->temperatura_tarde + actual->temperatura_noche;
        contador += 3; 
        actual = actual->siguiente;
    }
    return suma / contador;
}

float promedioTemperaturaTotal(RegistroTemperatura* cabeza) {
    RegistroTemperatura* actual = cabeza;
    float suma = 0;
    int contador = 0;
    while (actual != NULL) {
        suma += actual->temperatura_manana + actual->temperatura_tarde + actual->temperatura_noche;
        contador += 3; 
        actual = actual->siguiente;
    }
    return suma / contador;
}

int main() {
    RegistroTemperatura* cabeza = NULL;
    int opcion;

    do {
        cout << "\n1. Registrar temperatura\n";
        cout << "2. Mostrar registros\n";
        cout << "3. Calcular promedio de temperatura del día\n";
        cout << "4. Calcular promedio de temperatura total\n";
        cout << "5. Salir\n";
        cout << "Seleccione una opción: ";
        cin >> opcion;

        switch (opcion) {
            case 1:
                cabeza = registrarTemperatura(cabeza);
                break;
            case 2:
                if (cabeza != NULL)
                    mostrarRegistros(cabeza);
                else
                    cout << "No hay registros para mostrar.\n";
                break;
            case 3:
                if (cabeza != NULL)
                    cout << "Promedio de temperatura del día: " << promedioTemperaturaDia(cabeza) << " grados Celsius\n";
                else
                    cout << "No hay registros para calcular el promedio.\n";
                break;
            case 4:
                if (cabeza != NULL)
                    cout << "Promedio de temperatura total: " << promedioTemperaturaTotal(cabeza) << " grados Celsius\n";
                else
                    cout << "No hay registros para calcular el promedio.\n";
                break;
            case 5:
                cout << "Saliendo del programa...\n";
                break;
            default:
                cout << "Opción inválida. Por favor, seleccione nuevamente.\n";
        }
    } while (opcion != 5);

    return 0;
}
