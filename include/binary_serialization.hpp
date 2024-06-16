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

// Serialize arithmetic types to a binary stream
template<typename T>
typename std::enable_if<std::is_arithmetic<T>::value, void>::type
serialize(const T& value, std::ostream& os) {
    os.write(reinterpret_cast<const char*>(&value), sizeof(T));
}

// Deserialize arithmetic types from a binary stream
template<typename T>
typename std::enable_if<std::is_arithmetic<T>::value, void>::type
deserialize(T& value, std::istream& is) {
    is.read(reinterpret_cast<char *>(&value), sizeof(T));
}

// Serialize std::string to a binary stream
void serialize(const std::string& value, std::ostream& os) {
    size_t size = value.size();
    os.write(reinterpret_cast<const char*>(&size), sizeof(size_t)); // Write string size
    os.write(value.c_str(), size); // Write string content
}

// Deserialize std::string from a binary stream
void deserialize(std::string& value, std::istream& is) {
    size_t size;
    is.read(reinterpret_cast<char *>(&size), sizeof(size_t)); // Read string size
    value.resize(size);
    is.read(&value[0], size); // Read string content
}

// Serialize std::vector to a binary stream
template<typename T>
void serialize(const std::vector<T>& vec, std::ostream& os) {
    size_t size = vec.size();
    os.write(reinterpret_cast<const char *>(&size), sizeof(size_t)); // Write vector size
    for (const auto& element : vec) {
        serialize(element, os); // Serialize each element in the vector
    }
}

// Deserialize std::vector from a binary stream
template<typename T>
void deserialize(std::vector<T>& vec, std::istream& is) {
    size_t size;
    is.read(reinterpret_cast<char *>(&size), sizeof(size_t)); // Read vector size
    vec.resize(size);
    for (auto& element : vec) {
        deserialize(element, is); // Deserialize each element in the vector
    }
}

// Serialize std::pair to a binary stream
template<typename K, typename V>
void serialize(const std::pair<K, V>& pair, std::ostream& os) {
    serialize(pair.first, os); // Serialize first element of the pair
    serialize(pair.second, os); // Serialize second element of the pair
}

// Deserialize std::pair from a binary stream
template<typename K, typename V>
void deserialize(std::pair<K, V>& pair, std::istream& is) {
    deserialize(pair.first, is); // Deserialize first element of the pair
    deserialize(pair.second, is); // Deserialize second element of the pair
}

// Serialize std::map to a binary stream
template<typename K, typename V>
void serialize(const std::map<K, V>& map, std::ostream& os) {
    size_t size = map.size();
    os.write(reinterpret_cast<const char *>(&size), sizeof(size_t)); // Write map size
    for (const auto& element : map) {
        serialize(element, os); // Serialize each pair in the map
    }
}

// Deserialize std::map from a binary stream
template<typename K, typename V>
void deserialize(std::map<K, V>& map, std::istream& is) {
    size_t size;
    is.read(reinterpret_cast<char *>(&size), sizeof(size_t)); // Read map size
    map.clear();
    std::pair<K, V> element;
    for (size_t i = 0; i < size; i++) {
        deserialize(element, is); // Deserialize each pair in the map
        map.insert(element); // Insert the pair into the map
    }
}

// Serialize std::list to a binary stream
template<typename T>
void serialize(const std::list<T>& lst, std::ostream& os) {
    size_t size = lst.size();
    os.write(reinterpret_cast<const char *>(&size), sizeof(size_t)); // Write list size
    for (const auto& element : lst) {
        serialize(element, os); // Serialize each element in the list
    }
}

// Deserialize std::list from a binary stream
template<typename T>
void deserialize(std::list<T>& lst, std::istream& is) {
    size_t size;
    is.read(reinterpret_cast<char *>(&size), sizeof(size_t)); // Read list size
    lst.resize(size);
    for (auto& element : lst) {
        deserialize(element, is); // Deserialize each element in the list
    }
}

// Serialize std::set to a binary stream
template<typename T>
void serialize(const std::set<T>& st, std::ostream& os) {
    size_t size = st.size();
    os.write(reinterpret_cast<const char *>(&size), sizeof(size_t)); // Write set size
    for (const auto& element : st) {
        serialize(element, os); // Serialize each element in the set
    }
}

// Deserialize std::set from a binary stream
template<typename T>
void deserialize(std::set<T>& st, std::istream& is) {
    size_t size;
    is.read(reinterpret_cast<char *>(&size), sizeof(size_t)); // Read set size
    for (size_t i = 0; i < size; i++) {
        T element;
        deserialize(element, is); // Deserialize each element in the set
        st.insert(element); // Insert element into the set
    }
}

// Detect if a type has a member function `serialize(std::ostream&)`
template<typename, typename = std::void_t<>>
struct has_serialize : std::false_type {};

template<typename T>
struct has_serialize<T, std::void_t<decltype(std::declval<T>().serialize(std::declval<std::ostream&>()))>> : std::true_type {};

// Detect if a type has a member function `deserialize(std::istream&)`
template<typename, typename = std::void_t<>>
struct has_deserialize : std::false_type {};

template<typename T>
struct has_deserialize<T, std::void_t<decltype(std::declval<T>().deserialize(std::declval<std::istream&>()))>> : std::true_type {};

// Serialize user-defined types to a binary stream
template<typename T>
typename std::enable_if<has_serialize<T>::value, void>::type
serialize(const T& value, std::ostream& os) {
    value.serialize(os); // Call the user-defined serialize function
}

// Deserialize user-defined types from a binary stream
template<typename T>
typename std::enable_if<has_deserialize<T>::value, void>::type
deserialize(T& value, std::istream& is) {
    value.deserialize(is); // Call the user-defined deserialize function
}

// Serialize std::unique_ptr to a binary stream
template<typename T>
void serialize(const std::unique_ptr<T[]>& ptr, std::ostream& os, size_t size) {
    if (ptr) {
        os.write(reinterpret_cast<const char*>(&size), sizeof(size_t)); // Write size of unique_ptr array
        os.write(reinterpret_cast<const char*>(ptr.get()), size * sizeof(T)); // Write array content
    } else {
        throw std::runtime_error("invalid unique_ptr for serialization");
    }
}

// Deserialize std::unique_ptr from a binary stream
template<typename T>
void deserialize(std::unique_ptr<T[]>& ptr, std::istream& is) {
    size_t size;
    is.read(reinterpret_cast<char*>(&size), sizeof(size_t)); // Read size of unique_ptr array
    ptr = std::make_unique<T[]>(size);
    ASSERT(ptr != nullptr, "unexpected error");
    is.read(reinterpret_cast<char*>(ptr.get()), size * sizeof(T)); // Read array content
}

// Serialize std::shared_ptr to a binary stream
template<typename T>
void serialize(const std::shared_ptr<T[]>& ptr, std::ostream& os, size_t size) {
    if (ptr) {
        os.write(reinterpret_cast<const char*>(&size), sizeof(size_t)); // Write size of shared_ptr array
        os.write(reinterpret_cast<const char*>(ptr.get()), size * sizeof(T)); // Write array content
    } else {
        throw std::runtime_error("invalid shared_ptr for serialization");
    }
}

// Deserialize std::shared_ptr from a binary stream
template<typename T>
void deserialize(std::shared_ptr<T[]>& ptr, std::istream& is) {
    size_t size;
    is.read(reinterpret_cast<char*>(&size), sizeof(size_t)); // Read size of shared_ptr array
    ptr = std::shared_ptr<T[]>(new T[size]);
    ASSERT(ptr != nullptr, "unexpected error");
    is.read(reinterpret_cast<char*>(ptr.get()), size * sizeof(T)); // Read array content
}


};