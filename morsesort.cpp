#include <iostream>
#include <map>
#include <string>
#include <vector>
using namespace std;


// Tree node, with data and left/right node pointers.
struct Node {
    const char* data;
    struct Node *left, *right;
    Node(const char* data) {
        this->data = data;
        left = right = NULL;
    }
};

map<char, const char*> morse = {
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
Node *insert(Node *thisNode, const char* value) {
    if (thisNode == NULL) {
        return new Node(value);
    }
    if (strcmp(value, thisNode->data) > 0) {
        thisNode->left = insert(thisNode->left, value);
    } else if (strcmp(value, thisNode->data) < 0) {
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
    cout << "Morse Utility" << endl;

    map<char, const char*>::iterator iter = morse.begin();
    Node *root = new Node(iter->second);
    iter++;

    // for (iter; iter != morse.end(); iter++) {
    //     insert(root, iter->second);
    // }
    
    // while (iter != morse.end()){
    //     cout << iter->second << endl;
    //     //insert(root, iter->second);
    //     iter++;
    // }

    cout << morse['A'] << endl;
    cout << morse['B'] << endl;
    cout << morse['C'] << endl;
    cout << strcmp(morse['A'], morse['C']) << endl;

    // vector<Node *> collapsed;
    // collapse(root, &collapsed);
    // Node *newRoot = raise(collapsed);
    // traverse(newRoot);
    // return 0;
}