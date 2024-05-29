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
};
