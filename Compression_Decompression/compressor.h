#pragma once
#include <wx/wx.h>

#include <cstdlib>
#include <iostream>
#include <fstream>
#include <queue>
#include <string>
#include <vector>
#include <cstdio>


using namespace std;

struct huffmanNode {
    char ch;
    int freq;
    huffmanNode* left;
    huffmanNode* right;

    huffmanNode(char character, int frequency) : ch(character), freq(frequency), left(nullptr), right(nullptr) {}
};

struct Compare {
    bool operator()(huffmanNode* left, huffmanNode* right) {
        return left->freq > right->freq;
    }
};

class huffmanAlg {
private:
    string huffmanCodes[256];
    string encodedText;
    
public:
    void makeCode(huffmanNode* root, string code);
    huffmanNode* huffmanTree(const char characters[], const int frequencies[], int size);
    void fileReader(const string& filename, char characters[], int frequencies[], int& size);

    void compressFile(const wxString& filepath);
    void saveCompressedFile(const string& fileName);

    void decompressFile(const wxString& filepath); 
    string decodeText(huffmanNode* root);
    void saveDecompressedFile(const string& fileName, const string& decompressedText);
    string filterFileName(const string& filePath);
    
};

