#include "mainwindow.h"
#include <random>
#include <ctime>
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QStandardPaths>
#include "qthread.h"
#include "qtimer.h"
#include <iostream>
#include <vector>
#include "MainMachine.h"
#include "machine/Machine.h"
#include "UiFunctions.h"
#include <QTimer>
#include <QTime>
#include <QMessageBox>
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
    uif.setTables(ui->registerTable);
    uif.setTable2(ui->memoryTable_2);
    uif.setErrorLabel(ui->errorLabel);
    uif.setMachine(&machine);
    uif.setOutput(ui->outputField);
    uif.setPCLabel(ui->PCLabel);
    uif.setIRLabel(ui->IRLabel);
    uif.setSpeedLabel(ui->speedLabel);
    uif.setSpeedSlider(ui->speedSlider);
    uif.setUndoBtn(ui->stepBackBtn);
    uif.refreshTables();
    ui->errorLabel->setText("");
    uif.fetchOutput();
    // Timer for running speed
    connect(timer, &QTimer::timeout, this, &MainWindow::on_singleStepBtn_clicked);
    connect(ui->runBtn, &QPushButton::clicked, this, &MainWindow::on_running);
    connect(ui->instructionText, &QLineEdit::returnPressed, this, &MainWindow::on_addInstructionBtn_clicked);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete timer;
}

void MainWindow::choose_bmo_face(string emotion){
    vector<QString> images;
    if(emotion == "happy"){
        images = {
                 ":/styles/images/face_comfy",
                 ":/styles/images/face_impressed",
        };
    } else if (emotion == "smile"){
        images={
                 ":/styles/images/face_happy1",
                 ":/styles/images/face_happy2",
        };
    } else if(emotion == "sad"){
        images={
                 ":/styles/images/face_cry",
                 ":/styles/images/face_littlesad",
                 ":/styles/images/face_sad",
                 ":/styles/images/face_sad2",
        };
    } else if (emotion == "mad"){
        images={
                 ":/styles/images/face_angry",
                 ":/styles/images/face_angry2",
                 ":/styles/images/face_disrespect",
                 ":/styles/images/face_mad1",
                 ":/styles/images/face_mad2",
        };
    }
    int id = QTime::currentTime().msecsSinceStartOfDay() % images.size();
    QPixmap pxm(images[id]+".png");
    ui->bmo_face->setPixmap(pxm);
}

void MainWindow::guideMe(){
    ui->guidanceBox->setText(QString::fromStdString(machine.getGuide(machine.getPC())));
}

void MainWindow::on_singleStepBtn_clicked()
{
    if(machine.getIR()[0] == '0'){
        pause();
        guideMe();
        return;
        // choose_bmo_face("sad");
        // uif.refreshTables();
    }
    if(!machine.isHalted() && machine.getPC() < 255){
        machine.cycle();
        guideMe();
        choose_bmo_face("happy");
    } else {
        timer->stop();
        on_haltBtn_clicked();
        choose_bmo_face("sad");
    }
    uif.refreshTables();
}

void MainWindow::on_program_counter_change(){
    if(machine.getPC()>= 14){
        ui->memoryTable_2->scrollToItem(ui->memoryTable_2->item(machine.getPC()/2 - 1,0));
    } else {
        ui->memoryTable_2->scrollToItem(ui->memoryTable_2->item(0,0));
    }
}

void MainWindow::on_restartBtn_clicked()
{
    machine.restart();
    guideMe();
    // ui->runBtn->setDisabled(true);
    // ui->singleStepBtn->setDisabled(true);
    choose_bmo_face("happy");
    uif.refreshTables();
}

void MainWindow::pause(){
    // Making the run pause
    ui->runBtn->setText("");
    timer->stop();
    ui->speedSlider->setEnabled(true);
    ui->addInstructionBtn->setEnabled(true);
    ui->loadFileBtn->setEnabled(true);
    ui->clearAllBtn->setEnabled(true);
    ui->clearMemoryBtn->setEnabled(true);
    ui->singleStepBtn->setEnabled(true);
    choose_bmo_face("smile");
    uif.refreshTables();
}

void MainWindow::resume(){
    timer->setInterval((7 - ui->speedSlider->value()) * 150 - 140);
    timer->start();
    ui->speedSlider->setDisabled(true);
    ui->addInstructionBtn->setDisabled(true);
    ui->loadFileBtn->setDisabled(true);
    ui->clearAllBtn->setDisabled(true);
    ui->clearMemoryBtn->setDisabled(true);
    ui->singleStepBtn->setDisabled(true);
    ui->runBtn->setText("");
    choose_bmo_face("smile");
    uif.refreshTables();
}

void MainWindow::on_running(){
    static bool running = false;
    if(!running){
        on_singleStepBtn_clicked();
        resume();
        running = true;
    } else {
        pause();
        running = false;
    }
}


void MainWindow::on_loadFileBtn_clicked()
{
    std::string filePath;
    QString homeDir = QStandardPaths::writableLocation(QStandardPaths::HomeLocation);
    QString path = QFileDialog::getOpenFileName(this, tr("Open File"), homeDir, tr(".txt (*.txt);;All Files (*)"));
    if(path.isEmpty()) {
        choose_bmo_face("mad");
        uif.refreshTables();
    } else {
        try{
            filePath = path.toStdString();
            machine.addFile(filePath);
            choose_bmo_face("smile");
            uif.notError();
            uif.refreshTables();
            guideMe();
        } catch (...){
            machine.clearMemory();
            choose_bmo_face("mad");
            uif.handleError();
            uif.refreshTables();
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
        choose_bmo_face("happy");
        uif.refreshTables(); // refresh the tables
        uif.notError(); // in case a weird instruction
        guideMe();
    } catch (...) {
        choose_bmo_face("mad");
        uif.handleError();
    }
}

void MainWindow::on_clearMemoryBtn_clicked()
{
    machine.clearMemory();
    choose_bmo_face("sad");
    ui->guidanceBox->setText("");
    uif.refreshTables();
}



void MainWindow::on_speedSlider_sliderMoved(int position)
{
    choose_bmo_face("smile");
    ui->speedLabel->setText(QString::fromStdString(to_string(position/3.0).substr(0,3)));
    uif.refreshTables();
}

void MainWindow::on_clearAllBtn_clicked()
{
    machine.clearAll();
    ui->runBtn->setEnabled(true);
    ui->singleStepBtn->setEnabled(true);
    choose_bmo_face("sad");
    ui->guidanceBox->setText("");
    uif.refreshTables();
    // on_program_counter_change();
}


void MainWindow::on_haltBtn_clicked()
{
    pause();
    machine.halt();
    ui->runBtn->setDisabled(true);
    ui->singleStepBtn->setDisabled(true);
    choose_bmo_face("mad");
    uif.refreshTables();
}

void MainWindow::on_stepBackBtn_clicked()
{
    choose_bmo_face("smile");
    if(machine.getPC() >= 4){
        machine.undo();
        guideMe();
        uif.refreshTables();
    } else {
        ui->stepBackBtn->setDisabled(true);
        uif.refreshTables();
    }
}


void MainWindow::on_helpBtn_clicked()
{
    QMessageBox::information(this, "Hint", "You can hover on any item in the application to see a tool tip.\nHave fun using the software ");
    choose_bmo_face("smile");
    uif.refreshTables();
}


void MainWindow::on_instructionText_textChanged(const QString &arg1)
{
    QString arg = arg1;
    bool isHex;
    int jlkj = arg.toInt(&isHex, 16);
    if(!isHex) arg = "";
    ui->instructionText->setText(arg.toUpper());
}

