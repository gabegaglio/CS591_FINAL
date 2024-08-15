#define NOMINMAX
#include<iostream>
#include<fstream>
#include<string>
#include<windows.h>
#include<vector>
#include<string>
#include <stdlib.h>
#include <queue>
#include <limits>
#include<utility>
#include<functional>
#include<tuple>

using namespace std;
using std::max;

void menu() {
    cout << "Enter 1 for dijktras" << endl;
    cout << "Enter 2 for prims" << endl;
}


pair<vector<int>, vector<int>> dijkstra(vector<pair<int, int>>& edges, vector<int>& weights, int vertices, int start) {
    const int INF = numeric_limits<int>::max();
    vector<int> dist(vertices + 1, INF); // Initialize distances to infinity
    dist[start] = 0; // Distance from start vertex to itself is 0

    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq; // Min heap
    vector<int> parent(vertices + 1, -1); // To store parent vertices

    pq.push({ 0, start }); // Push the start vertex with distance 0

    while (!pq.empty()) {
        int u = pq.top().second;
        int u_dist = pq.top().first;
        pq.pop();

        // If the current distance is greater than the stored distance, skip
        if (u_dist > dist[u])
            continue;

        // Iterate through the adjacent vertices
        for (size_t i = 0; i < edges.size(); ++i) {
            int v, weight;
            if (edges[i].first == u) {
                v = edges[i].second;
                weight = weights[i];
            }
            else if (edges[i].second == u) {
                v = edges[i].first;
                weight = weights[i];
            }
            else {
                continue;
            }

            // Relaxation step
            if (dist[u] + weight < dist[v]) {
                dist[v] = dist[u] + weight;
                pq.push({ dist[v], v });
                parent[v] = u; // Update parent of v
            }
        }
    }

    // Build shortest path
    vector<int> shortest_path;
    int current = vertices;
    while (current != -1) {
        shortest_path.push_back(current);
        current = parent[current];
    }
    reverse(shortest_path.begin(), shortest_path.end());

    return { shortest_path, dist };
}

// Function to perform Prim's algorith
pair<vector<pair<int, int>>, int> prim(const vector<pair<int, int>>& edges, const vector<int>& weights, int vertices) {
    vector<bool> in_tree(vertices + 1, false); // To mark vertices included in the tree
    vector<pair<int, int>> tree_edges; // To store the edges of the minimum spanning tree
    vector<int> individualWeight;

    int total_weight = 0; // To store the total weight of the minimum spanning tree

    // Priority queue to store edges, sorted by weight. (weight, from, to)
    priority_queue<tuple<int, int, int>, vector<tuple<int, int, int>>, greater<tuple<int, int, int>>> pq;

    // Start with vertex 1 (or any arbitrary starting vertex)
    int start_vertex = 1;
    in_tree[start_vertex] = true; // Mark the start vertex as included in MST
    for (size_t i = 0; i < edges.size(); ++i) {
        if (edges[i].first == start_vertex || edges[i].second == start_vertex) {
            pq.push(make_tuple(weights[i], edges[i].first, edges[i].second));
        }
    }

    while (!pq.empty() && tree_edges.size() < vertices - 1) {
        int weight, u, v;
        tie(weight, u, v) = pq.top();
        pq.pop();

        // Check if we're adding an edge to the MST
        if (in_tree[u] && in_tree[v]) {
            continue; // Both vertices are already in the tree
        }

        // Determine the new vertex to add to MST
        int new_vertex = in_tree[u] ? v : u;

        tree_edges.push_back({ u, v });


        total_weight += weight;

        in_tree[new_vertex] = true;

        // Add new edges to the priority queue
        for (size_t i = 0; i < edges.size(); ++i) {
            if (edges[i].first == new_vertex && !in_tree[edges[i].second]) {
                pq.push(make_tuple(weights[i], edges[i].first, edges[i].second));
            }
            else if (edges[i].second == new_vertex && !in_tree[edges[i].first]) {
                pq.push(make_tuple(weights[i], edges[i].first, edges[i].second));
            }
        }
    }

    return { tree_edges, total_weight };
}

int main() {

    vector<string> sv; //sv = string vector will store unedited strings from each line
    string filePath = "";

 
        menu();
        int choice;
        cin >> choice;
        system("cls");
        if (choice == 1) {
            filePath = "dij.txt";
        }
        if (choice == 2) {
            filePath = "prim.txt";
        }

        ifstream file(filePath);

        if (!file.is_open()) { //throws error if cant find
            cerr << "Cannot open file, make sure prim.txt and dij.txt is in file directory" << endl;
            cerr << "1. Go to root folder for program. " << endl;
            cerr << "2. Place graph.txt in secondary program folder where CSS591.cpp source file is" << endl;
            system("pause");
            return 1;
        }


        //CASE SWITCHES
        switch (choice) {
        case 1:
            if (file.is_open()) {
                // FIRST LINE - get vertice and edge amounts
                string firstLine;
                getline(file, firstLine);
                file.ignore(); // stop at next line

                // find position of first space
                int edgeSpace = firstLine.find(' ');

                // extract substrings before and after space
                string vert = firstLine.substr(0, edgeSpace);
                string edg = firstLine.substr(edgeSpace + 1);

                // convert substrings to integers
                int vertices = stoi(vert);
                int edgeAmount = stoi(edg);

                // SECOND LINE - get 1 to n
                string path;
                getline(file, path);
                file.ignore(); // stop at next line

                // find position of first space
                int spacePos = path.find(' ');

                // Extract the substrings before and after the space
                string startLoc = path.substr(0, spacePos);  // gets string until first space
                string endLoc = path.substr(spacePos + 1);    // gets string of after space

                // Convert the substrings to integers
                int start = stoi(startLoc);
                int end = stoi(endLoc);

                // MAKING THE VECTOR 
                vector<pair<int, int>> edges;
                vector<int> weights;

                // first take in all the strings and add them to vector
                string input;
                while (getline(file, input)) {
                    sv.push_back(input);
                }
                // now going through string vector to extract edges and weights
                for (const string& str : sv) {
                    // Trim leading and trailing whitespace, was having an issue with stoi arguments out of range, possibly because of whitespace
                    string trimmedStr = str;
                    trimmedStr.erase(trimmedStr.begin(), find_if(trimmedStr.begin(), trimmedStr.end(), [](unsigned char ch) {
                        return !isspace(ch);
                        }));
                    trimmedStr.erase(find_if(trimmedStr.rbegin(), trimmedStr.rend(), [](unsigned char ch) {
                        return !isspace(ch);
                        }).base(), trimmedStr.end());

                    // Find the positions of the two spaces
                    size_t firstSpace = trimmedStr.find(' ');
                    size_t secondSpace = trimmedStr.find(' ', firstSpace + 1);

                    // Check if both spaces are found
                    if (firstSpace != string::npos && secondSpace != string::npos) {
                        // Extract edges and weight
                        int edge1 = stoi(trimmedStr.substr(0, firstSpace));
                        int edge2 = stoi(trimmedStr.substr(firstSpace + 1, secondSpace - firstSpace - 1)); // second minus the before first space - 1
                        int weight = stoi(trimmedStr.substr(secondSpace + 1));

                        // Add edge and weight to the respective vectors
                        edges.push_back({ edge1, edge2 });
                        weights.push_back(weight);
                    }

                }

                // OUTPUT SECTION
                cout << vertices << " Vertices" << endl;
                cout << "Start: " << start << " -> " << end << endl;

                cout << edgeAmount << " Edges:\n";
                for (size_t i = 0; i < edges.size(); ++i) {
                    cout << "(" << edges[i].first << ", " << edges[i].second << ") = ";
                    cout << weights[i] << " ";
                    cout << endl;
                }
                cout << endl;

                pair<vector<int>, vector<int>> result = dijkstra(edges, weights, vertices, start);
                vector<int> shortest_path = result.first;
                vector<int> distances = result.second;

                // Output shortest path and total weight
                cout << "Shortest path from " << start << " to " << end << ":" << endl;
                cout << "[";
                for (size_t i = 0; i < shortest_path.size(); ++i) {
                    cout << shortest_path[i];
                    if (i != shortest_path.size() - 1)
                        cout << ", ";
                }
                cout << "] ";
                cout << "[" << distances[end] << "]" << endl << endl;
                system("pause");
            }
            break;

        case 2:
            if (file.is_open()) {
                // FIRST LINE - get vertice and edge amounts
                string firstLine;
                getline(file, firstLine);
                file.ignore(); // stop at next line

                // find position of first space
                int edgeSpace = firstLine.find(' ');

                // extract substrings before and after space
                string vert = firstLine.substr(0, edgeSpace);
                string edg = firstLine.substr(edgeSpace + 1);

                // convert substrings to integers
                int vertices = stoi(vert);
                int edgeAmount = stoi(edg);

                // take in edge numbers
                string input;
                while (getline(file, input)) {
                    sv.push_back(input);
                }


                vector<pair<int, int>> edges;
                vector<int> weights;

                // now going through string vector to extract edges and weights
                for (const string& str : sv) {
                    // Trim leading and trailing whitespace, was having an issue with stoi arguments out of range, possibly because of whitespace
                    string trimmedStr = str;
                    trimmedStr.erase(trimmedStr.begin(), find_if(trimmedStr.begin(), trimmedStr.end(), [](unsigned char ch) {
                        return !isspace(ch);
                        }));
                    trimmedStr.erase(find_if(trimmedStr.rbegin(), trimmedStr.rend(), [](unsigned char ch) {
                        return !isspace(ch);
                        }).base(), trimmedStr.end());

                    // Find the positions of the two spaces
                    size_t firstSpace = trimmedStr.find(' ');
                    size_t secondSpace = trimmedStr.find(' ', firstSpace + 1);

                    // Check if both spaces are found
                    if (firstSpace != string::npos && secondSpace != string::npos) {
                        // Extract edges and weight
                        int edge1 = stoi(trimmedStr.substr(0, firstSpace));
                        int edge2 = stoi(trimmedStr.substr(firstSpace + 1, secondSpace - firstSpace - 1));
                        int weight = stoi(trimmedStr.substr(secondSpace + 1));

                        // Add edge and weight to the respective vectors
                        edges.push_back({ edge1, edge2 });
                        weights.push_back(weight);
                    }
                }

                // OUTPUT SECTION
                cout << vertices << " Vertices" << endl;
                cout << edgeAmount << " Edges:\n";
                for (size_t i = 0; i < edges.size(); ++i) {
                    cout << "(" << edges[i].first << ", " << edges[i].second << ") = ";
                    cout << weights[i] << " ";
                    cout << endl;
                }
                cout << endl;

                pair<vector<pair<int, int>>, int> result = prim(edges, weights, vertices);
                vector<pair<int, int>> tree_edges = result.first;
                int total_weight = result.second;

                // Output minimum spanning tree edges and total weight
                cout << "Minimum spanning tree edges:" << endl << endl;
                cout << "[";
                for (size_t i = 0; i < tree_edges.size(); ++i) {
                    cout << "" << tree_edges[i].first << " - " << tree_edges[i].second;
                    if (i != tree_edges.size() - 1) {
                        cout << ", ";
                    }
                }
                cout << "] ";
                cout << "[" << total_weight << "]" << endl << endl;
                system("pause");
            }
            break;
        }
   


    return 0;
}