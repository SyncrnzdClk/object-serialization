#pragma once
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

// Serialize function for arithmetic types (excluding char)
template<typename T>
typename std::enable_if<std::is_arithmetic<T>::value && !std::is_same<T, char>::value, void>::type
serialize_xml(const T& value, const std::string& name, const std::string& filename) {
    XMLDocument doc;
    XMLElement* serialization = nullptr;

    // Load the XML file, if it does not exist, create a new one
    if (doc.LoadFile(filename.c_str()) != XML_SUCCESS) {
        serialization = doc.NewElement("serialization");
        doc.InsertFirstChild(serialization);
    } else {
        serialization = doc.FirstChildElement("serialization");
        if (!serialization) {
            serialization = doc.NewElement("serialization");
            doc.InsertFirstChild(serialization);
        }
    }
    
    // Create a new element with the provided name and set its value attribute
    XMLElement* arithmetic = doc.NewElement(name.c_str());
    arithmetic->SetAttribute("val", value);
    serialization->InsertEndChild(arithmetic);
    doc.SaveFile(filename.c_str());
}

// Serialize function for char type
template<typename T>
typename std::enable_if<std::is_same<T, char>::value, void>::type
serialize_xml(const T& value, const std::string& name, const std::string& filename) {
    XMLDocument doc;
    XMLElement* serialization = nullptr;

    // Load the XML file, if it does not exist, create a new one
    if (doc.LoadFile(filename.c_str()) != XML_SUCCESS) {
        serialization = doc.NewElement("serialization");
        doc.InsertFirstChild(serialization);
    } else {
        serialization = doc.FirstChildElement("serialization");
        if (!serialization) {
            serialization = doc.NewElement("serialization");
            doc.InsertFirstChild(serialization);
        }
    }
    
    // Create a new element with the provided name and set its value attribute as a string of length 1
    XMLElement* charElement = doc.NewElement(name.c_str());
    charElement->SetAttribute("val", std::string(1, value).c_str());
    serialization->InsertEndChild(charElement);
    doc.SaveFile(filename.c_str());
}

// Deserialize function for arithmetic types (excluding char)
template<typename T>
typename std::enable_if<std::is_arithmetic<T>::value && !std::is_same<T, char>::value, void>::type
deserialize_xml(T& value, const std::string& name, const std::string& filename) {
    XMLDocument doc;

    // Load the XML file, throw an error if it cannot be opened
    if (doc.LoadFile(filename.c_str()) != XML_SUCCESS) {
        throw std::runtime_error("File open error");
    }

    // Locate the serialization element, throw an error if not found
    XMLElement* serialization = doc.FirstChildElement("serialization");
    if (!serialization) {
        throw std::runtime_error("Serialization element not found.");
    } 

    // Locate the specific element by name and retrieve its value attribute
    XMLElement* arithmetic = serialization->FirstChildElement(name.c_str());
    const char* val = arithmetic->Attribute("val");
    if (!val) {
        throw std::runtime_error("Value retrieval error");
    }

    // Convert the string attribute value to the appropriate type
    std::stringstream ss(val);
    ss >> value;
    if (ss.fail()) {
        throw std::runtime_error("Parsing attribute value error.");
    }
}

// Deserialize function for char type
template<typename T>
typename std::enable_if<std::is_same<T, char>::value, void>::type
deserialize_xml(T& value, const std::string& name, const std::string& filename) {
    XMLDocument doc;

    // Load the XML file, throw an error if it cannot be opened
    if (doc.LoadFile(filename.c_str()) != XML_SUCCESS) {
        throw std::runtime_error("File open error");
    }

    // Locate the serialization element, throw an error if not found
    XMLElement* serialization = doc.FirstChildElement("serialization");
    if (!serialization) {
        throw std::runtime_error("Serialization element not found.");
    } 

    // Locate the specific element by name and retrieve its value attribute
    XMLElement* charElement = serialization->FirstChildElement(name.c_str());
    const char* val = charElement->Attribute("val");
    if (!val) {
        throw std::runtime_error("Value retrieval error");
    }

    // Ensure the length of the retrieved string is exactly 1
    if (strlen(val) != 1) {
        throw std::runtime_error("Invalid char length");
    }

    // Assign the first character of the string to the value
    value = val[0];
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

// serialize for list
template<typename T>
void serialize_xml(const std::list<T>& lst, const std::string& name, const std::string& filename) {
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

    // create a new element for list
    XMLElement* std_list = doc.NewElement(name.c_str());

    // create elements for the list's contents
    for (const auto& element : lst) {
        XMLElement* xml_element = doc.NewElement("element");
        if constexpr (std::is_arithmetic_v<T>) xml_element->SetAttribute("val", element);
        else if constexpr (std::is_same_v<T, std::string>) xml_element->SetAttribute("val", element.c_str());
        else throw std::runtime_error("this type cannot be serialized");
        std_list->InsertEndChild(xml_element);
    }

    serialization->InsertFirstChild(std_list);

    doc.SaveFile(filename.c_str());
}


// deserialize for list
template<typename T>
void deserialize_xml(std::list<T>& lst, const std::string& name, const std::string& filename) {
    XMLDocument doc;
    if (doc.LoadFile(filename.c_str()) != XML_SUCCESS) {
        throw std::runtime_error("file open error");
    }

    XMLElement* serialization = doc.FirstChildElement("serialization");

    XMLElement* std_list;
    std_list = serialization->FirstChildElement(name.c_str());
    if (!std_list) {
        throw std::runtime_error("fail to find the serialization element");
    }

    // clear the list
    lst.resize(0);
    
    T value;
    for (XMLElement* xml_element = std_list->FirstChildElement("element"); xml_element; xml_element = xml_element->NextSiblingElement()) {
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
        lst.push_back(value);
    }
}

// serialize for set
template<typename T>
void serialize_xml(const std::set<T>& st, const std::string& name, const std::string& filename) {
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

    // create a new element for set
    XMLElement* std_set = doc.NewElement(name.c_str());

    // create elements for the set's contents
    for (const auto& element : st) {
        XMLElement* xml_element = doc.NewElement("element");
        if constexpr (std::is_arithmetic_v<T>) xml_element->SetAttribute("val", element);
        else if constexpr (std::is_same_v<T, std::string>) xml_element->SetAttribute("val", element.c_str());
        else throw std::runtime_error("this type cannot be serialized");
        std_set->InsertEndChild(xml_element);
    }

    serialization->InsertFirstChild(std_set);

    doc.SaveFile(filename.c_str());
}


// Deserialize std::set from XML
template<typename T>
void deserialize_xml(std::set<T>& st, const std::string& name, const std::string& filename) {
    XMLDocument doc;
    // Load the XML file
    if (doc.LoadFile(filename.c_str()) != XML_SUCCESS) {
        throw std::runtime_error("file open error");
    }

    // Get the root element <serialization>
    XMLElement* serialization = doc.FirstChildElement("serialization");

    // Find the specific set element by name
    XMLElement* std_set = serialization->FirstChildElement(name.c_str());
    if (!std_set) {
        throw std::runtime_error("fail to find the serialization element");
    }

    // Clear the set
    st.clear();
    
    T value;
    // Iterate through each <element> in the set
    for (XMLElement* xml_element = std_set->FirstChildElement("element"); xml_element; xml_element = xml_element->NextSiblingElement()) {
        const char* val = xml_element->Attribute("val");
        if (!val) {
            throw std::runtime_error("get value error");
        }
        // Parse the value based on its type
        if constexpr (std::is_arithmetic_v<T>) {
            std::stringstream ss(val);
            ss >> value;
            if (ss.fail()) {
                throw std::runtime_error("parsing attribute value error");
            }
        } else if constexpr (std::is_same_v<T, std::string>) {
            value = val;
        }
        st.insert(value);
    }
}

// Serialize std::pair to XML
template<typename K, typename V>
void serialize_xml(const std::pair<K, V>& pair, const std::string& name, const std::string& filename) {
    XMLDocument doc;
    XMLElement* serialization;
    // Load the XML file, create a new <serialization> element if not found
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
    
    // Create a new element for the pair
    XMLElement* std_pair = doc.NewElement(name.c_str());

    // Create and set the first element's attribute
    XMLElement* first = doc.NewElement("first");
    if constexpr (std::is_arithmetic_v<K>) first->SetAttribute("val", pair.first);
    else if constexpr (std::is_same_v<K, std::string>) first->SetAttribute("val", pair.first.c_str());
    else throw std::runtime_error("unexpected error");
    
    // Create and set the second element's attribute
    XMLElement* second = doc.NewElement("second");
    if constexpr (std::is_arithmetic_v<V>) second->SetAttribute("val", pair.second);
    else if constexpr (std::is_same_v<V, std::string>) second->SetAttribute("val", pair.second.c_str());
    else throw std::runtime_error("unexpected error");

    // Insert first and second elements into the pair element
    std_pair->InsertEndChild(first);
    std_pair->InsertEndChild(second);

    // Insert the pair element into the serialization root
    serialization->InsertEndChild(std_pair);
    doc.SaveFile(filename.c_str());
}

// Deserialize std::pair from XML
template<typename K, typename V>
void deserialize_xml(std::pair<K, V>& pair, const std::string& name, const std::string& filename) {
    XMLDocument doc;
    // Load the XML file
    if (doc.LoadFile(filename.c_str()) != XML_SUCCESS) {
        throw std::runtime_error("file open error");
    }
    
    // Get the root element <serialization>
    XMLElement* serialization = doc.FirstChildElement("serialization");
    // Find the specific pair element by name
    XMLElement* std_pair = serialization->FirstChildElement(name.c_str());
    if (!std_pair) {
        throw std::runtime_error("fail to find the serialization element");
    }

    // Get the first and second elements
    XMLElement* first = std_pair->FirstChildElement("first");
    XMLElement* second = std_pair->FirstChildElement("second");

    if (!first || !second) {
        throw std::runtime_error("fail to find the serialization element");
    }
    
    // Parse the first element's value
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

    // Parse the second element's value
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

// Serialize std::map to XML
template<typename K, typename V>
void serialize_xml(const std::map<K, V>& mp, const std::string& name, const std::string& filename) {
    XMLDocument doc;
    XMLElement* serialization;

    // Load the XML file, create a new <serialization> element if not found
    if (doc.LoadFile(filename.c_str()) != XML_SUCCESS) {
        serialization = doc.NewElement("serialization");
        doc.InsertEndChild(serialization);
    }
    else {
        serialization = doc.FirstChildElement("serialization");
        if (!serialization) {
            serialization = doc.NewElement("serialization");
            doc.InsertEndChild(serialization);
        }
    }

    // Create a new element for the map
    XMLElement* std_map = doc.NewElement(name.c_str());
    // Iterate through each pair in the map
    for (const auto& pair : mp) {
        XMLElement* xml_pair = doc.NewElement("pair");
        
        // Create and set the first element's attribute
        XMLElement* first = doc.NewElement("first");
        if constexpr(std::is_arithmetic_v<K>) first->SetAttribute("val", pair.first);
        else if constexpr(std::is_same_v<K, std::string>) first->SetAttribute("val", (pair.first).c_str());
        else throw std::runtime_error("this type cannot be serialized");
        
        // Create and set the second element's attribute
        XMLElement* second = doc.NewElement("second");
        if constexpr(std::is_arithmetic_v<V>) second->SetAttribute("val", pair.second);
        else if constexpr(std::is_same_v<V, std::string>) second->SetAttribute("val", (pair.second).c_str());
        else throw std::runtime_error("this type cannot be serialized");
        
        // Insert first and second elements into the pair element
        xml_pair->InsertEndChild(first);
        xml_pair->InsertEndChild(second);
        // Insert the pair element into the map element
        std_map->InsertEndChild(xml_pair);
    }

    // Insert the map element into the serialization root
    serialization->InsertEndChild(std_map);
    doc.SaveFile(filename.c_str());
}

// Deserialize std::map from XML
template<typename K, typename V>
void deserialize_xml(std::map<K, V>& mp, const std::string& name, const std::string& filename) {
    XMLDocument doc;
    // Load the XML file
    if (doc.LoadFile(filename.c_str()) != XML_SUCCESS) {
        throw std::runtime_error("fail to open file");
    }

    // Get the root element <serialization>
    XMLElement* serialization = doc.FirstChildElement("serialization");

    // Find the specific map element by name
    XMLElement* std_map = serialization->FirstChildElement(name.c_str());
    mp.clear();
    std::pair<K, V> pair;

    // Iterate through each pair element in the map
    for (XMLElement* xml_pair = std_map->FirstChildElement(); xml_pair; xml_pair = xml_pair->NextSiblingElement()) {
        XMLElement* first = xml_pair->FirstChildElement("first");
        XMLElement* second = xml_pair->FirstChildElement("second");
        
        // Parse the first element's value
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

        // Parse the second element's value
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
        // Insert the parsed pair into the map
        mp.insert(pair);
    }    
}

// A template struct to detect whether the object T has a member function `serialize_xml(tinyxml2::XMLElement&)`
template<typename, typename = std::void_t<>>
struct has_serialize_xml : std::false_type {};

template<typename T>
struct has_serialize_xml<T, std::void_t<decltype(std::declval<T>().serialize_xml(std::declval<tinyxml2::XMLElement&>()))>> : std::true_type {};

// A template struct to detect whether the object T has a member function `deserialize_xml(const tinyxml2::XMLElement&)`
template<typename, typename = std::void_t<>>
struct has_deserialize_xml : std::false_type {};

template<typename T>
struct has_deserialize_xml<T, std::void_t<decltype(std::declval<T>().deserialize_xml(std::declval<const tinyxml2::XMLElement&>()))>> : std::true_type {};

// Serialize user-defined type to XML
template<typename T>
typename std::enable_if<has_serialize_xml<T>::value, void>::type
serialize_xml(const T& value, const std::string& name, const std::string& filename) {
    tinyxml2::XMLDocument doc;
    tinyxml2::XMLElement* serialization;

    // Load the XML file, create a new <serialization> element if not found
    if (doc.LoadFile(filename.c_str()) != tinyxml2::XML_SUCCESS) {
        serialization = doc.NewElement("serialization");
        doc.InsertFirstChild(serialization);
    } else {
        serialization = doc.FirstChildElement("serialization");
        if (!serialization) {
            serialization = doc.NewElement("serialization");
            doc.InsertFirstChild(serialization);
        }
    }

    // Create a new element for the user-defined type
    tinyxml2::XMLElement* element = doc.NewElement(name.c_str());
    value.serialize_xml(*element); // Call the user-defined serialize function
    serialization->InsertEndChild(element); // Insert the element into the serialization root
    doc.SaveFile(filename.c_str());
}

// Deserialize user-defined type from XML
template<typename T>
typename std::enable_if<has_deserialize_xml<T>::value, void>::type
deserialize_xml(T& value, const std::string& name, const std::string& filename) {
    tinyxml2::XMLDocument doc;
    // Load the XML file
    if (doc.LoadFile(filename.c_str()) != tinyxml2::XML_SUCCESS) {
        throw std::runtime_error("file open error");
    }

    // Get the root element <serialization>
    tinyxml2::XMLElement* serialization = doc.FirstChildElement("serialization");
    if (!serialization) {
        throw std::runtime_error("fail to find serialization element.");
    }

    // Find the specific element by name
    tinyxml2::XMLElement* element = serialization->FirstChildElement(name.c_str());
    if (!element) {
        throw std::runtime_error("fail to find the element with the specified name.");
    }

    value.deserialize_xml(*element); // Call the user-defined deserialize function
}


};