#pragma once
#include "serialize_common.hpp"
#include "tinyxml2.h"
#include <string>

// class for xml archive to serialize/deserialize objects
class XmlArchive {
public:
    XmlArchive(tinyxml2::XMLElement *p, tinyxml2::XMLDocument& doc, bool mode) : p(p), doc(doc), mode(mode) {}

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
    tinyxml2::XMLDocument& doc;
    tinyxml2::XMLElement *p;
    std::string name;
    bool name_used = true;
};

// serialize/deserialize user-defined class

template<typename T>
void serialize_xml(T obj, const std::string& name, tinyxml2::XMLElement *p, tinyxml2::XMLDocument& doc) {
    tinyxml2::XMLElement *ele = doc.NewElement(name.c_str());
    XmlArchive ar(ele, doc, 0);
    obj.serialize(ar);
    p->InsertEndChild(ele);
}

template<typename T>
void deserialize_xml(T &obj, const std::string& name, tinyxml2::XMLElement *p, bool findchild = true) {
    tinyxml2::XMLElement *ele = p;
    // if findchild is true, find the first child element with name
    if (findchild) {
        ele = p->FirstChildElement(name.c_str());
    }
    if (ele == nullptr || name != ele->Name()) {
        throw std::runtime_error(std::string("no such element: ") + name);
    }
    XmlArchive ar(ele, *ele->GetDocument(), 1);
    obj.serialize(ar);
}

// serialize/deserialize arithmetic types

template<Arithemetic T>
void serialize_xml(T obj, const std::string& name, tinyxml2::XMLElement *p, tinyxml2::XMLDocument& doc) {
    tinyxml2::XMLElement *ele = doc.NewElement(name.c_str());
    if constexpr (std::is_same_v<T, bool>) {
        ele->SetAttribute("val", obj);
    } else {
        // convert to string first
        ele->SetAttribute("val", std::to_string(obj).c_str());
    }
    p->InsertEndChild(ele);
}

template<Arithemetic T>
void deserialize_xml(T &obj, const std::string& name, tinyxml2::XMLElement *p, bool findchild = true) {
    tinyxml2::XMLElement *ele = p;
    if (findchild) {
        ele = p->FirstChildElement(name.c_str());
    }
    if (ele == nullptr || name != ele->Name()) {
        throw std::runtime_error(std::string("no such element: ") + name);
    }
    if (!ele->Attribute("val")) {
        throw std::runtime_error("no val attribute");
    }
    // get string value first, then convert to arithmetic type
    std::string val_str = ele->Attribute("val");
    if constexpr (std::is_same_v<T, bool>) {
        if (val_str == "true") {
            obj = true;
        } else if (val_str == "false") {
            obj = false;
        } else {
            throw std::runtime_error("invalid bool value");
        }
    } else if constexpr (std::is_integral_v<T>) {
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
}

// serialize/deserialize string

template<typename T>
void serialize_xml(std::basic_string<T> obj, const std::string& name, tinyxml2::XMLElement *p, tinyxml2::XMLDocument& doc) {
    tinyxml2::XMLElement *ele = doc.NewElement(name.c_str());
    ele->SetText(obj.c_str());
    p->InsertEndChild(ele);
}

template<typename T>
void deserialize_xml(std::basic_string<T> &obj, const std::string& name, tinyxml2::XMLElement *p, bool findchild = true) {
    tinyxml2::XMLElement *ele = p;
    if (findchild) {
        ele = p->FirstChildElement(name.c_str());
    }
    if (ele == nullptr || name != ele->Name()) {
        throw std::runtime_error(std::string("no such element: ") + name);
    }
    if (!ele->GetText()) {
        throw std::runtime_error("no text");
    }
    obj = ele->GetText();
}

// serialize/deserialize pair

template<typename T, typename U>
void serialize_xml(std::pair<T, U> obj, const std::string& name, tinyxml2::XMLElement *p, tinyxml2::XMLDocument& doc) {
    tinyxml2::XMLElement *ele = doc.NewElement(name.c_str());
    serialize_xml(obj.first, "first", ele, doc);
    serialize_xml(obj.second, "second", ele, doc);
    p->InsertEndChild(ele);
}

template<typename T, typename U>
void deserialize_xml(std::pair<T, U> &obj, const std::string& name, tinyxml2::XMLElement *p, bool findchild = true) {
    tinyxml2::XMLElement *ele = p;
    if (findchild) {
        ele = p->FirstChildElement(name.c_str());
    }
    if (ele == nullptr || name != ele->Name()) {
        throw std::runtime_error(std::string("no such element: ") + name);
    }
    deserialize_xml(obj.first, "first", ele);
    deserialize_xml(obj.second, "second", ele);
}

// serialize/deserialize sequence container (vector, list, etc.)

template<SeqContainer T>
void serialize_xml(T obj, const std::string& name, tinyxml2::XMLElement *p, tinyxml2::XMLDocument& doc) {
    tinyxml2::XMLElement *ele = doc.NewElement(name.c_str());
    for (auto &item : obj) {
        serialize_xml(item, "item", ele, doc);
    }
    p->InsertEndChild(ele);
}

template<SeqContainer T>
void deserialize_xml(T &obj, const std::string& name, tinyxml2::XMLElement *p, bool findchild = true) {
    tinyxml2::XMLElement *ele = p;
    if (findchild) {
        ele = p->FirstChildElement(name.c_str());
    }
    if (ele == nullptr || name != ele->Name()) {
        throw std::runtime_error(std::string("no such element: ") + name);
    }
    tinyxml2::XMLElement *item = ele->FirstChildElement("item");
    // get next item until nullptr to get all items
    while (item != nullptr) {
        typename T::value_type val;
        // no need to find child element, since we already get the item element
        deserialize_xml(val, "item", item, false);
        obj.push_back(val);
        item = item->NextSiblingElement("item");
    }
}

// serialize/deserialize set container (set, unordered_set, etc.)

template<SetContainer T>
void serialize_xml(T obj, const std::string& name, tinyxml2::XMLElement *p, tinyxml2::XMLDocument& doc) {
    tinyxml2::XMLElement *ele = doc.NewElement(name.c_str());
    for (auto &item : obj) {
        serialize_xml(item, "item", ele, doc);
    }
    p->InsertEndChild(ele);
}

template<SetContainer T>
void deserialize_xml(T &obj, const std::string& name, tinyxml2::XMLElement *p, bool findchild = true) {
    tinyxml2::XMLElement *ele = p;
    if (findchild) {
        ele = p->FirstChildElement(name.c_str());
    }
    if (ele == nullptr || name != ele->Name()) {
        throw std::runtime_error(std::string("no such element: ") + name);
    }
    tinyxml2::XMLElement *item = ele->FirstChildElement("item");
    while (item != nullptr) {
        typename T::value_type val;
        deserialize_xml(val, "item", item, false);
        obj.insert(val);
        item = item->NextSiblingElement("item");
    }
}

// serialize/deserialize map container (map, unordered_map, etc.)

template<MapContainer T>
void serialize_xml(T obj, const std::string& name, tinyxml2::XMLElement *p, tinyxml2::XMLDocument& doc) {
    tinyxml2::XMLElement *ele = doc.NewElement(name.c_str());
    for (auto &item : obj) {
        tinyxml2::XMLElement *item_ele = doc.NewElement("item");
        serialize_xml(item.first, "key", item_ele, doc);
        serialize_xml(item.second, "value", item_ele, doc);
        ele->InsertEndChild(item_ele);
    }
    p->InsertEndChild(ele);
}

template<MapContainer T>
void deserialize_xml(T &obj, const std::string& name, tinyxml2::XMLElement *p, bool findchild = true) {
    tinyxml2::XMLElement *ele = p;
    if (findchild) {
        ele = p->FirstChildElement(name.c_str());
    }
    if (ele == nullptr || name != ele->Name()) {
        throw std::runtime_error(std::string("no such element: ") + name);
    }
    tinyxml2::XMLElement *item = ele->FirstChildElement("item");
    while (item != nullptr) {
        typename T::key_type key;
        typename T::mapped_type val;
        deserialize_xml(key, "key", item);
        deserialize_xml(val, "value", item);
        obj.insert(std::make_pair(key, val));
        item = item->NextSiblingElement("item");
    }
}

// serialize/deserialize function for user to call

template<typename T>
void serialize_xml(T obj, const std::string& name, const std::string& filename) {
    tinyxml2::XMLDocument doc;
    tinyxml2::XMLElement *root = doc.NewElement("serialization");
    try {
        serialize_xml(obj, name, root, doc);
        doc.InsertFirstChild(root);
    } catch (std::exception &e) {
        throw std::runtime_error(std::string("serialize failed") + e.what());
    }
    if (doc.SaveFile(filename.c_str()) != tinyxml2::XML_SUCCESS) {
        throw std::runtime_error("save file failed");
    }    
}

template<typename T>
void deserialize_xml(T &obj, const std::string& name, const std::string& filename) {
    tinyxml2::XMLDocument doc;
    tinyxml2::XMLError err = doc.LoadFile(filename.c_str());
    if (err != tinyxml2::XML_SUCCESS) {
        throw std::runtime_error("load file failed");
    }
    try {
        tinyxml2::XMLElement *root = doc.FirstChildElement("serialization");
        deserialize_xml(obj, name, root);
    } catch (std::exception &e) {
        throw std::runtime_error(std::string("deserialize failed") + e.what());
    }
}


template<typename T>
XmlArchive &XmlArchive::operator>>(T &obj) {
    if (mode) {
        if (!name_used) {
            deserialize_xml(obj, name, p);
            name_used = true;
        } else {
            deserialize_xml(obj, "item", p);
        }
    } else {
        if (!name_used) {
            serialize_xml(obj, name, p, doc);
            name_used = true;
        } else {
            serialize_xml(obj, "item", p, doc);
        }
    }
    return *this;
}

template<typename T>
XmlArchive &XmlArchive::set(const std::string& name, T &obj) {
    for (auto &c : name) {
        if (std::isspace(c)) {
            throw std::runtime_error("name contains space");
        }
    }
    if (mode) {
        deserialize_xml(obj, name, p);
    } else {
        serialize_xml(obj, name, p, doc);
    }
    return *this;
}