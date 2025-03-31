#include "./StanfordCPPLib/console.h"
#include "./StanfordCPPLib/map.h"
#include "./StanfordCPPLib/random.h"
#include "./StanfordCPPLib/simpio.h"
#include "./StanfordCPPLib/vector.h"
#include <fstream>
#include <iostream>
using namespace std;

ifstream openFile();
int getMarkovModel();
string analyzeFile(ifstream &, Map<string, Vector<char>> &,
                   int k); // It returns most frequent sequence
void generateRandomText(Map<string, Vector<char>> &, string);
const int LENGTH = 2000;

int main() {
  ifstream ifs = openFile();
  int k = getMarkovModel();
  Map<string, Vector<char>> data;
  string mostFrequent = analyzeFile(ifs, data, k);
  generateRandomText(data, mostFrequent);
  return 0;
}

void generateRandomText(Map<string, Vector<char>> &data, string mostFrequent) {
  int k = mostFrequent.size();
  string result = mostFrequent;
  string currentChunk = mostFrequent;
  for (int i = 0; i < LENGTH - k; i++) {
    if (data.containsKey(currentChunk) && !data[currentChunk].isEmpty()) {
      char nextChar =
          data[currentChunk][randomInteger(0, data[currentChunk].size() - 1)];
      result += nextChar;
      currentChunk = currentChunk.substr(1) + nextChar;
    } else {
      break;
    }
  }
  cout << result << endl;
}
string analyzeFile(ifstream &file, Map<string, Vector<char>> &data, int k) {
  cout << "ANALYZING..." << endl;
  string currentChunk = "";
  char c;
  string mostFrequent;
  int mostFrequentLength = 0;
  while (file.get(c)) {
    currentChunk += c;
    if (currentChunk.size() == k) {
      char nextChar = file.peek();
      if (!nextChar) {
        break;
      }
      data[currentChunk].add(nextChar);
      if (data[currentChunk].size() > mostFrequentLength) {
        mostFrequent = currentChunk;
        mostFrequentLength = data[currentChunk].size();
      }

      // Instead of deleting whole chunk and assembling it again, we can just
      // shift it by 1
      currentChunk.erase(0, 1);
    }
  }
  cout << "FINISHED ANALYZING" << endl;
  cout << "GENERATING TEXT" << endl;
  cout << "=================" << endl;
  file.close();
  return mostFrequent;
}

int getMarkovModel() {
  while (true) {
    int k = getInteger("Enter model's number (1-10): ");
    if (k >= 1 && k <= 10) {
      return k;
    }
    cout << "Not valid number" << endl;
  }
}
ifstream openFile() {
  while (true) {
    string name = getLine("Enter file name: ");
    ifstream ifs(name);
    if (ifs) {
      return ifs;
    }
    cout << "Cann't find such file" << endl;
  }
}
