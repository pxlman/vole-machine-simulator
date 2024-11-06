#ifndef UIF_H
#define UIF_H

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include "machine/Machine.h"
#include "machine/ALU.h"
class UiF {
private:
    QTableWidget *memory_table;
    QTableWidget *memory_table2;
    QTableWidget *reg_table;
    Machine *mm;
    QLabel *errLabel;
    QLineEdit *output;
    QSlider *speedSlider;
    QLabel *speedLabel;
    QLineEdit *PCLabel;
    QLineEdit *IRLabel;
    QPushButton *undoBtn;
public:
    UiF(){
    }
    void setTables(QTableWidget *r_table, QTableWidget *m_table){
        memory_table = m_table;
        reg_table = r_table;
    }
    void setTable2(QTableWidget *m_table){
        memory_table2 = m_table;
    }
    void setMachine(Machine *m){
        mm = m;
    }
    void setUndoBtn(QPushButton *b){
        undoBtn = b;
    }
    void setErrorLabel(QLabel *el){
        errLabel = el;
    }
    void setOutput(QLineEdit *le){
        output = le;
    }
    void setPCLabel(QLineEdit *le){
        PCLabel = le;
    }
    void setSpeedLabel(QLabel *sl){
        speedLabel = sl;
    }
    void setSpeedSlider(QSlider *sl){
        speedSlider = sl;
    }
    void setIRLabel(QLineEdit *le){
        IRLabel = le;
    }
    void handleError(){
        errLabel->setText("Be wise please!");
    }
    void notError(){
        errLabel->setText("");
    }
    void fetchingRegisterTable(vector<string> reg){
        auto table = reg_table;
        QString hex = "0123456789ABCDEF";
        for (int i = 0; i < 16; i++) {
            QString form = QString::fromStdString("R") + hex[i];
            auto *item = new QTableWidgetItem(form);
            item -> setTextAlignment(Qt::AlignCenter);
            table->setItem(i, 0, item);
        }
        for (int i = 0; i < 16; i++) {
            QString form = QString::fromStdString(reg[i]);
            auto *item = new QTableWidgetItem(form);
            item -> setTextAlignment(Qt::AlignCenter);
            table->setItem(i, 1, item);
        }
    }
    void fetchingMemoryTable(vector<string> memory){
        QTableWidget *table = memory_table;
        QString hex = "0123456789ABCDEF";
        // Columns
        for (int i = 1; i <= 16; i++) {
            QString form = QString(hex[i-1]);
            auto *item = new QTableWidgetItem(form);
            item -> setTextAlignment(Qt::AlignBottom + Qt::AlignRight);
            table->setItem(0, i, item);
        }
        // Rows
        for (int i = 1; i <= 16; i++) {
            QString form = QString(hex[i-1]);
            auto *item = new QTableWidgetItem(form);
            item -> setTextAlignment(Qt::AlignRight + Qt::AlignCenter);
            table->setItem(i, 0, item);
        }
        // Data from vector
        for (int i = 1; i <= 16; i++) { // the table is 1 based index
            for (int j = 1; j <= 16; j++) {
                int index = (i-1) * 16 + (j-1); // the vector is 0 based
                QString cell = QString::fromStdString(memory[index]);
                auto *item = new QTableWidgetItem(cell);
                item -> setTextAlignment(Qt::AlignCenter);
                if(index == mm->getPC()){
                    item->setBackground(QBrush("#d7827e"));
                    item->setForeground(QBrush("#21202e"));
                }
                if(i == 1 && (j == 1 || j == 2)){
                    item->setBackground(QBrush("#e0def4"));
                    item->setForeground(QBrush("#21202e"));
                }
                table->setItem(j, i, item);
            }
        }
    }
    void fetchingMemoryTableV2(vector<string> memory){
        QTableWidget *table = memory_table;
        QString hex = "0123456789ABCDEF";
        // Columns header
        for (int i = 1; i <= 16; i++) {
            QString form = QString(hex[i-1]);
            auto *item = new QTableWidgetItem(form);
            item -> setTextAlignment(Qt::AlignBottom + Qt::AlignRight);
            table->setItem(0, i, item);
        }
        // Rows header
        for (int i = 1; i <= 16; i+=2) {
            int c = (i-1)/2;
            QString form = QString(hex[i-1]) + "|" + QString(hex[i]);
            auto *item = new QTableWidgetItem(form);
            item -> setTextAlignment(Qt::AlignRight + Qt::AlignCenter);
            table->setItem(c+1, 0, item);
        }
        // Data from vector
        for (int i = 1; i <= 16; i++) { // the table is 1 based index
            for (int j = 1; j <= 16; j+=2) {
                int index = (i-1) * 16 + j - 1; // the vector is 0 based
                QString cell = QString::fromStdString(memory[index]) + "|" +
                               QString::fromStdString(memory[index+1]);
                auto *item = new QTableWidgetItem(cell);
                item -> setTextAlignment(Qt::AlignCenter);
                // Program counter
                if(index == mm->getPC()){
                    item->setBackground(QBrush("#c4a7e7"));
                    item->setForeground(QBrush("#21202e"));
                }
                if(index >= 254 && mm->getPC() >= 254){
                    item->setBackground(QBrush("#191724"));
                    item->setForeground(QBrush("#faf4ed"));
                }
                // Output cells
                if(i == 1 && (j == 1)){
                    item->setBackground(QBrush("#6e6a86"));
                    item->setForeground(QBrush("#faf4ed"));
                }
                table->setItem((j-1)/2 + 1, i, item);
            }
        }
    }
    void fetchingMemoryTableV3(vector<string> memory){
        QTableWidget *table = memory_table2;
        ALU alu = ALU();
        for (int i = 0; i < 256; i+=2) { // the table is 1 based index
            QString cell1 = QString::fromStdString(alu.decToHex(i));
            QString cell2 = QString::fromStdString(memory[i]);
            QString cell3 = QString::fromStdString(memory[i+1]);
            QString cell4 = QString::fromStdString(alu.decToHex(i+1));
            // QString cell5 = QString::fromStdString(mm->getGuide(i));
            auto *item1 = new QTableWidgetItem(cell1);
            auto *item2 = new QTableWidgetItem(cell2);
            auto *item3 = new QTableWidgetItem(cell3);
            auto *item4 = new QTableWidgetItem(cell4);
            // auto *item5 = new QTableWidgetItem(cell5);
            item1 -> setTextAlignment(Qt::AlignCenter);
            item2 -> setTextAlignment(Qt::AlignCenter);
            item3 -> setTextAlignment(Qt::AlignCenter);
            item4 -> setTextAlignment(Qt::AlignCenter);
            // Program counter
            if(i == mm->getPC()){
                item2->setBackground(QBrush("#c4a7e7"));
                item3->setBackground(QBrush("#c4a7e7"));
                item2->setForeground(QBrush("#21202e"));
                item3->setForeground(QBrush("#21202e"));
                item2->setToolTip("Cells of the current instruction");
                item3->setToolTip("Cells of the current instruction");
            }
            if(i >= 254 && mm->getPC() >= 254){
                item2->setBackground(QBrush("#191724"));
                item3->setBackground(QBrush("#191724"));
                item2->setForeground(QBrush("#faf4ed"));
                item3->setForeground(QBrush("#faf4ed"));
            }
            // Output cells
            if(i == 0){
                item2->setBackground(QBrush("#6e6a86"));
                item3->setBackground(QBrush("#6e6a86"));
                item2->setForeground(QBrush("#faf4ed"));
                item3->setForeground(QBrush("#faf4ed"));
                item2->setToolTip("Reserved for the screen");
                item3->setToolTip("Reserved for the screen");
            }
            table->setItem(i/2, 0, item1);
            table->setItem(i/2, 1, item2);
            table->setItem(i/2, 2, item3);
            table->setItem(i/2, 3, item4);
            // table->setItem(i/2, 4, item5);
            memory_table2->resizeColumnsToContents();
        }
    }
    void fetchingPCIR(){
        Machine machine = *mm;
        int pc = machine.getPC();
        PCLabel->setText(QString::fromStdString(to_string(pc)));
        string ir = machine.getIR();
        IRLabel->setText(QString::fromStdString(ir));
    }
    void refreshTables(){
        Machine machine = *mm;
        // Memory
        vector<string> memory = machine.getMemoryVector();
        // fetchingMemoryTableV2(memory);
        fetchingMemoryTableV3(memory);
        // Registers
        vector<string> reg = machine.getRegisterVector();
        fetchingRegisterTable(reg);
        // PC and IR
        fetchingPCIR();
        // output
        ALU alu;
        QString m0 = QString::fromStdString(""+(char)alu.hextodec(memory[0]));
        QString m1 = QString::fromStdString(""+(char)alu.hextodec(memory[1]));
        output->setText(m0+" "+m1);
        if(machine.getPC() <= 14){
            memory_table2->scrollToItem(memory_table2->item(0 ,0));
        } else {
            memory_table2->scrollToItem(memory_table2->item(machine.getPC()/2, 0));
        }
        if(mm->getPC() <= 2) undoBtn->setDisabled(true);
        else undoBtn->setEnabled(true);
    }
};

#endif // UIF_H
