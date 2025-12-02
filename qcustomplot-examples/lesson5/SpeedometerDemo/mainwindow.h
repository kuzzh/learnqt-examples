#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "qcustomplot.h"

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
    void onSpeedChanged(int value); // 接收滑块信号

private:
    Ui::MainWindow *ui;
    QCPItemLine *mArrow; // 哪怕是根线，也是我们心中的“指针”
    void setupSpeedometer();
};
#endif // MAINWINDOW_H
