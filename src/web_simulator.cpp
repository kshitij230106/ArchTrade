/**
 * ArchTrade - Educational Computer Architecture Simulator
 * Main CLI simulator entrypoint.
 */

#include "ArchitectureConfig.h"
#include "PipelineSimulator.h"
#include "SimulationResult.h"
#include "Workload.h"

#include <iostream>
#include <string>

/**
 * Build architecture configuration from CLI options.
 */
static ArchitectureConfig buildConfig(
    const std::string& instructionSetStr,
    const std::string& pipelineStr,
    const std::string& cacheStr,
    const std::string& ioStr)
{
    ArchitectureConfig config;

    // =====================================================
    // Instruction Set
    // =====================================================

    if (instructionSetStr == "risc") {

        config.isa = ISAType::RISC;

        config.aluLatency = 1;
        config.decodeLatency = 1;
    }
    else {

        config.isa = ISAType::CISC;

        config.aluLatency = 1;
        config.decodeLatency = 2;
    }

    // =====================================================
    // Pipeline
    // =====================================================

    if (pipelineStr == "5_stage") {

        config.pipelineDepth = 5;
        config.branchResolutionStage = 3;
    }
    else {

        config.pipelineDepth = 1;
        config.branchResolutionStage = 1;
    }

    // =====================================================
    // Cache
    // =====================================================

    if (cacheStr == "SetAssociative") {

        config.cacheType =
            CacheType::SetAssociative;

        config.cacheSizeKB = 64;
    }
    else {

        config.cacheType =
            CacheType::DirectMapped;

        config.cacheSizeKB = 32;
    }

    config.cacheLatency = 2;
    config.ramLatency = 20;
    config.memoryBusLatency = 1;

    // =====================================================
    // I/O
    // =====================================================

    if (ioStr == "dma") {

        config.ioType = IOType::DMA;

        config.deviceLatency = 0;
    }
    else {

        config.ioType = IOType::Polling;

        config.deviceLatency = 15;
    }

    return config;
}

int main(int argc, char* argv[])
{
    // =====================================================
    // CLI Arguments
    // =====================================================

    // workload
    // isa
    // pipeline
    // cache
    // io
    // cacheSize
    // cacheLatency
    // ramLatency
    // memBusLatency
    // aluLatency
    // decodeLatency
    // deviceLatency

    if (argc < 13) {

        std::cout
            << "{ \"error\": \"Invalid arguments\" }\n";

        return 1;
    }

    std::string workloadStr       = argv[1];
    std::string instructionSetStr = argv[2];
    std::string pipelineStr       = argv[3];
    std::string cacheStr          = argv[4];
    std::string ioStr             = argv[5];

    // =====================================================
    // Build architecture config
    // =====================================================

    ArchitectureConfig config =
        buildConfig(
            instructionSetStr,
            pipelineStr,
            cacheStr,
            ioStr
        );

    // =====================================================
    // Override hardware parameters
    // =====================================================

    config.cacheSizeKB =
        std::stoi(argv[6]);

    config.cacheLatency =
        std::stoi(argv[7]);

    config.ramLatency =
        std::stoi(argv[8]);

    config.memoryBusLatency =
        std::stoi(argv[9]);

    config.aluLatency =
        std::stoi(argv[10]);

    config.decodeLatency =
        std::stoi(argv[11]);

    config.deviceLatency =
        std::stoi(argv[12]);

    // =====================================================
    // Build workload
    // =====================================================

    Workload workload;

    if (workloadStr == "fibonacci") {

        workload =
            Workload::create(
                WorkloadType::Fibonacci
            );
    }
    else if (workloadStr == "array_sum") {

        workload =
            Workload::create(
                WorkloadType::ArraySum
            );
    }
    else if (workloadStr == "bubble_sort") {

        workload =
            Workload::createBubbleSort();
    }
    else if (workloadStr == "random_memory_access") {

        workload =
            Workload::createRandomMemory();
    }
    else if (workloadStr == "io_processing") {

        workload =
            Workload::createIOProcessing();
    }
    else {

        std::cout
            << "{ \"error\": \"Invalid workload\" }\n";

        return 1;
    }

    // =====================================================
    // Run simulation
    // =====================================================

    PipelineSimulator simulator;

    SimulationResult result =
        simulator.run(
            workload,
            config
        );

    // =====================================================
    // JSON Output
    // =====================================================

    std::cout << "{\n";

    std::cout
        << "  \"instructionsExecuted\": "
        << result.instructionsExecuted
        << ",\n";

    std::cout
        << "  \"totalCycles\": "
        << result.totalCycles
        << ",\n";

    std::cout
        << "  \"cpi\": "
        << result.cpi
        << ",\n";

    std::cout
        << "  \"pipelineStalls\": "
        << result.pipelineStalls
        << ",\n";

    std::cout
        << "  \"cacheHits\": "
        << result.cacheHits
        << ",\n";

    std::cout
        << "  \"cacheMisses\": "
        << result.cacheMisses
        << ",\n";

    std::cout
        << "  \"cpuIdleCycles\": "
        << result.cpuIdleCycles
        << ",\n";

    std::cout
        << "  \"memoryStallCycles\": "
        << result.memoryStallCycles
        << ",\n";

    std::cout
        << "  \"branchStallCycles\": "
        << result.branchStallCycles
        << ",\n";

    std::cout
        << "  \"ioStallCycles\": "
        << result.ioStallCycles
        << ",\n";

    std::cout
        << "  \"executionCycles\": "
        << result.executionCycles
        << "\n";

    std::cout << "}\n";

    return 0;
}