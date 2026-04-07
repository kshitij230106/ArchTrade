#include "ArchitectureConfig.h"
#include "PipelineSimulator.h"
#include "SimulationResult.h"
#include "Workload.h"
#include <iostream>
#include <string>

/**
 * Map high-level user choices onto ArchitectureConfig hardware parameters.
 * Hardware parameters passed as CLI args override defaults set here.
 */
static ArchitectureConfig buildConfig(
    const std::string& instructionSetStr,
    const std::string& pipelineStr,
    const std::string& cacheStr,
    const std::string& ioStr)
{
    ArchitectureConfig config;

    // ── Instruction Set ──────────────────────────────────────────────
    if (instructionSetStr == "risc") {
        config.isRISC        = true;
        config.aluLatency    = 1;
        config.decodeLatency = 1;
    } else {
        // CISC: heavier decode, slightly longer execute
        config.isRISC        = false;
        config.aluLatency    = 1;
        config.decodeLatency = 2;
    }

    // ── Pipeline ─────────────────────────────────────────────────────
    if (pipelineStr == "5_stage") {
        config.pipelineDepth         = 5;
        config.branchResolutionStage = 3;
    } else {
        config.pipelineDepth         = 1;
        config.branchResolutionStage = 1;
    }

    // ── Cache ────────────────────────────────────────────────────────
    config.cacheType = cacheStr;
    if (cacheStr == "SetAssociative") {
        config.cacheSizeKB      = 64;
        config.cacheLatency     = 2;
        config.ramLatency       = 20;
        config.memoryBusLatency = 1;
    } else {
        // DirectMapped
        config.cacheSizeKB      = 32;
        config.cacheLatency     = 2;
        config.ramLatency       = 20;
        config.memoryBusLatency = 1;
    }

    // ── I/O ──────────────────────────────────────────────────────────
    if (ioStr == "dma") {
        config.useDMA        = true;
        config.deviceLatency = 0;
    } else {
        config.useDMA        = false;
        config.deviceLatency = 15;
    }

    return config;
}

int main(int argc, char* argv[])
{
    // Args: workload isa pipeline cache io
    //       cache_size cache_latency ram_latency mem_bus_latency
    //       alu_latency decode_latency device_latency
    if (argc < 13) {
        std::cout << "Usage: program workload instruction_set pipeline cache io"
                  << " cacheSize cacheLatency ramLatency memBusLatency"
                  << " aluLatency decodeLatency deviceLatency\n";
        return 1;
    }

    std::string workloadStr       = argv[1];
    std::string instructionSetStr = argv[2];
    std::string pipelineStr       = argv[3];
    std::string cacheStr          = argv[4];
    std::string ioStr             = argv[5];

    // Build base config from high-level choices
    ArchitectureConfig config = buildConfig(instructionSetStr, pipelineStr, cacheStr, ioStr);

    // Override all hardware parameters from CLI arguments
    config.cacheSizeKB      = std::stoi(argv[6]);
    config.cacheLatency     = std::stoi(argv[7]);
    config.ramLatency       = std::stoi(argv[8]);
    config.memoryBusLatency = std::stoi(argv[9]);
    config.aluLatency       = std::stoi(argv[10]);
    config.decodeLatency    = std::stoi(argv[11]);
    config.deviceLatency    = std::stoi(argv[12]);

    // Build workload
    Workload w;
    if (workloadStr == "fibonacci")
        w = Workload::create(WorkloadType::Fibonacci);
    else if (workloadStr == "array_sum")
        w = Workload::create(WorkloadType::ArraySum);
    else if (workloadStr == "bubble_sort")
        w = Workload::createBubbleSort();
    else if (workloadStr == "random_memory_access")
        w = Workload::createRandomMemory();
    else if (workloadStr == "io_processing")
        w = Workload::createIOProcessing();
    else {
        std::cout << "Invalid workload\n";
        return 1;
    }

    PipelineSimulator sim;
    SimulationResult r = sim.run(w, config);

    std::cout << "===== Simulation Results =====\n";
    std::cout << "Instructions Executed: " << r.instructionsExecuted << "\n";
    std::cout << "Total Cycles: "          << r.totalCycles           << "\n";
    std::cout << "CPI: "                   << r.cpi                   << "\n";
    std::cout << "Pipeline Stalls: "       << r.pipelineStalls        << "\n";
    std::cout << "Cache Hits: "            << r.cacheHits             << "\n";
    std::cout << "Cache Misses: "          << r.cacheMisses           << "\n";
    std::cout << "CPU Idle Cycles: "       << r.cpuIdleCycles         << "\n";
    std::cout << "Memory Stall Cycles: "   << r.memoryStallCycles     << "\n";
    std::cout << "Branch Stall Cycles: "   << r.branchStallCycles     << "\n";
    std::cout << "IO Stall Cycles: "       << r.ioStallCycles         << "\n";
    std::cout << "Execution Cycles: "      << r.executionCycles       << "\n";

    return 0;
}