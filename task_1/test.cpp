#include "avl.hpp"
#include <iostream>
#include <cassert>
#include <gtest/gtest.h>
#include <set>


void print(std::vector<std::string> vec) {
    for (int i = 0; i < vec.size(); i++) {
        std::cout << vec[i] << " ";
    }
    std::cout << std::endl;
}

bool eq_vectors(const std::vector<std::string>& v1, const std::vector<std::string>& v2) {
    int min_len = std::min(v1.size(), v2.size());
    for (int i = 0; i < min_len; i++) {
        if (v1[i] != v2[i]) {
            return false;
        }
    }
    return true;
}

bool eq_sets_from_vectors(const std::vector<std::string>& v1, const std::vector<std::string>& v2) {
    std::set<std::string> s1, s2;
    for (int i = 0; i < v1.size(); i++) {
        if (v1[i] != "null") {
            s1.insert(v1[i]);
        }
    }
    for (int j = 0; j < v2.size(); j++) {
        if (v2[j] != "null") {
            s2.insert(v2[j]);
        }
    }
    return s1 == s2;
}

TEST(AVL, RightRotate) {
    AVL* avl = new AVL;
    avl->insert(20);
    avl->insert(4);
    ASSERT_TRUE(eq_vectors(avl->serialize(), std::vector<std::string>({"20", "4", "null", "null", "null"})));
    avl->insert(15);
    ASSERT_TRUE(eq_vectors(avl->serialize(), std::vector<std::string>{"15", "4", "null", "null", "20", "null", "null"}));
    delete avl;
    return;

}

TEST(AVL, BigRightRotate) {
    AVL* avl = new AVL;
    avl->insert(20);
    avl->insert(26);
    avl->insert(4);
    avl->insert(3);
    avl->insert(9);
    ASSERT_TRUE(eq_vectors(avl->serialize(), std::vector<std::string>{"20", "4", "3", "null", "null", "9", "null", "null", "26", "null", "null"}));
    avl->insert(15);
    ASSERT_TRUE(eq_vectors(avl->serialize(), std::vector<std::string>{"9", "4", "3", "null", "null", "null", "20", "15", "null", "null", "26", "null", "null"}));
    delete avl;
    return;
}

TEST(AVL, BigRightRotate2) {
    AVL* avl = new AVL;
    avl->insert(20);
    avl->insert(4);
    avl->insert(26);
    avl->insert(3);
    avl->insert(9);
    avl->insert(21);
    avl->insert(30);
    avl->insert(2);
    avl->insert(7);
    avl->insert(11); 
    ASSERT_TRUE(eq_vectors(avl->serialize(), std::vector<std::string>{"20", "4", "3", "2", "null", "null", "null", "9", "7", "null", "null", "11", "null", "null", "26", "21", "null", "null", "30", "null", "null"
}));
    avl->insert(15);
    ASSERT_TRUE(eq_vectors(avl->serialize(), std::vector<std::string>{"9", "4", "3", "2", "null", "null", "null", "7", "null", "null", "20", "11", "null", "15", "null", "null", "26", "21", "null", "null", "30", "null", "null"}));
    delete avl;
    return;
}

TEST(AVL, Remove) {
    AVL* avl = new AVL;
    avl->insert(20);
    avl->insert(4);
    avl->insert(26);
    avl->insert(3);
    avl->insert(9);
    avl->insert(21);
    avl->insert(30);
    avl->insert(2);
    avl->insert(7);
    avl->insert(11); 
    avl->insert(15);
    ASSERT_TRUE(eq_vectors(avl->serialize(), std::vector<std::string>{"9", "4", "3", "2", "null", "null", "null", "7", "null", "null", "20", "11", "null", "15", "null", "null", "26", "21", "null", "null", "30", "null", "null"}));
    avl->remove(21);
    ASSERT_TRUE(eq_vectors(avl->serialize(), std::vector<std::string>{"9", "4", "3", "2", "null", "null", "null", "7", "null", "null", "20", "11", "null", "15", "null", "null", "26", "null", "30", "null", "null"}));
    avl->remove(7);
    ASSERT_TRUE(eq_vectors(avl->serialize(), std::vector<std::string>{"9", "3", "2", "null", "null", "4", "null", "null", "20", "11", "null", "15", "null", "null", "26", "null", "30", "null", "null"}));
    delete avl;

}

TEST(AVL, CopyCtr) {
    AVL* avl1 = new AVL;
    avl1->insert(20);
    avl1->insert(4);
    avl1->insert(26);
    avl1->insert(3);
    avl1->insert(9);
    avl1->insert(21);
    avl1->insert(30);
    avl1->insert(2);
    avl1->insert(7);
    avl1->insert(11); 
    avl1->insert(15); 
    AVL& avl1_ref = *avl1;
    AVL avl2 = avl1_ref;
    ASSERT_TRUE(avl1->equals(avl2));
    delete avl1;
}


TEST(AVL, CopyAssignOper) {
    AVL avl1;
    avl1.insert(20);
    avl1.insert(4);
    avl1.insert(26);
    avl1.insert(3);
    avl1.insert(9);
    avl1.insert(21);
    avl1.insert(30);
    avl1.insert(2);
    avl1.insert(7);
    avl1.insert(11); 
    avl1.insert(15);
    AVL avl2;
    avl2.insert(20);
    avl2.insert(26);
    avl2.insert(4);
    avl2.insert(3);
    avl2.insert(9);
    avl2.insert(15);
    avl2 = avl1;
    ASSERT_TRUE(avl1.equals(avl2));
}


TEST(AVL, MoveCtr) {
    AVL avl1;
    avl1.insert(20);
    avl1.insert(4);
    avl1.insert(26);
    avl1.insert(3);
    avl1.insert(9);
    avl1.insert(21);
    avl1.insert(30);
    avl1.insert(2);
    avl1.insert(7);
    avl1.insert(11); 
    avl1.insert(15);
    AVL avl2(avl1);
    ASSERT_TRUE(eq_sets_from_vectors(avl1.serialize(), avl2.serialize()));
}


TEST(AVL, MoveAssignOper) {
    AVL avl1;
    avl1.insert(20);
    avl1.insert(4);
    avl1.insert(26);
    avl1.insert(3);
    avl1.insert(9);
    avl1.insert(21);
    avl1.insert(30);
    avl1.insert(2);
    avl1.insert(7);
    avl1.insert(11); 
    avl1.insert(15);
    AVL avl2;
    avl2.insert(20);
    avl2.insert(26);
    avl2.insert(4);
    avl2.insert(3);
    avl2.insert(9);
    avl2.insert(15);
    avl2 = std::move(avl1);
    ASSERT_TRUE(eq_vectors(avl1.serialize(), std::vector<std::string>({"null"})));
}


int main() {
    testing::InitGoogleTest();
    bool s = RUN_ALL_TESTS();
}

