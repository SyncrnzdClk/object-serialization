#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <list>
#include <set>
#include <map>
#include <type_traits>
#include <memory>
#include <cassert>
#include <sstream>
#include "tinyxml2.h"


#define ASSERT(expr, message) assert((expr) && (message))


using namespace tinyxml2;

namespace XMLSerialization{

// serialize for arithmetic
template<typename T>
typename std::enable_if<std::is_arithmetic<T>::value, void>::type
serialize_xml(const T& value, const std::string& name, const std::string& filename) {
    XMLDocument doc;
    XMLElement* serialization = nullptr;

    if (doc.LoadFile(filename.c_str()) != XML_SUCCESS) { // if the file does not exist, we need to insert the root element <serialization><\serialization>
        serialization = doc.NewElement("serialization");
        doc.InsertFirstChild(serialization);
    }
    else {
        serialization = doc.FirstChildElement("serialization"); // try to find the root element <serialization><\serialization>
        if (!serialization) { // insert the root element
            serialization = doc.NewElement("serialization");
            doc.InsertFirstChild(serialization);
        }
    }
    
    // create the arithmetic element
    XMLElement* arithmetic = doc.NewElement(name.c_str());
    arithmetic->SetAttribute("val", value);
    serialization->InsertEndChild(arithmetic);

    // save the contents in to the file
    doc.SaveFile(filename.c_str());
}

// deserialize for arithmetic
template<typename T>
typename std::enable_if<std::is_arithmetic<T>::value, void>::type
deserialize_xml (T& value, const std::string& name, const std::string& filename) {
    XMLDocument doc;

    // if the file does not exist, return
    if (doc.LoadFile(filename.c_str()) != XML_SUCCESS) {
        throw std::runtime_error("file open error");
    }

    XMLElement* serialization = doc.FirstChildElement("serialization");
    if (!serialization) {
        throw std::runtime_error("fail to find serialization element.");
    } 

    // find the first matched element
    XMLElement* arithmetic = serialization->FirstChildElement(name.c_str());
    const char* val = arithmetic->Attribute("val");
    if (!val) {
        throw std::runtime_error("get value error");
    }

    std::stringstream ss(val);
    ss >> value;
    if (ss.fail()) {
        throw std::runtime_error("parsing attribute value error.");
    }
}

// serialize for string
void serialize_xml(const std::string& value, const std::string& name, const std::string& filename) {
    XMLDocument doc;
    XMLElement* serialization;
    if (doc.LoadFile(filename.c_str()) != XML_SUCCESS) { // if the file does not exist, we need to insert the root element <serialization><\serialization>
        serialization = doc.NewElement("serialization");
        doc.InsertFirstChild(serialization);
    }
    else {
        serialization = doc.FirstChildElement("serialization"); // try to find the root element <serialization><\serialization>
        if (!serialization) { // insert the root element
            serialization = doc.NewElement("serialization");
            doc.InsertFirstChild(serialization);
        }
    }

    // create the new element
    XMLElement* String = doc.NewElement(name.c_str());
    String->SetAttribute("val", value.c_str());
    serialization->InsertEndChild(String);

    // save the contents in to the file
    doc.SaveFile(filename.c_str());
}

// deserialize for string
void deserialize_xml (std::string& value, const std::string& name, const std::string& filename) {
    XMLDocument doc;

    // if the file does not exist, return
    if (doc.LoadFile(filename.c_str()) != XML_SUCCESS) {
        throw std::runtime_error("file open error");
    }

    XMLElement* serialization = doc.FirstChildElement("serialization");
    if (!serialization) {
        throw std::runtime_error("fail to find serialization element.");
    } 

    // find the first matched element
    XMLElement* String = serialization->FirstChildElement(name.c_str());
    value = String->Attribute("val");
}

// serialize for vector
template<typename T>
void serialize_xml(const std::vector<T>& vec, const std::string& name, const std::string& filename) {
    XMLDocument doc;
    XMLElement* serialization;
    if (doc.LoadFile(filename.c_str()) != XML_SUCCESS) { // if the file does not exist
        serialization = doc.NewElement("serialization");
        doc.InsertFirstChild(serialization);
    }
    else {
        serialization = doc.FirstChildElement("serialization"); // try to find the root element <serialization><\serialization>
        if (!serialization) { // insert the root element
            serialization = doc.NewElement("serialization");
            doc.InsertFirstChild(serialization);
        }
    }

    // create a new element for vector
    XMLElement* std_vector = doc.NewElement(name.c_str());

    // create elements for the vector's contents
    for (const auto& element : vec) {
        XMLElement* xml_element = doc.NewElement("element");
        if constexpr (std::is_arithmetic_v<T>) xml_element->SetAttribute("val", element);
        else if constexpr (std::is_same_v<T, std::string>) xml_element->SetAttribute("val", element.c_str());
        else throw std::runtime_error("this type cannot be serialized");
        std_vector->InsertEndChild(xml_element);
    }

    serialization->InsertFirstChild(std_vector);

    doc.SaveFile(filename.c_str());
}


// deserialize for vector
template<typename T>
void deserialize_xml(std::vector<T>& vec, const std::string& name, const std::string& filename) {
    XMLDocument doc;
    if (doc.LoadFile(filename.c_str()) != XML_SUCCESS) {
        throw std::runtime_error("file open error");
    }

    XMLElement* serialization = doc.FirstChildElement("serialization");

    XMLElement* std_vec;
    std_vec = serialization->FirstChildElement(name.c_str());
    if (!std_vec) {
        throw std::runtime_error("fail to find the serialization element");
    }

    // clear the vector
    vec.resize(0);
    
    T value;
    for (XMLElement* xml_element = std_vec->FirstChildElement("element"); xml_element; xml_element = xml_element->NextSiblingElement()) {
        const char* val = xml_element->Attribute("val");
        if (!val) {
            throw std::runtime_error("get value error");
        }
        if constexpr (std::is_arithmetic_v<T>) {
            std::stringstream ss(val);
            ss >> value;
            if (ss.fail()) {
                throw std::runtime_error("parsing attribute value error");
            }
        }
        else if constexpr (std::is_same_v<T, std::string>) {
            value = val;
        }
        vec.push_back(value);
    }
}

template<typename K, typename V>
void serialize_xml(const std::pair<K, V>& pair, const std::string& name, const std::string& filename) {
    XMLDocument doc;
    XMLElement* serialization;
    if (doc.LoadFile(filename.c_str()) != XML_SUCCESS) {
        serialization = doc.NewElement("serialization");
        doc.InsertFirstChild(serialization);
    }
    else {
        serialization = doc.FirstChildElement("serialization");
        if (!serialization) {
            serialization = doc.NewElement("serialization");
            doc.InsertFirstChild(serialization);
        }
    }
    
    XMLElement* std_pair;
    std_pair = doc.NewElement(name.c_str());

    XMLElement* first = doc.NewElement("first");
    if constexpr (std::is_arithmetic_v<K>) first->SetAttribute("val", pair.first);
    else if constexpr (std::is_same_v<K, std::string>) first->SetAttribute("val", pair.first.c_str());
    else throw std::runtime_error("unexpected error");
    
    XMLElement* second = doc.NewElement("second");
    if constexpr (std::is_arithmetic_v<V>) second->SetAttribute("val", pair.second);
    else if constexpr (std::is_same_v<V, std::string>) second->SetAttribute("val", pair.second.c_str());
    else throw std::runtime_error("unexpected error");

    std_pair->InsertEndChild(first);
    std_pair->InsertEndChild(second);

    serialization->InsertEndChild(std_pair);
    doc.SaveFile(filename.c_str());
}

template<typename K, typename V>
void deserialize_xml(std::pair<K, V>& pair, const std::string& name, const std::string& filename) {
    XMLDocument doc;
    if (doc.LoadFile(filename.c_str()) != XML_SUCCESS) {
        throw std::runtime_error("file open error");
    }
    
    XMLElement* serialization = doc.FirstChildElement("serialization");
    XMLElement* std_pair = serialization->FirstChildElement(name.c_str());
    if (!std_pair) {
        throw std::runtime_error("fail to find the serialization element");
    }

    XMLElement* first = std_pair->FirstChildElement("first");
    XMLElement* second = std_pair->FirstChildElement("second");

    if (!first || !second) {
        throw std::runtime_error("fail to find the serialization element");
    }
    
    const char* val1 = first->Attribute("val");
    if constexpr (std::is_arithmetic_v<K>) {
        std::stringstream ss1(val1);
        ss1 >> pair.first;
        if (ss1.fail()) {
            throw std::runtime_error("parsing attribute value error");
        }
    }
    else if constexpr (std::is_same_v<K, std::string>) {
        pair.first = val1;
    }
    else {
        throw std::runtime_error("parsing attribute value error");
    }

    const char* val2 = second->Attribute("val");
    if constexpr (std::is_arithmetic_v<V>) {
        std::stringstream ss2(val2);
        ss2 >> pair.second;

        if (ss2.fail()) {
            throw std::runtime_error("parsing attribute value error");
        }
    }
    else if constexpr (std::is_same_v<V, std::string>) {
        pair.second = val2;
    }
    else {
        throw std::runtime_error("parsing attribute value error");
    }
}

}