#pragma once
#include <vector>
#include <string>

struct Node {
    int key_;
    int value_;
    Node* left_;
    Node* right_;
    int height_;

    Node(int key);
    ~Node();
    
    void update();
    int balance_factor();
};

class AVL{
public:
    AVL();
    ~AVL();

    void insert(int key);
    void remove(int key);
    int value_or(int key, int default_);
    std::vector<std::string> serialize();
    
private:
    Node* root_;
    void destruct_(Node* node);
    Node* insert_(Node* node, Node* new_node);
    Node* balance_(Node* node);
    Node* right_rotate_(Node* node);
    Node* left_rotate_(Node* node);
    Node* remove_(Node* node, int key);
    Node* min_node_(Node* node);
    std::vector<std::string> serialize_(Node* node);
};
