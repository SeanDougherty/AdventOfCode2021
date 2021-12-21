#include <iostream>
#include <vector>
#include <bits/stdc++.h>

using namespace std;

int buildInput() {
  ifstream input("input.txt");
  string buff;
  while (input.is_open()) {
    getline(input, buff);



    if (input.eof()) {
      input.close();
    }
  }
  return 0;
}

int main()
{

  return 0;
}