#pragma once

#ifndef EXTERN_C
#ifdef __cplusplus
#define EXTERN_C extern "C"
#else
#define EXTERN_C extern
#endif
#endif

typedef struct ucalc_expr ucalc_expr_t;

EXTERN_C ucalc_expr_t *ucalc_new_value(double value);

EXTERN_C double ucalc_evaluate(ucalc_expr_t *expr);

EXTERN_C ucalc_expr_t *ucalc_new_addition(ucalc_expr_t *e1, ucalc_expr_t *e2);

EXTERN_C ucalc_expr_t *ucalc_new_negation(ucalc_expr_t *expr);

EXTERN_C ucalc_expr_t *ucalc_new_multiplication(ucalc_expr_t *e1, ucalc_expr_t *e2);

EXTERN_C ucalc_expr_t *ucalc_new_inversion(ucalc_expr_t *expr);

EXTERN_C void ucalc_delete(ucalc_expr_t *expr);

#ifdef __cplusplus
#include <memory>

namespace ucalc {

typedef std::shared_ptr<::ucalc_expr_t> expr_ptr;

inline expr_ptr new_expr(::ucalc_expr_t *expr) {
    return expr_ptr(expr, ::ucalc_delete);
}

inline expr_ptr new_value(double value) {
    return new_expr(::ucalc_new_value(value));
}

inline double evaluate(expr_ptr expr) {
    return ::ucalc_evaluate(expr.get());
}

inline expr_ptr new_negation(expr_ptr expr) {
    return new_expr(::ucalc_new_negation(expr.get()));
}

inline expr_ptr new_inversion(expr_ptr expr) {
    return new_expr(::ucalc_new_inversion(expr.get()));
}

inline expr_ptr new_addition(expr_ptr e1, expr_ptr e2) {
    return new_expr(::ucalc_new_addition(e1.get(), e2.get()));
}

inline expr_ptr new_multipliation(expr_ptr e1, expr_ptr e2) {
    return new_expr(::ucalc_new_multiplication(e1.get(), e2.get()));
}

} // namespace ucalc
#endif
