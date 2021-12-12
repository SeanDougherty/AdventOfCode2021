#include <iostream>
#include <vector>
#include <bits/stdc++.h>

using namespace std;



int grade(string line) {
  unordered_map<char,char> pairs = {{'}','{'},{']','['},{')','('},{'>','<'}};
  unordered_map<char,int> vals = {{')',3},{']',57},{'}',1197},{'>',25137}};
  stack<char> s;
  for (char el : line) {
    if (!s.empty()) {
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
        return vals[el];
      }
    }
  }
  return 0;
}

int main()
{
  ifstream input("input.txt");
  string line;
  int totalScore = 0;
  int lineScore;
  while (input.is_open()) {
    getline(input,line);
    totalScore += grade(line);
    if (input.eof()) {
      input.close();
    }
  }

  cout << totalScore << endl;

  return 0;
}