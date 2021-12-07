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
    leftest = i < leftest ? i : leftest;
    rightest = i > rightest ? i : rightest;
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
  
  double minGas = LLONG_MAX;
  double sum;
  for (int i=leftest; i < rightest; i++){
    sum = 0;
    for (auto crab : positions) {
      sum += (pow(abs(crab-i),2) + abs(crab-i))/2; // Triangular Sum Formula (aka the Sum version of taking a Factorial)
    }
    minGas= min(minGas, sum);
  }

  cout << fixed << minGas << endl;

  return 0;
}