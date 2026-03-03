#include <iostream>
#include <type_traits>

template<typename T>
requires std::is_arithmetic_v<T>
struct Node {
    T value;
    Node<T>* parent;
    Node<T>* right;
    Node<T>* left;
    Node(T value)
            : value(value), left(nullptr), right(nullptr), parent(nullptr) {}
};

template<typename T>
requires std::is_arithmetic_v<T>
class BST {
private:
    Node<T>* root;

public:
    BST() {
        this->root = nullptr;
    }

    ~BST(){
        destroy(root);
    }

    Node<T>* getRoot() {
        return root;
    }

    Node<T>* search(T value, Node<T>* node) {
        if (node == nullptr || node->value == value)
            return node;
        if (value < node->value)
            return search(value, node->left);
        else
            return search(value, node->right);
    }

    Node<T>* treeMinimum(Node<T>* node){
        if (node == nullptr) return nullptr;

        while(node->left != nullptr)
        {
            node = node->left;
        }
        return node;
    }

    Node<T>* treeMaximum(Node<T>* node){
        if (node == nullptr) return nullptr;

        while(node->right != nullptr)
        {
            node = node->right;
        }
        return node;
    }

    Node<T>* treeSuccessor(Node<T>* node){
        if (node->right != nullptr) {
            return treeMinimum(node->right);
        }

        Node<T>* y = node->parent;
        while (y != nullptr && node == y->right) {
            node = y;
            y = y->parent;
        }
        return y;
    }



    void updateTree(Node<T>* node, T total, T& local_sum) {
        if (!node) return;

        updateTree(node->left, total, local_sum);
        local_sum += node->value;
        node->value += total - local_sum;
        updateTree(node->right, total, local_sum);
    }

    T sumSubtree(Node<T>* node){
        if(!node) return 0;
        return node->value + sumSubtree(node->left) + sumSubtree(node->right);
    }

    int countNodesSubtree(Node<T>* node){
        if(!node) return 0;
        return 1 + countNodesSubtree(node->left) + countNodesSubtree(node->right);
    }

    int countingOFNodesAverageSubtree(Node<T>* node){
        if (!node) return 0;
        int count = 0;
        int count_Nodes = countNodesSubtree(node);
        T sumSubtree1 = sumSubtree(node);
        if(node->value == sumSubtree1 / count_Nodes)
            count++;
        count += countingOFNodesAverageSubtree(node->left);
        count += countingOFNodesAverageSubtree(node->right);
        return count;
    }

    void insert(T value) {
        Node<T>* y = nullptr;
        Node<T>* x = root;
        Node<T>* z = new Node<T>(value);

        while (x!= nullptr) {
            y = x;
            if (z->value < x->value) {
                x = x->left;
            }
            else {
                x = x->right;
            }
        }

        z->parent = y;

        if (y == nullptr) {
            root = z;
        }
        else if (z->value < y->value) {
            y->left = z;
        }
        else {
            y->right = z;
        }
    }

    Node<T>* Transplant(Node<T>* u, Node<T>* v){
        if (u->parent == nullptr) {
            root = v;
        }
        else if (u == u->parent->left) {
            u->parent->left = v;
        }
        else if (u == u->parent->right) {
            u->parent->right = v;
        }

        if (v != nullptr) {
            v->parent = u->parent;
        }
        return root;
    }

    Node<T>* deleteNode(Node<T>* z){
        if(z == nullptr) {
            return root;
        }
        else if (z->left == nullptr) {
            root = Transplant(z, z->right);
        }
        else if (z->right == nullptr) {
            root = Transplant(z, z->left);
        }
        else {
            Node<T>* y = treeMinimum(z->right);
            if (y->parent != z) {
                root = Transplant(y, y->right);
                y->right = z->right;
                if(y->right)
                    y->right->parent = y;
            }
            root = Transplant(z, y);
            y->left = z->left;
            y->left->parent = y;
        }
        delete z;
        return root;
    }

    void inorder(Node<T>* node) {
        if (node == nullptr) return;

        inorder(node->left);
        std::cout << node->value << " ";
        inorder(node->right);
    }

    void preorder(Node<T>* node){
        if (!node) return;

        std::cout << node->value << " ";
        preorder(node->left);
        preorder(node->right);
    }

    void postorder(Node<T>* node){
        if (!node) return;

        postorder(node->left);
        postorder(node->right);
        std::cout << node->value << " ";
    }

    void destroy(Node<T>* node){
        if (!node) return;

        destroy(node->right);
        destroy(node->left);
        delete node;
    }
};

int main(){
    BST<int> tree;

    tree.insert(50);
    tree.insert(30);
    tree.insert(70);
    tree.insert(20);
    tree.insert(40);
    tree.insert(60);
    tree.insert(80);

    std::cout << "Inorder: ";
    tree.inorder(tree.getRoot());
    std::cout << std::endl;

    Node<int>* node = tree.search(50, tree.getRoot());
    tree.deleteNode(node);

    std::cout << "After deleting 50: ";
    tree.inorder(tree.getRoot());
    return 0;
}