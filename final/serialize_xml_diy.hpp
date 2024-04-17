#pragma once
#include "serialize_common.hpp"
#include "tinyxml2.h"
#include <string>

class XmlArchive {
public:
    XmlArchive(std::ifstream& ifs, size_t depth) : mode(1), ifs(ifs), ofs(*(new std::ofstream)), depth(depth) {}
    XmlArchive(std::ofstream& ofs, size_t depth) : mode(0), ifs(*(new std::ifstream)), ofs(ofs), depth(depth) {}

    XmlArchive &operator<<(const std::string& ele_name) {
        // if any blank char in string, throw exception
        for (auto &c : ele_name) {
            if (std::isspace(c)) {
                throw std::runtime_error("name contains space");
            }
        }
        name = ele_name;
        name_used = false;
        return *this;
    }
    template<typename T>
    XmlArchive &operator>>(T &obj);

    template<typename T>
    XmlArchive &set(const std::string& name, T &obj);
private:
    bool mode;      // 0: write, 1: read
    std::ofstream& ofs;
    std::ifstream& ifs;
    std::string name;
    bool name_used = false;
    size_t depth;
};

std::string trim(const std::string& str) {
    size_t begin = 0, end = str.size() - 1;
    while (std::isspace(str[begin]) && begin < end) begin++;
    while (std::isspace(str[end]) && end >= begin) end--;
    return str.substr(begin, end - begin + 1);
}

template<typename T>
void serialize_xml(T obj, const std::string& name, std::ofstream& ofs, int depth) {
    ofs << std::string(depth*4, ' ') << "<" << name << ">" << std::endl;
    XmlArchive ar(ofs, depth + 1);
    obj.serialize(ar);
    ofs << std::string(depth*4, ' ') << "</" << name << ">" << std::endl;
}

template<typename T>
void deserialize_xml(T &obj, std::ifstream& ifs) {
    std::string tmp, begin, end;
    std::getline(ifs, tmp, '<');
    std::getline(ifs, begin, '>');
    XmlArchive ar(ifs, 0);
    obj.serialize(ar);
    std::getline(ifs, tmp, '<');
    std::getline(ifs, end, '>');
    if (end[0] != '/' || trim(begin) != trim(end.substr(1, end.size() - 1))) {
        throw std::runtime_error("invalid xml file");
    }
}

template<Arithemetic T>
void serialize_xml(T obj, const std::string& name, std::ofstream& ofs, int depth) {
    ofs << std::string(depth*4, ' ') << "<" << name << " val=\"" << obj << "\"/>" << std::endl;
}

template<Arithemetic T>
void deserialize_xml(T &obj, std::ifstream& ifs) {
    std::string tmp;
    std::getline(ifs, tmp, '<');
    std::getline(ifs, tmp, '>');
    if (tmp[tmp.size() - 1] != '/') {
        throw std::runtime_error("invalid xml file");
    }
    size_t val_pos = tmp.find("val");
    if (val_pos == std::string::npos) {
        throw std::runtime_error("invalid xml file");
    }
    size_t equ_pos = tmp.find("=", val_pos);
    if (equ_pos == std::string::npos) {
        throw std::runtime_error("invalid xml file");
    }
    size_t quo_pos = tmp.find("\"", equ_pos);
    size_t quo_pos2 = tmp.find("\"", quo_pos + 1);
    if (quo_pos == std::string::npos || quo_pos2 == std::string::npos) {
        throw std::runtime_error("invalid xml file");
    }
    std::string val_str = tmp.substr(quo_pos + 1, quo_pos2 - quo_pos - 1);
    if constexpr (std::is_integral_v<T>) {
        if constexpr (std::is_signed_v<T>) {
            obj = std::stoll(val_str);
        } else {
            obj = std::stoull(val_str);
        }
    } else if constexpr (std::is_floating_point_v<T>) {
        obj = std::stold(val_str);
    } else {
        throw std::runtime_error("invalid type");
    }
    // std::cout << tmp << std::endl;
}

template<typename T>
void serialize_xml(std::basic_string<T> obj, const std::string& name, std::ofstream& ofs, int depth) {
    ofs << std::string(depth*4, ' ') << "<" << name << ">" << obj << "</" << name << ">" << std::endl;
}

template<typename T>
void deserialize_xml(std::basic_string<T> &obj, std::ifstream& ifs) {
    std::string tmp;
    std::string begin, end;
    std::getline(ifs, tmp, '<');
    std::getline(ifs, begin, '>');
    std::getline(ifs, obj, '<');
    std::getline(ifs, end, '>');
    // std::cout << begin << ":" << obj << ":" << end << std::endl;
    if (end[0] != '/') {
        throw std::runtime_error("invalid xml file");
    }
    end = end.substr(1, end.size() - 1);
    if (trim(begin) != trim(end)) {
        throw std::runtime_error("invalid xml file");
    }
}

template<typename T, typename U>
void serialize_xml(std::pair<T, U> obj, const std::string& name, std::ofstream& ofs, int depth) {
    ofs << std::string(depth*4, ' ') << "<" << name << ">" << std::endl;
    serialize_xml(obj.first, "first", ofs, depth + 1);
    serialize_xml(obj.second, "second", ofs, depth + 1);
    ofs << std::string(depth*4, ' ') << "</" << name << ">" << std::endl;
}

template<typename T, typename U>
void deserialize_xml(std::pair<T, U> &obj, std::ifstream& ifs) {
    std::string tmp, begin, end;
    std::getline(ifs, tmp, '<');
    std::getline(ifs, begin, '>');
    deserialize_xml(obj.first, ifs);
    deserialize_xml(obj.second, ifs);
    std::getline(ifs, tmp, '<');
    std::getline(ifs, end, '>');
    if (end[0] != '/') {
        throw std::runtime_error("invalid xml file");
    }
    end = end.substr(1, end.size() - 1);
    if (trim(begin) != trim(end)) {
        throw std::runtime_error("invalid xml file");
    }
}

template<SeqContainer T>
void serialize_xml(T obj, const std::string& name, std::ofstream& ofs, int depth) {
    ofs << std::string(depth*4, ' ') << "<" << name << ">" << std::endl;
    for (auto &e : obj) {
        serialize_xml(e, "element", ofs, depth + 1);
    }
    ofs << std::string(depth*4, ' ') << "</" << name << ">" << std::endl;
}

template<SeqContainer T>
void deserialize_xml(T &obj, std::ifstream& ifs) {
    std::string tmp, begin, end;
    std::getline(ifs, tmp, '<');
    std::getline(ifs, begin, '>');
    while (true) {
        typename T::value_type e;
        deserialize_xml(e, ifs);
        obj.push_back(e);
        std::streampos curr = ifs.tellg();
        std::getline(ifs, tmp, '<');
        std::getline(ifs, end, '>');
        if (end[0] == '/' && trim(end.substr(1, end.size() - 1)) == trim(begin)) {
            break;
        } else if (ifs.eof()) {
            throw std::runtime_error("invalid xml file");
        } else {
            ifs.seekg(curr);
        }
    }
}

template<SetContainer T>
void serialize_xml(T obj, const std::string& name, std::ofstream& ofs, int depth) {
    ofs << std::string(depth*4, ' ') << "<" << name << ">" << std::endl;
    for (auto &e : obj) {
        serialize_xml(e, "element", ofs, depth + 1);
    }
    ofs << std::string(depth*4, ' ') << "</" << name << ">" << std::endl;
}

template<SetContainer T>
void deserialize_xml(T &obj, std::ifstream& ifs) {
    std::string tmp, begin, end;
    std::getline(ifs, tmp, '<');
    std::getline(ifs, begin, '>');
    while (true) {
        typename T::value_type e;
        deserialize_xml(e, ifs);
        obj.insert(e);
        std::streampos curr = ifs.tellg();
        std::getline(ifs, tmp, '<');
        std::getline(ifs, end, '>');
        if (end[0] == '/' && trim(end.substr(1, end.size() - 1)) == trim(begin)) {
            break;
        } else if (ifs.eof()) {
            throw std::runtime_error("invalid xml file");
        } else {
            ifs.seekg(curr);
        }
    }
}

template<MapContainer T>
void serialize_xml(T obj, const std::string& name, std::ofstream& ofs, int depth) {
    ofs << std::string(depth*4, ' ') << "<" << name << ">" << std::endl;
    for (auto &e : obj) {
        ofs << std::string((depth+1)*4, ' ') << "<element>" << std::endl;
        serialize_xml(e.first, "key", ofs, depth + 2);
        serialize_xml(e.second, "value", ofs, depth + 2);
        ofs << std::string((depth+1)*4, ' ') << "</element>" << std::endl;
    }
    ofs << std::string(depth*4, ' ') << "</" << name << ">" << std::endl;
}

template<MapContainer T>
void deserialize_xml(T &obj, std::ifstream& ifs) {
    std::string tmp, begin, end;
    std::getline(ifs, tmp, '<');
    std::getline(ifs, begin, '>');
    while (true) {
        std::pair<typename T::key_type, typename T::mapped_type> e;
        deserialize_xml(e, ifs);
        obj.insert(e);
        std::streampos curr = ifs.tellg();
        std::getline(ifs, tmp, '<');
        std::getline(ifs, end, '>');
        if (end[0] == '/' && trim(end.substr(1, end.size() - 1)) == trim(begin)) {
            break;
        } else if (ifs.eof()) {
            throw std::runtime_error("invalid xml file");
        } else {
            ifs.seekg(curr);
        }
    }
}

template<typename T>
void serialize_xml(T obj, const std::string& name, const std::string& filename) {
    std::ofstream ofs(filename);
    ofs << "<serialization>" << std::endl;
    serialize_xml(obj, name, ofs, 1);
    ofs << "</serialization>" << std::endl;
}

template<typename T>
void deserialize_xml(T &obj, const std::string& name, const std::string& filename) {
    std::ifstream ifs(filename);
    std::string tmp;
    std::getline(ifs, tmp, '<');
    std::getline(ifs, tmp, '>');
    // if (tmp != "serialization") {
    //     throw std::runtime_error("invalid file");
    // }
    deserialize_xml(obj, ifs);
    std::getline(ifs, tmp, '<');
    std::getline(ifs, tmp, '>');
    // if (tmp != "/serialization") {
    //     throw std::runtime_error("invalid file");
    // }
}


template<typename T>
XmlArchive &XmlArchive::operator>>(T &obj) {
    if (mode) {
        deserialize_xml(obj, ifs);
    } else {
        if (!name_used) {
            serialize_xml(obj, name, ofs, depth);
        } else {
            serialize_xml(obj, "item", ofs, depth);
        }
    }
    return *this;
}

template<typename T>
XmlArchive &XmlArchive::set(const std::string& name, T &obj) {
    if (mode == 0) {
        serialize_xml(obj, name, ofs, depth);
    } else {
        deserialize_xml(obj, ifs);
    }
    return *this;
}