#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_TEXTO 50
#define ARCHIVO "laboratorio.txt"

typedef struct {
    char nombre[MAX_TEXTO];
    char atributo[MAX_TEXTO];
    char habilidad[MAX_TEXTO];
    char debilidad[MAX_TEXTO];
    int fuerza;
    int ingenio;
    int victorias;
    int derrotas;
    int mutaciones;
    int liberado;
} Criatura;

typedef struct {
    int indice_a;
    int indice_b;
    char tipo[MAX_TEXTO];
    int puntaje;
    char nota[MAX_TEXTO];
} Experimento;

typedef struct {
    Criatura *especimenes;
    int cantidad;
    int capacidad;

    Experimento *historial;
    int cant_exp;
    int cap_exp;
} Laboratorio;

Laboratorio *lab_crear(int cap_inicial) {
    if (cap_inicial < 1) cap_inicial = 1;

    Laboratorio *lab = malloc(sizeof(Laboratorio));
    if (lab == NULL) return NULL;

    lab->especimenes = malloc((size_t)cap_inicial * sizeof(Criatura));
    if (lab->especimenes == NULL) {
        free(lab);
        return NULL;
    }

    lab->historial = malloc((size_t)cap_inicial * sizeof(Experimento));
    if (lab->historial == NULL) {
        free(lab->especimenes);
        free(lab);
        return NULL;
    }

    lab->cantidad = 0;
    lab->capacidad = cap_inicial;
    lab->cant_exp = 0;
    lab->cap_exp = cap_inicial;
    return lab;
}

int lab_agregar_criatura(Laboratorio *lab, Criatura c) {
    if (lab->cantidad == lab->capacidad) {
        int nueva_cap = lab->capacidad * 2;
        Criatura *tmp = realloc(lab->especimenes, (size_t)nueva_cap * sizeof(Criatura));
        if (tmp == NULL) return 0;

        lab->especimenes = tmp;
        lab->capacidad = nueva_cap;
    }

    lab->especimenes[lab->cantidad] = c;
    lab->cantidad++;
    return 1;
}

int lab_agregar_experimento(Laboratorio *lab, Experimento e) {
    if (lab->cant_exp == lab->cap_exp) {
        int nueva_cap = lab->cap_exp * 2;
        Experimento *tmp = realloc(lab->historial, (size_t)nueva_cap * sizeof(Experimento));
        if (tmp == NULL) return 0;

        lab->historial = tmp;
        lab->cap_exp = nueva_cap;
    }

    lab->historial[lab->cant_exp] = e;
    lab->cant_exp++;
    return 1;
}

void lab_destruir(Laboratorio *lab) {
    if (lab != NULL) {
        free(lab->especimenes);
        free(lab->historial);
        free(lab);
    }
}

int lab_puntuacion(const Criatura *c, int dificultad) {
    int pts = (c->fuerza * 3) + (c->ingenio * 4) + (c->mutaciones * 5)
              + (c->victorias * 4) - (c->derrotas * 2) - (dificultad * 2);

    if (c->liberado) {
        pts -= 20;
    }
    return pts;
}

void lab_cargar(Laboratorio *lab) {
    FILE *f = fopen(ARCHIVO, "r");
    if (f == NULL) return;

    int total_c = 0;
    int total_e = 0;
    if (fscanf(f, "%d;%d\n", &total_c, &total_e) != 2) {
        fclose(f);
        return;
    }

    for (int i = 0; i < total_c; i++) {
        Criatura c;
        if (fscanf(
                f,
                "%49[^;];%49[^;];%49[^;];%49[^;];%d;%d;%d;%d;%d;%d\n",
                c.nombre,
                c.atributo,
                c.habilidad,
                c.debilidad,
                &c.fuerza,
                &c.ingenio,
                &c.victorias,
                &c.derrotas,
                &c.mutaciones,
                &c.liberado
            ) == 10) {
            lab_agregar_criatura(lab, c);
        }
    }

    for (int i = 0; i < total_e; i++) {
        Experimento e;
        if (fscanf(
                f,
                "%d;%d;%49[^;];%d;%49[^\n]\n",
                &e.indice_a,
                &e.indice_b,
                e.tipo,
                &e.puntaje,
                e.nota
            ) == 5) {
            lab_agregar_experimento(lab, e);
        }
    }

    fclose(f);
}

void lab_guardar(const Laboratorio *lab) {
    FILE *f = fopen(ARCHIVO, "w");
    if (f == NULL) {
        printf("Error al abrir archivo para guardar.\n");
        return;
    }

    fprintf(f, "%d;%d\n", lab->cantidad, lab->cant_exp);

    for (int i = 0; i < lab->cantidad; i++) {
        const Criatura *c = &lab->especimenes[i];
        fprintf(
            f,
            "%s;%s;%s;%s;%d;%d;%d;%d;%d;%d\n",
            c->nombre,
            c->atributo,
            c->habilidad,
            c->debilidad,
            c->fuerza,
            c->ingenio,
            c->victorias,
            c->derrotas,
            c->mutaciones,
            c->liberado
        );
    }

    for (int i = 0; i < lab->cant_exp; i++) {
        const Experimento *e = &lab->historial[i];
        fprintf(
            f,
            "%d;%d;%s;%d;%s\n",
            e->indice_a,
            e->indice_b,
            e->tipo,
            e->puntaje,
            e->nota
        );
    }

    fclose(f);
    printf("Laboratorio guardado correctamente.\n");
}

void accion_listar(const Laboratorio *lab) {
    if (lab->cantidad == 0) {
        printf("No hay monstruos registrados.\n");
        return;
    }

    for (int i = 0; i < lab->cantidad; i++) {
        const Criatura *c = &lab->especimenes[i];
        const char *estado = c->liberado ? "[LIBERADO]" : "[ACTIVO]";
        printf(
            "%d. %s %s | Atr: %s | Hab: %s | Deb: %s | F:%d I:%d | V:%d D:%d M:%d\n",
            i + 1,
            estado,
            c->nombre,
            c->atributo,
            c->habilidad,
            c->debilidad,
            c->fuerza,
            c->ingenio,
            c->victorias,
            c->derrotas,
            c->mutaciones
        );
    }
}

void accion_crear(Laboratorio *lab) {
    Criatura c;

    printf("Nombre: ");
    getchar();
    fgets(c.nombre, MAX_TEXTO, stdin);
    c.nombre[strcspn(c.nombre, "\n")] = '\0';

    printf("Atributo defectuoso: ");
    fgets(c.atributo, MAX_TEXTO, stdin);
    c.atributo[strcspn(c.atributo, "\n")] = '\0';

    printf("Habilidad absurda: ");
    fgets(c.habilidad, MAX_TEXTO, stdin);
    c.habilidad[strcspn(c.habilidad, "\n")] = '\0';

    printf("Debilidad ridicula: ");
    fgets(c.debilidad, MAX_TEXTO, stdin);
    c.debilidad[strcspn(c.debilidad, "\n")] = '\0';

    printf("Fuerza (1-10): ");
    scanf("%d", &c.fuerza);
    printf("Ingenio (1-10): ");
    scanf("%d", &c.ingenio);

    if (c.fuerza < 1) c.fuerza = 1;
    if (c.fuerza > 10) c.fuerza = 10;
    if (c.ingenio < 1) c.ingenio = 1;
    if (c.ingenio > 10) c.ingenio = 10;

    c.victorias = 0;
    c.derrotas = 0;
    c.mutaciones = 0;
    c.liberado = 0;

    if (lab_agregar_criatura(lab, c)) {
        printf("Monstruo creado exitosamente.\n");
    } else {
        printf("Error: Memoria insuficiente.\n");
    }
}

void accion_combinar(Laboratorio *lab) {
    accion_listar(lab);
    if (lab->cantidad < 2) {
        printf("Se necesitan al menos 2 monstruos.\n");
        return;
    }

    int a, b;
    printf("Numero del primer monstruo: ");
    scanf("%d", &a);
    printf("Numero del segundo monstruo: ");
    scanf("%d", &b);

    if (a < 1 || a > lab->cantidad || b < 1 || b > lab->cantidad || a == b) {
        printf("Seleccion invalida.\n");
        return;
    }

    Criatura *c1 = &lab->especimenes[a - 1];
    Criatura *c2 = &lab->especimenes[b - 1];

    Criatura hijo;
    snprintf(hijo.nombre, MAX_TEXTO, "%.20s+%.20s", c1->nombre, c2->nombre);
    snprintf(hijo.atributo, MAX_TEXTO, "%.48s", c1->atributo);
    snprintf(hijo.habilidad, MAX_TEXTO, "%.48s", c2->habilidad);
    snprintf(hijo.debilidad, MAX_TEXTO, "%.48s", c1->debilidad);

    hijo.fuerza = (c1->fuerza + c2->fuerza) / 2;
    if (hijo.fuerza < 1) hijo.fuerza = 1;
    hijo.ingenio = ((c1->ingenio + c2->ingenio) / 2) + 1;
    if (hijo.ingenio > 10) hijo.ingenio = 10;

    hijo.victorias = 0;
    hijo.derrotas = 0;
    hijo.mutaciones = 1;
    hijo.liberado = 0;

    c1->mutaciones++;
    c2->mutaciones++;

    if (lab_agregar_criatura(lab, hijo)) {
        Experimento exp;
        exp.indice_a = a - 1;
        exp.indice_b = b - 1;
        strcpy(exp.tipo, "Combinacion");
        exp.puntaje = lab_puntuacion(&hijo, 2);
        strcpy(exp.nota, "Nueva criatura mutante creada");
        lab_agregar_experimento(lab, exp);
        printf("Nuevo especimen creado: %s\n", hijo.nombre);
    } else {
        printf("Error: Memoria insuficiente.\n");
    }
}

void accion_probar(Laboratorio *lab) {
    accion_listar(lab);
    if (lab->cantidad == 0) return;

    int pos, dif;
    printf("Numero de monstruo a probar: ");
    scanf("%d", &pos);
    printf("Dificultad de la prueba (1-10): ");
    scanf("%d", &dif);

    if (pos < 1 || pos > lab->cantidad) {
        printf("Numero invalido.\n");
        return;
    }
    if (dif < 1) dif = 1;
    if (dif > 10) dif = 10;

    Criatura *c = &lab->especimenes[pos - 1];
    if (c->liberado) {
        printf("El monstruo ya fue liberado.\n");
        return;
    }

    int pts = lab_puntuacion(c, dif);
    int meta = 20 + (dif * 2);

    Experimento exp;
    exp.indice_a = pos - 1;
    exp.indice_b = -1;
    strcpy(exp.tipo, "Prueba");
    exp.puntaje = pts;

    if (pts >= meta) {
        c->victorias++;
        strcpy(exp.nota, "Sobrevivio a la prueba");
        printf("Puntaje %d (meta %d): %s SOBREVIVE.\n", pts, meta, c->nombre);
    } else {
        c->derrotas++;
        strcpy(exp.nota, "Fallo la prueba");
        printf("Puntaje %d (meta %d): %s NO SUPERA la prueba.\n", pts, meta, c->nombre);
    }

    lab_agregar_experimento(lab, exp);
}

void accion_liberar(Laboratorio *lab) {
    accion_listar(lab);
    if (lab->cantidad == 0) return;

    int pos;
    printf("Numero de monstruo a liberar: ");
    scanf("%d", &pos);

    if (pos < 1 || pos > lab->cantidad) {
        printf("Numero invalido.\n");
        return;
    }

    Criatura *c = &lab->especimenes[pos - 1];
    if (c->liberado) {
        printf("Ya estaba liberado.\n");
        return;
    }

    c->liberado = 1;

    Experimento exp;
    exp.indice_a = pos - 1;
    exp.indice_b = -1;
    strcpy(exp.tipo, "Liberacion");
    exp.puntaje = lab_puntuacion(c, 1);
    strcpy(exp.nota, "Especimen liberado");
    lab_agregar_experimento(lab, exp);

    printf("%s fue liberado.\n", c->nombre);
}

void accion_historial(const Laboratorio *lab) {
    if (lab->cant_exp == 0) {
        printf("No hay experimentos registrados.\n");
        return;
    }

    for (int i = 0; i < lab->cant_exp; i++) {
        const Experimento *e = &lab->historial[i];
        printf(
            "%d. [%s] Monstruo #%d (vs #%d) | Pts: %d | %s\n",
            i + 1,
            e->tipo,
            e->indice_a + 1,
            e->indice_b >= 0 ? e->indice_b + 1 : 0,
            e->puntaje,
            e->nota
        );
    }
}

int main(void) {
    Laboratorio *lab = lab_crear(2);
    if (lab == NULL) {
        printf("Error al reservar memoria inicial.\n");
        return 1;
    }

    lab_cargar(lab);

    int opcion = -1;
    while (opcion != 0) {
        printf("\n1) Listar  2) Crear  3) Combinar  4) Probar  5) Liberar  6) Historial  0) Guardar y salir\n> ");
        if (scanf("%d", &opcion) != 1) break;

        switch (opcion) {
            case 1: accion_listar(lab); break;
            case 2: accion_crear(lab); break;
            case 3: accion_combinar(lab); break;
            case 4: accion_probar(lab); break;
            case 5: accion_liberar(lab); break;
            case 6: accion_historial(lab); break;
            case 0: lab_guardar(lab); break;
            default: printf("Opcion no valida.\n");
        }
    }

    lab_destruir(lab);
    return 0;
}
