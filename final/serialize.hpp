#pragma once
#include "serialize_common.hpp"
#include <cstring>

// class for binary archive to serialize/deserialize objects
class BinArchive {
public:
    BinArchive(std::ifstream& ifs) : mode(1), ifs(ifs), ofs(*(new std::ofstream)) {}
    BinArchive(std::ofstream& ofs) : mode(0), ifs(*(new std::ifstream)), ofs(ofs) {}

    BinArchive &operator<<(const std::string &obj) {
        // if any blank char in string, throw exception
        for (auto &c : obj) {
            if (std::isspace(c)) {
                throw std::runtime_error("name contains space");
            }
        }
        return *this;
    }
    template<typename T>
    BinArchive &operator>>(T &obj);
    
    template<typename T>
    BinArchive &set(const std::string& name, T &obj);
private:
    bool mode;      // 0: write, 1: read
    std::ofstream& ofs;
    std::ifstream& ifs;
};

// serialize/deserialize user-defined class

template<typename T>
void serialize_bin(T obj, std::ofstream& ofs) {
    BinArchive ar(ofs);
    obj.serialize(ar);
}

template<typename T>
void deserialize_bin(T &obj, std::ifstream& ifs) {
    BinArchive ar(ifs);
    obj.serialize(ar);
}

// serialize/deserialize arithmetic types

template<Arithemetic T>
void serialize_bin(T obj, std::ofstream& ofs) {
    ofs.write(reinterpret_cast<char*>(&obj), sizeof(obj));
}

template<Arithemetic T>
void deserialize_bin(T &obj, std::ifstream& ifs) {
    ifs.read(reinterpret_cast<char*>(&obj), sizeof(obj));
}

// serialize/deserialize string
// string is serialized as a sequence of characters, with a size_t in front indicating the length of the string

template<typename T>
void serialize_bin(std::basic_string<T> obj, std::ofstream& ofs) {
    size_t size = strlen(obj.c_str());
    ofs.write(reinterpret_cast<char*>(&size), sizeof(size));
    ofs.write(obj.c_str(), size);
}

template<typename T>
void deserialize_bin(std::basic_string<T> &obj, std::ifstream& ifs) {
    size_t size;
    ifs.read(reinterpret_cast<char*>(&size), sizeof(size));
    char *buf = new char[size + 1];
    ifs.read(buf, size);
    buf[size] = '\0';
    obj = buf;
    delete[] buf;
}

// serialize/deserialize pair

template<typename T, typename U>
void serialize_bin(std::pair<T, U> obj, std::ofstream& ofs) {
    serialize_bin(obj.first, ofs);
    serialize_bin(obj.second, ofs);
}

template<typename T, typename U>
void deserialize_bin(std::pair<T, U> &obj, std::ifstream& ifs) {
    deserialize_bin(obj.first, ifs);
    deserialize_bin(obj.second, ifs);
}

// serialize/deserialize sequence containers (vector, list, deque)
// sequence containers are serialized as a sequence of elements, with a size_t in front indicating the length of the sequence

template<SeqContainer T>
void serialize_bin(T obj, std::ofstream& ofs) {
    size_t size = obj.size();
    ofs.write(reinterpret_cast<char*>(&size), sizeof(size));
    for (auto &e : obj) {
        serialize_bin(e, ofs);
    }
}

template<SeqContainer T>
void deserialize_bin(T &obj, std::ifstream& ifs) {
    size_t size;
    ifs.read(reinterpret_cast<char*>(&size), sizeof(size));
    for (size_t i = 0; i < size; i++) {
        typename T::value_type e;
        deserialize_bin(e, ifs);
        obj.push_back(e);
    }
}

// serialize/deserialize set containers (set, multiset)
// set containers are serialized as a sequence of elements, with a size_t in front indicating the length of the sequence

template<SetContainer T>
void serialize_bin(T obj, std::ofstream& ofs) {
    size_t size = obj.size();
    ofs.write(reinterpret_cast<char*>(&size), sizeof(size));
    for (auto &e : obj) {
        serialize_bin(e, ofs);
    }
}

template<SetContainer T>
void deserialize_bin(T &obj, std::ifstream& ifs) {
    size_t size;
    ifs.read(reinterpret_cast<char*>(&size), sizeof(size));
    for (size_t i = 0; i < size; i++) {
        typename T::value_type e;
        deserialize_bin(e, ifs);
        obj.insert(e);
    }
}

// serialize/deserialize map containers (map, multimap)
// map containers are serialized as a sequence of key-value pairs, with a size_t in front indicating the length of the sequence

template<MapContainer T>
void serialize_bin(T obj, std::ofstream& ofs) {
    size_t size = obj.size();
    ofs.write(reinterpret_cast<char*>(&size), sizeof(size));
    for (auto &e : obj) {
        serialize_bin(e, ofs);
    }
}

template<MapContainer T>
void deserialize_bin(T &obj, std::ifstream& ifs) {
    size_t size;
    ifs.read(reinterpret_cast<char*>(&size), sizeof(size));
    for (size_t i = 0; i < size; i++) {
        typename T::key_type key;
        typename T::mapped_type value;
        deserialize_bin(key, ifs);
        deserialize_bin(value, ifs);
        obj.insert(std::make_pair(key, value));
    }
}

// serialize/deserialize function for user to use

template<typename T>
void serialize(T obj, const std::string& filename) {
    std::ofstream ofs(filename, std::ios::binary);
    serialize_bin(obj, ofs);
    ofs.close();
}

template<typename T>
void deserialize(T &obj, const std::string& filename) {
    std::ifstream ifs(filename, std::ios::binary);
    deserialize_bin(obj, ifs);
    ifs.close();
}

template<typename T>
BinArchive &BinArchive::operator>>(T &obj) {
    if (mode) {
        deserialize_bin(obj, ifs);
    } else {
        serialize_bin(obj, ofs);
    }
    return *this;
}

template<typename T>
BinArchive &BinArchive::set(const std::string& name, T &obj) {
    for (auto &c : name) {
        if (std::isspace(c)) {
            throw std::runtime_error("name contains space");
        }
    }
    if (mode) {
        deserialize_bin(obj, ifs);
    } else {
        serialize_bin(obj, ofs);
    }
    return *this;
}