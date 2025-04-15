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
