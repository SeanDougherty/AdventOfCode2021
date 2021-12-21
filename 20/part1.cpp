#include <iostream>
#include <vector>
#include <bits/stdc++.h>

//guesses 5514, 5239


using namespace std;

struct Input {
  string algorithm;
  vector<string> image = {};
};

int enhance(vector<string> &image, const string &algorithm, bool borderOn, bool flipfill);
char getVal(const int &i, const int &j, bool borderOn, bool flipfill, const vector<string> &image, const string &algorithm);

Input buildInput() {
  Input in;
  ifstream input("input.txt");
  string buff;
  getline(input, buff);
  in.algorithm = buff;
  while (input.is_open()) {
    getline(input, buff);
    if (input.eof()) {
      input.close();
    }
    if (buff.size() == 0) { continue; }
    in.image.push_back(buff);
  }
  return in;
}

int main()
{
  Input input = buildInput();
  vector<string> image = input.image;
  string algorithm = input.algorithm;
  int steps = 51;
  for (int i=1; i<=steps; i++) {
    cout << "step " << i << ": " << enhance(image, algorithm, i%2==0, algorithm[0]=='#') << endl;
  }

  return 0;
}

int enhance(vector<string> &image, const string &algorithm, bool borderOn, bool flipfill) {
  vector<string> enhanced = vector<string>(image.size()+2, string(image.at(0).size()+2, '.'));
  int litCt = 0;
  char pixel;
  for (int i=0; i<enhanced.size(); i++) {
    for (int j=0; j<enhanced.at(i).size(); j++) {
      pixel = getVal(i, j, borderOn, flipfill, image, algorithm);
      if (pixel == '#') { litCt++; }
      enhanced.at(i).at(j) = pixel;
    }
  }
  image = enhanced;
  return litCt;
}

char getVal(const int &i, const int &j, bool borderOn, bool flipfill, const vector<string> &image, const string &algorithm) {
  int y = i-1;
  int x = j-1;
  int dy = 0, dx = 0;
  string subImage = "";
  vector<pair<int,int>> dirs = {
    {-1,-1},
    {-1,0},
    {-1,1},
    {0,-1},
    {0,0},
    {0,1},
    {1,-1},
    {1,0},
    {1,1},
  };
  for (pair<int,int> dir : dirs) {
    dy = dir.first;
    dx = dir.second;
    if (y+dy >= 0 && y+dy < image.size() && x+dx >= 0 && x+dx < image.at(y+dy).size()) {
      if (borderOn && flipfill) {
        if (image.at(y+dy).at(x+dx) == '#') {
          subImage.push_back('.');
        } else {
          subImage.push_back('#');
        }
      } else {
        subImage.push_back(image.at(y+dy).at(x+dx));
      }
    } else {
      if (borderOn && flipfill) {
        subImage.push_back('#');
      } else {
        subImage.push_back('.');
      }
    }
  }
  string binString = "";
  for (char pixel : subImage) {
    if (pixel == '.') {
      binString.push_back('0');
    } else if (pixel == '#') {
      binString.push_back('1');
    } else {
      throw runtime_error("Unexpected character found. " + pixel);
    }
  }
  bitset<9> num (binString);
  if (!borderOn && flipfill) {
    return algorithm.at(num.to_ulong()) == '#' ? '.' : '#';
  } else {
    return algorithm.at(num.to_ulong());
  }
}