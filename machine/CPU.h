#ifndef VOLE_MACHINE_CPU_H
#define VOLE_MACHINE_CPU_H

#include <vector>
#include <string>
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
    }
    void clearCPU(){
        program_counter = PC();
        reg = Register();
        alu = ALU();
        cu = CU();
    }
    void clearAll(){
        program_counter = PC();
        memory->clear();
        reg = Register();
        alu = ALU();
        cu = CU();
    }
    bool isHalted(){
        return halted;
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
        string R,S,T,XY;
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
            case 'B': // R, XY
                R = instructionRegister[1];
                decoded.push_back(instructionRegister[0]);
                decoded.push_back(alu.hextodec(R));
                decoded.push_back(alu.hextodec(instructionRegister.substr(2,2)));
                break;
            case 'C': // Halt
                decoded.push_back(instructionRegister[0]);
                break;
        }
        return decoded;
    }

    void execute(vector<int>) {
        auto instruction = decode();
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
            case 'B': // R, XY
                if(reg.getReg(instruction[1]) == reg.getReg(0)){
                    program_counter.set(instruction[2]);
                } else {
                    program_counter.increaseBy(2);
                }
                break;
            case 'C':
                halted = true;
                break;
        }
    }
};

#endif //VOLE_MACHINE_CPU_H
