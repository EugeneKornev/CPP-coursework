#pragma once
#include <vector>
#include <string>
#include <queue>
#include <memory>
#include <initializer_list>
#include <utility>

template <typename T>
class AVL{
    public:
    
        AVL() : root_(nullptr) {}

        AVL(std::initializer_list<std::pair<int, T>> init_list) : root_(nullptr) {
            for (const auto& i : init_list) {
                insert(i.first, i.second);
            }
        }
    
        ~AVL() = default;
    
        AVL(const AVL& other) : root_(copy_(other.root_, std::shared_ptr<Node>(nullptr))) {}
    
        AVL(AVL&& other) {
            root_ = std::move(other.root_);
        }
    
        AVL& operator=(const AVL& other) {
            if (this != &other) {
                AVL temp{other};
                std::swap(root_, temp.root_);
            }
            return *this;
        }

        AVL& operator=(AVL&& other) {
            if (this != &other) {
                root_ = std::move(other.root_);
            }
            return *this;
        }

        void insert(int key, T value) {
            root_ = insert_(root_, key, value, nullptr);
        }

        
        void remove(int key) {
            root_ = remove_(root_, key);
        }

        
        T value_or(int key, T default_) const {
            std::shared_ptr<Node> current = root_;
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
            std::shared_ptr<Node> left_;
            std::shared_ptr<Node> right_;
            std::weak_ptr<Node> parent_;
            int height_;

            Node(int key, T value, std::shared_ptr<Node> parent) :
             key_(key), value_(value), parent_(parent), height_(1) {}
    
            ~Node() = default;
    
            void update() {
                height_ = 1 + std::max(left_ != nullptr ? left_->height_ : 0,
                                     right_!= nullptr ? right_->height_ : 0);
            }
    
            int balance_factor() {
                return (left_ != nullptr ? left_->height_ : 0) - (right_ != nullptr ? right_->height_ : 0);
            }
    
        };

        std::shared_ptr<Node> root_;

    public:

        class iterator {
        private:
            std::queue<T> nodes;

            void make_list(std::shared_ptr<Node> node) {
                if (node) {
                    make_list(node->left_);
                    nodes.push(node->value_);
                    make_list(node->right_);
                }
            }
        public:

            iterator(std::shared_ptr<Node> node) {
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

            iterator operator++(int) {
                iterator tmp = *this;
                nodes.pop();
                return tmp;
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

        bool tree_compare_(std::shared_ptr<Node> node, const AVL& other) {
            if (node) {
                return (bool) other.value_or(node->key_, 0) && tree_compare_(node->left_, other) && tree_compare_(node->right_, other);
            }
            return true;
        }
    
        
        std::shared_ptr<Node> copy_(std::shared_ptr<Node> node, std::shared_ptr<Node> parent) {
            if (node == nullptr) {
                return nullptr;
            }
            auto new_node = std::make_shared<Node>(node->key_, node->value_, parent);
            new_node->height_ = node->height_;
            new_node->left_ = copy_(node->left_, new_node);
            new_node->right_ = copy_(node->right_, new_node);
            return new_node;
        }

        
        std::shared_ptr<Node> insert_(std::shared_ptr<Node> node, int key, T value, std::shared_ptr<Node> parent) {
            if (!node) {
                return std::make_shared<Node>(key, value, parent);
            }
            if (key < node->key_) {
                node->left_ = insert_(node->left_, key, value, node);
            } else if (key > node->key_) {
                node->right_ = insert_(node->right_, key, value, node);
            } else {
                node->value_ = value; 
                return node;
            }

            node->update();
            return balance_(node);
        }
    

        std::shared_ptr<Node> balance_(std::shared_ptr<Node> node) {
            int bf = node->balance_factor();
            if (bf > 1) {
                if (node->left_->balance_factor() < 0) {
                    node->left_ = left_rotate_(node->left_);
                }
                return right_rotate_(node);
            }
            if (bf < -1) {
                if (node->right_->balance_factor() > 0) {
                    node->right_ = right_rotate_(node->right_);
                }
                return left_rotate_(node);
            }
            return node;
        }
    

        std::shared_ptr<Node> right_rotate_(std::shared_ptr<Node> node) {
            std::shared_ptr<Node> y = node->left_; // b
            std::shared_ptr<Node> B = y->right_; // C

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
    

        std::shared_ptr<Node> left_rotate_(std::shared_ptr<Node> node) {
            std::shared_ptr<Node> y = node->right_; // b
            std::shared_ptr<Node> B = y->left_; // C

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

    
        std::shared_ptr<Node> remove_(std::shared_ptr<Node> node, int key) {
            if (node == nullptr) {
                return nullptr;
            }
            if (key < node->key_) {
                node->left_ = remove_(node->left_, key);
            } else if (key > node->key_) {
                node->right_ = remove_(node->right_, key);
            } else {
                if (!node->left_ || !node->right_) {
                    auto temp = node->left_ ? node->left_ : node->right_;
                    if (temp) {
                        temp->parent_ = node->parent_;
                    }
                    return temp;
                }
                auto temp = min_node_(node->right_);
                node->key_ = temp->key_;
                node->value_ = temp->value_;
                node->right_ = remove_(node->right_, temp->key_);
            }
            node->update();
            return balance_(node);
        }

    
        std::shared_ptr<Node> min_node_(std::shared_ptr<Node> node) {
            while (node->left_) {
                node = node->left_;
            }
            return node;
        }

            
        std::vector<std::string> serialize_(std::shared_ptr<Node> node) {
            if (!node) {
                return {"null"};
            }
            std::vector<std::string> res = {std::to_string(node->key_)};
            auto left = serialize_(node->left_);
            auto right = serialize_(node->right_);
            res.insert(res.end(), left.begin(), left.end());
            res.insert(res.end(), right.begin(), right.end());
            return res;
        }
};
