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

  cout << m.size() << " " << m[0].size() << endl;
  return m;
}

int main()
{
  int dy, dx, curr, sum=0;
  bool isLowPoint;
  vector<vector<int>> matrix = buildMatrix();
  vector<vector<int>> dirs = {{0, -1}, {0,1}, {1, 0}, {-1, 0}};
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
        sum += curr+1;
      }
    }
  }
  cout << "###" << endl;
  cout << sum << endl;
  return 0;
}