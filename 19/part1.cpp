/****************** HEADER ***********************/
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <set>
#include <cmath>
#include <climits>
#include <string>

using namespace std;

const int MATCH_COUNT = 12;
const int DIM = 3;
const int BEACONS_PER_SCANNER = 25;

struct PairedPoints {
  vector<int> absPos;
  vector<int> relPos;
  int absId;
  int relId;
};

class Beacon {
  vector<int> pos;
};

class Scanner {
  public:
    int id;
    vector<int> position = {0,0,0};
    set<vector<int>> beacons;
    Scanner(int id);
    void insertBeacon(string line);
    void insertBeacon(vector<int> beacon);
    void rotateScanner(int idx);
    vector<int> rotateBeacon(int idx, const vector<int> &beacon);
};


vector<PairedPoints> findSharedPoints(Scanner absolute, Scanner rel);
void matchOrientations(const Scanner &absolute, Scanner &rel, vector<PairedPoints> &shared);
void placeRelativeScanner(const Scanner &absolute, Scanner &rel, vector<PairedPoints> shared);
Scanner combineScanners(vector<Scanner> placed);
int countMatchedNums(vector<int> a, vector<int> b);
vector<int> subtract(vector<int> a, vector<int> b);
vector<int> add(vector<int> a, vector<int> b);
vector<int> invert(vector<int> a);
bool beaconMatch(vector<int> a, vector<int> b);
int manhattanDistance(vector<int> a, vector<int>b);

/****************** SCANNER.CPP *********************/

Scanner::Scanner(int id) {
  this->id = id;
}
void Scanner::insertBeacon(string line) {
  istringstream ss(line);
  vector<int> cords;
  while (getline(ss, line, ',')) {
    int i;
    istringstream iss (line);
    iss >> i;
    cords.push_back(i);
  }
  beacons.insert(cords);
}
void Scanner::insertBeacon(vector<int> beacon) {
  beacons.insert(beacon);
}
void Scanner::rotateScanner(int idx) {
  set<vector<int>> newBeacons;
  for (vector<int> beacon : beacons) {
    newBeacons.insert( rotateBeacon(idx, beacon) );
  }
  beacons = newBeacons;
}
vector<int> Scanner::rotateBeacon(int idx, const vector<int> &beacon){
  int x = beacon.at(0);
  int y = beacon.at(1);
  int z = beacon.at(2);
  vector<vector<int>> rots {
    // x is facing x
    {x, y, z},
    {x, -z, y},
    {x, -y, -z},
    {x, z, -y},
    // x is facing -x
    {-x, -y, z},
    {-x, -z, -y},
    {-x, y, -z},
    {-x, z, y},
    // x is facing y
    {-z, x, -y},
    {y, x, -z},
    {z, x, y},
    {-y, x, z},
    // x is facing -y
    {z, -x, -y},
    {y, -x, z},
    {-z, -x, y},
    {-y, -x, -z},
    // x is facing z
    {-y, -z, x},
    {z, -y, x},
    {y, z, x},
    {-z, y, x},
    // x is facing -z
    {z, y, -x},
    {-y, z, -x},
    {-z, -y, -x},
    {y, -z, -x}
  };
  return rots.at(idx);
}

/****************** MAIN.CPP ***********************/

vector<Scanner> buildInput() {
  vector<Scanner> scanners;
  ifstream input("input.txt");
  string buff;
  getline(input, buff);
  Scanner s = Scanner(0);
  int ct = 1;
  while (input.is_open()) {
    getline(input, buff);
    if (input.eof()) {
      input.close();
    }
    if (buff[1] == '-'){
      scanners.push_back(s);
      s = Scanner(ct);
      ct++;
      continue;
    }
    if (buff.size() == 0) { continue; }
    s.insertBeacon(buff);
  }
  scanners.push_back(s);
  cout << "Found " << scanners.size() << " scanners" << endl;
  return scanners;
}

int main()
{
  //10:10AM
  vector<Scanner> scanners = buildInput();
  set<int> toPlace = {};
  for (int i=1; i<scanners.size(); i++) { toPlace.insert(i); }
  vector<Scanner> placed = {};
  placed.push_back(scanners[0]);
  // cout << "1" << endl;
  while (placed.size()<scanners.size()) {
    // cout << "2" << endl;
    for (int i=0; i<placed.size(); i++) {
      // cout << "3" << endl;
      Scanner absolute = placed.at(i);
      bool placedSomething = false;
      for (int j : toPlace) {
        // cout << "4" << endl;
        Scanner relative = scanners.at(j);
        vector<PairedPoints> sharedPoints = findSharedPoints(absolute, relative);
        // cout << "5" << endl;
        if (sharedPoints.size() >= MATCH_COUNT) {
          // cout << "6" << endl;
          cout << "success" << endl;
          matchOrientations(absolute, relative, sharedPoints);
          placeRelativeScanner(absolute, relative, sharedPoints);
          placed.push_back(relative);
          // cout << "7" << endl;
          toPlace.erase(j);
          // cout << "8" << endl;
          placedSomething = true;
          break;
        } // endif sharedPoints >= MatchCount
      }// endfor j : toPlace
    }// endfor i : placed
  }//endWhile placed < scanners



  for (auto el : scanners) {
    cout << el.id << ", ";
  }
  cout << endl;
  for (auto el : placed) {
    cout << el.id << " at " << el.position.at(0) << "," << el.position.at(1) << "," << el.position.at(2) << endl;
  }
  cout << endl;

  int maxDist = INT_MIN;
  for (auto scanner : placed) {
    for (auto scanner2 : placed) {
      maxDist = max(manhattanDistance(scanner.position, scanner2.position), maxDist);
    }
  }
  cout << maxDist << endl;

  Scanner combined = combineScanners(placed);
  cout << combined.beacons.size() << endl;
  return 0;
}


vector<PairedPoints> findSharedPoints(Scanner absolute, Scanner rel) {
  vector<PairedPoints> result = {};
  vector<vector<int>> abs_beacons = {};
  vector<vector<int>> rel_beacons = {};
  for (auto beacon : absolute.beacons) {
    abs_beacons.push_back(beacon);
  }
  for (auto beacon : rel.beacons) {
    rel_beacons.push_back(beacon);
  }
  
  // for axis 0, 1, or 2
  for (int ax=0; ax<3; ax++) {
    // clear result
    vector<PairedPoints> result = {};
    //for each beacon of abs_diffs
    for (int i=0; i<abs_beacons.size(); i++) {
      vector<int> abs_diff = {};
      for (int j=0; j<abs_beacons.size(); j++) {
        if (i==j) { continue; }
        abs_diff.push_back( abs( abs_beacons.at(i).at(0) - abs_beacons.at(j).at(0) ) );
      }
      //for a beacon in rel_diffs 
      for (int a=0; a<rel_beacons.size(); a++) {
        vector<int> rel_diff = {};
        for (int b=0; b<rel_beacons.size(); b++) {
          if (a==b) { continue; }
          rel_diff.push_back( abs( rel_beacons.at(a).at(ax) - rel_beacons.at(b).at(ax) ) );
        }
          //if beacon in reldiffs has 11 numbers matched with beacon of absDiffs
          if(countMatchedNums(abs_diff, rel_diff) >= MATCH_COUNT-1) {
          //if such a beacon exists, it is the pair of the absolute beacon.
            // create a PairedPoint and add it to result
            result.push_back(PairedPoints{abs_beacons.at(i), rel_beacons.at(a), absolute.id, rel.id});
          }
        }//endFor rel_diffs
    }//endFor abs_diffs
    // if result.size() >= MatchCount
    if (result.size() >= MATCH_COUNT) {
      // return result
      return result;
    }
  }//endFor ax as {0,1,2}
  return result;
}

void matchOrientations(const Scanner &absolute, Scanner &rel, vector<PairedPoints> &shared) {
  //Find two paired points. making sure that for all axis: pairedpoint0_abs != pairedpoint1_abs
  PairedPoints b1 = shared.at(0);
  PairedPoints b2 = shared.at(1);
  int nextPair = 1;
  while (b1.absPos[0] == b2.absPos[0] || b1.absPos[0] == b2.absPos[0] || b1.absPos[0] == b2.absPos[0]) {
    b2 = shared.at(nextPair);
    nextPair++;
  }

  //Find the offset between the absolute positions of pairedPoint0 and pairedPoint1, call this offset
  vector<int> offset = subtract(b1.absPos, b2.absPos);
  //for each possible orientation
  for (int i=0; i<24; i++) {
    //rotate relative positions of pairedPoint0 and pairedPoint1
    b1.relPos = rel.rotateBeacon(i, shared.at(0).relPos);
    b2.relPos = rel.rotateBeacon(i, shared.at(nextPair).relPos);
    //If offset + pairedPoint1.relPos is equal to pairedPoint0.relPos
    if (beaconMatch(add(b2.relPos, offset), b1.relPos)) {
      //The orientation of abs and rel match
      //rotate all relative points in shared so that there is no stale oriented points
      for (int j=0; j<shared.size(); j++) {
        shared.at(j).relPos = rel.rotateBeacon(i, shared.at(j).relPos);
      }
      //rotate the relative scanner: rel.rotate(orientation);
      rel.rotateScanner(i);
      //return
      return;
    }
  }
}

void placeRelativeScanner(const Scanner &absolute, Scanner &rel, vector<PairedPoints> shared) {
  // relative's position is equal to abs.position + shared[0].absPos + (-1)shared[0].relPos
  rel.position = add( add(absolute.position, shared[0].absPos), invert(shared[0].relPos) );
}

Scanner combineScanners(vector<Scanner> placed) {
  Scanner master = Scanner(-2);
  for (auto scanner : placed) {
    for (auto beacon : scanner.beacons) {
      master.insertBeacon(add(scanner.position, beacon));
    }
  }
  return master;
}

int countMatchedNums(vector<int> a, vector<int> b) {
  vector<int> matched;
  set<int> alreadyMatchedIndices;
  for (int i=0; i<a.size(); i++) {
    for (int j=0; j<b.size(); j++) {
      if (alreadyMatchedIndices.count(j) < 1 && a.at(i) == b.at(j)) {
        matched.push_back(b.at(j));
        alreadyMatchedIndices.insert(j);
      }
    }
  }
  return matched.size();
}

vector<int> subtract(vector<int> a, vector<int> b) {
  for (int i=0; i<a.size(); i++) {
    a.at(i) = a.at(i) - b.at(i);
  }
  return a;
}
vector<int> add(vector<int> a, vector<int> b) {
  for (int i=0; i<a.size(); i++) {
    a.at(i) = a.at(i) + b.at(i);
  }
  return a;
}
vector<int> invert(vector<int> a) {
  for (int i=0; i<a.size(); i++) {
    a.at(i) = (-1)*a.at(i);
  }
  return a;
}
int manhattanDistance(vector<int> a, vector<int>b) {
  int dist = 0;
  for (int i=0; i<a.size(); i++) {
    dist += abs(a.at(i) - b.at(i));
  }
  return dist;
}
bool beaconMatch(vector<int> a, vector<int> b) {
  for (int i=0; i<a.size(); i++) {
    if (a.at(i) != b.at(i)) {
      return false;
    }
  }
  return true;
}