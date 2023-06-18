#include <iostream>
#include "AVL_Tree.h"
#include <ctime>
#include <climits>
#include <cstdlib>
#include <windows.h>
//song library
#include <thread>  
#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")
using namespace std;



const int MAP_WIDTH = 10;
const int MAP_HEIGHT = 10;
const int MAX = 100;

//const int numNodes = 100;
int parent[MAX], ranks[MAX];
bool visited[MAX];

void prim(int n, int graph[MAX][MAX]) {
    // initialize parent array and visited array
    for (int i = 0; i < n; i++) {
        parent[i] = -1;
        visited[i] = false;
    }

    // initialize distance array with max value for all vertices
    int dist[MAX];
    for (int i = 0; i < n; i++) {
        dist[i] = INT_MAX;
    }
    dist[0] = 0;

    // loop over all vertices
    for (int i = 0; i < n; i++) {
        // find the vertex with minimum distance that has not been visited
        int min_dist = INT_MAX;
        int min_vertex = -1;
        for (int j = 0; j < n; j++) {
            if (!visited[j] && dist[j] < min_dist) {
                min_dist = dist[j];
                min_vertex = j;
            }
        }

        // mark the min_vertex as visited
        visited[min_vertex] = true;

        // update the distances to its neighbors
        for (int j = 0; j < n; j++) {
            if (graph[min_vertex][j] > 0 && !visited[j] && graph[min_vertex][j] < dist[j]) {
                dist[j] = graph[min_vertex][j];
                parent[j] = min_vertex;
            }
        }
    }
}

void printMST(int n) {
    cout << "Edges in the minimum spanning tree:\n";
    for (int i = 1; i < n; i++) {
        cout << "(" << parent[i] + 1 << ", " << i + 1 << ")\n";
    }
}

struct Edge {
    int u, v, weight;   //u v are endpoints
};

void make_set(int v) {
    parent[v] = v;
    ranks[v] = 0;
}

int find_set(int v) {
    if (v == parent[v]) {
        return v;
    }
    return parent[v] = find_set(parent[v]);
}

void union_sets(int a, int b) {
    a = find_set(a);
    b = find_set(b);
    if (a != b) {
        if (ranks[a] < ranks[b]) {
            swap(a, b);
        }
        parent[b] = a;
        if (ranks[a] == ranks[b]) {
            ranks[a]++;
        }
    }
}

bool compare(Edge a, Edge b) {
    return a.weight < b.weight;
}

void kruskal(int n, int graph[MAX][MAX]) {

    //Edge edges[n * n];
    Edge* edges = new Edge[n * n];
    int index = 0;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (graph[i][j] != 0) {
                edges[index].u = i;
                edges[index].v = j;
                edges[index].weight = graph[i][j];
                index++;
            }
        }
    }

    //array is sorting
    for (int i = 0; i < index - 1; i++) {
        for (int j = i + 1; j < index; j++) {
            if (compare(edges[j], edges[i])) {
                Edge temp = edges[i];
                edges[i] = edges[j];
                edges[j] = temp;
            }
        }
    }

    for (int i = 0; i < n; i++) {
        make_set(i);
    }
    for (int i = 0; i < index; i++) {
        int u = edges[i].u;
        int v = edges[i].v;
        int w = edges[i].weight;
        if (find_set(u) != find_set(v)) {
            union_sets(u, v);
            cout << u << " - " << v << " : " << w << endl;
        }
    }

    delete edges;

}

// Function to generate a random map
void generateMap(char map[][MAP_HEIGHT]) {
    // Clear the map
    for (int i = 0; i < MAP_WIDTH; i++) {
        for (int j = 0; j < MAP_HEIGHT; j++) {
            map[i][j] = 'C';
        }
    }
    // Add jewels
    int numJewels = rand() % 10 + 1; // Random number between 1 and 10
    for (int i = 0; i < numJewels; i++) {
        int x = rand() % MAP_WIDTH;
        int y = rand() % MAP_HEIGHT;
        map[x][y] = 'J';
    }

    // Add potions
    int numPotions = rand() % 6 + 1; // Random number between 1 and 3
    for (int i = 0; i < numPotions; i++) {
        int x = rand() % MAP_WIDTH;
        int y = rand() % MAP_HEIGHT;
        map[x][y] = 'P';
    }

    // Add weapons
    int numWeapons = rand() % 10 + 1; // Random number between 1 and 2
    for (int i = 0; i < numWeapons; i++) {
        int x = rand() % MAP_WIDTH;
        int y = rand() % MAP_HEIGHT;
        map[x][y] = 'W';
    }

    // Add death points
    int numDeaths = rand() % 10 + 1; // Random number between 0 and 1
    for (int i = 0; i < numDeaths; i++) {
        int x = rand() % MAP_WIDTH;
        int y = rand() % MAP_HEIGHT;
        map[x][y] = '%';
    }

    // Add obstacles
    int numObstacles = rand() % 10 + 1; // Random number between 1 and 10
    for (int i = 0; i < numObstacles; i++) {
        int x = rand() % MAP_WIDTH;
        int y = rand() % MAP_HEIGHT;
        map[x][y] = '#';
    }

    // Add monsters
    int numMonsters = rand() % 5 + 1; // Random number between 1 and 5
    for (int i = 0; i < numMonsters; i++) {
        int x = rand() % MAP_WIDTH;
        int y = rand() % MAP_HEIGHT;
        int monster = rand() % 4; // Random number between 0 and 3

        switch (monster) {
        case 0:
            map[x][y] = '&'; // Dragon
            break;
        case 1:
            map[x][y] = '$'; // Goblin
            break;
        case 2:
            map[x][y] = '@'; // Werewolf
            break;
        default:
            break;
        }
    }

    map[0][0] = 'C';
    // Add the crystal (goal point)
    int x = rand() % MAP_WIDTH;
    int y = rand() % MAP_HEIGHT;
    map[x][y] = '*';
}

// Function to find the index of the starting and goal nodes
void findStartAndGoal(char map[][MAP_HEIGHT], int& start, int& goal) {
    start = 0;
    goal = -1;
    for (int i = 0; i < MAP_WIDTH; i++) {
        for (int j = 0; j < MAP_HEIGHT; j++) {
            int index = i * MAP_WIDTH + j;
            // if (map[i][j] == 'C') {
            //     start = index;
            // }
            if (map[i][j] == '*') {
                goal = index;
            }

            if (start != -1 && goal != -1) {
                return;
            }
        }
    }
}

void findGoal(char map[][MAP_HEIGHT], int& start, int& goal) {
    goal = -1;
    for (int i = 0; i < MAP_WIDTH; i++) {
        for (int j = 0; j < MAP_HEIGHT; j++) {
            int index = i * MAP_WIDTH + j;
            /*if (map[i][j] == 'C') {
                start = index;
            }*/
            if (map[i][j] == '*') {
                goal = index;
            }

            if (start != -1 && goal != -1) {
                return;
            }
        }
    }
}

// Function to initialize distance and visited arrays
void initDijkstra(int adjMatrix[][100], int start, int distance[], bool visited[]) {
    for (int i = 0; i < MAP_WIDTH * MAP_HEIGHT; i++) {
        distance[i] = INT_MAX;
        visited[i] = false;
    }
    distance[start] = 0;
}

// Function to find the minimum distance node from the distance array that has not been visited yet
int minDistance(int distance[], bool visited[]) {
    int minDist = INT_MAX, minIndex;
    for (int i = 0; i < MAP_WIDTH * MAP_HEIGHT; i++) {
        if (!visited[i] && distance[i] <= minDist) {
            minDist = distance[i];
            minIndex = i;
        }
    }
    return minIndex;
}

// Function to update the distance array with the newly found minimum distance node
void updateDistance(int adjMatrix[][100], int u, int distance[], bool visited[]) {
    for (int v = 0; v < MAP_WIDTH * MAP_HEIGHT; v++) {
        if (adjMatrix[u][v] != 0 && !visited[v] && distance[u] != INT_MAX &&
            distance[u] + adjMatrix[u][v] < distance[v]) {
            distance[v] = distance[u] + adjMatrix[u][v];
        }
    }
}

// Dijkstra's algorithm function
int dijkstra(int adjMatrix[][100], int start, int goal) {
    int distance[MAP_WIDTH * MAP_HEIGHT];
    bool visited[MAP_WIDTH * MAP_HEIGHT];

    // Initialize distance and visited arrays
    initDijkstra(adjMatrix, start, distance, visited);

    for (int i = 0; i < MAP_WIDTH * MAP_HEIGHT; i++) {
        int u = minDistance(distance, visited);
        visited[u] = true;
        updateDistance(adjMatrix, u, distance, visited);
    }
    return distance[goal];
}

void start_screen()
{
    system("color 0F");
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, 10);
    cout << "          Welcome to the          "<<endl;
    cout << " The Quest for the Crystal Kingdom"<<endl;
    cout << endl;
    cout << endl;


    int rows = 10;
    // Top half of the diamond
    for (int i = 1; i <= rows; i++) {
        for (int j = 1; j <= rows - i; j++) {
            cout << " ";
        }
        for (int k = 1; k <= 2 * i - 1; k++) {
            cout << "*";
        }
        cout << endl;
    }

    // Bottom half of the diamond
    for (int i = rows - 1; i >= 1; i--) {
        for (int j = 1; j <= rows - i; j++) {
            cout << " ";
        }
        for (int k = 1; k <= 2 * i - 1; k++) {
            cout << "*";
        }
        cout << endl;
    }


    Sleep(6000);
    system("CLS");

    SetConsoleTextAttribute(hConsole, 7);
}
void menu()
{
    cout << "\t\t\t\t|-------------------------------------------------------------------|\n";
    cout << "\t\t\t\t|                                                                   |\n";
    cout << "\t\t\t\t|                    What do you wanna do :                         |\n";
    cout << "\t\t\t\t|    1. Dijkstra : Find shortest path from 0,0 location             |\n";
    cout << "\t\t\t\t|    2. Dijkstra : Find shortest path from any start index          |\n";
    cout << "\t\t\t\t|    3. Kruskal                                                     |\n";
    cout << "\t\t\t\t|    4. Prims                                                       |\n";
    cout << "\t\t\t\t|    5. Floyd : Find shortest path from 0,0 location                |\n";
    cout << "\t\t\t\t|    6. Floyd : Find shortest path from any start index             |\n";
    cout << "\t\t\t\t|    0. Exit                                                        |\n";
    cout << "\t\t\t\t|                                                                   |\n";
    cout << "\t\t\t\t|-------------------------------------------------------------------|\n";
}
void playMusic()
{
    // Open and play the music file
    PlaySound(TEXT("tate_ture_dore.wav"), NULL, SND_FILENAME | SND_ASYNC);
}

int main()
{
    //thread musicThread(playMusic);

    //start_screen();


    srand(time(NULL));
    char map[MAP_WIDTH][MAP_HEIGHT];
    generateMap(map);

    AVLTree tree;

    for (int i = 0; i < MAP_WIDTH; i++) {
        for (int j = 0; j < MAP_HEIGHT; j++) {
            cout << map[i][j] << " ";
        }
        cout << endl;
    }

    int choice = 0;
    while (true)
    {
        menu();
        cin >> choice;

        if (choice == 1)
        {
            srand(time(NULL));
            int start, goal;
            tree.root = tree.insertnode(tree.root, 100, 0);
            findStartAndGoal(map, start, goal);

            const int numNodes = MAP_WIDTH * MAP_HEIGHT;
            int adjMatrix[numNodes][numNodes];

            for (int i = 0; i < numNodes; i++) {
                for (int j = 0; j < numNodes; j++) {
                    adjMatrix[i][j] = 0;
                }
            }

            for (int i = 0; i < MAP_WIDTH; i++) {
                for (int j = 0; j < MAP_HEIGHT; j++) {
                    if (map[i][j] == '#')
                    {
                        adjMatrix[i][j] = 100;
                    }
                    if (map[i][j] == '%')
                    {
                        adjMatrix[i][j] = 100;
                    }

                    if (map[i][j] == 'J')
                    {
                        tree.root = tree.insertnode(tree.root,rand() % 100 + 101, 50);
                    }
                    if (map[i][j] == 'W')
                    {
                        tree.root = tree.insertnode(tree.root,rand() % 100 + 101, 30);
                    }
                    if (map[i][j] == 'P')
                    {
                        tree.root = tree.insertnode(tree.root,rand() % 100 + 101, 70);
                    }
                    if (map[i][j] == '@')
                    {
                        tree.root = tree.insertnode(tree.root,rand() % 100 + 101, -30);
                    }
                    if (map[i][j] == '$')
                    {
                        tree.root = tree.insertnode(tree.root,rand() % 100 + 101, -70);
                    }
                    if (map[i][j] == '&')
                    {
                        tree.root = tree.insertnode(tree.root,rand() % 100 + 101, -50);
                    }
                    if (map[i][j] == 'C' || map[i][j] == '*') {
                        int source = i * MAP_WIDTH + j;
                        if (i > 0 && (map[i - 1][j] == 'C' || map[i - 1][j] == '*' || map[i - 1][j] == '#')) {
                            // Up
                            int target = (i - 1) * MAP_WIDTH + j;
                            adjMatrix[source][target] = 1;
                        }
                        if (i < MAP_WIDTH - 1 && (map[i + 1][j] == 'C' || map[i + 1][j] == '*' || map[i + 1][j] == '#')) {
                            // Down
                            int target = (i + 1) * MAP_WIDTH + j;
                            adjMatrix[source][target] = 1;
                        }
                        if (j > 0 && (map[i][j - 1] == 'C' || map[i][j - 1] == '*' || map[i][j - 1] == '#')) {
                            // Left
                            int target = i * MAP_WIDTH + (j - 1);
                            adjMatrix[source][target] = 1;
                        }
                        if (j < MAP_HEIGHT - 1 && (map[i][j + 1] == 'C' || map[i][j + 1] == '*' || map[i][j + 1] == '#')) {
                            // Right
                            int target = i * MAP_WIDTH + (j + 1);
                            adjMatrix[source][target] = 1;
                        }
                    }
                }
            }

            for (int i = 0; i < numNodes; i++) {
                for (int j = 0; j < numNodes; j++) {
                    cout<< adjMatrix[i][j] << " ";
                }
                cout << endl;
            }


            int shortestDistance = dijkstra(adjMatrix, start, goal);

            cout << "Shortest distance: " << shortestDistance << endl;

            printTree(tree.root);
            Score(tree.root);

        }
        else if (choice == 0)
        {
            //PlaySound(NULL, NULL, SND_PURGE);

            // Wait for the music thread to finish
            //musicThread.join();
            
            exit(1);

        }
        else if (choice == 2)
        {
            int start;
            cout << "\t\t\t\t|--------------------------------------------------------|\n";
            cout << "\t\t\t\t|               Enter Start Index :                      |\n";
            cout << "\t\t\t\t|--------------------------------------------------------|\n";
            cin >> start;;

            int goal;
            findGoal(map, start, goal);
            tree.root = tree.insertnode(tree.root, 100, 0);
            const int numNodes = MAP_WIDTH * MAP_HEIGHT;
            int adjMatrix[numNodes][numNodes];

            for (int i = 0; i < numNodes; i++) {
                for (int j = 0; j < numNodes; j++) {
                    adjMatrix[i][j] = 0;
                }
            }

            for (int i = 0; i < MAP_WIDTH; i++) {
                for (int j = 0; j < MAP_HEIGHT; j++) {
                    if (map[i][j] == '#')
                    {
                        adjMatrix[i][j] = -1;
                    }
                    if (map[i][j] == '%')
                    {
                        adjMatrix[i][j] = -1;
                    }
                    if (map[i][j] == 'J')
                    {
                        tree.root = tree.insertnode(tree.root, rand() % 100 + 101, 50);
                    }
                    if (map[i][j] == 'W')
                    {
                        tree.root = tree.insertnode(tree.root, rand() % 100 + 101, 30);
                    }
                    if (map[i][j] == 'P')
                    {
                        tree.root = tree.insertnode(tree.root, rand() % 100 + 101, 70);
                    }
                    if (map[i][j] == '@')
                    {
                        tree.root = tree.insertnode(tree.root, rand() % 100 + 101, -30);
                    }
                    if (map[i][j] == '$')
                    {
                        tree.root = tree.insertnode(tree.root, rand() % 100 + 101, -70);
                    }
                    if (map[i][j] == '&')
                    {
                        tree.root = tree.insertnode(tree.root, rand() % 100 + 101, -50);
                    }
                    if (map[i][j] == 'C' || map[i][j] == '*') {
                        int source = i * MAP_WIDTH + j;
                        if (i > 0 && (map[i - 1][j] == 'C' || map[i - 1][j] == '*' || map[i - 1][j] == '#')) {
                            // Up
                            int target = (i - 1) * MAP_WIDTH + j;
                            adjMatrix[source][target] = 1;
                        }
                        if (i < MAP_WIDTH - 1 && (map[i + 1][j] == 'C' || map[i + 1][j] == '*' || map[i + 1][j] == '#')) {
                            // Down
                            int target = (i + 1) * MAP_WIDTH + j;
                            adjMatrix[source][target] = 1;
                        }
                        if (j > 0 && (map[i][j - 1] == 'C' || map[i][j - 1] == '*' || map[i][j - 1] == '#')) {
                            // Left
                            int target = i * MAP_WIDTH + (j - 1);
                            adjMatrix[source][target] = 1;
                        }
                        if (j < MAP_HEIGHT - 1 && (map[i][j + 1] == 'C' || map[i][j + 1] == '*' || map[i][j + 1] == '#')) {
                            // Right
                            int target = i * MAP_WIDTH + (j + 1);
                            adjMatrix[source][target] = 1;
                        }
                    }
                }
            }


            int shortestDistance = dijkstra(adjMatrix, start, goal);

            cout << "Shortest distance: " << shortestDistance << endl;

            printTree(tree.root);
            Score(tree.root);

        }
        else if (choice == 3)
        {
            const int numNodes = MAP_WIDTH * MAP_HEIGHT;
            int adjMatrix[numNodes][numNodes];
            //tree.root = tree.insertnode(tree.root, 100, 0);
            for (int i = 0; i < numNodes; i++) {
                for (int j = 0; j < numNodes; j++) {
                    adjMatrix[i][j] = 0;
                }
            }

            for (int i = 0; i < MAP_WIDTH; i++) {
                for (int j = 0; j < MAP_HEIGHT; j++) {
                    if (map[i][j] == '#')
                    {
                        adjMatrix[i][j] = -100;
                    }
                    if (map[i][j] == '%')
                    {
                        adjMatrix[i][j] = -100;
                    }
                    if (map[i][j] == 'J')
                    {
                        tree.root = tree.insertnode(tree.root, rand() % 100 + 101, 50);
                    }
                    if (map[i][j] == 'W')
                    {
                        tree.root = tree.insertnode(tree.root, rand() % 100 + 101, 30);
                    }
                    if (map[i][j] == 'P')
                    {
                        tree.root = tree.insertnode(tree.root, rand() % 100 + 101, 70);
                    }
                    if (map[i][j] == '@')
                    {
                        tree.root = tree.insertnode(tree.root, rand() % 100 + 101, -30);
                    }
                    if (map[i][j] == '$')
                    {
                        tree.root = tree.insertnode(tree.root, rand() % 100 + 101, -70);
                    }
                    if (map[i][j] == '&')
                    {
                        tree.root = tree.insertnode(tree.root, rand() % 100 + 101, -50);
                    }
                    if (map[i][j] == 'C' || map[i][j] == '*') {
                        int source = i * MAP_WIDTH + j;
                        if (i > 0 && (map[i - 1][j] == 'C' || map[i - 1][j] == '*' || map[i - 1][j] == '#')) {
                            // Up
                            int target = (i - 1) * MAP_WIDTH + j;
                            adjMatrix[source][target] = 1;
                        }
                        if (i < MAP_WIDTH - 1 && (map[i + 1][j] == 'C' || map[i + 1][j] == '*' || map[i + 1][j] == '#')) {
                            // Down
                            int target = (i + 1) * MAP_WIDTH + j;
                            adjMatrix[source][target] = 1;
                        }
                        if (j > 0 && (map[i][j - 1] == 'C' || map[i][j - 1] == '*' || map[i][j - 1] == '#')) {
                            // Left
                            int target = i * MAP_WIDTH + (j - 1);
                            adjMatrix[source][target] = 1;
                        }
                        if (j < MAP_HEIGHT - 1 && (map[i][j + 1] == 'C' || map[i][j + 1] == '*' || map[i][j + 1] == '#')) {
                            // Right
                            int target = i * MAP_WIDTH + (j + 1);
                            adjMatrix[source][target] = 1;
                        }
                    }
                }
            }

            cout << "Kruskal : " << endl;
            kruskal(100, adjMatrix);

            /*printTree(tree.root);
            Score(tree.root);*/

        }
        else if (choice == 4)
        {
            const int numNodes = MAP_WIDTH * MAP_HEIGHT;
            int adjMatrix[numNodes][numNodes];

            //tree.root = tree.insertnode(tree.root, 100, 0);
            for (int i = 0; i < numNodes; i++) {
                for (int j = 0; j < numNodes; j++) {
                    adjMatrix[i][j] = 0;
                }
            }

            for (int i = 0; i < MAP_WIDTH; i++) {
                for (int j = 0; j < MAP_HEIGHT; j++) {
                    if (map[i][j] == '#')
                    {
                        adjMatrix[i][j] = 100;
                    }
                    if (map[i][j] == '%')
                    {
                        adjMatrix[i][j] = 100;
                    }
                    if (map[i][j] == 'J')
                    {
                        tree.root = tree.insertnode(tree.root,rand() % 100 + 101, 50);
                    }
                    if (map[i][j] == 'W')
                    {
                        tree.root = tree.insertnode(tree.root,rand() % 100 + 101, 30);
                    }
                    if (map[i][j] == 'P')
                    {
                        tree.root = tree.insertnode(tree.root,rand() % 100 + 101, 70);
                    }
                    if (map[i][j] == '@')
                    {
                        tree.root = tree.insertnode(tree.root,rand() % 100 + 101, -30);
                    }
                    if (map[i][j] == '$')
                    {
                        tree.root = tree.insertnode(tree.root,rand() % 100 + 101, -70);
                    }
                    if (map[i][j] == '&')
                    {
                        tree.root = tree.insertnode(tree.root,rand() % 100 + 101, -50);
                    }
                    if (map[i][j] == 'C' || map[i][j] == '*') {
                        int source = i * MAP_WIDTH + j;
                        if (i > 0 && (map[i - 1][j] == 'C' || map[i - 1][j] == '*' || map[i - 1][j] == '#')) {
                            // Up
                            int target = (i - 1) * MAP_WIDTH + j;
                            adjMatrix[source][target] = 1;
                        }
                        if (i < MAP_WIDTH - 1 && (map[i + 1][j] == 'C' || map[i + 1][j] == '*' || map[i + 1][j] == '#')) {
                            // Down
                            int target = (i + 1) * MAP_WIDTH + j;
                            adjMatrix[source][target] = 1;
                        }
                        if (j > 0 && (map[i][j - 1] == 'C' || map[i][j - 1] == '*' || map[i][j - 1] == '#')) {
                            // Left
                            int target = i * MAP_WIDTH + (j - 1);
                            adjMatrix[source][target] = 1;
                        }
                        if (j < MAP_HEIGHT - 1 && (map[i][j + 1] == 'C' || map[i][j + 1] == '*' || map[i][j + 1] == '#')) {
                            // Right
                            int target = i * MAP_WIDTH + (j + 1);
                            adjMatrix[source][target] = 1;
                        }
                    }
                }
            }

            prim(numNodes, adjMatrix);
            printMST(numNodes);

            /*printTree(tree.root);
            Score(tree.root);*/

        }
        else if (choice == 5)
        {
            const int INF = 1000000000;
            tree.root = tree.insertnode(tree.root, 100, 0);
            const int numNodes = MAP_WIDTH * MAP_HEIGHT;
            int adjMatrix[numNodes][numNodes];

            for (int i = 0; i < numNodes; i++) {
                for (int j = 0; j < numNodes; j++) {
                    adjMatrix[i][j] = INF;
                }
            }

            for (int i = 0; i < MAP_WIDTH; i++) {
                for (int j = 0; j < MAP_HEIGHT; j++) {
                    if (map[i][j] == '#')
                    {
                        adjMatrix[i][j] = 100;
                    }
                    if (map[i][j] == '%')
                    {
                        adjMatrix[i][j] = 100;
                    }

                    if (map[i][j] == 'J')
                    {
                        tree.root = tree.insertnode(tree.root,rand() % 100 + 101, 50);
                    }
                    if (map[i][j] == 'W')
                    {
                        tree.root = tree.insertnode(tree.root,rand() % 100 + 101, 30);
                    }
                    if (map[i][j] == 'P')
                    {
                        tree.root = tree.insertnode(tree.root,rand() % 100 + 101, 70);
                    }
                    if (map[i][j] == '@')
                    {
                        tree.root = tree.insertnode(tree.root,rand() % 100 + 101, -30);
                    }
                    if (map[i][j] == '$')
                    {
                        tree.root = tree.insertnode(tree.root,rand() % 100 + 101, -70);
                    }
                    if (map[i][j] == '&')
                    {
                        tree.root = tree.insertnode(tree.root,rand() % 100 + 101, -50);
                    }
                    if (map[i][j] == 'C' || map[i][j] == '*') {
                        int source = i * MAP_WIDTH + j;
                        if (i > 0 && (map[i - 1][j] == 'C' || map[i - 1][j] == '*' || map[i - 1][j] == '#')) {
                            // Up
                            int target = (i - 1) * MAP_WIDTH + j;
                            adjMatrix[source][target] = 1;
                        }
                        if (i < MAP_WIDTH - 1 && (map[i + 1][j] == 'C' || map[i + 1][j] == '*' || map[i + 1][j] == '#')) {
                            // Down
                            int target = (i + 1) * MAP_WIDTH + j;
                            adjMatrix[source][target] = 1;
                        }
                        if (j > 0 && (map[i][j - 1] == 'C' || map[i][j - 1] == '*' || map[i][j - 1] == '#')) {
                            // Left
                            int target = i * MAP_WIDTH + (j - 1);
                            adjMatrix[source][target] = 1;
                        }
                        if (j < MAP_HEIGHT - 1 && (map[i][j + 1] == 'C' || map[i][j + 1] == '*' || map[i][j + 1] == '#')) {
                            // Right
                            int target = i * MAP_WIDTH + (j + 1);
                            adjMatrix[source][target] = 1;
                        }
                    }
                }
            }


            for (int k = 0; k < MAP_HEIGHT * MAP_WIDTH; k++) {  //all vertices
                for (int i = 0; i < MAP_HEIGHT * MAP_WIDTH; i++) {  //source vertices
                    for (int j = 0; j < MAP_HEIGHT * MAP_WIDTH; j++) {  //destination vertices
                        if (adjMatrix[i][k] != INF && adjMatrix[k][j] != INF) { //check path
                            adjMatrix[i][j] = min(adjMatrix[i][j], adjMatrix[i][k] + adjMatrix[k][j]);
                        }
                    }
                }
            }
            int start, goal;
            findStartAndGoal(map, start, goal);

            if (adjMatrix[0][goal] == INT_MAX) {
                cout << "No path from source to goal" << endl;
            }
            else {
                cout << "Shortest distance from source to goal: " << adjMatrix[0][goal] << endl;
            }
            printTree(tree.root);
            Score(tree.root);

        }

        else if (choice == 6)
        {
            const int INF = 1000000000;
            tree.root = tree.insertnode(tree.root, 100, 0);
            int start;
            cout << "\t\t\t\t|--------------------------------------------------------|\n";
            cout << "\t\t\t\t|               Enter Start Index :                      |\n";
            cout << "\t\t\t\t|--------------------------------------------------------|\n";
            cin >> start;;

            int goal;
            findGoal(map, start, goal);


            const int numNodes = MAP_WIDTH * MAP_HEIGHT;
            int adjMatrix[numNodes][numNodes];

            for (int i = 0; i < numNodes; i++) {
                for (int j = 0; j < numNodes; j++) {
                    adjMatrix[i][j] = INF;
                }
            }

            for (int i = 0; i < MAP_WIDTH; i++) {
                for (int j = 0; j < MAP_HEIGHT; j++) {
                    if (map[i][j] == '#')
                    {
                        adjMatrix[i][j] = 100;
                    }
                    if (map[i][j] == '%')
                    {
                        adjMatrix[i][j] = 100;
                    }
                    if (map[i][j] == 'J')
                    {
                        tree.root = tree.insertnode(tree.root,rand() % 100 + 101, 50);
                    }
                    if (map[i][j] == 'W')
                    {
                        tree.root = tree.insertnode(tree.root,rand() % 100 + 101, 30);
                    }
                    if (map[i][j] == 'P')
                    {
                        tree.root = tree.insertnode(tree.root,rand() % 100 + 101, 70);
                    }
                    if (map[i][j] == '@')
                    {
                        tree.root = tree.insertnode(tree.root,rand() % 100 + 101, -30);
                    }
                    if (map[i][j] == '$')
                    {
                        tree.root = tree.insertnode(tree.root,rand() % 100 + 101, -70);
                    }
                    if (map[i][j] == '&')
                    {
                        tree.root = tree.insertnode(tree.root,rand() % 100 + 101, -50);
                    }
                    if (map[i][j] == 'C' || map[i][j] == '*') {
                        int source = i * MAP_WIDTH + j;
                        if (i > 0 && (map[i - 1][j] == 'C' || map[i - 1][j] == '*' || map[i - 1][j] == '#')) {
                            // Up
                            int target = (i - 1) * MAP_WIDTH + j;
                            adjMatrix[source][target] = 1;
                        }
                        if (i < MAP_WIDTH - 1 && (map[i + 1][j] == 'C' || map[i + 1][j] == '*' || map[i + 1][j] == '#')) {
                            // Down
                            int target = (i + 1) * MAP_WIDTH + j;
                            adjMatrix[source][target] = 1;
                        }
                        if (j > 0 && (map[i][j - 1] == 'C' || map[i][j - 1] == '*' || map[i][j - 1] == '#')) {
                            // Left
                            int target = i * MAP_WIDTH + (j - 1);
                            adjMatrix[source][target] = 1;
                        }
                        if (j < MAP_HEIGHT - 1 && (map[i][j + 1] == 'C' || map[i][j + 1] == '*' || map[i][j + 1] == '#')) {
                            // Right
                            int target = i * MAP_WIDTH + (j + 1);
                            adjMatrix[source][target] = 1;
                        }
                    }
                }
            }


            for (int k = 0; k < MAP_HEIGHT * MAP_WIDTH; k++) {
                for (int i = 0; i < MAP_HEIGHT * MAP_WIDTH; i++) {
                    for (int j = 0; j < MAP_HEIGHT * MAP_WIDTH; j++) {
                        if (adjMatrix[i][k] != INF && adjMatrix[k][j] != INF) {
                            adjMatrix[i][j] = min(adjMatrix[i][j], adjMatrix[i][k] + adjMatrix[k][j]);
                        }
                    }
                }
            }

            if (adjMatrix[start][goal] == INT_MAX) {
                cout << "No path from source to goal" << endl;
            }
            else {
                cout << "Shortest distance from source to goal: " << adjMatrix[start][goal] << endl;
            }

            printTree(tree.root);
            Score(tree.root);
        }
    }

    return 0;
}
