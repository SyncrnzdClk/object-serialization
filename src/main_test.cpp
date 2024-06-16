#include <iostream>
#include <fstream>
#include <vector>
#include <utility>
#include <map>
#include <list>
#include <set>
#include <memory>
#include "../include/binary_serialization.hpp"
#include "../include/xml_serialization.hpp"

using namespace BinarySerialization;
using namespace XMLSerialization;

class Person {
public:
    std::string name;
    int age;
    double height;

    Person() : name(""), age(0), height(0.0) {}
    Person(std::string name, int age, double height) : name(std::move(name)), age(age), height(height) {}

    // Overload the << operator
    friend std::ostream& operator<<(std::ostream& os, const Person& person) {
        os << "Person{name: " << person.name << ", age: " << person.age << ", height: " << person.height << "}";
        return os;
    }

    // Overload the == operator
    bool operator==(const Person& other) const {
        return name == other.name && age == other.age && height == other.height;
    }

    void serialize(std::ostream& os) const {
        BinarySerialization::serialize(this->name, os);
        BinarySerialization::serialize(this->age, os);
        BinarySerialization::serialize(this->height, os);
    }

    void deserialize(std::istream& is) {
        BinarySerialization::deserialize(this->name, is);
        BinarySerialization::deserialize(this->age, is);
        BinarySerialization::deserialize(this->height, is);
    }

    // Serialization to XML
    void serialize_xml(tinyxml2::XMLElement& element) const {
        tinyxml2::XMLDocument* doc = element.GetDocument();

        tinyxml2::XMLElement* nameElement = doc->NewElement("name");
        nameElement->SetText(name.c_str());
        element.InsertEndChild(nameElement);

        tinyxml2::XMLElement* ageElement = doc->NewElement("age");
        ageElement->SetText(age);
        element.InsertEndChild(ageElement);

        tinyxml2::XMLElement* heightElement = doc->NewElement("height");
        heightElement->SetText(height);
        element.InsertEndChild(heightElement);
    }

    // Deserialization from XML
    void deserialize_xml(const tinyxml2::XMLElement& element) {
        const tinyxml2::XMLElement* nameElement = element.FirstChildElement("name");
        if (nameElement) {
            name = nameElement->GetText();
        }

        const tinyxml2::XMLElement* ageElement = element.FirstChildElement("age");
        if (ageElement) {
            age = ageElement->IntText();
        }

        const tinyxml2::XMLElement* heightElement = element.FirstChildElement("height");
        if (heightElement) {
            height = heightElement->DoubleText();
        }
    }
};

void test_unique_ptr_serialization() {
    const size_t size = 5;
    auto uptr = std::make_unique<int[]>(size);
    for (size_t i = 0; i < size; ++i) {
        uptr[i] = static_cast<int>(i + 1);
    }

    // Serialize
    std::ofstream ofs("unique_ptr.bin", std::ios::binary);
    serialize(uptr, ofs, size);
    ofs.close();

    // Deserialize
    std::unique_ptr<int[]> uptr2;
    std::ifstream ifs("unique_ptr.bin", std::ios::binary);
    deserialize(uptr2, ifs);
    ifs.close();

    // Check deserialization result
    for (size_t i = 0; i < size; ++i) {
        ASSERT(uptr2[i] == static_cast<int>(i + 1), "unique_ptr deserialization failed");
    }
    std::cout << "Unique pointer serialization test passed." << std::endl;
}

void test_shared_ptr_serialization() {
    const size_t size = 5;
    std::shared_ptr<int[]> sptr(new int[size]);
    for (size_t i = 0; i < size; ++i) {
        sptr[i] = static_cast<int>(i + 1);
    }

    // Serialize
    std::ofstream ofs("shared_ptr.bin", std::ios::binary);
    serialize(sptr, ofs, size);
    ofs.close();

    // Deserialize
    std::shared_ptr<int[]> sptr2;
    std::ifstream ifs("shared_ptr.bin", std::ios::binary);
    deserialize(sptr2, ifs);
    ifs.close();

    // Check deserialization result
    for (size_t i = 0; i < size; ++i) {
        ASSERT(sptr2[i] == static_cast<int>(i + 1), "shared_ptr deserialization failed");
    }
    std::cout << "Shared pointer serialization test passed." << std::endl;
}

void test_binary_serialization() {
    // Initialize various variables
    int intVar = 42;
    double doubleVar = 3.14159;
    char charVar = 'A';
    std::string stringVar = "Hello, World!";
    std::vector<int> vectorVar = {1, 2, 3, 4, 5};
    std::vector<std::string> vectorVarStr = {"bob and john,", "leo,", "hi,", "hello world,"};
    std::pair<int, double> pairVar = {42, 3.14};
    std::pair<std::string, double> pairStrVar = {"leo", 19.2};
    std::map<double, double> mapVar = {{1.1, 2.2}, {3.3, 4.4}};
    std::list<float> listVar = {1.1f, 2.2f, 3.3f};
    std::set<int> setVar = {1, 2, 3, 4, 5};
    Person personVar("Leo Ding", 30, 1.75);

    // Serialization to binary
    {
        std::ofstream ofs("data.bin", std::ios::binary);
        if (!ofs) {
            std::cerr << "Failed to open file for writing." << std::endl;
            return;
        }
        serialize(intVar, ofs);
        serialize(doubleVar, ofs);
        serialize(charVar, ofs);
        serialize(stringVar, ofs);
        serialize(vectorVar, ofs);
        serialize(vectorVarStr, ofs);
        serialize(pairVar, ofs);
        serialize(pairStrVar, ofs);
        serialize(mapVar, ofs);
        serialize(listVar, ofs);
        serialize(setVar, ofs);
        serialize(personVar, ofs);
    }

    // Deserialization from binary
    {
        std::ifstream ifs("data.bin", std::ios::binary);
        if (!ifs) {
            std::cerr << "Failed to open file for reading." << std::endl;
            return;
        }

        int intVar2;
        double doubleVar2;
        char charVar2;
        std::string stringVar2;
        std::vector<int> vectorVar2;
        std::vector<std::string> vectorVarStr2;
        std::pair<int, double> pairVar2;
        std::pair<std::string, double> pairStrVar2;
        std::map<double, double> mapVar2;
        std::list<float> listVar2;
        std::set<int> setVar2;
        Person personVar2;

        deserialize(intVar2, ifs);
        deserialize(doubleVar2, ifs);
        deserialize(charVar2, ifs);
        deserialize(stringVar2, ifs);
        deserialize(vectorVar2, ifs);
        deserialize(vectorVarStr2, ifs);
        deserialize(pairVar2, ifs);
        deserialize(pairStrVar2, ifs);
        deserialize(mapVar2, ifs);
        deserialize(listVar2, ifs);
        deserialize(setVar2, ifs);
        deserialize(personVar2, ifs);

        // Validate deserialized values
        ASSERT(intVar2 == intVar, "Deserialized int does not match.");
        ASSERT(doubleVar2 == doubleVar, "Deserialized double does not match.");
        ASSERT(charVar2 == charVar, "Deserialized char does not match.");
        ASSERT(stringVar2 == stringVar, "Deserialized string does not match.");
        ASSERT(vectorVar2 == vectorVar, "Deserialized vector does not match.");
        ASSERT(vectorVarStr2 == vectorVarStr, "Deserialized vector(string) does not match.");
        ASSERT(pairVar2 == pairVar, "Deserialized pair does not match.");
        ASSERT(pairStrVar2 == pairStrVar, "Deserialized pair(string, double) does not match.");
        ASSERT(mapVar2 == mapVar, "Deserialized map does not match.");
        ASSERT(listVar2 == listVar, "Deserialized list does not match.");
        ASSERT(setVar2 == setVar, "Deserialized set does not match.");
        ASSERT(personVar2 == personVar, "Deserialized person does not match.");
    }

    std::cout << "Binary serialization test passed." << std::endl;
}

void test_xml_serialization() {
    int intVar = 42;
    double doubleVar = 3.14159;
    char charVar = 'A';
    std::string stringVar = "Hello, World!";
    std::vector<int> vectorVar = {1, 2, 3, 4, 5};
    std::vector<std::string> vectorVarStr = {"bob and john,", "leo,", "hi,", "hello world,"};
    std::pair<int, double> pairVar = {42, 3.14};
    std::pair<std::string, double> pairStrVar = {"leo", 19.2};
    std::map<double, double> mapVar = {{1.1, 2.2}, {3.3, 4.4}};
    std::list<float> listVar = {1.1f, 2.2f, 3.3f};
    std::set<int> setVar = {1, 2, 3, 4, 5};
    Person personVar("Leo Ding", 30, 1.75);

    // Serialization to XML
    serialize_xml(intVar, "int", "int.xml");
    serialize_xml(doubleVar, "double", "double.xml");
    serialize_xml(charVar, "char", "char.xml");
    serialize_xml(stringVar, "string", "string.xml");
    serialize_xml(vectorVar, "vector", "vector.xml");
    serialize_xml(vectorVarStr, "vector", "vectorStr.xml");
    serialize_xml(pairVar, "pair", "pair.xml");
    serialize_xml(pairStrVar, "pair", "pairStr.xml");
    serialize_xml(mapVar, "map", "map.xml");
    serialize_xml(listVar, "list", "list.xml");
    serialize_xml(setVar, "set", "set.xml");
    serialize_xml(personVar, "Person", "person.xml");

    // Deserialization from XML
    int intVar2;
    double doubleVar2;
    char charVar2;
    std::string stringVar2;
    std::vector<int> vectorVar2;
    std::vector<std::string> vectorVarStr2;
    std::pair<int, double> pairVar2;
    std::pair<std::string, double> pairStrVar2;
    std::map<double, double> mapVar2;
    std::list<float> listVar2;
    std::set<int> setVar2;
    Person personVar2;

    deserialize_xml(intVar2, "int", "int.xml");
    deserialize_xml(doubleVar2, "double", "double.xml");
    deserialize_xml(charVar2, "char", "char.xml");
    deserialize_xml(stringVar2, "string", "string.xml");
    deserialize_xml(vectorVar2, "vector", "vector.xml");
    deserialize_xml(vectorVarStr2, "vector", "vectorStr.xml");
    deserialize_xml(pairVar2, "pair", "pair.xml");
    deserialize_xml(pairStrVar2, "pair", "pairStr.xml");
    deserialize_xml(mapVar2, "map", "map.xml");
    deserialize_xml(listVar2, "list", "list.xml");
    deserialize_xml(setVar2, "set", "set.xml");
    deserialize_xml(personVar2, "Person", "person.xml");

    // Validate deserialized values
    ASSERT(intVar2 == intVar, "Deserialized int does not match.");
    ASSERT(doubleVar2 == doubleVar, "Deserialized double does not match.");
    ASSERT(charVar2 == charVar, "Deserialized char does not match.");
    ASSERT(stringVar2 == stringVar, "Deserialized string does not match.");
    ASSERT(vectorVar2 == vectorVar, "Deserialized vector does not match.");
    ASSERT(vectorVarStr2 == vectorVarStr, "Deserialized vector(string) does not match.");
    ASSERT(pairVar2 == pairVar, "Deserialized pair does not match.");
    ASSERT(pairStrVar2 == pairStrVar, "Deserialized pair(string, double) does not match.");
    ASSERT(mapVar2 == mapVar, "Deserialized map does not match.");
    ASSERT(listVar2 == listVar, "Deserialized list does not match.");
    ASSERT(setVar2 == setVar, "Deserialized set does not match.");
    ASSERT(personVar2 == personVar, "Deserialized person does not match.");

    std::cout << "XML serialization test passed." << std::endl;
}


int main() {
    try {
        test_binary_serialization();
        test_xml_serialization();
        test_unique_ptr_serialization();
        test_shared_ptr_serialization();
    } catch (const std::bad_alloc& e) {
        std::cerr << "Memory allocation failed: " << e.what() << std::endl;
        return 1;
    } catch (const std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
        return 1;
    } catch (...) {
        std::cerr << "Unknown exception occurred." << std::endl;
        return 1;
    }

    return 0;
}
