/*
 * Universidad de Costa Rica
 * IE-0117 Programación Bajo Plataformas Abiertas
 * Ejercicio 2 - Laboratorio #4
 * Jean Carlo Chinchilla Mora, C02276
 *
 * Este programa lee una imagen en formato PGM P2 desde el archivo
 * input.pgm. La imagen se guarda en memoria dinámica usando un arreglo
 * de unsigned char.
 *
 * Luego el usuario ingresa un valor de umbral. Con ese valor se genera
 * una imagen en blanco y negro, donde los píxeles mayores o iguales al
 * umbral pasan a 255 y los menores pasan a 0. Después se genera también
 * el negativo de esa imagen umbralizada.
 *
 * Finalmente, el programa escribe dos archivos de salida:
 * output_threshold.pgm y output_negative.pgm. También imprime algunas
 * estadísticas, como la cantidad de píxeles blancos, negros y el
 * promedio de los valores originales.
 */

#include <stdio.h>
#include <stdlib.h>

/*
Esta funcion lee una imagen PGM en formato P2.

El archivo debe tener esta estructura:
P2
ancho alto
valor_maximo
pixeles...

La funcion devuelve un puntero al arreglo de pixeles.
Tambien guarda el ancho, alto y valor maximo usando punteros.
*/
unsigned char *read_pgm(const char *filename,
                        int *width,
                        int *height,
                        int *max_val) {
    FILE *file;
    char p, two;
    int total;
    int i;
    int pixel;
    unsigned char *pixels;

    /*
    Se abre el archivo en modo lectura.
    */
    file = fopen(filename, "r");

    if (file == NULL) {
        printf("No se pudo abrir el archivo %s.\n", filename);
        return NULL;
    }

    /*
    Se leen los dos caracteres del formato.
    Para este laboratorio se espera que sean P y 2.
    */
    if (fscanf(file, " %c%c", &p, &two) != 2) {
        printf("No se pudo leer el formato del archivo.\n");
        fclose(file);
        return NULL;
    }

    /*
    Se revisa que el formato sea P2.
    */
    if (p != 'P' || two != '2') {
        printf("El archivo no esta en formato P2.\n");
        fclose(file);
        return NULL;
    }

    /*
    Se leen el ancho y el alto de la imagen.
    */
    if (fscanf(file, "%d %d", width, height) != 2) {
        printf("No se pudo leer el ancho y el alto.\n");
        fclose(file);
        return NULL;
    }

    /*
    Se lee el valor maximo de intensidad.
    Normalmente en este tipo de imagen es 255.
    */
    if (fscanf(file, "%d", max_val) != 1) {
        printf("No se pudo leer el valor maximo.\n");
        fclose(file);
        return NULL;
    }

    /*
    Se revisa que las dimensiones y el valor maximo tengan sentido.
    */
    if (*width <= 0 || *height <= 0 || *max_val <= 0 || *max_val > 255) {
        printf("Datos invalidos en el encabezado del archivo.\n");
        fclose(file);
        return NULL;
    }

    /*
    La cantidad total de pixeles es ancho por alto.
    */
    total = (*width) * (*height);

    /*
    Se reserva memoria dinamica para guardar todos los pixeles.
    */
    pixels = malloc(total * sizeof(unsigned char));

    if (pixels == NULL) {
        printf("No se pudo reservar memoria para los pixeles.\n");
        fclose(file);
        return NULL;
    }

    /*
    Se leen los pixeles uno por uno.
    Aunque se leen como enteros, se guardan como unsigned char.
    */
    for (i = 0; i < total; i++) {
        if (fscanf(file, "%d", &pixel) != 1) {
            printf("Error al leer los pixeles de la imagen.\n");
            free(pixels);
            fclose(file);
            return NULL;
        }

        /*
        Se revisa que el pixel este dentro del rango permitido.
        */
        if (pixel < 0 || pixel > *max_val) {
            printf("Se encontro un pixel fuera de rango.\n");
            free(pixels);
            fclose(file);
            return NULL;
        }

        /*
        Se guarda el pixel usando aritmetica de punteros.
        */
        *(pixels + i) = (unsigned char) pixel;
    }

    fclose(file);

    return pixels;
}

/*
Esta funcion hace una copia de los pixeles originales.

Se ocupa porque apply_threshold cambia el arreglo original.
Entonces guardamos una copia para poder calcular el promedio original.
*/
unsigned char *copy_pixels(unsigned char *pixels, int total) {
    int i;
    unsigned char *copy;

    /*
    Se reserva memoria para la copia de la imagen.
    */
    copy = malloc(total * sizeof(unsigned char));

    if (copy == NULL) {
        printf("No se pudo reservar memoria para la copia.\n");
        return NULL;
    }

    /*
    Se copian los pixeles uno por uno usando punteros.
    */
    for (i = 0; i < total; i++) {
        *(copy + i) = *(pixels + i);
    }

    return copy;
}

/*
Esta funcion aplica el umbral a la imagen.

Si un pixel es mayor o igual al umbral, pasa a 255.
Si es menor al umbral, pasa a 0.
*/
void apply_threshold(unsigned char *pixels, int total, int threshold) {
    int i;

    /*
    Se recorre todo el arreglo de pixeles.
    Aqui se usa aritmetica de punteros, sin corchetes.
    */
    for (i = 0; i < total; i++) {
        if (*(pixels + i) >= threshold) {
            *(pixels + i) = 255;
        } else {
            *(pixels + i) = 0;
        }
    }
}

/*
Esta funcion genera el negativo de la imagen umbralizada.

Como la imagen ya tiene solo 0 y 255, el negativo invierte esos valores.
*/
unsigned char *make_negative(unsigned char *pixels, int total) {
    int i;
    unsigned char *negative;

    /*
    Se reserva memoria para la imagen negativa.
    */
    negative = malloc(total * sizeof(unsigned char));

    if (negative == NULL) {
        printf("No se pudo reservar memoria para el negativo.\n");
        return NULL;
    }

    /*
    Se calcula el negativo de cada pixel.
    Se usa 255 - pixel.
    */
    for (i = 0; i < total; i++) {
        *(negative + i) = 255 - *(pixels + i);
    }

    return negative;
}

/*
Esta funcion escribe una imagen en formato PGM P2.

Recibe el nombre del archivo, los pixeles, el ancho,
el alto y el valor maximo.
*/
void write_pgm(const char *filename,
               unsigned char *pixels,
               int width,
               int height,
               int max_val) {
    FILE *file;
    int i;
    int total = width * height;

    /*
    Se abre el archivo en modo escritura.
    */
    file = fopen(filename, "w");

    if (file == NULL) {
        printf("No se pudo escribir el archivo %s.\n", filename);
        return;
    }

    /*
    Se escribe el encabezado del archivo PGM.
    */
    fprintf(file, "P2\n");
    fprintf(file, "%d %d\n", width, height);
    fprintf(file, "%d\n", max_val);

    /*
    Se escriben los pixeles.
    Cada vez que se completa una fila, se hace un salto de linea.
    */
    for (i = 0; i < total; i++) {
        fprintf(file, "%d ", *(pixels + i));

        if ((i + 1) % width == 0) {
            fprintf(file, "\n");
        }
    }

    fclose(file);
}

/*
Esta funcion imprime las estadisticas pedidas.

Cuenta los pixeles blancos y negros en la imagen umbralizada.
Tambien calcula el promedio de los pixeles originales.
*/
void print_stats(unsigned char *original,
                 unsigned char *thresholded,
                 int total) {
    int i;
    int blancos = 0;
    int negros = 0;
    int suma = 0;
    double promedio;

    /*
    Se recorren todos los pixeles.
    */
    for (i = 0; i < total; i++) {

        /*
        Esta suma usa la imagen original, antes del umbral.
        */
        suma += *(original + i);

        /*
        En la imagen umbralizada solo deberia haber 0 o 255.
        */
        if (*(thresholded + i) == 255) {
            blancos++;
        } else if (*(thresholded + i) == 0) {
            negros++;
        }
    }

    /*
    Se calcula el promedio original.
    */
    promedio = (double) suma / total;

    printf("Pixeles blancos: %d\n", blancos);
    printf("Pixeles negros: %d\n", negros);
    printf("Promedio original: %.2f\n", promedio);
}

int main(void) {
    int width, height, max_val;
    int threshold;
    int total;
    unsigned char *pixels = NULL;
    unsigned char *original = NULL;
    unsigned char *negative = NULL;

    /*
    Se lee la imagen input.pgm.
    El archivo debe estar en la misma carpeta que el programa.
    */
    pixels = read_pgm("input.pgm", &width, &height, &max_val);

    if (pixels == NULL) {
        printf("No se pudo cargar la imagen.\n");
        return 1;
    }

    /*
    Se calcula el total de pixeles de la imagen.
    */
    total = width * height;

    /*
    Se muestra informacion basica de la imagen leida.
    */
    printf("Imagen leida correctamente.\n");
    printf("Ancho: %d\n", width);
    printf("Alto: %d\n", height);
    printf("Valor maximo: %d\n", max_val);

    /*
    Se pide el umbral al usuario.
    */
    printf("Ingrese el valor del umbral: ");

    if (scanf("%d", &threshold) != 1 || threshold < 0 || threshold > 255) {
        printf("Umbral invalido.\n");
        free(pixels);
        return 1;
    }

    /*
    Se guarda una copia de la imagen original antes de modificarla.
    */
    original = copy_pixels(pixels, total);

    if (original == NULL) {
        free(pixels);
        return 1;
    }

    /*
    Se aplica el umbral sobre pixels.
    Despues de esto, pixels queda como imagen blanco y negro.
    */
    apply_threshold(pixels, total, threshold);

    /*
    Se genera el negativo usando la imagen ya umbralizada.
    */
    negative = make_negative(pixels, total);

    if (negative == NULL) {
        free(original);
        free(pixels);
        return 1;
    }

    /*
    Se escriben los archivos de salida pedidos.
    */
    write_pgm("output_threshold.pgm", pixels, width, height, 255);
    write_pgm("output_negative.pgm", negative, width, height, 255);

    /*
    Se imprimen las estadisticas en la terminal.
    */
    print_stats(original, pixels, total);

    /*
    Se libera toda la memoria dinamica usada.
    */
    free(original);
    free(negative);
    free(pixels);

    return 0;
}
