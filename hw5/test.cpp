/**
 * Use -std=c++20 to compile this file !!
*/

#include<iostream>

#include "fraction.hpp"

int main() {

    Fraction f1, f2;
    // Test Inserter for streams with fromString function inside
    std::cin >> f1 >> f2;

    // Test Typecast to double and Extractor for streams with toString function inside
    std::cout << "f1 = " << f1 << " = " << static_cast<double>(f1) << std::endl;
    std::cout << "f2 = " << f2 << " = " << static_cast<double>(f2) << std::endl;

    // Test Arithmetical operators: + - * /
    std::cout << "f1 + f2 = " << f1 << " + " << f2 << " = " << f1 + f2 << std::endl;
    std::cout << "f1 - f2 = " << f1 << " - " << f2 << " = " << f1 - f2 << std::endl;
    std::cout << "f1 * f2 = " << f1 << " * " << f2 << " = " << f1 * f2 << std::endl;
    std::cout << "f1 / f2 = " << f1 << " / " << f2 << " = " << f1 / f2 << std::endl;

    // Test Relational operators: < <= == != >= >
    std::cout << "f1 < f2 : " << f1 << " < " << f2 << " : " << (f1 < f2 ? "True" : "False") << std::endl;
    std::cout << "f1 <= f2 : " << f1 << " <= " << f2 << " : " << (f1 <= f2 ? "True" : "False") << std::endl;
    std::cout << "f1 == f2 : " << f1 << " == " << f2 << " : " << (f1 == f2 ? "True" : "False") << std::endl;
    std::cout << "f1 != f2 : " << f1 << " != " << f2 << " : " << (f1 != f2 ? "True" : "False") << std::endl;
    std::cout << "f1 >= f2 : " << f1 << " >= " << f2 << " : " << (f1 >= f2 ? "True" : "False") << std::endl;
    std::cout << "f1 > f2 : " << f1 << " > " << f2 << " : " << (f1 > f2 ? "True" : "False") << std::endl;
    
    // Test fromString function with exception handling
    std::cout << "Fraction::fromString(\"1/2\"): " << Fraction::fromString("1/2") << std::endl;
    std::cout << "Fraction::fromString(\"  3 /  4 \"): " << Fraction::fromString("  3 /  4 ") << std::endl;
    std::cout << "Fraction::fromString(\"1.2\"): " << Fraction::fromString("1.2") << std::endl;
    std::cout << "Fraction::fromString(\"1\"): " << Fraction::fromString("1") << std::endl;
    std::cout << "Fraction::fromString(\"1.2  \"): " << Fraction::fromString("1.2  ") << std::endl;
    std::cout << "Fraction::fromString(\" /6  \"): " << Fraction::fromString(" /6  ") << std::endl;
    std::cout << "Fraction::fromString(\"2.0000000000000000000000\"): " << Fraction::fromString("2.0000000000000000000000") << std::endl;
    try {
        std::cout << "Fraction::fromString(\"1. 2\"): ";
        Fraction::fromString("1. 2");
    } catch (std::invalid_argument& e) {
        std::cout << "std::invalid_argument::what(): " << e.what() << std::endl;
    }
    try {
        std::cout << "Fraction::fromString(\"1/2/3\"): ";
        Fraction::fromString("1/2/3");
    } catch (std::invalid_argument& e) {
        std::cout << "std::invalid_argument::what(): " << e.what() << std::endl;
    }
    try {
        std::cout << "Fraction::fromString(\"1.2 3\"): ";
        Fraction::fromString("1.2 3");
    } catch (std::invalid_argument& e) {
        std::cout << "std::invalid_argument::what(): " << e.what() << std::endl;
    }    
    try {
        std::cout << "Fraction::fromString(\"6/\"): ";
        Fraction::fromString("6/");
    } catch (std::invalid_argument& e) {
        std::cout << "std::invalid_argument::what(): " << e.what() << std::endl;
    }
    try {
        std::cout << "Fraction::fromString(\"1/0\"): ";
        Fraction::fromString("1/0");
    } catch (std::invalid_argument& e) {
        std::cout << "std::invalid_argument::what(): " << e.what() << std::endl;
    }
    try {
        std::cout << "Fraction::fromString(\"2.0000000000000000000001\"): ";
        Fraction::fromString("2.0000000000000000000001");
    } catch (std::out_of_range& e) {
        std::cout << "std::out_of_range::what(): " << e.what() << std::endl;
    }
    try {
        std::cout << "Fraction::fromString(\"0.0000000000000000000002\"): ";
        Fraction::fromString("0.0000000000000000000002");
    } catch (std::out_of_range& e) {
        std::cout << "std::out_of_range::what(): " << e.what() << std::endl;
    }

    return 0;
}