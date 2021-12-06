#include <iostream>
#include <vector>
#include <bits/stdc++.h>
#include <math.h>

using namespace std;

bool isDiagonal(vector<vector<int>> endpoints) {
  return endpoints[0][0] != endpoints[1][0] && endpoints [0][1] != endpoints[1][1];
}

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

vector<vector<int>> parseLine(string in) {
  vector<vector<int>> endpoints;
  vector<int> start, end;
  regex regexp("[0-9]+");
  sregex_token_iterator rit(in.begin(), in.end(), regexp);
  sregex_token_iterator rend;
  int numCt = 0;
  while (rit != rend) {
    istringstream iss(rit->str());
    int i;
    iss >> i;
    if (numCt / 2 < 1) {
      start.push_back(i);
    } else {
      end.push_back(i);
    }
    numCt++;
    rit++;
  }
  endpoints.push_back(start);
  endpoints.push_back(end);
  return endpoints;
}

void checkAndSwapOrder(int &start, int &end) {
  if (start > end) {
    int temp = end;
    end = start;
    start = temp;
  }
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
  ifstream input ("input.txt");
  string buff;
  int x_max = 0, y_max = 0;
  vector<vector<int>> endpoints;
  vector<vector<vector<int>>> lines;
  while (input.is_open()) {
    // pull lines 
    getline(input, buff);
    if (input.eof()) {
      input.close();
    }
    endpoints = parseLine(buff);
    
    //track largest y and x coordinate
    updateMax(endpoints, x_max, y_max);
    
    //ignore diagonals
    if (isDiagonal(endpoints)) {
      continue;
    }

    lines.push_back(endpoints);
  }

  // build matrix w/ size 10^(log10(x)+1) by 10^(log10(y)+1)
  vector<vector<int>> matrix (pow(10,(int) log10(x_max)+1), vector<int>(pow(10,(int) log10(y_max)+1), 0));

  // iterate over lines, placing them in the matrix.
  // anytime a value goes from 1 to 2, increase a counter by 1.
  int crossings = 0;
  for (auto line : lines) {
    if (line[0][0] == line[1][0]) {
      fillY(line[0][0], line[0][1], line[1][1], matrix, crossings);
    }
    if (line[0][1] == line[1][1]) {
      fillX(line[0][1], line[0][0], line[1][0], matrix, crossings);
    }
  }

  cout << crossings << endl;

  return 0;
}