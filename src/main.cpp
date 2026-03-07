/**
 * ArchTrade - Educational Computer Architecture Simulator
 * Main entry: menu, workload and architecture selection, run simulation, print results.
 */

#include "ArchitectureConfig.h"
#include "PipelineSimulator.h"
#include "SimulationResult.h"
#include "Workload.h"
#include <iomanip>
#include <iostream>
#include <limits>
#include <string>

static void printMenu() {
    std::cout << "\n========== ArchTrade - Computer Architecture Simulator ==========\n\n";
    std::cout << "  WORKLOAD\n";
    std::cout << "    1. Fibonacci (branch heavy)\n";
    std::cout << "    2. Matrix Multiply (memory heavy)\n";
    std::cout << "    3. Array Sum (good cache locality)\n";
    std::cout << "    4. Bubble Sort (branch + memory heavy)\n";
    std::cout << "    5. Random Memory Access (poor cache locality)\n";
    std::cout << "    6. I/O Processing (I/O heavy)\n";
    std::cout << "\n  ARCHITECTURE CONFIGURATION\n";
    std::cout << "    Instruction Set:  7. RISC  8. CISC\n";
    std::cout << "    Pipeline:         9. Single Cycle  10. 5-Stage Pipeline\n";
    std::cout << "    Cache:           11. Direct Mapped  12. Set Associative\n";
    std::cout << "    I/O:             13. Polling  14. DMA\n";
    std::cout << "\n   15. Run simulation\n";
    std::cout << "   16. Compare two architecture configurations\n";
    std::cout << "    0. Exit   99. Show menu again\n";
    std::cout << "\n================================================================\n";
}

static void printResult(const SimulationResult& r, const std::string& workloadName) {
    std::cout << "\n";
    std::cout << "  Workload: " << workloadName << "\n";
    std::cout << "\n  ---------- Simulation Results ----------\n\n";
    std::cout << "  Instructions Executed : " << r.instructionsExecuted << "\n";
    std::cout << "  Total Cycles          : " << r.totalCycles << "\n";
    std::cout.precision(2);
    std::cout << std::fixed;
    std::cout << "  CPI                   : " << r.cpi << "\n";
    std::cout << "\n  Pipeline Stalls       : " << r.pipelineStalls << "\n";
    std::cout << "  Cache Hits            : " << r.cacheHits << "\n";
    std::cout << "  Cache Misses          : " << r.cacheMisses << "\n";
    std::cout << "  CPU Idle Cycles       : " << r.cpuIdleCycles << "\n";
    std::cout << "\n  ----------------------------------------\n\n";
}

/** Returns the workload for menu option 1-6. */
static Workload getWorkloadForOption(int option) {
    if (option <= 3)
        return Workload::create(static_cast<WorkloadType>(option - 1));
    if (option == 4) return Workload::createBubbleSort();
    if (option == 5) return Workload::createRandomMemory();
    return Workload::createIOProcessing();
}

/** Prompt for one architecture configuration (RISC/CISC, pipeline, cache, I/O). */
static ArchitectureConfig promptForConfig(const char* configLabel) {
    ArchitectureConfig c;
    int val;
    std::cout << "\n  --- " << configLabel << " ---\n";
    std::cout << "    RISC (7) or CISC (8)? "; std::cin >> val; c.isRISC = (val == 7);
    std::cout << "    Single Cycle (9) or 5-Stage (10)? "; std::cin >> val; c.pipelineDepth = (val == 10) ? 5 : 1;
    std::cout << "    Direct Mapped (11) or Set Associative (12)? "; std::cin >> val; c.cacheType = (val == 12) ? "SetAssociative" : "DirectMapped";
    std::cout << "    Polling (13) or DMA (14)? "; std::cin >> val; c.useDMA = (val == 14);
    return c;
}

static void printComparisonResults(const SimulationResult& a, const SimulationResult& b) {
    std::cout << "\n  ---------- Comparison Results ----------\n\n";
    std::cout << "  " << std::left << std::setw(24) << "Metric" << std::right << std::setw(14) << "Config A" << std::setw(14) << "Config B" << "\n\n";
    std::cout << "  " << std::left << std::setw(24) << "Instructions" << std::right << std::setw(14) << a.instructionsExecuted << std::setw(14) << b.instructionsExecuted << "\n";
    std::cout << "  " << std::left << std::setw(24) << "Total Cycles" << std::right << std::setw(14) << a.totalCycles << std::setw(14) << b.totalCycles << "\n";
    std::cout.precision(2);
    std::cout << std::fixed;
    std::cout << "  " << std::left << std::setw(24) << "CPI" << std::right << std::setw(14) << a.cpi << std::setw(14) << b.cpi << "\n";
    std::cout << "  " << std::left << std::setw(24) << "Pipeline Stalls" << std::right << std::setw(14) << a.pipelineStalls << std::setw(14) << b.pipelineStalls << "\n";
    std::cout << "  " << std::left << std::setw(24) << "Cache Hits" << std::right << std::setw(14) << a.cacheHits << std::setw(14) << b.cacheHits << "\n";
    std::cout << "  " << std::left << std::setw(24) << "Cache Misses" << std::right << std::setw(14) << a.cacheMisses << std::setw(14) << b.cacheMisses << "\n";
    std::cout << "  " << std::left << std::setw(24) << "CPU Idle Cycles" << std::right << std::setw(14) << a.cpuIdleCycles << std::setw(14) << b.cpuIdleCycles << "\n";
    std::cout << "\n  ----------------------------------------\n\n";
}

int main() {
    int selectedWorkloadOption = 1;  // 1-6: which workload is selected
    ArchitectureConfig config;

    printMenu();

    while (true) {
        std::cout << "\n  Enter another choice (0 to exit, 99 to show menu again): ";
        int choice = -1;
        if (!(std::cin >> choice)) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "  Invalid input.\n";
            continue;
        }

        if (choice == 99) {
            printMenu();
            continue;
        }

        switch (choice) {
        case 0:
            std::cout << "  Goodbye.\n";
            return 0;
        case 1:
            selectedWorkloadOption = 1;
            std::cout << "  Workload set to: Fibonacci (branch heavy).\n";
            break;
        case 2:
            selectedWorkloadOption = 2;
            std::cout << "  Workload set to: Matrix Multiply (memory heavy).\n";
            break;
        case 3:
            selectedWorkloadOption = 3;
            std::cout << "  Workload set to: Array Sum (good cache locality).\n";
            break;
        case 4:
            selectedWorkloadOption = 4;
            std::cout << "  Workload set to: Bubble Sort (branch + memory heavy).\n";
            break;
        case 5:
            selectedWorkloadOption = 5;
            std::cout << "  Workload set to: Random Memory Access (poor cache locality).\n";
            break;
        case 6:
            selectedWorkloadOption = 6;
            std::cout << "  Workload set to: I/O Processing (I/O heavy).\n";
            break;
        case 7:
            config.isRISC = true;
            std::cout << "  Instruction set: RISC.\n";
            break;
        case 8:
            config.isRISC = false;
            std::cout << "  Instruction set: CISC.\n";
            break;
        case 9:
            config.pipelineDepth = 1;
            std::cout << "  Pipeline: Single Cycle.\n";
            break;
        case 10:
            config.pipelineDepth = 5;
            std::cout << "  Pipeline: 5-Stage Pipeline.\n";
            break;
        case 11:
            config.cacheType = "DirectMapped";
            std::cout << "  Cache: Direct Mapped.\n";
            break;
        case 12:
            config.cacheType = "SetAssociative";
            std::cout << "  Cache: Set Associative.\n";
            break;
        case 13:
            config.useDMA = false;
            std::cout << "  I/O: Polling.\n";
            break;
        case 14:
            config.useDMA = true;
            std::cout << "  I/O: DMA.\n";
            break;
        case 15: {
            Workload w = getWorkloadForOption(selectedWorkloadOption);
            PipelineSimulator sim;
            SimulationResult result = sim.run(w, config);
            printResult(result, w.name);
            break;
        }
        case 16: {
            std::cout << "  Select workload (1-6): ";
            int wopt = 1;
            std::cin >> wopt;
            if (wopt < 1 || wopt > 6) wopt = 1;
            Workload w = getWorkloadForOption(wopt);
            ArchitectureConfig configA = promptForConfig("Configuration A");
            ArchitectureConfig configB = promptForConfig("Configuration B");
            PipelineSimulator sim;
            SimulationResult resultA = sim.run(w, configA);
            SimulationResult resultB = sim.run(w, configB);
            std::cout << "\n  Workload: " << w.name << "\n";
            printComparisonResults(resultA, resultB);
            break;
        }
        default:
            std::cout << "  Unknown option. Try again.\n";
            break;
        }
    }
}
