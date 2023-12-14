/** CONCEPTOS CLAVE
 * -> REGLA DEL PREFIJO MAS LARGO
 *      -> Prefijo IP: en una ip, el prefijo se refiere a la parte inicial de la 
 *          direccion que identifica la red a la que pertenece el host. Por ejemplo, en 
 *          192.168.1.0/24, el prefijo seria 192.168.1 y /24 inidica que de los 24 bits 
 *          que tiene la direccion, los 24 primeros son el prefijo.
 *      -> Mas Largo: significa que, de todas las posibles coincidencias, se elige la
 *         que tenga el prefijo mas largo.
 * -> INTERFAZ DE SALIDA
 *     -> En redes una interfaz es un punto de conexion entre dos sistemas, que puede ser 
 *      fisico (por ethernet, por ejemplo) o virtual. Es por donde los datos deben ser enviados
 *      para que lleguen a su destino. 
 * -> LEER TABLA DE REENVIO
 *    -> Una tabla de reenvio, tambien llamada tabla de enrutamiento, es una lista que utiliza el
 *        router o un software para determinar a donde deben enviarse los datos. 
 *    -> Contiene info sobre las redes y que interfaz de salida corresponde cada una. Por ejemplo,
 *        193.144.0.0/16,3 significa que para la red 193.144.0.0 con un prefijo de 16 bits, la interfaz
 *        de salida es la 3. 
 * -> MASCARA DE RED
 *   -> Una mascara de red es una combinacion de bits que sirve para delimitar el prefijo de una direccion
 * -> inet_net_pton convierte una dirección IP de texto con una notación de prefijo de red (como "192.168.1.0/24") a una forma binaria. 
 * -> inet_net_ntop realiza la operación inversa, convirtiendo la forma binaria a texto.
*/

// Incluir bibliotecas
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <limits.h>

// Estructura para almacenar una entrada de la tabla de reenvío
typedef struct {
    int ip; // IP convertida a un número entero
    int prefijo;       // Longitud del prefijo
    int interfaz;    // Interfaz
} entradaReenvio;

/* DECLARACION DE FUNCIONES */
entradaReenvio *leerTabla(char *fileName, int *nEntradas);
int convertirIP(char *ipString);
void encontrarInterfazSalida(char *inputIP, entradaReenvio *tabla, int nEntradas);

int main(int argc, char *argv[]) {

    /* DECLARACION DE VARIABLES */

    char *fileName, *inputIP;
    entradaReenvio *tablaReenvio;
    int nEntradas = 0;

    /* CUERPO DE LA FUNCION */

    // Procesar argumentos de línea de comandos
    if (argc != 3) {
        printf("Uso: %s <archivo> <IP>\n", argv[0]);
        return 1;
    }

    fileName = argv[1]; // Nombre del archivo
    inputIP = argv[2]; // IP de entrada

    // Leer y analizar la tabla de reenvío
    tablaReenvio = leerTabla(fileName, &nEntradas);

    // Determinar y mostrar la interfaz de salida
    encontrarInterfazSalida(inputIP, tablaReenvio, nEntradas);

    free(tablaReenvio); // Liberar la memoria reservada para la tabla de reenvío

    return 0;
}

/**
 * @brief Convierte una dirección IP en una cadena de caracteres a un número entero sin signo.
 * @param ipString Dirección IP en formato de cadena de caracteres.
 * @return Dirección IP convertida a un número entero sin signo.
*/
int convertirIP(char *ipString){
    struct in_addr ipAddr; // Estructura para almacenar la dirección IP

    // inet_net_pton convierte una dirección IP en formato de cadena a binario
    // AF_INET es para IPv4, la función también llena la cantidad de bits (prefijo)
    if (inet_net_pton(AF_INET, ipString, &ipAddr, sizeof(ipAddr)) == -1) {
        fprintf(stderr, "Error al convertir la dirección %s\n", ipString);
        exit(EXIT_FAILURE);
    }

    // Convertir la IP a número de bytes del host (little endian o big endian)
    return ntohl(ipAddr.s_addr);
}


/**
 * @brief Lee la tabla de reenvío desde un archivo de texto y la almacena en memoria.
 * @param fileName Nombre del archivo de texto.
 * @param nEntradas Puntero a una variable donde se almacenará el número de entradas leídas.
 * @return Puntero a la tabla de reenvío.
*/
entradaReenvio *leerTabla(char *fileName, int *nEntradas){

    /* DECLARACION DE VARIABLES */

    FILE *fp; // Puntero al archivo
    entradaReenvio *tabla; // Tabla de reenvio
    char ip[16]; // IP
    int prefijo = 0, interfaz = 0; // Prefijo e interfaz
    char extraChar; // Para verificar si hay caracteres adicionales después de la entrada
    
    /* CUERPO DE LA FUNCION */

    // Abrir el archivo y comprobamos que se ha abierto correctamente
    if((fp = fopen(fileName, "r")) == NULL){
        printf("Error al abrir el archivo\n");
        exit(1);
    }

    *nEntradas = 0; // Inicializar nEntradas

    // Contar el número de entradas en el archivo
    while (fscanf(fp, "%[^/]/%d,%d%c", ip, &prefijo, &interfaz, &extraChar) != EOF) {
        // Incrementar el número de entradas válidas
        (*nEntradas)++;
    }

    // Volver al inicio del archivo
    rewind(fp);

    // Reservar memoria para la tabla de reenvio
    tabla = (entradaReenvio *)malloc(sizeof(entradaReenvio) * (*nEntradas));

    if(tabla == NULL){
        printf("Error al reservar memoria para la tabla de reenvio\n");
        exit(1);
    }

    int i = 0; // Indice de la tabla de reenvio

    int numLeidos;
    while((numLeidos = fscanf(fp, "%[^/]/%d,%d\n", ip, &prefijo, &interfaz)) != EOF){
        if(numLeidos != 3 || prefijo < 0 || prefijo > 32 || interfaz < 0 || interfaz > INT_MAX) {
            // Manejo del error: el formato no es correcto
            fprintf(stderr, "Error: formato de entrada incorrecto en la linea %d.\n", i + 1);
            break;
        }

        tabla[i].ip = convertirIP(ip); // Llamo a la funcion convertir 
        tabla[i].prefijo = prefijo; // Almacenar el prefijo
        tabla[i].interfaz = interfaz; // Almacenar la interfaz
        i++; // Incrementar el indice
    }

    fclose(fp);
    return tabla;
}


/**
 * @brief Determina la interfaz de salida para una dirección IP dada.
 * @param inputIP Dirección IP de entrada.
 * @param tabla Tabla de reenvío.
 * @param nEntradas Número de entradas en la tabla de reenvío.
*/
void encontrarInterfazSalida(char *inputIP, entradaReenvio *tabla, int nEntradas){

    /* DECLARACION DE VARIABLES */

    int ipEntrada = convertirIP(inputIP); // Convertimos la ip de entrada a un entero para poder compararla
    int interfaz = -1, prefijoMax = -1; // Interfaz de salida

    /* CUERPO DE LA FUNCION */

    for(int i = 0; i < nEntradas; i++) {
        //Generamos la mascara de red 
        /**
         * UINT32_MAX - es una constante que representa el valor máximo de un entero sin signo de 32 bits
         * >> - es el operador de desplazamiento a la derecha, desplaza los bits a la derecha de tal forma 
         *      que se pierden los bits desplazados para conseguir la máscara de red
         *      Si por ejemplo el prefijo es 24, se desplazan 8 bits a la derecha, por lo que queda 00000000 11111111 11111111 11111111
         * ~ - es el operador de negación, invierte todos los bits de la máscara de red. Siguiendo el ejemplo que acabo
         *      de poner quedaría 11111111 00000000 00000000 00000000, que es el formato clasico de una máscara de red
        */
        uint32_t mascara = ~(UINT32_MAX >> tabla[i].prefijo); // Crear máscara de red
        uint32_t ipTabla = tabla[i].ip & mascara; // Aplicar máscara a la IP de la tabla
        uint32_t ipComparar = ipEntrada & mascara; // Aplicar máscara a la IP de entrada

        // Comparar IPs y verificar si el prefijo es más largo que el anterior encontrado
        if(ipComparar == ipTabla && tabla[i].prefijo > prefijoMax) {
            prefijoMax = tabla[i].prefijo;
            interfaz = tabla[i].interfaz;
        }
    }

    // Si no se encuentra ninguna coincidencia, usar la interfaz por defecto (0)
    if(prefijoMax == -1) {
        interfaz = 0;
    }

    // Si no se ha encontrado ninguna coincidencia utilizar la interfaz 0 si se ha encontrado alguna muestra la interfaz de salida
    printf("Salida:"
        "\n\tInterfaz de salida -> %d"
        "\n\tBits del prefijo -> %d\n", interfaz, prefijoMax); // Mostrar la interfaz de salida
}