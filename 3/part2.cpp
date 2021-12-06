#include <iostream>
#include <vector>
#include <bits/stdc++.h>
#include "trie.h"

using namespace std;

int main()
{

  Trie* trie = new Trie();
  ifstream input("input.txt");
  string binary;
  getline(input, binary);

  while (input.is_open()) {
    if (input.eof()) {
      input.close();
    }
    trie->populateTrie(binary);
    getline(input, binary);
  }

  int co2 = trie->findRating(false);
  int oxy = trie->findRating(true);

  cout << co2 * oxy << endl;
  return 0;
}