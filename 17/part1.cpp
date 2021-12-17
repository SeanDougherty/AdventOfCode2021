#include <iostream>
#include <vector>
#include <bits/stdc++.h>

using namespace std;

vector<int> buildInput() {
  ifstream input("input.txt");
  string buff;
  getline(input, buff);
  int xStart, xEnd, yStart, yEnd;
  xStart = stoi(buff.substr(buff.find("=")+1, buff.find("..")-buff.find("=")-1));
  xEnd = stoi(buff.substr(buff.find("..")+2,buff.find(",")-buff.find("..")-2));
  yStart = stoi(buff.substr(buff.find("=", buff.find(","))+1,buff.find("..", buff.find(","))-buff.find("=", buff.find(","))-1));
  yEnd = stoi(buff.substr(buff.find("..", buff.find(","))+2));
  return {xStart, xEnd, yStart, yEnd};
}


int main()
{
  vector<int> target = buildInput();
  for (auto el : target) {
    cout << el << ", ";
  }
  cout << endl;
  int dy, dx, x, y, besty, best=0;
  bool works;
  for (int inY =-500; inY < 500; inY++) {
    for (int inX = 1; inX < 500; inX++) {
      dy = inY;
      dx = inX;
      x = 0;
      y = 0;
      besty = 0;
      works = false;
      for (int i = 1; i < 1000; i++) {
        x+=dx;
        y+=dy;
        if (y > besty) {
          besty = y;
        }
        if (dx > 0) {
          dx--;
        } else if (dx < 0) {
          dx++;
        }
        dy--;
        if (x >=target[0] && x <= target[1] && y >= target[2] && y <= target[3]) {
          if (besty > best) {
            best = besty;
          }
        }
        if (x > target[1] || y < target[2]){
          break;
        }
      }
    }
  }

  cout << best << endl;
  return 0;
}