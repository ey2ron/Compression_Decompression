#include "compressor.h"
#include <wx/wx.h>

#include <cstdlib>
#include <iostream>
#include <fstream>
#include <queue>
#include <string>
#include <vector>
#include <cstdio>
#include <wx/filename.h>



huffmanNode* huffmanAlg::huffmanTree(const char characters[], const int frequencies[], int size) {
    priority_queue<huffmanNode*, vector<huffmanNode*>, Compare> minHeap;

    for (int i = 0; i < size; ++i) {
        minHeap.push(new huffmanNode(characters[i], frequencies[i]));
    }

    while (minHeap.size() > 1) {
        huffmanNode* left = minHeap.top();
        minHeap.pop();

        huffmanNode* right = minHeap.top();
        minHeap.pop();

        huffmanNode* merged = new huffmanNode('\0', left->freq + right->freq);
        merged->left = left;
        merged->right = right;

        minHeap.push(merged);
    }

    return minHeap.top();
}

void huffmanAlg::makeCode(huffmanNode* root, string code) {
    if (!root)
        return;

    if (!root->left && !root->right) {
        huffmanCodes[(unsigned char)root->ch] = code;
    }

    makeCode(root->left, code + "0");
    makeCode(root->right, code + "1");
}

void huffmanAlg::fileReader(const string& filepath, char characters[], int frequencies[], int& size) {
    ifstream myFile(filepath);
    if (!myFile) {
        wxLogError("File not found.");
        return;
    }

    char ch;
    while (myFile.get(ch)) {
        bool found = false;
        for (int i = 0; i < size; i++) {
            if (characters[i] == ch) {
                frequencies[i]++;
                found = true;
                break;
            }
        }

        if (!found) {
            characters[size] = ch;
            frequencies[size] = 1;
            size++;
        }
    }
    myFile.close();
}

void huffmanAlg::compressFile(const wxString& filepath) {
    string filename = filepath.ToStdString();
    wxLogMessage("filename: %s", filename);


    char characters[256];
    int frequencies[256] = { 0 };
    int size = 0;

    fileReader(filename, characters, frequencies, size);

    if (size == 0) {
        wxLogError("error file size");
        return;
    }

    huffmanNode* root = huffmanTree(characters, frequencies, size);
    makeCode(root, "");

    ifstream inFile(filename);
    string content((istreambuf_iterator<char>(inFile)), istreambuf_iterator<char>());
    inFile.close();

    encodedText = "";
    for (char ch : content) {
        encodedText += huffmanCodes[(unsigned char)ch];
    }

    saveCompressedFile(filename + "-Compressed.bin");
    ofstream detailsFile(filename  + "-CompressionDetails.txt");
    for (int i = 0; i < 256; i++) {
        if (!huffmanCodes[i].empty()) {
            detailsFile << (char)i << ": " << huffmanCodes[i] << "\n";
        }
    }
    detailsFile.close();

    remove((filename).c_str());
    wxLogMessage("debug statement");
}

void huffmanAlg::saveCompressedFile(const string& fileName) {
    ofstream outFile(fileName, ios::binary);
    unsigned char byte = 0;
    int bitCount = 0;

    for (char bit : encodedText) {
        byte = (byte << 1) | (bit - '0');
        bitCount++;
        if (bitCount == 8) {
            outFile.put(byte);
            byte = 0;
            bitCount = 0;
        }
    }
    if (bitCount > 0) {
        byte <<= (8 - bitCount);
        outFile.put(byte);
    }
    outFile.close();
}

void huffmanAlg::decompressFile(const wxString& filepath) {
    string tempfilename = filepath.ToStdString();
    string filename = tempfilename.substr(0, tempfilename.find("-Compressed.bin"));

    string compressedFilename = filename + "-Compressed.bin";
    string detailsFilename = filename + "-CompressionDetails.txt";

    wxLogMessage("filename: %s", filename);
    wxLogMessage("compressed file name found: %s", compressedFilename);
    wxLogMessage("compressed details file name found: %s", detailsFilename);

    ifstream detailsFile(detailsFilename);
    if (!detailsFile) {
        wxLogMessage("Details file not found.");
        return;
    }

    for (int i = 0; i < 256; i++) {
        huffmanCodes[i] = "";
    }

    string line;
    while (getline(detailsFile, line)) {
        if (line.empty()) continue;
        char ch = line[0];
        string code = line.substr(3);
        huffmanCodes[(unsigned char)ch] = code;
    }
    detailsFile.close();

    huffmanNode* newRoot = new huffmanNode('\0', 0);
    for (int i = 0; i < 256; i++) {
        if (!huffmanCodes[i].empty()) {
            huffmanNode* current = newRoot;
            for (char bit : huffmanCodes[i]) {
                if (bit == '0') {
                    if (!current->left) {
                        current->left = new huffmanNode('\0', 0);
                    }
                    current = current->left;
                }
                else {
                    if (!current->right) {
                        current->right = new huffmanNode('\0', 0);
                    }
                    current = current->right;
                }
            }
            current->ch = i;
        }
    }

    string binaryData = "";
    ifstream inFile(compressedFilename, ios::binary);
    unsigned char byte;
    while (inFile.read(reinterpret_cast<char*>(&byte), 1)) {
        for (int i = 7; i >= 0; --i) {
            binaryData += ((byte >> i) & 1) ? '1' : '0';
        }
    }
    inFile.close();

    encodedText = binaryData;
    string decompressedText = decodeText(newRoot);

    string decompressedFileName = filename.substr(0, filename.find("-Compressed.bin"));
    saveDecompressedFile(decompressedFileName, decompressedText);

    remove(compressedFilename.c_str());
    remove(detailsFilename.c_str());

    wxLogMessage("File successfully decompressed.");
}

string huffmanAlg::decodeText(huffmanNode* root) {
    string decodedText = "";

    if (root == nullptr) {
        wxLogError("Error: Huffman tree root is null.");
        return decodedText;
    }

    huffmanNode* currentNode = root;

    for (char bit : encodedText) {
        if (bit == '0') {
            if (currentNode->left == nullptr) {
            
                return decodedText;
            }
            currentNode = currentNode->left;
        }
        else if (bit == '1') {
            if (currentNode->right == nullptr) {
                return decodedText;
            }
            currentNode = currentNode->right;
        }
        else {
            wxLogError("Error: Invalid bit encountered while decoding.");
            return decodedText;
        }

        if (!currentNode->left && !currentNode->right) {
            decodedText += currentNode->ch;
            currentNode = root;
        }
    }

    return decodedText;
}


void huffmanAlg::saveDecompressedFile(const string& fileName, const string& decompressedText) {
    ofstream outFile(fileName);
    outFile << decompressedText << endl;
    outFile.close();
}
