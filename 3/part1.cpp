#include <iostream>
#include <vector>
#include <bits/stdc++.h>

using namespace std;

int binStringToInt(string bin) {
  int val = 0;
  for (int i = bin.size()-1; i >= 0; i--) {
    if (bin[i] == '1') {
      val += pow(2, bin.size()-1-i);
    }
  }
  return val;
}

int getConsumption(string gamma, string epsilon) {
  int g = binStringToInt(gamma);
  int e = binStringToInt(epsilon);
  return e * g;
}

int main()
{
  unordered_map<int,int> bitCounts;

  ifstream input("input.txt");
  string binary;
  getline(input, binary);
  int binLength = binary.size();
  for (int i=0; i < binLength; i++) {
    bitCounts[i] = 0;
  }
  int count = 0;
  while (input.is_open()) {
    if (input.eof()) {
      input.close();
    }
    for (int i=0; i < binLength; i++) {
      bitCounts[i] += (binary[i] - '0');
    }
    count++;
    getline(input, binary);
  }
  string gamma = "";
  string epsilon = "";
  for (int i = 0; i < binLength; i++) {
    if (bitCounts[i] > count/2) {
      gamma += '1';
      epsilon += '0';
    } else {
      gamma += '0';
      epsilon += '1';
    }
  }
  cout << "GAMMA: " << gamma << endl;
  cout << "EPSILON: " << epsilon << endl;
  int powerConsumption = getConsumption(gamma, epsilon);
  cout << powerConsumption << endl;
  return 0;
}