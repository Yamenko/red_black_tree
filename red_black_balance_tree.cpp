#include <iostream>

/**
    @enum  Color
    @brief ��� ������� ����.
    @date  18.08.2023
**/
enum Color { RED, BLACK };

/**
    @struct MyException
    @brief  ��������� ��� ����������
**/
struct MyException
{
    std::string text_ex;

    explicit MyException(std::string str) {
        this->text_ex = str;
    }

};

/**
    @struct Node
    @brief  ���� �������� ������ (���� ������ ������). ����� ������� ������ �� ����� ����������� � ��������
**/
struct Node {
    int data;
    Color color;
    Node* left;
    Node* right;
    Node* parent;

    // ����������� ��� ���������
    explicit Node(int data) {
        this->data = data;
        color = RED;
        left = nullptr;
        right = nullptr;
        parent = nullptr;
    }

    // ���������������� ������� ������ � ����� (�� �����)
    friend std::ostream& operator<<(std::ostream& os, Node* nd){
        if (nd) {
            os <<
                "This data = " << nd->data <<
                " color = " << nd->color << std::endl <<
                " left: " << nd->left << std::endl <<
                " right: " << nd->right << std::endl;
        }
        else { os << ""; }
        return os;
    }
};


/**
    @class   RedBlackTree
    @brief   �������� ����� ����������������� ������-������� ������
    @details ~
    @author  Sorb Group
    @date    18.08.2023
**/
class RedBlackTree {

public:
    /**
        @brief ����������� �� ��������� RedBlackTree
        @date  18.08.2023
    **/
    RedBlackTree() {}

    /**
        @brief ����������� ������ RedBlackTree � ���������� ������
        @param data - ������ ��� ������������� ����
        @date  18.08.2023
    **/
    RedBlackTree(int data) { insert(data); }

    /**
        @brief ����������� ������ RedBlackTree � ���������� ����
        @param newNode - ������� ���� ������� ����� ���������������� �����
        @date  18.08.2023
    **/
    RedBlackTree(Node* newNode) { 
        if (newNode->parent == nullptr) {
            root = newNode;
        }
        else throw MyException("This node has a parent, can`t make a root!");
    }

    /**
        @brief ����� ������� ������ � ������
        @param data - ������ ��� ��������
        @date  18.08.2023
    **/
    void insert(int data) {
        Node* newNode = new Node(data);

        Node* current = root;
        Node* parent = nullptr;

        while (current != nullptr) {
            parent = current;
            if (data < current->data) { current = current->left; }
            else { current = current->right; }
        }

        newNode->parent = parent;
        if (parent == nullptr) { root = newNode; }
        else if (data < parent->data) { parent->left = newNode; }
        else { parent->right = newNode; }

        fixInsertion(newNode);
    }

    /**
        @brief ����� �������� ������ �� ������
        @param data - ����� ������ ���������� �������
        @date  18.08.2023
    **/
    void remove(int data) {
        Node* node = searchNode(data);
        if (node == nullptr) { return; }

        Node* child;
        if (node->left != nullptr && node->right != nullptr) {
            Node* successor = minimum(node->right);
            node->data = successor->data;
            node = successor;
        }

        if (node->left != nullptr) { child = node->left; }
        else { child = node->right; }

        if (node->color == BLACK) { fixDeletion(node); }

        if (node->parent == nullptr) { root = child; }      // ���� ������� ������ ������ - �������� �� ����� ��� ������ ���������
        else if (node == node->parent->left) { node->parent->left = child; }
        else { node->parent->right = child; }

        if (child != nullptr) { child->parent = node->parent; }

        delete node;
    }

private:

    // ��������� �� ���� ������ ������
    Node* root = nullptr;

    /**
        @brief ��������� ����� ��� ������ ���������
        @param node - ������������ ���� ������������ ������� ����� ��������
        @date  18.08.2023
    **/
    void rotateLeft(Node* node) {
        Node* rightChild = node->right;
        node->right = rightChild->left;
        if (rightChild->left != nullptr) { rightChild->left->parent = node; }

        rightChild->parent = node->parent;
        if (node->parent == nullptr) { root = rightChild; }
        else if (node == node->parent->left) { node->parent->left = rightChild; }
        else { node->parent->right = rightChild; }

        rightChild->left = node;
        node->parent = rightChild;
    }

    /**
        @brief ��������� ����� ��� ������� ���������
        @param node - ������������ ���� ������������ ������� ����� ��������
        @date  18.08.2023
    **/
    void rotateRight(Node* node) {
        Node* leftChild = node->left;
        node->left = leftChild->right;
        if (leftChild->right != nullptr) { leftChild->right->parent = node; }

        leftChild->parent = node->parent;

        if (node->parent == nullptr) { root = leftChild; }
        else if (node == node->parent->left) { node->parent->left = leftChild; }
        else { node->parent->right = leftChild; }

        leftChild->right = node;
        node->parent = leftChild;
    }

    /**
        @brief ����� �������� ���������� ������� ����� ������� ����� ����
        @param node - ��������� �� ���� ������� �������� � ��� ������� ���������� ������� ��������
        @date  18.08.2023
    **/
    void fixInsertion(Node* node) {
        while (node != root && node->parent->color == RED) {
            if (node->parent == node->parent->parent->left) {
                Node* uncle = node->parent->parent->right;

                if (uncle != nullptr && uncle->color == RED) {
                    node->parent->color = BLACK;
                    uncle->color = BLACK;
                    node->parent->parent->color = RED;
                    node = node->parent->parent;
                }
                else {
                    if (node == node->parent->right) {
                        node = node->parent;
                        rotateLeft(node);
                    }

                    node->parent->color = BLACK;
                    node->parent->parent->color = RED;
                    rotateRight(node->parent->parent);
                }
            }
            else {
                Node* uncle = node->parent->parent->left;

                if (uncle != nullptr && uncle->color == RED) {
                    node->parent->color = BLACK;
                    uncle->color = BLACK;
                    node->parent->parent->color = RED;
                    node = node->parent->parent;
                }
                else {
                    if (node == node->parent->left) {
                        node = node->parent;
                        rotateRight(node);
                    }

                    node->parent->color = BLACK;
                    node->parent->parent->color = RED;
                    rotateLeft(node->parent->parent);
                }
            }
        }

        root->color = BLACK;
    }

    /**
        @brief ����� �������� ������ ����� �������� ����
        @param node - ��������� �� ���� ������� ���������� �������
        @date  18.08.2023
    **/
    void fixDeletion(Node* node) {
        while (node != root && node->color == BLACK) {
            if (node == node->parent->left) {
                Node* sibling = node->parent->right;

                if (sibling->color == RED) {
                    sibling->color = BLACK;
                    node->parent->color = RED;
                    rotateLeft(node->parent);
                    sibling = node->parent->right;
                }

                if (sibling->left->color == BLACK && sibling->right->color == BLACK) {
                    sibling->color = RED;
                    node = node->parent;
                }
                else {
                    if (sibling->right == nullptr || sibling->right->color == BLACK) {
                        sibling->left->color = BLACK;
                        sibling->color = RED;
                        rotateRight(sibling);
                        sibling = node->parent->right;
                    }

                    sibling->color = node->parent->color;
                    node->parent->color = BLACK;
                    sibling->right->color = BLACK;
                    rotateLeft(node->parent);
                    node = root;
                }
            }
            else {
                Node* sibling = node->parent->left;

                if (sibling->color == RED) {
                    sibling->color = BLACK;
                    node->parent->color = RED;
                    rotateRight(node->parent);
                    sibling = node->parent->left;
                }

                if (sibling->right->color == BLACK && sibling->left->color == BLACK) {
                    sibling->color = RED;
                    node = node->parent;
                }
                else {
                    if (sibling->left->color == BLACK) {
                        sibling->right->color = BLACK;
                        sibling->color = RED;
                        rotateLeft(sibling);
                        sibling = node->parent->left;
                    }

                    sibling->color = node->parent->color;
                    node->parent->color = BLACK;
                    sibling->left->color = BLACK;
                    rotateRight(node->parent);
                    node = root;
                }
            }
        }
        node->color = BLACK;
    }

    /**
        @brief  ����� ������ ������ ������������ ��������
        @param  node - ���� ������������ ������� ��������� ������ ����������
        @retval Node* - ��������� �� ����� ����� ����
        @date   18.08.2023
    **/
    Node* minimum(Node* node) {
        while (node->left != nullptr) { node = node->left; }
        return node;
    }

    /**
        @brief  ����� ������ ���� ��������� �������� ��� ����������� �������� 
        @param  data - ������ ������� �� ����
        @retval Node* - ������ �� ��������� ���� ��� ������ ���������
        @date   18.08.2023
    **/
    Node* searchNode(int data) {
        Node* current = root;
        while (current != nullptr) {
            if (data == current->data) { return current; }
            else if (data < current->data) { current = current->left; }
            else { current = current->right; }
        }
        return nullptr;
    }

};

int main() {

    // ���� ���������������� ������� ������ 
    Node* test_node = new Node(7);
    test_node->left = new Node(8);  
    test_node->right = new Node(9);
    //test_node->parent = new Node(10);
    test_node->left->parent = test_node;
    test_node->right->parent = test_node;

    std::cout << test_node << std::endl;

    // ���� ������ � �������������� ������� �����
    try {
        RedBlackTree tree_test(test_node);
    
        tree_test.insert(5);
        tree_test.insert(10);
    }
    catch (MyException& ex) {
        std::cerr << ex.text_ex << std::endl;
    }

    RedBlackTree tree(5);

    tree.insert(3);
    tree.insert(8);
    tree.insert(7);
    tree.insert(20);
    tree.insert(19);

    tree.remove(8);
    tree.remove(4);


    return 0;
}