#include "singleton.hpp"
#include <gtest/gtest.h>

TEST(Singleton, T1) {
    SingletonPair<int, char> p1 = SingletonPair<int, char>{12, 24};
    try {
        SingletonPair<int, char> p1 = SingletonPair<int, char>{13, 26};
        FAIL()  << "didn't cathed";
    } catch (TooMuchInstances& exception) {
        SUCCEED() << "hurray!";
    }  
}


