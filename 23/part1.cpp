#include <iostream>
#include <vector>
#include <bits/stdc++.h>

using namespace std;

struct Pod {
  char type = 'E';
  int moves = 0;
};

struct GameState {
  vector<Pod> hallway = vector<Pod> (11, Pod());
  vector<vector<Pod>> rooms = vector<vector<Pod>>(4, vector<Pod>(2, Pod()));
  int score = 0;
};
void printGame(GameState g);
bool containsOnlyGoodPods(vector<Pod> room, int rID);
vector<GameState> store(Pod p, int startPos, GameState g);
vector<GameState> moveToHallway(int rID, GameState g);
bool blocked(int start, int dest, const GameState &g);
bool solved(const GameState &g);

unordered_map<char,int> roomIndices = {{'A',2},{'B',4},{'C',6},{'D',8}};
unordered_map<char,int> roomIDs = {{'A',0},{'B',1},{'C',2},{'D',3}};
unordered_map<char,int> podCost = {{'A',1},{'B',10},{'C',100},{'D',1000}};
unordered_map<char,int> roomTypes = {{0,'A'},{1,'B'},{2,'C'},{3,'D'}};

queue<GameState> buildInput() {
  ifstream input("input2.txt");
  string buff, coords;
  queue<GameState> possibilities;
  GameState start;
  int podsSeen = 0;
  while (input.is_open()) {
    getline(input, buff);

    stringstream ss(buff);
    char space = ss.get();
    while (space != EOF) {
      if (space >= 'A' && space <= 'D') {

        start.rooms.at(podsSeen%4).at(podsSeen/4).type = space;
        podsSeen++;
      }
      space = ss.get();
    }
    if (input.eof()) {
      input.close();
    }
  }
  possibilities.push(start);
  printGame(start);
  cout << "Done building..." << endl;
  return possibilities;
}
void printGame(GameState g) {
  cout << "####################################" << endl;
  for (Pod p : g.hallway) {
    cout << p.type << to_string(p.moves) << ' ';
  }
  cout << endl;
  for (vector<Pod> room : g.rooms) {
    cout << "####" << endl;
    for (Pod p : room) {
      cout << p.type << to_string(p.moves) << ' ';
    }
    cout << endl;
  }
  cout << endl;
}
string gamestateID(GameState g) {
  string id = "";
  for (Pod p : g.hallway){
    id.push_back(p.type);
  }
  for (vector<Pod> room : g.rooms){
    for (Pod p : room) {
      id.push_back(p.type);
    }
  }
  id.append(to_string(g.score));
  return id;
}

int main()
{
  queue<GameState> games = buildInput();
  unordered_set<string> memos;
  memos.insert(gamestateID(games.front()));
  GameState curr;
  int cheapest = INT_MAX;
  int wins=0;
  while (!games.empty()) {
    cout << "\r    games: "+to_string(games.size())+"              wins: "+to_string(wins)+"          ";
    curr = games.front();
    games.pop();
    if (solved(curr)) {
      cheapest = min(cheapest, curr.score);
      wins++;
      continue;
    }
    bool movedToRoom = false;
    //Move all hallway pieces
    for (int i=0; i<curr.hallway.size(); i++) {
      Pod p = curr.hallway.at(i);
      if (p.type != 'E') {
        int rID = roomIDs.at(p.type);
        if (containsOnlyGoodPods(curr.rooms.at(rID), rID)) {
          for (GameState g : store(p,i,curr)) {
            movedToRoom = true;
            if (memos.find(gamestateID(g)) == memos.end()) {
              games.push(g);
              memos.insert(gamestateID(g));
            }
          }
        }
      }
    }
    if (movedToRoom) {continue;}
    //Move all top room pieces into any available spot in the hallway
    for (int i=0; i<curr.rooms.size(); i++) {
      for (GameState g : moveToHallway(i, curr)) {
        if (memos.find(gamestateID(g)) == memos.end()) {
          games.push(g);
          memos.insert(gamestateID(g));
        }
      }
    }
  }

  cout << endl;
  cout << "Considered "+to_string(memos.size())+" total gamestates." << endl;

  cout << endl;
  cout << cheapest << endl;

  return 0;
}

bool solved(const GameState &g) {
  for (Pod p : g.hallway) {
    if (p.type != 'E') return false;
  }
  for (Pod p : g.rooms[0]) {
    if (p.type != 'A') return false;
  }
  for (Pod p : g.rooms[1]) {
    if (p.type != 'B') return false;
  }
  for (Pod p : g.rooms[2]) {
    if (p.type != 'C') return false;
  }
  for (Pod p : g.rooms[3]) {
    if (p.type != 'D') return false;
  }
  return true;
}

vector<GameState> moveToHallway(int rID, GameState g) {
  vector<GameState> games = {};
  int distance = 0;
  Pod toMove = Pod();
  for (int i=0; i<g.rooms.at(rID).size(); i++) {
    char podType = g.rooms.at(rID).at(i).type;
    if (podType != 'E' && g.rooms.at(rID).at(i).moves == 0) {
      toMove = g.rooms.at(rID).at(i);
      toMove.moves++;
      distance += i+1;
      g.rooms.at(rID).at(i) = Pod();
      break;
    }
    if (podType != 'E' && g.rooms.at(rID).at(i).moves == 2) {
      break;
    }
  }
  if (toMove.type == 'E') return games;
  vector<int> hallwayDests = {0,1,3,5,7,9,10};
  int start = roomIndices.at(roomTypes.at(rID));
  for (int dest : hallwayDests) {
    if (!blocked(start, dest, g)) {
      GameState g2 = g;
      g2.hallway.at(dest) = toMove;
      g2.score += (distance + (abs(start-dest)))*podCost.at(toMove.type);
      games.push_back(g2);
    }
  }
  return games;
}

vector<GameState> store(Pod p, int startPos, GameState g) {
  vector<GameState> games = {};
  int distance = abs(startPos-roomIndices.at(p.type));
  if (blocked(startPos, roomIndices.at(p.type), g)) return games;
  int rID = roomIDs.at(p.type);
  for (int i=g.rooms.at(rID).size()-1; i>=0; i--) {
    char podType = g.rooms.at(rID).at(i).type;
    if (podType == 'E') {
      distance += i+1;
      p.moves++;
      g.rooms.at(rID).at(i) = p;
      break;
    } else if (podType = p.type) {
      continue;
    } else {
      throw runtime_error("Attempted to store a pod in the incorrect room!");
    }
  }
  g.score += distance*podCost.at(p.type);
  g.hallway.at(startPos) = Pod();
  games.push_back(g);
  return games;
}

bool blocked(int start, int dest, const GameState &g){
  if (start<dest) {
    for (int i=start+1; i<=dest; i++) {
      if (g.hallway.at(i).type != 'E') {
        return true;
      }
    }
  } else {
    for (int i=start-1; i>=dest; i--) {
      if (g.hallway.at(i).type != 'E') {
        return true;
      }
    }
  }
  return false;
}

bool containsOnlyGoodPods(vector<Pod> room, int rID) {
  for (Pod p : room) {
    if (p.type == 'E')
      continue;
    if (p.type != roomTypes.at(rID))
      return false;
  }
  return true;
}