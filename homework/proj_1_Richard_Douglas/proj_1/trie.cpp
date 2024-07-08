#include <iostream>
#include <unordered_map>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

class TrieNode {
public:
    unordered_map<char, TrieNode*> children;
    bool isEndOfWord;

    TrieNode() : isEndOfWord(false) {}
};
class Trie {
private:
    TrieNode* root;

    void deleteAllNodes(TrieNode* node) {
        for (auto& pair : node->children) {
            deleteAllNodes(pair.second);
        }
        delete node;
    }
    void collectAllWords(TrieNode* node, string prefix, vector<string>& result) const {
        if (node->isEndOfWord) {
            result.push_back(prefix);
        }
        for (auto& pair : node->children) {
            collectAllWords(pair.second, prefix + pair.first, result);
        }
    }

public:
    Trie() {
        root = new TrieNode();
    }

    ~Trie() {
        deleteAllNodes(root);
    }

    TrieNode* getRoot() const {
        return root;
    }

    void insert(const string& word) {
        TrieNode* current = root;
        for (char ch : word) {
            if (current->children.find(ch) == current->children.end()) {
                current->children[ch] = new TrieNode();
            }
            current = current->children[ch];
        }
        current->isEndOfWord = true;
    }

    vector<string> search(const string& term) const {
        TrieNode* current = root;
        for (char ch : term) {
            if (current->children.find(ch) == current->children.end()) {
                return {};
            }
            current = current->children[ch];
        }

        vector<string> result;
        collectAllWords(current, term, result);
        return result;
    }

    bool startsWith(const string& prefix) const {
        TrieNode* current = root;
        for (char ch : prefix) {
            if (current->children.find(ch) == current->children.end()) {
                return false;
            }
            current = current->children[ch];
        }
        return true;
    }

    // Remove a word from the trie
    bool remove(const string& word) {
        return remove(root, word, 0);
    }

private:
    bool remove(TrieNode* node, const string& word, int depth) {
        if (depth == word.size()) {
            if (!node->isEndOfWord) return false;
            node->isEndOfWord = false;
            return node->children.empty();
        }

        char ch = word[depth];
        if (node->children.find(ch) == node->children.end()) return false;

        bool shouldDeleteChild = remove(node->children[ch], word, depth + 1);
        if (shouldDeleteChild) {
            delete node->children[ch];
            node->children.erase(ch);
            return node->children.empty() && !node->isEndOfWord;
        }

        return false;
    }

    friend std::vector<std::string> findNearestWords(const Trie& trie, const std::string& word, size_t numResults);
};
void insertFileIntoTrie(const string& filename, Trie& trie) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error opening file: " << filename << endl;
        return;
    }

    string line;
    while (getline(file, line)) {
        trie.insert(line);
    }

    file.close();
}

int levenshteinDistance(const string& s1, const string& s2) {
    const size_t m = s1.size();
    const size_t n = s2.size();

    if (m == 0) return n;
    if (n == 0) return m;

    vector<vector<size_t>> dp(m + 1, vector<size_t>(n + 1));

    for (size_t i = 0; i <= m; ++i) dp[i][0] = i;
    for (size_t j = 0; j <= n; ++j) dp[0][j] = j;

    for (size_t i = 1; i <= m; ++i) {
        for (size_t j = 1; j <= n; ++j) {
            size_t cost = (s1[i - 1] == s2[j - 1]) ? 0 : 1;
            dp[i][j] = min({ dp[i - 1][j] + 1, dp[i][j - 1] + 1, dp[i - 1][j - 1] + cost });
        }
    }

    return dp[m][n];
}

vector<string> findNearestWords(const Trie& trie, const string& word, size_t numResults = 3) {
    vector<string> allWords;
    trie.collectAllWords(trie.getRoot(), "", allWords);

    vector<pair<string, int>> distances;
    for (const auto& w : allWords) {
        distances.emplace_back(w, levenshteinDistance(word, w));
    }

    sort(distances.begin(), distances.end(), [](const auto& a, const auto& b) {
        return a.second < b.second;
    });

    vector<string> result;
    for (size_t i = 0; i < min(numResults, distances.size()); ++i) {
        result.push_back(distances[i].first);
    }

    return result;
}

int main() {
    
    Trie trie;

    string filename = "Dictionary.txt";
    insertFileIntoTrie(filename, trie);

    char selection;
    cout << "Would you like to search the trie? [y]";
    cin >> selection;
    if(selection == 'y' || selection == 'Y'){}
    else return 1;
    while(true){
        string searchTerm;
        cout << "enter a search term: ";
        cin >> searchTerm;
        vector<string> results = trie.search(searchTerm);
        cout << "Search results for '" << searchTerm << "':" << endl;
        for (const string& result : results) {
            cout << result << endl;
        }
        if (results.empty()) {
            cout << "No exact match found for '" << searchTerm << "'. Do you mean:" << endl;
            vector<string> nearestWords = findNearestWords(trie, searchTerm);
            for (const string& word : nearestWords) {
                cout << word << endl;
            }
        } else {
            cout << "your options for '" << searchTerm << "' are:" << endl;
            for (const string& result : results) {
                cout << result << endl;
            }
        }
        cout << "Would you like to search the trie? [y]";
        cin >> selection;
        if(selection == 'y' || selection == 'Y'){}
        else return 1;
    }
    
    return 0;
}
