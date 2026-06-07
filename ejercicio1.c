/*
* Universidad de Costa Rica
 * IE-0117 Programación Bajo Plataformas Abiertas
 * Ejercicio 1 - Laboratorio #4
 * Jean Carlo Chinchilla Mora, C02276
 *
 * Este programa trabaja con una matriz cuadrada dinámica de valores
 * binarios, es decir, ceros y unos. El usuario ingresa el tamaño de
 * la matriz, el programa reserva memoria, llena la matriz de forma
 * aleatoria y luego busca la secuencia más larga de unos consecutivos.
 *
 * La matriz se trata como un arreglo lineal para que los unos que
 * terminan en una fila y continúan en la siguiente también cuenten
 * como consecutivos. Además, el acceso a los elementos se realiza
 * usando aritmética de punteros, sin usar corchetes.
 */

#include <stdio.h>
#include <stdlib.h>
/*
Esta funcion busca la mayor cantidad de 1s seguidos en la matriz.

La matriz se recorre como si fuera una sola fila larga.
Por ejemplo, si una fila termina en 1 y la siguiente empieza en 1,
se siguen contando como consecutivos.
*/
void findLargestLine(int **matrix, int size, int *result) {
    int i;
    int total = size * size;
    int contador = 0;
    int mayor = 0;

    /*
    Se toma la direccion del primer elemento de la matriz.
    Como la matriz fue reservada en un solo bloque de memoria,
    desde aqui se puede recorrer todo seguido.
    */
    int *actual = *(matrix + 0);

    /*
    Se recorren todos los elementos de la matriz como si fueran
    un arreglo lineal de size * size posiciones.
    */
    for (i = 0; i < total; i++) {

        /*
        Si el valor actual es 1, se aumenta el contador de 1s seguidos.
        */
        if (*(actual + i) == 1) {
            contador++;

            /*
            Si la secuencia actual es mayor que la mayor encontrada,
            se actualiza el valor de mayor.
            */
            if (contador > mayor) {
                mayor = contador;
            }
        } else {
            /*
            Si aparece un 0, la secuencia de 1s se corta
            y el contador vuelve a empezar desde 0.
            */
            contador = 0;
        }
    }

    /*
    Se guarda el resultado final en la variable que viene desde main.
    */
    *result = mayor;
}

/*
Esta funcion reserva memoria para una matriz cuadrada.

Se usa un triple puntero porque necesitamos modificar el puntero
matrix que esta declarado en main.
*/
void allocateMatrix(int ***matrix, int size) {
    int i;
    int *data;

    /*
    Primero se reserva espacio para los punteros de las filas.
    Cada fila de la matriz va a tener su propio puntero.
    */
    *matrix = malloc(size * sizeof(int *));

    /*
    Se revisa si la memoria se pudo reservar correctamente.
    */
    if (*matrix == NULL) {
        printf("Error al reservar memoria para las filas.\n");
        exit(1);
    }

    /*
    Ahora se reserva un solo bloque de memoria para todos los datos.
    Esto ayuda a que la matriz pueda recorrerse como un arreglo lineal.
    */
    data = malloc(size * size * sizeof(int));

    /*
    Se revisa si el bloque de datos se pudo reservar correctamente.
    */
    if (data == NULL) {
        printf("Error al reservar memoria para los datos.\n");
        free(*matrix);
        exit(1);
    }

    /*
    Aqui se hace que cada fila apunte a la parte que le corresponde
    dentro del bloque de memoria reservado.
    */
    for (i = 0; i < size; i++) {
        *(*matrix + i) = data + i * size;
    }
}

/*
Esta funcion llena la matriz con valores aleatorios de 0 o 1.
*/
void fillMatrix(int **matrix, int size) {
    int i, j;

    /*
    Se recorren las filas y columnas de la matriz.
    No se usan corchetes, solo aritmetica de punteros.
    */
    for (i = 0; i < size; i++) {
        for (j = 0; j < size; j++) {

            /*
            rand() % 2 solo puede dar 0 o 1.
            Ese valor se guarda en la posicion actual de la matriz.
            */
            *(*(matrix + i) + j) = rand() % 2;
        }
    }
}

/*
Esta funcion imprime la matriz en la terminal.
*/
void printMatrix(int **matrix, int size) {
    int i, j;

    printf("Matriz (%dx%d):\n", size, size);

    /*
    Se recorre la matriz completa para imprimir cada valor.
    */
    for (i = 0; i < size; i++) {
        for (j = 0; j < size; j++) {

            /*
            Se imprime el valor que esta en la fila i y columna j.
            */
            printf("%d ", *(*(matrix + i) + j));
        }

        /*
        Al terminar una fila, se baja a la siguiente linea.
        */
        printf("\n");
    }
}

/*
Esta funcion libera la memoria que se habia reservado para la matriz.
*/
void freeMatrix(int **matrix, int size) {
    /*
    Esta linea evita una advertencia del compilador, porque en esta
    version no necesitamos usar size dentro de la funcion.
    */
    (void) size;

    /*
    Primero se libera el bloque donde estan guardados todos los datos.
    */
    free(*(matrix + 0));

    /*
    Luego se libera el arreglo de punteros de las filas.
    */
    free(matrix);
}

int main(void) {
    int size, largestLine;
    int **matrix = NULL;

    /*
    Se pide al usuario el tamano de la matriz cuadrada.
    */
    printf("Ingrese el tamano de la matriz: ");

    /*
    Se revisa que el usuario haya ingresado un numero valido
    y que el tamano sea mayor que cero.
    */
    if (scanf("%d", &size) != 1 || size <= 0) {
        printf("Tamano invalido.\n");
        return 1;
    }

    /*
    Se coloca una semilla fija para que las pruebas sean repetibles.
    Asi, si se ingresa el mismo tamano, se obtiene la misma matriz.
    */
    srand(1);

    /*
    Se reserva la memoria para la matriz.
    */
    allocateMatrix(&matrix, size);

    /*
    Se llena la matriz con ceros y unos aleatorios.
    */
    fillMatrix(matrix, size);

    /*
    Se muestra la matriz generada.
    */
    printMatrix(matrix, size);

    /*
    Se busca la secuencia mas larga de unos consecutivos.
    */
    findLargestLine(matrix, size, &largestLine);

    /*
    Se imprime el resultado encontrado.
    */
    printf("El tamano de la secuencia de 1s mas grande es: %d\n",
           largestLine);

    /*
    Se libera la memoria dinamica usada por la matriz.
    */
    freeMatrix(matrix, size);

    return 0;
}
