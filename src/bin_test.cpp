#include "../include/binary_serialization.hpp"
#include "../include/Person.hpp"
#include <iostream>
#include <fstream>
#include <vector>
#include <utility>
#include <map>
#include <list>
#include <set>
#include <new> 

using namespace BinarySerialization;

int main() {
    try {
        // initialize
        int intVar = 42;
        double doubleVar = 3.14159;
        char charVar = 'A';
        std::string stringVar = "Hello, World!";
        std::vector<int> vectorVar = {1, 2, 3, 4, 5};
        std::pair<int, double> pairVar = {42, 3.14};
        std::map<double, double> mapVar = {{1.1, 2.2}, {3.3, 4.4}};
        std::list<float> listVar = {1.1f, 2.2f, 3.3f};
        std::set<int> setVar = {1, 2, 3, 4, 5};
        Person personVar("Leo Ding", 30, 1.75);
        std::vector<std::vector<int>> nestedVector = {{1, 2, 4}, {2, 3}, {1, 2}};

        // serialization
        {
            std::ofstream ofs("data.bin", std::ios::binary);
            if (!ofs) {
                std::cerr << "Failed to open file for writing." << std::endl;
                return 1;
            }
            serialize(intVar, ofs);
            serialize(doubleVar, ofs);
            serialize(charVar, ofs);
            serialize(stringVar, ofs);
            serialize(vectorVar, ofs);
            serialize(pairVar, ofs);
            serialize(mapVar, ofs);
            serialize(listVar, ofs);
            serialize(setVar, ofs);
            serialize(personVar, ofs);
            serialize(nestedVector, ofs);
        }

        // deserialization
        {
            std::ifstream ifs("data.bin", std::ios::binary);
            if (!ifs) {
                std::cerr << "Failed to open file for reading." << std::endl;
                return 1;
            }

            int intVar2;
            double doubleVar2;
            char charVar2;
            std::string stringVar2;
            std::vector<int> vectorVar2;
            std::pair<int, double> pairVar2;
            std::map<double, double> mapVar2;
            std::list<float> listVar2;
            std::set<int> setVar2;
            Person personVar2;
            std::vector<std::vector<int>> nestedVector2;

            deserialize(intVar2, ifs);
            deserialize(doubleVar2, ifs);
            deserialize(charVar2, ifs);
            deserialize(stringVar2, ifs);
            deserialize(vectorVar2, ifs);
            deserialize(pairVar2, ifs);
            deserialize(mapVar2, ifs);
            deserialize(listVar2, ifs);
            deserialize(setVar2, ifs);
            deserialize(personVar2, ifs);
            deserialize(nestedVector2, ifs);

            // assert values
            ASSERT(intVar2 == intVar, "Deserialized int does not match.");
            ASSERT(doubleVar2 == doubleVar, "Deserialized double does not match.");
            ASSERT(charVar2 == charVar, "Deserialized char does not match.");
            ASSERT(stringVar2 == stringVar, "Deserialized string does not match.");
            ASSERT(vectorVar2 == vectorVar, "Deserialized vector does not match.");
            ASSERT(pairVar2 == pairVar, "Deserialized pair does not match.");
            ASSERT(mapVar2 == mapVar, "Deserialized map does not match.");
            ASSERT(listVar2 == listVar, "Deserialized list does not match.");
            ASSERT(setVar2 == setVar, "Deserialized set does not match.");
            ASSERT(personVar2 == personVar, "Deserialized person does not match.");
            ASSERT(nestedVector2 == nestedVector, "Deserialized nested vector does not match.");
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
