#include <algorithm>
#include <climits>
#include <iostream>
#include <map>
#include <utility>
#include <vector>
#include <cmath>
#include <fstream>

using namespace std;

struct Node {

        int weight;

        int currRow;
        int currCol;

        int prevRow;
        int prevCol;
};

void dijkstrater(vector<vector<int>> graph, vector<vector<int>> &route, const int &holds, int startRow, int startCol, int endRow, int endCol) {

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

        vector<pair<int, int>> thePath; //Making the path after dijkstrating

        int tempEndRow = endRow;
        int tempEndCol = endCol;

        while (!(tempEndRow == -1 && tempEndCol == -1)) {

                thePath.push_back(make_pair(tempEndRow, tempEndCol));

                pair<int, int> prevPair = prev[tempEndRow][tempEndCol];

                tempEndRow = prevPair.first;
                tempEndCol = prevPair.second;
        }

        //Reversing the path as it gets made backwards, then output
        reverse(thePath.begin(), thePath.end());
        cout << dist[endRow][endCol] << endl;
        for (size_t i = 0; i < thePath.size(); i++) cout << thePath[i].first << " " << thePath[i].second << endl;

}

double distanciator(const map<int, map<int, double[3]>>::iterator &it, const map<int, map<int, double[3]>>::iterator &itr) {
        //finds distance from midpoint to midpoint of each hold
        double y1 = it->second->first + it->second->second[0]/2;
        double y2 = itr->second->first + itr->second->second[0]/2;

        double x1 = it->first - it->second->second[1]/2;
        double x2 = itr->first - itr->second->second[1]/2;

        double y = pow(y1 + y2, 2);
        double x = pow(x1 + x2, 2);

        return sqrt(y+x);
}

vector<vector<int>> graphify(const map<int, map<int, double[3]>> &wall, int &holds) {

        vector<vector<int>> graph (holds, vector<int>(holds));

        int i = 0;
        for (map<int, map<int, double[3]>>::iterator it = wall.begin(); it != wall.end(); it++) {
                int j = 0;
                for (map<int, map<int, double[3]>>::iterator itr = wall.begin(); itr != wall.end(); itr++) {
                        if (it == itr) graph[i][j] = 0;
                        else {
                                double distance = distanciator(it, itr);
                                double area = 10 / itr->second->second[2];
                                graph[i][j] = distance + area;
                        }
                        j++;
                }
                i++;
        }

        return graph;
}

void unscrew(vector<vector<int>> &graph, const vector<vector<int>> route, int &holds) {

}
// Main Execution

int main(int argc, char* argv[]) {

        ifstream fin;
        fin.open(argv[1]);

        map<int, map<int, double[3]>> wall;
        int vGrade, holds;
        fin >> vGrade;

        while (fin) {

                int x, y, h, w, a;
                fin >> x >> y >> h >> w >> a;
                wall[x][y] = {h, w, a};
                holds++;

        }

        fin.close();

        vector<vector<int>> graph = graphify(wall, holds);

        while(graph.size() > 2) {

                vector<vector<int>> route;
                dijkstrater(graph, route, holds, startRow, startCol, endRow, endCol); //runs dijkstra to get routes
                unscrew(graph, route, holds); //removes holds

        }

        return 0;
}
