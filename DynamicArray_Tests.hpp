#pragma once
#include <gtest/gtest.h>
#include <string>
#include "DynamicArray.hpp"

#pragma region Constructors
TEST(DynamicArrayTest, DefaultConstructor) {
    DynamicArray<int> arr;
    EXPECT_EQ(arr.size(), 0);
}
TEST(DynamicArrayTest, ConstructorWithCapacity) {
    DynamicArray<int> arr(16);
    EXPECT_EQ(arr.size(), 0);
}

TEST(DynamicArrayTest, CopyConstructor) {
    DynamicArray<int> arr1;

    for (int i = 0; i < 10; ++i) {
        arr1.insert(i * 10);
    }

    DynamicArray<int> arr2 = arr1;

    EXPECT_EQ(arr2.size(), arr1.size());

    for (int i = 0; i < 10; ++i) {
        EXPECT_EQ(arr2[i], arr1[i]);
    }

    arr1[0] = 999;
    EXPECT_EQ(arr2[0], 0);
}

TEST(DynamicArrayTest, MoveConstructor) {
    DynamicArray<int> arr1;

    for (int i = 0; i < 10; ++i) {
        arr1.insert(i * 10);
    }

    int old_size = arr1.size();

    DynamicArray<int> arr2 = std::move(arr1);

    EXPECT_EQ(arr2.size(), old_size);
    EXPECT_EQ(arr1.size(), 0);

    for (int i = 0; i < 10; ++i) {
        EXPECT_EQ(arr2[i], i * 10);
    }
}

TEST(DynamicArrayTest, CopyAssignment) {
    DynamicArray<int> arr1;

    for (int i = 0; i < 5; ++i) {
        arr1.insert(i * 10);
    }

    DynamicArray<int> arr2 = arr1;

    EXPECT_EQ(arr2.size(), arr1.size());

    for (int i = 0; i < 5; ++i) {
        EXPECT_EQ(arr2[i], arr1[i]);
    }
}

TEST(DynamicArrayTest, MoveAssignment) {
    DynamicArray<int> arr1;

    for (int i = 0; i < 5; ++i) {
        arr1.insert(i * 10);
    }

    int old_size = arr1.size();

    DynamicArray<int> arr2 = std::move(arr1);

    EXPECT_EQ(arr2.size(), old_size);
    EXPECT_EQ(arr1.size(), 0);
}

TEST(DynamicArrayTest, Swap) {
    DynamicArray<int> arr1;
    arr1.insert(1);
    DynamicArray<int> arr2;
    arr2.insert(2);
    arr2.insert(3);

    arr1.swap(arr2);
    EXPECT_EQ(arr1[0], 2);
    EXPECT_EQ(arr1[1], 3);

    EXPECT_EQ(arr2[0], 1);

    EXPECT_EQ(arr1.size(), 2);
    EXPECT_EQ(arr2.size(), 1);
}

#pragma endregion
#pragma region Iterators
TEST(DynamicArrayTest, ForwardIterator) {
    DynamicArray<int> arr;

    for (int i = 0; i < 5; ++i) {
        arr.insert(i * 10);
    }

    auto it = arr.iterator();
    int expected = 0;

    while (true) {
        EXPECT_EQ(it.get(), expected);
        expected += 10;

        if (!it.hasNext()) break;
        it.next();
    }

    auto it2 = arr.iterator();
    it2.set(999);
    EXPECT_EQ(arr[0], 999);
}

TEST(DynamicArrayTest, ReverseIterator) {
    DynamicArray<int> arr;

    for (int i = 0; i < 5; ++i) {
        arr.insert(i * 10);
    }

    auto it = arr.reverseIterator();
    int expected = 40;

    while (true) {
        EXPECT_EQ(it.get(), expected);
        expected -= 10;

        if (!it.hasNext()) break;
        it.next();
    }
}
TEST(DynamicArrayTest, ForwardIteratorString) {
    DynamicArray<std::string> arr;

    arr.insert("iterator");
    arr.insert("test");
    arr.insert("for");
    arr.insert("strings");

    auto it = arr.iterator();

    EXPECT_EQ(it.get(), "iterator");

    it.next();
    EXPECT_EQ(it.get(), "test");

    it.next();
    EXPECT_EQ(it.get(), "for");

    it.next();
    EXPECT_EQ(it.get(), "strings");

    EXPECT_FALSE(it.hasNext());

    auto it2 = arr.iterator();
    it2.set("modified");
    EXPECT_EQ(arr[0], "modified");

    auto it3 = arr.iterator();
    int count = 0;
    while (true) {
        ++count;
        if (!it3.hasNext()) break;
        it3.next();
    }
    EXPECT_EQ(count, 4);
}
#pragma endregion
#pragma region BasicOperations
TEST(DynamicArrayTest, InsertAndAccess) {
    DynamicArray<int> arr;
    int x = 10, y = 20, z = 30;
    arr.insert(x);
    arr.insert(std::move(y));
    arr.insert(30);

    EXPECT_EQ(arr.size(), 3);
    EXPECT_EQ(arr[0], 10);
    EXPECT_EQ(arr[1], 20);
    EXPECT_EQ(arr[2], 30);
}
TEST(DynamicArrayTest, InsertAtIndex) {
    DynamicArray<int> arr;

    arr.insert(10);    // [10]
    arr.insert(30);    // [10, 30]
    arr.insert(1, 20); // [10, 20, 30]

    EXPECT_EQ(arr.size(), 3);
    EXPECT_EQ(arr[0], 10);
    EXPECT_EQ(arr[1], 20);
    EXPECT_EQ(arr[2], 30);

    arr.insert(0, 5); // [5, 10, 20, 30]
    EXPECT_EQ(arr[0], 5);
    EXPECT_EQ(arr.size(), 4);
}
TEST(DynamicArrayTest, RemoveElement) {
    DynamicArray<int> arr;

    for (int i = 0; i < 5; ++i) {
        arr.insert(i * 10);
    }
    // [0, 10, 20, 30, 40]

    arr.remove(2);
    // [0, 10, 30, 40]

    EXPECT_EQ(arr.size(), 4);
    EXPECT_EQ(arr[0], 0);
    EXPECT_EQ(arr[1], 10);
    EXPECT_EQ(arr[2], 30);
    EXPECT_EQ(arr[3], 40);

    arr.remove(0); // [10, 30, 40]
    EXPECT_EQ(arr.size(), 3);
    EXPECT_EQ(arr[0], 10);

    arr.remove(arr.size() - 1); // [10, 30]
    EXPECT_EQ(arr.size(), 2);
    EXPECT_EQ(arr[0], 10);
    EXPECT_EQ(arr[1], 30);
}
TEST(DynamicArrayTest, ModifyElements) {
    DynamicArray<int> arr;

    arr.insert(1);
    arr.insert(2);
    arr.insert(3);

    arr[0] = 100;
    arr[1] = 200;
    arr[2] = 300;

    EXPECT_EQ(arr[0], 100);
    EXPECT_EQ(arr[1], 200);
    EXPECT_EQ(arr[2], 300);
}



TEST(DynamicArrayTest, ClearArray) {
    DynamicArray<int> arr;

    for (int i = 0; i < 10; ++i) {
        arr.insert(i);
    }

    EXPECT_EQ(arr.size(), 10);

    arr.clear();
    EXPECT_EQ(arr.size(), 0);
    
    arr.insert(100);
    EXPECT_EQ(arr.size(), 1);
    EXPECT_EQ(arr[0], 100);
}
TEST(DynamicArrayTest, InsertAccessRemoveString) {
    DynamicArray<std::string> arr;
    std::string hello = "Hello";
    std::string test = "Test";
    arr.insert(hello);
    arr.insert("World");
    arr.insert(std::move(test));
    EXPECT_NE(test, "Test");

    EXPECT_EQ(arr.size(), 3);
    EXPECT_EQ(arr[0], "Hello");
    EXPECT_EQ(arr[1], "World");
    EXPECT_EQ(arr[2], "Test");

    arr.insert(1, "Beautiful");
    EXPECT_EQ(arr[0], "Hello");
    EXPECT_EQ(arr[1], "Beautiful");
    EXPECT_EQ(arr[2], "World");
    EXPECT_EQ(arr[3], "Test");

    arr.remove(2);
    EXPECT_EQ(arr.size(), 3);
    EXPECT_EQ(arr[0], "Hello");
    EXPECT_EQ(arr[1], "Beautiful");
    EXPECT_EQ(arr[2], "Test");
}

TEST(DynamicArrayTest, ModifyElementsString) {
    DynamicArray<std::string> arr;

    arr.insert("cat");
    arr.insert("dog");
    arr.insert("bird");

    arr[0] = "lion";
    arr[1] = "tiger";
    arr[2] = "bear";

    EXPECT_EQ(arr[0], "lion");
    EXPECT_EQ(arr[1], "tiger");
    EXPECT_EQ(arr[2], "bear");

    arr[0] += " king";
    EXPECT_EQ(arr[0], "lion king");
}
//Добавить все тесты стринг
#pragma endregion