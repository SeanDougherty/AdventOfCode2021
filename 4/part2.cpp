#include <iostream>
#include <vector>
#include <bits/stdc++.h>
#include "board.h"

using namespace std;


vector<int> getDraws(string line) {
  istringstream ss(line);
  vector<int> draws;
  string temp;
  while (getline(ss, temp, ',')) {
    istringstream iss(temp);
    int i;
    iss >> i;
    draws.push_back(i);
  }
  return draws;
}

unordered_map<int,vector<Board*>> getBoardMap(ifstream &input, vector<Board*> &board_set) {
  unordered_map<int,vector<Board*>> boards;
  vector<int> nums;
  string line;
  Board *board = new Board(-1);
  int boardCt = 0;
  while (getline(input, line)) {
    if (line.empty()) {
      board = new Board(boardCt);
      board_set.push_back(board);
      boardCt++;
    } else {
      nums = board->insertRow(line);
      for (auto num : nums) {
        boards[num].push_back(board);
      }
    }
  }
  return boards;
}

void removeBoardFromMap(Board *board, unordered_map<int,vector<Board*>> &boards) {
  vector<int> valsToCheck = board->getVals();
  vector<Board*> *toScrub;
  for (auto val : valsToCheck) {
    toScrub = &boards[val];
    for (int i=0; i < toScrub->size(); i++) {
      if (toScrub->at(i)->getID() == board->getID()) {
        toScrub->erase(toScrub->begin()+i);
        break;
      }
    }
  }
  return;
}

int getLosingBoardID(vector<int> &draws, unordered_map<int,vector<Board*>> &boards) {
  vector<Board*> toBeMarked;
  int answer = 0;
  Board* biggestLoser;
  int finalWinningDraw = 0;
  for (auto draw : draws) {
    if (boards.find(draw) == boards.end()) { continue; }
    toBeMarked = boards[draw];
    for (auto board : toBeMarked) {
      board->mark(draw);
      if (board->check()) {
        removeBoardFromMap(board, boards);
        biggestLoser = board;
        finalWinningDraw = draw;
        cout << boards.size() << endl;
      }
    }
  }
  return biggestLoser->getScore() * finalWinningDraw;
}

void printBoards(vector<Board*> boards) {
  for (auto board : boards) {
    cout << endl << "####" << endl << endl;
    board->print();
  }
}

int main()
{
  ifstream input("input.txt");
  string line;
  getline(input, line);
  vector<int> draws = getDraws(line);
  unordered_map<int,vector<Board*>> boards;
  vector<Board*> board_set;
  boards = getBoardMap(input, board_set);
  int answer = getLosingBoardID(draws, boards);
  cout << answer << endl;
  return 0;
}