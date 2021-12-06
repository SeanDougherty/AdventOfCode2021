#include <iostream>
#include <vector>
#include <bits/stdc++.h>

using namespace std;


vector<int> getFish(string in) {
  stringstream ss(in);
  string numstr;
  vector<int> fish;
  while (getline(ss, numstr, ',')) {
    istringstream iss(numstr);
    int i;
    iss >> i;
    fish.push_back(i);
  }
  return fish;
}

int main()
{
  ifstream input("input.txt");
  string buff;
  getline(input, buff);
  vector<int> fish = getFish(buff);

  const int days = 80;
  int fishToCheck = fish.size();
  for (int i=0; i < days; i++) {
    fishToCheck = fish.size();
    for (int j=0; j < fishToCheck; j++) {
      if (fish[j] == 0) {
        fish.push_back(8);
        fish[j] = 6;
      } else {
        fish[j]--;
      }
    }
  }

  cout << fish.size() << endl;

  return 0;
}