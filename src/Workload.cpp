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
        // Simulate loop with many branches: condition check -> branch -> ALU -> branch back
        const int loopIterations = 80;  // number of "fib" steps
        int id = 0;
        for (int i = 0; i < loopIterations; ++i) {
            instr.push_back({ InstructionType::ALU, id++ });    // compute
            instr.push_back({ InstructionType::BRANCH, id++ }); // loop condition
            instr.push_back({ InstructionType::ALU, id++ });
            instr.push_back({ InstructionType::BRANCH, id++ }); // branch back
        }
        instr.push_back({ InstructionType::IO, id++ });
        break;
    }
    case WorkloadType::MatrixMultiply: {
        name = "Matrix Multiply (memory heavy)";
        // NxN style: lots of LOAD, ALU, STORE with less predictable addresses
        const int N = 12;  // small "matrix" dimension
        int id = 0;
        for (int i = 0; i < N; ++i) {
            for (int j = 0; j < N; ++j) {
                for (int k = 0; k < N; ++k) {
                    instr.push_back({ InstructionType::LOAD, id++ });
                    instr.push_back({ InstructionType::LOAD, id++ });
                    instr.push_back({ InstructionType::ALU, id++ });
                    instr.push_back({ InstructionType::LOAD, id++ });
                    instr.push_back({ InstructionType::ALU, id++ });
                    instr.push_back({ InstructionType::STORE, id++ });
                }
            }
        }
        instr.push_back({ InstructionType::IO, id++ });
        break;
    }
    case WorkloadType::ArraySum: {
        name = "Array Sum (good cache locality)";
        // Sequential access: many ALU, sequential LOAD, one STORE at end
        const int arraySize = 200;
        int id = 0;
        for (int i = 0; i < arraySize; ++i) {
            instr.push_back({ InstructionType::LOAD, id++ });
            instr.push_back({ InstructionType::ALU, id++ });
        }
        instr.push_back({ InstructionType::STORE, id++ });
        instr.push_back({ InstructionType::IO, id++ });   // e.g. output result
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
