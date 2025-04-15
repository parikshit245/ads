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