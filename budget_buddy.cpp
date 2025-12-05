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


// ----------------------- ESTRUCTURAS --------------------
struct Gasto {
    double monto;
    char categoria[20];
    char fecha[12];
    char descripcion[50];
};


// ----------------------- FUNCIONES BASICAS --------------------
void pausa() {
    system("pause");
}

void borrar_pantalla() {
    system("cls");
}

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

void guardarCSV(const vector<Gasto>& gastos) {
    ofstream archivo(nombre_archivo.c_str());
    if (!archivo.is_open()) {
        cout<<"\nError escribiendo CSV.\n";
        return;
    }
    // ---------------------------------
    archivo <<"\nEJEMPLO: monto,categoria,fecha,descripcion\n";
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

// ---------------- FUNCION VALIDAR FECHA -----------------

bool validar_fecha(const char fecha[]) {
    if (strlen(fecha)!=10)
        return false;
    if (fecha[2]!='/' || fecha[5]!='/')
        return false;
    for (int i=0 ; i<10 ; ++i) {
        if (i==2 || i==5)
            continue;
        if (!isdigit(static_cast<unsigned char>(fecha[i])))
            return false;
    }
    int dia=(fecha[0] - '0') * 10 + (fecha[1] - '0');
    int mes=(fecha[3] - '0') * 10 + (fecha[4] - '0');
    if (mes < 1 || mes > 12)
        return false;
    if (dia < 1 || dia > 31)
        return false;
    return true;
}

// ----------------------- FUNCIONES ----------------------


void registrar_gasto(vector<Gasto>& gastos) {
    limpiar_buffer();
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
    guardarCSV(gastos);
    cout<<"\nGasto registrado exitosamente.\n";
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
    borrar_pantalla();
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
    guardarCSV(gastos);
    // ---------------------------------
    cout<<"\nGasto eliminado.\n";
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
    cout << "\nREPORTE POR FECHA\n";
    cout << "======================================================\n";
    cout << left << setw(15) << "Categoria"
         << setw(12) << "Total"
         << "Porcentaje\n";
    cout << "------------------------------------------------------\n";
    for (int i = 0; i < num_fechas; ++i) {
        double porcentaje = (totales[i] / total_general) * 100.0;
        cout << left << setw(15) << fechas[i]<< setw(12) << fixed << setprecision(2) << totales[i]<< fixed << setprecision(2) << porcentaje << " %\n";
    }
    cout << "\nTotal general: " << fixed << setprecision(2) << total_general << "\n";
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
    cout << "\nREPORTE POR MES\n";
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
    if (gastos.empty()) {
        cout << "\nNo hay datos para generar el reporte por día.\n";
        pausa();
        return;
    }
    
    const int MAX_DIAS = 365; 
    char dias[MAX_DIAS][12]; 
    double totales[MAX_DIAS] = {0}; 
    int num_dias = 0; 
    for (size_t i = 0; i < gastos.size(); ++i) {
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
                cout << "\nSe alcanzó el máximo de días en el reporte.\n";
                break;
            }
        }
    }
    double total_general = 0.0;
    for (size_t i = 0; i < gastos.size(); ++i) {
        total_general += gastos[i].monto;
    }
    
    cout << "\nREPORTE POR DÍA\n";
    cout << "======================================================\n";
    cout << left << setw(15) << "Fecha"
         << setw(12) << "Total"
         << "Porcentaje\n";
    cout << "------------------------------------------------------\n";
    
    for (int i = 0; i < num_dias; ++i) {
        double porcentaje = (totales[i] / total_general) * 100.0;
        cout << left << setw(15) << dias[i]
             << setw(12) << fixed << setprecision(2) << totales[i]
             << fixed << setprecision(2) << porcentaje << " %\n";
    }
    
    cout << "\nTotal general: " << fixed << setprecision(2) << total_general << "\n";
    cout << "Días con gastos registrados: " << num_dias << "\n";
}

void reporte_por_semana(const vector<Gasto>& gastos) {
    borrar_pantalla();
    if (gastos.empty()) {
        cout << "\nNo hay datos para generar el reporte por semana.\n";
        pausa();
        return;
    }
    
    auto extraer_fecha = [](const char fecha[], int& dia, int& mes, int& anio) {

        dia = (fecha[0] - '0') * 10 + (fecha[1] - '0');
        mes = (fecha[3] - '0') * 10 + (fecha[4] - '0');
        anio = (fecha[6] - '0') * 1000 + 
               (fecha[7] - '0') * 100 + 
               (fecha[8] - '0') * 10 + 
               (fecha[9] - '0');
    };
    
    auto dia_juliano = [](int dia, int mes, int anio) {
        int dias_mes[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
        
        if ((anio % 4 == 0 && anio % 100 != 0) || (anio % 400 == 0)) {
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
        int anio;
        int numero_semana;
        double total;
    };
    
    SemanaInfo semanas[MAX_SEMANAS];
    int num_semanas = 0;
    
    for (size_t i = 0; i < gastos.size(); ++i) {
        int dia, mes, anio;
        extraer_fecha(gastos[i].fecha, dia, mes, anio);
        
        int dia_del_anio = dia_juliano(dia, mes, anio);
        int numero_semana = (dia_del_anio - 1) / 7 + 1;

        bool encontrada = false;
        int pos = -1;
        for (int j = 0; j < num_semanas; ++j) {
            if (semanas[j].anio == anio && semanas[j].numero_semana == numero_semana) {
                encontrada = true;
                pos = j;
                break;
            }
        }
        
        if (encontrada) {
            semanas[pos].total += gastos[i].monto;
        } else {
            if (num_semanas < MAX_SEMANAS) {
                semanas[num_semanas].anio = anio;
                semanas[num_semanas].numero_semana = numero_semana;
                semanas[num_semanas].total = gastos[i].monto;
                num_semanas++;
            } else {
                cout << "\nSe alcanzó el máximo de semanas en el reporte.\n";
                break;
            }
        }
    }
    
    double total_general = 0.0;
    for (size_t i = 0; i < gastos.size(); ++i) {
        total_general += gastos[i].monto;
    }

    cout << "\nREPORTE POR SEMANA\n";
    cout << "======================================================\n";
    cout << left << setw(10) << "Año"
         << setw(10) << "Semana"
         << setw(12) << "Total"
         << "Porcentaje\n";
    cout << "------------------------------------------------------\n";
    
    for (int i = 0; i < num_semanas; ++i) {
        double porcentaje = (semanas[i].total / total_general) * 100.0;
        cout << left << setw(10) << semanas[i].anio
             << setw(10) << semanas[i].numero_semana
             << setw(12) << fixed << setprecision(2) << semanas[i].total
             << fixed << setprecision(2) << porcentaje << " %\n";
    }
    
    cout << "\nTotal general: " << fixed << setprecision(2) << total_general << "\n";
    cout << "Semanas con gastos registrados: " << num_semanas << "\n";
    cout << "\nNota: La semana 1 comienza el 1 de enero de cada año.\n";
}

// ----------------------- MENU DE OPCIONES ---------------
int main() {
    vector<Gasto> gastos;
    cargar_csv(gastos);
    int opcion;
    // ---------------------------------
   
    while (true) {
        cout<< "\n Bienvenido a Budget Buddy, que desea hacer?\n";
        cout<< "1. Registrar gasto\n";
        cout<< "2. Mostrar gastos\n";
        cout<< "3. Eliminar gastos\n";
        cout<< "4. Reporte por categoria\n";
        cout<< "5. Reporte por fecha\n";
        cout<< "6. Reporte por mes\n";
        cout<< "7. Reporte por dia\n";
        cout<< "8. Reporte por semana\n";
        cout<< "9. Salir\n";
        cout<< "Seleccione una opcion: ";
        cin>> opcion;
        
        // ---------------------------------
        if (!cin) {
            cin.clear();
            limpiar_buffer();
            cout << "\nOpcion no valida.\n";
            continue;
        }
        // ---------------------------------
        switch (opcion) {
            case 1: registrar_gasto(gastos); pausa(); break;
            case 2: mostrar_gastos(gastos); pausa(); break;
            case 3: eliminar_gasto(gastos); pausa(); break;
            case 4: reporte_por_categoria(gastos); pausa(); break;
            case 5: reporte_por_fecha(gastos); pausa(); break;
            case 6: reporte_por_mes(gastos); pausa(); break;
            case 7: reporte_por_dia(gastos); pausa(); break;
            case 8: reporte_por_semana(gastos); pausa(); break;
            case 9:
                cout<<"\nSaliendo del programa... Hasta pronto!\n";
                pausa();
                return 0;
            default:
                cout<<"\nOpcion no valida\n";
        }
        // ---------------------------------
    }
}
