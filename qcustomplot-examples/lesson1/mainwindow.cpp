// mainwindow.cpp
#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("Simple Curve"); // 设置窗口标题
    setupPlot(); // 在构造函数中调用我们的绘图函数
}

void MainWindow::setupPlot()
{
    // --- 1. 准备数据 ---
    // 使用QVector来存储x和y坐标
    QVector<double> x(101), y(101); // 总共101个数据点
    for (int i=0; i<101; ++i)
    {
        x[i] = i/50.0 * 2 * M_PI; // x 坐标范围 0 to 2*PI
        y[i] = qSin(x[i]);        // y 坐标为对应的sin值
    }

    // --- 2. 创建一个图表 (QCPGraph) 并将数据传入 ---
    // ui->customPlot 就是我们在UI设计器里提升的那个控件
    ui->customPlot->addGraph();
    ui->customPlot->graph(0)->setData(x, y);

    // --- 3. 设置坐标轴的标签和范围 ---
    ui->customPlot->xAxis->setLabel("x");
    ui->customPlot->yAxis->setLabel("y");
    // 让x轴和y轴的范围能自动包含所有数据
    ui->customPlot->graph(0)->rescaleAxes();

    // --- 4. 重绘图表 ---
    ui->customPlot->replot();
}

MainWindow::~MainWindow()
{
    delete ui;
}
