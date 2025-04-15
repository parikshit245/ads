#include <iostream>
#include <queue>
#include <string>
using namespace std;

// ---------------------- B-TREE SECTION ----------------------

const int T = 2; // B-Tree minimum degree (Order = 2*T - 1 = 3)

class BTreeNode {
public:
    int keys[2 * T - 1];
    BTreeNode* children[2 * T];
    int n;
    bool leaf;

    BTreeNode(bool isLeaf) {
        leaf = isLeaf;
        n = 0;
        for (int i = 0; i < 2 * T; i++) children[i] = nullptr;
    }

    void traverseLevelOrder();
    void insertNonFull(int k);
    void splitChild(int i, BTreeNode* y);
    friend class BTree;
};

class BTree {
    BTreeNode* root;

public:
    BTree() { root = nullptr; }

    void insert(int k);
    void levelOrderDisplay();
};

void BTreeNode::traverseLevelOrder() {
    queue<BTreeNode*> q;
    q.push(this);

    while (!q.empty()) {
        int sz = q.size();
        while (sz--) {
            BTreeNode* curr = q.front(); q.pop();
            for (int i = 0; i < curr->n; i++)
                cout << curr->keys[i] << " ";
            cout << "| ";
            if (!curr->leaf) {
                for (int i = 0; i <= curr->n; i++)
                    if (curr->children[i])
                        q.push(curr->children[i]);
            }
        }
        cout << endl;
    }
}

void BTree::levelOrderDisplay() {
    if (root) root->traverseLevelOrder();
    else cout << "B-Tree is empty.\n";
}

void BTree::insert(int k) {
    if (!root) {
        root = new BTreeNode(true);
        root->keys[0] = k;
        root->n = 1;
    }
    else {
        if (root->n == 2 * T - 1) {
            BTreeNode* s = new BTreeNode(false);
            s->children[0] = root;
            s->splitChild(0, root);
            int i = (s->keys[0] < k) ? 1 : 0;
            s->children[i]->insertNonFull(k);
            root = s;
        }
        else
            root->insertNonFull(k);
    }
}

void BTreeNode::insertNonFull(int k) {
    int i = n - 1;
    if (leaf) {
        while (i >= 0 && keys[i] > k) {
            keys[i + 1] = keys[i];
            i--;
        }
        keys[i + 1] = k;
        n++;
    }
    else {
        while (i >= 0 && keys[i] > k)
            i--;
        if (children[i + 1]->n == 2 * T - 1) {
            splitChild(i + 1, children[i + 1]);
            if (keys[i + 1] < k)
                i++;
        }
        children[i + 1]->insertNonFull(k);
    }
}

void BTreeNode::splitChild(int i, BTreeNode* y) {
    BTreeNode* z = new BTreeNode(y->leaf);
    z->n = T - 1;

    for (int j = 0; j < T - 1; j++)
        z->keys[j] = y->keys[j + T];

    if (!y->leaf)
        for (int j = 0; j < T; j++)
            z->children[j] = y->children[j + T];

    y->n = T - 1;

    for (int j = n; j >= i + 1; j--)
        children[j + 1] = children[j];
    children[i + 1] = z;

    for (int j = n - 1; j >= i; j--)
        keys[j + 1] = keys[j];
    keys[i] = y->keys[T - 1];
    n++;
}

// ------------------ RED-BLACK TREE FILE SYSTEM ------------------

enum Color { RED, BLACK };

struct DirNode {
    string name;
    Color color;
    DirNode *left, *right, *parent;

    DirNode(string name) : name(name), color(RED), left(nullptr), right(nullptr), parent(nullptr) {}
};

class DirectoryTree {
    DirNode* root;

    void rotateLeft(DirNode*&);
    void rotateRight(DirNode*&);
    void fixInsert(DirNode*&);
    void fixDelete(DirNode*&);
    void transplant(DirNode*, DirNode*);
    DirNode* minimum(DirNode*);
    void displayHelper(DirNode*, string, bool);
    DirNode* search(DirNode*, string);

public:
    DirectoryTree() : root(nullptr) {}
    void insert(string name);
    void deleteDirectory(string name);
    void display();
};

void DirectoryTree::rotateLeft(DirNode*& x) {
    DirNode* y = x->right;
    x->right = y->left;
    if (y->left) y->left->parent = x;
    y->parent = x->parent;
    if (!x->parent) root = y;
    else if (x == x->parent->left) x->parent->left = y;
    else x->parent->right = y;
    y->left = x;
    x->parent = y;
}

void DirectoryTree::rotateRight(DirNode*& x) {
    DirNode* y = x->left;
    x->left = y->right;
    if (y->right) y->right->parent = x;
    y->parent = x->parent;
    if (!x->parent) root = y;
    else if (x == x->parent->right) x->parent->right = y;
    else x->parent->left = y;
    y->right = x;
    x->parent = y;
}

void DirectoryTree::fixInsert(DirNode*& z) {
    while (z->parent && z->parent->color == RED) {
        DirNode* gp = z->parent->parent;
        if (z->parent == gp->left) {
            DirNode* y = gp->right;
            if (y && y->color == RED) {
                z->parent->color = BLACK;
                y->color = BLACK;
                gp->color = RED;
                z = gp;
            } else {
                if (z == z->parent->right) {
                    z = z->parent;
                    rotateLeft(z);
                }
                z->parent->color = BLACK;
                gp->color = RED;
                rotateRight(gp);
            }
        } else {
            DirNode* y = gp->left;
            if (y && y->color == RED) {
                z->parent->color = BLACK;
                y->color = BLACK;
                gp->color = RED;
                z = gp;
            } else {
                if (z == z->parent->left) {
                    z = z->parent;
                    rotateRight(z);
                }
                z->parent->color = BLACK;
                gp->color = RED;
                rotateLeft(gp);
            }
        }
    }
    root->color = BLACK;
}

void DirectoryTree::insert(string name) {
    DirNode* z = new DirNode(name);
    DirNode* y = nullptr;
    DirNode* x = root;

    while (x) {
        y = x;
        if (z->name < x->name)
            x = x->left;
        else
            x = x->right;
    }

    z->parent = y;
    if (!y)
        root = z;
    else if (z->name < y->name)
        y->left = z;
    else
        y->right = z;

    fixInsert(z);
}

DirNode* DirectoryTree::search(DirNode* node, string name) {
    if (!node || node->name == name)
        return node;
    if (name < node->name)
        return search(node->left, name);
    else
        return search(node->right, name);
}

DirNode* DirectoryTree::minimum(DirNode* node) {
    while (node->left)
        node = node->left;
    return node;
}

void DirectoryTree::transplant(DirNode* u, DirNode* v) {
    if (!u->parent)
        root = v;
    else if (u == u->parent->left)
        u->parent->left = v;
    else
        u->parent->right = v;
    if (v)
        v->parent = u->parent;
}

void DirectoryTree::fixDelete(DirNode*& x) {
    while (x != root && (!x || x->color == BLACK)) {
        if (x == x->parent->left) {
            DirNode* w = x->parent->right;
            if (w && w->color == RED) {
                w->color = BLACK;
                x->parent->color = RED;
                rotateLeft(x->parent);
                w = x->parent->right;
            }
            if ((!w->left || w->left->color == BLACK) &&
                (!w->right || w->right->color == BLACK)) {
                w->color = RED;
                x = x->parent;
            } else {
                if (!w->right || w->right->color == BLACK) {
                    if (w->left)
                        w->left->color = BLACK;
                    w->color = RED;
                    rotateRight(w);
                    w = x->parent->right;
                }
                w->color = x->parent->color;
                x->parent->color = BLACK;
                if (w->right)
                    w->right->color = BLACK;
                rotateLeft(x->parent);
                x = root;
            }
        } else {
            DirNode* w = x->parent->left;
            if (w && w->color == RED) {
                w->color = BLACK;
                x->parent->color = RED;
                rotateRight(x->parent);
                w = x->parent->left;
            }
            if ((!w->right || w->right->color == BLACK) &&
                (!w->left || w->left->color == BLACK)) {
                w->color = RED;
                x = x->parent;
            } else {
                if (!w->left || w->left->color == BLACK) {
                    if (w->right)
                        w->right->color = BLACK;
                    w->color = RED;
                    rotateLeft(w);
                    w = x->parent->left;
                }
                w->color = x->parent->color;
                x->parent->color = BLACK;
                if (w->left)
                    w->left->color = BLACK;
                rotateRight(x->parent);
                x = root;
            }
        }
    }
    if (x) x->color = BLACK;
}

void DirectoryTree::deleteDirectory(string name) {
    DirNode* z = search(root, name);
    if (!z) {
        cout << "Directory not found.\n";
        return;
    }

    DirNode* y = z;
    DirNode* x = nullptr;
    Color yOriginalColor = y->color;

    if (!z->left) {
        x = z->right;
        transplant(z, z->right);
    } else if (!z->right) {
        x = z->left;
        transplant(z, z->left);
    } else {
        y = minimum(z->right);
        yOriginalColor = y->color;
        x = y->right;
        if (y->parent == z) {
            if (x) x->parent = y;
        } else {
            transplant(y, y->right);
            y->right = z->right;
            y->right->parent = y;
        }
        transplant(z, y);
        y->left = z->left;
        y->left->parent = y;
        y->color = z->color;
    }

    if (yOriginalColor == BLACK)
        fixDelete(x);

    delete z;
    cout << "Directory deleted successfully.\n";
}

void DirectoryTree::displayHelper(DirNode* root, string indent, bool last) {
    if (root) {
        cout << indent;
        if (last) {
            cout << "R----";
            indent += "   ";
        } else {
            cout << "L----";
            indent += "|  ";
        }
        cout << root->name << " (" << (root->color == RED ? "RED" : "BLACK") << ")" << endl;
        displayHelper(root->left, indent, false);
        displayHelper(root->right, indent, true);
    }
}

void DirectoryTree::display() {
    if (root) displayHelper(root, "", true);
    else cout << "Directory Tree is empty.\n";
}

// --------------------------- MAIN MENU ---------------------------

int main() {
    BTree btree;
    DirectoryTree fileSystem;
    int choice;

    do {
        cout << "\n========= MENU =========\n";
        cout << "1. Insert into B-Tree\n";
        cout << "2. Display B-Tree (Level Order)\n";
        cout << "3. Insert Directory\n";
        cout << "4. Delete Directory\n";
        cout << "5. Display Directory Tree\n";
        cout << "0. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
        case 1: {
            int val;
            cout << "Enter value to insert into B-Tree: ";
            cin >> val;
            btree.insert(val);
            break;
        }
        case 2:
            btree.levelOrderDisplay();
            break;
        case 3: {
            string dir;
            cout << "Enter directory name: ";
            cin >> dir;
            fileSystem.insert(dir);
            break;
        }
        case 4: {
            string dir;
            cout << "Enter directory name to delete: ";
            cin >> dir;
            fileSystem.deleteDirectory(dir);
            break;
        }
        case 5:
            fileSystem.display();
            break;
        case 0:
            cout << "Exiting...\n";
            break;
        default:
            cout << "Invalid choice!\n";
        }
    } while (choice != 0);

    return 0;
}
