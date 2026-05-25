/**
 * ArchTrade - Educational Computer Architecture Simulator
<<<<<<< HEAD
 * CacheSimulator:
 * realistic educational cache behavior model.
=======
 * CacheSimulator: simplified cache hit/miss for memory instructions.
>>>>>>> fe6656cc5573ce4a019edd25d5b2d55b50053d6c
 */

#ifndef ARCHTRADE_CACHE_SIMULATOR_H
#define ARCHTRADE_CACHE_SIMULATOR_H

#include "ArchitectureConfig.h"
<<<<<<< HEAD

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
=======
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
>>>>>>> fe6656cc5573ce4a019edd25d5b2d55b50053d6c

        return hit;
    }

<<<<<<< HEAD
    /**
     * Reset cache state.
     */
    void reset() {

        cacheHits = 0;
        cacheMisses = 0;

        cacheLines.clear();

        lastAddress = -1;
=======
    void reset() {
        cacheHits   = 0;
        cacheMisses = 0;
>>>>>>> fe6656cc5573ce4a019edd25d5b2d55b50053d6c
    }
};

#endif // ARCHTRADE_CACHE_SIMULATOR_H