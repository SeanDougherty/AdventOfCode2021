#include <iostream>
#include <vector>
#include <bits/stdc++.h>

using namespace std;

struct TreeNode {
  double val = 0;
  TreeNode *left = nullptr;
  TreeNode *right = nullptr;
};

double magnitude(TreeNode *root) {
  if (root == nullptr) {
    return 0;
  }
  if (!root->left && !root->right) { return root->val; }
  return 3*magnitude(root->left)+2*magnitude(root->right);
}

void split(TreeNode *root) {
  if (!root || root->left || root->right){
    return;
  }
  root->left = new TreeNode {floor(root->val/2), nullptr, nullptr};
  root->right = new TreeNode {ceil((root->val/2)), nullptr, nullptr};
  root->val = 0;
}

void explode(TreeNode *root, TreeNode *toExplode, TreeNode *&prevVal, bool &isFound) {
  if (!root) {return;}
  if (isFound && !root->left && !root->right) {
    root->val += toExplode->right->val;
    isFound = false;
    return;
  }
  if (!root->left && !root->right) { prevVal = root; }
  if (root == toExplode) {
    if (prevVal) {
      prevVal->val += toExplode->left->val;
    }
    isFound = true;
    return;
  }
  explode(root->left, toExplode, prevVal, isFound);
  explode(root->right, toExplode, prevVal, isFound);
}

TreeNode* findToExplode(TreeNode* root) {
  if (!root) { return root;}
  stack<pair<TreeNode*,int>> toVisit ({{root, 0}});
  pair<TreeNode*,int> curr;
  while (!toVisit.empty()) {
    curr = toVisit.top();
    toVisit.pop();
    if (!curr.first) {continue;}
    if (curr.second > 3 && curr.first->left && curr.first->right) {return curr.first;}
    toVisit.push({curr.first->right, curr.second+1});
    toVisit.push({curr.first->left, curr.second+1});
  }
  return nullptr;
}

TreeNode* findToSplit(TreeNode* root) {
  if (!root) { return root;}
  stack<TreeNode*> toVisit ({root});
  TreeNode* curr;
  while (!toVisit.empty()) {
    curr = toVisit.top();
    toVisit.pop();
    if (!curr) {continue;}
    if (!curr->left && !curr->right && curr->val > 9) {return curr;}
    toVisit.push({curr->right});
    toVisit.push({curr->left});
  }
  return nullptr;
}

TreeNode* add(TreeNode* lhv, TreeNode* rhv) {
  return new TreeNode {0, lhv, rhv};
}

int findMiddle(string line) {
  stack<char> balance;
  int pos = 0;

  while (pos < line.size()) {
    if (line[pos] == '[') {
      balance.push(line[pos]);
    }
    if (line[pos] == ']') {
      balance.pop();
    }
    if (balance.empty() && line[pos] == ',') {
      return pos;
    }
    pos++;
  }
  return -1;
}

TreeNode* buildTree(string line) {
  TreeNode* root = new TreeNode();
  if (line.size() == 1 && line[0] >= '0' && line[0] <= '9') {
    root->val = line[0] - '0';
  } else if (line[0] == '[') {
    int pos = findMiddle(line.substr(1, line.size()-1));
    root->right = buildTree(line.substr(pos+2, line.size()-(pos+2)-1));
    root->left = buildTree(line.substr(1, pos));
  }
  return root;
}

vector<string> buildInput() {
  vector<string> lines;
  ifstream input("input.txt");
  string buff;
  while (input.is_open()) {
    getline(input, buff);

    lines.push_back(buff);

    if (input.eof()) {
      input.close();
    }
  }
  return lines;
}

void inOrder(TreeNode* root) {
  if (!root) {return;}
  if (root->left && root->right) {
    cout << "[";
  }
  inOrder(root->left);
  if (!root->left && !root->right) {
    cout << root->val;
  } else {
    cout << ",";
  }
  inOrder(root->right);
  if (root->left && root->right) {
    cout << "]";
  }
}

int main()
{
  vector<string> rootsToSum = buildInput();
  TreeNode* root, *toExplode, *toSplit, *prev;
  TreeNode* mx1, *mx2;
  double mx = INT_MIN;
  bool isFound;
  for (int i =0; i<rootsToSum.size(); i++) {
    for (int j=0; j<rootsToSum.size(); j++) {
      if (i == j) {
        continue;
      }
      TreeNode *tr1 = buildTree(rootsToSum[i]);
      TreeNode *tr2 = buildTree(rootsToSum[j]);
      root = add(tr1,tr2);
      do {
      toExplode = findToExplode(root);
      if (toExplode) {
        prev = nullptr;
        isFound = false;
        explode(root, toExplode, prev, isFound);
        toExplode->left = nullptr;
        toExplode->right = nullptr;
        toExplode->val = 0;
        continue;
      }
      toSplit = findToSplit(root);
      if (toSplit) {
        split(toSplit);
      }
      } while (toExplode || toSplit);
      if (magnitude(root) > mx) {
        mx = magnitude(root);
        mx1 = tr1;
        mx2 = tr2;
      }
    }
  }

  inOrder(mx1);
  cout << endl << "###" << endl;
  inOrder(mx2);
  cout << endl << "###" << endl;

  cout << mx << endl;

  return 0;
}