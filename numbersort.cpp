#include <iostream>
#include <map>
#include <string>
#include <vector>
using namespace std;


// Tree node, with data and left/right node pointers.
struct Node {
    int data;
    struct Node *left, *right;
    Node(int data) {
        this->data = data;
        left = right = NULL;
    }
};

map<char, string> morse = {
    {'A', ".-"},    {'B', "-..."},  {'C', "-.-."},  {'D', "-.."},
    {'E', "."},     {'F', "..-."},  {'G', "--."},   {'H', "...."},
    {'I', ".."},    {'J', ".---"},  {'K', "-.-"},   {'L', ".-.."},
    {'M', "--"},    {'N', "-."},    {'O', "---"},   {'P', ".--."},
    {'Q', "--.-"},  {'R', ".-."},   {'S', "..."},   {'T', "-"},
    {'U', "..-"},   {'V', "...-"},  {'W', ".--"},   {'X', "-..-"},
    {'Y', "-.--"},  {'Z', "--.."},  {'0', "-----"}, {'1', ".----"},
    {'2', "..---"}, {'3', "...--"}, {'4', "....-"}, {'5', "....."},
    {'6', "-...."}, {'7', "--..."}, {'8', "---.."}, {'9', "----."},
};

// Recursively determine where to insert a value in a tree,
// starting with the root node and binary searching for the position
// at which to create the new node.
Node *insert(Node *thisNode, int value) {
    if (thisNode == NULL) {
        return new Node(value);
    }
    if (value < thisNode->data) {
        thisNode->left = insert(thisNode->left, value);
    } else if (value > thisNode->data) {
        thisNode->right = insert(thisNode->right, value);
    }
    return thisNode;
}

// Recursively scan through a tree, left to right.
void traverse(Node *thisNode) {
    if (thisNode->left) {
        traverse(thisNode->left);
    }
    cout << thisNode->data << endl;
    if (thisNode->right) {
        traverse(thisNode->right);
    }
}

// Resolve a tree into a vector by recursively
// scanning through it from left to right.
void collapse(Node *thisNode, vector<Node *> *list) {
    if (thisNode->left) {
        collapse(thisNode->left, list);
    }
    list->push_back(thisNode);
    if (thisNode->right) {
        collapse(thisNode->right, list);
    }
}

// Transform a (collapse()d) vector into a tree
// by recursively locating the centermost elment
// and appropriately setting the values before and
// after it to its left and right pointers.
Node *raise(vector<Node *> list) {
    if (list.size() == 1) {
        list[0]->right = NULL;
        list[0]->left = NULL;
        return list[0];
    }
    vector<Node *> leftNodes =
        vector<Node *>(list.begin(), list.begin() + (list.size() / 2));
    vector<Node *> rightNodes = vector<Node *>(
        list.begin() + list.size() / 2 + 1, list.begin() + list.size());
    if (leftNodes.size() > 0) {
        list[list.size() / 2]->left = raise(leftNodes);
    }
    if (rightNodes.size() > 0) {
        list[list.size() / 2]->right = raise(rightNodes);
    }
    return list[list.size() / 2];
}

int main() {
    cout << "Binary Sort Utility" << endl;

    int nums[] = {0, 15, 25, 5, 12, 3, 34, 132};
    Node *root = new Node(nums[0]);

    for (int i = 1; i < sizeof(nums) / sizeof(nums[0]); i++) {
        insert(root, nums[i]);
    }
    vector<Node *> collapsed;
    collapse(root, &collapsed);
    Node *newRoot = raise(collapsed);
    traverse(newRoot);
    return 0;
}