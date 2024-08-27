#include <iostream>
#include <fstream>
#include <queue>
#include <vector>
#include <map>
#include <string>
#include <iterator>
#include <algorithm>
 
#define MAX_TREE_HT 256   // Huffman Tree
using namespace std;

// HuffmanCoding class
class HuffmanCoding {
public:
    map<char, string> codes;
    map<char, int> freq;

    struct MinHeapNode {
        char data;
        int freq;
        MinHeapNode *left, *right;

        MinHeapNode(char data, int freq) {
            left = right = NULL;
            this->data = data;
            this->freq = freq;
        }
    };

    struct compare {
        bool operator()(MinHeapNode* l, MinHeapNode* r) {
            return (l->freq > r->freq);
        }
    };

    priority_queue<MinHeapNode*, vector<MinHeapNode*>, compare> minHeap;

    void calcFreq(const string& str) {
        for (char ch : str) {
            freq[ch]++;
        }
    }

    void storeCodes(MinHeapNode* root, string str) {
        if (root == nullptr)
            return;
        if (root->data != '$')
            codes[root->data] = str;
        storeCodes(root->left, str + "0");
        storeCodes(root->right, str + "1");
    }

    void buildHuffmanTree() {
        MinHeapNode *left, *right, *top;
        for (auto v = freq.begin(); v != freq.end(); v++)
            minHeap.push(new MinHeapNode(v->first, v->second));
        while (minHeap.size() != 1) {
            left = minHeap.top();
            minHeap.pop();
            right = minHeap.top();
            minHeap.pop();
            top = new MinHeapNode('$', left->freq + right->freq);
            top->left = left;
            top->right = right;
            minHeap.push(top);
        }
        storeCodes(minHeap.top(), "");
    }

    string decode(MinHeapNode* root, const string& s) {
        string ans = "";
        MinHeapNode* curr = root;
        for (char ch : s) {
            if (ch == '0')
                curr = curr->left;
            else if (ch == '1')
                curr = curr->right;
            if (curr->left == NULL && curr->right == NULL) {
                ans += curr->data;
                curr = root;
            }
        }
        return ans;
    }

    string encode(const string& str) {
        string encodedString;
        for (char ch : str) {
            encodedString += codes[ch];
        }
        return encodedString;
    }
};

// Updated TextEditor class
class TextEditor {
private:
    struct Node {
        string word;
        Node* prev;
        Node* next;
        // Node(string w) : word(w), prev(nullptr), next(nullptr) {}
        Node (string w){
            word = w;
            prev = NULL;
            next = NULL;
        }
    };

    Node* head;
    Node* tail;
    map<string, vector<Node*>> wordMap;

public:
    // TextEditor() : head(nullptr), tail(nullptr) {}
    TextEditor(){
        head=NULL;
        tail=NULL;
    }

    void insert(const string& word) {
        Node* newNode = new Node(word);
        if (!head) {
            head = tail = newNode;
        } else {
            tail->next = newNode;
            newNode->prev = tail;
            tail = newNode;
        }
        wordMap[word].push_back(newNode);
        display();
    }

    void replace(const string& oldWord, const string& newWord) {
        if (wordMap.find(oldWord) != wordMap.end()) {
            for (Node* node : wordMap[oldWord]) {
                node->word = newWord;
                wordMap[newWord].push_back(node);
            }
            wordMap.erase(oldWord);
        }
        display();
    }

    void deleteWord(const string& word) {  // will delete last occurence 
        if (wordMap.find(word) != wordMap.end() && !wordMap[word].empty()) {
            Node* nodeToDelete = wordMap[word].back();
            wordMap[word].pop_back();
            
            if (wordMap[word].empty()) {
                wordMap.erase(word);
            }

            if (nodeToDelete == head) {
                head = head->next;
                if (head) head->prev = NULL;
            } else if (nodeToDelete == tail) {
                tail = tail->prev;
                if (tail) tail->next = NULL;
            } else {
                nodeToDelete->prev->next = nodeToDelete->next;
                nodeToDelete->next->prev = nodeToDelete->prev;
            }

            delete nodeToDelete;
        } else {
            cout << "Word not found!" << endl;
        }
        display();
    }

    void display() {
        cout << ".........................................................................................................................." << endl;
        Node* temp = head;
        while (temp) {
            cout << temp->word << " ";
            temp = temp->next;
        }
        cout << endl;
        cout << ".........................................................................................................................." << endl;
    }

    string getSentence() {
        string sentence;
        Node* temp = head;
        while (temp) {
            if (!sentence.empty()) {
                sentence += " ";
            }
            sentence += temp->word;
            temp = temp->next;
        }
        return sentence;
    }

    ~TextEditor() {
        Node* temp;
        while (head) {
            temp = head;
            head = head->next;
            delete temp;
        }
    }
};

// SpellChecker class
class SpellChecker {
    struct TreeNode {
        string word;
        int next[40];

        // TreeNode(string x) : word(x) {
        //     fill(begin(next), end(next), 0);
        // }
        TreeNode(string x){
            word=x;
            for(int i=0;i<40;i++){
                next[i]=0;
            }
        }

        TreeNode(){

        }
    };

    TreeNode rootNode;
    vector<TreeNode> tree;
    int nodePtr;

public:
    // SpellChecker() : tree(10000), nodePtr(0) {}
    SpellChecker(){
        tree.resize(10000);
        nodePtr=0;
    }

    
    int editDistance(string& a, string& b) {
        int m = a.length(), n = b.length();
        vector<vector<int>> dp(m + 1, vector<int>(n + 1));
        for (int i = 0; i <= m; i++) dp[i][0] = i;
        for (int j = 0; j <= n; j++) dp[0][j] = j;
        for (int i = 1; i <= m; i++) {
            for (int j = 1; j <= n; j++) {
                if (a[i - 1] != b[j - 1])
                    dp[i][j] = min({1 + dp[i - 1][j], 1 + dp[i][j - 1], 1 + dp[i - 1][j - 1]});
                else
                    dp[i][j] = dp[i - 1][j - 1];
            }
        }
        return dp[m][n];
    }

    void add(TreeNode& root, TreeNode& curr) {
        if (root.word == "") {
            root = curr;
            return;
        }
        
        int dist = editDistance(curr.word, root.word);
        if (tree[root.next[dist]].word == "") {
            nodePtr++;
            tree[nodePtr] = curr;
            root.next[dist] = nodePtr;
        } else {
            add(tree[root.next[dist]], curr);
        }
    }

    vector<string> getSimilarWords(TreeNode& root, const string& s, int TOL) {
        vector<string> ret;
        if (root.word == "") return ret;
        int dist = editDistance(root.word, const_cast<string&>(s));
        if (dist <= TOL) ret.push_back(root.word);
        int start = dist - TOL;
        if (start < 0) start = 1;  // can't be negative

        while (start <= dist + TOL) {
            vector<string> tmp = getSimilarWords(tree[root.next[start]], s, TOL);
            for (auto& i : tmp) ret.push_back(i);
            start++;
        }
        return ret;
    }

    void loadDictionary(const vector<string>& dictionary) {
        nodePtr = 0;
        for (const auto& dictWord : dictionary) {
            TreeNode tmp = TreeNode(dictWord);
            add(rootNode, tmp);
        }
    }

    vector<string> spellCheck(const string& word, int TOL = 2) {
        return getSimilarWords(rootNode, word, TOL);
    }
};

int main() {
    ifstream infile("input.txt");
    if (!infile) {
        cerr << "Error opening file!" << endl;
        return 1;
    }

    vector<string> dictionary;
    string word;
    while (infile >> word) {
        dictionary.push_back(word);
    }
    infile.close();

    SpellChecker spellChecker;
    spellChecker.loadDictionary(dictionary);

    TextEditor editor;
    HuffmanCoding huffman;

    string command, inputWord, oldWord, newWord;

    while (true) {
        cout << "Enter command (1.insert/2.replace/3.delete/4.exit): ";
        cin >> command;

        if (command == "1") {
            cout << "Enter word to insert: ";
            cin >> inputWord;
            editor.insert(inputWord);

            cout << "Do you want to perform a spell check? (yes/no): ";
            string choice;
            cin >> choice;

            if (choice == "yes") {
                vector<string> match = spellChecker.spellCheck(inputWord);
                cout<<endl;
                cout << "Similar words to " << inputWord << ": ";
                for (const auto& x : match) {
                    cout << x << " ";
                }
                cout << endl;
                cout<<endl;
            } else {
                editor.display();
            }

        } else if (command == "2") {
            cout << "Enter word to replace: ";
            cin >> oldWord;
            cout << "Enter new word: ";
            cin >> newWord;
            editor.replace(oldWord, newWord);

        } else if (command == "3") {
            cout << "Enter word to delete: ";
            cin >> inputWord;
            editor.deleteWord(inputWord);

        } else if (command == "4") {
            string sentence = editor.getSentence();
            cout<<endl;
            cout << "Generated Sentence: " << sentence << endl;
            cout<<endl;

            // Huffman Encoding
            string encodedString, decodedString;
            huffman.calcFreq(sentence);
            if (huffman.freq.empty()) {
                cerr << "Error: Frequency map is empty, no characters to encode!" << endl;
                return 1;
            }

            huffman.buildHuffmanTree();

            cout << "Character With their Frequencies:\n";
            for (auto v = huffman.codes.begin(); v != huffman.codes.end(); v++)
                cout << v->first << ' ' << v->second << endl;

            encodedString = huffman.encode(sentence);
            cout << "\nEncoded Huffman data:\n" << encodedString << endl;

            decodedString = huffman.decode(huffman.minHeap.top(), encodedString);
            cout << "\nDecoded Huffman Data:\n" << decodedString << endl;

            // Calculate and display compression ratio
            int originalSize = sentence.length() * 8; // size in bits
            int encodedSize = encodedString.length(); // size in bits
            double compressionRatio = static_cast<double>(encodedSize) / originalSize;

            cout << "\nOriginal Size (in bits): " << originalSize << endl;
            cout << "Encoded Size (in bits): " << encodedSize << endl;
            cout << "Compression Ratio: " << compressionRatio << endl;

            // Output encoded and decoded data to files
            ofstream encodedFile("encode.txt");
            if (!encodedFile) {
                cerr << "Error opening encoded file!" << endl;
                return 1;
            }
            encodedFile << encodedString;
            encodedFile.close();

            ofstream decodedFile("decode.txt");
            if (!decodedFile) {
                cerr << "Error opening decoded file!" << endl;
                return 1;
            }
            decodedFile << decodedString;
            decodedFile.close();

            break;

        } else {
            cout << "Invalid command!" << endl;
        }
    }

    return 0;
}