/**
 * ArchTrade - Educational Computer Architecture Simulator
 * Architecture configuration: ISA, pipeline, cache, and I/O choices.
 */

#ifndef ARCHTRADE_ARCHITECTURE_CONFIG_H
#define ARCHTRADE_ARCHITECTURE_CONFIG_H

#include <string>

/**
 * Holds user-selected architecture options.
 * Hardware parameters drive the simulation instead of hardcoded constants.
 */
struct ArchitectureConfig {
    // High-level choices
    bool isRISC = true;
    int pipelineDepth = 1;          // 1 = single cycle, 5 = 5-stage
    std::string cacheType = "DirectMapped";
    bool useDMA = false;

    // Memory hierarchy latencies (cycles)
    int cacheSizeKB = 32;
    int cacheLatency = 2;           // cycles to access cache (hit)
    int ramLatency = 20;            // extra cycles on cache miss
    int memoryBusLatency = 1;       // bus overhead per memory op

    // Execution latencies (cycles)
    int aluLatency = 1;             // cycles for ALU instruction
    int decodeLatency = 1;          // decode overhead (extra for CISC)

    // Branch resolution
    int branchResolutionStage = 3;  // pipeline stage where branch resolves

    // I/O
    int deviceLatency = 15;         // polling idle cycles per IO op

    /**
     * Compute derived values from config parameters.
     */

    /** Branch penalty: cycles wasted in pipeline on taken branch */
    int branchPenalty() const {
        if (pipelineDepth <= 1) return 0;
        return pipelineDepth - branchResolutionStage;
    }

    /** Data hazard stall cycles (depend on ALU latency) */
    int dataHazardStall() const {
        return aluLatency;
    }

    /** Base instruction cycles: decode + execute */
    int baseInstructionCycles() const {
        int cycles = decodeLatency + aluLatency;
        if (!isRISC) cycles += decodeLatency; // CISC extra decode overhead
        return cycles;
    }

    /** Cache hit cost */
    int cacheHitCycles() const {
        return cacheLatency + memoryBusLatency;
    }

    /** Cache miss cost */
    int cacheMissCycles() const {
        return cacheLatency + ramLatency + memoryBusLatency;
    }
};

#endif // ARCHTRADE_ARCHITECTURE_CONFIG_H