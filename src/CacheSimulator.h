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
 *
 * Hit probability is fully derived from ArchitectureConfig:
 *   - cacheType: SetAssociative gets a higher base rate than DirectMapped
 *   - cacheSizeKB: larger caches improve hit rate (capped at 95 %)
 *
 * The locality heuristic gives a second-chance hit on spatially adjacent
 * addresses, mimicking sequential access patterns seen in real workloads.
 */
class CacheSimulator {
public:
    int cacheHits   = 0;
    int cacheMisses = 0;

    CacheSimulator() {
        std::srand(static_cast<unsigned>(std::time(nullptr)));
    }

    /**
     * Simulate one memory access. Returns true if cache hit, false if miss.
     *
     * @param addressOrId  Instruction id used as a proxy address.
     * @param config       Full architecture config (drives hit-rate calculation).
     */
    bool accessMemory(int addressOrId, const ArchitectureConfig& config) {
        int hitRate = config.cacheHitRatePercent(); // 0-95 from config

        bool hit = (std::rand() % 100) < hitRate;

        // Spatial locality bonus: sequential IDs share a cache line → extra
        // second-chance hit (simulates stride-1 access patterns).
        if (!hit && (addressOrId % 8) < 3) {
            hit = (std::rand() % 100) < 40;
        }

        if (hit)
            ++cacheHits;
        else
            ++cacheMisses;

        return hit;
    }

    void reset() {
        cacheHits   = 0;
        cacheMisses = 0;
    }
};

#endif // ARCHTRADE_CACHE_SIMULATOR_H