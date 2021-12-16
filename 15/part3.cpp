#include <iostream>
#include <vector>
#include <bits/stdc++.h>
#include <chrono>

using namespace std;


vector<int> buildRow(string buff) {
  vector<int> row;
  for (char el : buff) {
    if (el >= '0' && el <= '9') {
      row.push_back(el-'0');
    }
  }
  return row;
}

void printMatrix(vector<vector<int>> &m) {
  for (auto row : m) {
    for (auto el : row) {
      cout << el << " ";
    }
    cout << endl;
  }
  cout << endl;
}

vector<vector<int>> fiveX(vector<vector<int>> m) {
  vector<vector<int>> m5 (m.size()*5, vector<int>(m[0].size()*5, 0));
  int dy = m.size(), dx = m[0].size();
  int x, y , newVal;
  for (int i=0; i < m5.size(); i++) {
    for (int j=0; j< m5.size(); j++) {
      if (i < m.size() && j < m[i].size()) {
        m5[i][j] = m[i][j];
      } else {
        if (i < m.size()) {
          y = i;
          // fill from left
          if (j >= m[0].size()) {
            x = j-dx;
          } else {
            x = j;
          }
        } else {
          // fill from above
          y = i-dy;
          x = j;
        }
        
        m5[i][j] = m5.at(y).at(x) + 1 == 10 ? 1 : m5.at(y).at(x) + 1;
      }
    }
  }
  // printMatrix(m5);
  return m5;
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
  cout << matrix.size() << " || " << matrix[0].size() << endl;
  return fiveX(matrix);
}

pair<int,int> findSmallest(vector<vector<int>> d, set<pair<int,int>>visited) {
  int smallest = INT_MAX;
  int x, y;
  for (int i=0; i < d.size(); i++) {
    for (int j=0; j < d[i].size(); j++) {
      if (d[i][j] < smallest && visited.find({i, j}) == visited.end()) {
        smallest = d[i][j];
        x = i;
        y = j;
      }
    }
  }
  return {x, y};
}

void updateNeighbors(pair<int,int> toVisit, vector<vector<int>> m, vector<vector<int>> &d, set<pair<int,int>> &visited) {
  set<pair<int,int>> dirs = {{0,1},{1,0},{-1,0},{0,-1}};
  int i = toVisit.first, j = toVisit.second, di, dj;
  for (auto dir : dirs) {
    di = dir.first;
    dj = dir.second;
    if (i+di < m.size() && i+di >= 0 && j+dj < m[i].size() && j+dj >= 0) {
      if (d[i+di][j+dj] > d[i][j] + m[i+di][j+dj]) {
        d[i+di][j+dj] =  d[i][j] + m[i+di][j+dj];
      }
    }
  }
  visited.insert(toVisit);
}

struct Node {
  int i;
  int j;
  int score;
  bool operator<(const Node& other) const { return score > other.score; }
};

int dijkstra(vector<vector<int>> m) {
  set<pair<int,int>> dirs = {{0,1},{1,0},{-1,0},{0,-1}};
  vector<vector<int>> d (m.size(), vector<int>(m[0].size(), INT_MAX));
  d[0][0] = 0;
  set<pair<int,int>> visited;
  priority_queue<Node> toVisit;
  toVisit.push(Node{0,0,0});
  int smallest, x, y, di, dj;
  while (!toVisit.empty()) {
    x = toVisit.top().j;
    y = toVisit.top().i;
    smallest = toVisit.top().score;
    toVisit.pop();

    if (visited.find({y,x}) != visited.end()) {
      continue;
    } 
    
    if (y == m.size()-1 && x == m[y].size()-1) {
      break;
    }

    for (auto dir : dirs) {
      di = dir.first;
      dj = dir.second;
      if (y+di < m.size() && y+di >= 0 && x+dj < m[y].size() && x+dj >= 0) {
        if (d[y+di][x+dj] > d[y][x] + m[y+di][x+dj]) {
          d[y+di][x+dj] =  d[y][x] + m[y+di][x+dj];
          toVisit.push(Node{y+di,x+dj,d[y+di][x+dj]});
        }
      }
    }
    visited.insert({y,x});
  }
  return d[m.size()-1][m[0].size()-1];
}

int main()
{
  auto start = chrono::high_resolution_clock::now();
  vector<vector<int>> m = buildMatrix();
  cout << m.size() << " :: " << m[0].size() << endl;
  cout << dijkstra(m) << endl;
  auto stop = chrono::high_resolution_clock::now();
  cout << chrono::duration_cast<std::chrono::microseconds>(stop-start).count() << endl;
  return 0;
}