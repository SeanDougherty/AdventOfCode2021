#include <iostream>
#include <vector>
#include <bits/stdc++.h>

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

void printMatrix(vector<vector<int>> &m) {
  for (auto row : m) {
    for (auto el : row) {
      cout << el << " ";
    }
    cout << endl;
  }
  cout << endl;
}

void flash(int i, int j, vector<vector<int>> &m, set<pair<int,int>> &toReset) {
  set<pair<int,int>> dirs = {{0,1},{1,1},{1,0},{1,-1},{-1,1},{0,-1},{-1,0},{-1,-1}};
  stack<pair<int,int>> toVisit;
  toVisit.push({i,j});
  toReset.insert({i,j});
  m[i][j] = -2000;
  int x, y, dx, dy;
  pair<int,int> loc;
  while (!toVisit.empty()) {
    loc = toVisit.top();
    toVisit.pop();
    y = loc.first;
    x = loc.second;
    for (auto dir : dirs) {
      dy = dir.first;
      dx = dir.second;
      if (y+dy >= 0 && y+dy < m.size() && x+dx >= 0 && x+dx < m[y+dy].size()) {
        m[y+dy][x+dx]++;
        if (m[y+dy][x+dx] > 9) {
          toVisit.push({y+dy,x+dx});
          toReset.insert({y+dy,x+dx});
          m[y+dy][x+dx] = -2000;
        }
      }
    }
  }
}

bool step(vector<vector<int>> &m) {
  set<pair<int,int>> toReset;
  for (int i=0; i < m.size(); i++) {
    for (int j=0; j < m[i].size(); j++) {
      m[i][j]++;
    }
  }
  for (int i=0; i < m.size(); i++) {
    for (int j=0; j < m[i].size(); j++) {
      if (m[i][j] > 9) {
        flash(i, j, m, toReset);
      }
    }
  }
  for (auto loc : toReset) {
    m[loc.first][loc.second] = 0;
  }
  return toReset.size() == (m.size() * m[0].size());
}

int simulate(vector<vector<int>> &m, int n) {
  int steps = 1;
  while (steps < n) {
    if (step(m)) {
      return steps;
    }
    steps++;
  }
  return steps;
}

int main()
{

  vector<vector<int>> m = buildMatrix();
  int n = 10000;
  cout << simulate(m, n) << endl;

  return 0;
}