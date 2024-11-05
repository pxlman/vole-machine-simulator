#ifndef VOLE_MACHINE_REGISTER_H
#define VOLE_MACHINE_REGISTER_H

#include <vector>
#include <string>

using namespace std;

class Register {
private:
    vector<string> reg;
public:
    Register() {
        vector<string> r(16, "00");
        reg = r;
    }

    void setReg(int address, string val) {
        reg[address] = val;
    }

    string getReg(int address) {
        return reg[address];
    }
    vector<string> getVec(){
        return reg;
    }
    void clear() {
        for (int i = 0; i < 16; i++) {
            this->setReg(i, "00");
        }
    }
};

#endif //VOLE_MACHINE_REGISTER_H
