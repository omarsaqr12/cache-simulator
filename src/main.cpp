#include "../include/cache.h"

#include <cstdint>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

struct Access { std::uint32_t address; char kind; };

// One 32-bit binary address immediately followed by I or D; blank lines are
// ignored. Invalid records fail the entire trace instead of truncating data.
std::vector<Access> parseAccessFile(const std::string &filename) {
    std::ifstream file(filename);
    if (!file) throw std::runtime_error("Cannot open access file: " + filename);
    std::vector<Access> accesses;
    std::string line;
    std::size_t lineNumber = 0;
    while (std::getline(file, line)) {
        ++lineNumber;
        const auto first = line.find_first_not_of(" \t\r");
        if (first == std::string::npos) continue;
        const auto last = line.find_last_not_of(" \t\r");
        line = line.substr(first, last - first + 1);
        if (line.size() != 33 || (line.back() != 'I' && line.back() != 'D')) {
            throw std::runtime_error("Malformed access on line " + std::to_string(lineNumber));
        }
        std::uint32_t address = 0;
        for (std::size_t i = 0; i < 32; ++i) {
            if (line[i] != '0' && line[i] != '1') {
                throw std::runtime_error("Nonbinary address on line " + std::to_string(lineNumber));
            }
            address = (address << 1) | static_cast<std::uint32_t>(line[i] - '0');
        }
        accesses.push_back({address, line.back()});
    }
    if (!file.eof() && file.fail()) {
        throw std::runtime_error("Failed reading access file: " + filename);
    }
    return accesses;
}

int main() {
    try {
        std::uint32_t iSize, dSize, block, latency;
        std::string filename;
        std::cout << "Enter instruction cache size (in bytes): ";
        if (!(std::cin >> iSize)) throw std::runtime_error("Invalid instruction cache size");
        std::cout << "Enter data cache size (in bytes): ";
        if (!(std::cin >> dSize)) throw std::runtime_error("Invalid data cache size");
        std::cout << "Enter cache block size (in bytes): ";
        if (!(std::cin >> block)) throw std::runtime_error("Invalid block size");
        std::cout << "Enter cache access latency (in cycles): ";
        if (!(std::cin >> latency)) throw std::runtime_error("Invalid cache access latency");
        std::cout << "Enter access sequence file name: ";
        if (!(std::cin >> filename)) throw std::runtime_error("Missing access filename");

        Cache instructions(iSize, block, latency);
        Cache data(dSize, block, latency);
        const auto accesses = parseAccessFile(filename);
        for (const auto &access : accesses) {
            (access.kind == 'I' ? instructions : data).request(access.address);
        }
        std::cout << "\nFinal Instruction Cache Status:\n";
        instructions.displayStats();
        std::cout << "\nFinal Data Cache Status:\n";
        data.displayStats();
        return 0;
    } catch (const std::exception &error) {
        std::cerr << "Error: " << error.what() << '\n';
        return 1;
    }
}
