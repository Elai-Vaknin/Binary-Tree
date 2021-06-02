#pragma once
#include <stack>
#include <iostream>
#include <vector>
#include <array>

using namespace std;

namespace ariel {
    enum Types {
        IN_ORDER = 0,
        POST_ORDER,
        PRE_ORDER,
        TYPES
    };

    template<class T>
    struct Node {
        T* value;
        Node *left;
        Node *right;

        Node(const Node &other) {
            this->value = new T(other.value);
            this->left = new Node<T>(other.left);
            this->right = new Node<T>(other.right);
        }

        Node& operator =(const Node &other) {
            if(this == &other) {
                return *this;
            }

            this->value = new T(other.value);
            this->left = new Node<T>(other.left);
            this->right = new Node<T>(other.right);

            return *this;
        }

        Node(Node&& source) noexcept : value(source.value), left(source.left), right(source.right) { }
        Node& operator =(Node&& other) noexcept {
            if(this == &other) {
                return *this;
            }

            this->value = other.value;
            this->left = other.left;
            this->right = other.right;

            return *this;
        }

        Node(T v) : left(nullptr), right(nullptr) {
            value = new T(v);
        }

        ~Node() {
            if(value) {
                delete value;
            }
        }
    };

    template<class T>
    class BinaryTree {
    private:
        Node<T>* root;
        array<vector<T*>, Types::TYPES> order;

        Node<T> *search(T value) {
            stack<Node<T> *> stack;

            stack.push(this->root);

            while (!stack.empty()) {
                Node<T>* temp = stack.top();
                stack.pop();

                if (!temp) {
                    continue;
                }

                if (*(temp->value) == value) {
                    return temp;
                }

                stack.push(temp->left);
                stack.push(temp->right);
            }

            return nullptr;
        }

    public:
        BinaryTree<T>() {
            root = nullptr;
        }

        BinaryTree<T> &add_root(T v) {
            if (!root) {
                root = new Node<T>(v);
            }
            else {
                *(this->root->value) = v;
            }

            return *this;
        }

        BinaryTree<T> &add_left(T value, T left_value) {
            Node<T> *root = search(value);

            if (root) {
                if (root->left) {
                    *(root->left->value) = left_value;
                }
                else {
                    Node<T> *left = new Node<T>(left_value);

                    root->left = left;
                }
            } else {
                throw invalid_argument("Error");
            }
            return *this;
        }

        BinaryTree<T> &add_right(T value, T right_value) {
            Node<T> *root = search(value);

            if (root) {
                if (root->right) {
                    *(root->right->value) = right_value;
                }
                else {
                    Node<T> *right = new Node<T>(right_value);

                    root->right = right;
                }
            } else {
                throw invalid_argument("Error");
            }
            return *this;
        }

        friend ostream &operator<<(ostream &os, const BinaryTree<T> &tree) {
            stack<Node<T> *> stack;

            stack.push(tree.root);

            Node<T> *top = nullptr;

            while (!stack.empty()) {
                top = stack.top();
                stack.pop();

                os << *top->value << "<";

                if (top->left) {
                    os << *top->left->value;
                    stack.push(top->left);
                } else {
                    os << "None";
                }

                os << ",";

                if (top->right) {
                    os << *top->right->value;
                    stack.push(top->right);
                } else {
                    os << "None";
                }

                os << ">" << endl;
            }

            return os;
        }

        void load_order(Node<T>* root, Types t, bool flag = false) {
            if (!flag) {
                order[t].clear();

                flag = true;
            }

            switch (t) {
                case Types::IN_ORDER: {

                    if (root) {
                        load_order(root->left, t, flag);

                        order[t].push_back(root->value);

                        load_order(root->right, t, flag);
                    }

                    break;
                }

                case Types::POST_ORDER: {
                    if (root) {
                        load_order(root->left, t, flag);
                        load_order(root->right, t, flag);

                        order[t].push_back(root->value);
                    }

                    break;
                }

                case Types::PRE_ORDER: {
                    if (root) {
                        order[t].push_back(root->value);

                        load_order(root->left, t, flag);
                        load_order(root->right, t, flag);
                    }

                    break;
                }

                default: break;
            }
        }

        struct Iterator
        {
        public:
            Iterator(T* ptr, vector<T*> v) : m_ptr(ptr), vec(v) {
                index = 0;
            }

            T& operator*() const { return *m_ptr; }
            T* operator->() { return m_ptr; }
            Iterator& operator++() { m_ptr = vec.at(++index); return *this; }
            Iterator operator++(int) { Iterator tmp = *this; ++(*this); return tmp; }
            friend bool operator== (const Iterator& a, const Iterator& b) { return a.m_ptr == b.m_ptr; };
            friend bool operator!= (const Iterator& a, const Iterator& b) { return a.m_ptr != b.m_ptr; };

        private:
            T* m_ptr;
            vector<T*> vec;
            unsigned int index;
        };

        /* BinaryTree class */
        Iterator begin_order(const Types t) {
            load_order(this->root, t);

            order[t].push_back(nullptr);

            return Iterator(order.at(t).at(0), order.at(t));
        }

        Iterator end_order(const Types t) {
            return Iterator(nullptr, order.at(t));
        }

        Iterator begin() {
            return begin_order(Types::IN_ORDER);
        }

        Iterator end() {
            return end_order(Types::IN_ORDER);
        }

        Iterator begin_preorder() {
            return begin_order(Types::PRE_ORDER);
        }

        Iterator begin_postorder() {
            return begin_order(Types::POST_ORDER);
        }

        Iterator begin_inorder() {
            return begin_order(Types::IN_ORDER);
        }

        Iterator end_preorder() {
            return end_order(Types::PRE_ORDER);
        }

        Iterator end_inorder() {
            return end_order(Types::IN_ORDER);
        }

        Iterator end_postorder() {
            return end_order(Types::POST_ORDER);
        }
    };
}
