/**
 * ArchTrade - Educational Computer Architecture Simulator
 * PipelineSimulator: runs instructions and applies simple pipeline/cache/IO rules.
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
 * Runs the workload through a simplified pipeline model.
 * Applies: base CPI = 1, branch penalty, data hazard stalls, cache miss latency, polling idle.
 */
class PipelineSimulator {
public:
    static const int BRANCH_PENALTY_CYCLES = 3;   // cycles lost on branch in 5-stage // if a branch occurs in a pipeline
    static const int MEMORY_MISS_LATENCY = 10;    // extra cycles per cache miss // 10 lost if cache miss
    static const int POLLING_IDLE_PER_IO = 5;     // idle cycles per IO under polling // if polling is used 
    static const int DATA_HAZARD_STALLS = 2;      // stalls when dependent on previous LOAD

    PipelineSimulator() {
        std::srand(static_cast<unsigned>(std::time(nullptr)));
    }

    /**
     * Run the workload with the given config; fill and return result.
     */
    SimulationResult run(const Workload& workload, const ArchitectureConfig& config) {
        cache_.reset();
        SimulationResult r;
        r.instructionsExecuted = static_cast<int>(workload.instructions.size()); // counts instructions executed

        const bool isPipeline = (config.pipelineDepth == 5); // if pipeline = 5 cpu uses pipelining
        // else single cycle 
        const std::string& cacheType = config.cacheType;
        int cycles = 0;
        int stalls = 0;
        int idleCycles = 0;

        const std::vector<Instruction>& instr = workload.instructions;
        int prevWasLoad = -1;  // id of previous LOAD (-1 = none)

        // cpu executes each instructions one by one 
        for (size_t i = 0; i < instr.size(); ++i) {
            const Instruction& inst = instr[i];
            int baseCycles = 1;  // base CPI = 1 (RISC)
            if (!config.isRISC)
                baseCycles = 2;  // CISC: often 1–2 cycles per instruction (complex ops)

            // --- Branch penalty (5-stage pipeline only) ---
            if (inst.type == InstructionType::BRANCH && isPipeline) {
                stalls += BRANCH_PENALTY_CYCLES;
            }  // add 3 stall cycles

            // --- Data hazard: use after load ---
            if (isPipeline && prevWasLoad >= 0) {
                if (inst.type == InstructionType::ALU || inst.type == InstructionType::STORE) {
                    // Some probability of data dependency (e.g. 30%)
                    if ((std::rand() % 100) < 30)
                        stalls += DATA_HAZARD_STALLS;
                }
            }
            if (inst.type == InstructionType::LOAD)
                prevWasLoad = inst.id;
            else
                prevWasLoad = -1;

            // --- Memory: cache hit/miss ---
            if (inst.type == InstructionType::LOAD || inst.type == InstructionType::STORE) {
                bool hit = cache_.accessMemory(inst.id, cacheType);
                if (!hit)
                    cycles += MEMORY_MISS_LATENCY;
            }

            // --- I/O: polling vs DMA ---
            if (inst.type == InstructionType::IO) {
                if (!config.useDMA)
                    idleCycles += POLLING_IDLE_PER_IO;
            }

            cycles += baseCycles;
        }

        r.totalCycles = cycles + stalls + idleCycles;
        r.pipelineStalls = stalls;
        r.cacheHits = cache_.cacheHits;
        r.cacheMisses = cache_.cacheMisses;
        r.cpuIdleCycles = idleCycles;
        r.cpi = (r.instructionsExecuted > 0)
            ? static_cast<double>(r.totalCycles) / r.instructionsExecuted
            : 0.0;

        return r;
    }

private:
    CacheSimulator cache_;
};

#endif // ARCHTRADE_PIPELINE_SIMULATOR_H
