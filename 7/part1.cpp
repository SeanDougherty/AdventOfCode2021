#include <iostream>
#include <vector>
#include <math.h>
#include <bits/stdc++.h>

using namespace std;

vector<int> getCrabs(string buff, int &leftest, int &rightest) {
  vector<int> positions;
  istringstream ss(buff);
  while (getline(ss, buff, ',')) {
    int i;
    istringstream iss (buff);
    iss >> i;
    positions.push_back(i);
    leftest = min(leftest, i);
    rightest = max(rightest, i);
  }
  return positions;
}

int main()
{
  ifstream ifs("input.txt");
  string buff;
  ifs >> buff;
  int leftest = INT_MAX, rightest = INT_MIN;
  vector<int> positions = getCrabs(buff, leftest, rightest);
  
  int minGas = INT_MAX;
  int sum;
  for (int i=leftest; i < rightest; i++){
    sum = 0;
    for (auto crab : positions) {
      sum += abs(crab-i);
    }
    minGas= min(minGas, sum);
  }

  cout << minGas << endl;

  return 0;
}