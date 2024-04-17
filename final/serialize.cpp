/**
 * use `clang++ serialize.cpp tinyxml2.cpp -o serialize -std=c++20` to compile
*/
#include <iostream>

#include "serialize.hpp"
#include "serialize_xml.hpp"

/**
 * 
 * User defined class needs to implement serialize() public function
 * 
 * in the function, use ar.set() to serialize data members
 * or use ar << >> to serialize data members like below
 * 
*/

class A {
private:
    bool idx;
    std::string name;
    std::vector<std::pair<double, long double>> data;

public:
    template<typename Archive>
    void serialize(Archive &ar) {
        ar.set("idx", idx)
          .set("name", name)
          .set("data", data);
    }

    // template<typename Archive>
    // void serialize(Archive &ar) {
    //     ar << "idx" >> idx
    //        << "name" >> name
    //        << "data" >> data;
    // }

    A () {}
    A (int idx, std::string name, std::vector<std::pair<double, long double>> data) : idx(idx), name(name), data(data) {}
    int get_idx() {return idx;}
    std::string get_name() {return name;}
    std::vector<std::pair<double, long double>> get_data() {return data;}
};

int main() {
    std::string filename = "test.xml";

    std::map<std::string, A> ele = {
        {"123", {true, "  A\n\t1  ", {{1.1, 2.1}, {3.1, 3.2}}}}, 
        {"abd", {false, "B123", {{23, 45}}}}, 
        {"hgf", {true, "edd", {{1, 33}, {110, 123}}}}
    };
    std::cout << "original: " << std::endl;
    for (auto &obj: ele) {
        std::cout << obj.first << ":";
        std::cout << obj.second.get_idx() << ":" << obj.second.get_name() << ":";
        for (auto &v: obj.second.get_data()) {
            std::cout << v.first << ":" << v.second << ":";
        }
        std::cout << std::endl;
    }

    serialize_xml(ele, "test", filename);
    ele.clear();
    deserialize_xml(ele, "test", filename);
    std::cout << "\nserialize and deserialize from xml: " << std::endl;
    for (auto &obj: ele) {
        std::cout << obj.first << ":";
        std::cout << obj.second.get_idx() << ":" << obj.second.get_name() << ":";
        for (auto &v: obj.second.get_data()) {
            std::cout << v.first << ":" << v.second << ":";
        }
        std::cout << std::endl;
    }

    serialize(ele, filename);
    ele.clear();
    deserialize(ele, filename);
    std::cout << "\nserialize and deserialize from binary: " << std::endl;
    for (auto &obj: ele) {
        std::cout << obj.first << ":";
        std::cout << obj.second.get_idx() << ":" << obj.second.get_name() << ":";
        for (auto &v: obj.second.get_data()) {
            std::cout << v.first << ":" << v.second << ":";
        }
        std::cout << std::endl;
    }

    return 0;
}