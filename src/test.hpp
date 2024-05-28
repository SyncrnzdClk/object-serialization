// person.hpp
#pragma once
#include <string>
#include <iostream>
#include "include/binary_serialization.hpp"

class Person {
public:
    std::string name;
    int age;
    double height;

    Person() : name(""), age(0), height(0.0) {}
    Person(std::string name, int age, double height) : name(name), age(age), height(height) {}

    friend std::ostream& operator<<(std::ostream& os, const Person& person) {
        os << "Person{name: " << person.name << ", age: " << person.age << ", height: " << person.height << "}";
        return os;
    }
};

// 序列化和反序列化函数
namespace BinarySerialization {

void serialize(const Person& person, std::ostream& os) {
    serialize(person.name, os);
    serialize(person.age, os);
    serialize(person.height, os);
}

void deserialize(Person& person, std::istream& is) {
    deserialize(person.name, is);
    deserialize(person.age, is);
    deserialize(person.height, is);
}

} // namespace BinarySerialization
