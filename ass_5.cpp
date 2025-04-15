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
