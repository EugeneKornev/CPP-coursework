#include "singleton.hpp"
#include <gtest/gtest.h>

TEST(Singleton, NotSingle) {
    SingletonPair<int, char> p1 = SingletonPair<int, char>{12, 24};
    ASSERT_THROW((SingletonPair<int, char>{13, 26}), TooMuchInstances);
}

TEST(Singleton, DifferentTypes) {
    SingletonPair<int, char> p1 = SingletonPair<int, char>{12, 24};
    ASSERT_NO_THROW((SingletonPair<std::string, std::string>{"Hello", "world"}));
}

TEST(Singleton, CopyCtr) {
    SingletonPair<int, int> p1 = SingletonPair<int, int>{56, 37};
    try {
        SingletonPair<int, int> p2 = p1;
    } catch (TooMuchInstances& exception) {
        SUCCEED();
        return;
    }
    FAIL();
}

TEST(Singleton, MoveCtr) {
    SingletonPair<int, std::string> p1 = SingletonPair<int, std::string>{124, "string"};
    try {
        SingletonPair<int, std::string> p2 = std::move(p1);
    } catch (TooMuchInstances& exception) {
        FAIL();
    }
    SUCCEED();
}

TEST(Doubleton, CopyAssign) {
    DoubletonPair<short, short> p1 = DoubletonPair<short, short>{354, 7546};
    DoubletonPair<short, short> p2 = DoubletonPair<short, short>{546, 756};
    try {
        p2 = p1;
    } catch (TooMuchInstances& exception) {
        FAIL();
    }
    SUCCEED();
}

