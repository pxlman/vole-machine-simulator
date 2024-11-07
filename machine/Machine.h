#ifndef VOLE_H
#define VOLE_H
// #pragma once

#include<iostream>
#include<vector>
#include<map>
#include "Memory.h"
#include "CPU.h"

using namespace std;

/*
    1	RXY	Copy the content of RAM cell XY to register R.
    2	RXY	Copy the bit-string XY to register R.
    3	RXY	Copy the content of register R to RAM cell XY.
    4	xRS	Copy the content of register R to register S.
    5	RST	Add the content of register S and register T, and
                put the result in register R.
                Data is interpreted as integers in two's-complement notation.
    6	RST	Add the content of register S and register T, and put
                the result in register R.
                Data is interpreted as floats in floating point notation.
    B	RXY	Jump to instruction in RAM cell XY if the content of
                register R equals the content of register 0.
    C	xxx	Halt execution.
 */

class Machine {
private:
    Memory memory;
    Memory old_memory;
    vector<Memory> past_memory;
    vector<CPU> past_cpu;
    CPU cpu;
public:
    Machine() {
        memory = Memory();
        old_memory = memory;
        cpu = CPU(&memory);
    }
    bool isHalted(){
        return cpu.isHalted();
    }
    void restart(){
        memory = old_memory;
        cpu.clearCPU();
    }
    void addInstruction() {
        cout << "Please input the instruction: ";
        string inst; cin >> inst;
        memory.insertCell(inst.substr(0,2));
        memory.insertCell(inst.substr(2,2));
    }
    void addInstruction(string inst) {
        if(memory.validInstructin(inst)){
            memory.insertCell(inst.substr(0,2));
            memory.insertCell(inst.substr(2,2));
            old_memory = memory;
        } else {
            throw "Invalid instruction";
        }
    }
    void addFile(string &path){
        memory.insertFile(path);
        old_memory = memory;
    }
    pair<Memory, CPU> getState(){
        pair<Memory, CPU> var = {memory,cpu};
        return var;
    }
    vector<string> getRegisterVector(){
        return cpu.getRegVec();
    }
    vector<string> getMemoryVector(){
        return memory.getMem();
    }
    int getPC(){
        return cpu.getPC();
    }
    string getIR(){
        cpu.fetch();
        return cpu.getIR();
    }
    string getGuide(int i){
        return cpu.guidance(cpu.decodeMemAddress(i));
    }
    void clearMemory(){
        memory.clear();
    }
    void clearAll(){
        memory.clear();
        cpu.clearCPU();
    }
    void cycle(){
        if(isHalted()) return;
        past_memory.push_back(memory);
        past_cpu.push_back(cpu);
        cpu.fetch();
        vector<int> instructionVector = cpu.decode();
        cpu.execute(instructionVector);
    }
    void undo(){
        memory = past_memory.at(past_memory.size()-1);
        past_memory.pop_back();
        cpu = past_cpu.at(past_cpu.size()-1);
        past_cpu.pop_back();
    }
    void halt(){
        cpu.setHalt();
    }
};

#endif //VOLE_H
