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

// a common interface
template<typename T>
void serialize_xml(const T& value, const std::string& name, const std::string& filename);

// a common interface
template<typename T>
void deserialize_xml(T& value, const std::string& name, const std::string& filename);

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
template<>
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
template<>
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
    const char* val = String->Attribute("val");
    if (!val) {
        throw std::runtime_error("get value error");
    }

    std::stringstream ss(val);
    ss >> value;
    if (ss.fail()) {
        throw std::runtime_error("parsing attribute value error.");
    }
}

// serialize for vector
template<typename T>
void serialize(const std::vector<T>& vec, const std::string& name, const std::string filename) {
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
    XMLElement* std_vector = doc.NewElement(name);

    // create a element for the vector's size
    XMLElement* vec_size = doc.NewElement("vec_size");
    vec_size->SetAttribute("val", vec.size());

    std_vector->InsertEndChild(vec_size);

    // create elements for the vector's contents
    for (const auto element : vec) {
        XMLElement* xml_element = doc.NewElement("element");
        xml_element->SetAttribute("val", element);
    }


    

}


// template<typename T1, typename T2>
// void serialize_xml(const std::pair<T1, T2>& pair, const std::string& name, const std::string& filename) {
//     XMLDocument doc;
//     XMLElement* serialization = doc.NewElement("serialization");
//     doc.InsertEndChild(serialization);

//     XMLElement* std_pair = doc.NewElement(name.c_str());
//     serialization->InsertEndChild(std_pair);

//     XMLElement* first = doc.NewElement("first");
//     first->SetAttribute("val", pair.first);
//     std_pair->InsertEndChild(first);

//     XMLElement* second = doc.NewElement("second");
//     second->SetAttribute("val", pair.second);
//     std_pair->InsertEndChild(second);

//     doc.SaveFile(filename.c_str());
// }

}