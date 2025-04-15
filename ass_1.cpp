#include <iostream>
#include <queue>
using namespace std;

struct Node {
    string key, value;
    Node* left;
    Node* right;
    Node(string k, string v) : key(k), value(v), left(nullptr), right(nullptr) {}
};

class Dictionary {
private:
    Node* root;

    Node* insert(Node* node, string key, string value) {
        if (!node) return new Node(key, value);
        if (key == node->key) {
            cout << "Duplicate key \"" << key << "\" found. Updating value.\n";
            node->value = value;
        } else if (key < node->key)
            node->left = insert(node->left, key, value);
        else
            node->right = insert(node->right, key, value);
        return node;
    }

    Node* search(Node* node, string key) {
        if (!node || node->key == key)
            return node;
        if (key < node->key)
            return search(node->left, key);
        return search(node->right, key);
    }

    Node* findMin(Node* node) {
        while (node && node->left)
            node = node->left;
        return node;
    }

    Node* deleteNode(Node* node, string key) {
        if (!node) return nullptr;
        if (key < node->key)
            node->left = deleteNode(node->left, key);
        else if (key > node->key)
            node->right = deleteNode(node->right, key);
        else {
            if (!node->left) {
                Node* temp = node->right;
                delete node;
                return temp;
            }
            else if (!node->right) {
                Node* temp = node->left;
                delete node;
                return temp;
            }
            Node* temp = findMin(node->right);
            node->key = temp->key;
            node->value = temp->value;
            node->right = deleteNode(node->right, temp->key);
        }
        return node;
    }

    void inorder(Node* node) {
        if (!node) return;
        inorder(node->left);
        cout << node->key << " : " << node->value << endl;
        inorder(node->right);
    }

    void mirror(Node* node) {
        if (!node) return;
        swap(node->left, node->right);
        mirror(node->left);
        mirror(node->right);
    }

    Node* copy(Node* node) {
        if (!node) return nullptr;
        Node* newNode = new Node(node->key, node->value);
        newNode->left = copy(node->left);
        newNode->right = copy(node->right);
        return newNode;
    }

    void levelOrder(Node* node) {
        if (!node) return;
        queue<Node*> q;
        q.push(node);
        while (!q.empty()) {
            Node* curr = q.front();
            q.pop();
            cout << curr->key << " : " << curr->value << endl;
            if (curr->left) q.push(curr->left);
            if (curr->right) q.push(curr->right);
        }
    }

public:
    Dictionary() : root(nullptr) {}

    void insert(string key, string value) {
        root = insert(root, key, value);
    }

    void deleteWord(string key) {
        root = deleteNode(root, key);
    }

    void searchWord(string key) {
        Node* result = search(root, key);
        if (result)
            cout << "Found: " << result->key << " : " << result->value << endl;
        else
            cout << "Word not found.\n";
    }

    void display() {
        cout << "Inorder Traversal:\n";
        inorder(root);
    }

    void mirrorImage() {
        mirror(root);
        cout << "Mirror image created.\n";
    }

    Dictionary* copyDictionary() {
        Dictionary* newDict = new Dictionary();
        newDict->root = copy(root);
        return newDict;
    }

    void displayLevelWise() {
        cout << "Level Order Traversal:\n";
        levelOrder(root);
    }
};

// ---------- Main Menu ----------
int main() {
    Dictionary dict;
    int choice;
    string key, value;

    do {
        cout << "\n----- Dictionary Menu -----\n";
        cout << "1. Insert word\n";
        cout << "2. Delete word\n";
        cout << "3. Search word\n";
        cout << "4. Display dictionary (Inorder)\n";
        cout << "5. Mirror image of dictionary\n";
        cout << "6. Create and display a copy\n";
        cout << "7. Display dictionary level-wise\n";
        cout << "0. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;
        cin.ignore();

        switch (choice) {
            case 1:
                cout << "Enter word (key): ";
                getline(cin, key);
                cout << "Enter meaning (value): ";
                getline(cin, value);
                dict.insert(key, value);
                break;

            case 2:
                cout << "Enter word to delete: ";
                getline(cin, key);
                dict.deleteWord(key);
                break;

            case 3:
                cout << "Enter word to search: ";
                getline(cin, key);
                dict.searchWord(key);
                break;

            case 4:
                dict.display();
                break;

            case 5:
                dict.mirrorImage();
                break;

            case 6: {
                Dictionary* copy = dict.copyDictionary();
                cout << "Copied dictionary (inorder):\n";
                copy->display();
                delete copy;
                break;
            }

            case 7:
                dict.displayLevelWise();
                break;

            case 0:
                cout << "Exiting program.\n";
                break;

            default:
                cout << "Invalid choice! Please try again.\n";
        }
    } while (choice != 0);

    return 0;
}
