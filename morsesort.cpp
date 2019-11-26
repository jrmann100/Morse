#include <cmath>
#include <iostream>
#include <map>
#include <vector>
using namespace std;

// I'm constantly debugging my methods.
// This flag will allow you to view some of the
// better-formatted test outputs.

//#define DEBUG

// Greatest number of morse characters.
#define MAXCHARS 7

// Example phrase to demonstrate.

#define EXAMPLE                                                            \
    "- .... .. ... ..--.- .... .- ... ..--.- - .- -.- . -. ..--.-  -- . "  \
    "..--.- .- .-.. .-.. ..--.- -. .. --. .... - .-.-.- ..--.- .. ..--.- " \
    ".--- ..- ... - ..--.- .... --- .--. . ..--.- .. - ..--.- .-- .- ... " \
    "..--.- .-- --- .-. - .... ..--.- .. - .-.-.-"
/*  Converts a morse value into an integer. This is going to take a while to
   write. From what I understand, we can "score" morse like binary, with values
   corresponding to a given node's distance from the root. How do we convert
   from morse to numeric? (This example will assume a maxchars value of 4, which
   is all the letters, but you can continue expanding upwards.)

    Given ..- as a morse code value, fill the morse into binary place values,
    Starting on the LEFT:

    .   .   -
    8   4   2   1

    Next, if it helps, think of the values like binary: multiply each place
    value by the replaced value in its box, with . being -1, - being +1, and a
    blank box being 0:

    -1  -1  1   0
    8   4   2   1
    ‾‾‾‾‾‾‾‾‾‾‾‾‾
    -8  -4  2   0

    Add the values, and you have your position: ..- or "U", has a ->numeric
    of 10. By converting each morse character to its numeric position, we can
    easily do a binary insertion into the tree.
*/

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

// Tree node, with morse data and left/right node pointers,
// as well as a corresponding character and numeric value.
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

// Morse letters. These are kept separate from numbers and characters,
// because we can make a well-distributed tree with them and then
// hang off the special values at the bottom.
// We also have the \uFFFX series so that our tree can be evened out.
// This is probably an appropriate time to mention that our end goal
// with these is to generate a usable "binary tree", which you can find online.
// This tree needs to be even at the top so that dahs and dits are
// translated into left and right motions (respectively) from the root node.
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
// inserted onto a balanced tree, or a tree that will not be balanced in the
// future. We'll come back to "balancing" later, but it is expressed in
// compress() and raise().
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
// Note that we use numeric values here, not binary sequences. This
// is because we don't know what order the binary values are being
// given to us (INTENTIONALLY) and are thus forced to create the
// balanced tree on our own.
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
    printf("%-5s %-2c %-3d\n", thisNode->data, thisNode->character,
           thisNode->numeric);
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

#ifdef DEBUG
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

// Trace a balanced morse tree down to a node,
// by moving left on a dah and right on a dit.
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

// Parse a string of morse characters into individual characters,
// then follow() each and add the result to a string.
string assembleString(string morses, Node *root) {
    if (morses.find(' ') == string::npos) {
        return string(1, follow(root, morses.c_str(), 0)->character);
    }
    return assembleString(morses.substr(0, morses.find(' ')), root) +
           assembleString(morses.substr(morses.find(' ') + 1), root);
}

// Demonstrate the functionalities of the program.
int main() {
    cout << "Jordan's Morse Code Tree Utility." << endl;

    // Roote node for unbalanced letter tree.
    Node *root = new Node(0);
    root->data = "ROOT";

    // Insert letters into tree.
    map<char, const char *>::iterator iter = morseLetters.begin();
    while (iter != morseLetters.end()) {
        insert(root, iter->first, iter->second);
        iter++;
    }

    // Collapse the unbalanced tree.
    vector<Node *> collapsed;
    collapse(root, &collapsed);
#ifdef DEBUG
    for (auto n : collapsed) {
        printf("%-5s %3d\n", n->data, n->numeric);
    }
#endif
    // Raise the collapsed unbalanced tree into a balanced tree.
    Node *newRoot = raise(collapsed);

#ifdef DEBUG
    traverse(newRoot);
#endif

    // Now that the tree is balanced, we can insert in the next
    // two rows of characters.
    iter = morseOthers.begin();
    while (iter != morseOthers.end()) {
        insert(root, iter->first, iter->second);
        iter++;
    }

    // Demonstrate the functionality of the assembled tree
    // with a long encoded example message.
    cout << "Example string:" << endl << EXAMPLE << endl;
    string completedString = assembleString(EXAMPLE, newRoot);
    // The message uses underscores for spaces. We can take those out.
    replace(completedString.begin(), completedString.end(), '_', ' ');
    cout << completedString << endl;

    // Demonstrate the functionality of the assembled tree using user input.
    // This program isn't error-trapped at all, so please be careful.
    string userinput = "Replace me.";
    while (userinput.length() != 0) {
        cout << endl
             << "What would you like to decode? (Empty input quits)" << endl
             << "> ";
        getline(std::cin, userinput);
        cout << assembleString(userinput, newRoot) << endl;
    }
    cout << "Goodbye." << endl;
    return 0;
}