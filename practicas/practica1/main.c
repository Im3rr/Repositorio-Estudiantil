#include <stdio.h>
#include <stdbool.h>

#define NUM_EQUIPOS 4
#define NUM_EVENTOS 4
#define TOTAL_LOGS 18

#define EVT_ACCESO_CORRECTO   0
#define EVT_PASS_INCORRECTA   1
#define EVT_USR_INEXISTENTE   2
#define EVT_ACCESO_BLOQUEADO  3

const char *NOMBRES_EVENTOS[] = {
    "Acceso correcto",
    "Contrasena incorrecta",
    "Usuario inexistente",
    "Acceso bloqueado"
};

const char *NOMBRES_EQUIPOS[] = {
    "PC-01", "PC-02", "PC-03", "PC-04"
};

bool validarEvento(int codigoEvento) {
    return (codigoEvento >= 0 && codigoEvento < NUM_EVENTOS);
}

void actualizarContador(int *contador) {
    if (contador != NULL) {
        (*contador)++;
    }
}

void procesarRegistros(const int *ptrEquipos, const int *ptrEventos, int totalLogs, int (*matriz)[NUM_EVENTOS]) {
    for (int i = 0; i < totalLogs; i++) {
        int idEquipo = *(ptrEquipos + i);
        int tipoEvento = *(ptrEventos + i);

        if (idEquipo >= 0 && idEquipo < NUM_EQUIPOS && validarEvento(tipoEvento)) {
            int *celda = *(matriz + idEquipo) + tipoEvento;
            actualizarContador(celda);
        }
    }
}

void imprimirLog(const int *ptrEquipos, const int *ptrEventos, int totalLogs) {
    printf("====================================================\n");
    printf("              REGISTRO CRONOLOGICO (LOG)            \n");
    printf("====================================================\n");
    printf("%-5s | %-8s | %s\n", "No.", "Equipo", "Evento Registrado");
    printf("----------------------------------------------------\n");
    for (int i = 0; i < totalLogs; i++) {
        int idEquipo = *(ptrEquipos + i);
        int tipoEvento = *(ptrEventos + i);
        printf("%-5d | %-8s | %s\n", i + 1, NOMBRES_EQUIPOS[idEquipo], NOMBRES_EVENTOS[tipoEvento]);
    }
    printf("\n");
}

void imprimirMatriz(int matriz[NUM_EQUIPOS][NUM_EVENTOS]) {
    printf("==========================================================================\n");
    printf("                         RESUMEN POR EQUIPO                               \n");
    printf("==========================================================================\n");
    printf("%-8s | %-12s | %-12s | %-12s | %-10s | %-6s\n", 
           "Equipo", "Acc. Correct", "Pass Incorr.", "Usr Inexist.", "Bloqueado", "Total");
    printf("--------------------------------------------------------------------------\n");

    for (int i = 0; i < NUM_EQUIPOS; i++) {
        int totalEquipo = 0;
        printf("%-8s | ", NOMBRES_EQUIPOS[i]);
        for (int j = 0; j < NUM_EVENTOS; j++) {
            int valor = *(*(matriz + i) + j);
            totalEquipo += valor;
            printf("%-12d | ", valor);
        }
        printf("%-6d\n", totalEquipo);
    }
    printf("\n");
}

void evaluarSeguridad(int matriz[NUM_EQUIPOS][NUM_EVENTOS], int umbralX) {
    printf("==========================================================================\n");
    printf("                DIAGNOSTICO DE SEGURIDAD (Umbral X = %d)                   \n", umbralX);
    printf("==========================================================================\n");
    printf("%-8s | %-15s | %-16s | %s\n", "Equipo", "Intentos Fall.", "Estado", "Justificacion");
    printf("--------------------------------------------------------------------------\n");

    for (int i = 0; i < NUM_EQUIPOS; i++) {
        int passIncorr = *(*(matriz + i) + EVT_PASS_INCORRECTA);
        int usrInexist = *(*(matriz + i) + EVT_USR_INEXISTENTE);
        int totalFallos = passIncorr + usrInexist;

        if (totalFallos > umbralX) {
            printf("%-8s | %-15d | SOSPECHOSO      | Supera el umbral (> %d fallos)\n", 
                   NOMBRES_EQUIPOS[i], totalFallos, umbralX);
        } else {
            printf("%-8s | %-15d | NORMAL          | Comportamiento seguro (<= %d fallos)\n", 
                   NOMBRES_EQUIPOS[i], totalFallos, umbralX);
        }
    }
    printf("==========================================================================\n");
}

int main() {
    int equipos_log[TOTAL_LOGS] = {
        0, 1, 1, 2, 0, 1, 3, 3, 3, 1, 2, 3, 0, 1, 3, 2, 1, 2
    };

    int eventos_log[TOTAL_LOGS] = {
        EVT_ACCESO_CORRECTO,
        EVT_PASS_INCORRECTA,
        EVT_PASS_INCORRECTA,
        EVT_PASS_INCORRECTA,
        EVT_ACCESO_CORRECTO,
        EVT_USR_INEXISTENTE,
        EVT_PASS_INCORRECTA,
        EVT_USR_INEXISTENTE,
        EVT_PASS_INCORRECTA,
        EVT_PASS_INCORRECTA,
        EVT_USR_INEXISTENTE,
        EVT_PASS_INCORRECTA,
        EVT_ACCESO_CORRECTO,
        EVT_ACCESO_BLOQUEADO,
        EVT_ACCESO_CORRECTO,
        EVT_USR_INEXISTENTE,
        EVT_ACCESO_CORRECTO,
        EVT_ACCESO_CORRECTO
    };

    int matriz[NUM_EQUIPOS][NUM_EVENTOS] = {0};
    int umbralX = 3;

    imprimirLog(equipos_log, eventos_log, TOTAL_LOGS);
    procesarRegistros(equipos_log, eventos_log, TOTAL_LOGS, matriz);
    imprimirMatriz(matriz);
    evaluarSeguridad(matriz, umbralX);

    return 0;
}