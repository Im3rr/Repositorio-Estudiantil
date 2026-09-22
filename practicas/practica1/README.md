# Práctica 1: Análisis de Registros de Ataques

Programa en lenguaje C enfocado en el procesamiento, clasificación y análisis de registros (*logs*) de acceso a un servidor para detectar comportamientos anómalos o sospechosos de fuerza bruta.

## Descripción del Programa

El sistema captura secuencias cronológicas de accesos usando arreglos unidimensionales paralelos (identificador de equipo y tipo de evento). Empleando aritmética de apuntadores y paso de parámetros por referencia, construye una matriz de contingencia de dos dimensiones (*equipos × tipos de evento*) para cuantificar los intentos de cada estación. 

Posteriormente, aplica una regla de detección de seguridad con umbral configurable $X = 3$: todo equipo cuyos intentos sumados de **contraseña incorrecta** y **usuario inexistente** excedan dicho umbral ($> X$) es marcado como **Sospechoso**; de lo contrario, se clasifica como **Normal**.

---

## Compilación y Ejecución

Primero, colócate dentro de la carpeta de la práctica desde la terminal:

```bash
cd practicas/practica1

En Ubuntu:
Asegurarse de contar con el compilador GCC:
sudo apt update && sudo apt install build-essential

COMPILAR: gcc main.c -o programa

EJECUTAR: ./programa

EN WINDOWS:
Contar con MinGW-w64 configurado en el PATH del sistema o utilizar la terminal Git Bash.

COMPILAR: gcc main.c -o programa.exe

EJECUTAR: programa.exe

CON GITBASH:
COMPILAR: gcc main.c -o programa.exe

EJECUTAR: ./programa.exe
