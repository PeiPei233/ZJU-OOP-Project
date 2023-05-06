#pragma once

#include<string>
#include<stdexcept>
#include<cmath>
#include<compare>

class Fraction {
    long long numerator;              // Numerator of the fraction
    long long denominator;            // Denominator of the fraction
    constexpr long long gcd(long long, long long) const;  // Greatest common divisor
public:
    Fraction();                 // Default constructor
    Fraction(long long, long long);         // Constructor with two integers
    Fraction(const Fraction&);  // Copy constructor
    Fraction operator+(const Fraction&) const;  // Overload operator +
    Fraction operator-(const Fraction&) const;  // Overload operator -
    Fraction operator*(const Fraction&) const;  // Overload operator *
    Fraction operator/(const Fraction&) const;  // Overload operator /
    auto operator<=>(const Fraction&) const;    // Overload operator <=> (<, <=, ==, !=, >=, >)
    operator double() const;                    // Convert fraction to double
    static Fraction fromString(const std::string&);      // Convert string to fraction
    std::string toString() const;               // Convert fraction to string

    friend std::ostream& operator<<(std::ostream&, const Fraction&);  // Inserter for stream
    friend std::istream& operator>>(std::istream&, Fraction&);        // Extractor for stream
};


constexpr long long Fraction::gcd(long long a, long long b) const {
    return b == 0 ? a : gcd(b, a % b);
}

Fraction::Fraction() : numerator(0), denominator(1) {}

Fraction::Fraction(long long n, long long d = 1) : numerator(n), denominator(d) {
    if (d == 0) throw std::invalid_argument("Denominator cannot be zero");
    if (denominator < 0) {          // Keep denominator positive
        numerator = -numerator;
        denominator = -denominator;
    }
    long long g = gcd(numerator, denominator);
    g = g < 0 ? -g : g;            // Keep greatest common divisor positive
    numerator /= g;
    denominator /= g;
}

Fraction::Fraction(const Fraction& f) : numerator(f.numerator), denominator(f.denominator) {}

Fraction Fraction::operator+(const Fraction& f) const {
    return Fraction(numerator * f.denominator + f.numerator * denominator, denominator * f.denominator);
}

Fraction Fraction::operator-(const Fraction& f) const {
    return Fraction(numerator * f.denominator - f.numerator * denominator, denominator * f.denominator);
}

Fraction Fraction::operator*(const Fraction& f) const {
    return Fraction(numerator * f.numerator, denominator * f.denominator);
}

Fraction Fraction::operator/(const Fraction& f) const {
    return Fraction(numerator * f.denominator, denominator * f.numerator);
}

auto Fraction::operator<=>(const Fraction& f) const {
    return numerator * f.denominator <=> f.numerator * denominator;
}

Fraction::operator double() const {
    return static_cast<double>(numerator) / denominator;
}

Fraction Fraction::fromString(const std::string& s) {

    // check if all the charactors are digit and return trim string
    auto checkAllDigit = [](const std::string& s) {
        if (s.empty()) {
            return s;
        }
        // ignore the blank charactor in the begin and end
        size_t begin = 0;
        while (begin < s.size() && std::isblank(s[begin])) {
            begin++;
        }
        size_t end = s.size() - 1;
        while (end > begin && std::isblank(s[end])) {
            end--;
        }
        // check if all the charactors are digit
        for (size_t i = begin; i <= end; i++) {
            if (!std::isdigit(s[i])) {
                throw std::invalid_argument("Invalid fraction");
            }
        }
        // return trim string
        return s.substr(begin, end - begin + 1);
    };

    try {
        size_t pos = s.find('/');
        if (pos == std::string::npos) {
            pos = s.find('.');
            if (pos == std::string::npos) {     // integer
                return Fraction(std::stoll(checkAllDigit(s)));
            } else {        // decimal like 0.1 or 1. or .1
                // remove 0 and blank in the end
                size_t end = s.size() - 1;
                while (end > pos && (s[end] == '0' || std::isblank(s[end]))) {
                    end--;
                }
                std::string ns = s.substr(0, end + 1);
                long long n = std::stoll(checkAllDigit(ns.substr(0, pos) + ns.substr(pos + 1)));
                long long d = 1;
                size_t expo = checkAllDigit(ns.substr(pos + 1)).size();
                if (expo > 18) {
                    throw std::out_of_range("Denominator is too large");
                }
                for (size_t i = 0; i < expo; i++) {
                    d *= 10;
                }
                return Fraction(n, d);
            }
        } else {        // fraction like 1/2 or /2 but not 1/
            long long n, d;
            try {
                std::string ns = checkAllDigit(s.substr(0, pos));
                if (ns.empty()) {
                    n = 0;
                } else {
                    n = std::stoll(ns);
                }
                d = std::stoll(checkAllDigit(s.substr(pos + 1)));
            } catch (std::invalid_argument& e) {
                throw std::invalid_argument("Invalid fraction");
            }
            return Fraction(n, d);
        }
    } catch (std::out_of_range& e) {
        throw std::out_of_range("Numerator or denominator is too large");
    }
}

std::string Fraction::toString() const {
    if (denominator == 1) {
        return std::to_string(numerator);
    }
    else {
        return std::to_string(numerator) + "/" + std::to_string(denominator);
    }
}

std::ostream& operator<<(std::ostream& os, const Fraction& f) {
    os << f.toString();
    return os;
}

std::istream& operator>>(std::istream& is, Fraction& f) {
    std::string s;
    is >> s;
    f = Fraction::fromString(s);
    return is;
}