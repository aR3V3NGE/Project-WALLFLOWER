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
        double h, w, a;
};

struct Node { //nodes for Dijkstras
        double weight;
        int holdIndex;
};

bool output(const double &weight, const vector<int> &route, const map<pair<double, double>, Hold>  &wall, const int &grade, const vector<pair<double, double>> &key) {
        //outputs the path if the weight of the path and the inputted grades match
        if ((grade*50) <= weight && weight <= (grade*50)+50 || (grade >= 10 && weight >= 500)) {
                for (size_t i = 0; i < route.size(); i++) {
                        map<pair<double, double>, Hold>::const_iterator it = wall.find(key[route[i]]);
                        //turns the path of hold indexes into the hold information
                        cout << it->first.first << " " << it->first.second << " " << it->second.w << " " << it->second.h << " ";
                }
                return true; //true if route was found, false if not
        }
        return false;

}

void dijkstrater(vector<vector<double>> graph, vector<int> &route, const int &holds, int start, int finish, double &weight) {
        //runs Dijkstra's algorithm to find routes
        /* Dijsktra's algorithm runs over the indexes of the holds and their weights rather than the holds themselves
         * making it easier to code. The route is a series of indices, and output is converted from index to hold and printed out
         */
        vector<bool> visited (holds, false); //stores visited holds
        vector<double> dist (holds, DBL_MAX); //stores weight of path
        vector<int> prev(holds, -1);// For path reconstruction
        multimap<double, Node> frontier; //stores nodes to be looped through, sorted by weight

        dist[start] = 0;
        frontier.insert({0, {0, start}});

        while (!frontier.empty()) { //Loops until there are no more nodes to loop trough

                multimap<double, Node>::iterator it = frontier.begin();
                Node current = it->second;

                if (visited[current.holdIndex]) {
                        frontier.erase(it);
                        continue;
                }

                visited[current.holdIndex] = true;
                frontier.erase(it);

                if (current.holdIndex == finish) break;

                for (int i = 0; i < holds; i++) {

                        if (visited[i] || i == current.holdIndex) continue;

                        double edgeCost = graph[current.holdIndex][i];

                        if (edgeCost < dist[i]) {
                                dist[i] = edgeCost;
                                prev[i] = current.holdIndex;
                                frontier.insert({edgeCost, {edgeCost, i}});
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
        //finds distance between any the center of two given nodes
        double y1 = it->first.second + it->second.h/2; //Euclidean distance formula
        double y2 = itr->first.second + itr->second.h/2;

        double x1 = it->first.first - it->second.w/2;
        double x2 = itr->first.first - itr->second.w/2;

        double y = pow(y1 - y2, 2);
        double x = pow(x1 - x2, 2);

        return sqrt(y+x);
}

vector<vector<double>> graphify(const map<pair<double, double>, Hold> &wall, int &holds, vector<pair<double, double>> &key) {
        //turns the input into an adjacency list
        vector<vector<double>> graph (holds, vector<double>(holds));

        int i = 0;
        for (map<pair<double, double>, Hold>::const_iterator it = wall.begin(); it != wall.end(); it++) {
                //turns wall into an adjacency list, mapping the cost of traversal to every edge
                int j = 0;
                key.push_back(it->first); //allows to get hold from hold index
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

void unscrew(vector<vector<double>> &graph, const vector<int> &route, map< pair<double, double>, Hold> &wall, const vector<pair<double, double>> &key, int &holds) {
        //removes a hold from the wall, unscrewing it
        int x = route[route.size()/2]; //finds a hold in the middle of the existing route
        for (size_t i = 0; i < graph.size(); i++) {
                //erases a hold from the adjacency list by setting its weight to double max so it won't ever be chosen by Dijkstra's algorithm
                graph[x][i] = DBL_MAX;
                graph[i][x] = DBL_MAX;
        }
        holds--;
}


int main(int argc, char* argv[]) {

        //reads in the holds from the input file
        ifstream fin;
        fin.open(argv[1]);

        //creates the wall, sorting by x and y coordinates
        map<pair<double, double>, Hold> wall;
        int vGrade, holds;
        fin >> vGrade;
        double x, y, h, w, a;

        while (fin >> x >> y >> w >> h >> a) {
                wall[{x, y}].h = h;
                wall[{x, y}].w = w;
                wall[{x, y}].a = a;
        }

        fin.close();
        holds = wall.size();

        //creates adjacency list
        vector<pair<double, double>> key;
        vector<vector<double>> graph = graphify(wall, holds, key);

        //chooses random start and finish
        srand(time(0));
        int start = rand() % holds/5  + 4*holds/5;
        int finish = rand() % holds/5;
        bool graded = false;

        while(!graded && holds > 2) { //runs until adequate route is found or it is determined that no route can be created
               
                vector<int> route;
                double weight = 0;
                dijkstrater(graph, route, holds, start, finish, weight); //runs dijkstra to get routes
                graded = output(weight, route, wall, vGrade, key); //sees if a grade and weight match and prints the path for that route
                unscrew(graph, route, wall, key, holds); //removes holds
        }

        if (graded) return 0;
        else cout << "-1";
}
