#include <iostream>
#include <map>
#include <string>
#include <vector>
using namespace std;

struct Node
{
    int data;
    struct Node *left, *right;
    Node(int data)
    {
        this->data = data;
        left = right = NULL;
    }
};

map<char, string> morse = {
    {'A', ".-"},
    {'B', "-..."},
    {'C', "-.-."},
    {'D', "-.."},
    {'E', "."},
    {'F', "..-."},
    {'G', "--."},
    {'H', "...."},
    {'I', ".."},
    {'J', ".---"},
    {'K', "-.-"},
    {'L', ".-.."},
    {'M', "--"},
    {'N', "-."},
    {'O', "---"},
    {'P', ".--."},
    {'Q', "--.-"},
    {'R', ".-."},
    {'S', "..."},
    {'T', "-"},
    {'U', "..-"},
    {'V', "...-"},
    {'W', ".--"},
    {'X', "-..-"},
    {'Y', "-.--"},
    {'Z', "--.."},
    {'0', "-----"},
    {'1', ".----"},
    {'2', "..---"},
    {'3', "...--"},
    {'4', "....-"},
    {'5', "....."},
    {'6', "-...."},
    {'7', "--..."},
    {'8', "---.."},
    {'9', "----."},
};

Node *insert(Node *thisNode, int value)
{
    if (thisNode == NULL)
    {
        return new Node(value);
    }
    if (value < thisNode->data)
    {
        thisNode->left = insert(thisNode->left, value);
    }
    else if (value > thisNode->data)
    {
        thisNode->right = insert(thisNode->right, value);
    }
    return thisNode;
}

void traverse(Node *thisNode)
{
    if (thisNode->left)
    {
        traverse(thisNode->left);
    }
    cout << thisNode->data << endl;
    if (thisNode->right)
    {
        traverse(thisNode->right);
    }
}

void collapse(Node *thisNode, vector<Node*> *list)
{
    if (thisNode->left)
    {
        collapse(thisNode->left, list);
    }
    list->push_back(thisNode);
    if (thisNode->right)
    {
        collapse(thisNode->right, list);
    }
}

Node* raise(vector<Node*> list)
{
    if (list.size() == 1)
    {
        list[0]->right = NULL;
        list[0]->left = NULL;
        return list[0];
    }

    /*printf("Center is index %d, value %d\n", (int)list.size() / 2, list[list.size() / 2]->data);
    cout << endl;
    cout << "Left: ";
    for (Node* node : vector<Node*>(list.begin(), list.begin() + ((int)list.size() / 2)))
    {
        cout << node->data << " ";
    }
    cout << endl;
    cout << "Right: ";
    for (Node* node : vector<Node*>(list.begin() + list.size() / 2 + 1, list.begin() + list.size() ))
    {
        cout << node->data << " ";
    }
    cout << endl;*/
    list[list.size() / 2]->left = raise(vector<Node*>(list.begin(), list.begin() + (list.size() / 2)));
    list[list.size() / 2]->right = raise(vector<Node*>(list.begin() + list.size() / 2 + 1, list.begin() + list.size()));
    return list[list.size() / 2];
}

int main()
{
    cout << "Binary Sort Utility" << endl;

    //int nums[] = {0, 15, 25, 5, 12, 3, 34, 132}; // Still need to handle this segfault.
    int nums[] = {0, 15, 25, 5, 12, 3, 34};
    Node *root = new Node(nums[0]);

    for (int i = 1; i < sizeof(nums) / sizeof(nums[0]); i++)
    {
        insert(root, nums[i]);
    }
    //traverse(root);
    vector<Node*> collapsed;
    collapse(root, &collapsed);
    // (for (Node node : collapsed)
    // {
    //     cout << node.data << " ";
    // }
    // cout << endl;
    Node* newRoot = raise(collapsed);
    traverse(newRoot);
    return 0;
}