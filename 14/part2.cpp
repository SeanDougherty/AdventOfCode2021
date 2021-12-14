#include <iostream>
#include <vector>
#include <bits/stdc++.h>

using namespace std;

// Creates a default value for map elements, 
// this avoids checking if an entry exists in 
// a map before incrementing it's count.
struct myDouble {
  double value = 0;
};

map<string,string> buildInput(string &templ) {
  ifstream input("input.txt");
  string buff;
  map<string,string> insertions;
  getline(input, templ); // Pull starting template
  while (input.is_open()) {
    getline(input, buff);
    if (buff.size() == 0) { continue; } // skip empty lines
    insertions[buff.substr(0,buff.find(' '))] = buff.substr(buff.find('>')+2); // add patterns to insertion map
    if (input.eof()) {
      input.close();
    }
  }
  return insertions;
}

void step(map<string,string> inserts, map<string,myDouble> &pairs, map<char,myDouble> &counts) {
  map<string,myDouble> newPairs;
  for (auto el : inserts) {
    if (pairs.find(el.first) != pairs.end()) { // if an insertion pattern exists in the current template
      counts[el.second[0]].value += pairs[el.first].value; // increase the count of the inserted char by the # of pairs in the template that matches this insert pattern
      newPairs[string() + el.first[0] + el.second[0]].value += pairs[el.first].value; // add the two versions of pairs created by inserting
      newPairs[string() + el.second[0] + el.first[1]].value += pairs[el.first].value; // EG: finding NN in your pairs, triggers the pattern NN -> C, which creates the following two pairs in the new template: NC, CN
    }
  }
  pairs = newPairs;
}

void printScore(map<char,myDouble> counts) {
  double minCount = LLONG_MAX;
  double maxCount = LLONG_MIN;
  for (auto it : counts) {
    minCount = minCount < it.second.value ? minCount : it.second.value;
    maxCount = maxCount > it.second.value ? maxCount : it.second.value;
  }
  cout << "score " << fixed << maxCount-minCount << endl;
}

map<string,myDouble> buildPairs(string templ) {
  map<string,myDouble> pairs;
  for (int i=0; i < templ.size()-1; i++) {
    pairs[templ.substr(i,2)].value++;
  }
  return pairs;
}

map<char,myDouble> getCounts(string templ) {
  map<char,myDouble> counts;
  for (auto el : templ) {
    counts[el].value++;
  }
  return counts;
}


int main()
{
  string templ;
  map<string,string> insertions = buildInput(templ);
  map<char,myDouble> counts = getCounts(templ);
  map<string,myDouble> pairs = buildPairs(templ);
  int n=40;
  for (int i=0; i < n; i++) {
    step(insertions, pairs, counts);
  }

  printScore(counts);

  return 0;
}