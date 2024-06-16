#include "../include/xml_serialization.hpp"
#include "../include/Person.hpp"
#include <iostream>
#include <fstream>
#include <vector>
#include <utility>
#include <map>
#include <list>
#include <set>
#include <new> // std::bad_alloc
#include <filesystem>

using namespace XMLSerialization;

int main() {
    try {
        // 初始化各种类型的变量
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

        // 序列化
        {
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
            XMLSerialization::serialize_xml(personVar, "Person", "person.xml");
        }

        // 反序列化
        {
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
            XMLSerialization::deserialize_xml(personVar2, "Person", "person.xml");

            // 使用断言来验证反序列化后的值
            ASSERT(intVar2 == intVar, "Deserialized int does not match.");
            ASSERT(doubleVar2 == doubleVar, "Deserialized double does not match.");
            ASSERT(charVar2 == charVar, "Deserialized char does not match.");
            ASSERT(stringVar2 == stringVar, "Deserialized string does not match.");
            ASSERT(vectorVar2 == vectorVar, "Deserialized vector does not match.");
            ASSERT(vectorVarStr2 == vectorVarStr, "Deserialized vector of strings does not match.");
            ASSERT(pairVar2 == pairVar, "Deserialized pair does not match.");
            ASSERT(pairStrVar2 == pairStrVar, "Deserialized pair of string and double does not match.");
            ASSERT(mapVar2 == mapVar, "Deserialized map does not match.");
            ASSERT(listVar2 == listVar, "Deserialized list does not match.");
            ASSERT(setVar2 == setVar, "Deserialized set does not match.");
            ASSERT(personVar2 == personVar, "Deserialized person does not match.");
        }
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
