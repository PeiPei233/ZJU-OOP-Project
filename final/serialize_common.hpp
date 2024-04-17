#pragma once
#include<iostream>
#include<fstream>
#include<vector>
#include<concepts>
#include<map>
#include<list>
#include<set>

// concept for arithmetic types
template<typename T>
concept Arithemetic = std::is_arithmetic<T>::value;

// concept for sequence containers (vector, list, etc.)
template<typename T>
concept SeqContainer = requires(T a) {
    typename T::value_type;
    typename T::iterator;
    typename T::const_iterator;
    { a.begin() };
    { a.end() };
    { a.cbegin() };
    { a.cend() };
    { a.size() };
    { a.push_back(std::declval<typename T::value_type>()) };
};

// concept for set containers (set, multiset, etc.)
template<typename T>
concept SetContainer = requires(T a) {
    typename T::value_type;
    typename T::iterator;
    typename T::const_iterator;
    { a.begin() };
    { a.end() };
    { a.cbegin() };
    { a.cend() };
    { a.size() };
    { a.insert(std::declval<const typename T::value_type>()) };
} && !requires(T a) {
    typename T::mapped_type;
};

// concept for map containers (map, multimap, etc.)
template<typename T>
concept MapContainer = requires(T a) {
    typename T::value_type;
    typename T::key_type;
    typename T::mapped_type;
    typename T::iterator;
    typename T::const_iterator;
    { a.begin() };
    { a.end() };
    { a.cbegin() };
    { a.cend() };
    { a.size() };
    { a.insert(std::declval<const typename T::value_type>()) };
};