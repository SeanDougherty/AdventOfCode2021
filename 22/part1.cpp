#include <iostream>
#include <vector>
#include <bits/stdc++.h>

using namespace std;

struct Zone {
  bool on = true;
  vector<double> topLeft = {};
  vector<double> bottomRight = {};
};

void checkValidity(Zone z);
double volume(Zone z);
void turnOff(vector<Zone> &ons, Zone off);
void turnOn(vector<Zone> &ons, Zone newOn);
bool overlap(Zone on, Zone off);
vector<Zone> decompose(Zone on, Zone off);


vector<Zone> buildInput() {
  ifstream input("input.txt");
  string buff, coords;
  vector<Zone> steps;
  while (input.is_open()) {
    getline(input, buff);
    Zone zone = Zone();
    if (buff[1] == 'f') {
      zone.on = false;
    }
    stringstream ss(buff.substr(buff.find(' ')));
    while (getline(ss, coords, ',')){
      istringstream iss(coords.substr(coords.find('=')+1, coords.find("..")-coords.find('=')-1));
      int i;
      iss >> i;
      zone.topLeft.push_back((double) i);
      istringstream iss2(coords.substr(coords.find("..")+2));
      int j;
      iss2 >> j;
      zone.bottomRight.push_back((double) j);
    }
    checkValidity(zone);
    steps.push_back(zone);
    if (input.eof()) {
      input.close();
    }
  }
  cout << "Done building..." << endl;
  return steps;
}
void printZone(Zone zone) {
    if (zone.on) {
      cout << "On ";
    } else {
      cout << "Off ";
    }
    cout << zone.topLeft[0] << "," << zone.topLeft[1] << "," << zone.topLeft[2] << " : ";
    cout << zone.bottomRight[0] << "," << zone.bottomRight[1] << "," << zone.bottomRight[2] << endl;
}
void printZones(vector<Zone> steps) {
  for (Zone step : steps) {
    printZone(step);
  }
}

int main()
{
  vector<Zone> steps = buildInput();
  cout << "found " << steps.size() << " steps." << endl;
  vector<Zone> lit = {};
  int ct = 1;
  for (Zone step : steps) {
    if (step.on) {
      turnOn(lit, step);
      cout << "On ";
    } else {
      turnOff(lit, step);
      cout << "Off";
    }
    cout << " | Step " << ct << ". ";
    cout << lit.size() << " zones lit. ";
    double sum = 0;
    for (auto zone : lit) {
      sum += volume(zone);
    }
    cout << fixed << sum << " cubes lit." << endl;
    ct++;
  }

  double sum = 0;
  for (auto zone : lit) {
    sum += volume(zone);
  }

  Zone init {false, {-50, -50, -50}, {50,50,50}};
  turnOff(lit, init);

  double initSum = 0;
  for (auto zone : lit) {
    initSum += volume(zone);
  }
  cout << "init: " << fixed << (sum - initSum) << endl;
  cout << "total: " << fixed << sum << endl;

  return 0;
}

double volume(Zone z) {
  return (1+z.bottomRight[0]-z.topLeft[0])*(1+z.bottomRight[1]-z.topLeft[1])*(1+z.bottomRight[2]-z.topLeft[2]);
}

void turnOff(vector<Zone> &ons, Zone off) {
  vector<Zone> newLit = {};
  for (Zone on : ons) {
    if (overlap(on, off)) {
      vector<Zone> remainingLit = decompose(on, off);
      for (Zone lit : remainingLit) {
        newLit.push_back(lit);
      }
    } else {
      newLit.push_back(on);
    }
  }
  ons = newLit;
}

void turnOn(vector<Zone> &ons, Zone newOn) {
  vector<Zone> newlyLitZones = {newOn};
  bool overlapExists = false;

  do {
    overlapExists = false;
    for (Zone on : ons) {
      vector<Zone> nextNewlyLitZones = {};
      for (Zone toAdd : newlyLitZones){
        if (overlap(on, toAdd)){
          overlapExists = true;
          vector<Zone> newZones = decompose(toAdd, on);
          for (Zone lit : newZones) {
            nextNewlyLitZones.push_back(lit);
          }
        } else {
          nextNewlyLitZones.push_back(toAdd);
        }
      }
      newlyLitZones = nextNewlyLitZones;
    } 
  } while (overlapExists);

  for (Zone toAdd : newlyLitZones) {
    ons.push_back(toAdd);
  }
}

bool overlap(Zone on, Zone off) {
  if (on.topLeft[0] <= off.bottomRight[0] && 
        on.topLeft[1] <= off.bottomRight[1] &&
          on.topLeft[2] <= off.bottomRight[2] &&
            on.bottomRight[0] >= off.topLeft[0] &&
              on.bottomRight[1] >= off.topLeft[1] &&
                on.bottomRight[2] >= off.topLeft[2]) {
    return true;
  }
  if (on.topLeft[0] >= off.bottomRight[0] && 
        on.topLeft[1] >= off.bottomRight[1] &&
          on.topLeft[2] >= off.bottomRight[2] &&
            on.bottomRight[0] <= off.topLeft[0] &&
              on.bottomRight[1] <= off.topLeft[1] &&
                on.bottomRight[2] <= off.topLeft[2]) {
    return true;
  }
  return false;
}

//decompose (Zone on, Zone off);
// can split the 'Zone on' into 6 smaller zones that border the off area.
vector<Zone> decompose(Zone on, Zone off) {
  vector<Zone> fragments = {};
  Zone top, bottom, front, back, left, right;
  //top is where on.top[y] is < off.top[y]
  if (on.topLeft[1] < off.topLeft[1]) {
    top.topLeft = on.topLeft;
    top.bottomRight = on.bottomRight;
    top.bottomRight[1] = off.topLeft[1]-1;
    checkValidity(top);
    on.topLeft[1] = off.topLeft[1];
    fragments.push_back(top);
  }
  //bottom is where on.bottom[y] is > off.bottom[y]
  if (on.bottomRight[1] > off.bottomRight[1]) {
    bottom.bottomRight = on.bottomRight;
    bottom.topLeft = on.topLeft;
    bottom.topLeft[1] = off.bottomRight[1]+1;
    checkValidity(bottom);
    on.bottomRight[1] = off.bottomRight[1];
    fragments.push_back(bottom);
  }

  //front is where on.bottomRight[z] > off.bottomRight[z]
  if (on.bottomRight[2] > off.bottomRight[2]) {
    front.bottomRight = on.bottomRight;
    front.topLeft = on.topLeft;
    front.topLeft[2] = off.bottomRight[2]+1;
    checkValidity(front);
    on.bottomRight[2] = off.bottomRight[2];
    fragments.push_back(front);
  }
  //back is where on.topLeft[z] < off.topLeft[z]
  if (on.topLeft[2] < off.topLeft[2]) {
    back.topLeft = on.topLeft;
    back.bottomRight = on.bottomRight;
    back.bottomRight[2] = off.topLeft[2]-1;
    checkValidity(back);
    on.topLeft[2] = off.topLeft[2];
    fragments.push_back(back);
  }

  //left is where on.topLeft[x] < off.topLeft[x]
  if (on.topLeft[0] < off.topLeft[0]) {
    left.topLeft = on.topLeft;
    left.bottomRight = on.bottomRight;
    left.bottomRight[0] = off.topLeft[0]-1;
    checkValidity(left);
    on.topLeft[0] = off.topLeft[0];
    fragments.push_back(left);
  }
  //right is where on.bottomRight[x] > off.topRight[x]
  if (on.bottomRight[0] > off.bottomRight[0]) {
    right.bottomRight = on.bottomRight;
    right.topLeft = on.topLeft;
    right.topLeft[0] = off.bottomRight[0]+1;
    checkValidity(right);
    on.bottomRight[0] = off.bottomRight[0];
    fragments.push_back(right);
  }
  return fragments;
}

void checkValidity(Zone z) {
  if (z.topLeft[0] > z.bottomRight[0] ||
      z.topLeft[1] > z.bottomRight[1] ||
      z.topLeft[2] > z.bottomRight[2])
      throw runtime_error("Found a zone that technically has \"negative\" space.");
}
