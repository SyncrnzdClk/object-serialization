// person.hpp
#pragma once
#include <string>
#include <iostream>
#include "../include/binary_serialization.hpp"
#include "../include/xml_serialization.hpp"

class Person {
public:
    std::string name;
    int age;
    double height;

    Person() : name(""), age(0), height(0.0) {}
    Person(std::string name, int age, double height) : name(name), age(age), height(height) {}

    // overload the << operator
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
