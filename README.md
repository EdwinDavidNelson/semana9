# Backfile Compiler

Compilador para el lenguaje de programación **Backfile**, implementado con **Flex** y **Yacc/Bison** en C.  
Proyecto final del curso de Compiladores — Universidad Adventista de Centro América.

> Los archivos fuente usan la extensión `.bafle`. El compilador se invoca con `./backfile programa.bafle`.

---

## Compilar y ejecutar

```bash
# Compilar el compilador
make

# Ejecutar el archivo de demostración
make run

# Ejecutar el test de funciones
make test

# Ejecutar todos los archivos .bafle en tests/
make test-all

# Limpiar archivos generados
make clean

# Ejecutar un archivo específico
./backfile tests/mi_programa.bafle
```

---

## Pipeline de compilación

Cada vez que se ejecuta `./backfile archivo.bafle` se realizan estas fases en orden:

```
Código fuente .bafle
       │
       ▼
1. Análisis Léxico      lexer.l        Tokeniza el código fuente
       │
       ▼
2. Análisis Sintáctico  parser.y       Construye el AST
       │
       ▼
3. Análisis Semántico   src/semantic.c Verifica tipos y scopes ── si hay errores, se aborta aquí
       │
       ▼
4. Representación       src/ast.c      Imprime el árbol AST en consola
   Intermedia (AST)
       │
       ▼
5. Interpretación       src/interpreter.c  Ejecuta el programa
```

---

## Estructura del proyecto

```
semana9/
├── lexer.l                  Análisis léxico (Flex)
├── parser.y                 Análisis sintáctico + construcción del AST (Yacc)
├── Makefile                 Automatiza la compilación
├── README.md
├── src/
│   ├── ast.h / ast.c        Árbol de Sintaxis Abstracta (18 tipos de nodos)
│   ├── semantic.h / .c      Análisis semántico en 2 pasadas
│   ├── interpreter.h / .c   Intérprete: ejecuta el AST con scopes y funciones
│   └── symbols.h / .c       Tabla de símbolos con soporte de shadowing
└── tests/
    ├── demo.bafle            Programa de demostración básico
    ├── test_functions.bafle  Prueba de funciones y shadowing
    ├── test_completo.bafle   Prueba completa de todos los elementos del lenguaje
    └── test_errores.bafle    Prueba de detección de errores semánticos
```

---

## Sintaxis del lenguaje Backfile

### Comentarios

```
~~ Esto es un comentario de una línea
set x como bafle_int = 10;  ~~ también al final de una instrucción
```

### Tipos de datos

| Tipo | Descripción | Ejemplo |
|---|---|---|
| `bafle_int` | Entero de 32 bits | `42` |
| `bafle_float` | Punto flotante (doble precisión) | `3.14` |
| `bafle_char` | Un solo carácter ASCII | `'A'` |
| `bafle_str` | Cadena de texto | `"Hola"` |
| `bafle_bool` | Booleano: `1` (verdadero) / `0` (falso) | `1` |

### Declaración de variables

```
set <nombre> como <tipo> = <valor>;
```

```
set edad    como bafle_int   = 21;
set pi      como bafle_float = 3.14;
set inicial como bafle_char  = 'J';
set mensaje como bafle_str   = "Hola, Mundo";
set activo  como bafle_bool  = 1;
set result  como bafle_int   = (10 + 5) * 2;
```

### Operadores aritméticos

| Operador | Operación | Ejemplo |
|---|---|---|
| `+` | Suma | `10 + 3` → `13` |
| `-` | Resta | `10 - 3` → `7` |
| `*` | Multiplicación | `4 * 5` → `20` |
| `/` | División | `20 / 4` → `5` |
| `()` | Agrupación | `(2+3)*4` → `20` |

### Operadores de comparación

| Operador | Significado |
|---|---|
| `==` | Igual a |
| `!=` | Distinto de |
| `<` | Menor que |
| `>` | Mayor que |
| `<=` | Menor o igual que |
| `>=` | Mayor o igual que |

### Impresión en pantalla

```
show edad;          ~~ imprime el valor de una variable
show "Hola!";       ~~ imprime un literal de texto
show cuadrado(7);   ~~ imprime el resultado de una función
```

### Condicional si / sino

```
si (x > 5) {
    show "mayor que 5";
} sino {
    show "menor o igual a 5";
}
```

### Ciclo mientras

```
set contador como bafle_int = 1;
mientras (contador <= 5) {
    show contador;
    set contador como bafle_int = contador + 1;
}
```

> Backfile no tiene operador `++`. Para avanzar un contador se redeclara: `set i como bafle_int = i + 1;`

### Funciones

```
~~ Sin parámetros
func saludar() -> bafle_str {
    retorna "Hola!";
}

~~ Con parámetros
func cuadrado(bafle_int n) -> bafle_int {
    retorna n * n;
}

func sumar(bafle_int a, bafle_int b) -> bafle_int {
    retorna a + b;
}

~~ Llamadas
show saludar();
show cuadrado(7);
set total como bafle_int = sumar(10, 32);
```

### Variable Shadowing

Las funciones tienen su propio scope aislado. Una variable local con el mismo nombre que una global la sombrea dentro de la función sin afectar el scope global.

```
set x como bafle_int = 100;

func inner() -> bafle_int {
    set x como bafle_int = 999;  ~~ sombrea la x global
    retorna x;
}

show x;       ~~ imprime: 100
show inner(); ~~ imprime: 999
show x;       ~~ imprime: 100  (la global no cambia)
```

---

## Análisis semántico

El compilador verifica el programa completo **antes de ejecutarlo**. Si se encuentra al menos un error semántico, la compilación se aborta y no se ejecuta nada. Los errores se reportan con número de línea exacto.

### Errores detectados

| Error | Causa | Mensaje |
|---|---|---|
| Variable no declarada | Usar una variable antes de declararla | `variable 'x' no ha sido declarada.` |
| Declaración duplicada | Declarar la misma variable dos veces en el mismo scope | `variable 'x' ya fue declarada en este scope.` |
| Tipo incompatible | Asignar un valor de tipo diferente al declarado | `tipo incompatible al declarar 'msg': se declaró bafle_str pero se asignó bafle_int.` |
| Aritmética sobre string | Usar `+`, `-`, `*`, `/` con `bafle_str` | `operación aritmética no permitida con tipo bafle_str.` |
| Función no declarada | Llamar a una función que no existe | `función 'foo' no está declarada.` |
| Arity incorrecto | Pasar más o menos argumentos de los esperados | `función 'sumar' espera 2 argumento(s), se pasaron 3.` |
| Tipo de argumento | Pasar un argumento de tipo incompatible | `argumento 1 de 'cuadrado': tipo incompatible.` |

### Ejemplo de salida de errores

```
[Error Semántico | línea 12] variable 'fantasma' no ha sido declarada.
[Error Semántico | línea 18] variable 'repetida' ya fue declarada en este scope.
[Error Semántico | línea 22] tipo incompatible al declarar 'edad': se declaró bafle_int pero se asignó bafle_str.

3 error(es) semántico(s) encontrado(s). Compilación abortada.
```

---

## Representación intermedia (AST)

Antes de ejecutar, el compilador imprime el Árbol de Sintaxis Abstracta completo con formato visual. Ejemplo para `set x como bafle_int = 10 + 5;`:

```
Programa  (1 sentencias)
└─ set 'x'  tipo: bafle_int  (línea 1)
   └─ valor: (+  10
              5
```

---

## Errores comunes

| Mensaje | Fase | Causa |
|---|---|---|
| `[línea N] Error sintáctico: ...` | Sintáctico | Falta `;`, palabra clave mal escrita |
| `[línea N] Error léxico: caracter desconocido 'X'` | Léxico | Símbolo no reconocido |
| `No se puede abrir: archivo.bafle` | Entrada | La ruta del archivo no existe |
| `[Error Semántico | línea N] ...` | Semántico | Ver tabla de errores semánticos |
| `make: yacc not found` | Compilación | Instalar con `sudo apt install bison` |
| `make: flex not found` | Compilación | Instalar con `sudo apt install flex` |

---

## Palabras reservadas

`set` `como` `show` `func` `retorna` `si` `sino` `mientras`  
`bafle_int` `bafle_float` `bafle_char` `bafle_str` `bafle_bool`
