#ifndef TRIE_H
#define TRIE_H

#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>
using namespace std;

class Trie {
  private:
  unordered_map<int, Trie*> m_children;
  bool m_isWord = false;
  string word = "000000000000";
  int ones = 0;
  int zeros = 0;


  public:
  void populateTrie(string in);
  int findRating(bool leastCommon);
};

#endif