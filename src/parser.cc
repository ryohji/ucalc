#include "ucalc.h"

#include <cstring>
#include <exception>
#include <iostream>
#include <iterator>
#include <sstream>

inline bool eq(const char *str1, const char *str2) {
    return std::strcmp(str1, str2) == 0;
}

struct result {
    ucalc::expr_ptr expr;
    const char *const *next;
};

result parse(const char *const *const begin, const char *const *const end) {
    if (begin != end) {
        if (eq("+", *begin)) {
            auto a = parse(begin + 1, end);
            auto b = parse(a.next, end);
            return {ucalc::new_addition(a.expr, b.expr), b.next};
        } else if (eq("-", *begin)) {
            auto a = parse(begin + 1, end);
            auto b = parse(a.next, end);
            auto expr = ucalc::new_negation(b.expr);
            return {ucalc::new_addition(a.expr, expr), b.next};
        } else if (eq("*", *begin)) {
            auto a = parse(begin + 1, end);
            auto b = parse(a.next, end);
            return {ucalc::new_multipliation(a.expr, b.expr), b.next};
        } else if (eq("/", *begin)) {
            auto a = parse(begin + 1, end);
            auto b = parse(a.next, end);
            auto expr = ucalc::new_inversion(b.expr);
            return {ucalc::new_multipliation(a.expr, expr), b.next};
        } else {
            char *it;
            auto &&text = *begin;
            auto value = std::strtod(text, &it);
            if (std::strlen(text) == std::distance(text, static_cast<const char *>(it))) {
                return {ucalc::new_value(value), begin + 1};
            } else {
                auto &&os = std::ostringstream();
                os << "cannot interpret as number: " << text;
                throw std::invalid_argument(os.str());
            }
        }
    } else {
        throw std::invalid_argument("insufficient argments.");
    }
}

int main(int argn, const char *const *args) {
    try {
        auto &&begin = args + 1;
        auto &&end = args + argn;
        if (begin == end) {
            std::cout << "Usage: " << *args << " [expression]...\n";
        } else {
            auto result = parse(begin, end);
            std::cout << ucalc::evaluate(result.expr) << std::endl;
            if (result.next != end) {
                auto &&os = std::cerr;
                os << "extra (ignored) arguments:";
                for (auto it = result.next; it != end; ++it) {
                    os << " " << *it;
                }
                os << std::endl;
            }
        }
        return 0;
    } catch (std::exception &e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }
}
