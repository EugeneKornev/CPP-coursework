#pragma once
#include <vector>
#include <string>


class AVL{
public:
    AVL();
    ~AVL();
    AVL(const AVL& other);
    AVL(AVL&& other);
    AVL& operator=(const AVL& other);
    AVL& operator=(AVL&& other);

    void insert(int key);
    void remove(int key);
    int value_or(int key, int default_) const;
    bool equals(const AVL& other);
    std::vector<std::string> serialize();
    
private:

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

    
    Node* root_;
    bool tree_compare_(Node* node, const AVL& other);
    void destruct_(Node* node);
    void copy_(AVL& this_, AVL::Node* node);
    Node* insert_(Node* node, Node* new_node);
    Node* balance_(Node* node);
    Node* right_rotate_(Node* node);
    Node* left_rotate_(Node* node);
    Node* remove_(Node* node, int key);
    Node* min_node_(Node* node);
    std::vector<std::string> serialize_(Node* node);
};
