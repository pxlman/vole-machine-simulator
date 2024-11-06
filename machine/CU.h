#ifndef VOLE_MACHINE_CU_H
#define VOLE_MACHINE_CU_H

#include <vector>
#include <string>
#include "Register.h"
#include "Memory.h"
#include "machine/ALU.h"

using namespace std;

class CU {
public:
    CU() {

    }

    void load(int id_reg, int id_mem, Register &reg, Memory mem) {
        reg.setReg(id_reg, mem.getCell(id_mem));
    }

    void load(int id_reg, int val, Register &reg) {
        ALU alu = ALU();
        reg.setReg(id_reg, alu.decToHex(val));
    }

    void move(int id_from, int id_to, Register &reg) {
        reg.setReg(id_to, reg.getReg(id_from));
    }

    void store(int id_reg, int id_mem, Register reg, Memory &mem) {
        mem.setCell(id_mem, reg.getReg(id_reg));
    }

    void jump(int id_reg, int id_mem, Register reg, PC *pc) {
        if (reg.getReg(0) == reg.getReg(id_reg)) {
            pc->set(id_mem);
        } else pc->increaseBy(2);
    }
    void jumpLessThan(int id_reg, int id_mem, Register reg, PC *pc) {
        ALU alu;
        string reg0 = alu.FloatRep(alu.hextodec(reg.getReg(0)));
        string regi = alu.FloatRep(alu.hextodec(reg.getReg(id_reg)));
        if (stof(regi) > stof(reg0)) {
            pc->set(id_mem);
        } else pc->increaseBy(2);
    }

    void halt() {
        return;
    }
};

#endif //VOLE_MACHINE_CU_H
