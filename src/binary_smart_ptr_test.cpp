#include <iostream>
#include <memory>
#include <fstream>
#include <cassert>
#include "../include/binary_serialization.hpp"

using namespace BinarySerialization;

void test_unique_ptr_serialization() {
    const size_t size = 5;
    auto uptr = std::make_unique<int[]>(size);
    for (size_t i = 0; i < size; ++i) {
        uptr[i] = static_cast<int>(i + 1);
    }

    // serializa
    std::ofstream ofs("unique_ptr.bin", std::ios::binary);
    serialize(uptr, ofs, size);
    ofs.close();

    // deserialize
    std::unique_ptr<int[]> uptr2;
    std::ifstream ifs("unique_ptr.bin", std::ios::binary);
    deserialize(uptr2, ifs);
    ifs.close();

    // check deserialization result
    for (size_t i = 0; i < size; ++i) {
        ASSERT(uptr2[i] == static_cast<int>(i + 1), "unique_ptr deserialization failed");
    }
    std::cout << "Unique pointer serialization test passed." << std::endl;
}

void test_shared_ptr_serialization() {
    const size_t size = 5;
    std::shared_ptr<int[]> sptr(new int[size]);
    for (size_t i = 0; i < size; ++i) {
        sptr[i] = static_cast<int>(i + 1);
    }

    // serialize
    std::ofstream ofs("shared_ptr.bin", std::ios::binary);
    serialize(sptr, ofs, size);
    ofs.close();

    // deserialize
    std::shared_ptr<int[]> sptr2;
    std::ifstream ifs("shared_ptr.bin", std::ios::binary);
    deserialize(sptr2, ifs);
    ifs.close();

    // check deserilization result
    for (size_t i = 0; i < size; ++i) {
        ASSERT(sptr2[i] == static_cast<int>(i + 1), "shared_ptr deserialization failed");
    }
    std::cout << "Shared pointer serialization test passed." << std::endl;
}

int main() {
    test_unique_ptr_serialization();
    test_shared_ptr_serialization();
    return 0;
}
