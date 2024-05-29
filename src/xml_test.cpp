#include "include/xml_serialization.hpp"
#include "test.hpp"
#include <iostream>
#include <fstream>
#include <vector>
#include <utility>
#include <map>
#include <list>
#include <set>
#include <new> // std::bad_alloc

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
        // std::map<double, double> mapVar = {{1.1, 2.2}, {3.3, 4.4}};
        // std::list<float> listVar = {1.1f, 2.2f, 3.3f};
        // std::set<int> setVar = {1, 2, 3, 4, 5};
        // Person personVar("Leo Ding", 30, 1.75);
        // std::vector<std::vector<int>> nestedVector = {{1, 2, 4}, {2, 3}, {1, 2}};

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
            // serialize(mapVar, ofs);
            // serialize(listVar, ofs);
            // serialize(setVar, ofs);
            // serialize(personVar, ofs);
            // serialize(nestedVector, ofs);
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
            // std::map<double, double> mapVar2;
            // std::list<float> listVar2;
            // std::set<int> setVar2;
            // Person personVar2;
            // std::vector<std::vector<int>> nestedVector2;

            deserialize_xml(intVar2, "int", "int.xml");
            deserialize_xml(doubleVar2, "double", "double.xml");
            deserialize_xml(charVar2, "char", "char.xml");
            deserialize_xml(stringVar2, "string", "string.xml");
            deserialize_xml(vectorVar2, "vector", "vector.xml");
            deserialize_xml(vectorVarStr2, "vector", "vectorStr.xml");
            deserialize_xml(pairVar2, "pair", "pair.xml");
            deserialize_xml(pairStrVar2, "pair", "pairStr.xml");
            // deserialize(mapVar2, ifs);
            // deserialize(listVar2, ifs);
            // deserialize(setVar2, ifs);
            // deserialize(personVar2, ifs);
            // deserialize(nestedVector2, ifs);

            // // 打印反序列化后的值
            std::cout << "Deserialized int: " << intVar2 << std::endl;
            std::cout << "Deserialized double: " << doubleVar2 << std::endl;
            std::cout << "Deserialized char: " << charVar2 << std::endl;
            std::cout << "Deserialized string: " << stringVar2 << std::endl;

            std::cout << "Deserialized vector: ";
            for (const auto& item : vectorVar2) {
                std::cout << item << " ";
            }
            std::cout << std::endl;

            std::cout << "Deserialized vector(string): ";
            for (const auto& item : vectorVarStr2) {
                std::cout << item << " ";
            }
            std::cout << std::endl;

            std::cout << "Deserialized pair: (" << pairVar2.first << ", " << pairVar2.second << ")" << std::endl;

            std::cout << "Deserialized pairStr: (" << pairStrVar2.first << ", " << pairStrVar2.second << ")" << std::endl;

            // std::cout << "Deserialized map: ";
            // for (const auto& [key, value] : mapVar2) {
            //     std::cout << "{" << key << ", " << value << "} ";
            // }
            // std::cout << std::endl;

            // std::cout << "Deserialized list: ";
            // for (const auto& item : listVar2) {
            //     std::cout << item << " ";
            // }
            // std::cout << std::endl;

            // std::cout << "Deserialized set: ";
            // for (const auto& item : setVar2) {
            //     std::cout << item << " ";
            // }
            // std::cout << std::endl;

            // std::cout << "Deserialized person: " << personVar2 << std::endl;

            // std::cout << "Deserialized Nested Vetcor: " << std::endl;
            // for (const auto& vec : nestedVector2) {
            //     for (const auto item : vec) {
            //         std::cout << item << " ";
            //     }
            //     std::cout << std::endl;
            // }
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
