#include <cstdlib>
#include <iostream>
#include <fstream>
#include <queue>
#include <string>
#include <vector>
#include <cstdio>

using namespace std;

//Huffman Node Struct
struct huffmanNode {
    char ch;
    int freq;
    huffmanNode* left;
    huffmanNode* right;

    huffmanNode(char character, int frequency) : ch(character), freq(frequency), left(nullptr), right(nullptr) {}
};

//Comparison struct to check frequency
struct Compare {
    bool operator()(huffmanNode* left, huffmanNode* right) {
        return left->freq > right->freq;
    }
};

//Huffman Algorithm Class
class huffmanAlg {
public:
    string huffmanCodes[256];
    string encodedText;

    huffmanAlg() {}

    //Create File for Text to be compressed
    void createTextFile() {
        string filename, content;
        cout << "Enter the name of the text file: ";
        cin >> filename;
        cin.ignore();
        cout << "Enter the content of the file: \n";
        getline(cin, content);

        ofstream outFile(filename + ".txt");
        if (!outFile) {
            cerr << "Error creating file." << endl;
            system("pause");
            return;
        }
        outFile << content;
        outFile.close();
        cout << "File saved successfully." << endl;
        system("pause");
    }

    //Filereader function
    void fileReader(const string& filename, char characters[], int frequencies[], int& size) {
        ifstream myFile(filename);
        if (!myFile) {
            cerr << "File not found." << endl;
            system("pause");
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

    //Build the Huffman Tree
    huffmanNode* huffmanTree(const char characters[], const int frequencies[], int size) {
        //Priority Queue built in queue library to use min heap
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

    //Creation of Huffman code 
    void makeCode(huffmanNode* root, string code) {
        if (!root)
            return;

        if (!root->left && !root->right) {
            huffmanCodes[(unsigned char)root->ch] = code;
        }

        makeCode(root->left, code + "0");
        makeCode(root->right, code + "1");
    }

    //Changing the structure of a text file to compress it into a .bin file
    void compressFile() {
        string filename;
        cout << "Enter the name of the text file to compress: ";
        cin >> filename;

        char characters[256];
        int frequencies[256] = {0};
        int size = 0;

        fileReader(filename + ".txt", characters, frequencies, size);

        if (size == 0) {
            cerr << "No content in the file to compress." << endl;
            system("pause");
            return;
        }

        huffmanNode* root = huffmanTree(characters, frequencies, size);
        makeCode(root, "");

        ifstream inFile(filename + ".txt");
        string content((istreambuf_iterator<char>(inFile)), istreambuf_iterator<char>());
        inFile.close();

        encodedText = "";
        for (char ch : content) {
            encodedText += huffmanCodes[(unsigned char)ch];
        }

        saveCompressedFile(filename + "-Compressed.bin");
        ofstream detailsFile(filename + "-CompressionDetails.txt");
        for (int i = 0; i < 256; i++) {
            if (!huffmanCodes[i].empty()) {
                detailsFile << (char)i << ": " << huffmanCodes[i] << "\n";
            }
        }
        detailsFile.close();

        remove((filename + ".txt").c_str());

        cout << "File compressed successfully." << endl;
        system("pause");
    }

    //Decompressing the .bin file to discover the text
    void decompressFile() {
        string filename;
        cout << "Enter the name of the compressed file (without -Compressed.bin): ";
        cin >> filename;

        string compressedFilename = filename + "-Compressed.bin";
        string detailsFilename = filename + "-CompressionDetails.txt";

        ifstream detailsFile(detailsFilename);
        if (!detailsFile) {
            cerr << "Details file not found." << endl;
            system("pause");
            return;
        }

        for (int i = 0; i < 256; i++) {
            huffmanCodes[i] = "";
        }

        string line;
        while (getline(detailsFile, line)) {
            if (line.empty())
                continue;
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
                    } else {
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

        saveDecompressedFile(filename + ".txt", decompressedText);

        remove(compressedFilename.c_str());
        remove(detailsFilename.c_str());

        cout << "File decompressed successfully." << endl;
        system("pause");
    }

    string decodeText(huffmanNode* root) {
        string decodedText = "";
        huffmanNode* currentNode = root;

        for (char bit : encodedText) {
            if (bit == '0') {
                currentNode = currentNode->left;
            } else {
                currentNode = currentNode->right;
            }

            if (!currentNode->left && !currentNode->right) {
                decodedText += currentNode->ch;
                currentNode = root;
            }
        }

        return decodedText;
    }

    void saveCompressedFile(const string& fileName) {
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

    void saveDecompressedFile(const string& fileName, const string& decompressedText) {
        ofstream outFile(fileName);
        outFile << decompressedText << endl;
        outFile.close();
    }

    int menu() {
        int option;
        cout << "*********************************************************" << endl;
        cout << "|                          Menu                         |" << endl;
        cout << "|                                                       |" << endl;
        cout << "| 1. Create Text File                                   |" << endl;
        cout << "| 2. Compress a Text File                               |" << endl;
        cout << "| 3. Decompress a File                                  |" << endl;
        cout << "| 4. Exit                                               |" << endl;
        cout << "*********************************************************" << endl;
        cout << ">: ";
        cin >> option;
        return option;
    }
};

//Main
int main() {
    huffmanAlg huffAlg;
    while (true) {
        system("cls");
        int choice = huffAlg.menu();

        switch (choice) {
            case 1:
                huffAlg.createTextFile();
                break;
            case 2:
                huffAlg.compressFile();
                break;
            case 3:
                huffAlg.decompressFile();
                break;
            case 4:
                cout << "Thank you for using the program!\n";
                system("pause");
                return 0;
            default:
                cout << "Invalid choice. Please try again." << endl;
                system("pause");
        }
    }
}
