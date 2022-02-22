#include "ucalc.h"
#include <cstring>
#include <exception>
#include <iostream>
#include <iterator>
#include <sstream>

struct result {
    int n;
    ucalc::expr_ptr e;
};

result parse(int argn, const char *const *args) {
    if (argn > 0) {
        if (std::strcmp("+", *args) == 0) {
            auto r1 = parse(argn - 1, args + 1);
            auto r2 = parse(r1.n, args + argn - r1.n);
            return {r2.n, ucalc::new_addition(r1.e, r2.e)};
        } else if (std::strcmp("-", *args) == 0) {
            auto r1 = parse(argn - 1, args + 1);
            auto r2 = parse(r1.n, args + argn - r1.n);
            auto e = ucalc::new_negation(r2.e);
            return {r2.n, ucalc::new_addition(r1.e, e)};
        } else if (std::strcmp("*", *args) == 0) {
            auto r1 = parse(argn - 1, args + 1);
            auto r2 = parse(r1.n, args + argn - r1.n);
            return {r2.n, ucalc::new_multipliation(r1.e, r2.e)};
        } else if (std::strcmp("/", *args) == 0) {
            auto r1 = parse(argn - 1, args + 1);
            auto r2 = parse(r1.n, args + argn - r1.n);
            auto e = ucalc::new_inversion(r2.e);
            return {r2.n, ucalc::new_multipliation(r1.e, e)};
        } else {
            char *end;
            auto value = std::strtod(*args, &end);
            if (std::strlen(*args) == end - *args) {
                return {argn - 1, ucalc::new_value(value)};
            } else {
                auto &&os = std::ostringstream();
                os << "cannot interpret as number: " << *args;
                throw std::invalid_argument(os.str());
            }
        }
    } else {
        throw std::invalid_argument("insufficient argments.");
    }
}

int main(int argn, const char *const *args) {
    try {
        auto result = parse(argn - 1, args + 1);
        if (result.n == 0) {
            std::cout << ucalc::evaluate(result.e) << std::endl;
            return 0;
        } else {
            std::cerr << "cannot parse input correctly."
                      << " n = " << result.n << std::endl;
            return 1;
        }
    } catch (std::exception &e) {
        std::cerr << e.what() << std::endl;
    }
}
