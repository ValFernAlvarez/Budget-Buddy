# Budget-Buddy
Una aplicación de consola en C++ que facilite el registro, análisis y control del presupuesto personal de estudiantes universitarios, mediante una interfaz sencilla y almacenamiento local en archivos CSV, permitiendo visualizar reportes, detectar excesos de gasto y fomentar hábitos financieros responsables.

## Funcionalidades principales
1. **Registrar gasto:** Permite ingresar un nuevo gasto con monto, categoría, fecha y descripción.
2. **Mostrar gastos:** Visualiza todos los gastos registrados en una tabla organizada.
3. **Eliminar gasto:** Elimina un gasto específico utilizando su índice.
4. **Reporte por categoría:** Genera un resumen de gastos por categoría, indicando total y porcentaje del total.
5. **Reporte por fecha:** Genera un resumen de gastos por fecha específica.
6. **Reporte por mes:** Genera un resumen mensual de los gastos registrados.
7. **Guardar y cargar CSV:** Todos los gastos se guardan en un archivo `gastos.csv` y se pueden cargar al iniciar la aplicación.

## Librerías utilizadas
#include <iostream>    // Entrada y salida estándar
#include <vector>      // Contenedor vector
#include <cstring>     // Manejo de cadenas
#include <fstream>     // Archivos
#include <iomanip>     // Formato de salida
#include <limits>      // Límites de tipos
#include <algorithm>   // Algoritmos como sort, max, etc.
#include <cstdlib>     // system(), atof()
#include <cctype>      // isdigit()

## Estructuras
struct Gasto {
    double monto;          // Monto del gasto
    char categoria[20];    // Categoría del gasto
    char fecha[12];        // Fecha en formato dd/mm/yyyy
    char descripcion[50];  // Descripción del gasto
};

## Diccionario de variables principales
| Variable                                                      | Tipo                   | Descripción                                             |
| ------------------------------------------------------------- | ---------------------- | ------------------------------------------------------- |
| `gastos`                                                      | `vector<Gasto>`        | Almacena todos los gastos registrados                   |
| `nombre_archivo`                                              | `const string`         | Nombre del archivo CSV (`gastos.csv`)                   |
| `opcion`                                                      | `int`                  | Opción seleccionada por el usuario en el menú           |
| `MAX_CAT`                                                     | `int`                  | Máximo número de categorías en el reporte por categoría |
| `MAX_FECHAS`                                                  | `int`                  | Máximo número de fechas en el reporte por fecha         |
| `MAX_MESES`                                                   | `int`                  | Máximo número de meses en el reporte por mes            |
| `categorias`                                                  | `char[MAX_CAT][20]`    | Arreglo que almacena las categorías únicas              |
| `totales`                                                     | `double[MAX_CAT]`      | Totales acumulados por categoría                        |
| `fechas`                                                      | `char[MAX_FECHAS][12]` | Arreglo que almacena las fechas únicas                  |
| `totales_mes`                                                 | `double[MAX_MESES]`    | Totales acumulados por mes                              |
| `g`                                                           | `Gasto`                | Variable temporal para registrar un gasto               |
| `indice`                                                      | `size_t`               | Índice de gasto para eliminar o modificar               |
| `campo_monto`, `campo_categoria`, `campo_fecha`, `campo_desc` | `char[]`               | Campos temporales para leer CSV                         |

## Funciones principales
| Función                                            | Descripción                                                |
| -------------------------------------------------- | ---------------------------------------------------------- |
| `void pausa()`                                     | Pausa la ejecución hasta que el usuario presione una tecla |
| `void borrar_pantalla()`                           | Limpia la consola                                          |
| `void limpiar_buffer()`                            | Limpia el buffer de entrada `cin` para evitar errores      |
| `void cargar_csv(vector<Gasto>&)`                  | Carga los gastos desde el archivo CSV                      |
| `void guardarCSV(const vector<Gasto>&)`            | Guarda los gastos actuales en el archivo CSV               |
| `bool validar_fecha(const char[])`                 | Valida que la fecha ingresada tenga formato `dd/mm/yyyy`   |
| `void registrar_gasto(vector<Gasto>&)`             | Permite al usuario registrar un nuevo gasto                |
| `void mostrar_gastos(const vector<Gasto>&)`        | Muestra todos los gastos registrados en la consola         |
| `void eliminar_gasto(vector<Gasto>&)`              | Elimina un gasto seleccionando su índice                   |
| `void reporte_por_categoria(const vector<Gasto>&)` | Muestra un resumen de gastos por categoría                 |
| `void reporte_por_fecha(const vector<Gasto>&)`     | Muestra un resumen de gastos por fecha                     |
| `void reporte_por_mes(const vector<Gasto>&)`       | Muestra un resumen de gastos por mes                       |

## Formato CSV
El archivo gastos.csv sigue el siguiente formato:
monto,categoria,fecha,descripcion
50.00,Alimentos,03/12/2025,Compra supermercado
100.50,Transporte,02/12/2025,Pasaje bus

## Uso
1. Ejecutar el programa.
2. Inicializar mes.
3. Seleccionar una opción del menú.
4. Registrar, mostrar, eliminar gastos o generar reportes según se necesite.
5. Todos los cambios se guardan automáticamente en gastos.csv.
