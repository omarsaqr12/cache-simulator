#include "../include/cache.h"
#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <vector>



using namespace std;

// Utility function to trim spaces from a line
string trimSpaces(const string &line)
{
    string cleanedLine;
    for (char ch : line)
    {
        if (ch != ' ')
        {
            cleanedLine += ch;
        }
    }
    return cleanedLine;
}

// Function to extract instruction and data access sequences from a file
void parseFile(const string &filepath, vector<unsigned int> &instructionSequence, vector<unsigned int> &dataSequence)
{
    ifstream inputFile(filepath);
    string lineContent;

    if (inputFile.is_open())
    {
        while (getline(inputFile, lineContent))
        {
            lineContent = trimSpaces(lineContent);
            string binaryAddress = lineContent.substr(0, lineContent.size() - 2);
            unsigned int address = stoul(binaryAddress, nullptr, 2);

            if (address > 0x3FFFFFFF)
            {
                cerr << "Error: Address exceeds 30 bits: " << address << endl;
                continue;
            }

            char accessType = lineContent.at(lineContent.size() - 1);
            if (accessType == 'I')
            {
                instructionSequence.push_back(address);
            }
            else if (accessType == 'D')
            {
                dataSequence.push_back(address);
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
        cerr << "Error opening file: " << filepath << endl;
    }
}

int main()
{
    // Cache configuration parameters
    unsigned int instructionCacheSize, dataCacheSize, blockSize, accessTime, latency;

    // User input for cache parameters
    cout << "Enter instruction cache size (in bytes): ";
    cin >> instructionCacheSize;

    cout << "Enter data cache size (in bytes): ";
    cin >> dataCacheSize;

    cout << "Enter cache block size (in bytes): ";
    cin >> blockSize;

    // Validation for power-of-2 sizes and constraints
    if ((instructionCacheSize & (instructionCacheSize - 1)) != 0 ||
        (dataCacheSize & (dataCacheSize - 1)) != 0 ||
        blockSize > instructionCacheSize ||
        dataCacheSize < blockSize)
    {
        cerr << "Error: Cache sizes must be powers of 2 and respect constraints." << endl;
        return 1;
    }

    cout << "Enter cache access latency (in cycles): ";
    cin >> accessTime;

    if (accessTime < 1 || accessTime > 10)
    {
        cerr << "Invalid cache access latency. Must be between 1 and 10 cycles." << endl;
        exit(2);
    }

    // Initialize cache objects
    Cache iCache(instructionCacheSize, blockSize, accessTime);
    Cache dCache(dataCacheSize, blockSize, accessTime);

    // File input for address sequence
    string sequenceFile;
    cout << "Enter access sequence file name: ";
    cin >> sequenceFile;

    vector<unsigned int> iSequence;
    vector<unsigned int> dSequence;
    parseFile(sequenceFile, iSequence, dSequence);

    // Instruction cache simulation
    cout << "Simulating Instruction Cache:\n";
    for (unsigned int address : iSequence)
    {
        iCache.request(address);
        iCache.displayStats();
    }

    // Data cache simulation
    cout << "\nSimulating Data Cache:\n";
    for (unsigned int address : dSequence)
    {
        dCache.request(address);
        dCache.displayStats();
    }

    // Final cache status
    cout << "-----------------------------------------------------------------------------" << endl;
    cout << "\nFinal Instruction Cache Status:\n";
    iCache.displayStats();

    cout << "\nFinal Data Cache Status:\n";
    dCache.displayStats();

    return 0;
}

