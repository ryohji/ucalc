#include "ucalc.h"

#include <cstring>
#include <exception>
#include <functional>
#include <iostream>
#include <iterator>
#include <map>
#include <sstream>
#include <string>

struct range {
    const char *const *const begin, *const *const end;
};

struct result {
    ucalc::expr_ptr expr;
    struct range remainder;
};

result parse(struct range range);
std::string to_string(const result &result);

int main(int argn, const char *const *args) {
    try {
        const auto &&end = args + argn;
        if (args + 1 == end) {
            std::cout << "Usage: " << args[0] << " [expression]...\n";
        } else {
            auto &&result = parse({args + 1, end});
            std::cout << to_string(result) << std::endl;
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
result parse(range range) {
    auto &&begin = range.begin;
    auto &&end = range.end;
    if (begin == end) {
        throw std::invalid_argument("insufficient arguments.");
    }

    auto &&next = (struct range){begin + 1, end};
    auto &&found = bin_ops.find(*begin);
    if (found != std::cend(bin_ops)) {
        auto &&op = found->second;
        auto &&a = parse(next);
        auto &&b = parse(a.remainder);
        return {op.f(a.expr, op.m(b.expr)), b.remainder};
    } else {
        auto &&v = as_double(*begin);
        return {ucalc::value(v), next};
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

inline std::string to_string(const result &result) {
    auto &&os = std::ostringstream();
    os << ucalc::evaluate(result.expr);

    auto &&begin = result.remainder.begin;
    auto &&end = result.remainder.end;
    if (begin != end) {
        os << std::endl
           << "extra (ignored) arguments: ";
        std::copy(begin, end, std::ostream_iterator<const char *>(os, " "));
    }
    return os.str();
}
