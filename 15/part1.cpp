#include <iostream>
#include <vector>
#include <bits/stdc++.h>

using namespace std;

int buildInput() {
  ifstream input("input.txt");
  string buff;
  while (input.is_open()) {
    getline(input, buff);




    if (input.eof()) {
      input.close();
    }
  }
  return 0;
}

vector<int> buildRow(string buff) {
  vector<int> row;
  for (char el : buff) {
    if (el >= '0' && el <= '9') {
      row.push_back(el-'0');
    }
  }
  return row;
}

vector<vector<int>> buildMatrix() {
  vector<vector<int>> matrix;
  ifstream input("input.txt");
  string buff;
  while (input.is_open()) {
    getline(input, buff);
    matrix.push_back(buildRow(buff));
    if (input.eof()) {
      input.close();
    }
  }
  return matrix;
}

pair<int,int> findSmallest(vector<vector<int>> d, set<pair<int,int>>visited) {
  int smallest = INT_MAX;
  int x, y;
  for (int i=0; i < d.size(); i++) {
    for (int j=0; j < d[i].size(); j++) {
      if (d[i][j] < smallest && visited.find({i, j}) == visited.end()) {
        // cout << "new smallest: " << d[i][j] << endl;
        smallest = d[i][j];
        x = i;
        y = j;
      }
    }
  }
  return {x, y};
}

void updateNeighbors(pair<int,int> toVisit, vector<vector<int>> m, vector<vector<int>> &d, set<pair<int,int>> &visited, vector<vector<vector<int>>> &parents) {
  set<pair<int,int>> dirs = {{0,1},{1,0},{-1,0},{0,-1}};
  int i = toVisit.first, j = toVisit.second, di, dj;
  for (auto dir : dirs) {
    di = dir.first;
    dj = dir.second;
    // cout << "checking: " << i+di << " | " << j+dj << endl;
    if (i+di < m.size() && i+di >= 0 && j+dj < m[i].size() && j+dj >= 0) {
      // cout << d.at(i+di).at(j+dj) << endl;
      // cout << d[i][j] + m[i+di][j+dj] << endl;
      if (d[i+di][j+dj] > d[i][j] + m[i+di][j+dj]) {
        // cout << "1" << endl;
        d[i+di][j+dj] =  d[i][j] + m[i+di][j+dj];
        // cout << "2" << endl;
        parents[i+di][j+dj] = {i, j};
        // cout << "3" << endl;
      }
    }
  }
  visited.insert(toVisit);
}

void printPath(vector<vector<int>> m, vector<vector<int>> &d, vector<vector<vector<int>>> &parents) {
  vector<int> curr = parents[m.size()-1][m[0].size()-1];
  int i, j;
  cout << m[m.size()-1][m[0].size()-1] << " ";
  while (curr[0] != 0 && curr[1] != 0) {
    i = curr[0];
    j = curr[1];
    cout << m[i][j] << " ";
    curr = parents[i][j];
  }
  cout << endl;
}

int dijkstra(vector<vector<int>> m) {
  vector<vector<int>> d (m.size(), vector<int>(m[0].size(), INT_MAX));
  d[0][0] = 0;
  set<pair<int,int>> visited;
  pair<int,int> toVisit;
  vector<vector<vector<int>>> parents (m.size(), vector<vector<int>> (m[0].size(), vector<int>(2, 0)));
  int loopCt = 0;
  while (visited.size() < m.size()*m[0].size()) {
    // cout << loopCt++ << endl;
    // cout << visited.size() << " : " << m.size()*m[0].size() << endl;
    toVisit = findSmallest(d, visited);
    // cout << toVisit.first << " :: " << toVisit.second << endl;
    updateNeighbors(toVisit, m, d, visited, parents);
  }
  printPath(m, d, parents);
  return d[m.size()-1][m[0].size()-1];
}

int main()
{
  vector<vector<int>> m = buildMatrix();
  cout << dijkstra(m) << endl;
  return 0;
}