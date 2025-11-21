#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "qcppiechart.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QCPPieChart *pie = new QCPPieChart(this);
    pie->setPieRadius(100);
    pie->addData("各种税费", 30, QColor("#ff6b6b"));
    pie->addData("研发投入", 25, QColor("#feca57"));
    pie->addData("市场营销", 20, QColor("#48dbfb"));
    pie->addData("人力成本", 25, QColor("#1dd1a1"));
    pie->repaintPie();

    // 加到布局里
    ui->verticalLayout->addWidget(pie);
}

MainWindow::~MainWindow()
{
    delete ui;
}
