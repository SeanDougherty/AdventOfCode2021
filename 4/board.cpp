#include "board.h"
using namespace std;


Board::Board(int id) {
  this->id = id;
}

vector<int> Board::insertRow(string input) {
  stringstream ss;
  ss << input;
  int n;
  vector<int> row;
  while (ss >> n) {
    row.push_back(n);
    this->score += n;
  }
  this->board.push_back(row);
  return row;
}

void Board::mark(int num) {
  for (int i=0; i < this->board.size(); i++) {
    for (int j=0; j < this->board[0].size(); j++) {
      if (this->board[i][j] == num) {
        this->board[i][j] = -1;
        this->score -= num;
      }
    }
  }
}

bool Board::check() {
  vector<int> columns (this->board.size(), 0);
  vector<int> rows (this->board[0].size(), 0);
  for (int i=0; i < this->board.size(); i++) {
    for (int j=0; j < this->board[0].size(); j++) {
      columns[j] += this->board[i][j];
      rows[i] += this->board[i][j];
      if (columns[j] == -5) {
        return true;
      } else if (rows[i] == -5) {
        return true;
      }
    }
  }
  return false;
}

vector<int> Board::getVals() {
  vector<int> vals;
  for (int i=0; i < this->board.size(); i++) {
    for (int j=0; j < this->board[0].size(); j++) {
      if (this->board[i][j] != -1) {
        vals.push_back(this->board[i][j]);
      }
    }
  }
  return vals;
}

int Board::getScore() {
  return this->score;
}

int Board::getID() {
  return this->id;
}

void Board::print() {
  for (int i=0; i < this->board.size(); i++) {
    for (int j=0; j < this->board[0].size(); j++) {
      cout << board[i][j] << " ";
    }
    cout << endl;
  }
}