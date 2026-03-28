#include "matrix.hpp"
#include "gtest/gtest.h"
#include <vector>
//#include <iostream>


TEST(CoreFunc, CtrAndDstr) {
    Matrix* m = new Matrix(10);
    delete m;
}

TEST(CoreFunc, CtrFromVector) {
    Matrix m(std::vector<double>({12.35, 74, 694.357}));
}

TEST(CoreFunc, DoubleCast) {
    Matrix m(std::vector<double>({12.35, 74, 694.357}));
    double sum = static_cast<double>(m);
    double res = 780.707;
    EXPECT_DOUBLE_EQ(sum, res);
}

TEST(CoreFunc, Zero) {
    Matrix m(7);
    double sum = static_cast<double>(m);
    ASSERT_DOUBLE_EQ(sum, 0.0);
    
}

TEST(CoreFunc, Operator) {
    Matrix m(10);
    m[5][5] = 42;
    ASSERT_DOUBLE_EQ(static_cast<double>(m), 42);
}

TEST(CoreFunc, OutOfBound) {
    Matrix m(4);
    ASSERT_THROW(m[7][2], std::runtime_error);
}

TEST(CoreFunc, CopyCtr) {
    Matrix m1 = Matrix(std::vector<double>({0.85, 0.34, 0.86, 0.84, 0.78, 0.34, 0.26, 0.83, 0.62, 0.46}));
    Matrix m2 = m1;
    ASSERT_TRUE(m1 == m2);
}

TEST(CoreFunc, MovCtr) {
    Matrix m1 = Matrix(std::vector<double>({0.85, 0.34, 0.86, 0.84, 0.78, 0.34, 0.26, 0.83, 0.62, 0.46}));
    Matrix m2 = std::move(m1);
    ASSERT_THROW(double t = m1[2][2], std::runtime_error);
}

TEST(CoreFunc, CopyAssign) {
    Matrix m1(std::vector<double>({1.3, 2.4, 3.5, 4.6, 5.7}));
    Matrix m2(12);
    m1 = m2;
    ASSERT_TRUE(m1 == m2);
}


TEST(CoreFunc, MoveAssign) {
    Matrix m1(std::vector<double>({1.3, 2.4, 3.5, 4.6, 5.7}));
    Matrix m2(12);
    m2 = std::move(m1);;
    ASSERT_THROW(m1[0][0], std::runtime_error);
}

TEST(CoreFunc, IAdd) {
    Matrix m1(std::vector<double>({1, 3, 5, 7, 9}));
    Matrix m2(std::vector<double>({2, 4, 6, 8, 10}));
    m1 += m2;
    ASSERT_TRUE(m1 == Matrix(std::vector<double>({3, 7, 11, 15, 19})));
}

TEST(CoreFunc, Add) {
    Matrix m1(std::vector<double>({1, 3, 5, 7, 9}));
    Matrix m2(std::vector<double>({2, 4, 6, 8, 10}));
    Matrix m3 = m1 + m2;
    ASSERT_TRUE(m3 == Matrix(std::vector<double>({3, 7, 11, 15, 19})));
}


TEST(CoreFunc, IMul) {
    Matrix m1(std::vector<double>({1, 14}));
    Matrix m2(std::vector<double>({7, 3}));
    m1[0][1] = 5;
    m1[1][0] = 12;
    m2[0][1] = 8;
    m2[1][0] = 6;
    m1 *= m2;
    Matrix m3(std::vector<double>({37, 138}));
    m3[0][1] = 23;
    m3[1][0] = 168;
    ASSERT_TRUE(m1 == m3);
}

TEST(CoreFunc, Mul) {
    Matrix m1(std::vector<double>({1, 14}));
    Matrix m2(std::vector<double>({7, 3}));
    m1[0][1] = 5;
    m1[1][0] = 12;
    m2[0][1] = 8;
    m2[1][0] = 6;
    Matrix m3 = m1 * m2;
    Matrix m4(std::vector<double>({37, 138}));
    m4[0][1] = 23;
    m4[1][0] = 168;
    ASSERT_TRUE(m3 == m4);
}


TEST(CoreFunc, IMulImm) {
    Matrix m(std::vector<double>({14, 50}));
    m *= 7;
    ASSERT_TRUE(m == Matrix(std::vector<double>({98, 350})));
}

TEST(CoreFunc, MulImm) {
    Matrix m1(std::vector<double>({13, 54}));
    Matrix m2 = m1 * 7;
    ASSERT_TRUE(m2 == Matrix(std::vector<double>({91, 378})));
}


int main() {
    testing::InitGoogleTest();
    bool succesful = RUN_ALL_TESTS();
    return succesful;
}