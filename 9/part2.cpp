#include <iostream>
#include <vector>
#include <bits/stdc++.h>

using namespace std;

vector<vector<int>> buildMatrix() {
  ifstream input("input.txt");
  string buff;
  int i;
  vector<vector<int>> m;
  while (input.is_open()) {
    vector<int> row;
    getline(input, buff);
    for (auto el : buff) {
        row.push_back(el - '0');
    }
    m.push_back(row);
    if (input.eof()) {
      input.close();
    }
  }

  return m;
}


int bfs(int i, int j, vector<vector<int>> matrix) {
  vector<vector<int>> dirs = {{0, -1}, {0,1}, {1, 0}, {-1, 0}};
  vector<vector<int>> visited (matrix.size(), vector<int>(matrix[0].size(), 0));
  vector<vector<vector<int>>> largestBasins;
  queue<pair<int,int>> toVisit;
  toVisit.push({i,j});
  visited[i][j] = 1;
  int x, y, dx, dy, basinSize = 0;
  while (!toVisit.empty()) {
    y = toVisit.front().first;
    x = toVisit.front().second;
    toVisit.pop();
    if (visited[y][x]) {
      continue;
    }
    basinSize++;
    for (auto dir : dirs) {
      dy = dir[0];
      dx = dir[1];
      if (y+dy < matrix.size() && y+dy >= 0
              && x+dx < matrix[y].size() && x+dx >= 0) {
        if (visited[y+dy][x+dx] == 0) {
          if (matrix[y+dy][x+dx] >= matrix[y][x] && matrix[y+dy][x+dx] != 9) {
            toVisit.push({y+dy, x+dx});
            visited[y+dy][x+dx] = 1;
          }
        }
      }
    }
  }
  return basinSize;
}

int main()
{
  int dy, dx, curr, sum=0;
  bool isLowPoint;
  vector<vector<int>> matrix = buildMatrix();
  vector<vector<int>> dirs = {{0, -1}, {0,1}, {1, 0}, {-1, 0}};
  vector<int> basinSizes;
  for (int i=0; i < matrix.size(); i++) {
    for (int j=0; j< matrix[i].size(); j++) {
      isLowPoint = true;
      curr = matrix[i][j];
      for (auto dir : dirs) {
        dy = dir[0];
        dx = dir[1];
        if (i+dy < matrix.size() && i+dy >= 0
              && j+dx < matrix[i].size() && j+dx >= 0) {
                if (matrix[i+dy][j+dx] <= curr) {
                  isLowPoint = false;
                }
              }
      }
      if (isLowPoint) {
        basinSizes.push_back(bfs(i, j, matrix));
      }
    }
  }
  sort (basinSizes.rbegin(), basinSizes.rend());
  int count = 0;
  int result = 1;
  for (auto basin : basinSizes) {
    if (count > 2) {
      break;
    }
    result *= basin;
    count++;
  }

  cout << result << endl;
  return 0;
}