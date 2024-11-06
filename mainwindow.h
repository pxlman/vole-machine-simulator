#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_loadFileBtn_clicked();

    void on_addInstructionBtn_clicked();

    void on_clearMemoryBtn_clicked();

    void on_singleStepBtn_clicked();

    void on_speedSlider_sliderMoved(int position);

    void on_clearAllBtn_clicked();

    void on_running();

    void on_haltBtn_clicked();

    void on_restartBtn_clicked();

    void on_stepBackBtn_clicked();

    void on_helpBtn_clicked();

    void on_instructionText_textChanged(const QString &arg1);

private:
    void on_program_counter_change();
    void choose_bmo_face(std::string emotion);
    void guideMe();
    void pause();
    void resume();
    Ui::MainWindow *ui;
    QTimer *timer;
};
#endif // MAINWINDOW_H
