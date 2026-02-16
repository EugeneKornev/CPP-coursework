#include "avl.hpp"
#include <algorithm>

Node::Node(int key) {
          key_ = key;
          value_ = key;
          height_ = 1;
          left_ = nullptr;
          right_ = nullptr;
}


Node::~Node() {}

void Node::update() {
    height_ = 1 + std::max(left_ != nullptr ? left_->height_ : 0, right_!= nullptr ? right_->height_ : 0);
}

int Node::balance_factor() {
    return (left_ != nullptr ? left_->height_ : 0) - (right_ != nullptr ? right_->height_ : 0);
}

AVL::AVL() {
    root_ = nullptr;
}

AVL::~AVL() {
    destruct_(root_);
}

void AVL::destruct_(Node* node) {
        if (node) {
            destruct_(node->left_);
            destruct_(node->right_);
            delete node;
        }
    }

Node* AVL::insert_(Node* node, Node* new_node) {
    if (!node) {
        return balance_(new_node); 
    }
    if (new_node->key_ < node->key_) {
        node->left_ = insert_(node->left_, new_node);
    } else if (new_node->key_ > node->key_) {
        node->right_ = insert_(node->right_, new_node);
    }
    node->update();
    return balance_(node);
}

Node* AVL::balance_(Node* node) {
    if (!node) {
        return node;
    }
    int balance = node->balance_factor();
    if (balance > 1) {
        if (node->left_->balance_factor() < 0) {
            node->left_ = left_rotate_(node->left_);
        }
        return right_rotate_(node);
    } else if (balance < -1) {
        if (node->right_->balance_factor() > 0) {
            node->right_ = right_rotate_(node->right_);
        }
        return left_rotate_(node);
    }
    return node;
}

Node* AVL::right_rotate_(Node* node) {
    Node* y = node->left_;
    Node* B = y->right_;
    y->right_ = node;
    node->left_ = B;
    node->update();
    y->update();
    return y;
}

Node* AVL::left_rotate_(Node* node) {
    Node* y = node->right_;
    Node* B = y->left_;
    node->right_ = B;
    y->left_ = node;
    node->update();
    y->update();
    return y;
}

Node* AVL::remove_(Node* node, int key) {
    if (!node) {
        return node;
    }
    if (key < node->key_) {
        node->left_ = remove_(node->left_, key);
    } else if (key > node->key_) {
        node->right_ = remove_(node->right_, key);
    } else {
        if (!node->left_) {
            Node* right_child = node->right_;
            delete node; 
            return right_child;
        }
        if (!node->right_) {
            Node* left_child = node->left_;
            delete node;
            return left_child;
        }
        Node* temp = min_node_(node->right_);
        node->key_ = temp->key_;
        //node->value_ = temp->value_;
        node->right_ = remove_(node->right_, temp->key_);
    }
    node->update();
    return balance_(node);
}

Node* AVL::min_node_(Node* node) {
    Node* current = node;
    while (current->left_) {
        current = current->left_;
    }
    return current;
}

void AVL::insert(int key) {
        root_ = insert_(root_, new Node{key});
    }

void AVL::remove(int key) {
    root_ = remove_(root_, key);
}

int AVL::value_or(int key, int default_) {
    Node* current = root_;
    while (current != nullptr) {
        if (key < current->key_) {
            current = current->left_;
        } else if (key > current->key_) {
            current = current->right_;
        } else {
            return current->key_;
        }
    }
    return default_;
}


std::vector<std::string> AVL::serialize_(Node* node) {
    if (node == nullptr) {
        return std::vector<std::string>{"null"};
    }
    std::vector<std::string> res;
    res.push_back(std::to_string(node->key_));
    std::vector<std::string> left_res = serialize_(node->left_);
    std::vector<std::string> right_res = serialize_(node->right_);
    res.insert(res.end(), left_res.begin(), left_res.end());
    res.insert(res.end(), right_res.begin(), right_res.end());
    return res;
}

std::vector<std::string> AVL::serialize() {
    return serialize_(root_);
}

