#include <iostream>
#include <vector>
#include <bits/stdc++.h>

using namespace std;
const int WIN = 21;
const int MAX_DICE = 3;
const int SPACES = 10;

struct GameState {
  int posOne = 0;
  int posTwo = 0;
  int turn = 0; // even == p1 moves, odd == p2 moves
  double parallelGames = 1;
};

vector<vector<vector<GameState>>> buildInput() {
  ifstream input("input.txt");
  string buff;
  vector<vector<vector<GameState>>> scores = vector<vector<vector<GameState>>>(21, vector<vector<GameState>>(21, vector<GameState>{}));
  while (input.is_open()) {
    getline(input, buff);
    buff.substr(buff.find(": ")+2);
    istringstream iss (buff.substr(buff.find(": ")+2));
    int i;
    iss >> i;
    getline(input, buff);
    buff.substr(buff.find(": ")+2);
    istringstream iss2 (buff.substr(buff.find(": ")+2));
    int j;
    iss2 >> j;
    scores.at(0).at(0).push_back(GameState{i,j,0, 1});
    if (input.eof()) {
      input.close();
    }
  }
  return scores;
}

int main()
{
  vector<vector<vector<GameState>>> games = buildInput();
  set<pair<int,int>> rollOutcomes {{3,1}, {4,3}, {5,6}, {6,7}, {7,6}, {8,3}, {9,1}};
  int nextScore;
  double winOne = 0;
  double winTwo = 0;
  double gamesInPlay = 0;
  for (int turn=0; turn < 26; turn++) {
    cout << "Turn " << turn;
    vector<vector<vector<GameState>>> nextRound = vector<vector<vector<GameState>>>(21, vector<vector<GameState>>(21, vector<GameState>{}));
    gamesInPlay = 0;
    for (int i=0; i < 21; i++) {
      for (int j=0; j < 21; j++) {
        if (games.at(i).at(j).size() > 0) {
          gamesInPlay += games.at(i).at(j).size();
          for (GameState game : games.at(i).at(j)) {
            for (pair<int,int> outcome : rollOutcomes) {
              if (game.turn%2 == 0) {
                nextScore = (game.posOne+outcome.first)%SPACES == 0 ? SPACES : (game.posOne+outcome.first)%SPACES;
                if (i+nextScore >= WIN) {
                  winOne += game.parallelGames*outcome.second;
                } else {
                  nextRound[i+nextScore][j].push_back(GameState{(game.posOne+outcome.first)%SPACES, game.posTwo, turn+1, game.parallelGames*outcome.second});
                }
              } else {
                nextScore = (game.posTwo+outcome.first)%SPACES == 0 ? SPACES : (game.posTwo+outcome.first)%SPACES;
                if (j+nextScore >= WIN) {
                  winTwo += game.parallelGames*outcome.second;
                } else {
                  nextRound[i][j+nextScore].push_back(GameState{game.posOne, (game.posTwo+outcome.first)%SPACES, turn+1, game.parallelGames*outcome.second});
                }
              }
            }
          }
        }
      }
    }
    games = nextRound;
    cout << " has " << fixed << gamesInPlay;
    cout << " games in play." << endl;
    if (gamesInPlay == 0) {
      break;
    }
  }
  cout << "Games in Play: " << fixed << gamesInPlay << endl;

  cout << "Player One wins: " << fixed << winOne << endl;
  cout << "Player Two wins: " << fixed << winTwo << endl;

}



