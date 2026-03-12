/**
 * ArchTrade - Educational Computer Architecture Simulator
 * CacheSimulator: simplified cache hit/miss for memory instructions.
 */

#ifndef ARCHTRADE_CACHE_SIMULATOR_H
#define ARCHTRADE_CACHE_SIMULATOR_H

#include "ArchitectureConfig.h"
#include <cstdlib>
#include <ctime>
#include <string>

/**
 * Tracks cache hits and misses for LOAD/STORE instructions.
 * Uses simple probabilistic model: Direct Mapped has lower hit rate than Set Associative.
 */
class CacheSimulator {
public:
    int cacheHits = 0;
    int cacheMisses = 0;

    CacheSimulator() {
        std::srand(static_cast<unsigned>(std::time(nullptr)));
    }

    /**
     * Simulate a memory access. Returns true if hit, false if miss.
     * Hit probability is higher for Set Associative caches.
     */
    bool accessMemory(int addressOrId, const std::string& cacheType) {
        bool hit;
        if (cacheType == "SetAssociative") {
            // Set associative: higher base hit rate (~90%)
            hit = (std::rand() % 100) < 90;
        } else {
            // Direct mapped: lower hit rate (~78%)
            hit = (std::rand() % 100) < 78;
        }
        // Locality effect: repeated access to same cache block more likely a hit
        if (!hit && (addressOrId % 8) == ((addressOrId / 8) % 8)) {
            hit = (std::rand() % 100) < 50;
        }
        if (hit)
            cacheHits++;
        else
            cacheMisses++;
        return hit;
    }

    void reset() {
        cacheHits = 0;
        cacheMisses = 0;
    }
};

#endif // ARCHTRADE_CACHE_SIMULATOR_H