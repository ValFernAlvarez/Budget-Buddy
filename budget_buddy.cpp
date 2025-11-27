// ========== Budget Buddy: A Personal Finance Tracker ==========

// ----------------------- LIBRERÍAS ----------------------
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <iomanip>
#include <limits>
#include <algorithm>

// ----------------------- NAMESPACE ----------------------
using namespace std;

// ----------------------- ESTRUCTURAS ----------------------
struct Gasto {
    double monto;
    string categoria;
    string fecha;
    string descripcion;
};
