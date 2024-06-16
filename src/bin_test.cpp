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

            // print values
            std::cout << "Deserialized int: " << intVar2 << std::endl;
            std::cout << "Deserialized double: " << doubleVar2 << std::endl;
            std::cout << "Deserialized char: " << charVar2 << std::endl;
            std::cout << "Deserialized string: " << stringVar2 << std::endl;

            std::cout << "Deserialized vector: ";
            for (const auto& item : vectorVar2) {
                std::cout << item << " ";
            }
            std::cout << std::endl;

            std::cout << "Deserialized pair: (" << pairVar2.first << ", " << pairVar2.second << ")" << std::endl;

            std::cout << "Deserialized map: ";
            for (const auto& [key, value] : mapVar2) {
                std::cout << "{" << key << ", " << value << "} ";
            }
            std::cout << std::endl;

            std::cout << "Deserialized list: ";
            for (const auto& item : listVar2) {
                std::cout << item << " ";
            }
            std::cout << std::endl;

            std::cout << "Deserialized set: ";
            for (const auto& item : setVar2) {
                std::cout << item << " ";
            }
            std::cout << std::endl;

            std::cout << "Deserialized person: " << personVar2 << std::endl;

            std::cout << "Deserialized Nested Vetcor: " << std::endl;
            for (const auto& vec : nestedVector2) {
                for (const auto item : vec) {
                    std::cout << item << " ";
                }
                std::cout << std::endl;
            }
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
