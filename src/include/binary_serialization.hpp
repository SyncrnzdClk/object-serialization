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

#define ASSERT(expr, message) assert((expr) && (message))


namespace BinarySerialization{

// serialize for arithmetic
template<typename T>
typename std::enable_if<std::is_arithmetic<T>::value, void>::type
serialize(const T& value, std::ostream& os) {
    os.write(reinterpret_cast<const char*>(&value), sizeof(T));
}

// deserialize for arithmetic
template<typename T>
typename std::enable_if<std::is_arithmetic<T>::value, void>::type
deserialize(T& value, std::istream& is) {
    is.read(reinterpret_cast<char *>(&value), sizeof(T));
}

// serialize for string
void serialize(const std::string& value, std::ostream& os) {
    size_t size = value.size();
    os.write(reinterpret_cast<const char*>(&size), sizeof(size_t));
    os.write(value.c_str(), size);
}

// deserialize for string
void deserialize(std::string& value, std::istream& is) {
    size_t size;
    is.read(reinterpret_cast<char *>(&size), sizeof(size_t));
    value.resize(size);
    is.read(&value[0], size);
}

// serialize for vector
template<typename T>
void serialize(const std::vector<T>& vec, std::ostream& os) {
    size_t size = vec.size();
    os.write(reinterpret_cast<const char *>(&size), sizeof(size_t));
    for (const auto& element : vec) {
        serialize(element, os);
    }
}

// deserialize for vector
template<typename T>
void deserialize(std::vector<T>& vec, std::istream& is) {
    size_t size;
    is.read(reinterpret_cast<char *>(&size), sizeof(size_t));
    vec.resize(size);
    for (auto& element : vec) {
        deserialize(element, is);
    }
}

// serialize for pair
template<typename K, typename V>
void serialize(const std::pair<K, V>& pair, std::ostream& os) {
    serialize(pair.first, os);
    serialize(pair.second, os);
}

// deserialize for pair
template<typename K, typename V>
void deserialize(std::pair<K, V>& pair, std::istream& is) {
    deserialize(pair.first, is);
    deserialize(pair.second, is);
}

// serialize for map
template<typename K, typename V>
void serialize(const std::map<K, V>& map, std::ostream& os) {
    size_t size = map.size();
    os.write(reinterpret_cast<const char *>(&size), sizeof(size_t));
    for (const auto element : map) {
        serialize(element, os);
    }
}

// deserialize for map
template<typename K, typename V>
void deserialize(std::map<K, V>& map, std::istream& is) {
    size_t size;
    is.read(reinterpret_cast<char *>(&size), sizeof(size_t));
    map.clear();
    std::pair<K, V> element;
    for (size_t i = 0; i < size; i++) {
        deserialize(element, is);
        map.insert(element);
    }
}

// serialize for list
template<typename T>
void serialize(const std::list<T>& lst, std::ostream& os) {
    size_t size = lst.size();
    os.write(reinterpret_cast<const char *>(&size), sizeof(size_t));
    for (const auto& element : lst) {
        serialize(element, os);
    }
}

// deserialize for list
template<typename T>
void deserialize(std::list<T>& lst, std::istream& is) {
    size_t size;
    is.read(reinterpret_cast<char *>(&size), sizeof(size_t));
    lst.resize(size);
    for (auto& element : lst) {
        deserialize(element, is);
    }
}

// serialize for set
template<typename T>
void serialize(const std::set<T>& st, std::ostream& os) {
    size_t size = st.size();
    os.write(reinterpret_cast<const char *>(&size), sizeof(size_t));
    for (const auto& element : st) {
        serialize(element, os);
    }
}

// deserialize for set
template<typename T>
void deserialize(std::set<T>& st, std::istream& is) {
    size_t size;
    is.read(reinterpret_cast<char *>(&size), sizeof(size_t));
    for (size_t i = 0; i < size; i++) {
        T element;
        deserialize(element, is);
        st.insert(element);
    }
}

// a template struct to detect whether the object T has a member function `serialize(std::ostream&)`
template<typename, typename = std::void_t<>>
struct has_serialize : std::false_type {};

template<typename T>
struct has_serialize<T, std::void_t<decltype(std::declval<T>().serialize(std::declval<std::ostream&>()))>> : std::true_type {};

// a template struct to detect whether the object T has a member function `deserialize(std::istream&)`
template<typename, typename = std::void_t<>>
struct has_deserialize : std::false_type {};

template<typename T>
struct has_deserialize<T, std::void_t<decltype(std::declval<T>().deserialize(std::declval<std::istream&>()))>> : std::true_type {};

// serialize for user defined type
template<typename T>
typename std::enable_if<has_serialize<T>::value, void>::type
serialize(const T& value, std::ostream& os) {
    value.serialize(os);
}

// deserialize for user define type
template<typename T>
typename std::enable_if<has_deserialize<T>::value, void>::type
deserialize(T& value, std::istream& is) {
    value.deserialize(is);
}

// serialize for unique pointers
template<typename T>
void serialize(const std::shared_ptr<T[]>& ptr, std::ostream& os, size_t size) {
    if (ptr) {
        os.write(reinterpret_cast<const char*>(size), sizeof(size_t));
        os.write(reinterpret_cast<const char*>(ptr.get()), size * sizeof(T));
    } else {
        throw std::runtime_error("invalid unique_ptr for serialization");
        return ;
    }
}

// deserialize for unique pointers
template<typename T>
void deserialize(std::shared_ptr<T[]>& ptr, std::istream& is) {
    size_t size;
    is.read(reinterpret_cast<char *>(size), sizeof(size_t));
    ptr = std::make_unique<T[]>(size);
    ASSERT(ptr != nullptr, "unexpected error");
    is.read(reinterpret_cast<char *>(ptr.get()), size * sizeof(T));
}

// serialize for shared pointers
template<typename T>
void serialize(const std::unique_ptr<T[]>& ptr, std::ostream& os, size_t size) {
    if (ptr) {
        os.write(reinterpret_cast<const char*>(size), sizeof(size_t));
        os.write(reinterpret_cast<const char*>(ptr.get()), size * sizeof(T));
    } else {
        throw std::runtime_error("invalid shared_ptr for serialization");
        return ;
    }
}

// deserialize for shared pointers
template<typename T>
void deserialize(std::unique_ptr<T[]>& ptr, std::istream& is) {
    size_t size;
    is.read(reinterpret_cast<char *>(size), sizeof(size_t));
    ptr = std::shared_ptr<T[]>(new T[size]);
    ASSERT(ptr != nullptr, "unexpected error");
    is.read(reinterpret_cast<char *>(ptr.get()), size * sizeof(T));
}

};