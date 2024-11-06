#ifndef VOLE_MACHINE_CPU_H
#define VOLE_MACHINE_CPU_H

#include <vector>
#include <string>
#include <format>
#include <iostream>
#include "Memory.h"
#include "Register.h"
#include "ALU.h"
#include "CU.h"

using namespace std;

class CPU {
private:
    PC program_counter;
    string instructionRegister;
    Register reg;
    ALU alu;
    CU cu;
    Memory *memory;
    bool halted;
public:
    CPU(){
        program_counter = PC();
        halted = false;
        reg = Register();
        alu = ALU();
        cu = CU();
        instructionRegister = "0000";
    }
    CPU(Memory *m) {
        program_counter = PC();
        memory = m;
        reg = Register();
        alu = ALU();
        cu = CU();
        instructionRegister = memory->getCell(2) + memory->getCell(3);
    }
    void clearCPU(){
        program_counter = PC();
        reg = Register();
        alu = ALU();
        cu = CU();
        halted = false;
    }
    void clearAll(){
        program_counter = PC();
        memory->clear();
        reg = Register();
        alu = ALU();
        cu = CU();
        halted = false;
    }
    bool isHalted(){
        return halted;
    }
    void setHalt(){
        halted = true;
    }
    int getPC(){
        return program_counter.get();
    }
    string getIR(){
        return instructionRegister;
    }
    Register getReg(){
        return reg;
    }
    vector<string> getRegVec(){
        return reg.getVec();
    }
    void fetch() {
        instructionRegister = memory->getCell(program_counter.get());
        instructionRegister += memory->getCell(program_counter.get()+1);
    }

    vector<int> decode() {
        vector<int> decoded;
        string R,S,T,XY,X;
        switch (instructionRegister[0]) {
            case '0': // ERROR
                decoded.push_back(instructionRegister[0]);
                break;
            case '1': // R, XY
                decoded.push_back(instructionRegister[0]);
                R = instructionRegister[1];
                decoded.push_back(alu.hextodec(R));
                decoded.push_back(alu.hextodec(instructionRegister.substr(2,2)));
                break;
            case '2': // R, XY
                decoded.push_back(instructionRegister[0]);
                R = instructionRegister[1];
                decoded.push_back(alu.hextodec(R));
                decoded.push_back(alu.hextodec(instructionRegister.substr(2,2)));
                break;
            case '3': // R, XY
                R = instructionRegister[1];
                decoded.push_back(instructionRegister[0]);
                decoded.push_back(alu.hextodec(R));
                decoded.push_back(alu.hextodec(instructionRegister.substr(2,2)));
                break;
            case '4': // R, S
                R = instructionRegister[1];
                S = instructionRegister[2];
                decoded.push_back(instructionRegister[0]);
                decoded.push_back(alu.hextodec(R));
                decoded.push_back(alu.hextodec(S));
                break;
            case '5': // R, S, T
                R = instructionRegister[1];
                S = instructionRegister[2];
                T = instructionRegister[3];
                decoded.push_back(instructionRegister[0]);
                decoded.push_back(alu.hextodec(S));
                decoded.push_back(alu.hextodec(T));
                decoded.push_back(alu.hextodec(R));
                break;
            case '6': // R, S, T
                R = instructionRegister[1];
                S = instructionRegister[2];
                T = instructionRegister[3];
                decoded.push_back(instructionRegister[0]);
                decoded.push_back(alu.hextodec(S));
                decoded.push_back(alu.hextodec(T));
                decoded.push_back(alu.hextodec(R));
                break;
            case '7': // R, S, T
                R = instructionRegister[1];
                S = instructionRegister[2];
                T = instructionRegister[3];
                decoded.push_back(instructionRegister[0]);
                decoded.push_back(alu.hextodec(S));
                decoded.push_back(alu.hextodec(T));
                decoded.push_back(alu.hextodec(R));
                break;
            case '8': // R, S, T
                R = instructionRegister[1];
                S = instructionRegister[2];
                T = instructionRegister[3];
                decoded.push_back(instructionRegister[0]);
                decoded.push_back(alu.hextodec(S));
                decoded.push_back(alu.hextodec(T));
                decoded.push_back(alu.hextodec(R));
                break;
            case '9': // R, S, T
                R = instructionRegister[1];
                S = instructionRegister[2];
                T = instructionRegister[3];
                decoded.push_back(instructionRegister[0]);
                decoded.push_back(alu.hextodec(S));
                decoded.push_back(alu.hextodec(T));
                decoded.push_back(alu.hextodec(R));
                break;
            case 'A': // RxX
                R = instructionRegister[1];
                X = instructionRegister[3];
                decoded.push_back(instructionRegister[0]);
                decoded.push_back(alu.hextodec(R));
                decoded.push_back(alu.hextodec(X));
                break;
            case 'B': // R, XY
                R = instructionRegister[1];
                decoded.push_back(instructionRegister[0]);
                decoded.push_back(alu.hextodec(R));
                decoded.push_back(alu.hextodec(instructionRegister.substr(2,2)));
                break;
            case 'C': // Halt
                decoded.push_back(instructionRegister[0]);
                break;
            case 'D': // R, XY
                R = instructionRegister[1];
                decoded.push_back(instructionRegister[0]);
                decoded.push_back(alu.hextodec(R));
                decoded.push_back(alu.hextodec(instructionRegister.substr(2,2)));
                break;
        }
        return decoded;
    }
    void execute(vector<int> instruction) {
        // auto instruction = decode();
        if(program_counter.get() >= 254) halted = true;
        if (halted) return;
        switch (instruction[0]) {
            case '0': // nothing
                program_counter.increaseBy(2);
                break;
            case '1': // R, XY
                cu.load(instruction[1],instruction[2], reg, *memory);
                program_counter.increaseBy(2);
                break;
            case '2': // R, XY
                cu.load(instruction[1],instruction[2], reg);
                program_counter.increaseBy(2);
                break;
            case '3': // R, XY
                cu.store(instruction[1],instruction[2],reg, *memory);
                program_counter.increaseBy(2);
                break;
            case '4': // R, S
                cu.move(instruction[1], instruction[2], reg);
                program_counter.increaseBy(2);
                break;
            case '5': // R, S, T
                // Add the content of R to S and put the result in T using two comp
                alu.addTwosC(instruction[1],instruction[2], instruction[3],reg);
                program_counter.increaseBy(2);
                break;
            case '6': // R, S, T
                // Add the content of R to S and put the result in T using floating point
                alu.addFloat(instruction[1],instruction[2], instruction[3],reg);
                program_counter.increaseBy(2);
                break;
            case '7': // R, S, T
                // Add the content of R to S and put the result in T using floating point
                alu.ORing(instruction[1],instruction[2], instruction[3],reg);
                program_counter.increaseBy(2);
                break;
            case '8': // R, S, T
                // Add the content of R to S and put the result in T using floating point
                alu.Anding(instruction[1],instruction[2], instruction[3],reg);
                program_counter.increaseBy(2);
                break;
            case '9': // R, S, T
                // Add the content of R to S and put the result in T using floating point
                alu.Xoring(instruction[1],instruction[2], instruction[3],reg);
                program_counter.increaseBy(2);
                break;
            case 'A': // RxX
                // Add the content of R to S and put the result in T using floating point
                alu.reverse(instruction[1],instruction[2], reg);
                program_counter.increaseBy(2);
                break;
            case 'B': // R, XY
                cu.jump(instruction[1],instruction[2],reg, &program_counter);
                break;
            case 'C':
                halted = true;
                break;
            case 'D': // R, XY
                cu.jumpLessThan(instruction[1],instruction[2],reg, &program_counter);
                break;
        }
    }
    string guidance(vector<int> decoded){
        vector<string> arr;
        arr.push_back(""+(char)decoded[0]);
        if(decoded.size() > 1){
            for (long unsigned var = 1; var < decoded.size(); ++var) {
                arr.push_back(alu.decToHex(decoded[var]));
            }
        }
        string guide;
        switch (decoded[0]) {
        case '0': // nothing
            guide = "Ignore it." ;
            break;
        case '1': // R, XY
            guide = "Copy the content of RAM cell " + arr[2] + " to register " + arr[1] + '.';
            break;
        case '2': // R, XY
            guide = "Copy the bit-string " + arr[2] + " to register " + arr[1] + '.';
            break;
        case '3': // R, XY
            guide = "Copy the content of register " + arr[1] + " to RAM cell " + arr[2] + '.';
            break;
        case '4': // R, S
            guide = "Copy the content of register " + arr[1] + " to register " + arr[2] + ".";
            break;
        case '5': // R, S, T
            // Add the content of R to S and put the result in T using two comp
            guide = "Add the content of register " + arr[2] + " and register " + arr[3] + " then put the result at register " + arr[1] + " as integers in two's complement notation.";
            break;
        case '6': // R, S, T
            // Add the content of R to S and put the result in T using floating point
            guide = "Add the content of register " + arr[2] + " and register " + arr[3] + " then put the result at register " + arr[1] + " as floats.";
            break;
        case '7': // R, S, T
            guide = "Bitwise OR (∨) the content of register " + arr[2] + " and register " + arr[3] + " then put the result at register " + arr[1] + " as floats.";
            break;
        case '8': // R, S, T
            guide = "Bitwise AND (∧) the content of register " + arr[2] + " and register " + arr[3] + " then put the result at register " + arr[1] + " as floats.";
            break;
        case '9': // R, S, T
            guide = "Bitwise XOR (⊕) the content of register " + arr[2] + " and register " + arr[3] + " then put the result at register " + arr[1] + " as floats.";
            break;
        case 'A': // RxX
            guide = "Rotate the content of register " + arr[1] + " cyclically right " + to_string(alu.hextodec(arr[2])) + " steps.";
            break;
        case 'B': // R, XY
            guide = "Jump to the instruction at RAM cell " + arr[2] + " if the content at register " + arr[1] + " equals to the content of register 0.";
            break;
        case 'C':
            guide = "Halt.";
            break;
        case 'D': // R, XY
            guide = "Jump to the instruction at RAM cell " + arr[2] + " if the content at register " + arr[1] + " greater than the content of register 0 where numbers are represented in floating point representation.";
            break;
        }
        return guide;
    }
    vector<int> decodeMemAddress(int i) {
        vector<int> decoded;
        string R,S,T,XY,X;
        string instruction = memory->getCell(i) + memory->getCell(i+1) ;
        switch (instruction[0]) {
            case '0': // ERROR
                decoded.push_back(instruction[0]);
                break;
            case '1': // R, XY
                decoded.push_back(instruction[0]);
                R = instruction[1];
                decoded.push_back(alu.hextodec(R));
                decoded.push_back(alu.hextodec(instruction.substr(2,2)));
                break;
            case '2': // R, XY
                decoded.push_back(instruction[0]);
                R = instruction[1];
                decoded.push_back(alu.hextodec(R));
                decoded.push_back(alu.hextodec(instruction.substr(2,2)));
                break;
            case '3': // R, XY
                R = instruction[1];
                decoded.push_back(instruction[0]);
                decoded.push_back(alu.hextodec(R));
                decoded.push_back(alu.hextodec(instruction.substr(2,2)));
                break;
            case '4': // R, S
                R = instruction[1];
                S = instruction[2];
                decoded.push_back(instruction[0]);
                decoded.push_back(alu.hextodec(R));
                decoded.push_back(alu.hextodec(S));
                break;
            case '5': // R, S, T
                R = instruction[1];
                S = instruction[2];
                T = instruction[3];
                decoded.push_back(instruction[0]);
                decoded.push_back(alu.hextodec(S));
                decoded.push_back(alu.hextodec(T));
                decoded.push_back(alu.hextodec(R));
                break;
            case '6': // R, S, T
                R = instruction[1];
                S = instruction[2];
                T = instruction[3];
                decoded.push_back(instruction[0]);
                decoded.push_back(alu.hextodec(S));
                decoded.push_back(alu.hextodec(T));
                decoded.push_back(alu.hextodec(R));
                break;
            case '7': // R, S, T
                R = instruction[1];
                S = instruction[2];
                T = instruction[3];
                decoded.push_back(instruction[0]);
                decoded.push_back(alu.hextodec(S));
                decoded.push_back(alu.hextodec(T));
                decoded.push_back(alu.hextodec(R));
                break;
            case '8': // R, S, T
                R = instruction[1];
                S = instruction[2];
                T = instruction[3];
                decoded.push_back(instruction[0]);
                decoded.push_back(alu.hextodec(S));
                decoded.push_back(alu.hextodec(T));
                decoded.push_back(alu.hextodec(R));
                break;
            case '9': // R, S, T
                R = instruction[1];
                S = instruction[2];
                T = instruction[3];
                decoded.push_back(instruction[0]);
                decoded.push_back(alu.hextodec(S));
                decoded.push_back(alu.hextodec(T));
                decoded.push_back(alu.hextodec(R));
                break;
            case 'A': // RxX
                R = instruction[1];
                X = instruction[3];
                decoded.push_back(instruction[0]);
                decoded.push_back(alu.hextodec(R));
                decoded.push_back(alu.hextodec(X));
                break;
            case 'B': // R, XY
                R = instruction[1];
                decoded.push_back(instruction[0]);
                decoded.push_back(alu.hextodec(R));
                decoded.push_back(alu.hextodec(instruction.substr(2,2)));
                break;
            case 'C': // Halt
                decoded.push_back(instruction[0]);
                break;
            case 'D': // R, XY
                R = instruction[1];
                decoded.push_back(instruction[0]);
                decoded.push_back(alu.hextodec(R));
                decoded.push_back(alu.hextodec(instruction.substr(2,2)));
                break;
        }
        return decoded;
    }
};

#endif //VOLE_MACHINE_CPU_H
