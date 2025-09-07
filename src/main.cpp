#include "../include/cache.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std; // Use the standard namespace

// Function to remove spaces in a line
string eliminateSpaces(const string &line)
{
    string processed;
    for (char character : line)
    {
        if (character != ' ')
        {
            processed += character;
        }
    }
    return processed;
}

// Function to read the access sequence from a file
void parseAccessFile(const string &filename, vector<unsigned int> &instructionSequence, vector<unsigned int> &dataSequence)
{
    ifstream inputFile(filename);
    string currentLine;

    if (inputFile.is_open())
    {
        while (getline(inputFile, currentLine))
        {
            currentLine = eliminateSpaces(currentLine);
            string binaryAddr = currentLine.substr(0, currentLine.size() - 2);
            unsigned int addr = stoul(binaryAddr, nullptr, 2);
            if (addr > 0xFFFFFFFF)
            {
                cerr << "Error: Address exceeds 32 bits: " << addr << endl;
                continue;
            }
            char accessType = currentLine.at(currentLine.size() - 1);
            if (accessType == 'I')
            {
                instructionSequence.push_back(addr);
            }
            else if (accessType == 'D')
            {
                dataSequence.push_back(addr);
            }
            else
            {
                cerr << "Error: Invalid access type: " << accessType << endl;
                exit(1);
            }
        }
        inputFile.close();
    }
    else
    {
        cerr << "Error opening file: " << filename << endl;
    }
}

int main()
{
    // Cache configurations
    unsigned int icacheSize, dcacheSize, blockSize, accessLatency;

    // Get cache configurations from user input
    cout << "Enter instruction cache size (in bytes): ";
    cin >> icacheSize;
    cout << "Enter data cache size (in bytes): ";
    cin >> dcacheSize;

    cout << "Enter cache block size (in bytes): ";
    cin >> blockSize;
    // Check if cache sizes are powers of 2 and not exceeding the cache size
    if ((icacheSize & (icacheSize - 1)) != 0 || (dcacheSize & (dcacheSize - 1)) != 0 || blockSize > icacheSize || dcacheSize < blockSize)
    {
        cerr << "Error: Cache sizes must be powers of 2 and not exceed the cache size." << endl;
        return 1;
    }
    cout << "Enter cache access latency (in cycles): ";
    cin >> accessLatency;
    if (accessLatency < 1 || accessLatency > 10)
    {
        cout << "Invalid number of cycles to access the cache. Must be between 1 and 10." << endl;
        exit(2);
    }
    // Instantiate caches
    Cache instructionCache(icacheSize, blockSize, accessLatency);
    Cache dataCache(dcacheSize, blockSize, accessLatency);
    // Read the access sequences
    string filename;
    cout << "Enter access sequence file name: ";
    cin >> filename;

    vector<unsigned int> instructionSequence;
    vector<unsigned int> dataSequence;
    parseAccessFile(filename, instructionSequence, dataSequence);

    // Simulate instruction cache behavior
    cout << "Simulating Instruction Cache:\n";
    for (unsigned int addr : instructionSequence)
    {
        instructionCache.request(addr);
        instructionCache.displayStats();
    }

    // Simulate data cache behavior
    cout << "\nSimulating Data Cache:\n";
    for (unsigned int addr : dataSequence)
    {
        dataCache.request(addr);
        dataCache.displayStats();
    }

    // Print final status of caches
    cout << "-----------------------------------------------------------------------------" << endl;
    cout << "\nFinal Instruction Cache Status:\n";
    instructionCache.displayStats();
    cout << "\nFinal Data Cache Status:\n";
    dataCache.displayStats();

    return 0;
}
