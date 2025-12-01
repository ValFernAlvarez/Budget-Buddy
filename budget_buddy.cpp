// ========== Budget Buddy: A Personal Finance Tracker ==========

// hola aqui trabajaremos
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
using namespace std;

string archivo = "gastos.csv";

struct Gasto {
    float monto;
    string categoria;
    string fecha;
    string descripcion;
};

// ======================================================
// FUNCIONES DE VALIDACIÓN
// ======================================================

bool validarFecha(const string& fecha) {
    // Formato: YYYY-MM-DD
    if (fecha.length() != 10) return false;
    if (fecha[4] != '-' || fecha[7] != '-') return false;
    
    for (int i = 0; i < 10; i++) {
        if (i == 4 || i == 7) continue;
        if (!isdigit(fecha[i])) return false;
    }
    return true;
}

bool validarMonto(float monto) {
    return monto > 0;
}

// ======================================================
// FUNCIONES DE ARCHIVO (FORMATO VERTICAL)
// ======================================================

vector<Gasto> cargarGastos() {
    vector<Gasto> gastos;
    ifstream f(archivo);
    
    if (!f) return gastos;
    
    string linea;
    Gasto g;
    
    // Leer en formato VERTICAL
    while (getline(f, linea)) {
        g.monto = stof(linea);           // Línea 1: monto
        getline(f, g.categoria);         // Línea 2: categoría
        getline(f, g.fecha);             // Línea 3: fecha
        getline(f, g.descripcion);       // Línea 4: descripción
        
        gastos.push_back(g);
    }
    
    f.close();
    return gastos;
}

void guardarGastos(const vector<Gasto>& gastos) {
    ofstream f(archivo);
    
    for (const auto& g : gastos) {
        f << g.monto << endl;
        f << g.categoria << endl;
        f << g.fecha << endl;
        f << g.descripcion << endl;
    }
    
    f.close();
}

// ======================================================
// FUNCIONES DE GESTIÓN
// ======================================================

void mostrarLogo() {
    cout << "=========================================\n";
    cout << "    💰 CONTROL DE GASTOS PERSONALES 💰   \n";
    cout << "=========================================\n\n";
}

void agregarGasto() {
    vector<Gasto> gastos = cargarGastos();
    Gasto nuevo;
    
    cout << "\n--- NUEVO GASTO ---\n";
    
    // Monto con validación
    do {
        cout << "Monto: $";
        cin >> nuevo.monto;
        if (!validarMonto(nuevo.monto)) {
            cout << "Error: El monto debe ser mayor a 0\n";
        }
    } while (!validarMonto(nuevo.monto));
    cin.ignore();
    
    // Categoría
    cout << "Categoria: ";
    getline(cin, nuevo.categoria);
    
    // Fecha con validación
    do {
        cout << "Fecha (YYYY-MM-DD): ";
        getline(cin, nuevo.fecha);
        if (!validarFecha(nuevo.fecha)) {
            cout << "Error: Formato debe ser YYYY-MM-DD\n";
        }
    } while (!validarFecha(nuevo.fecha));
    
    // Descripción
    cout << "Descripcion: ";
    getline(cin, nuevo.descripcion);
    
    gastos.push_back(nuevo);
    guardarGastos(gastos);
    cout << "✅ Gasto agregado correctamente!\n";
}

void mostrarGastos() {
    vector<Gasto> gastos = cargarGastos();
    
    if (gastos.empty()) {
        cout << "No hay gastos registrados.\n";
        return;
    }
    
    cout << "\n--- TODOS LOS GASTOS ---\n";
    for (int i = 0; i < gastos.size(); i++) {
        cout << i + 1 << ". $" << gastos[i].monto 
             << " - " << gastos[i].categoria
             << " (" << gastos[i].fecha << ")\n";
        cout << "   " << gastos[i].descripcion << "\n\n";
    }
}

void eliminarGasto() {
    vector<Gasto> gastos = cargarGastos();
    
    if (gastos.empty()) {
        cout << "No hay gastos para eliminar.\n";
        return;
    }
    
    mostrarGastos();
    
    int indice;
    cout << "Numero de gasto a eliminar (0 para cancelar): ";
    cin >> indice;
    
    if (indice == 0) return;
    
    if (indice < 1 || indice > gastos.size()) {
        cout << "❌ Numero invalido\n";
        return;
    }
    
    gastos.erase(gastos.begin() + (indice - 1));
    guardarGastos(gastos);
    cout << "✅ Gasto eliminado correctamente!\n";
}

// ======================================================
// FUNCIONES DE ANÁLISIS
// ======================================================

void gastoTotal() {
    vector<Gasto> gastos = cargarGastos();
    float total = 0;
    
    for (const auto& g : gastos) {
        total += g.monto;
    }
    
    cout << "\n--- GASTO TOTAL ---\n";
    cout << "Total gastado: $" << total << endl;
    cout << "Cantidad de gastos: " << gastos.size() << endl;
}

void gastoPorCategoria() {
    vector<Gasto> gastos = cargarGastos();
    
    if (gastos.empty()) {
        cout << "No hay gastos registrados.\n";
        return;
    }
    
    cout << "\n--- GASTOS POR CATEGORIA ---\n";
    
    vector<string> categorias;
    vector<float> totales;
    
    for (const auto& g : gastos) {
        bool encontrado = false;
        for (int i = 0; i < categorias.size(); i++) {
            if (categorias[i] == g.categoria) {
                totales[i] += g.monto;
                encontrado = true;
                break;
            }
        }
        if (!encontrado) {
            categorias.push_back(g.categoria);
            totales.push_back(g.monto);
        }
    }
    
    for (int i = 0; i < categorias.size(); i++) {
        cout << "• " << categorias[i] << ": $" << totales[i] << endl;
    }
}

void gastoPorPeriodo() {
    vector<Gasto> gastos = cargarGastos();
    
    if (gastos.empty()) {
        cout << "No hay gastos registrados.\n";
        return;
    }
    
    cout << "\n--- GASTOS POR PERIODO ---\n";
    
    vector<string> meses;
    vector<float> totales;
    
    for (const auto& g : gastos) {
        string mes = g.fecha.substr(0, 7); // YYYY-MM
        
        bool encontrado = false;
        for (int i = 0; i < meses.size(); i++) {
            if (meses[i] == mes) {
                totales[i] += g.monto;
                encontrado = true;
                break;
            }
        }
        if (!encontrado) {
            meses.push_back(mes);
            totales.push_back(g.monto);
        }
    }
    
    for (int i = 0; i < meses.size(); i++) {
        cout << "• " << meses[i] << ": $" << totales[i] << endl;
    }
}

// ======================================================
// MENÚ PRINCIPAL
// ======================================================

void pausa() {
    cout << "\nPresione Enter para continuar...";
    cin.ignore();
    cin.get();
}

void menuPrincipal() {
    int opcion;
    
    do {
        system("cls");
        mostrarLogo();
        
        cout << "1. Agregar gasto\n";
        cout << "2. Ver todos los gastos\n";
        cout << "3. Eliminar gasto\n";
        cout << "4. Gasto total\n";
        cout << "5. Gastos por categoria\n";
        cout << "6. Gastos por periodo\n";
        cout << "7. Salir\n";
        cout << "\nSeleccione una opcion: ";
        cin >> opcion;
        cin.ignore();
        
        switch(opcion) {
            case 1: agregarGasto(); break;
            case 2: mostrarGastos(); break;
            case 3: eliminarGasto(); break;
            case 4: gastoTotal(); break;
            case 5: gastoPorCategoria(); break;
            case 6: gastoPorPeriodo(); break;
            case 7: cout << "¡Hasta pronto! 👋\n"; break;
            default: cout << "❌ Opcion invalida!\n";
        }
        
        if (opcion != 7) pausa();
        
    } while(opcion != 7);
}

int main() {
    menuPrincipal();
    return 0;
}
