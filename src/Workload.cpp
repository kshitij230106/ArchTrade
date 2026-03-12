/**
 * ArchTrade - Educational Computer Architecture Simulator
 * Workload implementation: generates instruction sequences per workload type.
 */

#include "Workload.h"
#include <cstdlib>
#include <ctime>

Workload Workload::create(WorkloadType type) {
    std::srand(static_cast<unsigned>(std::time(nullptr)));
    std::vector<Instruction> instr;
    std::string name;

    switch (type) {
    case WorkloadType::Fibonacci: {
        name = "Fibonacci (branch heavy)";
        const int loopIterations = 80;
        int id = 0;
        for (int i = 0; i < loopIterations; ++i) {
            instr.push_back({ InstructionType::ALU,    id++ });
            instr.push_back({ InstructionType::BRANCH, id++ });
            instr.push_back({ InstructionType::ALU,    id++ });
            instr.push_back({ InstructionType::BRANCH, id++ });
            // Occasional IO to demonstrate idle cycle behavior
            if (i % 20 == 19)
                instr.push_back({ InstructionType::IO, id++ });
        }
        instr.push_back({ InstructionType::IO, id++ });
        break;
    }
    case WorkloadType::ArraySum: {
        name = "Array Sum (good cache locality)";
        const int arraySize = 200;
        int id = 0;
        for (int i = 0; i < arraySize; ++i) {
            instr.push_back({ InstructionType::LOAD, id++ });
            instr.push_back({ InstructionType::ALU,  id++ });
        }
        instr.push_back({ InstructionType::STORE, id++ });
        instr.push_back({ InstructionType::IO,    id++ }); // output result
        break;
    }
    }

    return Workload(name, std::move(instr));
}

Workload Workload::createBubbleSort() {
    Workload w("Bubble Sort (branch + memory heavy)");
    for (int i = 0; i < 120; i++) {
        w.addInstruction(InstructionType::LOAD);
        w.addInstruction(InstructionType::LOAD);
        w.addInstruction(InstructionType::ALU);
        w.addInstruction(InstructionType::BRANCH);
        w.addInstruction(InstructionType::STORE);
        // Occasional IO: e.g. progress output every 30 iterations
        if (i % 30 == 29)
            w.addInstruction(InstructionType::IO);
    }
    return w;
}

Workload Workload::createRandomMemory() {
    Workload w("Random Memory Access (poor cache locality)");
    for (int i = 0; i < 150; i++) {
        w.addInstruction(InstructionType::LOAD);
        w.addInstruction(InstructionType::ALU);
        w.addInstruction(InstructionType::LOAD);
        w.addInstruction(InstructionType::ALU);
        w.addInstruction(InstructionType::LOAD);
        // Periodic IO to show DMA vs polling difference
        if (i % 25 == 24)
            w.addInstruction(InstructionType::IO);
    }
    return w;
}

Workload Workload::createIOProcessing() {
    Workload w("I/O Processing (I/O heavy)");
    for (int i = 0; i < 100; i++) {
        w.addInstruction(InstructionType::ALU);
        w.addInstruction(InstructionType::IO);
        w.addInstruction(InstructionType::IO);
        w.addInstruction(InstructionType::LOAD);
        w.addInstruction(InstructionType::IO);
    }
    return w;
}