#include "../include/cache.h"
#include <cassert>
#include <cmath>
#include <stdexcept>

int main() {
    Cache cache(4, 1, 2);
    assert(cache.requests() == 0 && cache.computeAMAT() == 0);
    assert(cache.computeHitRate() == 0 && cache.computeMissRate() == 0);
    cache.request(1);        // miss: index 1, tag 0
    cache.request(1);        // hit
    cache.request(5);        // miss: index 1, tag 1
    cache.request(1);        // miss: original line evicted
    assert(cache.requests() == 4 && cache.hits() == 1 && cache.misses() == 3);
    assert(std::fabs(cache.computeAMAT() - 77.0) < 1e-9);
    Cache fullWidth(2, 1, 1);
    fullWidth.request(0x80000000u);
    fullWidth.request(0u);
    assert(fullWidth.misses() == 2);  // highest address bit must not be lost
    for (unsigned size : {0u, 3u}) {
        bool rejected = false;
        try { Cache bad(size, 1, 1); } catch (const std::invalid_argument &) { rejected = true; }
        assert(rejected);
    }
    for (unsigned block : {0u, 3u, 8u}) {
        bool rejected = false;
        try { Cache bad(4, block, 1); } catch (const std::invalid_argument &) { rejected = true; }
        assert(rejected);
    }
}
