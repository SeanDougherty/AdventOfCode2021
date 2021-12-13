#include <iostream>
#include <vector>
#include <bits/stdc++.h>

using namespace std;

int buildInput(set<pair<int,int>> &pairs, queue<pair<char,int>> &folds) {
  ifstream input("input.txt");
  string buff;
  int y, x;
  while (input.is_open()) {
    try {

    getline(input, buff);
    if (buff.length() == 0) {continue;}
    if (buff[0] != 'f') {
      x = stoi(buff.substr(0,buff.find(',')));
      y = stoi(buff.substr(buff.find(',')+1));
      pairs.insert({x,y});
    } else if (buff.find('x') != string::npos) {
      folds.push({'x',stoi(buff.substr(buff.find('=')+1))});
    } else if (buff.find('y') != string::npos) {
      folds.push({'y',stoi(buff.substr(buff.find('=')+1))});
    }

    if (input.eof()) {
      input.close();
    }
    } catch (exception e) {
      cout << buff << endl;
      cout << e.what() << endl;
    }
  }
  cout << "Found " << pairs.size() << " pairs and " << folds.size() << " folds." << endl;
  return 0;
}

set<pair<int,int>> fold(set<pair<int,int>> pairs, char dir, int line) {
  set<pair<int,int>> newPairs;
  for (auto loc : pairs) {
    if (dir == 'x') {
      if (loc.first > line) {
        newPairs.insert({(line - (loc.first % line))% line, loc.second});
      } else {
        newPairs.insert(loc);
      }
    } else {
      if (loc.second > line) {
        newPairs.insert({loc.first, (line - (loc.second % line))% line});
      } else {
        newPairs.insert(loc);
      }
    }
  }
  return newPairs;
}


void printMatrix(vector<vector<int>> &m) {
  for (auto row : m) {
    for (auto el : row) {
      if (el) {
        cout << '#';
      } else {
        cout << ' ';
      }
    }
    cout << endl;
  }
  cout << endl;
}

void printPairs(set<pair<int,int>> pairs) {
  int xMax = INT_MIN, yMax = INT_MIN;
  for (auto loc : pairs) {
    xMax = xMax > loc.first ? xMax : loc.first;
    yMax = yMax > loc.second ? yMax : loc.second;
  }
  vector<vector<int>> display (yMax+1, vector<int>(xMax+1, 0));
  for (auto loc : pairs) {
    display.at(loc.second).at(loc.first) = 1;
  }
  printMatrix(display);
}

int main()
{
  set<pair<int,int>> pairs;
  queue<pair<char,int>> folds;
  buildInput(pairs, folds);
  int foldCount = folds.size();
  for (int i=1; i < foldCount+1; i++) {
    cout << "After " << i << " folds: ";
    pairs = fold(pairs, folds.front().first, folds.front().second);
    cout << pairs.size() << endl;
    folds.pop();
  }

  printPairs(pairs);

  return 0;
}