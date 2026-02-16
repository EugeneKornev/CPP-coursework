#include "avl.hpp"
#include <iostream>
#include <cassert>


void print(std::vector<std::string> vec) {
    for (int i = 0; i < vec.size(); i++) {
        std::cout << vec[i] << " ";
    }
    std::cout << std::endl;
}

bool eq_vectors(const std::vector<std::string>& v1, const std::vector<std::string>& v2) {
    int min_len = std::min(v1.size(), v1.size());
    for (int i = 0; i < min_len; i++) {
        if (v1[i] != v2[i]) {
            return false;
        }
    }
    return true;
}

void case1() {
    AVL* avl = new AVL;
    avl->insert(20);
    avl->insert(4);
    assert(eq_vectors(avl->serialize(), std::vector<std::string>({"20", "4", "null", "null", "null"})));
    avl->insert(15);
    assert(eq_vectors(avl->serialize(), std::vector<std::string>{"15", "4", "null", "null", "20", "null", "null"}));
    delete avl;
    return;
}

void case2() {
    AVL* avl = new AVL;
    avl->insert(20);
    avl->insert(26);
    avl->insert(4);
    avl->insert(3);
    avl->insert(9);
    assert(eq_vectors(avl->serialize(), std::vector<std::string>{"20", "4", "3", "null", "null", "9", "null", "null", "26", "null", "null"}));
    avl->insert(15);
    assert(eq_vectors(avl->serialize(), std::vector<std::string>{"9", "4", "3", "null", "null", "null", "20", "15", "null", "null", "26", "null", "null"}));
    delete avl;
    return;
}


void case3() {
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
    assert(eq_vectors(avl->serialize(), std::vector<std::string>{"20", "4", "3", "2", "null", "null", "null", "9", "7", "null", "null", "11", "null", "null", "26", "21", "null", "null", "30", "null", "null"
}));
    avl->insert(15);
    assert(eq_vectors(avl->serialize(), std::vector<std::string>{"9", "4", "3", "2", "null", "null", "null", "7", "null", "null", "20", "11", "null", "15", "null", "null", "26", "21", "null", "null", "30", "null", "null"}));
    delete avl;
    return;
}


int main() {
    case1();
    case2();
    case3();
}

