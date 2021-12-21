#include <iostream>
#include <vector>
#include <bits/stdc++.h>

using namespace std;
const int WIN = 1000;
const int MAX_DICE = 100;
const int SPACES = 10;

vector<int> buildInput() {
  ifstream input("input.txt");
  string buff;
  vector<int> players;
  while (input.is_open()) {
    getline(input, buff);
    buff.substr(buff.find(": ")+2);
    istringstream iss (buff.substr(buff.find(": ")+2));
    int i;
    iss >> i;
    players.push_back(i);
    if (input.eof()) {
      input.close();
    }
  }
  return players;
}

int main()
{

  vector<int> p = buildInput();
  vector<int> scores = vector<int>(p.size(), 0);
  int dice = 1;
  int rolls = 0;
  int p1Turn = true;
  int moves = 0;
  int loops = 0;
  do {
    if (p1Turn) {
      moves = roll(dice, 3);
      rolls += 3;
      p.at(0) = (p.at(0)+moves)%SPACES;
      if (p.at(0) == 0) {
        scores.at(0) += 10;
      } else {
        scores.at(0) += p.at(0);
      }
    } else {
      moves = roll(dice, 3);
      rolls += 3;
      p.at(1) = (p.at(1)+moves)%SPACES;
      if (p.at(1) == 0) {
        scores.at(1) += 10;
      } else {
        scores.at(1) += p.at(1);
      }
    }
    p1Turn = !p1Turn;
    loops++;
  } while (scores.at(0) < WIN && scores.at(1) < WIN);
  cout << "PLAYER 1" << endl;
  cout << "position: " << p.at(0) << endl;
  cout << "score: " << scores.at(0) << endl;
  cout << endl;
  cout << "PLAYER 2" << endl;
  cout << "position: " << p.at(1) << endl;
  cout << "score: " << scores.at(1) << endl;
  cout << endl << "total rolls: " << rolls << endl;
  if (scores.at(0) > scores.at(1)) {
    cout << scores.at(1) * rolls << endl;
  } else {
    cout << scores.at(0) * rolls << endl;
  }
  return 0;
}

int roll(int &dice, int rolls) {
  int moves = 0;
  int move;
  for (int i=0; i<rolls; i++) {
    move = ((dice+i)%MAX_DICE);
    if (move == 0) {
      moves += MAX_DICE;
    }else {
      moves += move;
    }
  }
  dice = dice+3%MAX_DICE;
  return moves;
}