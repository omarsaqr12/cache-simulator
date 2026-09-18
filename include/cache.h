#ifndef CACHE_H
#define CACHE_H

#include <bitset>
#include <cstdint>
#include <iomanip>
#include <iostream>
#include <stdexcept>
#include <vector>

// A direct-mapped, read-only cache model. Misses incur a fixed 100-cycle
// memory penalty in addition to the configurable cache-access latency.
class Cache {
public:
    Cache(std::uint32_t cacheSize, std::uint32_t lineSize,
          std::uint32_t cacheAccessTime)
        : totalCacheSize(cacheSize), blockSize(lineSize),
          cyclesPerAccess(cacheAccessTime) {
        if (!isPowerOfTwo(cacheSize) || !isPowerOfTwo(lineSize) ||
            lineSize > cacheSize || cacheAccessTime < 1 ||
            cacheAccessTime > 10) {
            throw std::invalid_argument(
                "Cache and block sizes must be nonzero powers of two, block <= cache; latency must be 1..10");
        }
        numBlocks = cacheSize / lineSize;
        while ((std::uint64_t{1} << idxBits) < numBlocks) ++idxBits;
        while ((std::uint64_t{1} << offsetBits) < lineSize) ++offsetBits;
        storage.resize(numBlocks);
    }

    void request(std::uint32_t address) {
        const std::uint32_t index = (address >> offsetBits) & (numBlocks - 1);
        const unsigned shift = idxBits + offsetBits;
        const std::uint32_t tag = shift >= 32 ? 0 : (address >> shift);
        ++totalRequests;
        Line &line = storage[index];
        if (line.valid && line.tag == tag) {
            ++successfulHits;
            totalCycles += cyclesPerAccess;
        } else {
            ++unsuccessfulHits;
            line = {tag, true};
            totalCycles += cyclesPerAccess + memoryLatency;
        }
    }

    double computeHitRate() const {
        return totalRequests ? static_cast<double>(successfulHits) / totalRequests : 0.0;
    }
    double computeMissRate() const {
        return totalRequests ? static_cast<double>(unsuccessfulHits) / totalRequests : 0.0;
    }
    double computeAMAT() const {
        return totalRequests ? static_cast<double>(totalCycles) / totalRequests : 0.0;
    }
    std::uint64_t hits() const { return successfulHits; }
    std::uint64_t misses() const { return unsuccessfulHits; }
    std::uint64_t requests() const { return totalRequests; }

    void displayStats() const {
        std::cout << "Cache Status Report:\n";
        const unsigned tagBits = 32 - idxBits - offsetBits;
        for (std::uint32_t i = 0; i < numBlocks; ++i) {
            if (storage[i].valid) {
                const auto bits = std::bitset<32>(storage[i].tag).to_string();
                std::cout << "Block " << i << ": Active = 1, Tag = "
                          << bits.substr(32 - tagBits) << '\n';
            }
        }
        std::cout << "Cache Details:\n"
                  << "Cache Size: " << totalCacheSize << " bytes\n"
                  << "Block Size: " << blockSize << " bytes\n"
                  << "Total Blocks: " << numBlocks << '\n'
                  << "Access Time per Block: " << cyclesPerAccess << " cycles\n"
                  << "Memory Latency: " << memoryLatency << " cycles\n"
                  << "Total Requests: " << totalRequests << '\n'
                  << "Successful Hits: " << successfulHits << '\n'
                  << "Misses: " << unsuccessfulHits << '\n'
                  << std::fixed << std::setprecision(2)
                  << "Hit Rate: " << (100 * computeHitRate()) << "%\n"
                  << "Miss Rate: " << (100 * computeMissRate()) << "%\n"
                  << "Average Memory Access Time: " << computeAMAT() << " cycles\n";
    }

private:
    struct Line { std::uint32_t tag = 0; bool valid = false; };
    static bool isPowerOfTwo(std::uint32_t value) {
        return value != 0 && (value & (value - 1)) == 0;
    }
    static constexpr std::uint32_t memoryLatency = 100;
    std::uint32_t totalCacheSize, blockSize, cyclesPerAccess;
    std::uint32_t numBlocks = 0, idxBits = 0, offsetBits = 0;
    std::uint64_t totalRequests = 0, successfulHits = 0;
    std::uint64_t unsuccessfulHits = 0, totalCycles = 0;
    std::vector<Line> storage;
};

#endif  // CACHE_H
