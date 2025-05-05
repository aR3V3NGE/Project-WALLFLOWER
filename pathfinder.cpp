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

using namespace std;

struct Hold {

        int h, w, a;
};

struct Node {

        int weight;

        int currRow;
        int currCol;

        int prevRow;
        int prevCol;
};

void output(const int &weight, const vector<pair<int, int>> &route, const map<pair<int, int>, Hold>  &wall, const int &grade) {

        if (grade*100 <= weight <= grade*100+100) {
                for (size_t i = 0; i < route.size(); i++) {
                        int x = route[i].first;
                        int xCount = 0;
                        for( map<pair<int, int>, Hold>::const_iterator it = wall.begin(); it != wall.end(); it++) {
                                if (xCount == x) {
                                        cout << it->first.first << " " << it->first.second << " " << it->second.h << " " << it->second.w << " ";
                                }
                                xCount++;
                }
        }
        }
}

void dijkstrater(vector<vector<int>> graph, vector<pair<int, int>> &route, const int &holds, int startRow, int startCol, int endRow, int endCol, int &weight) {

        vector<vector<bool>> visited(holds, vector<bool>(holds, false));
        vector<vector<int>> dist(holds, vector<int>(holds, INT_MAX));
        vector<vector<pair<int, int>>> prev(holds, vector<pair<int, int>>(holds, make_pair(-1, -1))); // For path reconstruction
        multimap<int, Node> frontier; //stores nodes to be looped through, sorted by weight

        Node start;

        dist[startRow][startCol] = 0;
        start.weight = 0;
        start.currRow = startRow;
        start.currCol = startCol;
        start.prevRow = -1;
        start.prevCol = -1;

        frontier.insert(make_pair(0, start));

        int dirRow[4] = {-1, 1, 0, 0};
        int dirCol[4] = {0, 0, -1, 1};
        //add diagonal implementation

        while (!frontier.empty()) { //Loops until there are no more nodes to loop trough

                multimap<int, Node>::iterator it = frontier.begin();

                Node current = it->second;

                if (visited[current.currRow][current.currCol]) {
                        frontier.erase(it);
                        continue;
                }

                visited[current.currRow][current.currCol] = true;
                frontier.erase(it);

                if ((current.currRow == endRow) && (current.currCol == endCol)) break;

                for (int i = 0; i < 4; i++) { //Explores all 4 potential paths (up, down, left, right)

                        if (current.currRow + dirRow[i] < 0 ||
                                        current.currRow + dirRow[i] >= holds ||
                                        current.currCol + dirCol[i] < 0 ||
                                        current.currCol + dirCol[i] >= holds) {

                                continue;
                        }

                        if (visited[current.currRow + dirRow[i]][current.currCol + dirCol[i]]) continue;

                        int edgeCost = graph[current.currRow][current.currCol];
                        int betterDistance = current.weight + edgeCost;

                        if (betterDistance <
                                        dist[current.currRow + dirRow[i]][current.currCol + dirCol[i]]) {

                                dist[current.currRow + dirRow[i]][current.currCol + dirCol[i]] =
                                        betterDistance;

                                prev[current.currRow + dirRow[i]][current.currCol + dirCol[i]] =
                                        make_pair(current.currRow, current.currCol);

                                Node neighbor;

                                neighbor.weight = betterDistance;

                                neighbor.currRow = current.currRow + dirRow[i];
                                neighbor.currCol = current.currCol + dirCol[i];

                                neighbor.prevRow = current.currRow;
                                neighbor.prevCol = current.currCol;

                                frontier.insert(make_pair(neighbor.weight, neighbor));
                        }
                }
        }

        //Making the path after dijkstrating

        int tempEndRow = endRow;
        int tempEndCol = endCol;

        while (!(tempEndRow == -1 && tempEndCol == -1)) {

                route.push_back(make_pair(tempEndRow, tempEndCol));

                pair<int, int> prevPair = prev[tempEndRow][tempEndCol];

                tempEndRow = prevPair.first;
                tempEndCol = prevPair.second;
        }


        //Reversing the path as it gets made backwards, and passing through the weight so it can sort the route to a V grade
        reverse(route.begin(), route.end());
        weight = dist[endRow][endCol];

}

double distanciator(const map<pair<int, int>, Hold> ::const_iterator &it, const map<pair<int, int>, Hold> ::const_iterator &itr) {

        double y1 = it->first.second + it->second.h;
        double y2 = itr->first.second + itr->second.h;

        double x1 = it->first.first - it->second.w;
        double x2 = itr->first.first - itr->second.w;

        double y = pow(y1 - y2, 2);
        double x = pow(x1 - x2, 2);

        return sqrt(y+x);
}

vector<vector<int>> graphify(const map<pair<int, int>, Hold> &wall, int &holds) { //turns the input into graph of edge and node weights

        vector<vector<int>> graph (holds, vector<int>(holds));

        int i = 0;
        for (map<pair<int, int>, Hold>::const_iterator it = wall.begin(); it != wall.end(); it++) {
                int j = 0;
                for (map<pair<int, int>, Hold>::const_iterator itr = wall.begin(); itr != wall.end(); itr++) {
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

void unscrew(vector<vector<int>> &graph, const vector<pair<int, int>> &route, int &holds) {

                int x = route[holds/2].first;
                int y = route[holds/2].second;
                graph[x][y] = INT_MAX;


}

// Main Execution
int main(int argc, char* argv[]) {

        ifstream fin;
        fin.open(argv[1]);

        map<pair<int, int>, Hold> wall;
        int vGrade, holds;
        fin >> vGrade;

        while (fin) {

                int x, y, h, w, a;
                fin >> x >> y >> h >> w >> a;
                wall[{x, y}].h = h;
                wall[{x, y}].w = w;
                wall[{x, y}].a = a;
                holds++;

        }

        fin.close();

        vector<vector<int>> graph = graphify(wall, holds);
        srand(time(0));
        int startRow = rand() % holds + 3*holds/5 - 1;
        int startCol = rand() % holds + 4*holds/5 - 1;
        int endRow = rand() % holds;
        int endCol = rand() % holds;

        while(graph.size() > 2) {

                vector<pair<int, int>> route;
                int weight;
                dijkstrater(graph, route, holds, startRow, startCol, endRow, endCol, weight); //runs dijkstra to get routes
                output(weight, route, wall, vGrade);
                unscrew(graph, route, holds); //removes holds

        }

        return 0;
}
