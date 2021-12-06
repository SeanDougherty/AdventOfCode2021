#include "trie.h"
#include <math.h>
using namespace std;

// UTIL
int binStringToInt(string bin) {
  int val = 0;
  for (int i = bin.size()-1; i >= 0; i--) {
    if (bin[i] == '1') {
      val += pow(2, bin.size()-1-i);
    }
  }
  return val;
}

// TRIE CLASS
void Trie::populateTrie(string in){
  Trie* curr = this;
  for (char bit : in) {
    int bitVal = bit - '0';
    if (bitVal) {
      curr->ones++;
    } else {
      curr->zeros++;
    }
    if (curr->m_children.find(bitVal) != curr->m_children.end()) { // Already inserted a similar bin_string, continue traversal.
      curr = curr->m_children[bitVal];
    } else { // New Trie path is needed, create next Child and continue traversal.
      Trie* newChild = new Trie();
      curr->m_children.insert({bitVal, newChild});
      curr = newChild;
    }
  }
  curr->m_isWord = true;
  curr->word = in;
}


int Trie::findRating(bool leastCommon){
  Trie* curr = this;  

  while (curr->zeros+curr->ones > 1) {
    if (leastCommon) {

      if (curr->ones == curr->zeros) {
        curr = curr->m_children[0];
      } else if (curr->ones > curr->zeros) {
        curr = curr->m_children[0];
      } else {
        curr = curr->m_children[1];
      }

    } else {

      if (curr->ones == curr->zeros) {
        curr = curr->m_children[1];
      } else if (curr->ones > curr->zeros) {
        curr = curr->m_children[1];
      } else {
        curr = curr->m_children[0];
      } 

    }
  }
  
  while (!curr->m_isWord) { // make sure we are at a leaf node.
    curr = curr->m_children[curr->ones];
  }

  return binStringToInt(curr->word);
}
