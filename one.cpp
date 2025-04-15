//1
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

//2
#include <iostream>
#include <algorithm>
using namespace std;

struct PlayerNode {
    int player_id;
    int score;
    PlayerNode* left;
    PlayerNode* right;
    int height;

    PlayerNode(int id, int sc) : player_id(id), score(sc), left(nullptr), right(nullptr), height(1) {}
};

class AVLGameSystem {
private:
    PlayerNode* root;

    int height(PlayerNode* node) {
        return node ? node->height : 0;
    }

    int getBalance(PlayerNode* node) {
        return node ? height(node->left) - height(node->right) : 0;
    }

    PlayerNode* rotateRight(PlayerNode* y) {
        PlayerNode* x = y->left;
        PlayerNode* T2 = x->right;
        x->right = y;
        y->left = T2;
        y->height = max(height(y->left), height(y->right)) + 1;
        x->height = max(height(x->left), height(x->right)) + 1;
        return x;
    }

    PlayerNode* rotateLeft(PlayerNode* x) {
        PlayerNode* y = x->right;
        PlayerNode* T2 = y->left;
        y->left = x;
        x->right = T2;
        x->height = max(height(x->left), height(x->right)) + 1;
        y->height = max(height(y->left), height(y->right)) + 1;
        return y;
    }

    PlayerNode* insert(PlayerNode* node, int player_id, int score) {
        if (!node)
            return new PlayerNode(player_id, score);

        if (player_id < node->player_id)
            node->left = insert(node->left, player_id, score);
        else if (player_id > node->player_id)
            node->right = insert(node->right, player_id, score);
        else {
            cout << "Player ID already exists. Updating score.\n";
            node->score = score;
            return node;
        }

        node->height = max(height(node->left), height(node->right)) + 1;
        int balance = getBalance(node);

        // Balance the tree
        if (balance > 1 && player_id < node->left->player_id)
            return rotateRight(node);
        if (balance < -1 && player_id > node->right->player_id)
            return rotateLeft(node);
        if (balance > 1 && player_id > node->left->player_id) {
            node->left = rotateLeft(node->left);
            return rotateRight(node);
        }
        if (balance < -1 && player_id < node->right->player_id) {
            node->right = rotateRight(node->right);
            return rotateLeft(node);
        }

        return node;
    }

    PlayerNode* minValueNode(PlayerNode* node) {
        PlayerNode* current = node;
        while (current && current->left)
            current = current->left;
        return current;
    }

    PlayerNode* deleteNode(PlayerNode* root, int player_id) {
        if (!root)
            return root;

        if (player_id < root->player_id)
            root->left = deleteNode(root->left, player_id);
        else if (player_id > root->player_id)
            root->right = deleteNode(root->right, player_id);
        else {
            if (!root->left || !root->right) {
                PlayerNode* temp = root->left ? root->left : root->right;
                if (!temp) {
                    temp = root;
                    root = nullptr;
                } else
                    *root = *temp;
                delete temp;
            } else {
                PlayerNode* temp = minValueNode(root->right);
                root->player_id = temp->player_id;
                root->score = temp->score;
                root->right = deleteNode(root->right, temp->player_id);
            }
        }

        if (!root)
            return root;

        root->height = max(height(root->left), height(root->right)) + 1;
        int balance = getBalance(root);

        if (balance > 1 && getBalance(root->left) >= 0)
            return rotateRight(root);
        if (balance > 1 && getBalance(root->left) < 0) {
            root->left = rotateLeft(root->left);
            return rotateRight(root);
        }
        if (balance < -1 && getBalance(root->right) <= 0)
            return rotateLeft(root);
        if (balance < -1 && getBalance(root->right) > 0) {
            root->right = rotateRight(root->right);
            return rotateLeft(root);
        }

        return root;
    }

    void inOrder(PlayerNode* root) {
        if (!root) return;
        inOrder(root->right);  // Reverse order for leaderboard (high to low)
        cout << "Player ID: " << root->player_id << " | Score: " << root->score << endl;
        inOrder(root->left);
    }

public:
    AVLGameSystem() : root(nullptr) {}

    void registerPlayer(int player_id, int score) {
        root = insert(root, player_id, score);
    }

    void displayLeaderboard() {
        cout << "\n--- Leaderboard ---\n";
        if (!root) {
            cout << "No players registered yet.\n";
        } else {
            inOrder(root);
        }
    }

    void removePlayer(int player_id) {
        root = deleteNode(root, player_id);
    }
};

// ------------------- Main Menu -------------------

int main() {
    AVLGameSystem game;
    int choice, id, score;

    do {
        cout << "\n======= Multiplayer Game Menu =======\n";
        cout << "1. Player Registration\n";
        cout << "2. Display Leaderboard\n";
        cout << "3. Remove Player\n";
        cout << "0. Exit\n";
        cout << "Enter choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                cout << "Enter Player ID: ";
                cin >> id;
                cout << "Enter Score: ";
                cin >> score;
                game.registerPlayer(id, score);
                break;

            case 2:
                game.displayLeaderboard();
                break;

            case 3:
                cout << "Enter Player ID to remove: ";
                cin >> id;
                game.removePlayer(id);
                break;

            case 0:
                cout << "Exiting game system.\n";
                break;

            default:
                cout << "Invalid choice. Try again.\n";
        }
    } while (choice != 0);

    return 0;
}

//3
#include<bits/stdc++.h>

using namespace std;

vector<vector<int>> GRAPH;
vector<bool> visited;

void DFS(int current)
{
    visited[current] = true; // Mark current as visited
    cout << "Current on (DFS): " << current << endl;
    for (int i = 0; i < GRAPH.size(); ++i)
    { // Check all possible nodes

        if (GRAPH[current][i] == 1 && !visited[i])
        {
            DFS(i);
        }
    }
}

void BFS(int start)
{
    queue<int> q;
    visited[start] = true;
    q.push(start);
    while (!q.empty())
    {
        int current = q.front();
        q.pop();
        cout << "Current on (BFS): " << current << endl;
        for (int i = 0; i < GRAPH.size(); ++i)
        {
            if (GRAPH[current][i] == 1 && !visited[i])
            {
                q.push(i);
                visited[i] = true;
            }
        }
    }
}
int main()
{
    GRAPH = {
        {1, 0, 1, 1, 1},
        {1, 0, 1, 0, 1},
        {1, 1, 1, 0, 1},
        {0, 0, 0, 0, 1},
        {1, 1, 1, 1, 1}};
    int numNodes = GRAPH.size();
    visited.assign(numNodes, 0);
    int startNode = 0;
    cout << "Starting maze and finding min path using DFS from node " << startNode << endl;
    DFS(startNode);
    visited.assign(numNodes, 0);
    cout << endl
         << endl;
    cout << "Starting maze and finding min path using BFS from node " << startNode
         << endl;
    BFS(startNode);
    return 0;
}

//4
#include<bits/stdc++.h>
using namespace std;

struct Department {
    string name;
    string location;
};

int findMinKey(int key[], bool mstSet[], int V) {
    int min = INT_MAX, index = -1;
    for (int v = 0; v < V; v++) {
        if (!mstSet[v] && key[v] < min) {
            min = key[v];
            index = v;
        }
    }
    return index;
}

void printMST(int parent[], int graph[][100], Department departments[], int V) {
    cout << "\nMinimum Spanning Tree Connections:\n";
    cout << "---------------------------------\n";

    int totalDistance = 0;
    for (int i = 1; i < V; i++) {
        if (parent[i] != -1) {
            cout << departments[parent[i]].name << " - " << departments[i].name
                 << " : " << graph[i][parent[i]] << " meters\n";
            totalDistance += graph[i][parent[i]];
        }
    }
    cout << "\nTotal cable length: " << totalDistance << " meters\n";
}

void findMST(int graph[][100], int V, Department departments[], int start) {
    int parent[V], key[V];
    bool mstSet[V];

    for (int i = 0; i < V; i++) {
        key[i] = INT_MAX;
        mstSet[i] = false;
    }

    key[start] = 0;
    parent[start] = -1;

    for (int count = 0; count < V - 1; count++) {
        int u = findMinKey(key, mstSet, V);
        mstSet[u] = true;

        for (int v = 0; v < V; v++) {
            if (graph[u][v] && !mstSet[v] && graph[u][v] < key[v]) {
                parent[v] = u;
                key[v] = graph[u][v];
            }
        }
    }

    printMST(parent, graph, departments, V);
}

int main() {
    int V;
    cout << "Enter number of departments: ";
    cin >> V;
    cin.ignore();

    Department departments[100];
    int graph[100][100] = {0};

    cout << "\nEnter department details:\n";
    for (int i = 0; i < V; i++) {
        cout << "Department " << i << " name: ";
        getline(cin, departments[i].name);
        cout << "Location: ";
        getline(cin, departments[i].location);
    }

    cout << "\nEnter distances between departments:\n";
    for (int i = 0; i < V; i++) {
        for (int j = i + 1; j < V; j++) {
            cout << departments[i].name << " to " << departments[j].name << ": ";
            cin >> graph[i][j];
            graph[j][i] = graph[i][j];  // Symmetric
        }
    }

    while (true) {
        cout << "\nAvailable departments:\n";
        for (int i = 0; i < V; i++) {
            cout << i << ": " << departments[i].name << endl;
        }

        int start;
        cout << "Enter starting department index (0-" << V - 1 << "): ";
        cin >> start;

        findMST(graph, V, departments, start);

        char again;
        cout << "Try another starting point? (y/n): ";
        cin >> again;
        if (again != 'y' && again != 'Y') break;
    }

    return 0;
}


//5
#include<bits/stdc++.h>
using namespace std;

const int MAX = 100;
const int INF = 9999;

struct Location {
    string name;
    string type;
};

int getNearest(int dist[], bool visited[], int count) {
    int minDist = INF, index = -1;
    for (int i = 0; i < count; i++) {
        if (!visited[i] && dist[i] < minDist) {
            minDist = dist[i];
            index = i;
        }
    }
    return index;
}

void displayRoutes(int dist[], int count, int source, Location locs[]) {
    cout << "\nOptimal Routes from " << locs[source].name << " (" << locs[source].type << "):\n";
    cout << "----------------------------------------------------------\n";
    cout << "Destination\t\tType\t\tDistance (km)\n";
    cout << "----------------------------------------------------------\n";

    for (int i = 0; i < count; i++) {
        cout << locs[i].name << "\t\t" << locs[i].type << "\t\t";
        if (dist[i] == INF)
            cout << "Unreachable\n";
        else
            cout << dist[i] << "\n";
    }
}

void dijkstra(int graph[MAX][MAX], int count, int source, Location locs[]) {
    int dist[MAX];
    bool visited[MAX] = {false};

    for (int i = 0; i < count; i++) dist[i] = INF;
    dist[source] = 0;

    for (int i = 0; i < count - 1; i++) {
        int u = getNearest(dist, visited, count);
        if (u == -1) break;

        visited[u] = true;

        for (int v = 0; v < count; v++) {
            if (!visited[v] && graph[u][v] != INF && dist[u] + graph[u][v] < dist[v]) {
                dist[v] = dist[u] + graph[u][v];
            }
        }
    }

    displayRoutes(dist, count, source, locs);
}

int main() {
    int n, graph[MAX][MAX];
    Location locs[MAX];

    cout << "SUPPLY CHAIN ROUTE OPTIMIZATION\n";
    cout << "===============================\n";

    // Number of locations
    do {
        cout << "Enter number of locations (1-100): ";
        cin >> n;
    } while (n < 1 || n > 100);

    cin.ignore(); // Clear input buffer

    // Location details
    for (int i = 0; i < n; i++) {
        cout << "\nLocation " << i << ":\n";
        cout << " Name: ";
        getline(cin, locs[i].name);
        cout << " Type (supplier/warehouse/distribution/retail): ";
        getline(cin, locs[i].type);
    }

    // Initialize distances
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            graph[i][j] = (i == j) ? 0 : INF;

    // Enter distance between locations
    cout << "\nEnter distances between locations (in km):\n";
    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
            int d;
            cout << "Distance between " << locs[i].name << " and " << locs[j].name << " (-1 if none): ";
            cin >> d;
            if (d >= 0) {
                graph[i][j] = graph[j][i] = d;
            }
        }
    }

    // Source location
    int src;
    do {
        cout << "\nEnter source location index (0 to " << n - 1 << "): ";
        cin >> src;
    } while (src < 0 || src >= n);

    dijkstra(graph, n, src, locs);

    return 0;
}


//6
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

