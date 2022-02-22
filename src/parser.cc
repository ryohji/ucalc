#include "ucalc.h"

#include <exception>
#include <functional>
#include <iostream>
#include <iterator>
#include <map>
#include <sstream>
#include <string>

struct result {
    ucalc::expr_ptr expr;
    const char *const *next;
};

result parse(const char *const *const begin, const char *const *const end);

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

struct bin_op {
    std::function<ucalc::expr_ptr(ucalc::expr_ptr, ucalc::expr_ptr)> f; // function
    std::function<ucalc::expr_ptr(ucalc::expr_ptr)> m;                  // modifier
};

auto &&bin_ops = std::map<std::string, bin_op>{
    {"+", {ucalc::add, [](auto &&e) { return e; }}},
    {"-", {ucalc::add, ucalc::negate}},
    {"*", {ucalc::multiply, [](auto &&e) { return e; }}},
    {"/", {ucalc::multiply, ucalc::invert}},
};

result parse(const char *const *const begin, const char *const *const end) {
    if (begin != end) {
        auto &&text = *begin;
        auto &&found = bin_ops.find(text);
        if (found != std::cend(bin_ops)) {
            auto &&op = found->second;
            auto &&a = parse(begin + 1, end);
            auto &&b = parse(a.next, end);
            return {op.f(a.expr, op.m(b.expr)), b.next};
        } else {
            char *it;
            auto &&value = std::strtod(text, &it);
            if (std::strlen(text) == std::distance(text, static_cast<const char *>(it))) {
                return {ucalc::value(value), begin + 1};
            } else {
                auto &&os = std::ostringstream();
                os << "cannot interpret as a number: " << text;
                throw std::invalid_argument(os.str());
            }
        }
    } else {
        throw std::invalid_argument("insufficient argments.");
    }
}
