#include <algorithm>
#include <climits>
#include <iostream>
#include <map>
#include <utility>
#include <vector>
#include <cmath>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <cfloat>

using namespace std;

struct Hold { //stores information about a given hold
        int h, w, a;
};

struct Node { //nodes for dijkstras
        double weight;
        int holdIndex;
};

void output(const double &weight, const vector<int> &route, const map<pair<double, double>, Hold>  &wall, const int &grade, const vector<pair<double, double>> &key) {

        cout << "output: \nweight: " << weight << endl << "path: \n";

        if (grade*3000 <= weight && weight <= grade*3000+1000) {
                for (size_t i = 0; i < route.size(); i++) {
                        map<pair<double, double>, Hold>::const_iterator it = wall.find(key[route[i]]);

                        cout << it->first.first << " " << it->first.second << " " << it->second.h << " " << it->second.w << " " << endl;
                }
                cout << endl;
        }
}

void dijkstrater(vector<vector<double>> graph, vector<int> &route, const int &holds, int start, int finish, double &weight) {

        vector<bool> visited (holds, false);
        vector<double> dist (holds, DBL_MAX);
        vector<int> prev(holds, -1);// For path reconstruction
        multimap<double, Node> frontier; //stores nodes to be looped through, sorted by weight

        dist[start] = 0;

        frontier.insert({0, {0, start}});

        while (!frontier.empty()) { //Loops until there are no more nodes to loop trough

                multimap<double, Node>::iterator it = frontier.begin();

                Node current = it->second;

                if (visited[current.holdIndex]) continue;

                visited[current.holdIndex] = true;
                frontier.erase(it);

                if (current.holdIndex == finish) break;

                for (int i = 0; i < holds; i++) {

                        if (visited[i] || i == current.holdIndex) continue;

                        double edgeCost = graph[current.holdIndex][i];
                        double betterDistance = dist[current.holdIndex] + edgeCost;

                        if (betterDistance < dist[i]) {
                                dist[i] =betterDistance;
                                prev[i] = current.holdIndex;
                                frontier.insert({betterDistance, {betterDistance, i}});
                        }
                }
        }

        //Making the path after dijkstrating
        int tempFinish = finish;
        while (tempFinish != -1) {
                route.push_back(tempFinish);
                tempFinish = prev[tempFinish];
        }


        //Reversing the path as it gets made backwards, and passing through the weight so it can sort the route to a V grade
        reverse(route.begin(), route.end());
        weight = dist[finish];

}

double distanciator(const map<pair<double, double>, Hold> ::const_iterator &it, const map<pair<double, double>, Hold> ::const_iterator &itr) {
        //finds distance any two given nodes
        double y1 = it->first.second + it->second.h/2;
        double y2 = itr->first.second + itr->second.h/2;

        double x1 = it->first.first - it->second.w/2;
        double x2 = itr->first.first - itr->second.w/2;

        double y = pow(y1 - y2, 2);
        double x = pow(x1 - x2, 2);

        return sqrt(y+x);
}

vector<vector<double>> graphify(const map<pair<double, double>, Hold> &wall, int &holds, vector<pair<double, double>> &key) { //turns the input into an adjacency list

        vector<vector<double>> graph (holds, vector<double>(holds));

        int i = 0;
        for (map<pair<double, double>, Hold>::const_iterator it = wall.begin(); it != wall.end(); it++) {
                int j = 0;
                key.push_back(it->first);
                for (map<pair<double, double>, Hold>::const_iterator itr = wall.begin(); itr != wall.end(); itr++) {
                        if (it == itr) graph[i][j] = 0;
                        else {
                                double distance = distanciator(it, itr);
                                double area = 10 / itr->second.a;
                                graph[i][j] = distance + area;
                        }
                        j++;
                }
                i++;
        }

        return graph;
}

void unscrew(vector<vector<double>> &graph, const vector<int> &route, int &holds) {

        int x = route[route.size()/2];
        for (size_t i = 0; i < graph.size(); i++) {
                graph[x][i] = DBL_MAX;
                graph[i][x] = DBL_MAX;
        }
        holds--;

}

// Main Execution
int main(int argc, char* argv[]) {

        ifstream fin;
        fin.open(argv[1]);

        map<pair<double, double>, Hold> wall;
        int vGrade, holds;
        fin >> vGrade;
        double x, y, h, w, a;

        while (fin >> x >> y >> h >> w >> a) {

                wall[{x, y}].h = h;
                wall[{x, y}].w = w;
                wall[{x, y}].a = a;

        }

        fin.close();
        holds = wall.size();

        vector<pair<double, double>> key (holds);
        vector<vector<double>> graph = graphify(wall, holds, key);

        srand(time(0));
        int start = rand() % holds/5  + 4*holds/5;
        int finish = rand() % holds/5;

        while(holds > 2) {

                vector<int> route;
                double weight = 0;
                dijkstrater(graph, route, holds, start, finish, weight); //runs dijkstra to get routes
                output(weight, route, wall, vGrade, key);
                unscrew(graph, route, holds); //removes holds

        }

        return 0;
}
