/**
 * ArchTrade - Educational Computer Architecture Simulator
 * Architecture configuration: ISA, pipeline, cache, and I/O choices.
 */

#ifndef ARCHTRADE_ARCHITECTURE_CONFIG_H
#define ARCHTRADE_ARCHITECTURE_CONFIG_H

#include <string>

/**
 * Holds user-selected architecture options.
 * These choices affect CPI, stalls, and idle cycles in the simulation.
 */
struct ArchitectureConfig {
    bool isRISC = true;           // RISC vs CISC (affects base CPI / instruction count)
    int pipelineDepth = 1;        // 1 = single cycle, 5 = 5-stage pipeline
    std::string cacheType = "DirectMapped";  // "DirectMapped" or "SetAssociative"
    bool useDMA = false;          // true = DMA, false = Polling (affects CPU idle cycles)
};

#endif // ARCHTRADE_ARCHITECTURE_CONFIG_H
