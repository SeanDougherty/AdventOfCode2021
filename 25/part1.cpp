#include <iostream>
#include <vector>
#include <bits/stdc++.h>

using namespace std;
using Matrix = vector<vector<char>>;

vector<string> split(const string& str, char delimiter) {
    vector<string> tokens;

    stringstream stream(str);
    string token;
    while (getline(stream, token, delimiter)) {
        tokens.push_back(token);
    }

    return tokens;
}

vector<char> buildRow(string buff) {
  vector<char> row;
  for (char el : buff) {
    row.push_back(el);
  }
  return row;
}

Matrix buildMatrix(bool realInput) {
  Matrix matrix;
  string fname;
  if (realInput) {
    fname = "input.txt";
  } else {
    fname = "input2.txt";
  }
  ifstream input(fname);
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

void printMatrix(Matrix m) {
  for (auto r : m) {
    for (auto c : r) {
      cout << c;
    }
    cout << endl;
  }
}

// copying because if top row moves, and bottom row thinks it can move to top row on that same step, that is incorrect.
Matrix step(Matrix m, bool &hasMoved) {
  Matrix n = Matrix(m.size(), vector<char>(m.at(0).size(), '.'));
  int rowSize, columnSize = m.size();
  for (int i=0; i<columnSize; i++) {
    rowSize = m.at(i).size();
    for (int j=0; j<rowSize; j++) {
      if (m[i][j] == '>') {
        if (m[i][(j+1)%rowSize] == '.') {
          n[i][(j+1)%rowSize] = '>';
          hasMoved = true;
        } else {
          n[i][j] = '>';
        }
      } else {
        if (m[i][j] == 'v') {
          n[i][j] = 'v';
        }
      }
    }
  }
  m = Matrix(m.size(), vector<char>(m.at(0).size(), '.'));
  for (int i=0; i<columnSize; i++) {
    rowSize = m.at(i).size();
    for (int j=0; j<rowSize; j++) {
      if (n[i][j] == 'v') {
        if (n[(i+1)%columnSize][j] == '.') {
          m[(i+1)%columnSize][j] = 'v';
          hasMoved = true;
        } else {
          m[i][j] = 'v';
        }
      } else {
        if (n[i][j] == '>') {
          m[i][j] = '>';
        }
      }
    }
  }
  return m;
}



int main()
{
  Matrix m = buildMatrix(false);
  bool hasMoved = true;
  int steps = 0;
  cout << "After "+to_string(steps)+" steps:" << endl;
  printMatrix(m);
  while (hasMoved) {
    // if (steps%25 == 0) {
    //   cout << "\r Steps: "+to_string(steps);
    // }
    hasMoved = false;
    m = step(m, hasMoved);
    steps++;
    if (steps < 10) {
      cout << endl;
      cout << "After "+to_string(steps)+" steps:" << endl;
      printMatrix(m);
    } else if (steps%10 ==0) {
      cout << endl;
      cout << "After "+to_string(steps)+" steps:" << endl;
      printMatrix(m);
    }
  }
  cout << "After "+to_string(steps)+" steps:" << endl;
  printMatrix(m);
  cout << "[Example] Final Step Count: "+to_string(steps) << endl;
  m = buildMatrix(true);
  hasMoved = true;
  steps = 0;
  while (hasMoved) {
    if (steps%25) {
      cout << "\r Steps: "+to_string(steps);
    }
    hasMoved = false;
    m = step(m, hasMoved);
    steps++;
  }
  cout << "\r [Real] Final Step Count: "+to_string(steps) << endl;

  return 0;
}