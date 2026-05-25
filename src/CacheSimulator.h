/**
 * ArchTrade - Educational Computer Architecture Simulator
 * CacheSimulator:
 * realistic educational cache behavior model.
 */

#ifndef ARCHTRADE_CACHE_SIMULATOR_H
#define ARCHTRADE_CACHE_SIMULATOR_H

#include "ArchitectureConfig.h"

#include <random>
#include <unordered_set>
#include <cmath>

class CacheSimulator {
private:

    std::mt19937 rng;
    std::uniform_int_distribution<int> percentDist;

    // Recently accessed addresses
    std::unordered_set<int> cacheLines;

    // Previous address for locality tracking
    int lastAddress = -1;

public:

    int cacheHits = 0;
    int cacheMisses = 0;

    CacheSimulator()
        : rng(std::random_device{}()),
          percentDist(0, 99) {}

    /**
     * Simulate memory access.
     */
    bool accessMemory(
        int address,
        const ArchitectureConfig& config
    ) {

        int hitRate =
            config.cacheHitRatePercent();

        // ============================================
        // Sequential locality bonus
        // ============================================

        if (lastAddress >= 0 &&
            std::abs(address - lastAddress) <= 2) {

            hitRate += 20;
        }

        // ============================================
        // Temporal locality bonus
        // ============================================

        if (cacheLines.count(address)) {

            hitRate += 15;
        }

        // ============================================
        // Random access penalty
        // ============================================

        if ((address % 13) == 0) {

            hitRate -= 45;
        }

        // ============================================
        // DirectMapped conflict penalty
        // ============================================

        if (config.cacheType ==
            CacheType::DirectMapped) {

            if ((address % 8) == 0) {

                hitRate -= 20;
            }
        }

        // ============================================
        // Clamp realistic bounds
        // ============================================

        if (hitRate > 98)
            hitRate = 98;

        if (hitRate < 5)
            hitRate = 5;

        // ============================================
        // Final cache outcome
        // ============================================

        bool hit =
            percentDist(rng) < hitRate;

        // ============================================
        // Update stats
        // ============================================

        if (hit) {

            ++cacheHits;

            cacheLines.insert(address);
        }
        else {

            ++cacheMisses;
        }

        // ============================================
        // Simulate limited cache capacity
        // ============================================

        int maxLines =
            config.cacheSizeKB / 2;

        if ((int)cacheLines.size() > maxLines) {

            cacheLines.clear();
        }

        lastAddress = address;

        return hit;
    }

    /**
     * Reset cache state.
     */
    void reset() {

        cacheHits = 0;
        cacheMisses = 0;

        cacheLines.clear();

        lastAddress = -1;
    }
};

#endif // ARCHTRADE_CACHE_SIMULATOR_H