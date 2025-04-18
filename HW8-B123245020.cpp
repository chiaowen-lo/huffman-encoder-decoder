#include <bits/stdc++.h>
using namespace std;

struct Node {
    unsigned char ch; // 字元
    int freq;         // 頻率
    Node *left, *right; // 左子節點, 右子節點

    Node(unsigned char c, int f) : ch(c), freq(f), left(nullptr), right(nullptr) {} //初始化
};

struct Compare { //比較器，用來排序PQ
    bool operator()(Node* a, Node* b) {
        return a->freq > b->freq;
    }
};

void buildFrequencyTable(const string& inputFile, unordered_map<unsigned char, int>& freqMap) {
    //建立頻率表
    ifstream inFile(inputFile, ios::binary); //用二進制開啟檔案
    unsigned char byte; //確保有足夠空間可以存圖片檔
    while (inFile.read(reinterpret_cast<char*>(&byte), 1)) {//讀取檔案中的每個byte
        //將byte的值加到freqMap中
        freqMap[byte]++;
    }
    inFile.close();
}

Node* buildHuffmanTree(const unordered_map<unsigned char, int>& freqMap) {
    //建立Huffman樹
    priority_queue<Node*, vector<Node*>, Compare> pq;
    for (const auto& pair : freqMap) {
        //將freqMap中的每個元素轉換成Node，並加入到pq中
        pq.push(new Node(pair.first, pair.second));
    }
    while (pq.size() > 1) {
        //從pq中取出兩個最小的Node，並將它們合併成一個新的Node
        Node* left = pq.top(); pq.pop();
        Node* right = pq.top(); pq.pop();
        Node* parent = new Node('\0', left->freq + right->freq); 
        //創建新的Node，頻率為兩個Node頻率之和 
        parent->left = left; //將左子節點指向left
        parent->right = right; //將右子節點指向right
        pq.push(parent); //將新的Node加入到pq中
    }
    return pq.top(); //返回pq中唯一的Node，即Huffman樹的根節點
}

void buildEncodingTable(Node* root, string code, unordered_map<unsigned char, string>& encodingTable) {
    if (!root) return; //如果root為空，則返回
    if (!root->left && !root->right) {
        encodingTable[root->ch] = code; //如果root沒有子節點，則將code加入到encodingTable中
    }
    buildEncodingTable(root->left, code + "0", encodingTable); //遞歸處理左子節點
    buildEncodingTable(root->right, code + "1", encodingTable); //遞歸處理右子節點
}

void saveEncodingTable(const unordered_map<unsigned char, string>& encodingTable, ofstream& outFile) {
    //保存編碼表
    size_t tableSize = encodingTable.size(); //取得encodingTable的大小
    outFile.write(reinterpret_cast<const char*>(&tableSize), sizeof(tableSize)); //將tableSize寫入到outFile中
    for (const auto& pair : encodingTable) {
        outFile.put(pair.first); //將pair.first寫入到outFile中
        size_t codeLength = pair.second.size(); //取得pair.second的大小
        outFile.write(reinterpret_cast<const char*>(&codeLength), sizeof(codeLength)); //將codeLength寫入到outFile中
        outFile.write(pair.second.c_str(), codeLength); //將pair.second寫入到outFile中
    }
}

void compressFile(const string& inputFile, const string& outputFile) {
    //壓縮檔案
    unordered_map<unsigned char, int> freqMap;
    buildFrequencyTable(inputFile, freqMap); //建立頻率表

    Node* root = buildHuffmanTree(freqMap); //建立Huffman樹
    unordered_map<unsigned char, string> encodingTable;
    buildEncodingTable(root, "", encodingTable); //建立編碼表

    ifstream inFile(inputFile, ios::binary); //用二進制開啟輸入檔案
    ofstream outFile(outputFile, ios::binary); //用二進制開啟輸出檔案
    saveEncodingTable(encodingTable, outFile); //保存編碼表

    string encodedString = ""; //初始化編碼字串
    unsigned char byte;
    while (inFile.read(reinterpret_cast<char*>(&byte), 1)) { //讀取檔案中的每個byte
        //將byte的值加入到encodedString中
        encodedString += encodingTable[byte];
    }

    size_t bitsCount = encodedString.size(); //取得encodedString的大小
    outFile.write(reinterpret_cast<const char*>(&bitsCount), sizeof(bitsCount)); //將bitsCount寫入到outFile中

    int paddingBits = (8 - (encodedString.size() % 8)) % 8; //計算填充的位數
    for (int i = 0; i < paddingBits; ++i) {
        encodedString += "0"; //在encodedString的末尾填充0
    }

    for (size_t i = 0; i < encodedString.size(); i += 8) { //將encodedString分成8位一組，並寫入到outFile中
        bitset<8> byte(encodedString.substr(i, 8)); //將encodedString的每8位轉換成bitset<8>
        outFile.put(static_cast<unsigned char>(byte.to_ulong())); //將bitset<8>轉換成unsigned char，並寫入到outFile中
    }

    inFile.close(); //關閉輸入檔案
    outFile.close(); //關閉輸出檔案
}

void loadEncodingTable(ifstream& inFile, unordered_map<string, unsigned char>& decodingTable) {
    //載入編碼表
    size_t tableSize;
    inFile.read(reinterpret_cast<char*>(&tableSize), sizeof(tableSize)); //讀取編碼表的大小
    for (size_t i = 0; i < tableSize; ++i) {
        unsigned char ch = inFile.get(); //讀取編碼表中的每個字元
        size_t codeLength;
        inFile.read(reinterpret_cast<char*>(&codeLength), sizeof(codeLength)); //讀取編碼表中的每個編碼
        string code(codeLength, '\0'); //創建一個長度為codeLength的字串，並初始化為'\0'
        inFile.read(&code[0], codeLength); //將編碼讀入到code中
        decodingTable[code] = ch; //將編碼和字元加入到decodingTable中
    }
}

void decompressFile(const string& inputFile, const string& outputFile) {
    //解壓縮檔案
    ifstream inFile(inputFile, ios::binary); //用二進制開啟輸入檔案
    ofstream outFile(outputFile, ios::binary); //用二進制開啟輸出檔案

    unordered_map<string, unsigned char> decodingTable;
    loadEncodingTable(inFile, decodingTable); //載入編碼表

    size_t bitsCount;
    inFile.read(reinterpret_cast<char*>(&bitsCount), sizeof(bitsCount)); //讀取編碼表的大小

    string encodedString = "";
    char byte;
    while (inFile.get(byte)) { //讀取輸入檔案中的每個byte
        bitset<8> bits(byte); //將byte轉換成bitset<8>
        encodedString += bits.to_string(); //將bitset<8>轉換成string，並加入到encodedString中
    }

    encodedString = encodedString.substr(0, bitsCount); //截取encodedString的前bitsCount位
    string currentCode = ""; //初始化currentCode
    for (char bit : encodedString) {
        currentCode += bit; //將bit加入到currentCode中
        if (decodingTable.count(currentCode)) {
            outFile.put(decodingTable[currentCode]); //將decodingTable[currentCode]寫入到outFile中
            currentCode = ""; //將currentCode清空
        }
    }

    inFile.close(); //關閉輸入檔案
    outFile.close(); //關閉輸出檔案
}

int main(int argc, char* argv[]) {
    if (argc < 5) {
        cerr << "Usage: huffman -c|-u -i infile -o outfile" << endl;
        return 1;
    }

    string mode = argv[1]; //取得模式
    string inputFile = argv[3]; //取得輸入檔案
    string outputFile = argv[5]; //取得輸出檔案

    if (mode == "-c") {
        compressFile(inputFile, outputFile); //壓縮檔案
    } else if (mode == "-u") {
        decompressFile(inputFile, outputFile); //解壓縮檔案
    } else {
        cerr << "Invalid mode: " << mode << endl;
        return 1;
    }

    return 0;
}
