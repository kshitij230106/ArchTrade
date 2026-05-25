/**
 * ArchTrade - Educational Computer Architecture Simulator
 * Architecture configuration: ISA, pipeline, cache, and I/O choices.
 */

#ifndef ARCHTRADE_ARCHITECTURE_CONFIG_H
#define ARCHTRADE_ARCHITECTURE_CONFIG_H

#include <string>
<<<<<<< HEAD
#include <cmath>

/**
 * ISA types
 */
enum class ISAType {
    RISC,
    CISC
};

/**
 * Cache organization types
 */
enum class CacheType {
    DirectMapped,
    SetAssociative
};

/**
 * I/O handling types
 */
enum class IOType {
    Polling,
    DMA
};
=======
>>>>>>> fe6656cc5573ce4a019edd25d5b2d55b50053d6c

/**
 * Holds user-selected architecture options.
 * Hardware parameters drive the simulation instead of hardcoded constants.
 */
<<<<<<< HEAD
class ArchitectureConfig {
public:

    // =========================
    // High-level architecture
    // =========================

    ISAType isa = ISAType::RISC;

    // Number of pipeline stages
    int pipelineDepth = 1;

    CacheType cacheType = CacheType::DirectMapped;

    IOType ioType = IOType::Polling;

    // =========================
    // Memory hierarchy
    // =========================

    int cacheSizeKB = 32;

    // Cache hit latency
    int cacheLatency = 2;

    // Main memory access latency
    int ramLatency = 20;

    // Bus transfer overhead
    int memoryBusLatency = 1;

    // =========================
    // Execution latencies
    // =========================

    // ALU execution cycles
    int aluLatency = 1;

    // Instruction decode cycles
    int decodeLatency = 1;

    // =========================
    // Branch handling
    // =========================

    // Stage where branch resolves
    int branchResolutionStage = 3;

    // Branch prediction accuracy (%)
    int branchPredictionAccuracy = 85;

    // =========================
    // I/O
    // =========================

    // Polling/device wait latency
    int deviceLatency = 15;

    // =========================
    // Future scalability
    // =========================

    // Instructions issued per cycle
    int issueWidth = 1;

    // =========================================================
    // Validation
    // =========================================================

    /**
     * Validate architecture parameters.
     */
    bool isValid() const {
        return
            pipelineDepth >= 1 &&
            cacheSizeKB > 0 &&
            cacheLatency > 0 &&
            ramLatency > 0 &&
            memoryBusLatency >= 0 &&
            aluLatency > 0 &&
            decodeLatency > 0 &&
            branchResolutionStage >= 1 &&
            branchPredictionAccuracy >= 0 &&
            branchPredictionAccuracy <= 100 &&
            deviceLatency >= 0 &&
            issueWidth >= 1;
    }

    // =========================================================
    // Derived metrics
    // =========================================================

    /**
     * Branch penalty:
     * cycles lost on branch misprediction.
     */
    int branchPenalty() const {
        if (pipelineDepth <= 1)
            return 0;

        int penalty = pipelineDepth - branchResolutionStage;

        return (penalty > 0) ? penalty : 0;
    }

    /**
     * Effective branch penalty after prediction accuracy.
     */
    double effectiveBranchPenalty() const {
        double mispredictionRate =
            (100.0 - branchPredictionAccuracy) / 100.0;

        return branchPenalty() * mispredictionRate;
    }

    /**
     * Data hazard stall cycles.
     */
=======
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
>>>>>>> fe6656cc5573ce4a019edd25d5b2d55b50053d6c
    int dataHazardStall() const {
        return aluLatency;
    }

    /**
<<<<<<< HEAD
     * Base instruction execution cycles.
     *
     * CISC instructions require extra decode work
     * due to variable-length instruction formats.
     */
    int baseInstructionCycles() const {

        int cycles = decodeLatency + aluLatency;

        if (isa == ISAType::CISC) {
            cycles += decodeLatency;
        }

        return cycles;
    }

    /**
     * Cache hit latency.
     */
=======
     * Base instruction cycles: decode + execute.
     * CISC adds an extra decode pass (variable-length instruction overhead).
     */
    int baseInstructionCycles() const {
        int cycles = decodeLatency + aluLatency;
        if (!isRISC) cycles += decodeLatency; // CISC extra decode overhead
        return cycles;
    }

    /** Cache hit cost: latency + bus overhead */
>>>>>>> fe6656cc5573ce4a019edd25d5b2d55b50053d6c
    int cacheHitCycles() const {
        return cacheLatency + memoryBusLatency;
    }

<<<<<<< HEAD
    /**
     * Cache miss latency.
     */
    int cacheMissCycles() const {
        return cacheLatency +
               ramLatency +
               memoryBusLatency;
    }

    /**
     * Compute approximate cache hit rate (%).
     *
     * Educational approximation:
     * - Set associative caches perform better
     * - Larger caches improve locality
     * - Logarithmic scaling models diminishing returns
     */
    int cacheHitRatePercent() const {

        int base =
            (cacheType == CacheType::SetAssociative)
            ? 85
            : 70;

        // Logarithmic scaling
        int sizeBonus =
            static_cast<int>(std::log2(cacheSizeKB));

        int rate = base + sizeBonus;

        // Clamp realistic bounds
        if (rate > 95)
            rate = 95;

        if (rate < 50)
            rate = 50;

=======
    /** Cache miss cost: latency + RAM penalty + bus overhead */
    int cacheMissCycles() const {
        return cacheLatency + ramLatency + memoryBusLatency;
    }

    /**
     * Compute cache hit rate (0-100) based on cache type and size.
     *  - SetAssociative has a higher base hit rate than DirectMapped.
     *  - Larger caches increase the hit rate, capped at 95%.
     */
    int cacheHitRatePercent() const {
        int base = (cacheType == "SetAssociative") ? 85 : 70;
        int sizeBonus = cacheSizeKB / 16; // +1% per 16 KB
        int rate = base + sizeBonus;
        if (rate > 95) rate = 95;
>>>>>>> fe6656cc5573ce4a019edd25d5b2d55b50053d6c
        return rate;
    }

    /**
<<<<<<< HEAD
     * Pipeline startup overhead:
     * cycles required to initially fill pipeline.
     */
    int pipelineStartupCycles() const {

        if (pipelineDepth <= 1)
            return 0;

        return pipelineDepth - 1;
    }

    /**
     * Compute theoretical IPC.
     *
     * Simple educational estimate:
     * deeper pipelines + wider issue improve IPC.
     */
    double theoreticalIPC() const {

        double ipc = static_cast<double>(issueWidth);

        // Pipeline benefit
        if (pipelineDepth > 1) {
            ipc *= 1.2;
        }

        // CISC penalty
        if (isa == ISAType::CISC) {
            ipc *= 0.9;
        }

        return ipc;
    }

    /**
     * Compute theoretical CPI.
     */
    double theoreticalCPI() const {

        double ipc = theoreticalIPC();

        if (ipc <= 0.0)
            return 1.0;

        return 1.0 / ipc;
    }
=======
     * Pipeline startup overhead for first few instructions (pipelined CPUs only).
     * Reflects pipeline fill latency.
     */
    int pipelineStartupCycles() const {
        if (pipelineDepth <= 1) return 0;
        return pipelineDepth - 1;
    }
>>>>>>> fe6656cc5573ce4a019edd25d5b2d55b50053d6c
};

#endif // ARCHTRADE_ARCHITECTURE_CONFIG_H