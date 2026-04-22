#ifndef SEMANTIC_H
#define SEMANTIC_H

#include "ast.h"
#include "symbols.h"

/*
 * Recorre el AST ANTES de ejecutarlo y detecta:
 *   - Variable no declarada
 *   - Variable declarada dos veces en el mismo scope
 *   - Incompatibilidad de tipos en declaración
 *   - Función no declarada al momento de la llamada
 *   - Número incorrecto de argumentos en llamada a función
 *
 * Retorna 0 si no hay errores, > 0 si hay errores (número de errores).
 */
int semantic_check(ASTNode *program);

#endif /* SEMANTIC_H */
