#include "ArchitectureConfig.h"
#include "PipelineSimulator.h"
#include "SimulationResult.h"
#include "Workload.h"
#include <iostream>
#include <string>

int main(int argc, char* argv[])
{
    if (argc < 6) {
        std::cout << "Usage: program workload instruction_set pipeline cache io\n";
        return 1;
    }

    std::string workloadStr = argv[1];
    std::string instructionSetStr = argv[2];
    std::string pipelineStr = argv[3];
    std::string cacheStr = argv[4];
    std::string ioStr = argv[5];

    ArchitectureConfig config;

    // Instruction set
    if (instructionSetStr == "risc")
        config.isRISC = true;
    else
        config.isRISC = false;

    // Pipeline
    if (pipelineStr == "5_stage")
        config.pipelineDepth = 5;
    else
        config.pipelineDepth = 1;

    // Cache
    config.cacheType = cacheStr;

    // IO type
    if (ioStr == "dma")
        config.useDMA = true;
    else
        config.useDMA = false;

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
    std::cout << "Total Cycles: " << r.totalCycles << "\n";
    std::cout << "CPI: " << r.cpi << "\n";
    std::cout << "Cache Hits: " << r.cacheHits << "\n";
    std::cout << "Cache Misses: " << r.cacheMisses << "\n";

    return 0;
}