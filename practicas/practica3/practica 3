#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define ARCH_SUP_FINAL "supervivientes.txt"
#define ARCH_SUP_TMP   "supervivientes.tmp"
#define ARCH_REC_FINAL "recursos.txt"
#define ARCH_REC_TMP   "recursos.tmp"
#define ARCH_HIS_FINAL "historial.txt"
#define ARCH_HIS_TMP   "historial.tmp"
#define RECURSO_AGUA 1
#define RECURSO_COMIDA 2
#define RECURSO_MEDICAMENTOS 3
#define RECURSO_MUNICIONES 4
#define MAX_TEXTO_EVENTO 60

typedef struct {
    char nombre[31];
    int edad;
    char habilidad[31];
    int salud;
} Superviviente;

typedef struct {
    int agua;
    int comida;
    int medicamentos;
    int municiones;
} Recursos;

typedef struct {
    int dia;
    char descripcion[MAX_TEXTO_EVENTO + 1];
} Evento;

typedef struct {
    Superviviente *lista;
    int n;
    int capacidad;
    Recursos recursos;
    Evento *historial;
    int num_eventos;
    int cap_historial;
    int dia_actual;
} Refugio;

void inicializar_refugio(Refugio *r) {
    if (r == NULL) {
        return;
    }
    r->lista = NULL;
    r->n = 0;
    r->capacidad = 0;
    r->recursos.agua = 20;
    r->recursos.comida = 20;
    r->recursos.medicamentos = 10;
    r->recursos.municiones = 15;
    r->historial = NULL;
    r->num_eventos = 0;
    r->cap_historial = 0;
    r->dia_actual = 1;
}

void mostrar_recursos(const Refugio *r) {
    if (r == NULL) {
        printf("Refugio no valido.\n");
        return;
    }

    printf("\n--- Recursos del refugio ---\n");
    printf("Agua:         %d\n", r->recursos.agua);
    printf("Comida:       %d\n", r->recursos.comida);
    printf("Medicamentos: %d\n", r->recursos.medicamentos);
    printf("Municiones:   %d\n", r->recursos.municiones);
    printf("................................\n");
}

int modificar_recurso(Refugio *r, int tipo, int cantidad) {
    if (r == NULL || cantidad == 0) {
        printf("Operacion invalida: refugio nulo o cantidad igual a 0.\n");
        return 0;
    }

    int *recurso_ptr = NULL;
    switch (tipo) {
        case RECURSO_AGUA:
            recurso_ptr = &r->recursos.agua;
            break;
        case RECURSO_COMIDA:
            recurso_ptr = &r->recursos.comida;
            break;
        case RECURSO_MEDICAMENTOS:
            recurso_ptr = &r->recursos.medicamentos;
            break;
        case RECURSO_MUNICIONES:
            recurso_ptr = &r->recursos.municiones;
            break;
        default:
            printf("Tipo de recurso invalido (%d).\n", tipo);
            return 0;
    }

    if (cantidad < 0) {
        int salida = -cantidad;
        if (salida > *recurso_ptr) {
            printf("Salida rechazada: disponible %d, solicitado %d.\n", *recurso_ptr, salida);
            return 0;
        }
        *recurso_ptr -= salida;
        return 1;
    }

    *recurso_ptr += cantidad;
    return 1;
}

static void limpiar_buffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {
    }
}

static void quitar_salto_linea(char *cadena) {
    size_t len = strlen(cadena);
    if (len > 0 && cadena[len - 1] == '\n') {
        cadena[len - 1] = '\0';
    }
}

static int solo_espacios_o_vacio(const char *cadena) {
    if (cadena[0] == '\0') {
        return 1;
    }
    for (size_t i = 0; cadena[i] != '\0'; i++) {
        if (cadena[i] != ' ' && cadena[i] != '\t' && cadena[i] != '\r') {
            return 0;
        }
    }
    return 1;
}

int agregar_superviviente(Refugio *r) {
    if (r == NULL) {
        return 0;
    }

    Superviviente nuevo;
    memset(&nuevo, 0, sizeof(Superviviente));

    printf("Nombre del superviviente: ");
    if (fgets(nuevo.nombre, sizeof(nuevo.nombre), stdin) == NULL) {
        printf("Error al leer el nombre.\n");
        return 0;
    }
    quitar_salto_linea(nuevo.nombre);
    if (strchr(nuevo.nombre, '\0') == nuevo.nombre + sizeof(nuevo.nombre) - 1 &&
        strlen(nuevo.nombre) == sizeof(nuevo.nombre) - 1) {
        limpiar_buffer();
    }

    if (solo_espacios_o_vacio(nuevo.nombre)) {
        printf("Nombre invalido: no puede estar vacio.\n");
        return 0;
    }

    printf("Edad: ");
    if (scanf("%d", &nuevo.edad) != 1 || nuevo.edad <= 0) {
        printf("Edad invalida: debe ser mayor que 0.\n");
        limpiar_buffer();
        return 0;
    }
    limpiar_buffer();

    printf("Habilidad especial: ");
    if (fgets(nuevo.habilidad, sizeof(nuevo.habilidad), stdin) == NULL) {
        printf("Error al leer la habilidad especial.\n");
        return 0;
    }
    quitar_salto_linea(nuevo.habilidad);
    if (strlen(nuevo.habilidad) == sizeof(nuevo.habilidad) - 1) {
        limpiar_buffer();
    }

    printf("Salud (0 a 100): ");
    if (scanf("%d", &nuevo.salud) != 1 || nuevo.salud < 0 || nuevo.salud > 100) {
        printf("Salud invalida: debe estar entre 0 y 100.\n");
        limpiar_buffer();
        return 0;
    }
    limpiar_buffer();

    if (r->capacidad == 0 || r->lista == NULL) {
        int nueva_capacidad = 4;
        Superviviente *nueva_lista = malloc((size_t)nueva_capacidad * sizeof(Superviviente));
        if (nueva_lista == NULL) {
            printf("Error: no se pudo reservar memoria inicial con malloc.\n");
            return 0;
        }
        r->lista = nueva_lista;
        r->capacidad = nueva_capacidad;
        r->n = 0;
    } else if (r->n >= r->capacidad) {
        int nueva_capacidad = r->capacidad * 2;
        Superviviente *temp = realloc(r->lista, (size_t)nueva_capacidad * sizeof(Superviviente));
        if (temp == NULL) {
            printf("Error: no se pudo ampliar memoria con realloc. Se conservan los registros previos.\n");
            return 0;
        }
        r->lista = temp;
        r->capacidad = nueva_capacidad;
    }

    r->lista[r->n] = nuevo;
    r->n++;
    return 1;
}

void listar_supervivientes(const Refugio *r) {
    if (r == NULL || r->n <= 0 || r->lista == NULL) {
        printf("No hay supervivientes registrados en el refugio.\n");
        return;
    }

    printf("\n--- Lista de supervivientes (%d) ---\n", r->n);
    for (int i = 0; i < r->n; i++) {
        printf("%d. Nombre: %s | Edad: %d | Habilidad: %s | Salud: %d\n",
               i + 1,
               r->lista[i].nombre,
               r->lista[i].edad,
               r->lista[i].habilidad,
               r->lista[i].salud);
    }
    printf("..................................................\n");
}

int registrar_evento(Refugio *r, const char *texto) {
    if (r == NULL || texto == NULL || texto[0] == '\0') {
        printf("Evento invalido: refugio nulo o descripcion vacia.\n");
        return 0;
    }

    if (r->cap_historial == 0 || r->historial == NULL) {
        int nueva_cap = 4;
        Evento *nuevo_hist = malloc((size_t)nueva_cap * sizeof(Evento));
        if (nuevo_hist == NULL) {
            printf("Error: no se pudo reservar memoria para el historial.\n");
            return 0;
        }
        r->historial = nuevo_hist;
        r->cap_historial = nueva_cap;
        r->num_eventos = 0;
    } else if (r->num_eventos >= r->cap_historial) {
        int nueva_cap = r->cap_historial * 2;
        Evento *temp = realloc(r->historial, (size_t)nueva_cap * sizeof(Evento));
        if (temp == NULL) {
            printf("Error: no se pudo ampliar memoria del historial. Se conserva el anterior.\n");
            return 0;
        }
        r->historial = temp;
        r->cap_historial = nueva_cap;
    }

    Evento nuevo_ev;
    nuevo_ev.dia = r->dia_actual;
    snprintf(nuevo_ev.descripcion, sizeof(nuevo_ev.descripcion), "%s", texto);

    r->historial[r->num_eventos] = nuevo_ev;
    r->num_eventos++;
    return 1;
}

void mostrar_historial(const Refugio *r) {
    if (r == NULL || r->num_eventos <= 0 || r->historial == NULL) {
        printf("No hay acontecimientos registrados en el historial.\n");
        return;
    }

    printf("\n--- Historial de acontecimientos (%d) ---\n", r->num_eventos);
    for (int i = 0; i < r->num_eventos; i++) {
        printf("%d. [Dia %d] %s\n",
               i + 1,
               r->historial[i].dia,
               r->historial[i].descripcion);
    }
    printf("..................................................\n");
}

int dar_de_baja(Refugio *r, const char *nombre) {
    if (r == NULL || nombre == NULL || nombre[0] == '\0') {
        printf("Baja invalida: refugio nulo o nombre vacio.\n");
        return 0;
    }

    if (r->lista == NULL || r->n <= 0) {
        printf("No hay supervivientes registrados para dar de baja a '%s'.\n", nombre);
        return 0;
    }

    int indice = -1;
    for (int i = 0; i < r->n; i++) {
        if (strcmp(r->lista[i].nombre, nombre) == 0) {
            indice = i;
            break;
        }
    }

    if (indice == -1) {
        printf("Superviviente '%s' no encontrado. No se realizaron cambios.\n", nombre);
        return 0;
    }

    char nombre_baja[31];
    snprintf(nombre_baja, sizeof(nombre_baja), "%s", r->lista[indice].nombre);
    for (int i = indice; i < r->n - 1; i++) {
        r->lista[i] = r->lista[i + 1];
    }
    r->n--;

    if (r->n == 0) {
        free(r->lista);
        r->lista = NULL;
        r->capacidad = 0;
    } else if (r->capacidad > 4 && r->n <= r->capacidad / 2) {
        int nueva_cap = r->capacidad / 2;
        if (nueva_cap < r->n) {
            nueva_cap = r->n;
        }
        Superviviente *temp = realloc(r->lista, (size_t)nueva_cap * sizeof(Superviviente));
        if (temp != NULL) {
            r->lista = temp;
            r->capacidad = nueva_cap;
        }
    }
    char msg[MAX_TEXTO_EVENTO + 1];
    snprintf(msg, sizeof(msg), "Baja de superviviente: %s", nombre_baja);
    registrar_evento(r, msg);

    return 1;
}
    int expedicion(Refugio *r) {
    if (r == NULL || r->lista == NULL || r->n <= 0) {
        printf("Expedicion cancelada: no hay supervivientes con salud mayor que 0.\n");
        return 0;
    }

    int num_aptos = 0;
    for (int i = 0; i < r->n; i++) {
        if (r->lista[i].salud > 0) {
            num_aptos++;
        }
    }

    if (num_aptos == 0) {
        printf("Expedicion cancelada: no hay supervivientes con salud mayor que 0.\n");
        return 0;
    }

    int costo_municiones = 3;
    if (!modificar_recurso(r, RECURSO_MUNICIONES, -costo_municiones)) {
        printf("Expedicion cancelada: no alcanzan las municiones (%d requeridas).\n", costo_municiones);
        return 0;
    }

    int objetivo = rand() % num_aptos;
    int elegido_idx = -1;
    int contador_aptos = 0;
    for (int i = 0; i < r->n; i++) {
        if (r->lista[i].salud > 0) {
            if (contador_aptos == objetivo) {
                elegido_idx = i;
                break;
            }
            contador_aptos++;
        }
    }

    if (elegido_idx < 0) {
        return 0;
    }

    char nombre_expedicionario[31];
    snprintf(nombre_expedicionario, sizeof(nombre_expedicionario), "%s", r->lista[elegido_idx].nombre);

    int dano = 15 + (rand() % 21);
    r->lista[elegido_idx].salud -= dano;
    if (r->lista[elegido_idx].salud < 0) {
        r->lista[elegido_idx].salud = 0;
    }

    int agua_ganada = 4;
    int comida_ganada = 4;
    modificar_recurso(r, RECURSO_AGUA, agua_ganada);
    modificar_recurso(r, RECURSO_COMIDA, comida_ganada);

    int desaparecio = (r->lista[elegido_idx].salud == 0) || ((rand() % 100) < 30);

char resumen[MAX_TEXTO_EVENTO + 1];
    if (desaparecio) {
        snprintf(resumen, sizeof(resumen), "Exp: %.25s desaparecio (+%da/+%dc)",
                 nombre_expedicionario, agua_ganada, comida_ganada);
        registrar_evento(r, resumen);
        dar_de_baja(r, nombre_expedicionario);
        printf("Expedicion con baja: %s perdio %d salud y desaparecio. +%d agua, +%d comida.\n",
               nombre_expedicionario, dano, agua_ganada, comida_ganada);
    } else {
        snprintf(resumen, sizeof(resumen), "Exp: %.25s -%d sal (+%da/+%dc)",
                 nombre_expedicionario, dano, agua_ganada, comida_ganada);
        registrar_evento(r, resumen);
        printf("Expedicion exitosa: %s perdio %d salud (actual: %d). +%d agua, +%d comida.\n",
               nombre_expedicionario, dano, r->lista[elegido_idx].salud, agua_ganada, comida_ganada);
    }
    return 1;
}

void perder_recurso(Refugio *r, int tipo, int cantidad) {
    if (r == NULL || cantidad <= 0) {
        printf("Perdida de recurso invalida: refugio nulo o cantidad menor o igual a 0.\n");
        return;
    }

    int *recurso_ptr = NULL;
    const char *nombre_recurso = NULL;

    switch (tipo) {
        case RECURSO_AGUA:
            recurso_ptr = &r->recursos.agua;
            nombre_recurso = "agua";
            break;
        case RECURSO_COMIDA:
            recurso_ptr = &r->recursos.comida;
            nombre_recurso = "comida";
            break;
        case RECURSO_MEDICAMENTOS:
            recurso_ptr = &r->recursos.medicamentos;
            nombre_recurso = "medicamentos";
            break;
        case RECURSO_MUNICIONES:
            recurso_ptr = &r->recursos.municiones;
            nombre_recurso = "municiones";
            break;
        default:
            printf("Tipo de recurso invalido (%d) en perdida de recurso.\n", tipo);
            return;
    }

    int disponible = *recurso_ptr;
    int perdido = (cantidad >= disponible) ? disponible : cantidad;
    *recurso_ptr = disponible - perdido;
    if (*recurso_ptr < 0) {
        *recurso_ptr = 0;
    }

    char msg[MAX_TEXTO_EVENTO + 1];
    snprintf(msg, sizeof(msg), "Perdida de %d de %.15s (quedan %d)",
             perdido, nombre_recurso, *recurso_ptr);
    registrar_evento(r, msg);

    printf("Perdida registrada: se perdieron %d de %s (disponible ahora: %d).\n",
           perdido, nombre_recurso, *recurso_ptr);
}

static void limpiar_temporales_guardado(void) {
    remove(ARCH_SUP_TMP);
    remove(ARCH_REC_TMP);
    remove(ARCH_HIS_TMP);
}

int guardar_partida(const Refugio *r) {
    if (r == NULL) {
        printf("Error al guardar: refugio nulo.\n");
        return 0;
    }

    if (r->n > 0 && r->lista == NULL) {
        printf("Error al guardar: lista de supervivientes inconsistente.\n");
        return 0;
    }

    if (r->num_eventos > 0 && r->historial == NULL) {
        printf("Error al guardar: historial inconsistente.\n");
        return 0;
    }

    FILE *f_sup = fopen(ARCH_SUP_TMP, "w");
    if (f_sup == NULL) {
        printf("Error al abrir '%s' para guardar supervivientes.\n", ARCH_SUP_TMP);
        limpiar_temporales_guardado();
        return 0;
    }

    if (fprintf(f_sup, "%d\n", r->n) < 0) {
        printf("Error al escribir cantidad de supervivientes.\n");
        fclose(f_sup);
        limpiar_temporales_guardado();
        return 0;
    }

    for (int i = 0; i < r->n; i++) {
        if (fprintf(f_sup, "%d|%d|%s|%s\n",
                    r->lista[i].edad,
                    r->lista[i].salud,
                    r->lista[i].nombre,
                    r->lista[i].habilidad) < 0) {
            printf("Error al escribir el superviviente %d.\n", i + 1);
            fclose(f_sup);
            limpiar_temporales_guardado();
            return 0;
        }
    }

    if (fclose(f_sup) != 0) {
        printf("Error al cerrar '%s'.\n", ARCH_SUP_TMP);
        limpiar_temporales_guardado();
        return 0;
    }

    FILE *f_rec = fopen(ARCH_REC_TMP, "w");
    if (f_rec == NULL) {
        printf("Error al abrir '%s' para guardar recursos.\n", ARCH_REC_TMP);
        limpiar_temporales_guardado();
        return 0;
    }

    if (fprintf(f_rec, "%d %d %d %d %d\n",
                r->recursos.agua,
                r->recursos.comida,
                r->recursos.medicamentos,
                r->recursos.municiones,
                r->dia_actual) < 0) {
        printf("Error al escribir los recursos del refugio.\n");
        fclose(f_rec);
        limpiar_temporales_guardado();
        return 0;
    }

    if (fclose(f_rec) != 0) {
        printf("Error al cerrar '%s'.\n", ARCH_REC_TMP);
        limpiar_temporales_guardado();
        return 0;
    }

    FILE *f_his = fopen(ARCH_HIS_TMP, "w");
    if (f_his == NULL) {
        printf("Error al abrir '%s' para guardar historial.\n", ARCH_HIS_TMP);
        limpiar_temporales_guardado();
        return 0;
    }

    if (fprintf(f_his, "%d\n", r->num_eventos) < 0) {
        printf("Error al escribir cantidad de eventos.\n");
        fclose(f_his);
        limpiar_temporales_guardado();
        return 0;
    }

    for (int i = 0; i < r->num_eventos; i++) {
        if (fprintf(f_his, "%d|%s\n",
                    r->historial[i].dia,
                    r->historial[i].descripcion) < 0) {
            printf("Error al escribir el evento %d.\n", i + 1);
            fclose(f_his);
            limpiar_temporales_guardado();
            return 0;
        }
    }

    if (fclose(f_his) != 0) {
        printf("Error al cerrar '%s'.\n", ARCH_HIS_TMP);
        limpiar_temporales_guardado();
        return 0;
    }

    remove(ARCH_SUP_FINAL);
    if (rename(ARCH_SUP_TMP, ARCH_SUP_FINAL) != 0) {
        printf("Error al renombrar '%s' a '%s'.\n", ARCH_SUP_TMP, ARCH_SUP_FINAL);
        limpiar_temporales_guardado();
        return 0;
    }

    remove(ARCH_REC_FINAL);
    if (rename(ARCH_REC_TMP, ARCH_REC_FINAL) != 0) {
        printf("Error al renombrar '%s' a '%s'.\n", ARCH_REC_TMP, ARCH_REC_FINAL);
        limpiar_temporales_guardado();
        return 0;
    }

    remove(ARCH_HIS_FINAL);
    if (rename(ARCH_HIS_TMP, ARCH_HIS_FINAL) != 0) {
        printf("Error al renombrar '%s' a '%s'.\n", ARCH_HIS_TMP, ARCH_HIS_FINAL);
        limpiar_temporales_guardado();
        return 0;
    }

    printf("Partida guardada de forma segura.\n");
    return 1;
}

static int archivo_existe(const char *ruta) {
    FILE *f = fopen(ruta, "r");
    if (f != NULL) {
        fclose(f);
        return 1;
    }
    return 0;
}

static void limpiar_linea_archivo(char *s) {
    size_t len = strlen(s);
    while (len > 0 && (s[len - 1] == '\n' || s[len - 1] == '\r')) {
        s[--len] = '\0';
    }
}

int cargar_partida(Refugio *r) {
    if (r == NULL) {
        return 0;
    }

    int ex_sup = archivo_existe(ARCH_SUP_FINAL);
    int ex_rec = archivo_existe(ARCH_REC_FINAL);
    int ex_his = archivo_existe(ARCH_HIS_FINAL);


    if (!ex_sup && !ex_rec && !ex_his) {
        printf("No se encontro partida guardada. Iniciando refugio nuevo.\n");
        return 1;
    }


    if (!ex_sup || !ex_rec || !ex_his) {
        printf("Error: guardado incompleto (falta uno o mas archivos). No se cargaran datos.\n");
        return 0;
    }

    Superviviente *temp_lista = NULL;
    int temp_n = 0;
    int temp_cap = 0;
    Recursos temp_rec = {20, 20, 10, 15};
    int temp_dia = 1;
    Evento *temp_hist = NULL;
    int temp_num_ev = 0;
    int temp_cap_ev = 0;
    char linea[256];

    FILE *f_sup = fopen(ARCH_SUP_FINAL, "r");
    if (f_sup == NULL) {
        printf("Error al abrir '%s'.\n", ARCH_SUP_FINAL);
        return 0;
    }

    if (fgets(linea, sizeof(linea), f_sup) == NULL || sscanf(linea, "%d", &temp_n) != 1 || temp_n < 0) {
        printf("Error: cantidad de supervivientes invalida en '%s'.\n", ARCH_SUP_FINAL);
        fclose(f_sup);
        return 0;
    }

    if (temp_n > 0) {
        temp_cap = (temp_n < 4) ? 4 : temp_n;
        temp_lista = malloc((size_t)temp_cap * sizeof(Superviviente));
        if (temp_lista == NULL) {
            printf("Error: sin memoria para cargar.\n");
            fclose(f_sup);
            return 0;
        }

        for (int i = 0; i < temp_n; i++) {
            if (fgets(linea, sizeof(linea), f_sup) == NULL) {
                printf("Error: faltan registros de supervivientes en '%s'.\n", ARCH_SUP_FINAL);
                free(temp_lista);
                fclose(f_sup);
                return 0;
            }
            limpiar_linea_archivo(linea);

            Superviviente s;
            memset(&s, 0, sizeof(Superviviente));
            int leidos = sscanf(linea, "%d|%d|%30[^|]|%30[^\n]",
                                &s.edad, &s.salud, s.nombre, s.habilidad);
            if (leidos < 3 || s.edad <= 0 || s.salud < 0 || s.salud > 100 || solo_espacios_o_vacio(s.nombre)) {
                printf("Error:fuera de rango en superviviente %d.\n", i + 1);
                free(temp_lista);
                fclose(f_sup);
                return 0;
            }
            temp_lista[i] = s;
        }
    }
    fclose(f_sup);

    FILE *f_rec = fopen(ARCH_REC_FINAL, "r");
    if (f_rec == NULL) {
        printf("Error al abrir '%s'.\n", ARCH_REC_FINAL);
        free(temp_lista);
        return 0;
    }

    if (fgets(linea, sizeof(linea), f_rec) == NULL ||
        sscanf(linea, "%d %d %d %d %d",
               &temp_rec.agua,
               &temp_rec.comida,
               &temp_rec.medicamentos,
               &temp_rec.municiones,
               &temp_dia) != 5 ||
        temp_rec.agua < 0 || temp_rec.comida < 0 ||
        temp_rec.medicamentos < 0 || temp_rec.municiones < 0 || temp_dia < 1) {
        printf("Error: recursos o dia fuera de rango en '%s'.\n", ARCH_REC_FINAL);
        free(temp_lista);
        fclose(f_rec);
        return 0;
    }
    fclose(f_rec);

    FILE *f_his = fopen(ARCH_HIS_FINAL, "r");
    if (f_his == NULL) {
        printf("Error al abrir '%s'.\n", ARCH_HIS_FINAL);
        free(temp_lista);
        return 0;
    }

    if (fgets(linea, sizeof(linea), f_his) == NULL || sscanf(linea, "%d", &temp_num_ev) != 1 || temp_num_ev < 0) {
        printf("Error: cantidad de eventos invalida en '%s'.\n", ARCH_HIS_FINAL);
        free(temp_lista);
        fclose(f_his);
        return 0;
    }

    if (temp_num_ev > 0) {
        temp_cap_ev = (temp_num_ev < 4) ? 4 : temp_num_ev;
        temp_hist = malloc((size_t)temp_cap_ev * sizeof(Evento));
        if (temp_hist == NULL) {
            printf("Error: sin memoria para cargar historial.\n");
            free(temp_lista);
            fclose(f_his);
            return 0;
        }

        for (int i = 0; i < temp_num_ev; i++) {
            if (fgets(linea, sizeof(linea), f_his) == NULL) {
                printf("Error: faltan registros en '%s'.\n", ARCH_HIS_FINAL);
                free(temp_hist);
                free(temp_lista);
                fclose(f_his);
                return 0;
            }
            limpiar_linea_archivo(linea);

            Evento ev;
            memset(&ev, 0, sizeof(Evento));
            if (sscanf(linea, "%d|%60[^\n]", &ev.dia, ev.descripcion) != 2 ||
                ev.dia < 1 || solo_espacios_o_vacio(ev.descripcion)) {
                printf("Error: evento %d danado o fuera de rango en '%s'.\n", i + 1, ARCH_HIS_FINAL);
                free(temp_hist);
                free(temp_lista);
                fclose(f_his);
                return 0;
            }
            temp_hist[i] = ev;
        }
    }
    fclose(f_his);

    free(r->lista);
    free(r->historial);

    r->lista = temp_lista;
    r->n = temp_n;
    r->capacidad = temp_cap;
    r->recursos = temp_rec;
    r->dia_actual = temp_dia;
    r->historial = temp_hist;
    r->num_eventos = temp_num_ev;
    r->cap_historial = temp_cap_ev;

    printf("Partida recuperada correctamente (Dia %d, %d supervivientes, %d eventos).\n",
           r->dia_actual, r->n, r->num_eventos);
    return 1;
}
int main(void) {
    Refugio refugio;
    inicializar_refugio(&refugio);

    registrar_evento(&refugio, "Estado previo intacto.");

    printf("--- Prueba HU-11: Datos danados o fuera de rango ---\n");
    int res = cargar_partida(&refugio);
    printf("Resultado cargar_partida: %d (esperado: 0)\n", res);

    listar_supervivientes(&refugio);
    mostrar_recursos(&refugio);
    mostrar_historial(&refugio);

    free(refugio.lista);
    refugio.lista = NULL;
    free(refugio.historial);
    refugio.historial = NULL;
    return 0;
}
