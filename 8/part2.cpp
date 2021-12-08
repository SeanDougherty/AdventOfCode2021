#include <iostream>
#include <vector>
#include <bits/stdc++.h>

using namespace std;

char filterOutSegments(vector<set<char>> codes, vector<set<char>> filterCodes) {
  for (int i=0; i < codes.size(); i++) {
    for (auto filter : filterCodes) {
      for (auto segment : filter) {
        auto it = codes[i].find(segment);
        if (it != codes[i].end()) {
          codes[i].erase(it);
        }
      }
    }
    if (codes[i].size() == 1) {
      return *codes[i].begin();
    }
  }
  throw invalid_argument("Segments not narrowed down enough.");
}

bool matchSets(set<char> a, set<char> b) {
  if (a.size() != b.size()) {return false;}
  for (char el : a) {
    if (b.find(el) != b.end()){
      b.erase(b.find(el));
    }
  }
  return b.size() == 0;
}

set<char> buildSet(string word) {
  set<char> chars;
  for (auto el : word) {
    chars.insert(el);
  }
  return chars;
}

unordered_map<int,set<char>> generateCodeLookup(string input) {
  unordered_map<int,set<char>> lookups;
  vector<set<char>> sixLengthCodes;
  unordered_map<int,int> segmentSumLookups;
  string word;
  istringstream iss(input);
  
  int wordVal = 0;
  while (iss >> word) {
    if (word.size() == 2) {
      lookups[1] = buildSet(word);
    } else if (word.size() == 4) {
      lookups[4] = buildSet(word);
    } else if (word.size() == 3) {
      lookups[7] = buildSet(word);
    } else if (word.size() == 7) {
      lookups[8] = buildSet(word);
    } else if (word.size() == 6) {
        sixLengthCodes.push_back(buildSet(word));
    }
  }
  char bottom = filterOutSegments(sixLengthCodes, {lookups[4], lookups[7]});
  char top = filterOutSegments({lookups[7]}, {lookups[1]});
  char bottomLeft = filterOutSegments(sixLengthCodes, {lookups[4], {bottom,top}});
  char topLeft = filterOutSegments(sixLengthCodes, {lookups[1], {bottom, top, bottomLeft}});
  char middle = filterOutSegments({lookups[8]}, {lookups[1], {bottom, top, bottomLeft, topLeft}});
  char bottomRight = filterOutSegments(sixLengthCodes, {{bottom,top,bottomLeft,topLeft, middle}});
  char topRight = filterOutSegments({lookups[8]}, {{bottom,top,bottomLeft,topLeft,middle,bottomRight}});
  
  lookups[2] = {top, topRight, middle, bottomLeft, bottom};
  lookups[3] = {top, topRight, middle, bottomRight, bottom};
  lookups[5] = {top, topLeft, middle, bottomRight, bottom};
  lookups[6] = {top, topLeft, middle, bottomLeft, bottomRight, bottom};
  lookups[9] = {top, topLeft, topRight, middle, bottomRight, bottom};
  lookups[0] = {top, topLeft, topRight, bottomLeft, bottomRight, bottom};

  return lookups;
}

int findSetMatch(string word, unordered_map<int,set<char>> lookup) {
  set<char> wordSet = buildSet(word);
  for (auto it : lookup) {
    if (matchSets(wordSet, it.second)) {
      return it.first;
    }
  }
  throw invalid_argument("Code not found within lookup table");
}


int main()
{
  ifstream input("input.txt");
  unordered_map<int,set<char>> codeLookup;
  string buff, word;
  int output, wordVal;
  set<char> wordSet;
  size_t outputStart;
  int count = 0;
  while (input.is_open()){
    output = 0;
    getline(input, buff);
    outputStart = buff.find('|')+1;
    if (outputStart != string::npos) {
      codeLookup = generateCodeLookup(buff.substr(0, outputStart-1));
      istringstream iss(buff.substr(outputStart));
      while (iss >> word) {
        wordVal = findSetMatch(word, codeLookup);
        output = output*10 + wordVal;
      }
    }
    count += output;
    
    if (input.eof()) {
      input.close();
    }
  }
  cout << count << endl;

  return 0;
}