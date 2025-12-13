#include <iostream>
#include "DynamicArray.hpp"
#include "DynamicArray_Tests.hpp"
#include <gtest/gtest.h>

int main(int argc, char** argv)
{
    DynamicArray<int> a;
    for (int i = 0; i < 10; ++i)
        a.insert(i + 1);
    for (int i = 0; i < a.size(); ++i)
        a[i] *= 2;
    for (auto it = a.iterator(); it.hasNext(); it.next())
        std::cout << it.get() << " ";
    std::cout << "\n";
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}