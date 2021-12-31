#include <iostream>
#include <vector>
#include <bits/stdc++.h>

using namespace std;

void evaluate(vector<int> instruction, double *w, double *x, double *y, double *z);
void add(double *a, double *b);
void mul(double *a, double *b);
void div(double *a, double *b);
void mod(double *a, double *b);
void eql(double *a, double *b);

struct pair_hash {
    template <class T1, class T2>
    std::size_t operator () (const std::pair<T1,T2> &p) const {
        auto h1 = std::hash<T1>{}(p.first);
        auto h2 = std::hash<T2>{}(p.second);

        // Mainly for demonstration purposes, i.e. works but is overly simple
        // In the real world, use sth. like boost.hash_combine
        return h1 ^ h2;  
    }
};


/*
 * Instruction format:
 * {
 * CODE_FOR_COMMAND (ie inp=0, add=1, etc), 
 * VARIABLE CODE (ie w=0, x=1, etc),
 * {OPTIONAL} VARIABLE CODE (ie w=0, x=1, etc),
 * {OPTIONAL} STATIC VAL {ie 127, 349, 4949993820}
 * VARIABLE FLAG (1 if second val is variable code, 0 if static value)
 * }
 */
vector<vector<vector<int>>> buildInput() {
  unordered_map<string,int> commandCodes {{"inp",0}, {"add",1}, {"mul",2}, {"div",3}, {"mod",4}, {"eql",5}};
  unordered_map<char, int> variableCodes {{'w',0},{'x',1},{'y',2},{'z',3}};
  ifstream input("input.txt");
  string buff;
  vector<vector<vector<int>>> instructionBlocks = {};
  vector<vector<int>> block = {};
  while (input.is_open()) {
    vector<int> instruction = vector<int>(5, 0);
    getline(input, buff);
    stringstream ss(buff);
    string val;
    int slot = 0;
    while (getline(ss, val, ' ')) {
      if (slot > 2) {
        break;
      }
      if (slot==0) {
        if (commandCodes.at(val) == 0) {
          instructionBlocks.push_back(block);
          vector<vector<int>> block = {};
        }
        instruction.at(slot) = commandCodes.at(val);
      }
      if (slot==1) {
        instruction.at(slot) = val.at(0)-'w';
      }
      if (slot==2) {
        if (val.at(0) <= 'z' && val.at(0) >= 'w') {
          instruction.at(slot) = val.at(0)-'w';
          instruction.at(slot+2) = 1;
        } else {
          istringstream iss(val);
          double i;
          iss >> i;
          instruction.at(slot+1) = i;
        }
      }
        slot++;
    }
    block.push_back(instruction);
    if (input.eof()) {
      input.close();
    }
  }
  instructionBlocks.push_back(block);
  instructionBlocks.erase(instructionBlocks.begin());
  cout << "Blocks: " << instructionBlocks.size() << endl;
  return instructionBlocks;
}


int main()
{
  using std::chrono::high_resolution_clock;
  using std::chrono::duration_cast;
  using std::chrono::duration;
  using std::chrono::milliseconds;
  auto t1 = high_resolution_clock::now();
  double big = LLONG_MIN, small = LLONG_MIN;
  double w = 0, x = 0, y = 0, z = 0;
  vector<vector<vector<int>>> instructionBlocks = buildInput();
  vector<unordered_map<double,vector<pair<int, double>>>> startZ = {};
  startZ.push_back(unordered_map<double,vector<pair<int, double>>>({{0,{pair<int,double>{0,0}}}}));
  // for each block of instructions
  for (int i=0; i<instructionBlocks.size(); i++) {
    cout << "\r   block: "+to_string(i)+"       considering "+to_string(startZ.at(i).size())+" vals";
    vector<vector<int>> block = instructionBlocks.at(i);
    startZ.push_back(unordered_map<double,vector<pair<int, double>>>());
    // for all possible starting z values (based off the last block of instructions)
    for (auto z_mapping : startZ.at(i)) {
     // for all possible values of w, aka inp. (1-9)
      for (int k=1; k<10; k++){
      z = z_mapping.first;
      w = k;
      // calculate the end value of z based off of startingZ and w
        for (auto instruction : block) {
          evaluate(instruction, &w, &x, &y, &z);
        }
        // store the endvalue of z in the 'possible startingZ values for the next block of commands'
        if (startZ.at(i+1).find(z) == startZ.at(i+1).end()) {
          startZ.at(i+1).insert({z, vector<pair<int,double>>({pair<int,double>{k,z_mapping.first}})});
        } else {
          startZ.at(i+1).at(z).push_back({k, z_mapping.first});
        }
      }
    }
  }
  cout << endl;
  cout << "startZ size: " << startZ.size() << endl;
  cout << "deepest size: " << startZ.at(startZ.size()-1).size() << endl;
  if (startZ.at(startZ.size()-1).find(0) == startZ.at(startZ.size()-1).end()) {
    cout << "no solutions found?" << endl;
  } else {
    cout << startZ.at(startZ.size()-1).at(0).size() << endl;
  }
  // cout << "possible solutions: " << startZ.at(startZ.size()-1).at(0).size() << endl;
  auto t2 = high_resolution_clock::now();

  /* Getting number of milliseconds as a double. */
  duration<double, std::milli> ms_double = t2 - t1;

  std::cout << ms_double.count() << "ms\n"; 
  return 0;
}

void buildResult(double prevZ, vector<unordered_map<double,vector<pair<int, double>>>> *startZ, int depth, string partialNum, vector<string> answers) {
  if (partialNum.size() >= 14) {
    answers.push_back(partialNum);
    return;
  }

  vector<pair<int, double>> prevZs = startZ->at(depth).at(prevZ);
  for (auto prevs : prevZs) {
    buildResult(prevs.second, startZ, depth-1, partialNum+to_string(prevs.first), answers);
  }
  return;
}

void evaluate(vector<int> instruction, double *w, double *x, double *y, double *z){

  vector<double*> v = {w,x,y,z};

  switch (instruction.at(0)) {
    case 0:
      return;
    break;
    case 1:
      if (instruction.at(4) == 1) {
        add(v.at(instruction.at(1)), v.at(instruction.at(2)));
      } else {
        double val = (double) instruction.at(3);
        add(v.at(instruction.at(1)), &val);
      }
    break;
    case 2:
      if (instruction.at(4) == 1) {
        mul(v.at(instruction.at(1)), v.at(instruction.at(2)));
      } else {
        double val = (double) instruction.at(3);
        mul(v.at(instruction.at(1)), &val);
      }
    break;
    case 3:
      if (instruction.at(4) == 1) {
        div(v.at(instruction.at(1)), v.at(instruction.at(2)));
      } else {
        double val = (double) instruction.at(3);
        div(v.at(instruction.at(1)), &val);
      }
    break;
    case 4:
      if (instruction.at(4) == 1) {
        mod(v.at(instruction.at(1)), v.at(instruction.at(2)));
      } else {
        double val = (double) instruction.at(3);
        mod(v.at(instruction.at(1)), &val);
      }
    break;
    case 5:
      if (instruction.at(4) == 1) {
        eql(v.at(instruction.at(1)), v.at(instruction.at(2)));
      } else {
        double val = (double) instruction.at(3);
        eql(v.at(instruction.at(1)), &val);
      }
    break;
  }
}


void add(double *a, double *b){
  *a = *a + *b;
}

void mul(double *a, double *b){
  *a = *a * *b;
}

void div(double *a, double *b){
  if (*b == 0) throw runtime_error("divide by zero.");
  if ( *a / *b >= 0) {
    *a = floor(*a / *b);
  } else {
    *a = ceil(*a / *b);
  }
}

void mod(double *a, double *b){
  if (*b <= 0) throw runtime_error("mod by zero or fewer.");
  *a = fmod(*a,*b);
}

void eql(double *a, double *b){
  *a = (int) *a == *b;
}
