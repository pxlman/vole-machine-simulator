#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QStandardPaths>
#include"qthread.h"
#include"qtimer.h"
#include <iostream>
#include<vector>
#include"MainMachine.h"
#include"machine/Machine.h"
#include"UiFunctions.h"
#include <QTimer>
using namespace std;
UiF uif = UiF(); // My UI functions
Machine machine = Machine();

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , timer(new QTimer(this))
{
    ui->setupUi(this); // Default, don't touch it

    // Setting tables to UI functions
    uif.setTables(ui->registerTable, ui->memoryTable);
    uif.setErrorLabel(ui->errorLabel);
    uif.setMachine(&machine);
    uif.setOutput(ui->outputField);
    uif.setPCLabel(ui->PCLabel);
    uif.setIRLabel(ui->IRLabel);
    uif.setSpeedLabel(ui->speedLabel);
    uif.setSpeedSlider(ui->speedSlider);
    uif.refreshTables();

    // Timer for running speed
    connect(timer, &QTimer::timeout, this, &MainWindow::on_singleStepBtn_clicked);
    connect(ui->runBtn, &QPushButton::clicked, this, &MainWindow::on_running);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete timer;
}

void MainWindow::on_singleStepBtn_clicked()
{
    if(!machine.isHalted() && machine.getPC() < 255){
        machine.cycle();
        uif.refreshTables();
    } else {
        timer->stop();
        ui->speedSlider->setEnabled(true);
    }
}

void MainWindow::on_restartBtn_clicked()
{
    machine.restart();
    uif.refreshTables();
}

void MainWindow::on_running(){
    timer->setInterval((4 - ui->speedSlider->value()) * 200);
    timer->start();
    ui->speedSlider->setDisabled(true);
}


void MainWindow::on_loadFileBtn_clicked()
{
    std::string filePath;
    QString homeDir = QStandardPaths::writableLocation(QStandardPaths::HomeLocation);
    QString path = QFileDialog::getOpenFileName(this, tr("Open File"), homeDir, tr(".txt (*.txt);;All Files (*)"));
    if(path.isEmpty()) {
        std::cout << "Cencelled\n";
    } else {
        try{
            filePath = path.toStdString();
            machine.addFile(filePath);
            uif.refreshTables();
        } catch (...){
            machine.clearMemory();
            uif.handleError();
        }
    }
}

// Adding single instruction
void MainWindow::on_addInstructionBtn_clicked()
{
    string instruction = ui->instructionText->text().toUpper().toStdString(); // get instruction from lineedit
    ui->instructionText->setText("");
    ui->instructionText->setFocus();
    try{
        machine.addInstruction(instruction); // Add inst to memory
        uif.refreshTables(); // refresh the tables
        uif.notError(); // in case a weird instruction
    } catch (...) {
        uif.handleError();
    }
}

void MainWindow::on_clearMemoryBtn_clicked()
{
    machine.clearMemory();
    uif.refreshTables();
}



void MainWindow::on_speedSlider_sliderMoved(int position)
{
    ui->speedLabel->setText(QString::fromStdString(to_string(position/2.0).substr(0,3)));
}

void MainWindow::on_clearAllBtn_clicked()
{
    machine.clearAll();
    uif.refreshTables();
}


void MainWindow::on_haltBtn_clicked()
{
    timer->stop();
    ui->speedSlider->setEnabled(true);
}



