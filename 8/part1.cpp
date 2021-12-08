#include <iostream>
#include <vector>
#include <bits/stdc++.h>

using namespace std;


int main()
{
  set<int> uniqueSegCounts {2, 4, 3, 7};
  ifstream input("input.txt");
  string buff;
  int count = 0;
  int totalWords = 0;
  size_t outputStart;
  while (input.is_open()){


    getline(input, buff);
    outputStart = buff.find('|')+1;
    string word;
    if (outputStart != string::npos) {
      istringstream iss(buff.substr(outputStart));
      while (iss >> word) {
        totalWords++;
        if (uniqueSegCounts.find(word.size()) != uniqueSegCounts.end()) {
          cout << word << endl;
          count++;
        }
      }
    }
    if (input.eof()) {
      input.close();
    }
  }

  cout << totalWords << endl;
  cout << count << endl;
  return 0;
}