#pragma once
#include <vector>
#include <string>
#include <queue>

template <typename T>
class AVL{
    public:
    
        AVL() {
            root_ = nullptr;
        }
    
        ~AVL() {
            destruct_(root_);
        }
    
        AVL(const AVL& other) : root_(copy_(other.root_, nullptr)) {}
    
        AVL(AVL&& other) {
            this->root_ = other.root_;
            other.root_ = nullptr;
        }
    
        AVL& operator=(const AVL& other) {
            AVL tree{other};
            std::swap(root_, tree.root_);
            return *this;

        }
        AVL& operator=(AVL&& other) {
            if (this != &other) {
                destruct_(root_);
                root_ = other.root_;
                other.root_ = nullptr;
            }
            return *this;
        }

        void insert(int key, T value) {
            root_ = insert_(root_, new Node{key, value, nullptr});
        }

        
        void remove(int key) {
            root_ = remove_(root_, key);
        }

        
        T value_or(int key, T default_) const {
            Node* current = root_;
            while (current != nullptr) {
                if (key < current->key_) {
                    current = current->left_;
                } else if (key > current->key_) {
                    current = current->right_;
                } else {
                    return current->value_;
                }
            }
            return default_;
        }
        
        bool equals(const AVL& other) {
            return tree_compare_(this->root_, other);
        }

        
        std::vector<std::string> serialize() {
            return serialize_(root_);
        }
    
    private:

        struct Node {
            int key_;
            T value_;
            Node* left_;
            Node* right_;
            Node* parent_;
            int height_;

            Node(int key, T value, Node* parent) {
                key_ = key;
                value_ = value;
                height_ = 1;
                left_ = nullptr;
                right_ = nullptr;
                parent_ = parent;
            }
    
            ~Node() {}
    
            void update() {
                height_ = 1 + std::max(left_ != nullptr ? left_->height_ : 0, right_!= nullptr ? right_->height_ : 0);
            }
    
            int balance_factor() {
                return (left_ != nullptr ? left_->height_ : 0) - (right_ != nullptr ? right_->height_ : 0);
            }
    
        };

        Node* root_;

    public:

        class iterator {
        private:
            std::queue<T> nodes;

            void make_list(Node* node) {
                if (node) {
                    make_list(node->left_);
                    nodes.push(node->value_);
                    make_list(node->right_);
                }
            }
        public:

            iterator(Node* node) {
                make_list(node);
            }

            iterator() {}

            bool operator==(const iterator other) const {
                if (nodes.empty() && other.nodes.empty()) {
                    return true;
                } else if (nodes.empty() || other.nodes.empty()) {
                    return false;
                } else {
                    return nodes.front() == other.nodes.front();
                }
            }


            bool operator!=(const iterator other) const {
                return !(*this == other);
            }

            iterator& operator++() {
                nodes.pop();
                return *this;
            }

            iterator& operator++(int) {
                auto tmp = this;
                nodes.pop();
                return *tmp;
            }

            T& operator*() {
                return nodes.front();
            }
            
        };

        iterator begin() {
            return iterator(root_);;
        }

        iterator end() {
            return iterator(nullptr);
        }

    private:

        bool tree_compare_(Node* node, const AVL& other) {
            if (node) {
                return (bool) other.value_or(node->key_, 0) && tree_compare_(node->left_, other) && tree_compare_(node->right_, other);
            }
            return true;
        }

        
        void destruct_(Node* node) {
            if (node) {
                destruct_(node->left_);
                destruct_(node->right_);
                delete node;
            }
        }
    
        
        Node* copy_(Node* node, Node* parent) {
            if (node == nullptr) {
                return nullptr;
            }
            Node* new_node = new Node(node->key_, node->value_, parent);
            new_node->height_ = node->height_;
            new_node->left_ = copy_(node->left_, new_node);
            new_node->right_ = copy_(node->right_, new_node);
            return new_node;
        }

        
        Node* insert_(Node* node, Node* new_node) {
            if (!node) {
                return balance_(new_node); 
            }
            if (new_node->key_ < node->key_) {
                node->left_ = insert_(node->left_, new_node);
                if (node->left_) {
                    node->left_->parent_ = node;
                }
            } else if (new_node->key_ > node->key_) {
                node->right_ = insert_(node->right_, new_node);
                if (node->right_) {
                    node->right_->parent_ = node;
                }
            } else {
                delete new_node;
                return node;
            }
            node->update();
            Node* new_root = balance_(node);
            if (new_root != node) {
                new_root->parent_ = node->parent_;
            }
            return new_root;
        }
    
        Node* balance_(Node* node) {
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
    
        Node* right_rotate_(Node* node) {
            Node* y = node->left_; // b
            Node* B = y->right_; // C

            y->parent_ = node->parent_;
            node->parent_ = y;
            if (B) {
                B->parent_ = node;
            }



            y->right_ = node; // b.right = a
            node->left_ = B; // a.left = C
            node->update(); // a.update()
            y->update(); // b.update()
            return y;
        }
    
        Node* left_rotate_(Node* node) {
            Node* y = node->right_; // b
            Node* B = y->left_; // C

            y->parent_ = node->parent_;
            node->parent_ = y;
            if (B) {
                B->parent_ = node;
            }

            node->right_ = B; // a.right = C
            y->left_ = node; // b.left = a
            node->update(); // a.update()
            y->update(); // b.update()
            return y;
        }

    
        Node* remove_(Node* node, int key) {
            if (!node) {
                return node;
            }
            if (key < node->key_) {
                node->left_ = remove_(node->left_, key);
                if (node->left_) {
                    node->left_->parent_ = node;
                }
            } else if (key > node->key_) {
                node->right_ = remove_(node->right_, key);
                if (node->right_) {
                    node->right_->parent_ = node;
                }
            } else {
                if (!node->left_) {
                    Node* right_child = node->right_;
                    if (right_child) {
                        right_child->parent_ = node->parent_;
                    }
                    delete node; 
                    return right_child;
                }
                if (!node->right_) {
                    Node* left_child = node->left_;
                    if (left_child) {
                        left_child->parent_ = node->parent_;
                    }
                    delete node;
                    return left_child;
                }
                Node* temp = min_node_(node->right_);
                node->key_ = temp->key_;
                node->value_ = temp->value_;
                node->right_ = remove_(node->right_, temp->key_);
            }
            node->update();
            Node* new_root = balance_(node);
            if (new_root != node) {
                new_root->parent_ = node->parent_;
            }
            return new_root;
        }

    
        Node* min_node_(Node* node) {
            Node* current = node;
            while (current->left_) {
                current = current->left_;
            }
            return current;
        }

            
        std::vector<std::string> serialize_(Node* node) {
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
};
