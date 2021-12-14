#include <iostream>
#include <vector>
#include <bits/stdc++.h>

using namespace std;

map<string,string> buildInput(string &templ) {
  ifstream input("input.txt");
  string buff;
  map<string,string> insertions;
  getline(input, templ);
  while (input.is_open()) {
    getline(input, buff);
    if (buff.size() == 0) { continue; }
    insertions[buff.substr(0,buff.find(' '))] = buff.substr(buff.find('>')+2);
    if (input.eof()) {
      input.close();
    }
  }
  return insertions;
}

void step(map<string,string> ins, string &templ) {
  string newTempl = "";
  for (int i=0; i < templ.size()-1; i++) {
    newTempl += templ[i];
    newTempl += ins[templ.substr(i,2)];
  }
  newTempl += templ[templ.size()-1];
  templ = newTempl;
}

void printScore(string templ) {
  map<char, int> counts;
  int minCount = INT_MAX;
  int maxCount = INT_MIN;
  for (auto el : templ) {
    if (counts.find(el) == counts.end()) {
      counts[el] = 0;
    }
    counts[el]++;
    if (maxCount < counts[el]) {
      maxCount = counts[el];
    }
  }

  for (auto it : counts) {
    if (minCount > it.second) {
      minCount = it.second;
    }
  }
  cout << "score " << maxCount-minCount << endl;

}

int main()
{
  string templ;
  map<string,string> insertions = buildInput(templ);
  
  int n=10;
  for (int i=0; i < n; i++) {
    step(insertions, templ);
  }

  printScore(templ);

  return 0;
}