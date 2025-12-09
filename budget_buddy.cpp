// ========== Budget Buddy ==========

// ----------------------- LIBRERÍAS ---------------------

# include <iostream>
# include <vector>
# include <cstring>
# include <fstream>
# include <iomanip>
# include <limits>
# include <algorithm>
# include <cstdlib>
# include <cctype>

// ----------------------- NAMESPACE Y ARCHIVO ----------------------

using namespace std;
const string nombre_archivo = "gastos.csv";
const string archivo_presupuestos = "presupuestos.csv";
char MES_ACTIVO[8] = "";
double presupuesto_mes = 0.0;

// ----------------------- ESTRUCTURAS --------------------

struct Gasto {
    double monto;
    char categoria[20];
    char fecha[12];
    char descripcion[50];
};
//
struct PresupuestoMes {
    char mes[8];   
    double monto;
};

// ----------------------- FUNCIONES BASICAS --------------------

void pausa() {
    system("pause");
}
//
void borrar_pantalla() {
    system("cls");
}
//
void limpiar_buffer() {
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

// ----------------------- FUNCIONES CSV ------------------------

void cargar_csv(vector<Gasto>& gastos) {
    ifstream archivo(nombre_archivo);
    // --------------------------------
    if (!archivo.is_open()) {
        cout << "\nNo se pudo abrir el archivo CSV. Se creara posteriormente. " << nombre_archivo;
        return;
    }
    // ---------------------------------
    char linea[256]; 
    archivo.getline(linea, sizeof(linea)); 
    archivo.getline(linea, sizeof(linea));
    // ---------------------------------
    while (archivo.getline(linea, sizeof(linea))) {
        if (linea[0] =='\0')
            continue; 
        char* p1 = strchr(linea, ',');
        if (!p1) 
            continue;
        char* p2 = strchr(p1 + 1, ',');
        if (!p2)
            continue;
        char* p3 = strchr(p2 + 1, ',');
        if (!p3)
            continue;
        // ---------------------------------
        char campo_monto[20];
        char campo_categoria[20];
        char campo_fecha[12];
        char campo_desc[60];
        // ---------------------------------
        int len1=p1-linea;
        strncpy(campo_monto, linea, len1);
        campo_monto[len1]='\0';
        int len2=p2-(p1+1);
        strncpy(campo_categoria, p1+1, len2);
        campo_categoria[len2]='\0';
        int len3=p3-(p2+1);
        strncpy(campo_fecha, p2+1, len3);
        campo_fecha[len3]='\0';
        strcpy(campo_desc, p3+1);
        // ---------------------------------
        Gasto g;
        g.monto = atof(campo_monto);
        strncpy(g.categoria, campo_categoria, sizeof(g.categoria)-1);
        g.categoria[sizeof(g.categoria)-1] = '\0';
        strncpy(g.fecha, campo_fecha, sizeof(g.fecha)-1);
        g.fecha[sizeof(g.fecha)-1] = '\0';
        strncpy(g.descripcion, campo_desc, sizeof(g.descripcion)-1);
        g.descripcion[sizeof(g.descripcion)-1] = '\0';
        // ---------------------------------
        gastos.push_back(g);
    }
    archivo.close();
}
// --------------------------------------------------------

void guardar_csv(const vector<Gasto>& gastos) {
    ofstream archivo(nombre_archivo.c_str());
    if (!archivo.is_open()) {
        cout<<"\nError escribiendo CSV.\n";
        return;
    }
    // ---------------------------------
    archivo <<"\nmonto,categoria,fecha,descripcion\n";
    // ---------------------------------
    for (const auto& g : gastos) {
        archivo<<g.monto<<","
                <<g.categoria<<","
                <<g.fecha<<","
                <<g.descripcion<<"\n";
    }
    // ---------------------------------
    archivo.close();
}

// --------------------------------------------------------

void cargar_presupuestos( vector<PresupuestoMes>& presupuesto) {
    ifstream archivo(archivo_presupuestos.c_str());
    if (!archivo.is_open()) {
        return;
    }
    // ---------------------------------
    char linea[64];
    while (archivo.getline(linea, sizeof(linea))) {
        if (linea[0] == '\0')
            continue;
        char* p = strchr(linea, ',');
        if (!p) continue;
        char campo_mes[8];
        int len = p - linea;
        if (len != 7) continue;
        strncpy(campo_mes, linea, len);
        campo_mes[len] = '\0';
        double monto = atof(p + 1);
        PresupuestoMes pr;
        strcpy(pr.mes, campo_mes);
        pr.monto = monto;
        presupuesto.push_back(pr);
    }
    // ---------------------------------
    archivo.close();
}

// --------------------------------------------------------

void guardar_presupuestos( vector<PresupuestoMes>& presupuesto) {
    ofstream archivo(archivo_presupuestos.c_str());
    if (!archivo.is_open()) {
        cout << "\nError escribiendo archivo de presupuestos.\n";
        return;
    }
    // ---------------------------------
    for (size_t i = 0; i < presupuesto.size(); ++i) {
        archivo << presupuesto[i].mes << ","
                << presupuesto[i].monto << "\n";
    }
    // ---------------------------------
    archivo.close();
}

// --------------------------------------------------------

double buscar_presupuesto_para_mes( vector<PresupuestoMes>& presupuesto, const char mes[]) {
    for (size_t i = 0; i < presupuesto.size(); ++i) {
        if (strcmp(presupuesto[i].mes, mes) == 0) {
            return presupuesto[i].monto;
        }
    }
    return 0.0;
}



// ---------------- FUNCION VALIDAR FECHA -----------------

bool validar_fecha(const char fecha[]) {
    if (strlen(fecha) != 10)
        return false;
    if (fecha[2] != '/' || fecha[5] != '/')
        return false;
    for (int i = 0; i < 10; ++i) {
        if (i == 2 || i == 5)
            continue;
        if (!isdigit(fecha[i]))
            return false;
    }
    int dia = (fecha[0] - '0') * 10 + (fecha[1] - '0');
    int mes = (fecha[3] - '0') * 10 + (fecha[4] - '0');
    if (mes < 1 || mes > 12)
        return false;
    if (dia < 1 || dia > 31)
        return false;
    return true;
}


// --------------------------------------------------------

bool validar_mes_anio(const char mesAnio[]) {
    if (strlen(mesAnio) != 7)
        return false;
    if (mesAnio[2] != '/')
        return false;
    if (!isdigit(mesAnio[0]) || !isdigit(mesAnio[1]))
        return false;
    for (int i = 3; i < 7; ++i) {
        if (!isdigit(mesAnio[i]))
            return false;
    }
    int mes = (mesAnio[0] - '0') * 10 + (mesAnio[1] - '0');
    if (mes < 1 || mes > 12)
        return false;
    return true;
}


// ------------------- SLECCIONAR MES ACTIVO ---------------

void seleccionar_mes_activo(vector<PresupuestoMes>& presupuesto ) {
    char buffer[8];
    while (true) {
        cout << "\nIngrese el mes a gestionar (mm/aaaa): ";
        cin.getline(buffer, sizeof(buffer));
        if (!validar_mes_anio(buffer)) {
            cout << "Formato invalido. Use mm/aaaa.\n";
            continue;
        }
        strcpy(MES_ACTIVO, buffer);
        break;
    }
    presupuesto_mes = buscar_presupuesto_para_mes(presupuesto,MES_ACTIVO);
}


// ---------------------------------------------------------



void configurar_presupuesto_mes(vector<PresupuestoMes>& presupuesto) {
    if (MES_ACTIVO[0] == '\0') {
        cout << "\nPrimero debe seleccionar un mes activo.\n";
        pausa();
        return;
    }
    borrar_pantalla();
    cout << "Configurar presupuesto para el mes: " << MES_ACTIVO << "\n\n";
    double p;
    cout << "Ingrese presupuesto del mes (monto > 0): ";
    if (!(cin >> p) || p <= 0) {
        cin.clear();
        limpiar_buffer();
        cout << "\nPresupuesto invalido.\n";
        pausa();
        return;
    }
    limpiar_buffer();
    presupuesto_mes = p;
    bool encontrado = false;
    for (size_t i = 0; i < presupuesto.size(); ++i) {
        if (strcmp(presupuesto[i].mes, MES_ACTIVO) == 0) {
            presupuesto[i].monto = p;
            encontrado = true;
            break;
        }
    }
    if (!encontrado) {
        PresupuestoMes pr;
        strcpy(pr.mes, MES_ACTIVO);
        pr.monto = p;
        presupuesto.push_back(pr);
    }
    guardar_presupuestos(presupuesto);
    cout << "\nPresupuesto guardado para " << MES_ACTIVO 
         << ": " << fixed << setprecision(2) << presupuesto_mes << "\n";
    pausa();
}


// ----------------------- FUNCIONES ----------------------


void registrar_gasto(vector<Gasto>& gastos) {
    Gasto g;
    // ---------------------------------
    cout << "Monto: ";
    cin >> g.monto;
    if (!cin || g.monto <= 0) {
        cout << "\nMonto invalido. Debe ser mayor a 0.\n";
        cin.clear();
        limpiar_buffer();
        return;
    }
    limpiar_buffer();
    // ---------------------------------
    bool error_categoria=true;
    while (error_categoria){
        cout<<"Categoria (sin comas): ";
        cin.getline(g.categoria, sizeof(g.categoria));
        if (g.categoria [0]=='\0'){
            cout<<"La categoria no puede estar vacia.\n";
            continue;
        }
        bool tiene_coma=false;
        for (int i=0 ; g.categoria[i]!='\0' ; ++i) {
            if (g.categoria[i]==',') {
                tiene_coma=true;
                break;
            }
        }
        if (tiene_coma) 
            cout<<"\nLa categoría no puede contener comas (,) por el CSV.\n";
        else 
            error_categoria=false;
    }
    // ---------------------------------
    bool error_fecha=true;
    while (error_fecha) {
        cout<<"Fecha (dd/mm/aaaa): ";
        cin.getline(g.fecha, sizeof(g.fecha));

        if (!validar_fecha(g.fecha)) {
            cout<<"Fecha invalida. Use formato dd/mm/aaaa.\n";
        } else {
            error_fecha=false;
        }
    }
    // ---------------------------------
    bool error_descripcion=true;
    while (error_descripcion){
        cout<<"Descripcion (sin comas): ";
        cin.getline(g.descripcion, sizeof(g.descripcion));
        if (g.descripcion [0]=='\0'){
            cout<<"\nLa descripcion no puede estar vacia.\n";
            continue;
        }
        bool tiene_coma=false;
        for (int i=0 ; g.descripcion[i]!='\0' ; ++i) {
            if (g.descripcion[i]==',') {
                tiene_coma=true;
                break;
            }
        }
        if (tiene_coma) 
            cout<<"\nLa descripcion no puede contener comas (,) por el CSV.\n";
        else 
            error_descripcion=false;
    }
    
    // ---------------------------------
    gastos.push_back(g);
    guardar_csv(gastos);
    cout<<"\nGasto registrado exitosamente.\n";
}

// ---------------------------------

void mostrar_gastos_mes(const vector<Gasto>& gastos) {
    borrar_pantalla();
    if (MES_ACTIVO[0] == '\0') {
        cout << "No hay mes activo seleccionado.\n";
        pausa();
        return;
    }
    cout << "Mes activo: " << MES_ACTIVO << "\n";
    bool hay = false;
    cout << "\nGastos del mes seleccionado:\n";
    cout << left << setw(5)  << "#"
         << setw(12) << "Monto"
         << setw(15) << "Categoria"
         << setw(12) << "Fecha"
         << "Descripcion\n";
    cout << "==============================================================\n";
    for (size_t i = 0; i < gastos.size(); ++i) {
        if (strncmp(gastos[i].fecha + 3, MES_ACTIVO, 7) == 0) {
            hay = true;
            cout << left << setw(5)  << i
                 << setw(12) << fixed << setprecision(2) << gastos[i].monto
                 << setw(15) << gastos[i].categoria
                 << setw(12) << gastos[i].fecha
                 << gastos[i].descripcion << "\n";
        }
    }
    if (!hay) {
        cout << "\nNo hay gastos registrados para este mes.\n";
    }
}


// ---------------------------------

void mostrar_gastos(const vector<Gasto>& gastos) {
    borrar_pantalla();
    if (gastos.empty()) {
        cout << "\nNo hay gastos registrados.\n";
        pausa();
        return;
    }
    // ---------------------------------
    cout<<"\nLista de gastos:\n";
    cout<<left<< setw(5)<<"#"<<setw(12)<<"Monto"<<setw(15)<<"Categoria"<<setw(12)<<"Fecha"<<"Descripcion\n";
    cout<<"==============================================================\n";
    // ---------------------------------
    for (size_t i=0; i<gastos.size(); ++i) {
        cout <<left<<setw(5)<<i
        <<setw(12)<<fixed<<setprecision(2)<<gastos[i].monto
        <<setw(15)<<gastos[i].categoria
        <<setw(12)<<gastos[i].fecha
        <<gastos[i].descripcion << "\n";
    }
}


// ---------------------------------

void eliminar_gasto(vector<Gasto>& gastos) {
    borrar_pantalla();
    if (gastos.empty()) {
        cout<<"\nNo hay gastos para eliminar.\n";
        pausa();
        return;
    }
    // ---------------------------------
    mostrar_gastos(gastos);
    // ---------------------------------
    cout << "\nIngrese el indice del gasto a eliminar: ";
    size_t indice;
    // ---------------------------------
    if (!(cin>>indice) || indice>=gastos.size()) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout<<"\nIndice invalido.\n";
        return;
    }
    // ---------------------------------
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    // ---------------------------------
    gastos.erase(gastos.begin()+indice);
    guardar_csv(gastos);
    // ---------------------------------
    cout<<"\nGasto eliminado.\n";
}

// ----------------------- REPORTES ----------------------

void reporte_total_general(const vector<Gasto>& gastos) {
    borrar_pantalla();
    if (gastos.empty()) {
        cout << "\nNo hay datos para calcular total general.";
        pausa();
        return;
    }
    double total = 0.0;
    for (size_t i = 0; i < gastos.size(); ++i) {
        total += gastos[i].monto;
    }
    cout << "\nREPORTE - TOTAL GENERAL\n";
    cout << "=====================================\n";
    cout << "Total gastado: " << fixed << setprecision(2) << total << "\n";
}


// ---------------------------------

void reporte_por_categoria_mes(const vector<Gasto>& gastos) {
    borrar_pantalla();
    if (MES_ACTIVO[0] == '\0') {
        cout << "No hay mes activo seleccionado.\n";
        pausa();
        return;
    }
    double total_mes = 0.0;
    const int MAX_CAT = 100;
    char categorias[MAX_CAT][20];
    double totales[MAX_CAT];
    int num_categorias = 0;
    for (size_t i = 0; i < gastos.size(); ++i) {
        if (strncmp(gastos[i].fecha + 3, MES_ACTIVO, 7) != 0)
            continue;
        total_mes += gastos[i].monto;
        bool encontrada = false;
        int pos = -1;
        for (int j = 0; j < num_categorias; ++j) {
            if (strcmp(categorias[j], gastos[i].categoria) == 0) {
                encontrada = true;
                pos = j;
                break;
            }
        }
        if (encontrada) {
            totales[pos] += gastos[i].monto;
        } else if (num_categorias < MAX_CAT) {
            strcpy(categorias[num_categorias], gastos[i].categoria);
            totales[num_categorias] = gastos[i].monto;
            num_categorias++;
        }
    }
    if (total_mes == 0.0) {
        cout << "\nNo hay gastos en el mes " << MES_ACTIVO << " para reportar.\n";
        pausa();
        return;
    }
    cout << "\nREPORTE POR CATEGORIA (mes " << MES_ACTIVO << ")\n";
    cout << "======================================================\n";
    cout << left << setw(15) << "Categoria"
         << setw(12) << "Total"
         << "Porcentaje\n";
    cout << "------------------------------------------------------\n";
    for (int i = 0; i < num_categorias; ++i) {
        double porcentaje = (totales[i] / total_mes) * 100.0;
        cout << left << setw(15) << categorias[i]
             << setw(12) << fixed << setprecision(2) << totales[i]
             << fixed << setprecision(2) << porcentaje << " %\n";
    }
    cout << "\nTotal del mes: " << fixed << setprecision(2) << total_mes << "\n";
    if (presupuesto_mes > 0.0) {
        double porcentaje_pres = (total_mes / presupuesto_mes) * 100.0;
        cout << "Presupuesto del mes: " << fixed << setprecision(2)
             << presupuesto_mes << "\n";
        cout << "Usado del presupuesto: " << fixed << setprecision(2)
             << porcentaje_pres << " %\n";
        if (total_mes > presupuesto_mes) {
            cout << "\n*** ALERTA: Se ha superado el presupuesto del mes. ***\n";
        }
    } else {
        cout << "\n(No hay presupuesto definido para este mes.)\n";
    }
}


// ---------------------------------

void reporte_por_categoria(const vector<Gasto>& gastos) {
    borrar_pantalla();
    if (gastos.empty()) {
        cout << "\nNo hay datos para generar el reporte por categoria.";
        pausa();
        return;
    }
    const int MAX_CAT = 100;
    char categorias[MAX_CAT][20];
    double totales[MAX_CAT];
    int num_categorias = 0;
    for (size_t i = 0; i < gastos.size(); ++i) {
        bool encontrada = false;
        int pos = -1;
        for (int j = 0; j < num_categorias; ++j) {
            if (strcmp(categorias[j], gastos[i].categoria) == 0) {
                encontrada = true;
                pos = j;
                break;
            }
        }
        if (encontrada) {
            totales[pos] += gastos[i].monto;
        } else {
            if (num_categorias < MAX_CAT) {
                strcpy(categorias[num_categorias], gastos[i].categoria);
                totales[num_categorias] = gastos[i].monto;
                num_categorias++;
            } else {
                cout << "\nSe alcanzo el maximo de categorias en el reporte.";
                break;
            }
        }
    }
    double total_general = 0.0;
    for (size_t i = 0; i < gastos.size(); ++i) {
        total_general += gastos[i].monto;
    }
    cout << "\nREPORTE POR CATEGORIA\n";
    cout << "======================================================\n";
    cout << left << setw(15) << "Categoria"
         << setw(12) << "Total"
         << "Porcentaje\n";
    cout << "------------------------------------------------------\n";
    for (int i = 0; i < num_categorias; ++i) {
        double porcentaje = (totales[i] / total_general) * 100.0;
        cout << left << setw(15) << categorias[i]<< setw(12) << fixed << setprecision(2) << totales[i]<< fixed << setprecision(2) << porcentaje << " %\n";
    }
    cout << "\nTotal general: " << fixed << setprecision(2) << total_general << "\n";
}

// ---------------------------------

void reporte_por_fecha(const vector<Gasto>& gastos) {
    borrar_pantalla();
    if (gastos.empty()) {
        cout << "\nNo hay datos para generar el reporte por fecha.";
        pausa();
        return;
    }
    const int MAX_FECHAS = 200;
    char fechas[MAX_FECHAS][12];
    double totales[MAX_FECHAS];
    int num_fechas = 0;
    for (size_t i = 0; i < gastos.size(); ++i) {
        bool encontrada = false;
        int pos = -1;
        for (int j = 0; j < num_fechas; ++j) {
            if (strcmp(fechas[j], gastos[i].fecha) == 0) {
                encontrada = true;
                pos = j;
                break;
            }
        }
        if (encontrada) {
            totales[pos] += gastos[i].monto;
        } else {
            if (num_fechas < MAX_FECHAS) {
                strcpy(fechas[num_fechas], gastos[i].fecha);
                totales[num_fechas] = gastos[i].monto;
                num_fechas++;
            } else {
                cout << "\nSe alcanzo el maximo de categorias en el reporte.";
                break;
            }
        }
    }
    double total_general = 0.0;
    for (size_t i = 0; i < gastos.size(); ++i) {
        total_general += gastos[i].monto;
    }
   cout << "\nREPORTE POR FECHA (dd/mm/aaaa)\n";
    cout << "=========================================\n";
    cout << left << setw(12) << "Fecha" << "Total\n";
    cout << "-----------------------------------------\n";

    for (int i = 0; i < num_fechas; ++i) {
        cout << left << setw(12) << fechas[i]
             << fixed << setprecision(2) << totales[i] << "\n";
    }
}



void reporte_por_mes(const vector<Gasto>& gastos) {
    borrar_pantalla();
    if (gastos.empty()) {
        cout << "\nNo hay datos para generar el reporte por mes.";
        pausa();
        return;
    }
    const int MAX_MESES = 200;
    char meses[MAX_MESES][8]; 
    double totales_mes[MAX_MESES];
    int num_meses = 0;
    for (size_t i = 0; i < gastos.size(); ++i) {
        char mes_actual[8];
        strncpy(mes_actual, gastos[i].fecha + 3, 7);
        mes_actual[7] = '\0';
        bool encontrado = false;
        int pos = -1;
        for (int j = 0; j < num_meses; ++j) {
            if (strcmp(meses[j], mes_actual) == 0) {
                encontrado = true;
                pos = j;
                break;
            }
        }

        if (encontrado) {
            totales_mes[pos] += gastos[i].monto;
        } else {
            if (num_meses < MAX_MESES) {
                strcpy(meses[num_meses], mes_actual);
                totales_mes[num_meses] = gastos[i].monto;
                num_meses++;
            } else {
                cout << "\nSe alcanzo el maximo de meses en el reporte.";
                break;
            }
        }
    }
        double total_general = 0.0;
    for (size_t i = 0; i < gastos.size(); ++i) {
        total_general += gastos[i].monto;
    }
    cout << "\nREPORTE POR MES (mm/aaaa)\n";
    cout << "===========================================================\n";
    cout << left << setw(10) << "Mes"
         << setw(12) << "Total"
         << "Porcentaje\n";
    cout << "-----------------------------------------------------------\n";
    for (int i = 0; i < num_meses; ++i) {
        double porcentaje = (totales_mes[i] / total_general) * 100.0;
        cout << left << setw(10) << meses[i]<< setw(12) << fixed << setprecision(2) << totales_mes[i]<< fixed << setprecision(2) << porcentaje << " %\n";
    }
    cout << "\nTotal general: " << fixed << setprecision(2) << total_general << "\n";
}


void reporte_por_dia(const vector<Gasto>& gastos) {
    borrar_pantalla();
    if (MES_ACTIVO[0] == '\0') {
        cout << "\nNo hay mes activo seleccionado.\n";
        return;
    }
    if (gastos.empty()) {
        cout << "\nNo hay datos para generar el reporte por dia.\n";
        return;
    }
    const int MAX_DIAS = 365; 
    char dias[MAX_DIAS][12]; 
    double totales[MAX_DIAS] = {0}; 
    int num_dias = 0; 
    double total_general_mes = 0.0;
    for (size_t i = 0; i < gastos.size(); ++i) {
        if (strncmp(gastos[i].fecha + 3, MES_ACTIVO, 7) != 0)
            continue;
        total_general_mes += gastos[i].monto;
        bool encontrado = false;
        int pos = -1;
        for (int j = 0; j < num_dias; ++j) {
            if (strcmp(dias[j], gastos[i].fecha) == 0) {
                encontrado = true;
                pos = j;
                break;
            }
        }
        if (encontrado) {
            totales[pos] += gastos[i].monto;
        } else {
            if (num_dias < MAX_DIAS) {
                strcpy(dias[num_dias], gastos[i].fecha);
                totales[num_dias] = gastos[i].monto;
                num_dias++;
            } else {
                cout << "\nSe alcanzo el maximo de dias en el reporte.\n";
                break;
            }
        }
    }
    if (total_general_mes == 0.0) {
        cout << "\nNo hay gastos en el mes " << MES_ACTIVO << " para reportar por dia.\n";
        return;
    }
    cout << "\nREPORTE POR DIA (mes " << MES_ACTIVO << ")\n";
    cout << "======================================================\n";
    cout << left << setw(15) << "Fecha"
         << setw(12) << "Total"
         << "Porcentaje\n";
    cout << "------------------------------------------------------\n";
    for (int i = 0; i < num_dias; ++i) {
        double porcentaje = (totales[i] / total_general_mes) * 100.0;
        cout << left << setw(15) << dias[i]
             << setw(12) << fixed << setprecision(2) << totales[i]
             << fixed << setprecision(2) << porcentaje << " %\n";
    }
    cout << "\nTotal del mes: " << fixed << setprecision(2) << total_general_mes << "\n";
    cout << "Dias con gastos registrados en el mes: " << num_dias << "\n";
}



void reporte_por_semana(const vector<Gasto>& gastos) {
    borrar_pantalla();
    if (MES_ACTIVO[0] == '\0') {
        cout << "\nNo hay mes activo seleccionado.\n";
        return;
    }
    if (gastos.empty()) {
        cout << "\nNo hay datos para generar el reporte por semana.\n";
        return;
    }
    auto extraer_fecha = [](const char fecha[], int& dia, int& mes, int& ano) {
        dia = (fecha[0] - '0') * 10 + (fecha[1] - '0');
        mes = (fecha[3] - '0') * 10 + (fecha[4] - '0');
        ano = (fecha[6] - '0') * 1000 + 
              (fecha[7] - '0') * 100 + 
              (fecha[8] - '0') * 10 + 
              (fecha[9] - '0');
    };
    auto dia_juliano = [](int dia, int mes, int ano) {
        int dias_mes[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};  
        if ((ano % 4 == 0 && ano % 100 != 0) || (ano % 400 == 0)) {
            dias_mes[1] = 29;
        }
        int total = 0;
        for (int i = 0; i < mes - 1; i++) {
            total += dias_mes[i];
        }
        total += dia;
        return total;
    };
    const int MAX_SEMANAS = 53; 
    struct SemanaInfo {
        int ano;
        int numero_semana;
        double total;
    };
    SemanaInfo semanas[MAX_SEMANAS];
    int num_semanas = 0;
    double total_general_mes = 0.0;
    for (size_t i = 0; i < gastos.size(); ++i) {
        if (strncmp(gastos[i].fecha + 3, MES_ACTIVO, 7) != 0)
            continue;
        int dia, mes, ano;
        extraer_fecha(gastos[i].fecha, dia, mes, ano);
        int dia_del_ano = dia_juliano(dia, mes, ano);
        int numero_semana = (dia_del_ano - 1) / 7 + 1;
        total_general_mes += gastos[i].monto;
        bool encontrada = false;
        int pos = -1;
        for (int j = 0; j < num_semanas; ++j) {
            if (semanas[j].ano == ano && semanas[j].numero_semana == numero_semana) {
                encontrada = true;
                pos = j;
                break;
            }
        }
        if (encontrada) {
            semanas[pos].total += gastos[i].monto;
        } else {
            if (num_semanas < MAX_SEMANAS) {
                semanas[num_semanas].ano = ano;
                semanas[num_semanas].numero_semana = numero_semana;
                semanas[num_semanas].total = gastos[i].monto;
                num_semanas++;
            } else {
                cout << "\nSe alcanzo el maximo de semanas en el reporte.\n";
                break;
            }
        }
    }
    if (total_general_mes == 0.0) {
        cout << "\nNo hay gastos en el mes " << MES_ACTIVO << " para reportar por semana.\n";
        return;
    }
    cout << "\nREPORTE POR SEMANA (mes " << MES_ACTIVO << ")\n";
    cout << "======================================================\n";
    cout << left << setw(10) << "Ano"
         << setw(10) << "Semana"
         << setw(12) << "Total"
         << "Porcentaje\n";
    cout << "------------------------------------------------------\n";
    
    for (int i = 0; i < num_semanas; ++i) {
        double porcentaje = (semanas[i].total / total_general_mes) * 100.0;
        cout << left << setw(10) << semanas[i].ano
             << setw(10) << semanas[i].numero_semana
             << setw(12) << fixed << setprecision(2) << semanas[i].total
             << fixed << setprecision(2) << porcentaje << " %\n";
    }
    cout << "\nTotal del mes: " << fixed << setprecision(2) << total_general_mes << "\n";
    cout << "Semanas con gastos registrados en el mes: " << num_semanas << "\n";
    cout << "\nNota: La semana 1 comienza el 1 de enero de cada ano.\n";
}


// ------------------- SUBMENU ANALISIS MES ACTIVO ---------------

void submenu_analisis(const vector<Gasto>& gastos) {
    while (true) {
        borrar_pantalla();
        cout << " MODULO DE ANALISIS (MES ACTIVO) \n";
        cout << " 1) Totales y % por categoria (mes activo) \n";
        cout << " 2) Reporte por dia (mes activo)           \n";
        cout << " 3) Reporte por semana (mes activo)        \n";
        cout << " 0) Volver                                 \n\n";
        cout << "Mes activo: "<< (MES_ACTIVO[0] ? MES_ACTIVO : "no definido")<< "   |   Presupuesto: ";
        if (presupuesto_mes > 0)
            cout << fixed << setprecision(2) << presupuesto_mes;
        else
            cout << "(no definido)";
        cout << "\n\n";
        cout << "Seleccione una opcion: ";
        int op;
        if (!(cin >> op)) {
            cin.clear();
            limpiar_buffer();
            cout << "Opcion invalida.\n";
            pausa();
            continue;
        }
        limpiar_buffer();
        switch (op) {
            case 1: reporte_por_categoria_mes(gastos); pausa(); break;
            case 2: reporte_por_dia(gastos); pausa(); break;
            case 3: reporte_por_semana(gastos); pausa(); break;
            case 0: return;
            default:
                cout << "Opcion invalida.\n";
                break;
        }
    }
}


// ------------------- SUBMENU REPORTES GLOBALES ---------------

void submenu_global(const vector<Gasto>& gastos) {
    while (true) {
        borrar_pantalla();
        cout << " REPORTES GLOBALES \n";
        cout << " 1) Total general\n";
        cout << " 2) Reporte por categoria (global)\n";
        cout << " 3) Reporte por fecha (global)\n";
        cout << " 4) Reporte por mes (global)\n";
        cout << " 0) Volver\n\n";
        cout << "Seleccione una opcion: ";
        int op;
        if (!(cin >> op)) {
            cin.clear();
            limpiar_buffer();
            cout << "Opcion invalida.\n";
            pausa();
            continue;
        }
        limpiar_buffer();
        switch (op){
        case 1: reporte_total_general(gastos); pausa(); break;
        case 2: reporte_por_categoria(gastos); pausa(); break;
        case 3: reporte_por_fecha(gastos); pausa(); break;
        case 4: reporte_por_mes(gastos); pausa(); break;
        case 0: return;
        
        default:
            cout << "Opcion invalida.\n";
            break;
        }
    }
}
void crear_grafico_off(const vector<Gasto>& gastos) {
    borrar_pantalla();
    
    if (gastos.empty()) {
        cout << "\nNo hay gastos para generar el gráfico.\n";
        pausa();
        return;
    }
    
    // Calcular totales por categoría
    map<string, double> totales_categoria;
    double total_general = 0.0;
    
    for (const auto& g : gastos) {
        string categoria = g.categoria;
        transform(categoria.begin(), categoria.end(), categoria.begin(), ::tolower);
        categoria.erase(0, categoria.find_first_not_of(" \t"));
        categoria.erase(categoria.find_last_not_of(" \t") + 1);
        
        totales_categoria[categoria] += g.monto;
        total_general += g.monto;
    }
    
    // Ordenar
    vector<pair<string, double>> categorias_ordenadas;
    for (const auto& cat : totales_categoria) {
        categorias_ordenadas.push_back(cat);
    }
    
    sort(categorias_ordenadas.begin(), categorias_ordenadas.end(),
         [](const pair<string, double>& a, const pair<string, double>& b) {
             return a.second > b.second;
         });
    
    // Crear archivo SVG
    string nombre_archivo = "grafico_gastos.svg";
    ofstream archivo(nombre_archivo);
    
    if (!archivo) {
        cout << "Error al crear el archivo SVG.\n";
        pausa();
        return;
    }
    
    // Colores para el gráfico
    vector<string> colores = {
        "#FF6B6B", "#4ECDC4", "#FFD166", "#06D6A0", "#118AB2",
        "#EF476F", "#073B4C", "#7209B7", "#F72585", "#3A86FF"
    };
    
    // Configuración SVG
    const int ANCHO = 800;
    const int ALTO = 600;
    const int RADIO = 200;
    const int CENTRO_X = ANCHO / 2;
    const int CENTRO_Y = ALTO / 2;
    
    // Encabezado SVG
    archivo << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
    archivo << "<!DOCTYPE svg PUBLIC \"-//W3C//DTD SVG 1.1//EN\" \"http://www.w3.org/Graphics/SVG/1.1/DTD/svg11.dtd\">\n";
    archivo << "<svg xmlns=\"http://www.w3.org/2000/svg\" width=\"" << ANCHO << "\" height=\"" << ALTO << "\">\n";
    
    // Fondo
    archivo << "  <rect width=\"100%\" height=\"100%\" fill=\"#f8f9fa\"/>\n";
    
    // Título
    archivo << "  <text x=\"" << ANCHO/2 << "\" y=\"40\" text-anchor=\"middle\" font-family=\"Arial\" font-size=\"24\" font-weight=\"bold\" fill=\"#333\">\n";
    archivo << "    📊 Distribución de Gastos\n";
    archivo << "  </text>\n";
    
    archivo << "  <text x=\"" << ANCHO/2 << "\" y=\"70\" text-anchor=\"middle\" font-family=\"Arial\" font-size=\"16\" fill=\"#666\">\n";
    archivo << "    Total: $" << fixed << setprecision(2) << total_general << "\n";
    archivo << "  </text>\n";
    
    // Calcular ángulos para el pastel
    double angulo_actual = 0;
    vector<double> angulos_inicio;
    vector<double> angulos_fin;
    
    for (size_t i = 0; i < categorias_ordenadas.size(); i++) {
        double porcentaje = (categorias_ordenadas[i].second / total_general);
        double angulo = porcentaje * 360.0;
        double angulo_fin = angulo_actual + angulo;
        
        angulos_inicio.push_back(angulo_actual);
        angulos_fin.push_back(angulo_fin);
        
        // Convertir ángulos a radianes para coordenadas
        double inicio_rad = (angulo_actual - 90) * M_PI / 180.0;
        double fin_rad = (angulo_fin - 90) * M_PI / 180.0;
        
        // Puntos para el arco
        double x1 = CENTRO_X + RADIO * cos(inicio_rad);
        double y1 = CENTRO_Y + RADIO * sin(inicio_rad);
        double x2 = CENTRO_X + RADIO * cos(fin_rad);
        double y2 = CENTRO_Y + RADIO * sin(fin_rad);
        
        // Dibujar segmento del pastel
        archivo << "  <path d=\"M " << CENTRO_X << " " << CENTRO_Y 
                << " L " << x1 << " " << y1 
                << " A " << RADIO << " " << RADIO << " 0 " 
                << (angulo > 180 ? "1" : "0") << " 1 "
                << x2 << " " << y2 
                << " Z\" fill=\"" << colores[i % colores.size()] 
                << "\" stroke=\"white\" stroke-width=\"2\"/>\n";
        
        // Etiqueta en el segmento
        double angulo_medio = (angulo_actual + angulo_fin) / 2.0;
        double angulo_medio_rad = (angulo_medio - 90) * M_PI / 180.0;
        double etiqueta_distancia = RADIO * 0.7;
        double etiqueta_x = CENTRO_X + etiqueta_distancia * cos(angulo_medio_rad);
        double etiqueta_y = CENTRO_Y + etiqueta_distancia * sin(angulo_medio_rad);
        
        archivo << "  <text x=\"" << etiqueta_x << "\" y=\"" << etiqueta_y 
                << "\" text-anchor=\"middle\" font-family=\"Arial\" font-size=\"12\" fill=\"white\" font-weight=\"bold\">\n";
        archivo << "    " << fixed << setprecision(1) << (porcentaje * 100) << "%\n";
        archivo << "  </text>\n";
        
        angulo_actual = angulo_fin;
    }
    
    // Leyenda
    archivo << "  <rect x=\"550\" y=\"100\" width=\"230\" height=\"" 
            << (categorias_ordenadas.size() * 30 + 40) 
            << "\" rx=\"10\" ry=\"10\" fill=\"white\" stroke=\"#ddd\" stroke-width=\"1\"/>\n";
    
    archivo << "  <text x=\"665\" y=\"130\" text-anchor=\"middle\" font-family=\"Arial\" font-size=\"16\" font-weight=\"bold\" fill=\"#333\">\n";
    archivo << "    Leyenda\n";
    archivo << "  </text>\n";
    
    for (size_t i = 0; i < categorias_ordenadas.size(); i++) {
        double y_pos = 160 + i * 30;
        double porcentaje = (categorias_ordenadas[i].second / total_general) * 100.0;
        
        // Cuadro de color
        archivo << "  <rect x=\"570\" y=\"" << (y_pos - 10) 
                << "\" width=\"15\" height=\"15\" fill=\"" 
                << colores[i % colores.size()] << "\" stroke=\"#ddd\" stroke-width=\"1\"/>\n";
        
        // Nombre de categoría
        archivo << "  <text x=\"595\" y=\"" << y_pos 
                << "\" font-family=\"Arial\" font-size=\"12\" fill=\"#333\">\n";
        string nombre = categorias_ordenadas[i].first;
        if (nombre.length() > 15) nombre = nombre.substr(0, 12) + "...";
        archivo << "    " << nombre << "\n";
        archivo << "  </text>\n";
        
        // Porcentaje y monto
        archivo << "  <text x=\"750\" y=\"" << y_pos 
                << "\" text-anchor=\"end\" font-family=\"Arial\" font-size=\"11\" fill=\"#666\">\n";
        archivo << "    " << fixed << setprecision(1) << porcentaje << "% ($" 
                << fixed << setprecision(2) << categorias_ordenadas[i].second << ")\n";
        archivo << "  </text>\n";
    }
    
    // Pie de página
    archivo << "  <text x=\"" << ANCHO/2 << "\" y=\"" << (ALTO - 20) 
            << "\" text-anchor=\"middle\" font-family=\"Arial\" font-size=\"10\" fill=\"#999\">\n";
    archivo << "    Generado el " << __DATE__ << " - Budget Buddy v1.0\n";
    archivo << "  </text>\n";
    
    archivo << "</svg>\n";
    archivo.close();
    
    cout << "\n✅ Archivo SVG creado exitosamente!\n";
    cout << "📂 Archivo: " << nombre_archivo << "\n\n";
    
    // Mostrar resumen en consola
    cout << "📊 RESUMEN DE GASTOS:\n";
    cout << string(50, '=') << "\n";
    for (size_t i = 0; i < categorias_ordenadas.size(); i++) {
        double porcentaje = (categorias_ordenadas[i].second / total_general) * 100.0;
        cout << "  " << left << setw(20) << categorias_ordenadas[i].first
             << ": " << right << setw(6) << fixed << setprecision(1) << porcentaje << "%"
             << "  $" << setw(10) << fixed << setprecision(2) 
             << categorias_ordenadas[i].second << "\n";
    }
    cout << "\n💰 TOTAL: $" << fixed << setprecision(2) << total_general << "\n\n";
    
    // Preguntar si abrir
    cout << "¿Abrir gráfico SVG? (s/n): ";
    char respuesta;
    cin >> respuesta;
    limpiar_buffer();
    
    if (respuesta == 's' || respuesta == 'S') {
        #ifdef _WIN32
            system(("start \"\" \"" + nombre_archivo + "\"").c_str());
        #elif __APPLE__
            system(("open " + nombre_archivo).c_str());
        #elif __linux__
            system(("xdg-open " + nombre_archivo).c_str());
        #else
            cout << "Por favor, abra manualmente: " << nombre_archivo << "\n";
        #endif
    }
    
    pausa();
}

// ----------------------- MENU DE OPCIONES ---------------
int main() {
    vector<Gasto> gastos;
    cargar_csv(gastos);
    vector<PresupuestoMes> presupuesto;
    cargar_presupuestos(presupuesto);
    // ---------------------------------
    cout << "\n--- CONFIGURAR MES ACTIVO ---\n";
    seleccionar_mes_activo(presupuesto);
    // ---------------------------------
    while (true) {
    borrar_pantalla();
    cout << "Mes activo: " << MES_ACTIVO 
         << "   |   Presupuesto: ";

    if (presupuesto_mes > 0)
        cout << fixed << setprecision(2) << presupuesto_mes;
    else
        cout << "(no definido)";
    cout << "\n\n";

    cout << "\n\nMENU PRINCIPAL\n";
    cout << " 1) Registrar gasto\n";
    cout << " 2) Mostrar gastos del mes activo\n";
    cout << " 3) Eliminar gasto\n";
    cout << " 4) Modulo de analisis (mes activo)\n";
    cout << " 5) Configurar presupuesto del mes activo\n";
    cout << " 6) Cambiar mes activo\n";
    cout << " 7) Reportes globales\n";
    cout << " 8) Gráficos estadísticos \n";
    cout << " 9) Salir\n\n";
    cout << "Seleccione una opcion: ";

    int opcion;
    if (!(cin >> opcion)) {
        cin.clear();
        limpiar_buffer();
        cout << "Opcion invalida.\n";
        pausa();
        continue;
    }
    limpiar_buffer();
    switch (opcion) {
            case 1: registrar_gasto(gastos); pausa(); break;
            case 2: mostrar_gastos_mes(gastos); pausa(); break;
            case 3: eliminar_gasto(gastos); pausa(); break;
            case 4: submenu_analisis(gastos); break;
            case 5: configurar_presupuesto_mes(presupuesto); pausa(); break;
            case 6: seleccionar_mes_activo(presupuesto); pausa(); break;
            case 7: submenu_global(gastos); break;
            case 8: crear_grafico_off (gastos); break;
            case 9:
                cout << "Saliendo del programa... ¡Hasta pronto!\n";
                guardar_csv(gastos);
                pausa();
                return 0;
            default:
                cout << "Opcion no valida.\n";
                pausa();
        }
    }
}
