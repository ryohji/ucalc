#include "ucalc.h"

#include <cstring>
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
std::string to_string(const result &result, const char *const *const end);

int main(int argn, const char *const *args) {
    try {
        const auto &&end = args + argn;
        if (args + 1 == end) {
            std::cout << "Usage: " << args[0] << " [expression]...\n";
        } else {
            auto &&result = parse(args + 1, end);
            std::cout << to_string(result, end) << std::endl;
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

const auto &&bin_ops = std::map<const std::string, const bin_op>{
    {"+", {ucalc::add, [](auto &&e) { return std::move(e); }}},
    {"-", {ucalc::add, ucalc::negate}},
    {"*", {ucalc::multiply, [](auto &&e) { return std::move(e); }}},
    {"/", {ucalc::multiply, ucalc::invert}},
};

double as_double(const char *token) throw(std::invalid_argument);
result parse(const char *const *const begin, const char *const *const end) {
    if (begin == end) {
        throw std::invalid_argument("insufficient arguments.");
    }

    auto &&found = bin_ops.find(*begin);
    if (found != std::cend(bin_ops)) {
        auto &&op = found->second;
        auto &&a = parse(begin + 1, end);
        auto &&b = parse(a.next, end);
        return {op.f(a.expr, op.m(b.expr)), b.next};
    } else {
        auto &&v = as_double(*begin);
        return {ucalc::value(v), begin + 1};
    }
}

inline double as_double(const char *token) throw(std::invalid_argument) {
    const char *end;
    auto &&value = std::strtod(token, const_cast<char **>(&end));
    if (std::strlen(token) - std::distance(token, end) != 0) {
        auto &&os = std::ostringstream();
        os << "cannot interpret as a number: " << token;
        throw std::invalid_argument(os.str());
    }
    return value;
}

inline std::string to_string(const result &result, const char *const *const end) {
    auto &&os = std::ostringstream();
    os << ucalc::evaluate(result.expr);
    if (result.next != end) {
        os << std::endl
           << "extra (ignored) arguments: ";
        std::copy(result.next, end, std::ostream_iterator<const char *>(os, " "));
    }
    return os.str();
}
