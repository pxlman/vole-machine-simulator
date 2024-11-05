#ifndef MAINMACHINE_H
#define MAINMACHINE_H
// #include"machine/Machine.h"
#include"machine/Machine.h"

class MainMachine: public Machine {
private:
    MainMachine(){}
public:
    static MainMachine &machine(){
        static MainMachine machine;
        return machine;
    }
};

#endif // MAINMACHINE_H
