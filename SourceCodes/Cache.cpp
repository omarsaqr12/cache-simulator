#ifndef CACHE_H
#define CACHE_H

#include <iostream>
#include <vector>
#include <cmath>
#include <iomanip>
#include <bitset>

using namespace std;

struct Line
{
    int identifier;
    bool isModified;
    bool isActive;
};

class Cache
{
private:
    unsigned int totalCacheSize;
    unsigned int blockSize;
    unsigned int numBlocks;
    unsigned int cyclesPerAccess;
    unsigned int timeToMemory;
    unsigned int totalRequests;
    unsigned int successfulHits;
    unsigned int unsuccessfulHits;
    unsigned int idxBits;
    unsigned int offsetBits;
    unsigned int tagBitCount;
    unsigned int totalCycles;
    vector<Line> storage;

    unsigned int calculateIndex(unsigned int addr);
    unsigned int extractTag(unsigned int addr);

public:
    Cache(unsigned int cacheSize, unsigned int lineSize, unsigned int cacheAccessTime);
    void request(unsigned int addr);
    void displayStats();
    float computeHitRate();
    float computeMissRate();
    float computeAMAT();
};

Cache::Cache(unsigned int cacheSize, unsigned int lineSize, unsigned int cacheAccessTime)
    : totalCacheSize(cacheSize), blockSize(lineSize), cyclesPerAccess(cacheAccessTime),
      totalRequests(0), successfulHits(0), unsuccessfulHits(0)
{
    timeToMemory = 100;                                   // Set memory latency to 100 cycles
    numBlocks = cacheSize / lineSize;                     // Determine block count
    idxBits = (unsigned int)log2(numBlocks);              // Derive index bits
    offsetBits = (unsigned int)log2(lineSize);            // Derive offset bits
    tagBitCount = 32 - idxBits - offsetBits;              // Adjust for 32-bit address
    storage.resize(numBlocks, {0, false, false});         // Initialize cache
}
void Cache::displayStats()
{
    cout << "Cache Status Report:\n";
    for (unsigned int i = 0; i < numBlocks; i++)
    {
        if (storage[i].isActive)
        {
            unsigned int tagValue = storage[i].identifier;
            string binaryRepresentation = bitset<32>(tagValue).to_string();
            binaryRepresentation = binaryRepresentation.substr(32 - tagBitCount);
            cout << "Block " << i << ": Active = " << storage[i].isActive
                 << ", Tag = " << binaryRepresentation << "\n";
        }
    }
    cout << "Cache Details:\n";
    cout << "Cache Size: " << totalCacheSize << " bytes\n";
    cout << "Block Size: " << blockSize << " bytes\n";
    cout << "Total Blocks: " << numBlocks << "\n";
    cout << "Access Time per Block: " << cyclesPerAccess << " cycles\n";
    cout << "Memory Latency: " << timeToMemory << " cycles\n";
    cout << "Total Requests: " << totalRequests << "\n";
    cout << "Successful Hits: " << successfulHits << "\n";
    cout << "Misses: " << unsuccessfulHits << "\n";
    cout << "Hit Rate: " << fixed << setprecision(2) << computeHitRate() << "\n";
    cout << "Miss Rate: " << fixed << setprecision(2) << computeMissRate() << "\n";
    cout << "Average Memory Access Time: " << fixed << setprecision(2) << computeAMAT() << " cycles\n";
}
unsigned int Cache::calculateIndex(unsigned int addr)
{
    return (addr >> offsetBits) & (numBlocks - 1);
}

unsigned int Cache::extractTag(unsigned int addr)
{
    return addr >> (idxBits + offsetBits);
}

void Cache::request(unsigned int addr)
{
    totalRequests++;
    unsigned int idx = calculateIndex(addr);
    unsigned int tagValue = extractTag(addr);

    if (storage[idx].isActive && storage[idx].identifier == tagValue)
    {
        successfulHits++;
        totalCycles += cyclesPerAccess;
    }
    else
    {
        unsuccessfulHits++;
        storage[idx].isActive = true;
        storage[idx].identifier = tagValue;
        totalCycles += (timeToMemory + cyclesPerAccess);
    }
}

float Cache::computeHitRate()
{
    return (float)successfulHits / totalRequests;
}

float Cache::computeMissRate()
{
    return (float)unsuccessfulHits / totalRequests;
}

float Cache::computeAMAT()
{
    return (float)totalCycles / totalRequests;
}

#endif // CACHE_H
