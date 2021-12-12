#include <iostream>
#include <vector>
#include <bits/stdc++.h>

using namespace std;



bool grade(string line, double &score) {
  unordered_map<char,char> pairs = {{'}','{'},{']','['},{')','('},{'>','<'}};
  unordered_map<char,int> vals = {{'(',1},{'[',2},{'{',3},{'<',4}};
  stack<char> s;
  for (char el : line) {
    if (s.empty()) {
      s.push(el);
      continue;
    }
    if (pairs.find(el) == pairs.end()) {
      s.push(el);
    } else {
      if (pairs[el] == s.top()) {
        s.pop();
        continue;
      } else {
        return false;
      }
    }
  }
  score = 0;
  while (!s.empty()) {
    score = score*5 + vals[s.top()];
    s.pop();
  }
  return true;
}

int main()
{
  ifstream input("input.txt");
  string line;
  vector<double> scores;
  double lineScore;
  while (input.is_open()) {
    getline(input,line);
    if (grade(line, lineScore)) {
      scores.push_back(lineScore);
    }
    if (input.eof()) {
      input.close();
    }
  }

  sort(scores.begin(), scores.end());
  cout << fixed << scores[(scores.size()/2)] << endl;

  return 0;
}