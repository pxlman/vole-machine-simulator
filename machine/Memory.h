#ifndef VOLE_MACHINE_MEMORY_H
#define VOLE_MACHINE_MEMORY_H

#include <vector>
#include <string>
#include <fstream>
#include <deque>

using namespace std;

class Memory {
private:
    vector<string> mem;
    int lastInserted;
public:
    Memory() {
        vector<string> m(256, "00");
        lastInserted = 1; // Memory 0 is let for run, and we mustn't mess with it
        mem = m;
    }
    void setCell(int address, string val) {
        mem[address] = val;
    }
    void insertCell(string val) {
        if(!validHexa(val)){
            throw "Invalid instruction";
        } else if (lastInserted >= 255)throw "Memory is full";
        else {
            mem[++lastInserted] = val;
        }
    }
    bool validHexa(string hex){
        string hexa = "0123456789ABCDEF";
        for(size_t i = 0; i < hex.length(); i++){
            for (int j = 0; j < 16; j++) {
                if(hex[i] == hexa[j]) return true;
            }
        }
        return false;
    }
    void insertFile(string &path) {
        ifstream iFile(path);
//        vector<string> instructions;
        if(!iFile.is_open()) throw "Invalid file";
        string inst;
        deque<string> instructions;
        while(iFile >> inst){
            if(!validInstructin(inst)){
                throw "Invalid instruction";
                return;
            } else {
                instructions.push_back(inst.substr(0,2));
                instructions.push_back(inst.substr(2,2));
            }
        }
        while(instructions.size()){
            insertCell(instructions[0]);
            instructions.pop_front();
        }
    }
    bool validInstructin(string inst){
        if(inst.length() != 4) return false;
        if((inst[0] > '6' || inst[0] < '0') && (inst[0] < 'B' || inst[0] > 'C')) return false;
        for(int i = 1; i < 4; i++){
            if((inst[0] > '9' || inst[0] < '0') && (inst[0] < 'A' || inst[0] > 'F')) return false;
        }
        return true;
    }
    string getCell(int address) {
        return mem[address];
    }
    vector<string> getMem(){
        return mem;
    }
    void clear() {
        for (int i = 0; i < 256; i++) {
            this->setCell(i, "00");
            this->lastInserted = 1;
        }
    }
};

class PC {
private:
    int counter;
public:
    PC() {
        counter = 2;
    }

    void set(int c) {
        counter = c;
    }

    void increaseBy(int i) {
        counter += i;
    }

    int get() {
        return counter;
    }

    int getThenIncreaseBy(int i){
        counter += i;
        return counter - i;
    }
};


#endif //VOLE_MACHINE_MEMORY_H
