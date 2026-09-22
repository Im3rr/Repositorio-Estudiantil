#include <stdio.h>

#define EQUIPOS 4
#define TIPOS_EVENTO 4
#define TOTAL_LOGS 18

int validarTipo(int evento) {
    if (evento >= 0 && evento < TIPOS_EVENTO) {
        return 1;
    }
    return 0;
}

void sumarUno(int *contador) {
    *contador = *contador + 1;
}

void procesarLogs(int equipos[], int eventos[], int total, int matriz[EQUIPOS][TIPOS_EVENTO]) {
    int *ptrEq = equipos;
    int *ptrEv = eventos;

    for (int i = 0; i < total; i++) {
        int eq = *(ptrEq + i);
        int ev = *(ptrEv + i);

        if (eq >= 0 && eq < EQUIPOS && validarTipo(ev) == 1) {
            sumarUno(&matriz[eq][ev]);
        }
    }
}

void mostrarLogs(int equipos[], int eventos[], int total) {
    printf("--- REGISTRO DE EVENTOS (LOGS) ---\n");
    printf("No.\tEquipo\tEvento\n");
    for (int i = 0; i < total; i++) {
        printf("%d\tPC-0%d\tTipo %d\n", i + 1, equipos[i] + 1, eventos[i]);
    }
    printf("\n");
}

void mostrarMatriz(int matriz[EQUIPOS][TIPOS_EVENTO]) {
    printf("--- RESUMEN: EQUIPOS X EVENTOS ---\n");
    printf("Equipo\tAcceso\tPassErr\tNoUser\tBloq\tTotal\n");

    for (int i = 0; i < EQUIPOS; i++) {
        int totalEquipo = 0;
        printf("PC-0%d\t", i + 1);

        for (int j = 0; j < TIPOS_EVENTO; j++) {
            int cantidad = matriz[i][j];
            totalEquipo = totalEquipo + cantidad;
            printf("%d\t", cantidad);
        }
        printf("%d\n", totalEquipo);
    }
    printf("\n");
}

void detectarAtaques(int matriz[EQUIPOS][TIPOS_EVENTO], int x) {
    printf("--- REGLA DE DETECCION (X = %d) ---\n", x);
    printf("Equipo\tFallos\tResultado\n");

    for (int i = 0; i < EQUIPOS; i++) {
        int passErr = matriz[i][1];
        int noUser  = matriz[i][2];
        int totalFallos = passErr + noUser;

        if (totalFallos > x) {
            printf("PC-0%d\t%d\tSOSPECHOSO (Mayor a %d)\n", i + 1, totalFallos, x);
        } else {
            printf("PC-0%d\t%d\tNORMAL\n", i + 1, totalFallos);
        }
    }
}

int main() {
    int equipos[TOTAL_LOGS] = {
        0, 1, 1, 2, 0, 1, 3, 3, 3, 1, 2, 3, 0, 1, 3, 2, 1, 2
    };

    int eventos[TOTAL_LOGS] = {
        0, 1, 1, 1, 0, 2, 1, 2, 1, 1, 2, 1, 0, 3, 0, 2, 0, 0
    };

    int matriz[EQUIPOS][TIPOS_EVENTO] = {
        {0, 0, 0, 0},
        {0, 0, 0, 0},
        {0, 0, 0, 0},
        {0, 0, 0, 0}
    };

    int x = 3;

    mostrarLogs(equipos, eventos, TOTAL_LOGS);
    procesarLogs(equipos, eventos, TOTAL_LOGS, matriz);
    mostrarMatriz(matriz);
    detectarAtaques(matriz, x);

    return 0;
}