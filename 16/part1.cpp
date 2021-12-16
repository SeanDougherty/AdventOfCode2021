#include <iostream>
#include <vector>
#include <bits/stdc++.h>

using namespace std;

struct Packet {
  int version;
  int typeId;
  bool isLiteral;
  double val = 0;
  bool opTypeId;
  int subPacketLen = 0;
  int subPacketCt = 0;
  vector<Packet> subPackets;
};

Packet fetchNextPacket(int &pos, string &bin);

void printPacket(Packet packet, int indent) {
  cout << string(indent, ' ') << "Version: " << packet.version << endl;
  cout << string(indent, ' ') << "typeId: " << packet.typeId << endl;
  cout << string(indent, ' ') << "isLiteral: " << packet.isLiteral << endl;
  cout << string(indent, ' ') << "val: " << packet.val << endl;
  cout << string(indent, ' ') << "opTypeId: " << packet.opTypeId << endl;
  cout << string(indent, ' ') << "subPacketLen: " << packet.subPacketLen << endl;
  cout << string(indent, ' ') << "subPacketCt: " << packet.subPacketCt << endl;
  cout << string(indent, ' ') << "subPackets: " << packet.subPackets.size() << endl;
  for (auto sub : packet.subPackets) {
    printPacket(sub, indent+2);
  }
}

string buildInput() {
  string buff;
  getline(ifstream("input.txt"), buff);
  return buff;
}

string hexToBin(const string input){
  stringstream output;
  unordered_map<char, string> h2b {
    {'0',"0000"},
    {'1',"0001"},
    {'2',"0010"},
    {'3',"0011"},
    {'4',"0100"},
    {'5',"0101"},
    {'6',"0110"},
    {'7',"0111"},
    {'8',"1000"},
    {'9',"1001"},
    {'A',"1010"},
    {'B',"1011"},
    {'C',"1100"},
    {'D',"1101"},
    {'E',"1110"},
    {'F',"1111"}
  };
  for (int i=0; i < input.size(); i++) {
      output << h2b[input[i]];
  }
  return output.str();
}

double readLiteralVal(int &pos, string &bin) {
  stringstream byteCode;
  string subByte = bin.substr(pos, 5);
  pos += 5;
  while (subByte[0] != '0') {
    byteCode << subByte.substr(1);
    subByte = bin.substr(pos, 5);
    pos+=5;
  }
  byteCode << subByte.substr(1);
  return bitset<64>(byteCode.str()).to_ullong();;
}

void appendPacketsUntilPos(int &pos, string &bin, Packet &parent) {
  int startPos = pos;
  while (pos < startPos+parent.subPacketLen) {
    parent.subPackets.push_back(fetchNextPacket(pos,bin));
  }
}

void appendNpackets(int &pos, string &bin, Packet &parent) {
  int subPacketsBuilt = 0;
  while (subPacketsBuilt < parent.subPacketCt) {
    parent.subPackets.push_back(fetchNextPacket(pos,bin));
    subPacketsBuilt++;
  }
}

void fetchSubPackets(int &pos, string &bin, Packet &parent) {
  if (parent.opTypeId) {
    appendNpackets(pos,bin,parent);
  } else {
    appendPacketsUntilPos(pos, bin, parent);
  }
}

Packet fetchNextPacket(int &pos, string &bin) {
  Packet packet;
  packet.version = (int) bitset<32>(bin.substr(pos,3)).to_ulong();
  pos += 3;
  packet.typeId = (int) bitset<32>(bin.substr(pos,3)).to_ulong();
  pos += 3;
  packet.isLiteral = packet.typeId == 4;
  if (packet.isLiteral) {
    packet.val = readLiteralVal(pos, bin);
  } else {
    packet.opTypeId = bin[pos] == '1';
    pos++;
    if (packet.opTypeId) {
      packet.subPacketCt = (int) bitset<32>(bin.substr(pos,11)).to_ulong();
      pos += 11;
    } else {
      packet.subPacketLen = (int) bitset<32>(bin.substr(pos,15)).to_ulong();
      pos += 15;
    }
  }
  if (!packet.isLiteral) {
    fetchSubPackets(pos, bin, packet);
  }
  return packet;
}

void checkError(int &pos, const string &bin) {
  while (pos < bin.size()) {
    pos++;
    if (bin[pos] == '1') {
      throw runtime_error("Found a 1 bit out of place");
    }
  }
}

void process(string in, Packet &packet) {
  string bin = hexToBin(in);
  int pos = 0;
  packet = fetchNextPacket(pos, bin);
  checkError(pos, bin);
}

int dfs(Packet packet) {
  stack<Packet> toVisit;
  toVisit.push(packet);
  Packet curr;
  int sum = 0;
  while (!toVisit.empty()) {
    curr = toVisit.top();
    toVisit.pop();
    sum += curr.version;
    for (auto child : curr.subPackets) {
      toVisit.push(child);
    }
  }
  return sum;
}

void traverse(Packet packet) {
  cout << "version sum: " << dfs(packet) << endl;
}

double evaluate(Packet packet) {
  double sum=0;
  double product=1;
  double minimum = LLONG_MAX;
  double maximum = LLONG_MIN;
  switch (packet.typeId) {
    case 0:
      for (auto el : packet.subPackets) {
        sum += evaluate(el);
      }
      return sum;
    break;
    case 1:
      for (auto el : packet.subPackets) {
        product *= evaluate(el);
      }
      return product;
    break;
    case 2:
      for (auto el : packet.subPackets) {
        minimum = min(minimum,evaluate(el));
      }
      return minimum;
    break;
    case 3:
      for (auto el : packet.subPackets) {
        maximum = max(maximum,evaluate(el));
      }
      return maximum;    
      break;
    case 4:
      return packet.val;
    break;
    case 5:
      return (double) (evaluate(packet.subPackets[0]) > evaluate(packet.subPackets[1]));
    break;
    case 6:
      return (double) (evaluate(packet.subPackets[0]) < evaluate(packet.subPackets[1]));
      break;
    case 7:
      return (double) (evaluate(packet.subPackets[0]) == evaluate(packet.subPackets[1]));
      break;
  }
  return 0;
}

int main()
{
  string input = buildInput();
  cout << input.size()*4 << endl;
  Packet packet;
  process(input, packet);
  traverse(packet);
  double result = evaluate(packet);
  cout << endl << fixed << result << endl;
  return 0;
}