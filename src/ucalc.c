#include "ucalc.h"
#include <memory.h>
#include <stdlib.h>

typedef enum expr_type {
    EXPR_VAL,
    EXPR_ADD,
    EXPR_MUL,
    EXPR_NEG,
    EXPR_INV,
} expr_type_t;

struct ucalc_expr {
    const expr_type_t type;
    int ref;
    union {
        const double value;
        ucalc_expr_t *const expr;
        struct {
            ucalc_expr_t *const fst;
            ucalc_expr_t *const snd;
        } pair;
    } as;
};

double ucalc_evaluate(ucalc_expr_t *expr) {
    switch (expr->type) {
    case EXPR_VAL:
        return expr->as.value;
    case EXPR_NEG:
        return -ucalc_evaluate(expr->as.expr);
    case EXPR_INV:
        return 1 / ucalc_evaluate(expr->as.expr);
    case EXPR_ADD:
        return ucalc_evaluate(expr->as.pair.fst) + ucalc_evaluate(expr->as.pair.snd);
    case EXPR_MUL:
        return ucalc_evaluate(expr->as.pair.fst) * ucalc_evaluate(expr->as.pair.snd);
    default:
        return 0. / 0.; // NaN
    }
}

ucalc_expr_t *ucalc_new_value(double value) {
    return memcpy(malloc(sizeof(ucalc_expr_t)),
                  &(ucalc_expr_t){.type = EXPR_VAL, .ref = 1, .as.value = value},
                  sizeof(ucalc_expr_t));
}

ucalc_expr_t *ucalc_new_addition(ucalc_expr_t *e1, ucalc_expr_t *e2) {
    e1->ref += 1;
    e2->ref += 1;
    return memcpy(malloc(sizeof(ucalc_expr_t)),
                  &(ucalc_expr_t){.type = EXPR_ADD, .ref = 1, .as.pair.fst = e1, .as.pair.snd = e2},
                  sizeof(ucalc_expr_t));
}

ucalc_expr_t *ucalc_new_negation(ucalc_expr_t *expr) {
    expr->ref += 1;
    return memcpy(malloc(sizeof(ucalc_expr_t)),
                  &(ucalc_expr_t){.type = EXPR_NEG, .ref = 1, .as.expr = expr},
                  sizeof(ucalc_expr_t));
}

ucalc_expr_t *ucalc_new_multiplication(ucalc_expr_t *e1, ucalc_expr_t *e2) {
    e1->ref += 1;
    e2->ref += 1;
    return memcpy(malloc(sizeof(ucalc_expr_t)),
                  &(ucalc_expr_t){.type = EXPR_MUL, .ref = 1, .as.pair.fst = e1, .as.pair.snd = e2},
                  sizeof(ucalc_expr_t));
}

ucalc_expr_t *ucalc_new_inversion(ucalc_expr_t *expr) {
    expr->ref += 1;
    return memcpy(malloc(sizeof(ucalc_expr_t)),
                  &(ucalc_expr_t){.type = EXPR_INV, .ref = 1, .as.expr = expr},
                  sizeof(ucalc_expr_t));
}

void ucalc_delete(ucalc_expr_t *expr) {
    if (--expr->ref == 0) {
        switch (expr->type) {
        default:
            break;
        case EXPR_ADD:
        case EXPR_MUL:
            ucalc_delete(expr->as.pair.fst);
            ucalc_delete(expr->as.pair.snd);
            break;
        case EXPR_NEG:
        case EXPR_INV:
            ucalc_delete(expr->as.expr);
            break;
        }
        free(expr);
    }
}
