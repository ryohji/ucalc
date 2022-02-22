#include "ucalc.h"
#include <stdlib.h>
#include <string.h>

struct ucalc_expr {
    double value;
};

ucalc_expr_t *ucalc_new_value(double value) {
    ucalc_expr_t *const p = malloc(sizeof(ucalc_expr_t));
    memcpy(p, &(ucalc_expr_t){.value = value}, sizeof(*p));
    return p;
}

double ucalc_evaluate(ucalc_expr_t *expr) {
    return expr->value;
}

ucalc_expr_t *ucalc_new_addition(ucalc_expr_t *e1, ucalc_expr_t *e2) {
    return ucalc_new_value(ucalc_evaluate(e1) + ucalc_evaluate(e2));
}

ucalc_expr_t *ucalc_new_negation(ucalc_expr_t *expr) {
    return ucalc_new_value(-ucalc_evaluate(expr));
}

ucalc_expr_t *ucalc_new_multiplication(ucalc_expr_t *e1, ucalc_expr_t *e2) {
    return ucalc_new_value(ucalc_evaluate(e1) * ucalc_evaluate(e2));
}

ucalc_expr_t *ucalc_new_inversion(ucalc_expr_t *expr) {
    return ucalc_new_value(1 / ucalc_evaluate(expr));
}

void ucalc_delete(ucalc_expr_t *expr) {
    free(expr);
}
