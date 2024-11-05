#ifndef RECEIVER_H
#define RECEIVER_H

#include <QMainWindow>
#include <QtWidgets/QMainWindow>

class Receiver : public QMainWindow
{
    Q_OBJECT
public:
    explicit Receiver(QWidget *parent = nullptr);

public slots:
    void onRun();
signals:

};

#endif // RECEIVER_H
