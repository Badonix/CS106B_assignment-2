#include "StanfordCPPLib/console.h"
#include "StanfordCPPLib/lexicon.h"
#include "StanfordCPPLib/queue.h"
#include "StanfordCPPLib/simpio.h"
#include "StanfordCPPLib/vector.h"
#include <iostream>
#include <string>
using namespace std;

const int SENTINEL = -1;

pair<string, string> getInput(Lexicon &);
void findLadder(pair<string, string>, Lexicon &);
string stringToLower(string);
void printPath(Vector<string> &);
string joinVector(Vector<string> &, string);
void generateAllPossible(Vector<string> &, Queue<Vector<string>> &,
                         Set<string> &, Lexicon &);

int main() {
  Lexicon wordlist("EnglishWords.txt");
  while (true) {
    pair<string, string> words = getInput(wordlist);

    // If it's empty it means user entered sentinel
    if (words.first.empty()) {
      break;
    }

    findLadder(words, wordlist);
  }
  return 0;
}

void printPath(Vector<string> &path) {
  cout << "Found Ladder: " << joinVector(path, " ") << endl;
}

// Standart BFS approach
void findLadder(pair<string, string> words, Lexicon &wordlist) {
  Queue<Vector<string>> q;
  Set<string> viewed;
  string startWord = words.first;
  string destWord = words.second;

  // We can optimize and not run huge bfs in cases where lengths do not match
  if (startWord.length() != destWord.length()) {
    cout << "No ladder found" << endl;
    return;
  }

  Vector<string> startVector;
  startVector.add(startWord);
  viewed.add(startWord);
  q.enqueue(startVector);

  while (!q.isEmpty()) {
    Vector<string> currentPath = q.dequeue();

    // Last element of the vector is current word
    string currentWord = currentPath[currentPath.size() - 1];

    // If it matches the destination => we found the ladder
    if (currentWord == destWord) {
      printPath(currentPath);
      return;
    }

    // If not we generate all permutations of word
    generateAllPossible(currentPath, q, viewed, wordlist);
  }
  cout << "No ladder found" << endl;
}

void generateAllPossible(Vector<string> &ladder, Queue<Vector<string>> &q,
                         Set<string> &viewed, Lexicon &wordlist) {

  string word = ladder[ladder.size() - 1];

  // We iterate over each character and replace them from 'a' to 'z' => we
  // generate all permutations which are 1 step ahead
  for (int i = 0; i < word.size(); i++) {
    for (char j = 'a'; j <= 'z'; j++) {
      string newWord = stringToLower(word);
      newWord[i] = j;

      // If we already viewed that no need to add again so we skip current
      // iteration
      if (!wordlist.contains(newWord) || viewed.contains(newWord)) {
        continue;
      }

      viewed.add(newWord);
      ladder.add(newWord);
      q.enqueue(ladder);
      // We remove so that next iteration doesnt include previous permutation
      ladder.remove(ladder.size() - 1);
    }
  }
}

// Just a helper function to lowercase strings
string stringToLower(string word) {
  string lowerCase;
  for (char c : word) {
    lowerCase += tolower(c);
  }
  return lowerCase;
}

pair<string, string> getInput(Lexicon &wordlist) {
  pair<string, string> data;
  string startWord;
  string destinationWord;

  // Here we get start word over and over till its valid
  while (true) {
    startWord = stringToLower(
        getLine("Enter Start Word (" + to_string(SENTINEL) + " to quit): "));
    if (startWord == "-1") {
      return data;
    }
    if (wordlist.contains(startWord)) {
      break;
    }
    cout << "Not valid word" << endl;
  }

  // Here we get destination word over and over till its valid
  while (true) {
    destinationWord = stringToLower(getLine("Enter Destination Word: "));
    if (wordlist.contains(destinationWord)) {
      break;
    }
    cout << "Not valid word" << endl;
  }

  // We add those words to pair & return
  data.first = startWord;
  data.second = destinationWord;
  return data;
}

// A helper function to print elements of vector (for ladder)
string joinVector(Vector<string> &arr, string delimiter) {
  if (arr.isEmpty()) {
    return "";
  }
  string str;
  for (string s : arr) {
    str += s + delimiter;
  }
  str = str.substr(0, str.size() - delimiter.size());
  return str;
}
