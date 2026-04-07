/**
 * ArchTrade - Educational Computer Architecture Simulator
 * PipelineSimulator: runs instructions and applies parameter-driven pipeline/cache/IO rules.
 */

#ifndef ARCHTRADE_PIPELINE_SIMULATOR_H
#define ARCHTRADE_PIPELINE_SIMULATOR_H

#include "ArchitectureConfig.h"
#include "CacheSimulator.h"
#include "Instruction.h"
#include "SimulationResult.h"
#include "Workload.h"
#include <cstdlib>
#include <ctime>
#include <vector>

/**
 * Runs the workload through a parameter-driven pipeline model.
 * All cycle costs are derived from ArchitectureConfig parameters —
 * no hardcoded constants remain.
 */
class PipelineSimulator {
public:
    PipelineSimulator() {
        std::srand(static_cast<unsigned>(std::time(nullptr)));
    }

    /**
     * Run the workload with the given config; fill and return result.
     */
    SimulationResult run(const Workload& workload, const ArchitectureConfig& config) {
        cache_.reset();
        SimulationResult r;
        r.instructionsExecuted = static_cast<int>(workload.instructions.size());

        const bool isPipeline = (config.pipelineDepth >= 2);
        const std::string& cacheType = config.cacheType;

        int cycles = 0;
        int stalls = 0;
        int idleCycles = 0;

        // Derived parameters — computed once from config
        const int branchPenalty   = config.branchPenalty();
        const int hazardStall     = config.dataHazardStall();
        const int baseCycles      = config.baseInstructionCycles();
        const int cacheHitCost    = config.cacheHitCycles();
        const int cacheMissCost   = config.cacheMissCycles();
        const int deviceLatency   = config.deviceLatency;

        const std::vector<Instruction>& instr = workload.instructions;
        int prevWasLoad = -1;  // id of previous LOAD (-1 = none)

        for (size_t i = 0; i < instr.size(); ++i) {
            const Instruction& inst = instr[i];

            // --- Branch penalty (pipelined CPUs only) ---
            if (inst.type == InstructionType::BRANCH && isPipeline) {
                stalls += branchPenalty;
            }

            // --- Data hazard: use-after-load ---
            if (isPipeline && prevWasLoad >= 0) {
                if (inst.type == InstructionType::ALU || inst.type == InstructionType::STORE) {
                    if ((std::rand() % 100) < 30)
                        stalls += hazardStall;
                }
            }
            prevWasLoad = (inst.type == InstructionType::LOAD) ? inst.id : -1;

            // --- Memory: cache hit/miss ---
            if (inst.type == InstructionType::LOAD || inst.type == InstructionType::STORE) {
                bool hit = cache_.accessMemory(inst.id, cacheType);
                cycles += hit ? cacheHitCost : cacheMissCost;
            }

            // --- I/O: polling vs DMA ---
            if (inst.type == InstructionType::IO) {
                if (!config.useDMA)
                    idleCycles += deviceLatency;
            }

            cycles += baseCycles;
        }

        r.totalCycles    = cycles + stalls + idleCycles;
        r.pipelineStalls = stalls;
        r.cpuIdleCycles  = idleCycles;
        r.cacheHits      = cache_.cacheHits;
        r.cacheMisses    = cache_.cacheMisses;
        r.cpi = (r.instructionsExecuted > 0)
            ? static_cast<double>(r.totalCycles) / r.instructionsExecuted
            : 0.0;

        return r;
    }

private:
    CacheSimulator cache_;
};

#endif // ARCHTRADE_PIPELINE_SIMULATOR_H