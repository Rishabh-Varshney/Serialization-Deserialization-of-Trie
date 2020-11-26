#include<bits/stdc++.h>
using namespace std;
/**
 * This structure is used to represent a node of the trie.
 */
class trieNode {
public:
  /**
  * indicates if the word ending here is a valid word.
  */
  bool isValidWord;
  /**
  * number of words have the prefix that is represented by this node.
  */
  int numberOfWordsWithPrefix;
  /**
  * represent all child nodes of this node.
  */
  unordered_map
  <char, trieNode*> children;
  /**
  * this constructor used to create a node
  */
  trieNode() {
    isValidWord = false;
    numberOfWordsWithPrefix = 0;
  }
};

/**
  * This class represents a generic trie data structure
  */
class TrieDS {
private:


  /**
    * Root of the Trie.
    */
  trieNode *root;
  /**
    * stores the total number of words present in Trie.
    */
  int totalWords;
  /**
    * helper function that traverse the trie and finds all word present in trie.
    */
  void treeDepthTraversal(trieNode *currentNode,
                          vector<string>& trieToString, string prefix) {
    if (currentNode->isValidWord) {
      trieToString.push_back(prefix);
    }
    for (auto p : currentNode->children) {
      prefix.push_back(p.first);
      treeDepthTraversal(p.second, trieToString, prefix);
      prefix.pop_back();
    }
  }
  /**
    * helper function to serialise the string
    */
  void buildSerialTrie(trieNode *root, string &serialTrie) {
    if (root->isValidWord)
      serialTrie.push_back(']');
    unordered_map<char, trieNode*> &children = root->children;
    for (auto child : children)
    {
      serialTrie.push_back(child.first);
      buildSerialTrie(child.second, serialTrie);
    }
    serialTrie.push_back('>');
  }

  /**
    * helper function to deserialise the string
    */
  int stringToTrie(trieNode *node, string &serialTrie, int &read) {
    int wordSeen = 0;
    if (serialTrie[read] == ']')
    {
      node->isValidWord = 1;
      wordSeen++;
      read++;
    }
    else node->isValidWord = 0;


    unordered_map<char, trieNode*> &childMap = node->children;
    while (serialTrie[read] != '>')
    {
      char ch = serialTrie[read++];
      childMap[ch] = new trieNode();
      wordSeen += stringToTrie(childMap[ch], serialTrie, read);
    }
    read++;
    node->numberOfWordsWithPrefix = wordSeen;
    return wordSeen;
  }

public:
  /**
    * used to construct an empty trie.
    */
  TrieDS() {
    root = new trieNode();
    totalWords = 0;
  }
  /**
    * Used to construct a trie initialized with a list of words.
    */
  TrieDS(const vector<string> &words) {
    totalWords = 0;
    root = new trieNode();
    for (string word : words)
    {
      insertWord(word);
    }
  }
  /**
    * Used to construct a trie using an existing trie given in serial format.
    */
  TrieDS(string &serialTrie) {
    root = new trieNode();
    int read = 0;
    totalWords = stringToTrie(root, serialTrie, read);
  }
  /**
    * used to insert a word in the trie.
    */
  void insertWord(const string &word) {
    trieNode *currentNode = root;
    for (int i = 0; i < word.length(); i++)
    {
      currentNode->numberOfWordsWithPrefix++;
      unordered_map<char, trieNode*>& childMap = (currentNode->children);
      auto it = childMap.find(word[i]);
      if (it == childMap.end())
      {
        currentNode = new trieNode();
        childMap[word[i]] = currentNode;
      }
      else
      {
        currentNode = childMap[word[i]];
      }
    }
    currentNode->numberOfWordsWithPrefix++;
    currentNode->isValidWord = true;
    totalWords++;
  }
  /**
    * gives the total number of words currently present in the trie.
    */
  int numberOfWordsInTrie() {
    return totalWords;
  }
  /**
    * Used to convert the trie to a list of words(serialization).
    */
  vector<string> trieToList() {
    vector<string> trieToString;
    treeDepthTraversal(root, trieToString, "");
    return trieToString;
  }
  /**
    * check to see if a word is present in the trie.
    */
  bool findWord(const string &word) {
    int countOfWord = wordsWithThePrefix(word, 0);
    if (countOfWord > 0)
      return true;
    else return false;
  }
  /**
    * Number of words in the trie the have a given prefix.
    */
  int wordsWithThePrefix(const string &prefix, int calledBy = 1) {
    if (root == NULL)
      return 0;
    trieNode *currentNode = root;
    for (int i = 0; i < prefix.length(); i++) {
      unordered_map<char, trieNode*>& childMap = (currentNode->children);
      auto it = childMap.find(prefix[i]);
      if (it == childMap.end())
        return 0;
      else
        currentNode = childMap[prefix[i]];
    }
    if (calledBy || currentNode->isValidWord)
      return currentNode->numberOfWordsWithPrefix;
    else return 0;
  }
  /**
    * Function To call TrieToString-which buildsSerialTree
    */
  string trieToString() {
    string serializedTrie;
    buildSerialTrie(root, serializedTrie);
    return serializedTrie;
  }

};

int main() {

  /**
    * Menu
    */
  cout << "**********-Menu-**********" << endl;
  cout << "1. Serialise" << endl;
  cout << "2. Deserialse" << endl;
  cout << "3. Other Features" << endl;
  cout << "4. Exit" << endl;

  int option;
  cin >> option;

  /**
    * Option-1:Serialisation
    */
  if (option == 1) {
    cout << "Input data to make Trie and then to convert it into Serialised Form:" << endl;
    vector<string> input;
    string word;

    cin >> word;
    while (word != "#END#")
    {
      input.push_back(word);
      cin >> word;
    }

    /**
    * Creating Serialised form
    */

    TrieDS *myTrie = new TrieDS(input);
    cout << "Serialised Form : " << endl;
    cout << myTrie->trieToString() << endl;
  }
  /**
    * Option-2:Deserialisation
    */
  else if (option == 2) {
    cout << "Provide The Serialised Form Of Trie:" << endl;
    string ser_tree;
    cin >> ser_tree;
    TrieDS *myTrie = new TrieDS(ser_tree);
    /**
    * outputting the number of words and the words present in the trie after converting it from deserialised form
    */
    cout << "Total Number of Words : ";
    cout << myTrie->numberOfWordsInTrie() << endl;
    vector<string>des_tree = myTrie->trieToList();
    for (auto x : des_tree) {
      cout << x << endl;
    }
  }
  /**
    * Option-3:Other Functionalities
    */
  else if (option == 3) {
    cout << "Input Data To form Trie First:" << endl;
    vector<string> input;
    string word;
    cin >> word;
    while (word != "#END#") {
      input.push_back(word);
      cin >> word;
    }
    /**
    * other features of trie
    */
    TrieDS *myTrie = new TrieDS(input);

    cout << " 1. Find total Number of Words In the Trie." << endl;
    cout << " 2. Find out how many words in the trie that we made before have a given prefix" << endl;
    cout << " 3. Check Whether the given word is present in trie or not." << endl;

    int option;
    cin >> option;
    if (option == 1) {
      cout << myTrie->numberOfWordsInTrie() << endl;
    }
    else if (option == 2) {
      cout << "Prefix:" << endl;
      string pre;
      cin >> pre;
      int number_of_words_with_given_prefix = myTrie->wordsWithThePrefix(pre);
      cout << number_of_words_with_given_prefix << endl;
    }
    else if (option == 3) {
      cout << "Word:" << endl;
      string word;
      cin >> word;
      bool check = myTrie->findWord(word);
      if (check) {
        cout << "Present" << endl;
      }
      else {
        cout << "Not Present" << endl;
      }
    }
    else {
      cout << "Option Not Available" << endl;
    }
  }
  /**
    * Option-4:Exit
    */
  else if (option == 4) {
    return 0;
  }
  /**
    * Option-Any Other:Default case
    */
  else {
    cout << "Option Not Available" << endl;
  }

  return 0;
}

