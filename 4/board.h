#ifndef BOARD_H
#define BOARD_H

#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <math.h>

using namespace std;

class Board {
  private:
    vector<vector<int>> board;
    int score = 0;
    int id = 0;

  public:
    Board(int id);
    vector<int> insertRow(string input);
    void mark(int num);
    bool check();
    int getScore();
    int getID();
    vector<int> getVals();
    void print();
};

#endif