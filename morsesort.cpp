#include <cmath>
#include <iostream>
#include <map>
#include <vector>

//#define debug

// Greatest number of morse characters.
#define MAXCHARS 7

using namespace std;

int morse2num(const char *data) {
    int output = 0;
    int places = MAXCHARS - 1;
    for (int i = places; i >= 0 && places - i <= strlen(data); i--) {
        if (data[places - i] == '.') {
            output -= pow(2, i);
        } else if (data[places - i] == '-') {
            output += pow(2, i);
        }
    }
    return output;
}

// Tree node, with data and left/right node pointers.
struct Node {
    const char *data;
    int numeric;
    char character;
    struct Node *left, *right;
    Node(char character, const char *data) {
        this->data = data;
        this->numeric = morse2num(data);
        this->character = character;
        left = right = NULL;
    }
    Node(int numeric) { this->numeric = numeric; }
};

map<char, const char *> morseLetters = {
    {'A', ".-"},         {'B', "-..."},       {'C', "-.-."},
    {'D', "-.."},        {'E', "."},          {'F', "..-."},
    {'G', "--."},        {'H', "...."},       {'I', ".."},
    {'J', ".---"},       {'K', "-.-"},        {'L', ".-.."},
    {'M', "--"},         {'N', "-."},         {'O', "---"},
    {'P', ".--."},       {'Q', "--.-"},       {'R', ".-."},
    {'S', "..."},        {'T', "-"},          {'U', "..-"},
    {'V', "...-"},       {'W', ".--"},        {'X', "-..-"},
    {'Y', "-.--"},       {'Z', "--.."},       {L'\uFFF0', "..--"},
    {L'\uFFF1', ".-.-"}, {L'\uFFF2', "---."}, {L'\uFFF3', "----"}};

// Morse numbers and special characters should only be
// inserted onto a balanced tree, or a tree that will not be balanced.
map<char, const char *> morseOthers = {
    {'0', "-----"},  {'1', ".----"},   {'2', "..---"},   {'3', "...--"},
    {'4', "....-"},  {'5', "....."},   {'6', "-...."},   {'7', "--..."},
    {'8', "---.."},  {'9', "----."},   {'.', ".-.-.-"},  {',', "--..--"},
    {'?', "..--.."}, {'\'', ".----."}, {'!', "-.-.--"},  {'/', "-..-."},
    {'(', "-.--."},  {')', "-.--.-"},  {'&', ".-..."},   {':', "---..."},
    {';', "-.-.-."}, {'=', "-...-"},   {'+', ".-.-."},   {'-', "-....-"},
    {'_', "..--.-"}, {'"', ".-..-."},  {'$', "...-..-"}, {'@', ".--.-."}};

// Recursively determine where to insert a value in a tree,
// starting with the root node and binary searching for the position
// at which to create the new node.
Node *insert(Node *thisNode, char character, const char *value) {
    if (thisNode == NULL) {
        return new Node(character, value);
    }
    if (morse2num(value) > thisNode->numeric) {
        thisNode->left = insert(thisNode->left, character, value);
    } else if (morse2num(value) < thisNode->numeric) {
        thisNode->right = insert(thisNode->right, character, value);
    }
    return thisNode;
}

// Recursively scan through a tree, left to right.
void traverse(Node *thisNode) {
    if (thisNode->left) {
        traverse(thisNode->left);
    }
    cout << thisNode->character << " : " << thisNode->data << endl;
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

#ifdef debug
    printf("Center is index %d, value %s\n", (int)list.size() / 2,
           list[list.size() / 2]->data);
    cout << endl;
    cout << "Left: ";
    for (Node *node :
         vector<Node *>(list.begin(), list.begin() + ((int)list.size() / 2))) {
        cout << node->data << " ";
    }
    cout << endl;
    cout << "Right: ";
    for (Node *node : vector<Node *>(list.begin() + list.size() / 2 + 1,
                                     list.begin() + list.size())) {
        cout << node->data << " ";
    }
    cout << endl;
#endif

    if (leftNodes.size() > 0) {
        list[list.size() / 2]->left = raise(leftNodes);
    }
    if (rightNodes.size() > 0) {
        list[list.size() / 2]->right = raise(rightNodes);
    }
    return list[list.size() / 2];
}

Node *follow(Node *thisNode, const char *morse, int pathPointer) {
    if (strlen(morse) == pathPointer) {
        return thisNode;
    }
    if (morse[pathPointer] == '.') {
        return follow(thisNode->right, morse, ++pathPointer);
    }
    if (morse[pathPointer] == '-') {
        return follow(thisNode->left, morse, ++pathPointer);
    }
    return NULL;
}

string assembleString(string morses, Node *root) {
    if (morses.find(' ') == string::npos) {
        return string(1, follow(root, morses.c_str(), 0)->character);
    }
    return assembleString(morses.substr(0, morses.find(' ')), root) +
           assembleString(morses.substr(morses.find(' ') + 1), root);
}

int main() {
    cout << "Jordan's Morse Code Tree Utility." << endl;

    Node *root = new Node(0);
    root->data = "ROOT";

    map<char, const char *>::iterator iter = morseLetters.begin();
    while (iter != morseLetters.end()) {
        insert(root, iter->first, iter->second);
        iter++;
    }
    vector<Node *> collapsed;
    collapse(root, &collapsed);
    /*for (auto n : collapsed) {
        printf("%-5s %3d\n", n->data, n->numeric);
    }*/
    Node *newRoot = raise(collapsed);

    iter = morseOthers.begin();

#ifdef debug
    traverse(newRoot);
#endif

    while (iter != morseOthers.end()) {
        insert(root, iter->first, iter->second);
        iter++;
    }

    string completedString = assembleString(
        "- .... .. ... ..--.- .... .- ... ..--.- - .- -.- . -. ..--.- "
        "-- . ..--.- .- .-.. .-.. ..--.- -. .. --. .... - .-.-.- ..--.- "
        ".. ..--.- .--- ..- ... - ..--.- .... --- .--. . ..--.- .. - "
        "..--.- .-- .- ... ..--.- .-- --- .-. - .... ..--.- .. - .-.-.-",
        newRoot);
    replace(completedString.begin(), completedString.end(), '_', ' ');
    cout << completedString << endl;
    return 0;
}