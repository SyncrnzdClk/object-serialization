#pragma once
#include <type_traits>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <utility>
#include <map>
#include <list>
#include <set>

namespace BinarySerialization {

// 序列化函数，只对算术类型有效
template<typename T>
typename std::enable_if<std::is_arithmetic<T>::value, void>::type
serialize(const T& value, std::ostream& os) {
    os.write(reinterpret_cast<const char*>(&value), sizeof(T));
}

// 反序列化函数，只对算术类型有效
template<typename T>
typename std::enable_if<std::is_arithmetic<T>::value, void>::type
deserialize(T& value, std::istream& is) {
    is.read(reinterpret_cast<char*>(&value), sizeof(T));
}

// 序列化 string 类型
void serialize(const std::string& value, std::ostream& os) {
    size_t length = value.size();
    os.write(reinterpret_cast<const char*>(&length), sizeof(length));
    os.write(value.data(), length);
}

// 反序列化 string 类型
void deserialize(std::string& value, std::istream& is) {
    size_t length;
    is.read(reinterpret_cast<char*>(&length), sizeof(length));
    value.resize(length);
    is.read(&value[0], length);
}

// 序列化 vector<int> 类型
template<typename T>
void serialize(const std::vector<T>& vec, std::ostream& os) {
    size_t size = vec.size();
    os.write(reinterpret_cast<const char*>(&size), sizeof(size));
    for (const T& item : vec) {
        serialize(item, os);
    }
}

// 反序列化 vector<int> 类型
template<typename T>
void deserialize(std::vector<T>& vec, std::istream& is) {
    size_t size;
    is.read(reinterpret_cast<char*>(&size), sizeof(size));
    vec.resize(size);
    for (T& item : vec) {
        deserialize(item, is);
    }
}

// 序列化 pair<int, double> 类型
template<typename T1, typename T2>
void serialize(const std::pair<T1, T2>& p, std::ostream& os) {
    serialize(p.first, os);
    serialize(p.second, os);
}

// 反序列化 pair<int, double> 类型
template<typename T1, typename T2>
void deserialize(std::pair<T1, T2>& p, std::istream& is) {
    deserialize(p.first, is);
    deserialize(p.second, is);
}

// 序列化 map<double, double> 类型
template<typename K, typename V>
void serialize(const std::map<K, V>& map, std::ostream& os) {
    size_t size = map.size();
    os.write(reinterpret_cast<const char*>(&size), sizeof(size));
    for (const auto& pair : map) {
        serialize(pair, os);
    }
}

// 反序列化 map<double, double> 类型
template<typename K, typename V>
void deserialize(std::map<K, V>& map, std::istream& is) {
    size_t size;
    is.read(reinterpret_cast<char*>(&size), sizeof(size));
    map.clear();
    for (size_t i = 0; i < size; ++i) {
        std::pair<K, V> pair;
        deserialize(pair, is);
        map.insert(pair);
    }
}

// 序列化 list<float> 类型
template<typename T>
void serialize(const std::list<T>& list, std::ostream& os) {
    size_t size = list.size();
    os.write(reinterpret_cast<const char*>(&size), sizeof(size));
    for (const T& item : list) {
        serialize(item, os);
    }
}

// 反序列化 list<float> 类型
template<typename T>
void deserialize(std::list<T>& list, std::istream& is) {
    size_t size;
    is.read(reinterpret_cast<char*>(&size), sizeof(size));
    list.clear();
    for (size_t i = 0; i < size; ++i) {
        T item;
        deserialize(item, is);
        list.push_back(item);
    }
}

// 序列化 set<int> 类型
template<typename T>
void serialize(const std::set<T>& set, std::ostream& os) {
    size_t size = set.size();
    os.write(reinterpret_cast<const char*>(&size), sizeof(size));
    for (const T& item : set) {
        serialize(item, os);
    }
}

// 反序列化 set<int> 类型
template<typename T>
void deserialize(std::set<T>& set, std::istream& is) {
    size_t size;
    is.read(reinterpret_cast<char*>(&size), sizeof(size));
    set.clear();
    for (size_t i = 0; i < size; ++i) {
        T item;
        deserialize(item, is);
        set.insert(item);
    }
}

} // namespace BinarySerialization
