cmake_minimum_required(VERSION 3.29)
project(vole_machine_simulator)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)
include_directories(build)
include_directories(build/Desktop-Debug)
include_directories(machine)

add_executable(vole_machine_simulator
    build/Desktop-Debug/moc_predefs.h
    machine/ALU.h
    machine/CPU.h
    machine/CU.h
    machine/Machine.h
    machine/Memory.h
    machine/Register.h
    main.cpp
    MainMachine.h
    mainwindow.cpp
    mainwindow.h
    receiver.cpp
    receiver.h
    UiFunctions.h)
