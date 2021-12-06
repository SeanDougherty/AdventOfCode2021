#include <iostream>
#include <vector>
#include <bits/stdc++.h>

using namespace std;


int main()
{
    ifstream input ("input.txt");
    string buff;
    stringstream ss;
    string direction;
    int distance;
    int depth = 0, travel = 0;
    while (input.is_open()) {
      getline(input,buff);
      ss << buff;
      ss >> direction;
      ss >> distance;
      ss.str("");
      ss.clear();
      if (direction[0] == 'u') {
        depth -= distance;
      }
      if (direction[0] == 'd') {
        depth += distance;
      }
      if (direction[0] == 'f') {
        travel += distance;
      }
      if (input.eof()) {
        input.close();
      }
    }

    cout << travel << endl;
    cout << depth << endl;

    cout << depth * travel << endl;

    return 0;
}
