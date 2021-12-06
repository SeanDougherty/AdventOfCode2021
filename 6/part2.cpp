#include <iostream>
#include <vector>
#include <bits/stdc++.h>

using namespace std;


vector<double> getFish(string in) {
  stringstream ss(in);
  string numstr;
  vector<double> fish(10,0);
  while (getline(ss, numstr, ',')) {
    istringstream iss(numstr);
    int i;
    iss >> i;
    fish[i]++;
  }
  return fish;
}

void sumFish(vector<double> fish) {
  double fishSum = 0;
  for (int i=0; i < 9; i++) {
    fishSum += fish[i];
  }
  cout << fixed << fishSum << endl;
}

int main()
{
  ifstream input("input.txt");
  string buff;
  getline(input, buff);
  vector<double> fish = getFish(buff);

  for (int i=0; i < 256; i++) {
    fish[9] = fish[0];
    for (int j=0; j < fish.size()-1; j++) {
      fish[j] = fish[j+1];
    }
    fish[6] += fish[8];
  }

  sumFish(fish);

  return 0;
}