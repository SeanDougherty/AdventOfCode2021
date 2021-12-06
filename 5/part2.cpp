#include <iostream>
#include <vector>
#include <bits/stdc++.h>
#include <math.h>

using namespace std;

void updateMax(const vector<vector<int>> &endpoints, int &x_max, int &y_max) {
  int x = max(endpoints[0][0], endpoints[1][0]);
  int y = max(endpoints[0][1], endpoints[1][1]);
  if (x > x_max) {
    x_max = x;
  }
  if (y > y_max) {
    y_max = y;
  }
}

vector<int> parseLine(string in) {
  vector<int> endpoints;
  regex regexp("[0-9]+");
  sregex_token_iterator rit(in.begin(), in.end(), regexp);
  sregex_token_iterator rend;
  while (rit != rend) {
    endpoints.push_back(stoi(rit->str()));
    rit++;
  }
  return endpoints;
}

void checkAndSwapOrder(int &start, int &end) {
  if (start > end) {
    int temp = end;
    end = start;
    start = temp;
  }
}

bool checkAndSwapDiagonalOrder(int &x1, int &y1, int &x2, int &y2) {
  if (y1 > y2) {
    int tempx = x1;
    int tempy = y1;
    y1 = y2;
    x1 = x2;
    y2 = tempy;
    x2 = tempx;
  }
  return x1 < x2;
}

void fillY(int constant, int start, int end, vector<vector<int>> &matrix, int &crossings) {
  checkAndSwapOrder(start, end);
  for (int i=start; i <= end; i++) {
    matrix[constant][i]++;
    if (matrix[constant][i] == 2) {
      crossings++;
    }
  } 
}

void fillX(int constant, int start, int end, vector<vector<int>> &matrix, int &crossings) {
  checkAndSwapOrder(start, end);
  for (int i=start; i <= end; i++) {
    matrix[i][constant]++;
    if (matrix[i][constant] == 2) {
      crossings++;
    }
  } 
}

void fillDiagonal(vector<int> diagonal, vector<vector<int>> &matrix, int &crossings) {
  int y1 = diagonal[0], x1 = diagonal[1], y2 = diagonal[2], x2 = diagonal[3];
  bool isLeftToRight = checkAndSwapDiagonalOrder(x1, y1, x2, y2);
  if (isLeftToRight) {
    for (int i=x1; i <= x2; (isLeftToRight ? i++ : i--)) {
      matrix[i][y1]++;
      if (matrix[i][y1] == 2) {
        crossings++;
      }
      y1++;
    }
  } else {
    for (int i=x1; i >= x2; i--) {
      matrix[i][y1]++;
      if (matrix[i][y1] == 2) {
        crossings++;
      }
      y1++;
    }
  }
}

void printMatrix(vector<vector<int>> m) {
  for (int i=0; i < m.size(); i++) {
    for (int j=0; j < m[0].size(); j++) {
      if (m[i][j]) {
        cout << m[i][j];
      } else {
        cout << ".";
      }
    }
    cout << endl;
  }
}

int main()
{
  /*Approach*/
  // pull lines
    //ignore diagonals
    //track largest y and x coordinate
  // build matrix w/ size 10^(log10(x)+1) by 10^(log10(y)+1)
  // iterate over lines, placing them in the matrix.
  // anytime a value goes from 1 to 2, increase a counter by 1.

  /*Code*/
  const int MAT_SIZE = 1000;
  vector<vector<int>> matrix (MAT_SIZE, vector<int>(MAT_SIZE, 0));
  ifstream input ("input.txt");
  string buff;
  vector<int> endpoints;
  vector<vector<int>> lines;
  vector<vector<int>> diagonals;
  while (input.is_open()) {
    // pull lines 
    getline(input, buff);
    if (input.eof()) { input.close(); }
    endpoints = parseLine(buff);
    lines.push_back(endpoints);
  }


  // iterate over lines, placing them in the matrix.
  // anytime a value goes from 1 to 2, increase a counter by 1.
  int crossings = 0;
  for (auto line : lines) {
    if (line[0] == line[2]) {
      fillX(line[0], line[1], line[3], matrix, crossings);
    } else if (line[1] == line[3]) {
      fillY(line[1], line[0], line[2], matrix, crossings);
    } else {
      fillDiagonal(line, matrix, crossings);
    }
  }

  cout << crossings << endl;


  return 0;
}