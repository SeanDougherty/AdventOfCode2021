#include <iostream>
#include <vector>
#include <bits/stdc++.h>

using namespace std;


void insert(map<string,vector<string>> &g, string start, string end) {
  if (g.find(start) == g.end()) {
    g[start] = {};
  }
  if (g.find(end) == g.end()) {
    g[end] = {};
  }
  g[start].push_back(end);
}

bool canCycle(string s, string e) {
  return s[0] >= 'A' && s[0] <= 'Z' && e[0] >= 'A' && e[0] <= 'Z';
}

map<string,vector<string>> buildGraph() {
  map<string,vector<string>> g;
  ifstream input("input.txt");
  string buff, start, end;
  while (input.is_open()) {
    getline(input, buff);
    start = buff.substr(0,buff.find('-'));
    end = buff.substr(buff.find('-')+1);
    if (canCycle(start, end)) {
      throw logic_error("two large caves are next to each other. potential solution is to merge the two large caves into 1 node.");
    }
    insert(g, start, end);
    insert(g, end, start); // undirected, so add the bidirectional path
    if (input.eof()) {
      input.close();
    }
  }
  return g;
}

bool isLarge(string s) {
  return s[0] >= 'A' && s[0] <= 'Z';
}

struct Path {
  vector<string> traversal = {};
  set<string> set = {};
  bool doubleVisit = false;
};

void visit(queue<pair<string,Path>> &toVisit, string neighbor, Path path) {
  if (!isLarge(neighbor) && path.set.find(neighbor) != path.set.end()){
    path.doubleVisit = true;
  }
  path.traversal.push_back(neighbor);
  path.set.insert(neighbor);
  toVisit.push({neighbor,path});
}

void printPath(Path p) {
  for (auto node : p.traversal) {
    cout << node << " -> ";
  }
  cout << endl;
}

void printGraph(map<string,vector<string>> g) {
  for (auto it : g) {
    cout << it.first << " : ";
    for (auto node : it.second) {
      cout << node << ", ";
    }
    cout << endl;
  }
}

bool cantAdd(Path path, string neighbor) {
  if (neighbor == "start") {
    return true;
  }
  if (path.set.find(neighbor) != path.set.end() && !isLarge(neighbor) && path.doubleVisit) {
    return true;
  }
  return false;  
}

int countPaths(map<string,vector<string>> &g) {
  queue<pair<string,Path>> toVisit;
  Path path {{"start"},{"start"}};
  toVisit.push({"start", path});
  string curr;
  int paths = 0;
  while (!toVisit.empty()) {
    curr = toVisit.front().first;
    path = toVisit.front().second;
    toVisit.pop();
    if (curr == "end") {
      paths++;
      continue;
    }
    for (auto neighbor : g[curr]) {
      if (cantAdd(path, neighbor)) {
        continue;
      }
      visit(toVisit, neighbor, path);
    }
  }
  return paths;
}

int main()
{
  map<string,vector<string>> graph = buildGraph();
  cout << countPaths(graph) << endl;
  return 0;
}